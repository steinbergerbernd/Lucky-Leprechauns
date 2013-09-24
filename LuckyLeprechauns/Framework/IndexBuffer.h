#pragma once

#include <d3d9.h>

#include "GraphicsBuffer.h"
#include "Exceptions.h"

class IndexBuffer : public GraphicsBuffer<IDirect3DIndexBuffer9, D3DINDEXBUFFER_DESC>
{
protected:
	const char* getLockFailedException() const;
	const char* getUnlockFailedException() const;
	const char* getGetDescFailedException() const;
};