#pragma once
#include <iostream>
#include "fmod inc/fmod.hpp"
class MySound {
	static FMOD::System* g_sound_system;

	FMOD::Sound* my_sound;
	FMOD::Channel* my_channel;
public:
	MySound(const char* path, bool loop);
	~MySound();
	static void Init();
	static void Release();

	void play(float speed = 1);
	void stop();
};