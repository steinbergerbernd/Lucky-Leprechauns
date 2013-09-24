#include "BasicEffect.h"


void BasicEffect::setWorld(const Matrix& world)
{
	setMatrix("World", world);
}


void BasicEffect::setView(const Matrix& view)
{
	setMatrix("View", view);
}


void BasicEffect::setProjection(const Matrix& projection)
{
	setMatrix("Projection", projection);
}


void BasicEffect::setLight(const Light& light)
{
	setValue("Light", &EffectLight(light), sizeof(EffectLight));
}


void BasicEffect::setMaterial(const Material& material)
{
	setValue("Material", &EffectMaterial(material), sizeof(EffectMaterial));
}


void BasicEffect::setColormap(const Texture& texture)
{
	Effect::setTexture("Colormap", texture);
}


void BasicEffect::setClipPlane(const Plane& clipPlane)
{
	setVector("ClipPlane", (const float*)clipPlane);
}


void BasicEffect::setCamera(const Camera& camera)
{
	setValue("Camera", &EffectCamera(camera), sizeof(EffectCamera)); 
}


void BasicEffect::setAlphaCutoff(float cutoff)
{
	setFloat("AlphaCutoff", cutoff);
}