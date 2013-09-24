#pragma once

#include "Types.h"
#include <fbxsdk.h>

struct FBXBone
{
	unsigned index;

	Matrix globalTransform;
	Matrix globalTransformInverse;

	KFbxAnimCurve* rotationCurves[3];
	KFbxAnimCurve* translationCurves[3];

	Vector3 translation;
	Vector3 rotation;
};