#pragma once

#include <d3d9.h>

#include "GraphicsBuffer.h"
#include "Exceptions.h"

class VertexBuffer : public GraphicsBuffer<IDirect3DVertexBuffer9, D3DVERTEXBUFFER_DESC>
{
protected:
	const char* getLockFailedException() const;
	const char* getUnlockFailedException() const;
	const char* getGetDescFailedException() const;
};