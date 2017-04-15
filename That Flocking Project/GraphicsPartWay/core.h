#pragma once
#include "nGL.h"
#include <string>
#include <vector>
#include <limits>

namespace nGraphics
{
	inline void Set(glm::vec3& v, float x, float y, float z)
	{
		v.x = x; v.y = y; v.z = z;
	}
	inline void Set(glm::vec4& v, float x, float y, float z, float w)
	{
		v.x = x; v.y = y; v.z = z; v.w = w;
	}
	inline void SetMax(glm::vec3& v, float x, float y, float z)
	{
		if (x > v.x) v.x = x;
		if (y > v.y) v.y = y;
		if (z > v.z) v.z = z;
	}
	inline void SetMin(glm::vec3& v, float x, float y, float z)
	{
		if (x < v.x) v.x = x;
		if (y < v.y) v.y = y;
		if (z < v.z) v.z = z;
	}
	inline void SetMax(glm::vec3& v)
	{
		v.x = std::numeric_limits<float>::max();
		v.y = std::numeric_limits<float>::max();
		v.z = std::numeric_limits<float>::max();
	}
	inline void SetMin(glm::vec3& v)
	{
		v.x = std::numeric_limits<float>::min();
		v.y = std::numeric_limits<float>::min();
		v.z = std::numeric_limits<float>::min();
	}
	/**
	describes in individual attribute
	*/
	class cVertexAttributeDesc
	{
	public:
		// default setup is for a vec4 (float, float, float, float)
		cVertexAttributeDesc() : ByteSize(4 * sizeof(float)), NumVariables(4), Type(0x1406), Normalize(false) {}
		cVertexAttributeDesc(int byteSize, int numVariables, unsigned int type, bool normalize)
			: ByteSize(byteSize), NumVariables(numVariables), Type(type), Normalize(normalize) {}
		~cVertexAttributeDesc() {}

		int ByteSize;		// default 4*sizeof(float) for vec4
		int NumVariables;	// default 4 for a vec4
		unsigned int Type;  // default 0x1406 (GL_FLOAT) for vec4
		bool Normalize;		// normalize the values
	};
	/**
	helper struct to contain buffer ids
	*/
	struct sBufferIds
	{
		sBufferIds() : VertexId(0), IndexId(0), BufferIds(0), NumBufferIds(0) {}

		void Clear()
		{
			VertexId = 0;
			IndexId = 0;
			if (BufferIds) delete BufferIds;
			BufferIds = 0;
			NumBufferIds = 0;
		}

		void Destroy()
		{
			glDeleteBuffers(1, &VertexId);
			glDeleteBuffers(1, &IndexId);
			if (NumBufferIds > 0)
			{
				glDeleteBuffers(NumBufferIds, BufferIds);
				delete BufferIds;
			}
			VertexId = 0;
			IndexId = 0;
			BufferIds = 0;
		}

		GLuint VertexId;
		GLuint IndexId;
		GLuint* BufferIds;
		int NumBufferIds;
	};

	template<class VerType>
	class cVertexDescT : public std::vector<cVertexAttributeDesc>
	{
	public:
		cVertexAttributeDesc Vertex;

		cVertexDescT() : mVertexByteSize(sizeof(VerType)), Vertex() {};

		inline int GetVertexByteSize() const { return mVertexByteSize; }

