#pragma once

#include <d3d9.h>

#include "Exception.h"

class Validator
{
public:
	static void validate(HRESULT result, const std::string& exceptionMessage);
};