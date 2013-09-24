#pragma once

#include "Rocket/Core.h"
#include "Rocket/Core/RenderInterface.h"
#include "Rocket/Core/FileInterface.h"
#include "GraphicsManager.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "VertexTypes.h"

class GuiManager;

class GuiRenderer : public Rocket::Core::RenderInterface
{
	typedef VertexPositionColorTexture GuiVertex;

	// This structure is created for each set of geometry that Rocket compiles. It stores the vertex and index buffers and
	// the texture associated with the geometry, if one was specified.
	struct CompiledGeometry
	{
		VertexBuffer vertices;
		unsigned long numVertices;

		IndexBuffer indices;
		unsigned long numPrimitives;

		Texture texture;
	};

	// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
	#pragma pack(1) 
	struct TGAHeader 
	{
		char idLength;
		char colourMapType;
		char dataType;
		short int colourMapOrigin;
		short int colourMapLength;
		char colourMapDepth;
		short int xOrigin;
		short int yOrigin;
		short int width;
		short int height;
		char bitsPerPixel;
		char imageDescriptor;
	};
	// Restore packing
	#pragma pack()

public:
	GuiRenderer(GuiManager* guiManager);
	~GuiRenderer(void);

	/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
	virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int numVertices,
		int* indices, int numIndices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);

	/// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
	virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices,
		int numVertices, int* indices, int numIndices, Rocket::Core::TextureHandle texture);

	/// Called by Rocket when it wants to render application-compiled geometry.
	virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation);
	/// Called by Rocket when it wants to release application-compiled geometry.
	virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

	/// Called by Rocket when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion(bool enable);
	/// Called by Rocket when it wants to change the scissor region.
	virtual void SetScissorRegion(int x, int y, int width, int height);

	/// Called by Rocket when a texture is required by the library.
	virtual bool LoadTexture(Rocket::Core::TextureHandle& textureHandle,
		Rocket::Core::Vector2i& textureDimensions, const Rocket::Core::String& source);
	/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
	virtual bool GenerateTexture(Rocket::Core::TextureHandle& textureHandle, const unsigned char* source,
		const Rocket::Core::Vector2i& sourceDimensions);
	/// Called by Rocket when a loaded texture is no longer required.
	virtual void ReleaseTexture(Rocket::Core::TextureHandle textureHandle);

	/// Returns the native horizontal texel offset for the renderer.
	float GetHorizontalTexelOffset();
	/// Returns the native vertical texel offset for the renderer.
	float GetVerticalTexelOffset();
private:
	GuiManager* guiManager;

	static Vector2 convert(Rocket::Core::Vector2f);
	static Color convert(Rocket::Core::Colourb);
};

