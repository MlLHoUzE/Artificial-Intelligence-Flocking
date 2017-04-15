#include "core.h"

namespace nGraphics
{
	iBuffer* gBufferInUse = 0;
	void SetActiveBuffer(iBuffer* buffer)
	{
		if (gBufferInUse == buffer) return;
		if (gBufferInUse) gBufferInUse->Disable();
		gBufferInUse = buffer;
		if (gBufferInUse) gBufferInUse->Bind();
	}

	void CreateDesc(cVertexDescT<cVert_PNT>& desc)
	{
		desc.clear();
		desc.push_back(cVertexAttributeDesc());		// position
		desc.push_back(cVertexAttributeDesc());		// normal
		desc.push_back(cVertexAttributeDesc());		// tex coord 0 and 1
	}
};