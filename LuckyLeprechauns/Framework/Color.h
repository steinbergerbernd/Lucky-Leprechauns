#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Color
{
public:
	Color();
	Color(float f);
	Color(float r, float g, float b, float a = 1.0f);

	operator D3DCOLORVALUE() const { return D3DXCOLOR(color); }
	operator D3DCOLOR() const { return color; }

	static const Color black;
	static const Color white;

private:
	D3DCOLOR color;
};