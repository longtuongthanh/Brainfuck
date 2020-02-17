#include "SoundClass.h"
#include "useful_stuff.h"

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif
RESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	RESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}

	return S_OK;

}

RESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	RESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

Sound::Sound()
{
	pXAudio2 = nullptr;
	pMasterVoice = nullptr;
	pSourceVoice = nullptr;
	Initialize();
}

Sound::Sound(const Sound&)
{
}

Sound::~Sound()
{
	DESTROY(pXAudio2);
}

RESULT Sound::Initialize()
{
	if (FAILED(XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		return 1;
	}
	if (FAILED(pXAudio2->CreateMasteringVoice(&pMasterVoice)))
	{
		return 1;
	}

	wfx = { 0 };
	mainBuffer = { 0 };
	tempBuffer = { 0 };
	volume = 1;

	return 0;
}

RESULT Sound::Release()
{
	delete this;
	return 0;
}

RESULT Sound::InitializeDirectSound()
{

	return false;
}

RESULT Sound::ShutdownDirectSound()
{
	return 0;
}

RESULT Sound::LoadWaveFile(const char* strFileName)
{
	HANDLE hFile = CreateFile(
		strFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	if (tempBuffer.pAudioData)
	{
		delete[] tempBuffer.pAudioData;
		tempBuffer.pAudioData = nullptr;
	}

	tempBuffer.AudioBytes = dwChunkSize;  //buffer containing audio data
	tempBuffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
	tempBuffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	if (!pSourceVoice)
	{
		if (FAILED(pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx))) return 1;
	}

	
}

RESULT Sound::ShutdownWaveFile(IDirectSoundBuffer8**)
{
	return 0;
}

RESULT Sound::PlayWaveFile()
{
	if (!pSourceVoice)
	{
		// if source voice is not created, we cannot play any sound
		return 1;
	}

	if (!tempBuffer.pAudioData)
	{
		if (!mainBuffer.pAudioData)
		{
			// if both temporary buffer and main buffer are empty, we cannot play sound
			return 1;
		}
		else
		{
			// if temporary buffer is empty but the main buffer not, play sound on main buffer
			pSourceVoice->Start();
			return 0;
		}
	}

	// if new wav file has been loaded into temporary buffer, replace main buffer with temporary buffer
	if (pSourceVoice)
	{
		pSourceVoice->Stop();
		pSourceVoice->FlushSourceBuffers();
		delete[] mainBuffer.pAudioData;
		mainBuffer.pAudioData = nullptr;
	}
	
	mainBuffer.AudioBytes = tempBuffer.AudioBytes;
	mainBuffer.pAudioData = tempBuffer.pAudioData;
	tempBuffer.Flags = XAUDIO2_END_OF_STREAM;

	tempBuffer.pAudioData = nullptr;

	if (FAILED(pSourceVoice->SubmitSourceBuffer(&mainBuffer)))
		return 1;

	pSourceVoice->Start();
	tempBuffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	return 0;
}

RESULT Sound::Pause()
{
	pSourceVoice->Stop();
	return 0;
}

RESULT Sound::Resume()
{
	pSourceVoice->Start();
	return 0;
}

RESULT Sound::SetVolumm(float volume)
{
	this->volume = volume;
	if (this->volume < 0)
	{
		this->volume = 0;
	}
	if (this->volume > 1)
	{
		this->volume = 1;
	}
	if (pSourceVoice)
	{
		pSourceVoice->SetVolume(this->volume);
	}
	else
	{
		return 1;
	}
	return 0;
}
