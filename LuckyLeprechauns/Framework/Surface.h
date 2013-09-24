#pragma once

#include <d3d9.h>

#include "Resource.h"

class Surface : public Resource<IDirect3DSurface9>
{
public:
	Surface();
	Surface(IDirect3DSurface9* resource);
};