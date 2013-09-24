#pragma once

#include "GraphicsTypes.h"

class GraphicsDevice;

class Model
{
	typedef std::vector<Material> MaterialCollection;
	typedef std::vector<Texture> TextureCollection;

public:
	static Model createFromFile(const GraphicsDevice& device, const std::string& filename);

	void draw(const GraphicsDevice& device);

	const Mesh& getMesh() const;

private:
	Mesh mesh;

	MaterialCollection materials;
	TextureCollection textures;
};