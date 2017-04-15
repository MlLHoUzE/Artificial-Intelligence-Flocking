#pragma once
#include "core.h"
#include "shader.h"

namespace nGraphics
{

	class cScene;
	class cSceneObject
	{
	public:
		sShaderPerItemVars Vars;

		cSceneObject();
		virtual ~cSceneObject();

		void GetMeshExtents(glm::vec3& minExtents, glm::vec3& maxExtents);
		void SetVarsRecursive(const sShaderPerItemVars& vars);
		// gets called after loading, but before meshes are passed to gpu
		// do any custom mesh manipulations here
		virtual void PrepMesh() {};

		inline unsigned int NumMeshes() const { return mMeshes.size(); }
		inline iMesh* GetMesh(unsigned int idx) { return mMeshes[idx]; }

		inline void AddMesh(iMesh* mesh) { mMeshes.push_back(mesh); }
		inline void RemoveMeshes() { mMeshes.clear(); }

		inline const std::string& GetName() { return mName; }
		inline void SetName(const char* name) { mName = name; }
		inline void SetName(const std::string& name) { mName = name; }

		void SetColor(const glm::vec4& color, bool setChildrensColor);
		iMesh* GetMeshByName(const std::string& name, bool recursive = false);

		virtual cSceneObject* GetChildByName(const std::string& name, bool recursive = false);
		virtual unsigned int NumChildren();
		virtual void AddChild(cSceneObject* child);
		virtual cSceneObject* GetChild(unsigned int index);
		virtual cSceneObject* RemoveChild(cSceneObject* child, bool deleteIt = false);
		virtual void RemoveChildren(bool deleteIt = false);
		virtual void SetChildIndex(cSceneObject* child, unsigned int index);
		virtual void RemoveFromParent();
		virtual cSceneObject* Clone(cSceneObject* result = 0);

	private:
		void GetMeshExtentsInternal(glm::vec3& minExtents, glm::vec3& maxExtents);
		friend class cScene;
		void SetScene(cScene* scene);
		cScene* mScene;

		void Render(cRenderSupport* support);
		void RenderChildren(cRenderSupport* support);
		void SetParent(cSceneObject* parent);
		cSceneObject* mParent;
		std::vector<cSceneObject*> mChildren;
		std::string mName;
		std::vector<iMesh*> mMeshes;

		cSceneObject(const cSceneObject& other) {}
		cSceneObject& operator=(const cSceneObject& other) { return *this; }
	};

	class iCamera
	{
	public:
		virtual ~iCamera() {}
		virtual void Resize(int width, int height) = 0;
		virtual void Update(float dt) = 0;
		virtual void PreRender() = 0;
		virtual void SetFrameVars(sShaderPerFrameVars* vars) = 0;

		// debug solution...eugh
		virtual void SetViewMatrix(glm::mat4& mat) = 0;
		virtual void SetProjectionMatrix(glm::mat4& mat) = 0;
		virtual void SetEyePosition(glm::vec3& vec) = 0;
	};

	class cBasicCamera : public iCamera
	{
	public:
		glm::vec3 Target;
		glm::vec3 Eye;
		glm::vec3 Up;
		int Width;
		int Height;
		float ZNear;
		float ZFar;
		float FovH;
		cBasicCamera();
		cBasicCamera(const cBasicCamera& other);
		cBasicCamera& operator=(const cBasicCamera& other);

		virtual ~cBasicCamera();
		virtual void Resize(int width, int height);
		virtual void Update(float dt);
		virtual void PreRender();
		virtual void SetFrameVars(sShaderPerFrameVars* vars);
		virtual void SetViewMatrix(glm::mat4& mat);
		virtual void SetProjectionMatrix(glm::mat4& mat);
		virtual void SetEyePosition(glm::vec3& vec);
	};

	class cScene
	{
	public:

		iCamera* Camera;

		cScene();
		~cScene();

		sShaderPerFrameVars* GetFrameVars();

		bool IsInitialized();
		//void Resize(int width, int height);
		void Render();
		bool Init();
		cSceneObject* GetChildByName(const std::string& name);
		unsigned int NumChildren();
		void AddChild(cSceneObject* child);
		cSceneObject* RemoveChild(cSceneObject* child, bool deleteIt = false);
		void RemoveChildren(bool deleteIt = false);
		void SetChildIndex(cSceneObject* child, unsigned int index);

	private:
		bool mInitialized;
		cRenderSupport* mSupport;
		cSceneObject* mRoot;
	};


};