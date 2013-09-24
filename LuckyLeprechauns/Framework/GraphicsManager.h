#pragma once

#include "Game.h"
#include "BasicComponent.h"
#include "BasicManager.h"

#include "Graphics.h"
#include "Camera.h"

#include "FBXModelLoader.h"
#include "FBXRenderer.h"

class GraphicsComponent;

class GraphicsManager : public BasicComponent<Game>, public BasicManager
{
public:
	GraphicsManager(Game* game);
	virtual ~GraphicsManager();

	virtual void init();
	virtual void release();
	virtual void update(const GameTime& gameTime);
	virtual void draw(const GameTime& gameTime);

	void GraphicsManager::draw(const GameTime& gameTime, ComponentCollection& components);

	virtual void onLostDevice() { BasicManager::onLostDevice(); }
	virtual void onResetDevice(const GraphicsDevice& device) { BasicManager::onResetDevice(device); }

	const GraphicsDevice& getDevice() const { return device; }
	void setDevice(const GraphicsDevice& device) { this->device = device; }

	Camera& getCamera() { return getManager().getCamera(); }
	const Camera& getCamera() const { return getManager().getCamera(); }

	const Matrix& getWorld() const { return world; }
	const Matrix& getView() const { return (renderShadowMap) ? lightView : view; }
	const Matrix& getProjection() const { return (renderShadowMap) ? lightProjection : projection; }

	void setWorld(const Matrix& world) { this->world = world; }
	void setView(const Matrix& view) { this->view = view; }
	void setProjection(const Matrix& projection) { this->projection = projection; }

	void setClipPlane(const Plane& clipPlane) { this->clipPlane = clipPlane; }
	const Plane& getClipPlane() const { return clipPlane; }

	void setLight(const Light& light) { this->light = light; }
	const Light& getLight() const { return light; }

	Matrix getViewProjection() const { return getView() * getProjection(); }
	Matrix getWorldViewProjection() const { return world * getViewProjection(); }

	Texture& getShadowMap() { return shadowMap; }
	const Texture& getShadowMap() const { return shadowMap; }

	Matrix getLightViewProjection() const { return lightView * lightProjection; }

	void setLightView(const Matrix& lightView) { this->lightView = lightView; }
	void setLightProjection(const Matrix& lightProjection) { this->lightProjection = lightProjection; }

	bool isShadowMapEnabled() const { return shadowMapEnabled; }
	bool isRenderShadowMap() const { return renderShadowMap; }

	void setShadowMap(const Texture& shadowMap) { this->shadowMap = shadowMap; }
	void setShadowMapEnabled(bool shadowMapEnabled) { this->shadowMapEnabled = shadowMapEnabled; }
	void setRenderShadowMap(bool renderShadowMap) { this->renderShadowMap = renderShadowMap; }

private:
	GraphicsDevice device;

	Matrix world;
	Matrix view;
	Matrix projection;

	Plane clipPlane;
	Light light;

	Matrix lightView;
	Matrix lightProjection;

	Texture shadowMap;
	bool shadowMapEnabled;
	bool renderShadowMap;
};