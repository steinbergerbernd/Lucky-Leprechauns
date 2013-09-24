#pragma once

#include "BasicEffect.h"

class FBXEffect : public BasicEffect
{
public:
	void setBoneMatrices(const std::vector<Matrix>& boneMatrices);
	void setTechnique(bool skinning, bool shadowMap);
};