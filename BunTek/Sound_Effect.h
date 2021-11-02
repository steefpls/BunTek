#include <stdio.h>
#include "cprocessing.h"

// Store the sounds file location
#define BallBounce CP_Sound_Load("./Assets/Ball_Bounce_trimmed.wav")
#define GameOverSound CP_Sound_Load("./Assets/mixkit-arcade-retro-game-over-213.wav")
//Global Variables
float GlobalVolume = 1;
float SFXVolume = 1;
float MusicVolume = 1;


// Playing sound effect with the correctly set volume
void PlaySoundEffect(CP_Sound audio) {
	CP_Sound_PlayAdvanced(audio, GlobalVolume * SFXVolume, 1, false, CP_SOUND_GROUP_SFX);
}

// Playing sound effect with random-ish pitch
void PlayPitchedSoundEffect(CP_Sound audio, float pitchrange) {
	float pitch = CP_Random_RangeFloat(1 - pitchrange, 1 + pitchrange);
	CP_Sound_PlayAdvanced(audio, GlobalVolume * SFXVolume, pitch, false, CP_SOUND_GROUP_SFX);
}

//play Looping Music
void PlayMusic(CP_Sound audio) {
	CP_Sound_PlayAdvanced(audio, GlobalVolume * MusicVolume, 1, true, CP_SOUND_GROUP_MUSIC);
}


void PauseMusic() {
	CP_Sound_PauseGroup(CP_SOUND_GROUP_MUSIC);
}

void PauseSFX() {
	CP_Sound_PauseGroup(CP_SOUND_GROUP_SFX);
}

void ResumeMusic() {
	CP_Sound_ResumeGroup(CP_SOUND_GROUP_MUSIC);
}

void ResumeSFX() {
	CP_Sound_ResumeGroup(CP_SOUND_GROUP_SFX);
}

void PauseAllSounds() {
	CP_Sound_PauseAll();
}

void ResumeAllSounds() {
	CP_Sound_ResumeAll();
}