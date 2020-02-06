#pragma once
/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <xaudio2.h>

class SoundClass
{
public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	HRESULT Initialize();
	HRESULT Shutdown();
	HRESULT LoadWaveFile(const char*);
	HRESULT PlayWaveFile();

private:
	HRESULT InitializeDirectSound();
	HRESULT ShutdownDirectSound();

	HRESULT ShutdownWaveFile(IDirectSoundBuffer8**);
private:
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
	IXAudio2SourceVoice* pSourceVoice;
	IXAudio2SourceVoice* pOldSourceVoice;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	int count;
};