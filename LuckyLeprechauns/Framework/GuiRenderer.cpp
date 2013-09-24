#include "GuiRenderer.h"
#include "GuiManager.h"


GuiRenderer::GuiRenderer(GuiManager* guiManager)
{
	this->guiManager = guiManager;
}


GuiRenderer::~GuiRenderer(void)
{
}

// Called by Rocket when it wants to render geometry that it does not wish to optimise.
void GuiRenderer::RenderGeometry(Rocket::Core::Vertex* ROCKET_UNUSED(vertices), int ROCKET_UNUSED(num_vertices),
	int* ROCKET_UNUSED(indices), int ROCKET_UNUSED(num_indices), const Rocket::Core::TextureHandle ROCKET_UNUSED(texture),
	const Rocket::Core::Vector2f& ROCKET_UNUSED(translation))
{
}

// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
Rocket::Core::CompiledGeometryHandle GuiRenderer::CompileGeometry(Rocket::Core::Vertex* vertices, int numVertices,
	int* indices, int numIndices, Rocket::Core::TextureHandle texture)
{
	const GraphicsDevice& device = guiManager->getGraphicsDevice();

	// Construct a new CompiledGeometry structure, which will be returned as the handle, and the buffers to store the geometry
	CompiledGeometry* geometry = new CompiledGeometry();
	geometry->vertices = device.createVertexBuffer(numVertices * sizeof(GuiVertex), GuiVertex::fvf, D3DUSAGE_WRITEONLY, D3DPOOL_DEFAULT);
	geometry->indices = device.createIndexBuffer(numIndices * sizeof(unsigned int), D3DFMT_INDEX32, D3DUSAGE_WRITEONLY, D3DPOOL_DEFAULT);

	// Fill the vertex buffer
	std::vector<GuiVertex> guiVertices(numVertices);
	for (int i = 0; i < numVertices; ++i)
	{
		guiVertices[i].position = convert(vertices[i].position);
		guiVertices[i].position.z = 0;
		guiVertices[i].color = convert(vertices[i].colour);
		guiVertices[i].texCoord = convert(vertices[i].tex_coord);
	}
	geometry->vertices.setData(guiVertices);

	// Fill the index buffer
	geometry->indices.setData(indices, numIndices);

	geometry->numVertices = (unsigned long)numVertices;
	geometry->numPrimitives = (unsigned long)numIndices / 3;

	if(texture)
		geometry->texture = Texture((IDirect3DTexture9*)texture);

	return (Rocket::Core::CompiledGeometryHandle)geometry;
}

// Called by Rocket when it wants to render application-compiled geometry.
void GuiRenderer::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)
{
	// Build and set the transform matrix.
	const GraphicsDevice& device = guiManager->getGraphicsDevice();

	device.setTransform(D3DTS_WORLD, Matrix::createTranslation(translation.x, translation.y, 0));

	CompiledGeometry* guiGeometry = (CompiledGeometry*)geometry;

	// Set the vertex format for the Rocket vertices, and bind the vertex and index buffers.
	device.setFVF(GuiVertex::fvf);
	device.setStreamSource(guiGeometry->vertices, sizeof(GuiVertex));
	device.setIndices(guiGeometry->indices);
	
	// Set the texture, if this geometry has one.
	device.setTexture(guiGeometry->texture);

	// Draw the primitives.
	device.beginScene();
	device.drawIndexedPrimitive(D3DPT_TRIANGLELIST, guiGeometry->numPrimitives, guiGeometry->numVertices);
	device.endScene();
}

// Called by Rocket when it wants to release application-compiled geometry.
void GuiRenderer::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
	CompiledGeometry* guiGeometry = (CompiledGeometry*)geometry;

	guiGeometry->vertices.release();
	guiGeometry->indices.release();
	guiGeometry->texture.release();

	delete guiGeometry;
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.
void GuiRenderer::EnableScissorRegion(bool enable)
{
	guiManager->getGraphicsDevice().setRenderState(D3DRS_SCISSORTESTENABLE, enable);
}

// Called by Rocket when it wants to change the scissor region.
void GuiRenderer::SetScissorRegion(int x, int y, int width, int height)
{
	RECT scissorRect;
	scissorRect.left = x;
	scissorRect.right = x + width;
	scissorRect.top = y;
	scissorRect.bottom = y + height;

	guiManager->getGraphicsDevice().setScissorRect(scissorRect);
}

