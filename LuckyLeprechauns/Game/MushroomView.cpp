#include "MushroomView.h"


MushroomView::MushroomView(LLGraphicsManager* manager, const MushroomController* controller) : LLControlledGraphicsComponent(manager, controller)
{
	stone = manager->addComponent<StoneView>(manager, &controller->getStone());
}


void MushroomView::init()
{
	model = FBXModelLoader::getModelClone("Mushroom");
	neutralTexture = model.getMeshes()["Mushroom"].materials[0].texture;

	unsigned maxPlayerCount = Config::getValue<unsigned>(LLConfigKeys::gameMaxPlayers);
	std::string texturePath = Config::getValue(LLConfigKeys::mushroomTexturePath);
	std::string textureDataType = Config::getValue(LLConfigKeys::mushroomTextureDataType);

	for(unsigned i = 0; i < maxPlayerCount; ++i)
		playerTextures.push_back(Texture::createFromFile(getManager().getDevice(), texturePath + Converter::convert<std::string>(i) + textureDataType));
}


void MushroomView::draw(const GameTime& gameTime)
{
	const MushroomController& controller = getController();

	if (!controller.isEnabled())
		return;

	std::string animation;

	if (controller.isAttacking())
		animation = AnimationKeys::mushroomAttack;
	else if (controller.isMoving())
		animation = AnimationKeys::mushroomRun;
	else
		animation = AnimationKeys::mushroomIdle;

	model.setAnimation(animation);

	FBXModel::MeshMap& meshes = model.getMeshes();

	Texture texture = neutralTexture;

	if (controller.isOwned())
		texture = playerTextures[controller.getOwner()->getPlayerId()];

	meshes["Mushroom"].materials[0].texture = texture;
	meshes["Stone"].visible = !controller.getStone().isEnabled();

	FBXRenderer::draw(model, gameTime, getManager(), controller.getWorld());
}