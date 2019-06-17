#ifndef SOUNDENGINE_H_
#define SOUNDENGINE_H_

#include"Sound.h"
class SoundEngine
{
	SoundClass *sound;
	HWND hwnd;
public:
	SoundEngine()
	{
		sound = new SoundClass();
	}
	void SetWindowHandler(HWND hwnd)
	{
		this->hwnd = hwnd;
	}
	void StartBackMusic(char* path)
	{
		PlaySound((LPCWSTR)path, NULL, SND_ASYNC);
	}
	void PlayShortSound(char* path)
	{
		sound->Initialize(hwnd, path);
	}
};
#endif