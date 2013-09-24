#pragma once

#include "STL.h"
#include "Mesh.h"
#include "VertexTypes.h"
#include "GraphicsDevice.h"
#include "Camera.h"
#include "GraphicsTypes.h"

#include "FBXVertex.h"
#include "FBXBone.h"
#include "FBXMesh.h"
#include "FBXEffect.h"
#include "FBXInstance.h"
#include "FBXAnimation.h"

#include <fbxsdk.h>

class FBXModel
{
public:
	typedef std::vector<Matrix> MatrixCollection;
	typedef std::vector<FBXVertex> VertexCollection;
	typedef std::vector<FBXMaterial> MaterialCollection;

	typedef std::map<unsigned, MatrixCollection> BoneMatricesMap;

	typedef std::map<std::string, FBXBone> BoneMap;
	typedef std::map<std::string, FBXAnimation> AnimationMap;
	typedef std::map<std::string, FBXMesh> MeshMap;

	typedef Matrix InstanceType;

	FBXModel();

	static FBXModel createFromFile(const GraphicsDevice& device, const std::string& filename, unsigned keyframes = 0);

	void setAnimation(const std::string& name, bool force = false);
	void addAnimation(const std::string& name, const FBXAnimation& animation);

	FBXAnimation& getAnimation(const std::string& name) { return animations[name]; }

	void draw(const GraphicsDevice& device, const GameTime& gameTime, const Matrix& world, const Matrix& view, const Matrix& projection, bool shadowMap);
	void draw(const GraphicsDevice& device, const GameTime& gameTime, const Matrix& world, const Matrix& view, const Matrix& projection, const std::vector<Matrix>& instanceTransforms, unsigned numInstances, bool shadowMap);
	void draw(const GraphicsDevice& device, const GameTime& gameTime, const Matrix& world, const Matrix& view, const Matrix& projection, FBXEffect& effect, bool shadowMap);
	void draw(const GraphicsDevice& device, const GameTime& gameTime, const Matrix& world, const Matrix& view, const Matrix& projection, const std::vector<Matrix>& instanceTransforms, unsigned numInstances, FBXEffect& effect, bool shadowMap);

	void setCamera(const Camera& camera) { this->camera = camera; }
	void setLight(const Light& light) { this->light = light; }
	void setClipPlane(const Plane& clipPlane) { this->clipPlane = clipPlane; }
	void setAlphaCutoff(float alphaCutoff) { this->alphaCutoff = alphaCutoff; }

	MeshMap& getMeshes() { return meshes; }
	const MeshMap& getMeshes() const { return meshes; }

	void onResetDevice(const GraphicsDevice& device);
	void onLostDevice();

	bool isLoaded() const { return loaded; }

	void load(const GraphicsDevice& device, const std::string& filename, unsigned keyframes);
	void unload();

	void setWorldMatrix(const std::string& name, const Matrix& worldMatrix);
	void setSkinning(const std::string& name, bool skinning);

	const std::string getAnimationName() const;

	void setKeyColorEnabled(bool keyColorEnabled) { this->keyColorEnabled = keyColorEnabled; }
	void setKeyColor(const Vector3& keyColor) { this->keyColor = keyColor; }
	void setKeyColorReplace(const Vector3& keyColorReplace) { this->keyColorReplace = keyColorReplace; }

private:
	bool loaded;

	Camera camera;
	Light light;
	Plane clipPlane;
	float alphaCutoff;

	Vector3 keyColor;
	Vector3 keyColorReplace;
	bool keyColorEnabled;

	static FBXEffect effect;

	Texture defaultTexture;

	MeshMap meshes;

	FBXAnimation* animationCurrent;
	FBXAnimation* animationNext;
	float animationStart;
	unsigned animationFrame;
	
	BoneMap bones;
	BoneMatricesMap boneMatricesMap;
	AnimationMap animations;

	VertexDeclaration vertexDeclaration;

	static VertexBuffer instanceVertexBuffer;
	static const unsigned maxInstances;
	static const char* defaultTexturePath;

	MatrixCollection traverseBones(unsigned frame, KFbxNode* node, const Matrix& parent, MatrixCollection& boneMatrices);
	
	void loadBones(KFbxNode* node, KFbxNode** rootBone, KFbxAnimLayer* animLayer);
	void loadMeshes(KFbxNode* node, const GraphicsDevice& device, KFbxGeometryConverter& converter);
	
	MaterialCollection getMaterials(const GraphicsDevice& device, KFbxNode* node);
	VertexCollection getVertices(KFbxMesh* mesh);

	unsigned getAnimationFrame(const GameTime& gameTime);

	const MatrixCollection& getBoneMatrices(unsigned frame) const;

	static Matrix convert(const KFbxXMatrix& matrix);
	static Vector4 convert(const KFbxVector4& vector);
	static Vector2 convert(const KFbxVector2& vector);
	static D3DCOLORVALUE convert(const fbxDouble3& vector, float a = 1.0f);
};
