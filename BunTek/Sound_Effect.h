/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * file:	    Sound_Effect.h
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 *
 * brief:	Contains the functions for audio flow and controls. 
 *
* ---------------------------------------------------------*/

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
float BGMVolume = 1;

// Transition Control
bool soundplaying = false;
bool soundstopped = false;
bool gameplaying = false;
bool gamestopped = false;
bool backtomenu = false;

// Volume Bars 
float current_volume = 3; 
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void PlaySoundEffect(CP_Sound audio)
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [SFX]- Function for playing a certain sound effect at a fixed volume. 
 *              
 *             
 *             
 *              
* ---------------------------------------------------------*/
// Playing sound effect with the correctly set volume
void PlaySoundEffect(CP_Sound audio) {
	CP_Sound_PlayAdvanced(audio, GlobalVolume * SFXVolume, 1, false, CP_SOUND_GROUP_SFX);
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void PlayPitchedSoundEffect(CP_Sound audio, float pitchrange)
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [SFX] - Function for playing a certain sound effect at an adjustable pitch. 
 *
 *
 *
 *
* ---------------------------------------------------------*/
// Playing sound effect with random-ish pitch
void PlayPitchedSoundEffect(CP_Sound audio, float pitchrange) {
	float pitch = CP_Random_RangeFloat(1 - pitchrange, 1 + pitchrange);
	CP_Sound_PlayAdvanced(audio, GlobalVolume * SFXVolume, pitch, false, CP_SOUND_GROUP_SFX);
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void PlayMusic(CP_Sound audio)
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [BGM] - Sets selected audio file and loop it once audio clip has played finished. 
 *
 *
 *
 *
* ---------------------------------------------------------*/
//play Looping Music
void PlayMusic(CP_Sound audio) {
	CP_Sound_PlayAdvanced(audio, GlobalVolume * BGMVolume, 1, true, CP_SOUND_GROUP_MUSIC);
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void StopMusic() 
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [BGM] - Stops currently playing music. 
 *
 *
 *
 *
* ---------------------------------------------------------*/
void StopMusic() {
	CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void StopSFX()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [SFX] - Stops currently playing sound effects. 
 *
 *
 *
 *
* ---------------------------------------------------------*/
void StopSFX() {
	CP_Sound_StopGroup(CP_SOUND_GROUP_SFX);
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void StopAllSounds()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [SFX & BGM] - stops every sound in the game. 
 *
 *
 *
 *
* ---------------------------------------------------------*/
void StopAllSounds() {
	CP_Sound_StopAll();
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void PauseMusic()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [BGM] - Pauses all background music playing currently.
 *
 *
 *
 *
* ---------------------------------------------------------*/
void PauseMusic() {
	CP_Sound_PauseGroup(CP_SOUND_GROUP_MUSIC);
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void PauseSFX()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [SFX] - Pauses all background sound effects. 
 *
 *
 *
 *
* ---------------------------------------------------------*/
void PauseSFX() {
	CP_Sound_PauseGroup(CP_SOUND_GROUP_SFX);
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void ResumeMusic()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [BGM] - Resumes all background music in the game
 *
 *
 *
 *
* ---------------------------------------------------------*/
void ResumeMusic() {
	CP_Sound_ResumeGroup(CP_SOUND_GROUP_MUSIC);
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void ResumeSFX()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [SFX] - Resumes all sound effects.
 *
 *
 *
 *
* ---------------------------------------------------------*/
void ResumeSFX() {
	CP_Sound_ResumeGroup(CP_SOUND_GROUP_SFX);
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void PauseAllSounds()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [SFX & BGM] - Pause every sound in the game.
 *
 *
 *
 *
* ---------------------------------------------------------*/
void PauseAllSounds() {
	CP_Sound_PauseAll();
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void ResumeAllSounds()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [SFX & BGM] - Resume every sound in the game.
 *
 *
 *
 *
* ---------------------------------------------------------*/
void ResumeAllSounds() {
	CP_Sound_ResumeAll();
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void SetSFXVolume()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [SFX] - adjust the volume of sound effects. 
 *
 *
 *
 *
* ---------------------------------------------------------*/
void SetSFXVolume(float vol) {
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, vol);
	SFXVolume = vol;
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void SetMusicVolume()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [BGM] - adjust the volume of background music.
 *
 *
 *
 *
* ---------------------------------------------------------*/
void SetMusicVolume(float vol) {
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, vol);
	BGMVolume = vol;
}
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void SetGlobalVolume()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [BGM] - adjust the volume of every audio element in the game
 *
 *
 *
 *
* ---------------------------------------------------------*/
void SetGlobalVolume(float vol) {
	SetSFXVolume(vol);
	SetMusicVolume(vol);
}

/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void bgm_control()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [BGM] - sound control of different background music, depending on which screen it is currently on screen. 
 *
 *
 *
 *
* ---------------------------------------------------------*/

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
/*---------------------------------------------------------
 * Copyright � 2021 DigiPen, All rights reserved.
 *
 * void play_ballbounce_sfx()
 * author:	    [Cheong Ming Lun]
 * email:	    [m.cheong@digipen.edu]
 * Co-author:	[]
 * email:	    []
 * Co-author:   []
 * email:       []
 *
 * brief:       [SFX] - shortcut function for playing bouncy ball sound effect. 
 *
 *
 *
 *
* ---------------------------------------------------------*/
void play_ballbounce_sfx() {
    PlayPitchedSoundEffect(BallBounce, 0.1f); //  Audio : ball bouncing off other balls 
}
