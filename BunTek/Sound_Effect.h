#include <stdio.h>
#include "cprocessing.h"

// Store the sounds file location
#define BallBounce CP_Sound_Load("./Assets/Audio/Rubber_Ball.wav")
#define GameOverSound CP_Sound_Load("./Assets/Audio/mixkit-arcade-retro-game-over-213.wav")
#define MainMenuBGM	CP_Sound_Load("./Assets/Audio/Main_menu.wav")
#define LevelBGM CP_Sound_Load("./Assets/Audio/Level_BGM.wav")


//Global Variables
float GlobalVolume = 1;
float SFXVolume = 1;
float MusicVolume = 1;

// Transition Control
bool soundplaying = false;
bool soundstopped = false;
bool gameplaying = false;
bool gamestopped = false;
bool backtomenu = false;


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

void StopMusic() {
	CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
}

void StopSFX() {
	CP_Sound_StopGroup(CP_SOUND_GROUP_SFX);
}

void StopAllSounds() {
	CP_Sound_StopAll();
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

void SetSFXVolume(float vol) {
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, vol);
	SFXVolume = vol;
}

void SetMusicVolume(float vol) {
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, vol);
	MusicVolume = vol;
}

void SetGlobalVolume(float vol) {
	SetSFXVolume(vol);
	SetMusicVolume(vol);
}


// Notes : Screen_name Current_screen_name =
// How do i toggle music based on level? 
void bgm_control(Screen_name* current_sc_name) {

    switch (*current_sc_name) {
    case Main_menu:
    case Level_Select:
        if (gameplaying == true) {
            StopMusic();
            soundplaying = false;
            gameplaying = false;
        }
        if (gameplaying == false && soundplaying == false) {
            PlayMusic(MainMenuBGM);
            soundplaying = true;
        }

        soundstopped = false;
        break;

    case Level_1:
    case Level_2:
    case Level_3:
    case Level_4:
    case Level_5:
    case Level_6:
    case Level_7:
    case Level_8:
    case Level_9:
    case Level_10:
        if (soundstopped == false) {
            StopMusic();
            soundstopped = true;
            soundplaying = false;
        }

        if (soundplaying == false) {
            PlayMusic(LevelBGM);
            soundplaying = true;
            gameplaying = true;
        }
        break;
    }




}

void play_ballbounce_sfx() {
    PlayPitchedSoundEffect(BallBounce, 0.1f); //  Audio : ball bouncing off other balls 
}
