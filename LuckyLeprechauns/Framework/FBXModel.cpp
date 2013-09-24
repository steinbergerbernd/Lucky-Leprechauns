#include "FBXModel.h"

#undef min
#undef max

const char* FBXModel::defaultTexturePath = "Content/Textures/FBX/Default.png";
const unsigned FBXModel::maxInstances = 1024;
VertexBuffer FBXModel::instanceVertexBuffer;

FBXEffect FBXModel::effect;


FBXModel::FBXModel() : animationCurrent(0), animationNext(0), animationStart(0), loaded(false)
{
}


FBXModel FBXModel::createFromFile(const GraphicsDevice& device, const std::string& filename, unsigned keyframes)
{
	FBXModel model;

	model.load(device, filename, keyframes);

	return model;
}


void FBXModel::draw(const GraphicsDevice& device, const GameTime& gameTime, const Matrix& world, const Matrix& view, const Matrix& projection, bool shadowMap)
{
	draw(device, gameTime, world, view, projection, std::vector<Matrix>(1, Matrix::identity), 1, this->effect, shadowMap);
}


void FBXModel::draw(const GraphicsDevice& device, const GameTime& gameTime, const Matrix& world, const Matrix& view, const Matrix& projection, FBXEffect& effect, bool shadowMap)
{
	draw(device, gameTime, world, view, projection, std::vector<Matrix>(1, Matrix::identity), 1, effect, shadowMap);
}


void FBXModel::draw(const GraphicsDevice& device, const GameTime& gameTime, const Matrix& world, const Matrix& view, const Matrix& projection, const std::vector<Matrix>& instanceTransforms, unsigned numInstances, bool shadowMap)
{
	draw(device, gameTime, world, view, projection, instanceTransforms, numInstances, effect, shadowMap);
}


void FBXModel::draw(const GraphicsDevice& device, const GameTime& gameTime, const Matrix& world, const Matrix& view, const Matrix& projection, const std::vector<Matrix>& instanceTransforms, unsigned numInstances, FBXEffect& effect, bool shadowMap)
{
	if (numInstances == 0)
		return;

	instanceVertexBuffer.setData(&instanceTransforms[0], numInstances, D3DLOCK_DISCARD);

	if (!animations.empty())
		effect.setBoneMatrices(getBoneMatrices(getAnimationFrame(gameTime)));

	effect.setTechnique(!animations.empty(), shadowMap);

	effect.setCamera(camera);
	effect.setLight(light);
	effect.setClipPlane(clipPlane);
	effect.setAlphaCutoff(alphaCutoff);

	effect.setWorld(world);
	effect.setView(view);
	effect.setProjection(projection);

	device.setVertexDeclaration(vertexDeclaration);

	device.setStreamSource(instanceVertexBuffer, sizeof(InstanceType), 1);
	device.setStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	effect.beginSinglePass();

	for (MeshMap::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		FBXMesh& fbxMesh = it->second;

		if (!fbxMesh.visible)
			continue;

		Mesh& mesh = fbxMesh.mesh;

		device.setIndices(mesh.getIndexBuffer());

		device.setStreamSource(mesh.getVertexBuffer(), sizeof(FBXVertex));
		device.setStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | numInstances);

		unsigned numMaterials = fbxMesh.materials.size();

		Mesh::AttributeTable attributeTable = mesh.getAttributeTable(numMaterials);

		for (unsigned j = 0; j < numMaterials; ++j)
		{
			FBXMaterial& fbxMaterial = fbxMesh.materials[j];

			Material material = fbxMaterial.material;

			if (Vector3(material.Diffuse) == keyColor)
				material.Diffuse = Vector4((keyColorEnabled) ? keyColorReplace : keyColor);

			if (Vector3(material.Ambient) == keyColor)
				material.Ambient = Vector4((keyColorEnabled) ? keyColorReplace : keyColor);

			switch (fbxMesh.colorFunction)
			{
				case FBXColorFunction::colorMultiply:
					{
						material.Ambient = Vector4(material.Ambient) * Vector4(fbxMesh.color);
						material.Diffuse = Vector4(material.Diffuse) * Vector4(fbxMesh.color);
						effect.setColormap(fbxMaterial.texture);
					}
					break;

				case FBXColorFunction::colorOverride:
					{
						material.Ambient = fbxMesh.color;
						material.Diffuse = fbxMesh.color;
						effect.setColormap(defaultTexture);
					}
					break;

				case FBXColorFunction::colorAdd:
					{
						material.Ambient = Vector4(Vector3(fbxMesh.color + Vector3(material.Ambient)));
						material.Diffuse = Vector4(Vector3(fbxMesh.color + Vector3(material.Diffuse)));
						effect.setColormap(fbxMaterial.texture);
					}
					break;
			}

			effect.setMaterial(material);
			effect.commitChanges();

			D3DXATTRIBUTERANGE& range = attributeTable[j];

			device.drawIndexedPrimitive(D3DPT_TRIANGLELIST, range.FaceCount, range.VertexCount, 0, range.VertexStart, range.FaceStart * 3);
		}
	}

	effect.endSinglePass();
}