// Called by Rocket when a texture is required by the library.
bool GuiRenderer::LoadTexture(Rocket::Core::TextureHandle& textureHandle, Rocket::Core::Vector2i& textureDimensions, const Rocket::Core::String& source)
{
	Rocket::Core::FileInterface* fileInterface = Rocket::Core::GetFileInterface();
	Rocket::Core::FileHandle fileHandle = fileInterface->Open(source);
	if (fileHandle == NULL)
		return false;

	fileInterface->Seek(fileHandle, 0, SEEK_END);
	size_t bufferSize = fileInterface->Tell(fileHandle);
	fileInterface->Seek(fileHandle, 0, SEEK_SET);
	
	char* buffer = new char[bufferSize];
	fileInterface->Read(buffer, bufferSize, fileHandle);
	fileInterface->Close(fileHandle);

	TGAHeader header;
	memcpy(&header, buffer, sizeof(TGAHeader));
	
	int colorMode = header.bitsPerPixel / 8;
	int imageSize = header.width * header.height * 4; // We always make 32bit textures 
	
	if (header.dataType != 2)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24/32bit uncompressed TGAs are supported.");
		return false;
	}
	
	// Ensure we have at least 3 colors
	if (colorMode < 3)
	{
		Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR, "Only 24 and 32bit textures are supported");
		return false;
	}
	
	const char* imageSrc = buffer + sizeof(TGAHeader);
	unsigned char* imageDest = new unsigned char[imageSize];
	
	// Targa is BGR, swap to RGB and flip Y axis
	for (long y = 0; y < header.height; ++y)
	{
		long readIndex = y * header.width * colorMode;
		long writeIndex = ((header.imageDescriptor & 32) != 0) ? readIndex : (header.height - y - 1) * header.width * colorMode;
		for (long x = 0; x < header.width; ++x)
		{
			imageDest[writeIndex] = imageSrc[readIndex+2];
			imageDest[writeIndex+1] = imageSrc[readIndex+1];
			imageDest[writeIndex+2] = imageSrc[readIndex];
			if (colorMode == 4)
				imageDest[writeIndex+3] = imageSrc[readIndex+3];
			else
				imageDest[writeIndex+3] = 255;
			
			writeIndex += 4;
			readIndex += colorMode;
		}
	}

	textureDimensions.x = header.width;
	textureDimensions.y = header.height;
	
	bool success = GenerateTexture(textureHandle, imageDest, textureDimensions);
	
	delete[] imageDest;
	delete[] buffer;
	
	return success;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool GuiRenderer::GenerateTexture(Rocket::Core::TextureHandle& textureHandle, const unsigned char* source, const Rocket::Core::Vector2i& sourceDimensions)
{
	// Create a Direct3DTexture9, which will be set as the texture handle. Note that we only create one surface for
	// this texture; because we're rendering in a 2D context, mip-maps are not required.
	IDirect3DTexture9* texture;
	guiManager->getGraphicsDevice().resource->CreateTexture(sourceDimensions.x, sourceDimensions.y, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, NULL);

	// Lock the top surface and write the pixel data onto it.
	D3DLOCKED_RECT lockedRect;
	texture->LockRect(0, &lockedRect, 0, 0);
	for (int y = 0; y < sourceDimensions.y; ++y)
	{
		for (int x = 0; x < sourceDimensions.x; ++x)
		{
			const unsigned char* sourcePixel = source + (sourceDimensions.x * 4 * y) + (x * 4);
			unsigned char* destinationPixel = ((unsigned char*) lockedRect.pBits) + lockedRect.Pitch * y + x * 4;

			destinationPixel[0] = sourcePixel[2];
			destinationPixel[1] = sourcePixel[1];
			destinationPixel[2] = sourcePixel[0];
			destinationPixel[3] = sourcePixel[3];
		}
	}
	texture->UnlockRect(0);

	// Set the handle on the Rocket texture structure.
	textureHandle = (Rocket::Core::TextureHandle)texture;
	return true;
}

// Called by Rocket when a loaded texture is no longer required
void GuiRenderer::ReleaseTexture(Rocket::Core::TextureHandle textureHandle)
{
	((IDirect3DTexture9*)textureHandle)->Release();
}

// Returns the native horizontal texel offset for the renderer.
float GuiRenderer::GetHorizontalTexelOffset()
{
	return -0.5f;
}

// Returns the native vertical texel offset for the renderer.
float GuiRenderer::GetVerticalTexelOffset()
{
	return -0.5f;
}

Vector2 GuiRenderer::convert(Rocket::Core::Vector2f vector)
{
	return Vector2(vector.x, vector.y);
}

Color GuiRenderer::convert(Rocket::Core::Colourb color)
{
	return Color((float)(color.red / UCHAR_MAX), (float)(color.green/ UCHAR_MAX), (float)(color.blue / UCHAR_MAX), (float)(color.alpha / UCHAR_MAX));
}