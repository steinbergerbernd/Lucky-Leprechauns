#include "Mesh.h"
#include "GraphicsDevice.h"


Mesh Mesh::create(const GraphicsDevice& device, unsigned long numFaces, unsigned long numVertices, const D3DVERTEXELEMENT9* vertexElements, unsigned long options)
{
	Mesh mesh;

	Validator::validate(D3DXCreateMesh(numFaces, numVertices, options, vertexElements, device, &mesh), Exceptions::meshFailed);

	return mesh;
}


VertexBuffer Mesh::getVertexBuffer() const
{
	VertexBuffer vertexBuffer;

	Validator::validate(resource->GetVertexBuffer(&vertexBuffer), Exceptions::meshGetVertexBufferFailed);

	return vertexBuffer;
}


IndexBuffer Mesh::getIndexBuffer() const
{
	IndexBuffer indexBuffer;

	Validator::validate(resource->GetIndexBuffer(&indexBuffer), Exceptions::meshGetIndexBufferFailed);

	return indexBuffer;
}


Mesh::AttributeTable Mesh::getAttributeTable(unsigned long size) const
{
	AttributeTable attributeTable(size);

	Validator::validate(resource->GetAttributeTable(&attributeTable[0], &size), Exceptions::meshFailed);

	return attributeTable;
}


void Mesh::drawSubset(unsigned long attribute) const
{
	Validator::validate(resource->DrawSubset(attribute), Exceptions::meshFailed);
}


unsigned long* Mesh::lockAttributeBuffer(unsigned long flags) const
{
	unsigned long* buffer;

	Validator::validate(resource->LockAttributeBuffer(flags, &buffer), Exceptions::meshFailed);

	return buffer;
}


void Mesh::unlockAttributeBuffer() const
{
	Validator::validate(resource->UnlockAttributeBuffer(), Exceptions::meshFailed);
}


Mesh::Adjacency Mesh::generateAdjacency(float epsilon) const
{
	std::vector<unsigned long> adjacency(resource->GetNumFaces() * 3);

	Validator::validate(resource->GenerateAdjacency(epsilon, &adjacency[0]), Exceptions::meshFailed);

	return adjacency;
}


void Mesh::clean(D3DXCLEANTYPE cleanType, Adjacency& adjacency)
{
	ID3DXMesh* newResource;

	Validator::validate(D3DXCleanMesh(cleanType, resource, &adjacency[0], &newResource, &adjacency[0], 0), Exceptions::meshFailed);

	resource->Release();
	resource = newResource;
}


void Mesh::optimizeInplace(unsigned long flags, Adjacency& adjacency)
{
	Validator::validate(resource->OptimizeInplace(flags, &adjacency[0], &adjacency[0], 0, 0), Exceptions::meshFailed);
}


void Mesh::computeNormals(const Adjacency& adjacency)
{
	Validator::validate(D3DXComputeNormals(resource, &adjacency[0]), Exceptions::meshFailed);
}