#pragma once

#include <d3dx9.h>

#include "Resource.h"
#include "Validator.h"
#include "Exceptions.h"

class GraphicsDevice;

class RenderToSurface : public Resource<ID3DXRenderToSurface>
{
public:
	static RenderToSurface create(const GraphicsDevice& device, unsigned width, unsigned height, D3DFORMAT format = D3DFMT_X8R8G8B8, bool depthStencil = true, D3DFORMAT depthStencilFormat = D3DFMT_D24X8);
};