const std::string FBXModel::getAnimationName() const
{
	return animationCurrent ? animationCurrent->name : "";
}


void FBXModel::setAnimation(const std::string& name, bool force)
{
	FBXAnimation& animation = animations[name];

	if (force || animationNext == animationCurrent || animation.priority >= animationNext->priority)
	{
		animationNext = &animation;

		if (force)
		{
			animationStart = 0;
			animationCurrent = 0;
		}
	}
}


void FBXModel::addAnimation(const std::string& name, const FBXAnimation& animation)
{
	animations[name] = animation;
}


unsigned FBXModel::getAnimationFrame(const GameTime& gameTime)
{
	if (!animationNext)
		return 0;

	if (!animationCurrent)
		animationCurrent = animationNext;

	if (!animationStart)
		animationStart = gameTime.total;

	// duration in seconds of current animation
	float duration = (float)animationCurrent->length / (float)animationCurrent->currentFps;

	// time difference between duration of animation and time passed
	float delta = gameTime.total - animationStart - duration;

	// if animation is over or next animation has equal or higher priority
	if (delta > 0 || animationNext->priority >= animationCurrent->priority)
	{
		// if next animation is new animation -> set start time to current time
		if (animationCurrent != animationNext)
			animationStart = gameTime.total;

		// switch to next animation (can be the same animation)
		animationCurrent = animationNext;
	}

	FBXAnimation& animation = *animationCurrent;

	// calculate current animation key frame
	return (unsigned)((gameTime.total - animationStart) * animation.currentFps) % animation.length + animation.start;
}


void FBXModel::load(const GraphicsDevice& device, const std::string& filename, unsigned keyframes)
{
	if (effect.resource == 0)
		effect = Effect::createFromFile<FBXEffect>(device, Config::getValue(ConfigKeys::fbxEffectPath));

	defaultTexture = Texture::createFromFile(device, defaultTexturePath);
	
	vertexDeclaration = device.createVertexDeclaration(FBXInstance::vertexElements);

	KFbxSdkManager* sdkManager = KFbxSdkManager::Create();
	KFbxIOSettings* ios = KFbxIOSettings::Create(sdkManager, IOSROOT);
	sdkManager->SetIOSettings(ios);

	// Create an importer using our sdk manager.
	KFbxImporter* importer = KFbxImporter::Create(sdkManager, "");

	importer->Initialize(filename.c_str(), -1, sdkManager->GetIOSettings());

	// Create a new scene so it can be populated by the imported file.
	KFbxScene* scene = KFbxScene::Create(sdkManager, "");

	// Import the contents of the file into the scene.
	importer->Import(scene);

	KFbxNode* rootBone = 0;
	KFbxNode* rootNode = scene->GetRootNode();

	KFbxAnimStack* animStack = KFbxCast<KFbxAnimStack>(scene->GetSrcObject(FBX_TYPE(KFbxAnimStack), 0));
	KFbxAnimLayer* animLayer = 0;

	if (animStack)
	{
		animLayer = animStack->GetMember(FBX_TYPE(KFbxAnimLayer), 0);
		scene->GetEvaluator()->SetContext(animStack);
	}

	loadBones(rootNode, &rootBone, animLayer);
	loadMeshes(rootNode, device, KFbxGeometryConverter(sdkManager));

	if (animLayer)
	{
		for (unsigned i = 0; i <= keyframes; ++i)
			boneMatricesMap[i] = traverseBones(i, rootBone, Matrix::identity, MatrixCollection(bones.size()));
	}

	sdkManager->Destroy();

	loaded = true;
}


