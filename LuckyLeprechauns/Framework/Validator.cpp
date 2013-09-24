#include "Validator.h"


void Validator::validate(HRESULT result, const std::string& exceptionMessage)
{
	if (result != D3D_OK)
		throw Exception(exceptionMessage);
}
