#pragma once

#include <d3d9.h>

#include "Vector4.h"

struct Material : D3DMATERIAL9
{
	Material(const D3DMATERIAL9& material); 
	Material(const Vector4& ambient = Vector4::one, const Vector4& diffuse = Vector4::one, const Vector4& specular = Vector4::one, float power = 0.0f, const Vector4& emissive = Vector4::zero);
};