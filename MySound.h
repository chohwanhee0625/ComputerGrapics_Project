#pragma once
#include <iostream>
#include "fmod inc/fmod.hpp"
class MySound {
	static FMOD::System* g_sound_system;

	FMOD::Sound* m_sound;
	FMOD::Channel* m_channel;
	float m_volume;
public:
	MySound(const char* path, bool loop, float volume = 0.1f);
	~MySound();
	static void Init();
	static void Release();

	void play(float speed = 1);
	void stop();
};