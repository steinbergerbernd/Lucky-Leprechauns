#pragma once
#include "GameComponent.h"
#include "BasicManager.h"

#include <xact3.h>
#include <xact3d3.h>

class SoundManager : public GameComponent, public BasicManager
{
public:
	typedef X3DAUDIO_LISTENER AudioListener;
	typedef IXACT3Engine AudioEngine;
	typedef X3DAUDIO_HANDLE AudioHandle3D;
	typedef IXACT3WaveBank WaveBank;
	typedef XACT_RUNTIME_PARAMETERS AudioParameters;

	SoundManager(Game* game);
	virtual ~SoundManager(void);

	virtual void init();
	virtual void update(const GameTime& gameTime);
	virtual void release();

	virtual void onLostDevice();
	virtual void onResetDevice(const GraphicsDevice& device);

	AudioEngine& getEngine() { return *engine; }
	const AudioListener& getListener() const { return listener; }
	AudioHandle3D& getAudioHandle3D() { return audioHandle3D; }

	void setListener(const Vector3& position, const Vector3& forward, const Vector3& up);
protected:
	void loadConfig(const std::string& path);
	void loadWaveBank(const std::string& path);

private:
	AudioEngine* engine;
	
	AudioHandle3D audioHandle3D;
	AudioListener listener;

	void* waveBankMapping;
	WaveBank* waveBank;
};

