#include "RenderToSurface.h"
#include "GraphicsDevice.h"


RenderToSurface RenderToSurface::create(const GraphicsDevice& device, unsigned width, unsigned height, D3DFORMAT format, bool depthStencil, D3DFORMAT depthStencilFormat)
{
	RenderToSurface renderToSurface;

	Validator::validate(D3DXCreateRenderToSurface(device, width, height, format, depthStencil, depthStencilFormat, &renderToSurface), Exceptions::createRenderToSurfaceFailed);

	return renderToSurface;
}