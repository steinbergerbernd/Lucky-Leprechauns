#pragma once

#include <d3dx9.h>

#include "STL.h"
#include "Exception.h"
#include "GraphicsTypes.h"
#include "Types.h"
#include "Constants.h"

class GraphicsDevice;

class Effect : public Resource<ID3DXEffect>
{
	typedef std::pair<std::string, D3DXHANDLE> HandleKeyValuePair;
	typedef std::map<std::string, D3DXHANDLE> HandleCollection;

public:
	typedef std::map<std::string, float> FloatCache;
	typedef std::map<std::string, Vector4> VectorCache;
	typedef std::map<std::string, bool> BoolCache;
	typedef std::map<std::string, Texture> TextureCache;
	typedef std::map<std::string, int> IntCache;

	typedef std::map<ID3DXEffect*, FloatCache> FloatCacheMap;
	typedef std::map<ID3DXEffect*, VectorCache> VectorCacheMap;
	typedef std::map<ID3DXEffect*, BoolCache> BoolCacheMap;
	typedef std::map<ID3DXEffect*, TextureCache> TextureCacheMap;
	typedef std::map<ID3DXEffect*, IntCache> IntCacheMap;

	template <class T>
	static T createFromFile(const GraphicsDevice& device, const std::string& filename);

	void onResetDevice();
	void onLostDevice();

	unsigned begin(unsigned long flags = 0) const;
	void end() const;

	void beginPass(unsigned pass) const;
	void endPass() const;

	void beginSinglePass(unsigned long flags = 0) const;
	void endSinglePass() const;

	void commitChanges();

//protected:
	void setMatrix(const std::string& parameterName, const Matrix& matrix);
	void setMatrixArray(const std::string& parameterName, const std::vector<Matrix>& matrices);
	void setTexture(const std::string& parameterName, const Texture& texture);
	void setInt(const std::string& parameterName, int value);
	void setFloat(const std::string& parameterName, float value);
	void setVector(const std::string& parameterName, const Vector4& vector);
	void setVectorArray(const std::string& parameterName, const std::vector<Vector4>& matrices);
	void setBool(const std::string& parameterName, bool value);
	void setValue(const std::string& parameterName, const void* data, unsigned bytes);

	void setTechnique(const std::string& techniqueName);

protected:
	D3DXHANDLE getHandle(const std::string& name);
	D3DXHANDLE getTechnique(const std::string& name);

	HandleCollection handles;
	HandleCollection techniques;

private:
	void checkResult(HRESULT result, const std::string& exceptionMessage) const;

	static FloatCacheMap floatCache;
	static VectorCacheMap vectorCache;
	static BoolCacheMap boolCache;
	static TextureCacheMap textureCache;
	static IntCacheMap intCache;
};


template <class T>
T Effect::createFromFile(const GraphicsDevice& device, const std::string& filename)
{
	T effect;
	
	ID3DXBuffer* compilationErrors;

	if (D3DXCreateEffectFromFileA(device, filename.c_str(), 0, 0, D3DXSHADER_DEBUG | D3DXFX_LARGEADDRESSAWARE, 0, &effect, &compilationErrors) != D3D_OK)
	{
		char* error = (char*)compilationErrors->GetBufferPointer();

		OutputDebugStringA(std::string(error).c_str());

		throw Exception(Exceptions::createEffectFromFileFailed);
	}

	return effect;
}