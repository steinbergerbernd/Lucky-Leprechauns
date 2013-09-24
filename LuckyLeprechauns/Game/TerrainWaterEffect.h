#pragma once

#include "BasicEffect.h"

class TerrainWaterEffect : public BasicEffect
{
public:
	void setReflectionmap(const Texture& reflectionmap);
	void setRefractionmap(const Texture& refractionmap);
	void setWavemap(const Texture& wavemap);
	void setWorldViewProjection(const Matrix& worldViewProjection);
	void setWaveLength(const Vector2& waveLength);
	void setWaveHeight(const Vector2& waveHeight);
	void setWaveSpeed(const Vector2& waveSpeed);
	void setTerrainHeightmap(const Texture& heightmap);
	void setTerrainScale(const Vector2& terrainScale);
	void setTerrainOffset(float terrainOffset);
	void setTime(float time);
	void setCameraPosition(const Vector3& cameraPosition);

private:
	static const char* reflectionmap;
	static const char* refractionmap;
	static const char* wavemap;
	static const char* worldViewProjection;
	static const char* waveLength;
	static const char* waveHeight;
	static const char* waveSpeed;
	static const char* time;
	static const char* terrainHeightmap;
	static const char* terrainScale;
	static const char* terrainOffset;
	static const char* cameraPosition;
};