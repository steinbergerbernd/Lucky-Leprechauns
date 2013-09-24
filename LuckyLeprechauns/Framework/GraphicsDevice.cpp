#include "GraphicsDevice.h"


GraphicsDevice::RenderStateMap GraphicsDevice::renderStates;


GraphicsDevice::GraphicsDevice()
{
}


GraphicsDevice::GraphicsDevice(IDirect3DDevice9* resource) : Resource<IDirect3DDevice9>(resource)
{
}


void GraphicsDevice::clear(const Color& color, unsigned long flags, float z, unsigned long stencil, unsigned long count, const D3DRECT *pRects) const
{
	Validator::validate(resource->Clear(count, pRects, flags, color, z, stencil), Exceptions::clearFailed);
}


void GraphicsDevice::beginScene() const
{
	Validator::validate(resource->BeginScene(), Exceptions::beginSceneFailed);
}


void GraphicsDevice::endScene() const
{
	Validator::validate(resource->EndScene(), Exceptions::endSceneFailed);
}


Surface GraphicsDevice::getRenderTarget(unsigned long index) const
{	
	Surface surface;

	Validator::validate(resource->GetRenderTarget(index, &surface), Exceptions::getRenderTargetFailed);

	return surface;
}


void GraphicsDevice::setRenderTarget(const Surface& target, unsigned long index) const
{
	Validator::validate(resource->SetRenderTarget(index, target), Exceptions::setRenderTargetFailed);
}


void GraphicsDevice::setRenderState(D3DRENDERSTATETYPE state, unsigned long value) const
{
	renderStates[state] = value;

	Validator::validate(resource->SetRenderState(state, value), Exceptions::setRenderStateFailed);
}


Viewport GraphicsDevice::getViewport() const
{
	Viewport viewport;

	Validator::validate(resource->GetViewport(&viewport), Exceptions::getViewportFailed);

	return viewport;
}


Texture GraphicsDevice::createTexture(unsigned width, unsigned height, unsigned levels, unsigned long usage, D3DFORMAT format, D3DPOOL pool) const
{
	Texture texture;

	Validator::validate(resource->CreateTexture(width, height, levels, usage, format, pool, &texture, 0), Exceptions::createTextureFailed);

	return texture;
}


VertexBuffer GraphicsDevice::createVertexBuffer(unsigned length, unsigned long fvf, unsigned long usage, D3DPOOL pool, HANDLE* pSharedHandle) const
{
	VertexBuffer vertexBuffer;

	Validator::validate(resource->CreateVertexBuffer(length, usage, fvf, pool, &vertexBuffer, pSharedHandle), Exceptions::createVertexBufferFailed);

	return vertexBuffer;
}


IndexBuffer GraphicsDevice::createIndexBuffer(unsigned length, D3DFORMAT format, unsigned long usage, D3DPOOL pool, HANDLE* pSharedHandle) const
{
	IndexBuffer indexBuffer;

	Validator::validate(resource->CreateIndexBuffer(length, usage, format, pool, &indexBuffer, pSharedHandle), Exceptions::createIndexBufferFailed);

	return indexBuffer;
}


VertexDeclaration GraphicsDevice::createVertexDeclaration(const D3DVERTEXELEMENT9 *pVertexElements) const
{
	VertexDeclaration vertexDeclaration;

	Validator::validate(resource->CreateVertexDeclaration(pVertexElements, &vertexDeclaration), Exceptions::createVertexDeclarationFailed);

	return vertexDeclaration;
}


StateBlock GraphicsDevice::createStateBlock(D3DSTATEBLOCKTYPE type) const
{
	StateBlock stateBlock;

	Validator::validate(resource->CreateStateBlock(type, &stateBlock), Exceptions::createStateBlockFailed);

	return stateBlock;
}


void GraphicsDevice::setTransform(D3DTRANSFORMSTATETYPE state, const Matrix& matrix) const
{
	Validator::validate(resource->SetTransform(state, &matrix), Exceptions::setTransformFailed);
}


void GraphicsDevice::setFVF(unsigned long fvf) const
{
	Validator::validate(resource->SetFVF(fvf), Exceptions::setFVFFailed);
}


void GraphicsDevice::setVertexDeclaration(const VertexDeclaration& vertexDeclaration) const
{
	Validator::validate(resource->SetVertexDeclaration(vertexDeclaration), Exceptions::setVertexDeclarationFailed);
}


void GraphicsDevice::setStreamSourceFreq(unsigned streamNumber, unsigned frequencyParameter) const
{
	Validator::validate(resource->SetStreamSourceFreq(streamNumber, frequencyParameter), Exceptions::setStreamSourceFreqFailed);
}


