#pragma once

#include "Resource.h"
#include "STL.h"
#include "Exceptions.h"
#include "GraphicsTypes.h"
#include "GraphicsHelpers.h"
#include "Helpers.h"
#include "Types.h"

class GraphicsDevice : public Resource<IDirect3DDevice9>
{
	typedef std::map<D3DRENDERSTATETYPE, unsigned long> RenderStateMap;

public:
	GraphicsDevice();
	GraphicsDevice(IDirect3DDevice9* device);

	operator IDirect3DDevice9*() const { return resource; }

	Viewport getViewport() const;

	unsigned long getRenderState(D3DRENDERSTATETYPE state) const;

	void clear(const Color& color = Color::black, unsigned long flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, float z = 1, unsigned long stencil = 0, unsigned long count = 0, const D3DRECT *pRects = 0) const;
	
	void beginScene() const;
	void endScene() const;

	Surface getRenderTarget(unsigned long index = 0) const;
	void setRenderTarget(const Surface& target, unsigned long index = 0) const;

	VertexDeclaration createVertexDeclaration(const D3DVERTEXELEMENT9 *pVertexElements) const;
	VertexBuffer createVertexBuffer(unsigned length, unsigned long fvf, unsigned long usage = 0, D3DPOOL pool = D3DPOOL_MANAGED, HANDLE* pSharedHandle = 0) const;
	IndexBuffer createIndexBuffer(unsigned length, D3DFORMAT format, unsigned long usage = 0, D3DPOOL pool = D3DPOOL_MANAGED, HANDLE* pSharedHandle = 0) const;
	Texture createTexture(unsigned width, unsigned height, unsigned levels = 0, unsigned long usage = 0, D3DFORMAT format = D3DFMT_X8R8G8B8, D3DPOOL pool = D3DPOOL_MANAGED) const;

	StateBlock createStateBlock(D3DSTATEBLOCKTYPE type = D3DSBT_ALL) const;

	Surface createDepthStencilSurface(unsigned width, unsigned height, D3DFORMAT format = D3DFMT_D16, D3DMULTISAMPLE_TYPE multiSample = D3DMULTISAMPLE_NONE, unsigned long multisampleQuality = 0, bool discard = true, HANDLE *pSharedHandle = 0) const;
	Surface getDepthStencilSurface() const;
	void setDepthStencilSurface(Surface depthStencilSurface) const;

	void setTransform(D3DTRANSFORMSTATETYPE state, const Matrix& matrix) const;

	void setWorld(const Matrix& world) const;
	void setView(const Matrix& view) const;
	void setProjection(const Matrix& projection) const;
	
	void setRenderState(D3DRENDERSTATETYPE state, unsigned long value) const;
	void setSamplerState(unsigned long sampler, D3DSAMPLERSTATETYPE type, unsigned long value) const;
	void setFVF(unsigned long fvf) const;
	void setVertexDeclaration(const VertexDeclaration& vertexDeclaration) const;

	void setClipPlane(const Plane& plane, unsigned long index = 0) const;
	void setScissorRect(const RECT& scissorRect) const;

	void setStreamSourceFreq(unsigned streamNumber, unsigned frequencyParameter) const;
	void setStreamSource(const VertexBuffer& vertexBuffer, unsigned stride, unsigned streamNumber = 0, unsigned offsetInBytes = 0) const;
	void setIndices(const IndexBuffer& indexBuffer) const;

	void drawPrimitive(D3DPRIMITIVETYPE primitiveType, unsigned primitiveCount, unsigned startVertex = 0) const;
	void drawIndexedPrimitive(D3DPRIMITIVETYPE primitiveType, unsigned primitiveCount, unsigned numVertices, INT baseVertexIndex = 0, unsigned minIndex = 0, unsigned startIndex = 0) const;
	void drawPrimitiveUP(D3DPRIMITIVETYPE primitiveType, unsigned primitiveCount, const void* vertices, unsigned vertexSize) const;

	void setTexture(const Texture& texture, unsigned long sampler = 0) const;
	void setMaterial(const Material& material) const;

	void setLight(unsigned long index, const Light& light) const;
	void lightEnable(unsigned long index, bool bEnable) const;

private:
	static RenderStateMap renderStates;
};