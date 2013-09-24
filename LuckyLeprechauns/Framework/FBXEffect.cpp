#include "FBXEffect.h"

void FBXEffect::setBoneMatrices(const std::vector<Matrix>& boneMatrices)
{
	setMatrixArray("BoneMatrices", boneMatrices);
}


void FBXEffect::setTechnique(bool skinning, bool shadowMap)
{
	if (skinning && shadowMap)
		Effect::setTechnique("ShadowMapSkinned");
	else if (shadowMap)
		Effect::setTechnique("ShadowMapDefault");
	else if (skinning)
		Effect::setTechnique("Skinned");
	else
		Effect::setTechnique("Default");
}