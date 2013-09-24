#include "Texture.h"
#include "GraphicsDevice.h"


Texture::Texture()
{
}


Texture::Texture(IDirect3DTexture9* resource) : Resource<IDirect3DTexture9>(resource)
{
}


Texture Texture::createFromFile(const GraphicsDevice& device, const std::string& filename)
{
	Texture texture;

	Validator::validate(D3DXCreateTextureFromFileA(device, filename.c_str(), &texture), Exceptions::createTextureFromFileFailed);

	return texture;
}


Texture Texture::create(const GraphicsDevice& device, unsigned width, unsigned height, unsigned levels, unsigned long usage, D3DFORMAT format, D3DPOOL pool)
{
	return device.createTexture(width, height, levels, usage, format, pool);
}


TextureDesc Texture::getDesc(unsigned level) const
{
	TextureDesc desc;

	Validator::validate(resource->GetLevelDesc(level, &desc), Exceptions::textureGetDescFailed);

	return desc;
}


Surface Texture::getSurfaceLevel(unsigned level) const
{
	Surface surface;

	Validator::validate(resource->GetSurfaceLevel(level, &surface), Exceptions::textureGetSurfaceLevelFailed);

	return surface;
}


void Texture::lockRect(D3DLOCKED_RECT *pLockedRect, unsigned long flags, unsigned level, const RECT* pRect) const
{
	Validator::validate(resource->LockRect(level, pLockedRect, pRect, flags), Exceptions::textureLockRectFailed);
}


void Texture::unlockRect(unsigned level) const
{
	Validator::validate(resource->UnlockRect(level), Exceptions::textureUnlockRectFailed);
}