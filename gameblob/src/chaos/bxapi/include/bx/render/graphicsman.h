#ifndef BX_RENDER_GRAPHICSMAN_H
#define BX_RENDER_GRAPHICSMAN_H

#include <ml/types.h>

class cGraphicsMan {
   public:
	typedef u32 TexHandle;
	typedef u32 eGMLayer; // TODO this is an enum obviously.

	TexHandle AllocShape(u8* pShapeData, const char* pszName, u32 unk = 0);

	void Layer(eGMLayer layer);

	void SetTexture(TexHandle handle, u32 unkIndex);

	void Submit();
};

extern cGraphicsMan GrMan;

#endif
