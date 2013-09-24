#include "GuiManager.h"
#include "GuiComponent.h"

GuiManager::GuiManager(Game* game) : GameComponent(game), guiRenderer(this), context(0), deviceWasLost(false), visible(true)
{
}


GuiManager::~GuiManager(void)
{
}


void GuiManager::init()
{
	Rocket::Core::SetSystemInterface(&guiSystem);
	Rocket::Core::SetRenderInterface(&guiRenderer);
	Rocket::Core::Initialise();

	initContext();
	loadFonts();

	BasicManager::init();
}


void GuiManager::initContext()
{
	Viewport viewport = getGraphicsDevice().getViewport();
	context = Rocket::Core::CreateContext("default", Rocket::Core::Vector2i(viewport.Width, viewport.Height));

	Rocket::Core::Vector2i dimension = context->GetDimensions();
	D3DXMatrixOrthoOffCenterLH(&projection, 0, (float)dimension.x, (float)dimension.y, 0, -1, 1);
}

void GuiManager::loadFonts()
{
}


void GuiManager::loadFont(const std::string& path)
{
	Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String(path.c_str()));
}


Rocket::Core::Context& GuiManager::getContext() const
{
	return *context;
}


void GuiManager::draw(const GameTime& gameTime)
{
	if (!visible)
		return;

	GraphicsDevice device = getGraphicsDevice();

	StateBlock state = device.createStateBlock();

	device.setProjection(projection);
	device.setView(Matrix::identity);

	device.setRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	device.setRenderState(D3DRS_ALPHABLENDENABLE, true);
	device.setRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device.setRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device.setRenderState(D3DRS_LIGHTING, FALSE);

	for (ComponentCollection::iterator it = components.begin(); it != components.end(); ++it)
		if ((*it)->isEnabled())
			((GuiComponent*)(*it))->draw(gameTime);

	context->Render();

	state.apply();
}


void GuiManager::update(const GameTime& gameTime)
{
	BasicManager::update(gameTime);

	context->Update();
}


void GuiManager::release()
{
	Rocket::Core::Shutdown();
}

void GuiManager::onLostDevice()
{
	BasicManager::onLostDevice();
	context->RemoveReference();
	deviceWasLost = true;
}


void GuiManager::onResetDevice(const GraphicsDevice& device)
{
	if(deviceWasLost)
	{
		initContext();

		BasicManager::onResetDevice(device);

		deviceWasLost = false;
	}
}