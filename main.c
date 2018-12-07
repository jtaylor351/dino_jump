#include <stdio.h>
#include <stdlib.h>
#include "text.h"
#include "dinoSplash.h"
#include "myLib.h"
#include "pics.h"
#include "gameover.h"

void resetDinos(SPRITE *dino1, SPRITE *dino2);
void resetCacti(SPRITE *cact1, SPRITE *cact2, SPRITE *cact3, SPRITE *cact4);
void resetGameState(GAME_STATE *gs);

// states of the game
enum GBAState {
	HOME,
	WAIT_START,
	PLAYING,
	GAME_OVER,
	WAIT_REPLAY
};


int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	int bgcolor = LTGRAY;

	SPRITE dino1 = {
		20,
		GROUND - DINORUN1_HEIGHT,
		DINORUN1_HEIGHT,
		DINORUN1_WIDTH,
		dinoRun1,
	};

	SPRITE dino2 = {
		20,
		GROUND - DINORUN2_HEIGHT,
		DINORUN2_HEIGHT,
		DINORUN2_WIDTH,
		dinoRun2,
	};


	SPRITE cact1 = {
		239 - CACTUS1_WIDTH,
		GROUND - CACTUS1_HEIGHT + 4,
		CACTUS1_HEIGHT,
		CACTUS1_WIDTH,
		cactus1,
	};

	SPRITE cact2 = {
		239  - CACTUS2_WIDTH,
		GROUND - CACTUS2_HEIGHT + 4,
		CACTUS2_HEIGHT,
		CACTUS2_WIDTH,
		cactus2,
	};

	SPRITE cact3 = {
		239 - CACTUS3_WIDTH,
		GROUND - CACTUS3_HEIGHT + 4,
		CACTUS3_HEIGHT,
		CACTUS3_WIDTH,
		cactus3,
	};

	SPRITE cact4 = {
		230 - CACTUS4_WIDTH,
		GROUND - CACTUS4_HEIGHT + 4,
		CACTUS4_HEIGHT,
		CACTUS4_WIDTH,
		cactus4,
	};

	SPRITE *dinoList[] = {&dino1, &dino2};
	SPRITE *cactusList[] = {&cact1, &cact2, &cact3, &cact4};

	enum GBAState state = HOME;
	int selectWasPressedDown = 0;
	// int delayCounter = 0;
	GAME_STATE gState = {
		0,
		0,
		100,
		dinoList[0],
		&cact1,
		0
	};

	while(1) {

		waitForVBlank();

		switch(state) {
		case HOME:
			resetGameState(&gState);
			resetDinos(&dino1, &dino2);
			resetCacti(&cact1, &cact2, &cact3, &cact4);
			drawBackground((u32*)&bgcolor);
			drawSplash(0, 0, DINOSPLASH_HEIGHT, DINOSPLASH_WIDTH, dinoSplash);
			drawString(15, 70, "Press Start to be", BLACK);
			drawString(25, 60, "Rescued from Boredom", BLACK);
			drawString(35, 50, "T-Rex meets the matrix", BLACK);

			state = WAIT_START;
			break;

		case WAIT_START:
			if (selectWasPressedDown) {
				break;
			} else if (KEY_DOWN_NOW(BUTTON_START)) {
				drawBackground((u32*)&bgcolor);
				state = PLAYING;
			}
			break;

		case PLAYING:

		/*
		********************************************************************
			Logic for transitioning between states
		********************************************************************
		*/
			if (KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = HOME;
				break;
			}

		/*
		********************************************************************
			Logic for displaying score
		********************************************************************
		*/

			if ((gState.score % 60) == 0)
			{
				drawRect(0, 0, 20, 240, LTGRAY);
				char score[50];
				sprintf(score, "SCORE: %d", gState.score/60);
				drawString(10, 10, score, BLACK);
			}

		/*
		********************************************************************
			Logic for if the up button was pressde to see if a jump should
			be made. Only if not currently jumping (defined by jumpCounter
			equalling 0)
		********************************************************************
		*/

			if (KEY_DOWN_NOW(BUTTON_UP)  && (gState.jumpCounter == 0)) {
				gState.jumpCounter = 20; // this numebr divided by 2 is make jump height
			} else if (KEY_DOWN_NOW(BUTTON_UP)) {
				if ((gState.score % 60) != 0) {
					break;
				}
			}


		/*
		********************************************************************
			Logic for how to move dino based on whether you're jumping or not
		********************************************************************
		*/
		if ((gState.score % 4) == 0)
		{
			if(gState.jumpCounter == 0) // normail case where no jumping
			{
				eraseSprite(gState.dino, (u32 *)&bgcolor);
				gState.dino = dinoList[gState.score%2];
				drawSprite(gState.dino);
			}
			else if (gState.jumpCounter == 1) // need to clean up and return to normal
			{
				eraseSprite(gState.dino, (u32 *)&bgcolor);
				resetDinos(&dino1, &dino2);
				drawSprite(gState.dino);
				gState.jumpCounter = 0;
			}
			else
			{

					eraseSprite(gState.dino, (u32 *)&bgcolor);
					gState.dino->row += (11 - (gState.jumpCounter));
					drawSprite(gState.dino);
					gState.jumpCounter--;
				}
			}


		/*
		********************************************************************
			Logic for checking if an object colided with our dyno
		********************************************************************
		*/


			if (testCollision(gState.dino, gState.object)) {

				resetCacti(&cact1, &cact2, &cact3, &cact4);
				state = GAME_OVER;
				break;
			}


		/*
		********************************************************************
			If we get here there wasn't a collison so it's safe to erase
			and update our cactus.
		********************************************************************
		*/

		if ((gState.score % 2) == 0)
		{
			eraseSprite(gState.object, (u32 *)&bgcolor);

			if (gState.object->col <= 1) {
				resetCacti(&cact1, &cact2, &cact3, &cact4);
				gState.object = cactusList[(gState.score * 31) % 4];
			} else {
				gState.object->col -= 4; // SPEED OF CACTI
			}

			drawSprite(gState.object);
		}

			gState.score++;

			break;

		case GAME_OVER:

			// drawBackground((u32*)&bgcolor);
			drawSplash(0, 0, GAMEOVER_HEIGHT, GAMEOVER_WIDTH, gameOver);
			char finalScore[50];
			sprintf(finalScore, "SCORE: %d", gState.score/60);;
			drawString(75, 80, finalScore, LTGRAY);
			resetGameState(&gState);
			resetDinos(&dino1, &dino2);
			resetCacti(&cact1, &cact2, &cact3, &cact4);
			state = WAIT_REPLAY;
			break;

		case WAIT_REPLAY:

			if (KEY_DOWN_NOW(BUTTON_START)) {
				state = PLAYING;
				drawBackground((u32*)&bgcolor);
				break;
			}
			if (KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = HOME;
				break;
			}
			break;
		}

		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			selectWasPressedDown = 1;
		} else {
			selectWasPressedDown = 0;
		}


	}

	return 0;

}

void resetDinos(SPRITE *dino1, SPRITE *dino2) {

	dino1->col = 20;
	dino1->row = GROUND - DINORUN1_HEIGHT;

	dino2->col = 20;
	dino2->row = GROUND - DINORUN1_HEIGHT;
}

void resetCacti(SPRITE *cact1, SPRITE *cact2, SPRITE *cact3, SPRITE *cact4) {
	cact1 -> col = 239 - CACTUS1_WIDTH;
	cact2 -> col = 239 - CACTUS2_WIDTH;
	cact3 -> col = 239 - CACTUS3_WIDTH;
	cact4 -> col = 239 - CACTUS4_WIDTH;

}

void resetGameState(GAME_STATE *gs) {
	gs -> score = 0;
	gs -> jumpCounter = 0;
}
