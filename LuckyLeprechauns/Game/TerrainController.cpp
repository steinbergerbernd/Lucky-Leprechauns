#include "TerrainController.h"

#undef min
#undef max

TerrainController::TerrainController(LuckyLeprechauns* game) : LLDrawableGameComponent(game)
{
}


void TerrainController::init()
{
	GraphicsDevice device = ((Game&)getManager()).getGraphicsDevice();

	width = Config::getValue<int>(LLConfigKeys::terrainWidth);
	length = Config::getValue<int>(LLConfigKeys::terrainLength);

	setPosition(Vector3(width / -2.0f, 0, length / -2.0f));

	bumpiness = Config::getValue<float>(LLConfigKeys::terrainBumpiness);
	quality = Config::getValue<float>(LLConfigKeys::terrainQuality);
	offset = Config::getValue<float>(LLConfigKeys::terrainOffset);

	heightmap = Texture::createFromFile(device, Config::getValue(LLConfigKeys::terrainHeightmapPath));
	normalmap = Texture::createFromFile(device, Config::getValue(LLConfigKeys::terrainNormalmapPath));

	heightmapDesc = heightmap.getDesc();
	normalmapDesc = normalmap.getDesc();

	std::vector<HeightmapFormat> hData = std::vector<HeightmapFormat>(heightmapDesc.Width * heightmapDesc.Height);
	std::vector<NormalmapFormat> nData = std::vector<NormalmapFormat>(normalmapDesc.Width * normalmapDesc.Height);

	heightmap.getData(&hData[0]);
	normalmap.getData(&nData[0]);

	heightData = HeightData(heightmapDesc.Width * heightmapDesc.Height);
	normalData = NormalData(normalmapDesc.Width * normalmapDesc.Height);

	std::transform(hData.begin(), hData.end(), heightData.begin(), &TerrainController::convertHeight);
	std::transform(nData.begin(), nData.end(), normalData.begin(), &TerrainController::convertNormal);
}


float TerrainController::getHeight(float x, float z) const
{
	return get<float>(x, z, &TerrainController::getHeight);
}


Vector3 TerrainController::getNormal(float x, float z) const
{
	return get<Vector3>(x, z, &TerrainController::getNormal);
}


float TerrainController::getHeight(const TerrainPoint& p) const
{
	unsigned x = (unsigned)(((float)p.col / width) * heightmapDesc.Width);
	unsigned y = (unsigned)(((float)(length - p.row) / length) * heightmapDesc.Height);

	return getHeightmapData(x, y);
}


Vector3 TerrainController::getNormal(const TerrainPoint& p) const
{
	unsigned x = (unsigned)(((float)p.col / width) * normalmapDesc.Width);
	unsigned y = (unsigned)(((float)(length - p.row) / length) * normalmapDesc.Height);

	return getNormalmapData(x, y);
}


float TerrainController::getHeightmapData(unsigned x, unsigned y) const
{
	x = MathHelper::clamp(x, 0U, heightmapDesc.Width - 1U);
	y = MathHelper::clamp(y, 0U, heightmapDesc.Height - 1U);

	return (heightData[y * heightmapDesc.Width + x] + offset) * bumpiness;
}


Vector3 TerrainController::getNormalmapData(unsigned x, unsigned y) const
{
	x = MathHelper::clamp(x, 0U, normalmapDesc.Width - 1U);
	y = MathHelper::clamp(y, 0U, normalmapDesc.Height - 1U);

	return normalData[y * normalmapDesc.Width + x];
}


float TerrainController::convertHeight(HeightmapFormat height)
{
	return (float)height / std::numeric_limits<HeightmapFormat>::max();
}


Vector3 TerrainController::convertNormal(NormalmapFormat normal)
{
	Vector3 v;

	v.x = (((unsigned char*)&normal)[2] / 255.0f) * 2.0f - 1.0f;
	v.y = (((unsigned char*)&normal)[3] / 255.0f) * 2.0f - 1.0f;
	v.z = (((unsigned char*)&normal)[1] / 255.0f) * 2.0f - 1.0f;

	v.z *= -1;

	return Vector3::normalize(v);
}


Vector3 TerrainController::getHeight(const Vector3& position) const
{
	return Vector3(position.x, getHeight(position.x, position.z), position.z);
}


Vector3 TerrainController::getNormal(const Vector3& position) const
{
	return getNormal(position.x, position.z);
}