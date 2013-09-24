#pragma once

#include <d3d9.h>

struct Viewport : public D3DVIEWPORT9
{
	float getAspectRatio() const;
};