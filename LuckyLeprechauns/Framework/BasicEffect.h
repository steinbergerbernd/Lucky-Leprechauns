#pragma once

#include "Effect.h"
#include "Camera.h"

class BasicEffect : public Effect
{
public:
	void setWorld(const Matrix& world);
	void setView(const Matrix& view);
	void setProjection(const Matrix& projection);

	void setLight(const Light& light);
	void setMaterial(const Material& material);

	void setColormap(const Texture& texture);

	void setClipPlane(const Plane& clipPlane);
	void setCamera(const Camera& camera);

	void setAlphaCutoff(float cutoff);

private:
	struct EffectCamera
	{
		EffectCamera(const Camera& c) : position(c.getPosition()) {}

		Vector3 position;
	};

	struct EffectMaterial
	{
		EffectMaterial(const Material& m) 
			: ambient(m.Ambient), diffuse(m.Diffuse), specular(m.Specular), emissive(m.Emissive), power(m.Power) {}

		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector4 emissive;
		float power;
	};

	struct EffectLight
	{
		EffectLight(const Light& light)
			: direction(light.Direction), ambient(light.Ambient), diffuse(light.Diffuse), specular(light.Specular) {}

		Vector3 direction;
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
	};
};