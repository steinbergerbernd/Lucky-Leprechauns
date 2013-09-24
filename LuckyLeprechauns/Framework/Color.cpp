#include "Color.h"


const Color Color::black = Color(0);
const Color Color::white = Color(1);


Color::Color() : color(D3DXCOLOR(0, 0, 0, 1))
{
}


Color::Color(float r, float g, float b, float a) : color(D3DXCOLOR(r, g, b, a))
{
}


Color::Color(float f) : color(D3DXCOLOR(f, f, f, 1))
{
}