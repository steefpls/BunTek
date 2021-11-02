#include <stdio.h>
#include "cprocessing.h"

#define AUDIO_BOUNCE CP_Sound_Load("./Assets/Ball_Bounce_trimmed.wav")


// Function : Load Sound File 
// Arguements : wav. location

void play_ball_bounce(void) {

	if (AUDIO_BOUNCE == NULL) {
		printf("Ball bounce Audio did not load.\n"); 
	}

	CP_Sound_Play(AUDIO_BOUNCE); 
}







// Task : Load sound path - ball bouncing 
