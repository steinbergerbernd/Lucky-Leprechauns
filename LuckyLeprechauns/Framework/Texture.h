#pragma once

#include <d3d9.h>

#include "Resource.h"
#include "Validator.h"
#include "STL.h"
#include "Surface.h"

class GraphicsDevice;

typedef D3DSURFACE_DESC TextureDesc;

class Texture : public Resource<IDirect3DTexture9>
{
public:
	Texture();
	Texture(IDirect3DTexture9* resource);

	operator IDirect3DBaseTexture9*() const { return resource; }

	template <class T>
	void getData(T* data) const;

	TextureDesc getDesc(unsigned level = 0) const;
	Surface getSurfaceLevel(unsigned level = 0) const;

	static Texture createFromFile(const GraphicsDevice& device, const std::string& filename);
	static Texture create(const GraphicsDevice& device, unsigned width, unsigned height, unsigned levels = 0, unsigned long usage = 0, D3DFORMAT format = D3DFMT_X8R8G8B8, D3DPOOL pool = D3DPOOL_MANAGED);

	void lockRect(D3DLOCKED_RECT *pLockedRect, unsigned long flags = 0, unsigned level = 0, const RECT* pRect = 0) const;
	void unlockRect(unsigned level = 0) const;
};


template <class T>
void Texture::getData(T* data) const
{
	D3DLOCKED_RECT rect;
	TextureDesc desc = getDesc();

	lockRect(&rect, D3DLOCK_READONLY);

	memcpy(data, rect.pBits, desc.Width * desc.Height * sizeof(T));

	unlockRect();
}
