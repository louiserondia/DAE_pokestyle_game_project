#include "pch.h"
#include "Core.h"
#include "Audio.h"

bool InitAudio()
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return false;

	// 44.1 Khz, default format, 2 channels (stereo), 2048 bytes chunk
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		return false;

	return true;
}

void EndAudio() {
	Mix_CloseAudio();
	SDL_Quit();
}

void LoadMusic(Mix_Music*& music, const std::string& path) {
	music = Mix_LoadMUS(path.c_str());
	if (music == NULL)
		std::cout << "Couldn't load music at " << path << std::endl;
}

void LoadSoundEffect(Mix_Chunk*& sound, const std::string& path) {
	sound = Mix_LoadWAV(path.c_str());
	if (sound == NULL)
		std::cout << "Couldn't load sound effect at " << path << std::endl;
}

void PlayMusic(Mix_Music* sound, int repeat) {
	Mix_PlayMusic(sound, -1);
}

void PlaySoundEffect(Mix_Chunk* sound, int repeat, int canal) {
	Mix_PlayChannel(canal, sound, repeat);
}

