#include "TerrainWaterEffect.h"

const char* TerrainWaterEffect::reflectionmap = "Reflectionmap";
const char* TerrainWaterEffect::refractionmap = "Refractionmap";
const char* TerrainWaterEffect::wavemap = "Wavemap";
const char* TerrainWaterEffect::worldViewProjection = "WorldViewProjection";
const char* TerrainWaterEffect::waveLength = "WaveLength";
const char* TerrainWaterEffect::waveHeight = "WaveHeight";
const char* TerrainWaterEffect::waveSpeed = "WaveSpeed";
const char* TerrainWaterEffect::time = "Time";
const char* TerrainWaterEffect::terrainHeightmap = "TerrainHeightmap";
const char* TerrainWaterEffect::terrainScale = "TerrainScale";
const char* TerrainWaterEffect::terrainOffset = "TerrainOffset";
const char* TerrainWaterEffect::cameraPosition = "CameraPosition";


void TerrainWaterEffect::setReflectionmap(const Texture& reflectionmap)
{
	setTexture(this->reflectionmap, reflectionmap);
}


void TerrainWaterEffect::setRefractionmap(const Texture& refractionmap)
{
	setTexture(this->refractionmap, refractionmap);
}


void TerrainWaterEffect::setWavemap(const Texture& wavemap)
{
	setTexture(this->wavemap, wavemap);
}


void TerrainWaterEffect::setWorldViewProjection(const Matrix& worldViewProjection)
{
	setMatrix(this->worldViewProjection, worldViewProjection);
}


void TerrainWaterEffect::setWaveLength(const Vector2& waveLength)
{
	setVector(this->waveLength, waveLength);
}


void TerrainWaterEffect::setWaveHeight(const Vector2& waveHeight)
{
	setVector(this->waveHeight, waveHeight);
}


void TerrainWaterEffect::setWaveSpeed(const Vector2& waveSpeed)
{
	setVector(this->waveSpeed, waveSpeed);
}


void TerrainWaterEffect::setTerrainHeightmap(const Texture& heightmap)
{
	setTexture(this->terrainHeightmap, heightmap);
}


void TerrainWaterEffect::setTerrainScale(const Vector2& terrainScale)
{
	setVector(this->terrainScale, terrainScale);
}


void TerrainWaterEffect::setTerrainOffset(float terrainOffset)
{
	setFloat(this->terrainOffset, terrainOffset);
}


void TerrainWaterEffect::setTime(float time)
{
	setFloat(this->time, time);
}


void TerrainWaterEffect::setCameraPosition(const Vector3& cameraPosition)
{
	setVector(this->cameraPosition, cameraPosition);
}