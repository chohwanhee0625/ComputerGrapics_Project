#include "MySound.h"
FMOD::System* MySound::g_sound_system;

MySound::MySound(const char* path, bool loop,float volume) {
	if (loop) {
		auto result = g_sound_system->createSound(path, FMOD_LOOP_NORMAL, 0, &m_sound);
		if (result != FMOD_OK) {
			std::cerr << "���� �ε� ����: " << result <<std::endl;
		}	
	}
	else {
		auto result = g_sound_system->createSound(path, FMOD_DEFAULT, 0, &m_sound);
		if (result != FMOD_OK) {
			std::cerr << "���� �ε� ����: " << result << std::endl;
		}
	}
	m_volume = std::max(0.f, std::min(volume, 1.f));
	m_channel = nullptr;
}
MySound::~MySound() {
	m_sound->release();
}
void MySound::Init() {
	auto result = FMOD::System_Create(&g_sound_system);
	if (result != FMOD_OK) {
		std::cerr << "FMOD �ʱ�ȭ ����: " << result << std::endl;
	}
	result = g_sound_system->init(32, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK) {
		std::cerr << "FMOD �ý��� �ʱ�ȭ ����: " << result << std::endl;
	}
}

void MySound::Release() {
	g_sound_system->close();
	g_sound_system->release();
}

void MySound::play(float speed) {
	g_sound_system->playSound(m_sound, 0, false, &m_channel);
	m_channel->setVolume(m_volume);
	m_channel->setPitch(speed);
}
void MySound::stop() {
	m_channel->stop();
}