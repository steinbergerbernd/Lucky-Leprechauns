#include "Model.h"
#include "GraphicsDevice.h"

Model Model::createFromFile(const GraphicsDevice& device, const std::string& filename)
{
	Model model;

	ID3DXBuffer* materialBuffer;
	unsigned long numMaterials;

	D3DXLoadMeshFromXA(filename.c_str(), D3DXMESH_MANAGED, device, 0, &materialBuffer, 0, &numMaterials, &model.mesh);

	model.materials = MaterialCollection(numMaterials);
	model.textures = TextureCollection(numMaterials);

	D3DXMATERIAL* materials = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();

	std::string path = filename.substr(0, filename.find_last_of('/') + 1);

	for (unsigned i = 0; i < numMaterials; ++i)
	{
		if (materials[i].pTextureFilename)
			model.textures[i] = Texture::createFromFile(device, path + materials[i].pTextureFilename);

		model.materials[i] = materials[i].MatD3D;
	}

	materialBuffer->Release();

	return model;
}


void Model::draw(const GraphicsDevice& device)
{
	for (unsigned i = 0; i < materials.size(); ++i)
	{
		device.setMaterial(materials[i]);
		device.setTexture(textures[i]);

		mesh.drawSubset(i);
	}
}


const Mesh& Model::getMesh() const
{
	return mesh;
}