		bool GenerateBuffers(sBufferIds& ids, std::vector<VerType>& vertices, std::vector<unsigned int>& indices, GLenum usage)
		{
			glGenVertexArrays(1, &ids.VertexId);
			glBindVertexArray(ids.VertexId);
			if (nGL::IsThereAnError("cVertexAttributeDesc::GenerateBuffers"))
				return false;

			ids.NumBufferIds = size();
			if (ids.NumBufferIds > 0)
			{
				ids.BufferIds = new GLuint[ids.NumBufferIds];

				glGenBuffers(ids.NumBufferIds, ids.BufferIds);
				for (int idxId = 0; idxId < ids.NumBufferIds; idxId++)
				{
					glBindBuffer(GL_ARRAY_BUFFER, ids.BufferIds[idxId]);
					if (nGL::IsThereAnError("cVertexAttributeDesc::GenerateBuffers"))
						return false;
				}
			}

			int bytesInVertexArray = vertices.size() * mVertexByteSize;
			glBufferData(GL_ARRAY_BUFFER, bytesInVertexArray, &vertices[0], usage);
			if (nGL::IsThereAnError("cVertexAttributeDesc::GenerateBuffers"))
				return false;

			glGenBuffers(1, &ids.IndexId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids.IndexId);
			if (nGL::IsThereAnError("cVertexAttributeDesc::GenerateBuffers"))
				return false;

			int bytesInIndexArray = indices.size() * sizeof(GLuint);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesInIndexArray, &indices[0], GL_STATIC_DRAW);
			if (nGL::IsThereAnError("cVertexAttributeDesc::GenerateBuffers"))
				return false;

			return true;
		}

		bool BindBuffers(sBufferIds& ids)
		{
			glBindVertexArray(ids.VertexId);
			for (int idx = 0; idx < ids.NumBufferIds; idx++)
				glBindBuffer(GL_ARRAY_BUFFER, ids.BufferIds[idx]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids.IndexId);
			glEnableVertexAttribArray(0);
			for (int idx = 0; idx < ids.NumBufferIds; idx++)
				glEnableVertexAttribArray(idx + 1);
			if (nGL::IsThereAnError("cVertexAttributeDesc::BindBuffers"))
				return false;

			glVertexAttribPointer(0,		// index or "slot" in the shader
				Vertex.NumVariables,		// Number of variables: vec4 would be 4 32-bit variables
				Vertex.Type,				// Type: vec4 is float
				Vertex.Normalize,			// "normalize" the values (or not)
				mVertexByteSize,			// Number of bytes per vertex (the "stride")
				(GLvoid*)0);				// Offset from vertex (position is first, so offset = 0)
			if (nGL::IsThereAnError("cVertexAttributeDesc::BindBuffers"))
				return false;

			int offset = Vertex.ByteSize;

			for (int idx = 0; idx < ids.NumBufferIds; idx++)
			{
				cVertexAttributeDesc& attrib = at(idx);
				glVertexAttribPointer(idx + 1,
					attrib.NumVariables,
					attrib.Type,
					attrib.Normalize,
					mVertexByteSize,
					(GLvoid*)offset);
				offset += attrib.ByteSize;
				if (nGL::IsThereAnError("cVertexAttributeDesc::BindBuffers"))
					return false;
			}
			return true;
		}

