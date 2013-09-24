#include "TerrainEffect.h"

const char* TerrainEffect::worldViewProjection = "WorldViewProjection";
const char* TerrainEffect::heightmap = "Heightmap";
const char* TerrainEffect::normalmap = "Normalmap";
const char* TerrainEffect::terrainWidth = "TerrainWidth";
const char* TerrainEffect::terrainLength = "TerrainLength";
const char* TerrainEffect::lightDirection = "LightDirection";
const char* TerrainEffect::bumpiness = "Bumpiness";
const char* TerrainEffect::patchOffset = "PatchOffset";
const char* TerrainEffect::terrainOffset = "TerrainOffset";
const char* TerrainEffect::levelOfDetail = "LevelOfDetail";
const char* TerrainEffect::blending = "Blending";
const char* TerrainEffect::minLevelOfDetail = "MinLevelOfDetail";


void TerrainEffect::setWorldViewProjection(const Matrix& worldViewProjection)
{
	setMatrix(this->worldViewProjection, worldViewProjection);
}


void TerrainEffect::setHeightmap(const Texture& heightmap)
{
	setTexture(this->heightmap, heightmap);
}


void TerrainEffect::setTerrainWidth(int width)
{
	setInt(this->terrainWidth, width);
}


void TerrainEffect::setTerrainLength(int length)
{
	setInt(this->terrainLength, length);
}


void TerrainEffect::setNormalmap(const Texture& normalmap)
{
	setTexture(this->normalmap, normalmap);
}


void TerrainEffect::setLightDirection(const Vector3& direction)
{
	setVector(this->lightDirection, direction);
}


void TerrainEffect::setBumpiness(float bumpiness)
{
	setFloat(this->bumpiness, bumpiness);
}


void TerrainEffect::setPatchOffset(const TerrainPoint& offset)
{
	setVector(this->patchOffset, (Vector2)offset);
}


void TerrainEffect::setTerrainOffset(float offset)
{
	setFloat(this->terrainOffset, offset);
}


void TerrainEffect::setLevelOfDetail(unsigned levelOfDetail)
{
	setInt(this->levelOfDetail, (int)levelOfDetail);
}


void TerrainEffect::setMinLevelOfDetail(unsigned minLevelOfDetail)
{
	setInt(this->minLevelOfDetail, (int)minLevelOfDetail);
}


void TerrainEffect::setBlending(float blending)
{
	setFloat(this->blending, blending);
}