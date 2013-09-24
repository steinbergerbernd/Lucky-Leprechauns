#pragma once

#include <d3d9.h>

#include "Validator.h"
#include "Exceptions.h"
#include "Resource.h"

class StateBlock : public Resource<IDirect3DStateBlock9>
{
public:
	void apply() const;
};