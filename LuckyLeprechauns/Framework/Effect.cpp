 #include "Effect.h"


Effect::FloatCacheMap Effect::floatCache;
Effect::VectorCacheMap Effect::vectorCache;
Effect::BoolCacheMap Effect::boolCache;
Effect::TextureCacheMap Effect::textureCache;
Effect::IntCacheMap Effect::intCache;


void Effect::commitChanges()
{
	checkResult(resource->CommitChanges(), Exceptions::effectCommitChangesFailed);
}


void Effect::onLostDevice()
{
	for (TextureCache::iterator it = textureCache[resource].begin(); it != textureCache[resource].end(); ++it)
		it->second.release();

	if (resource)
		checkResult(resource->OnLostDevice(), Exceptions::effectOnLostDeviceFailed);
}


void Effect::onResetDevice()
{
	if (resource)
		checkResult(resource->OnResetDevice(), Exceptions::effectOnResetDeviceFailed);
}


D3DXHANDLE Effect::getTechnique(const std::string& name)
{
	if (techniques.count(name) == 0)
		techniques.insert(HandleKeyValuePair(name, resource->GetTechniqueByName(name.c_str())));

	return techniques[name];
}


D3DXHANDLE Effect::getHandle(const std::string& name)
{
	if (handles.count(name) == 0)
		handles.insert(HandleKeyValuePair(name, resource->GetParameterByName(0, name.c_str())));

	return handles[name];
}


unsigned Effect::begin(unsigned long flags) const
{
	unsigned passes;

	checkResult(resource->Begin(&passes, flags), Exceptions::effectBeginFailed);

	return passes;
}


void Effect::end() const
{
	checkResult(resource->End(), Exceptions::effectEndFailed);
}


void Effect::beginPass(unsigned pass) const
{
	checkResult(resource->BeginPass(pass), Exceptions::effectBeginPassFailed);
}


void Effect::endPass() const
{
	checkResult(resource->EndPass(), Exceptions::effectEndPassFailed);
}


void Effect::beginSinglePass(unsigned long flags) const
{
	begin();
	beginPass(0);
}


void Effect::endSinglePass() const
{
	endPass();
	end();
}


void Effect::checkResult(HRESULT result, const std::string& exceptionMessage) const
{
	Validator::validate(result, exceptionMessage);
}


void Effect::setTechnique(const std::string& techniqueName)
{
	checkResult(resource->SetTechnique(getTechnique(techniqueName)), Exceptions::effectFailed);
}


void Effect::setMatrix(const std::string& parameterName, const Matrix& matrix)
{
	checkResult(resource->SetMatrix(getHandle(parameterName), &matrix), Exceptions::effectSetMatrixFailed);
}


void Effect::setMatrixArray(const std::string& parameterName, const std::vector<Matrix>& matrices)
{
	checkResult(resource->SetMatrixArray(getHandle(parameterName), &matrices[0], matrices.size()), Exceptions::effectSetMatrixFailed);
}


void Effect::setTexture(const std::string& parameterName, const Texture& texture)
{
	if (textureCache[resource][parameterName] != texture)
	{
		checkResult(resource->SetTexture(getHandle(parameterName), texture), Exceptions::effectSetTextureFailed);
		textureCache[resource][parameterName] = texture;
	}
}


void Effect::setInt(const std::string& parameterName, int value)
{
	if (intCache[resource][parameterName] != value)
	{
		checkResult(resource->SetInt(getHandle(parameterName), value), Exceptions::effectSetIntFailed);
		intCache[resource][parameterName] = value;
	}
}


void Effect::setFloat(const std::string& parameterName, float value)
{
	if (floatCache[resource][parameterName] != value)
	{
		checkResult(resource->SetFloat(getHandle(parameterName), value), Exceptions::effectSetFloatFailed);
		floatCache[resource][parameterName] = value;
	}
}


void Effect::setVector(const std::string& parameterName, const Vector4& vector)
{
	if (vectorCache[resource][parameterName] != vector)
	{
		checkResult(resource->SetVector(getHandle(parameterName), &vector), Exceptions::effectSetVectorFailed);
		vectorCache[resource][parameterName] = vector;
	}
}


void Effect::setVectorArray(const std::string& parameterName, const std::vector<Vector4>& vectors)
{
	checkResult(resource->SetVectorArray(getHandle(parameterName), &vectors[0], vectors.size()), Exceptions::effectSetVectorFailed);
}


void Effect::setBool(const std::string& parameterName, bool value)
{
	if (boolCache[resource][parameterName] != value)
	{
		checkResult(resource->SetBool(getHandle(parameterName), value), Exceptions::effectSetBoolFailed);
		boolCache[resource][parameterName] = value;
	}
}


void Effect::setValue(const std::string& parameterName, const void* data, unsigned bytes)
{
	checkResult(resource->SetValue(getHandle(parameterName), data, bytes), Exceptions::effectFailed);
}