		bool DisableBuffers(sBufferIds& ids)
		{
			glDisableVertexAttribArray(0);
			for (int idx = 0; idx < ids.NumBufferIds; idx++)
				glDisableVertexAttribArray(idx + 1);
			glBindVertexArray(0);
			if (nGL::IsThereAnError("cVertexAttributeDesc::DisableBuffers"))
				return false;
			return true;
		}
	private:
		int mVertexByteSize;
	};

	class iMesh;
	class iBuffer
	{
	public:
		virtual ~iBuffer() {}
		virtual bool Bind() = 0;
		virtual bool Disable() = 0;
		// render the whole buffer
		virtual bool Render() = 0;
		// render mesh id
		virtual bool Render(int meshId) = 0;
		// render an individual mesh
		virtual bool Render(int baseVertex, unsigned int indexStart, unsigned int indexEnd, int numIndices) = 0;

		virtual unsigned int NumMeshes() = 0;
		virtual iMesh* GetMesh(unsigned int index) = 0;
	};

	extern iBuffer* gBufferInUse;
	void SetActiveBuffer(iBuffer* buffer);

	class iMesh
	{
	public:
		bool ClearVerticesOnLoad;
		bool ClearIndicesOnLoad;

		iMesh() : ClearVerticesOnLoad(true), ClearIndicesOnLoad(true) {};

		virtual ~iMesh() {}
		virtual unsigned int NumVertices() = 0;
		virtual unsigned int NumIndices() = 0;
		virtual void GetVertexPosition(unsigned int idx, glm::vec3& positionOut) = 0;
		virtual void ClearVertices() = 0;
		virtual void ClearIndices() = 0;
		virtual bool Render() = 0;
		virtual void RecalculateMaxExtents() = 0;
		virtual void Rescale(float appliedScale, bool recalculateMaxExtents) = 0;
		virtual glm::vec4 CenterVertexPositions(bool recalculateMaxExtents) = 0;
		inline const std::string& GetName() { return mName; }
		inline void SetName(const std::string& name) { mName = name; }
		virtual iBuffer* GetBuffer() = 0;

		inline const glm::vec3& GetMaxExtents() { return mMaxExtents; }
		inline const glm::vec3& GetMinExtents() { return mMinExtents; }
		inline void SetMinMaxExtents(const glm::vec3& minExtents, const glm::vec3& maxExtents)
		{
			mMinExtents = minExtents;
			mMaxExtents = maxExtents;
		}
	protected:
		glm::vec3 mMaxExtents; // to be set by inheriting classes
		glm::vec3 mMinExtents;
	private:
		std::string mName;
	};

	template<class VerType>
	class cBufferT;
	template<class VerType>
	class cMeshT : public iMesh
	{
	public:
		cMeshT() : iMesh(), mBuffer(0), mId(-1), mBaseVertex(0), mNumIndices(0), mIndexStart(0), mIndexEnd(0) {}
		virtual ~cMeshT()
		{
			mBuffer = 0;
		}

		inline int GetId() const { return mId; }
		inline int GetBaseVertex() const { return mBaseVertex; }
		inline int GetNumIndices() const { return mNumIndices; }
		inline unsigned int GetIndexStart() const { return mIndexStart; }
		inline unsigned int GetIndexEnd() const { return mIndexEnd; }

		inline bool IsLoaded() { return mBuffer != 0; }

		inline void ReserveVertices(unsigned int count) { mVertices.resize(count); }
		inline void ReserveIndices(unsigned int count) { mIndices.resize(count); }
		inline void SetVertex(const VerType& vertex, const unsigned int& index) { mVertices[index] = vertex; }
		inline void SetIndex(const unsigned int& value, const unsigned int& index) { mIndices[index] = value; }

		inline const std::string& GetName() { return mName; }
		inline void SetName(const char* name) { mName = name; }

		virtual void ClearVertices() { mVertices.clear(); }
		virtual void ClearIndices() { mIndices.clear(); }

		virtual unsigned int NumVertices() { return mVertices.size(); }
		virtual unsigned int NumIndices() { return mIndices.size(); }

		virtual bool Render()
		{
			if (mBuffer)
			{
				return mBuffer->Render(mBaseVertex, mNumIndices, mIndexEnd, mNumIndices);
			}
			else
			{
				return false;
			}
		}
		virtual iBuffer* GetBuffer() { return mBuffer; }
	protected:
		// cleared by cBufferT after loaded by default
		std::vector<VerType> mVertices;
		std::vector<unsigned int> mIndices;
	private:

		friend class cBufferT<VerType>;
		// set by cBufferT when loaded
		void BufferSet(iBuffer* buffer, int id, int baseVertex, unsigned int indexStart, unsigned int indexEnd, int numIndices)
		{
			mBuffer = buffer;
			mId = id;
			mBaseVertex = baseVertex;
			mIndexStart = indexStart;
			mIndexEnd = indexEnd;
			mNumIndices = numIndices;
		}
		std::string mName;
		iBuffer* mBuffer;
		int mId;  // index id in the buffer
		int mBaseVertex;
		unsigned int mIndexStart;
		unsigned int mIndexEnd;
		int mNumIndices;

	};

	template<class VerType>
	class cBufferT : public iBuffer
	{
	public:
		cBufferT() : mLoaded(false), mBufferIds() {}
		virtual ~cBufferT()
		{
			Unload();
		}

		bool Unload()
		{
			if (!mLoaded) return false;
			mBufferIds.Destroy();
			unsigned int numMeshes = mMeshes.size();
			for (unsigned int idxMesh = 0; idxMesh < numMeshes; idxMesh++)
			{
				cMeshT<VerType>* mesh = mMeshes[idxMesh];
				mesh->BufferSet(0, -1, 0, 0, 0, 0);
				delete mesh;
			}
			mMeshes.clear();
			mLoaded = false;
			return true;
		}

		bool Load(const std::vector<cMeshT<VerType>*>& meshes)
		{
			if (mLoaded) return false;
			unsigned int totalVertices = 0;
			unsigned int totalIndices = 0;
			unsigned int numMeshes = meshes.size();
			for (unsigned int idxMesh = 0; idxMesh < numMeshes; idxMesh++)
			{
				cMeshT<VerType>* mesh = meshes[idxMesh];
				if (mesh->IsLoaded()) return false;
				totalVertices += mesh->mVertices.size();
				totalIndices += mesh->mIndices.size();
			}

			std::vector<VerType> vertices;
			vertices.resize(totalVertices);
			std::vector<unsigned int> indices;
			indices.resize(totalIndices);

			mNumIndices = totalIndices;
			mNumVertices = totalVertices;

			unsigned int baseVertex = 0;
			unsigned int baseIndex = 0;
			for (unsigned int idxMesh = 0; idxMesh < numMeshes; idxMesh++)
			{
				cMeshT<VerType>* mesh = meshes[idxMesh];

				// copy in vertices
				unsigned int numVertices = mesh->mVertices.size();
				unsigned int termVert = baseVertex + numVertices;
				unsigned int tmpBaseVertex = baseVertex;
				for (unsigned int idxVert = 0; idxVert < numVertices; idxVert++)
				{
					vertices[baseVertex++] = mesh->mVertices[idxVert];
				}
				if (baseVertex != termVert)
				{
					bool sexybreakpoint = true;
				}
				// copy in indices
				unsigned int numIndices = mesh->mIndices.size();
				unsigned int termInd = baseIndex + numIndices;
				unsigned int tmpBaseIndex = baseIndex;
				for (unsigned int idxIndex = 0; idxIndex < numIndices; idxIndex++)
				{
					indices[baseIndex++] = mesh->mIndices[idxIndex];
				}
				if (baseIndex != termInd)
				{
					bool sexybreakpoint = true;
				}
				/// ??? SHOULD index end be baseIndex - 1?
				mesh->BufferSet(this, idxMesh, tmpBaseVertex, tmpBaseIndex, baseIndex, numIndices);
				if (mesh->ClearVerticesOnLoad) mesh->mVertices.clear();
				if (mesh->ClearIndicesOnLoad) mesh->mIndices.clear();
			}
			mMeshes.assign(meshes.begin(), meshes.end()); // copy the meshes in

			if (!mDesc.GenerateBuffers(mBufferIds, vertices, indices, GL_STATIC_DRAW))
			{
				return false;
			}
			mLoaded = true;
			return true;
		}
		// render everything
		virtual bool Render()
		{
			return Render(0, 0, mNumIndices, mNumIndices);
		}
		// render mesh id
		virtual bool Render(int meshId)
		{
			cMeshT<VerType>* mesh = mMeshes[meshId];
			return mesh->Render();
		}
		// render a subsection - the meshes know what to do
		virtual bool Render(int baseVertex, unsigned int indexStart, unsigned int indexEnd, int numIndices)
		{
			if (!mLoaded) return false;
			SetActiveBuffer(this);
			glDrawRangeElementsBaseVertex(GL_TRIANGLES, indexStart, indexEnd, numIndices, GL_UNSIGNED_INT, 0, baseVertex);
			if (nGL::IsThereAnError("cBufferT::Render()"))
			{
				return false;
			}
			return true;
		}

		virtual bool Bind()
		{
			return mDesc.BindBuffers(mBufferIds);
		}

		virtual bool Disable()
		{
			return mDesc.DisableBuffers(mBufferIds);
		}

		unsigned int NumMeshes()
		{
			return mMeshes.size();
		}

		virtual iMesh* GetMesh(unsigned int index)
		{
			return mMeshes[index];
		}
	protected:
		// to be called by inheriting classes
		cVertexDescT<VerType> mDesc;
	private:
		bool mLoaded;

		std::vector<cMeshT<VerType>*> mMeshes;
		sBufferIds mBufferIds;
		int mNumVertices;
		int mNumIndices;

		cBufferT(const cBufferT& other) {}
		cBufferT& operator=(const cBufferT& other) { return *this; }
	};

	class cVert_PNT
	{
	public:
		glm::vec4 Position;
		glm::vec4 Normal;
		glm::vec4 TexCoord;
	};

	void CreateDesc(cVertexDescT<cVert_PNT>& desc);

	class cMesh_PNT : public cMeshT<cVert_PNT>
	{
	public:
		cMesh_PNT() : cMeshT<cVert_PNT>() {}
		virtual ~cMesh_PNT() {}
		virtual void GetVertexPosition(unsigned int idx, glm::vec3& positionOut)
		{
			const glm::vec4& p = mVertices[idx].Position;
			Set(positionOut, p.x, p.y, p.z);
		}
		virtual void RecalculateMaxExtents()
		{
			SetMin(mMaxExtents);
			SetMax(mMinExtents);
			unsigned int numVertices = mVertices.size();
			for (unsigned int idxVert = 0; idxVert < numVertices; idxVert++)
			{
				glm::vec4& pos = mVertices[idxVert].Position;
				SetMax(mMaxExtents, pos.x, pos.y, pos.z);
				SetMin(mMinExtents, pos.x, pos.y, pos.z);
			}
		}
		virtual void Rescale(float appliedScale, bool recalculateMaxExtents)
		{
			unsigned int numVertices = mVertices.size();
			for (unsigned int idxVert = 0; idxVert < numVertices; idxVert++)
			{
				glm::vec4& pos = mVertices[idxVert].Position;
				pos *= appliedScale;
				pos.w = 1.f;
			}
			if (recalculateMaxExtents) RecalculateMaxExtents();
		}
		virtual glm::vec4 CenterVertexPositions(bool recalculateMaxExtents)
		{
			glm::vec4 center;
			unsigned int numVertices = mVertices.size();
			for (unsigned int idxVert = 0; idxVert < numVertices; idxVert++)
			{
				center += mVertices[idxVert].Position;
			}
			center /= (float)numVertices;
			for (unsigned int idxVert = 0; idxVert < numVertices; idxVert++)
			{
				glm::vec4& pos = mVertices[idxVert].Position;
				pos -= center;
				pos.w = 1.f;
			}
			center.w = 1.f;
			if (recalculateMaxExtents) RecalculateMaxExtents();
			return center;
		};
	private:
		cMesh_PNT(const cMesh_PNT& other) {}
		cMesh_PNT& operator=(const cMesh_PNT& other) { return *this; }
	};

	class cBuffer_PNT : public cBufferT<cVert_PNT>
	{
	public:
		cBuffer_PNT() : cBufferT<cVert_PNT>()
		{
			CreateDesc(mDesc);
		}
		virtual ~cBuffer_PNT() {}
	private:
		cBuffer_PNT(const cBuffer_PNT& other) {}
		cBuffer_PNT& operator=(const cBuffer_PNT& other) { return *this; }
	};
};