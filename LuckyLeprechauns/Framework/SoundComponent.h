#pragma once
#include "BasicComponent.h"
#include "ControlledComponent.h"
#include "SoundManager.h"

#define INPUTCHANNELS 2
#define OUTPUTCHANNELS 8

template <class T>
class SoundComponent : public BasicComponent<SoundManager>, public ControlledComponent<T>
{
public:
	typedef IXACT3SoundBank SoundBank;
	typedef X3DAUDIO_EMITTER AudioEmitter;
	typedef X3DAUDIO_DSP_SETTINGS DSPSettings;
	typedef IXACT3Cue Cue;

	SoundComponent(SoundManager* manager, T* controller);
	virtual ~SoundComponent(void) = 0;

	virtual void init();
	virtual void update(const GameTime& gameTime);
	virtual void release();

	void initEmitter(AudioEmitter& emitter);
protected:
	void loadSoundBank(const std::string& path);

	void playSound(const char* name);
	void play3DSound(const char* name);
	void playStatic3DSound(const char* name, const Vector3& position);

	void stopSound(const char* name);

	void setEmitter(const Vector3& position, const Vector3& forward, const Vector3& up);
private:
	struct Cue3D
	{
		bool active;
		bool useStaticEmitter;
		AudioEmitter staticEmitter;
		Cue* cue;
	};

	typedef std::vector<Cue3D> Cue3DList;

	Cue3DList cue3DList;

	void* soundBankMemory;
	SoundBank* soundBank;
	
	AudioEmitter emitter;
	DSPSettings dspSettings;

	float delayTimes[1];
	float matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	unsigned frameToApply3DAudio;

	void play3DSound(const char* name, bool isStatic, const Vector3& staticPosition);
	void update3DCue(Cue3D& cue3D);
};

template <class T>
SoundComponent<T>::SoundComponent(SoundManager* manager, T* controller) 
	: BasicComponent(manager), ControlledComponent(controller), frameToApply3DAudio(0)
{
}

template <class T>
SoundComponent<T>::~SoundComponent()
{
}


template <class T>
void SoundComponent<T>::init()
{
	initEmitter(emitter);

	WAVEFORMATEXTENSIBLE wfxFinalMixFormat;
	getManager().getEngine().GetFinalMixFormat(&wfxFinalMixFormat);

	delayTimes[0] = 0;
	ZeroMemory(&matrixCoefficients, sizeof(float) * INPUTCHANNELS * OUTPUTCHANNELS);
	
	ZeroMemory(&dspSettings, sizeof(DSPSettings));
	dspSettings.pMatrixCoefficients = matrixCoefficients;
	dspSettings.pDelayTimes = delayTimes;
	dspSettings.SrcChannelCount = INPUTCHANNELS;
	dspSettings.DstChannelCount = wfxFinalMixFormat.Format.nChannels;
}


template <class T>
void SoundComponent<T>::initEmitter(AudioEmitter& e)
{
	e.pCone = 0;
	e.OrientFront = Vector3::forward;
	e.OrientTop = Vector3::up;
	e.Position = emitter.Velocity = Vector3::zero;
	e.ChannelCount = INPUTCHANNELS;
	e.ChannelRadius = 1.0f;
	e.pChannelAzimuths = 0;
	e.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	e.pLFECurve = 0;
	e.pLPFDirectCurve = 0;
	e.pLPFReverbCurve = 0;
	e.pReverbCurve = 0;
	e.CurveDistanceScaler = 80.0f;
	e.DopplerScaler = 0.0f;
	e.InnerRadius = 2.0f;
	e.InnerRadiusAngle = X3DAUDIO_PI/4.0f;
}


template <class T>
void SoundComponent<T>::update(const GameTime& gameTime)
{
	if (frameToApply3DAudio)
	{
		for(Cue3DList::iterator it = cue3DList.begin(); it != cue3DList.end(); ++it)
		{
			if (it->active)
			{
				unsigned long state;
				it->cue->GetState(&state);
				if (state == XACT_CUESTATE_STOPPED)
					it->active = false;
				else
					update3DCue(*it);
			}
		}
	}
	++frameToApply3DAudio %= 2;
}


template <class T>
void SoundComponent<T>::release()
{
	delete[] soundBankMemory;
	soundBankMemory = 0;
}


template <class T>
void SoundComponent<T>::loadSoundBank(const std::string& path)
{
	void* file = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	unsigned long size = GetFileSize(file, 0);

	soundBankMemory = new unsigned char[size];
	unsigned long bytesRead;
	ReadFile(file, soundBankMemory, size, &bytesRead, 0);
	HRESULT hr = getManager().getEngine().CreateSoundBank(soundBankMemory, size, 0, 0, &soundBank);
	unsigned long er = GetLastError();
	CloseHandle(file);
}


template <class T>
void SoundComponent<T>::playSound(const char* name)
{
	soundBank->Play(soundBank->GetCueIndex(name), 0, 0, 0);
}


template <class T>
void SoundComponent<T>::play3DSound(const char* name)
{
	play3DSound(name, false, Vector3::zero);
}


template <class T>
void SoundComponent<T>::playStatic3DSound(const char* name, const Vector3& position)
{
	play3DSound(name, true, position);
}


template <class T>
void SoundComponent<T>::play3DSound(const char* name, bool isStatic, const Vector3& staticPosition)
{
	Cue3D* cue3D = 0;

	for(Cue3DList::iterator it = cue3DList.begin(); it != cue3DList.end(); ++it)
	{
		if (!it->active)
		{
			cue3D = &(*it);
			break;
		}
	}

	if(cue3D == 0)
	{
		cue3DList.push_back(Cue3D());
		cue3D = &cue3DList[cue3DList.size() - 1];
	}

	initEmitter(cue3D->staticEmitter);
	cue3D->staticEmitter.Position = staticPosition;
	cue3D->useStaticEmitter = isStatic;
	soundBank->Prepare(soundBank->GetCueIndex(name), 0, 0, &cue3D->cue);
	update3DCue(*cue3D);

	cue3D->active = true;
	cue3D->cue->Play();
}


template <class T>
void SoundComponent<T>::stopSound(const char* name)
{
	soundBank->Stop(soundBank->GetCueIndex(name), 0);
}


template <class T>
void SoundComponent<T>::setEmitter(const Vector3& position, const Vector3& forward, const Vector3& up)
{
	emitter.Position = position;
	emitter.OrientFront = forward;
	emitter.OrientTop = up;
}


template <class T>
void SoundComponent<T>::update3DCue(Cue3D& cue3D)
{
	SoundManager& soundManager = getManager();
	XACT3DCalculate(soundManager.getAudioHandle3D(), &soundManager.getListener(), cue3D.useStaticEmitter ? &cue3D.staticEmitter : &emitter, &dspSettings);
	XACT3DApply(&dspSettings, cue3D.cue);
}