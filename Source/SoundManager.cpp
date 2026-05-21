#include "System/Audio.h"
#include "SoundManager.h"

void SoundManager::Initialize()
{
	sounds[SoundList::titleBGM] = Audio::Instance().LoadAudioSource("Data/Sound/title.wav");
	sounds[SoundList::gameBGM] = Audio::Instance().LoadAudioSource("Data/Sound/office.wav");
	//sounds[SoundList::gameBGM] = Audio::Instance().LoadAudioSource("Data/Sound/BGM.wav");
	sounds[SoundList::fightingBGM] = Audio::Instance().LoadAudioSource("Data/Sound/fighting1.wav");
	sounds[SoundList::resultOverBGM] = Audio::Instance().LoadAudioSource("Data/Sound/lose.wav");
	sounds[SoundList::resultClearBGM] = Audio::Instance().LoadAudioSource("Data/Sound/win.wav");
	sounds[SoundList::punchSE] = Audio::Instance().LoadAudioSource("Data/Sound/punch.wav");
	sounds[SoundList::kickSE] = Audio::Instance().LoadAudioSource("Data/Sound/kick.wav");
	sounds[SoundList::LariatSE] = Audio::Instance().LoadAudioSource("Data/Sound/Lariat.wav");
	//sounds[SoundList::startSE] = Audio::Instance().LoadAudioSource("Data/Sound/start.wav");
	//sounds[SoundList::heartSE] = Audio::Instance().LoadAudioSource("Data/Sound/heart.wav");

}

void SoundManager::Finalize()
{
	for (int i = 0; i < SoundList::SoundCount; i++)
	{
		if (sounds[i] != nullptr)
		{
			delete sounds[i];
			sounds[i] = nullptr;
		}
	}
}