void GraphicsDevice::setStreamSource(const VertexBuffer& vertexBuffer, unsigned stride, unsigned streamNumber, unsigned offsetInBytes) const
{
	Validator::validate(resource->SetStreamSource(streamNumber, vertexBuffer, offsetInBytes, stride), Exceptions::setStreamSourceFailed);
}


void GraphicsDevice::drawPrimitive(D3DPRIMITIVETYPE primitiveType, unsigned primitiveCount, unsigned startVertex) const
{
	Validator::validate(resource->DrawPrimitive(primitiveType, startVertex, primitiveCount), Exceptions::drawPrimitiveFailed);
}


void GraphicsDevice::drawIndexedPrimitive(D3DPRIMITIVETYPE primitiveType, unsigned primitiveCount, unsigned numVertices, INT baseVertexIndex, unsigned minIndex, unsigned startIndex) const
{
	Validator::validate(resource->DrawIndexedPrimitive(primitiveType, baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount), Exceptions::drawIndexedPrimitiveFailed); 
}


void GraphicsDevice::drawPrimitiveUP(D3DPRIMITIVETYPE primitiveType, unsigned primitiveCount, const void* vertices, unsigned vertexSize) const
{
	Validator::validate(resource->DrawPrimitiveUP(primitiveType, primitiveCount, vertices, vertexSize), Exceptions::drawPrimitiveUPFailed);
}


void GraphicsDevice::setTexture(const Texture& texture, unsigned long sampler) const
{
	Validator::validate(resource->SetTexture(sampler, texture), Exceptions::setTextureFailed);
}


void GraphicsDevice::setMaterial(const Material& material) const
{
	Validator::validate(resource->SetMaterial(&material), Exceptions::setMaterialFailed);
}


void GraphicsDevice::setIndices(const IndexBuffer& indexBuffer) const
{
	Validator::validate(resource->SetIndices(indexBuffer), Exceptions::setIndicesFailed);
}


void GraphicsDevice::setWorld(const Matrix& world) const
{
	setTransform(D3DTS_WORLD, world);
}


void GraphicsDevice::setView(const Matrix& view) const
{
	setTransform(D3DTS_VIEW, view);
}


void GraphicsDevice::setProjection(const Matrix& projection) const
{
	setTransform(D3DTS_PROJECTION, projection);
}


void GraphicsDevice::setLight(unsigned long index, const Light& light) const
{
	Validator::validate(resource->SetLight(index, &light), Exceptions::setLightFailed);
}


void GraphicsDevice::lightEnable(unsigned long index, bool bEnable) const
{
	Validator::validate(resource->LightEnable(index, bEnable), Exceptions::lightEnableFailed);
}


unsigned long GraphicsDevice::getRenderState(D3DRENDERSTATETYPE state) const
{
	RenderStateMap::iterator it = renderStates.find(state);

	unsigned long renderState;

	if (it != renderStates.end())
		renderState = it->second;
	else
		Validator::validate(resource->GetRenderState(state, &renderState), Exceptions::getRenderStateFailed);

	return renderState;
}


void GraphicsDevice::setSamplerState(unsigned long sampler, D3DSAMPLERSTATETYPE type, unsigned long value) const
{
	Validator::validate(resource->SetSamplerState(sampler, type, value), Exceptions::setSamplerStateFailed);
}


void GraphicsDevice::setClipPlane(const Plane& plane, unsigned long index) const
{
	Validator::validate(resource->SetClipPlane(index, plane), Exceptions::setClipPlaneFailed);
}


void GraphicsDevice::setScissorRect(const RECT& rect) const
{
	Validator::validate(resource->SetScissorRect(&rect), Exceptions::setScissorRectFailed);
}


Surface GraphicsDevice::createDepthStencilSurface(unsigned width, unsigned height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multiSample, unsigned long multisampleQuality, bool discard, HANDLE *pSharedHandle) const
{
	Surface surface;

	Validator::validate(resource->CreateDepthStencilSurface(width, height, format, multiSample, multisampleQuality, discard, &surface, pSharedHandle), Exceptions::depthStencilFailed);

	return surface;
}


Surface GraphicsDevice::getDepthStencilSurface() const
{
	Surface surface;

	Validator::validate(resource->GetDepthStencilSurface(&surface), Exceptions::depthStencilFailed);

	return surface;
}


void GraphicsDevice::setDepthStencilSurface(Surface depthStencilSurface) const
{
	Validator::validate(resource->SetDepthStencilSurface(depthStencilSurface), Exceptions::depthStencilFailed);
}