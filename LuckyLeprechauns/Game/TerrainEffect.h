#pragma once

#include "BasicEffect.h"
#include "TerrainPoint.h"

class TerrainEffect : public BasicEffect
{
public:
	void setWorldViewProjection(const Matrix& worldViewProjection);
	void setHeightmap(const Texture& heightmap);
	void setNormalmap(const Texture& normalmap);
	void setTerrainWidth(int width);
	void setTerrainLength(int length);
	void setLightDirection(const Vector3& direction);
	void setBumpiness(float bumpiness);
	void setPatchOffset(const TerrainPoint& offset);
	void setTerrainOffset(float offset);
	void setLevelOfDetail(unsigned levelOfDetail);
	void setBlending(float blending);
	void setMinLevelOfDetail(unsigned minLevelOfDetail);

private:
	static const char* worldViewProjection;
	static const char* heightmap;
	static const char* normalmap;
	static const char* terrainWidth;
	static const char* terrainLength;
	static const char* lightDirection;
	static const char* bumpiness;
	static const char* patchOffset;
	static const char* terrainOffset;
	static const char* levelOfDetail;
	static const char* blending;
	static const char* minLevelOfDetail;
};