#include "System/Audio.h"
#include "SoundManager.h"

void SoundManager::Initialize()
{
	sounds[SoundList::titleBGM] = Audio::Instance().LoadAudioSource("Data/Sound/title.mp3");
	sounds[SoundList::gameBGM] = Audio::Instance().LoadAudioSource("Data/Sound/office.mp3");
	sounds[SoundList::fightingBGM] = Audio::Instance().LoadAudioSource("Data/Sound/fighting1.mp3");
	sounds[SoundList::resultOverBGM] = Audio::Instance().LoadAudioSource("Data/Sound/lose.mp3");
	sounds[SoundList::resultClearBGM] = Audio::Instance().LoadAudioSource("Data/Sound/win.mp3");
	sounds[SoundList::punchSE] = Audio::Instance().LoadAudioSource("Data/Sound/punch.mp3");
	sounds[SoundList::kickSE] = Audio::Instance().LoadAudioSource("Data/Sound/kick.mp3");
	sounds[SoundList::LariatSE] = Audio::Instance().LoadAudioSource("Data/Sound/Lariat.mp3");
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