#pragma once
#include <utils.h>
#include <iostream>
#include <map>
#include "../External/SDL2/include/SDL.h"
#include <../External/SDL2_mixer/include/SDL_mixer.h>
using namespace utils;

bool InitAudio();
void EndAudio();
void LoadMusic(Mix_Music*& music, const std::string& path);
void LoadSoundEffect(Mix_Chunk*& sound, const std::string& path);
void PlayMusic(Mix_Music* sound, int repeat = -1);
void PlaySoundEffect(Mix_Chunk* sound, int repeat = 0, int canal = -1);

// call Init and End in game.cpp 
// load your files with LoadMusic or SoundEffect
// play them
// change volume with	Mix_VolumeMusic(64);         // 0-128
//						Mix_Volume(-1, 96);          // sound effects
//
// add fade out with	Mix_FadeOutMusic(float ms);
//
// free data with		Mix_FreeMusic(music)
//						Mix_FreeChunk(soundEffect)