void FBXModel::unload()
{
	effect.release();
	defaultTexture.release();

	for (MeshMap::iterator itMesh = meshes.begin(); itMesh != meshes.end(); ++itMesh)
	{
		FBXMesh& fbxMesh = itMesh->second;

		fbxMesh.mesh.release();

		for (MaterialCollection::iterator itMaterial = fbxMesh.materials.begin(); itMaterial != fbxMesh.materials.end(); ++itMaterial)
			itMaterial->texture.release();
	}

	instanceVertexBuffer.release();
	vertexDeclaration.release();
}


void FBXModel::loadBones(KFbxNode* node, KFbxNode** rootBone, KFbxAnimLayer* animLayer)
{
	const char* name = node->GetName();

	if (node->GetNodeAttribute())
	{
		KFbxNodeAttribute::EAttributeType attributeType = node->GetNodeAttribute()->GetAttributeType();

		if(attributeType == KFbxNodeAttribute::eSKELETON)
		{
			if (!animLayer)
				return;

			if (*rootBone == 0)
				*rootBone = node;

			Matrix globalTransform = convert(node->EvaluateGlobalTransform());

			FBXBone bone;

			bone.index = bones.size();

			bone.rotationCurves[0] = node->LclRotation.GetCurve<KFbxAnimCurve>(animLayer, KFCURVENODE_T_X);
			bone.rotationCurves[1] = node->LclRotation.GetCurve<KFbxAnimCurve>(animLayer, KFCURVENODE_T_Y);
			bone.rotationCurves[2] = node->LclRotation.GetCurve<KFbxAnimCurve>(animLayer, KFCURVENODE_T_Z);

			bone.translationCurves[0] = node->LclTranslation.GetCurve<KFbxAnimCurve>(animLayer, KFCURVENODE_T_X);
			bone.translationCurves[1] = node->LclTranslation.GetCurve<KFbxAnimCurve>(animLayer, KFCURVENODE_T_Y);
			bone.translationCurves[2] = node->LclTranslation.GetCurve<KFbxAnimCurve>(animLayer, KFCURVENODE_T_Z);

			fbxDouble3 translation = node->LclTranslation.Get();
			fbxDouble3 rotation = node->LclRotation.Get();

			bone.translation = Vector3((float)translation[0], (float)translation[1], (float)translation[2]);
			bone.rotation = Vector3((float)rotation[0], (float)rotation[1], (float)rotation[2]);

			bone.globalTransform = globalTransform;
			bone.globalTransformInverse = Matrix::invert(globalTransform);

			bones[node->GetName()] = bone;
		}
	}

	for(int i = 0; i < node->GetChildCount(); i++)
		loadBones(node->GetChild(i), rootBone, animLayer);
}


