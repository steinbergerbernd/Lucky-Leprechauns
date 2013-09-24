#include "Material.h"


Material::Material(const D3DMATERIAL9& material) : D3DMATERIAL9(material)
{
}


Material::Material(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, float power, const Vector4& emissive)
{
	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
	Power = power;
	Emissive = emissive;
}
