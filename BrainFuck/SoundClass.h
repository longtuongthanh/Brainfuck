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
#include "useful_stuff.h"

class Sound
{
public:
	Sound();
	Sound(const Sound&);
	~Sound();

	float GetVolume() { return volume; }
	
	RESULT Initialize();
	RESULT Shutdown();
	
	/*
		Load wav file to temp buffer, only play that buffer when PlayWaveBuffer is call
	*/
	RESULT LoadWaveFile(const char*);
	RESULT PlayWaveFile();
	RESULT Pause();
	RESULT Resume();

	RESULT SetVolumm(float volume);

private:
	RESULT InitializeDirectSound();
	RESULT ShutdownDirectSound();

	RESULT ShutdownWaveFile(IDirectSoundBuffer8**);
private:
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasterVoice;
	IXAudio2SourceVoice* pSourceVoice;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER mainBuffer;
	XAUDIO2_BUFFER tempBuffer;

private:
	float volume;
};