void FBXModel::loadMeshes(KFbxNode* node, const GraphicsDevice& device, KFbxGeometryConverter& converter)
{
	const char* name = node->GetName();

	if (node->GetNodeAttribute())
	{
		KFbxNodeAttribute::EAttributeType attributeType = node->GetNodeAttribute()->GetAttributeType();

		if (attributeType == KFbxNodeAttribute::eMESH)
		{
			KFbxMesh* kfbxMesh = converter.TriangulateMesh((KFbxMesh*)node->GetNodeAttribute());

			VertexCollection vertices = getVertices(kfbxMesh);
			MaterialCollection materials = getMaterials(device, node);

			unsigned long numFaces = kfbxMesh->GetPolygonCount();
			unsigned long numVertices = kfbxMesh->GetControlPointsCount();
				
			Mesh mesh = Mesh::create(device, numFaces, numVertices, FBXVertex::vertexElements, D3DXMESH_MANAGED | D3DXMESH_32BIT);

			mesh.getVertexBuffer().setData(vertices);
			mesh.getIndexBuffer().setData(kfbxMesh->GetPolygonVertices(), kfbxMesh->GetPolygonVertexCount());

			KFbxLayerElementArrayTemplate<int>* materialIndices;
			kfbxMesh->GetMaterialIndices(&materialIndices);

			unsigned long* buffer = mesh.lockAttributeBuffer();

			for(int i = 0; i < kfbxMesh->GetPolygonCount(); ++i)
				buffer[i] = materialIndices->GetAt(i);
			
			mesh.unlockAttributeBuffer();
				
			Mesh::Adjacency adjacency = mesh.generateAdjacency();

			mesh.clean(D3DXCLEAN_SIMPLIFICATION, adjacency);
			mesh.optimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, adjacency);
			mesh.computeNormals(adjacency);
			

			meshes[name] = FBXMesh(mesh, materials, name);
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++)
		loadMeshes(node->GetChild(i), device, converter);
}


const FBXModel::MatrixCollection& FBXModel::getBoneMatrices(unsigned frame) const
{	
	return boneMatricesMap.find(frame)->second;
}


FBXModel::VertexCollection FBXModel::getVertices(KFbxMesh* mesh)
{
	const KFbxLayer* layer = mesh->GetLayer(0);
	
	const KFbxLayerElementNormal* normals = layer->GetNormals();
	const KFbxLayerElementUV* texCoord = layer->GetUVs();

	VertexCollection vertices = VertexCollection(mesh->GetControlPointsCount());

	// positions
	for (unsigned i = 0; i < vertices.size(); i++)
		vertices[i].position = convert(mesh->GetControlPointAt(i));

	// normals
	//if (normals)
	//{
	//	for (unsigned i = 0; i < vertices.size(); i++)
	//		vertices[i].normal = Vector3::normalize(convert(normals->GetDirectArray().GetAt(i)));
	//}

	// UV
	if (texCoord)
	{
		for (int i = 0; i < mesh->GetPolygonCount(); ++i)
			for (int j = 0; j < mesh->GetPolygonSize(i); ++j)
				vertices[mesh->GetPolygonVertex(i, j)].texCoord = convert(texCoord->GetDirectArray().GetAt(mesh->GetTextureUVIndex(i,j)));
	}
	
	// skin weights
	std::vector<int> blendCount(vertices.size(), 0);

	for (int i = 0; i < mesh->GetDeformerCount(KFbxDeformer::eSKIN); ++i)
	{
		KFbxSkin* skin = (KFbxSkin*)mesh->GetDeformer(i, KFbxDeformer::eSKIN);

		for (int j = 0; j < skin->GetClusterCount(); ++j)
		{
			KFbxCluster* cluster = skin->GetCluster(j);
			unsigned boneIndex = bones[cluster->GetLink()->GetName()].index;

			int* vertexIndices = cluster->GetControlPointIndices();
			double* vertexBlendWeights = cluster->GetControlPointWeights();

			for (int k = 0; k < cluster->GetControlPointIndicesCount(); ++k)
			{
				int index = vertexIndices[k];
				int blendIndex = blendCount[index]++;

				vertices[index].blendWeights[blendIndex] = (float)vertexBlendWeights[k];
				vertices[index].blendIndices[blendIndex] = (unsigned char)boneIndex;
			}
		}
	}

	return vertices;
}


