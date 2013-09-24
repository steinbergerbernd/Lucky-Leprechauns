#include "SoundManager.h"

void WINAPI XACTNotificationCallback(const XACT_NOTIFICATION* pNotification);

SoundManager::SoundManager(Game* game) : GameComponent(game), engine(0)
{
}


SoundManager::~SoundManager(void)
{
}


void SoundManager::init()
{
	listener.pCone = 0;
	listener.OrientFront = Vector3::forward;
	listener.OrientTop = Vector3::up;
	listener.Position = listener.Velocity = Vector3::zero;

	BasicManager::init();
}


void SoundManager::update(const GameTime& gameTime)
{
	BasicManager::update(gameTime);

	if (engine)
		engine->DoWork();
}


void SoundManager::release()
{
	if (engine)
	{
		engine->ShutDown();
		engine->Release();
	}

	BasicManager::release();

	if(waveBankMapping)
		UnmapViewOfFile(waveBankMapping);
	waveBankMapping = 0;

	CoUninitialize();
}


void SoundManager::onLostDevice()
{
	BasicManager::onLostDevice();
}


void SoundManager::onResetDevice(const GraphicsDevice& device)
{
	BasicManager::onResetDevice(device);
}


void SoundManager::loadConfig(const std::string& path)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	XACT3CreateEngine(0, &engine);

	void* file = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	unsigned long size = GetFileSize(file, 0);
	void* globalSettingsBuffer = CoTaskMemAlloc(size);
	unsigned long bytesRead;
	ReadFile(file, globalSettingsBuffer, size, &bytesRead, 0);
	CloseHandle(file);

	AudioParameters parameters = {0};
	parameters.pGlobalSettingsBuffer = globalSettingsBuffer;
	parameters.globalSettingsBufferSize = size;
	parameters.globalSettingsFlags = XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA;
	parameters.fnNotificationCallback = XACTNotificationCallback;
	parameters.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;

	engine->Initialize(&parameters);
	XACT3DInitialize(engine, audioHandle3D);
}


void SoundManager::loadWaveBank(const std::string& path)
{
	void* file = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	unsigned long size = GetFileSize(file, 0);
	void* mapFile = CreateFileMapping(file, 0, PAGE_READONLY, 0, size, 0);
	waveBankMapping = MapViewOfFile(mapFile, FILE_MAP_READ, 0, 0, 0);

	engine->CreateInMemoryWaveBank(waveBankMapping, size, 0, 0, &waveBank);

	CloseHandle(mapFile);
	CloseHandle(file);
}


void SoundManager::setListener(const Vector3& position, const Vector3& forward, const Vector3& up)
{
	listener.OrientFront = forward;
	listener.OrientTop = up;
	listener.Position = position;
}


void WINAPI XACTNotificationCallback(const XACT_NOTIFICATION* pNotification)
{
}