FBXModel::MaterialCollection FBXModel::getMaterials(const GraphicsDevice& device, KFbxNode* node)
{		
	MaterialCollection materials;

	for (int i = 0; i < node->GetMaterialCount(); ++i)
	{
		FBXMaterial material;

		KFbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);

		KFbxProperty prop = surfaceMaterial->FindProperty(KFbxSurfaceMaterial::sDiffuse);

		if	(prop.IsValid())
		{
			int textureCount = prop.GetSrcObjectCount(KFbxFileTexture::ClassId);

			if (textureCount)
			{
				KFbxFileTexture* fbxTexture = prop.GetSrcObject(FBX_TYPE(KFbxFileTexture), 0);

				if (fbxTexture)
					material.texture = Texture::createFromFile(device, fbxTexture->GetFileName());
			}
		}

		if (!material.texture)
			material.texture = defaultTexture;
						
		if	(surfaceMaterial->GetClassId().Is(KFbxSurfacePhong::ClassId))
		{
			Material& mat = material.material;

			KFbxSurfacePhong* phong = (KFbxSurfacePhong*)surfaceMaterial;

			mat.Ambient = convert(phong->Diffuse.Get());
			mat.Diffuse = mat.Ambient;
			mat.Specular = convert(phong->Specular.Get());
			mat.Emissive = convert(phong->Emissive.Get());
			mat.Power = (float)phong->SpecularFactor.Get();
		}

		materials.push_back(material);
	}

	return materials;
}


FBXModel::MatrixCollection FBXModel::traverseBones(unsigned frame, KFbxNode* node, const Matrix& parent, MatrixCollection& boneMatrices)
{
	FBXBone& bone = bones[node->GetName()];

	Vector3 rotation, translation;

	for (int i = 0; i < 3; ++i)
	{
		rotation[i] = D3DXToRadian((bone.rotationCurves[i] != 0) ? bone.rotationCurves[i]->KeyGetValue(frame) : bone.rotation[i]);
		translation[i] = (bone.translationCurves[i] != 0) ? bone.translationCurves[i]->KeyGetValue(frame) : bone.translation[i];
	}

	Matrix localRotation =
		Matrix::createFromAxisAngle(Vector3::right, rotation.x) *
		Matrix::createFromAxisAngle(Vector3::up, rotation.y) *
		Matrix::createFromAxisAngle(Vector3::forward, rotation.z);

	Matrix localTranslation = Matrix::createTranslation(translation - bone.translation);

	boneMatrices[bone.index] = bone.globalTransformInverse * localRotation * bone.globalTransform * localTranslation * parent;

	for (int i = 0; i < node->GetChildCount(); ++i)
		traverseBones(frame, node->GetChild(i), boneMatrices[bone.index], boneMatrices);

	return boneMatrices;
}


Matrix FBXModel::convert(const KFbxXMatrix& matrix)
{
	Matrix m;

	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col)
			m[row * 4 + col] = (float)matrix.Get(row, col);

	return m;
}


Vector4 FBXModel::convert(const KFbxVector4& vector)
{
	return Vector4((float)vector.GetAt(0), (float)vector.GetAt(1), (float)vector.GetAt(2), (float)vector.GetAt(3));
}


D3DCOLORVALUE FBXModel::convert(const fbxDouble3& vector, float a)
{
	return Vector4((float)vector[0], (float)vector[1], (float)vector[2], a);
}


Vector2 FBXModel::convert(const KFbxVector2& vector)
{
	return Vector2((float)vector.GetAt(0), (float)vector.GetAt(1));
}


void FBXModel::onResetDevice(const GraphicsDevice& device)
{
	instanceVertexBuffer = device.createVertexBuffer(sizeof(InstanceType) * maxInstances, 0, D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);
	effect.onResetDevice();
}


void FBXModel::onLostDevice()
{
	instanceVertexBuffer.release();
	effect.onLostDevice();
}