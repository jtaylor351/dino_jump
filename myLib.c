#include "myLib.h"
#include "pics.h"

// extern unsigned short birdList[] = {dinoRun1, dinoRun2};

unsigned short *videoBuffer = (unsigned short *)0x6000000;

// const unsigned short *cacti= {cactus1, cactus2};
void drawSplash(int row, int col, int height, int width, const unsigned short *img);

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}


void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{
	for(int r=0; r<height; r++)
	{
		REG_DMA3SAD = (u32)&color; // This is always an address!
		REG_DMA3DAD = (u32)&videoBuffer[OFFSET(row+r, col, 240)]; // This is always an address!
		REG_DMA3CNT = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
	}
}

void drawSprite(SPRITE *sprite)
{
	// for (int r = 0; r < sprite->height; r++)
	// {
    //     for (int c = 0; c < sprite->width; c++)
	// 	{
	// 		if (sprite->img[r * sprite->width + c] == 0)
	// 		{
	// 			setPixel(sprite->row + r, c + sprite->col, LTGRAY);
	// 		}
	// 		else
	// 		{
	// 			unsigned short image_pixel = sprite->img[(r * sprite->width) + c];
	// 			setPixel(sprite->row + r, c + sprite->col, image_pixel);
	// 		}
    //     }
    // }
	drawSplash(sprite->row, sprite->col, sprite->height, sprite->width, sprite->img);
}

// void drawPartialCactus()

void drawSplash(int row, int col, int height, int width, const unsigned short *img)
{
	for(int r=0; r<height; r++)
	{
		REG_DMA3SAD = (u32)(img + r * width);
		REG_DMA3DAD = (u32)&videoBuffer[OFFSET(row+r, col, 240)];
		REG_DMA3CNT = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
	}
}

void delay(int n)
{
	volatile int x = 0;
	for(int i=0; i<n*5000; i++)
	{
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			return;
		}
		if (KEY_DOWN_NOW(BUTTON_UP)) {
			return;
		}

		x = x + 1;
	}
}

void waitForVBlank()
{
	if (KEY_DOWN_NOW(BUTTON_SELECT)) {
		return;
	}
	if (KEY_DOWN_NOW(BUTTON_UP)) {
		return;
	}
	while(SCANLINECOUNTER > 160) {
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			return;
		}
		if (KEY_DOWN_NOW(BUTTON_UP)) {
			return;
		}
	};
	while(SCANLINECOUNTER < 161) {
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			return;
		}
		if (KEY_DOWN_NOW(BUTTON_UP)) {
			return;
		}
	};
}

void drawBackground(u32* bgcolor)
{
    REG_DMA3SAD = (u32)bgcolor; // This is always an address!
    REG_DMA3DAD = (u32)videoBuffer; // This is always an address!
    REG_DMA3CNT = (240*160) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
}


int testCollision(SPRITE *s1, SPRITE *s2)
{
	if (s1->col + s1->width >= s2->col)
	{
		if (s1->col > (s2->col + s2->width))
		{
			return 0;
		}
		if (s1->row + s1->height >= s2->row)
		{
			return 1;
		}
	}
	return 0;
	// else
	// {
	// 	// if we get here we know there is overlap
	// 	if ((s1->row + s1->height) >= s2->row)
	// 	{
	// 		if (s2->col < s1->col) {
	// 			return 0;
	// 		}
	// 		return 1;
	// 	}
	// 	// if () {
	// 	//
	// 	// }
	// }
	//
	// return 0;

}

void eraseSprite(SPRITE *sprite, u32 *bg)
{
	// for(int r=0; r<sprite->height; r++)
	// {
	// 	REG_DMA3SAD = (u32)(img + r * width); // This is always an address!
	// 	// printf(REG_DMA3SAD)
	// 	REG_DMA3DAD = (u32)&videoBuffer[OFFSET(row+r, col, 240)]; // This is always an address!
	// 	//REG_DMA3DAD = (u32)(videoBuffer + OFFSET(row+r, col, 240) ); // This is always an address!
	// 	REG_DMA3CNT = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
	//
	// }
	for(int r=0; r<sprite->height; r++)
	{
		REG_DMA3SAD = (u32)bg; // This is always an address!
		REG_DMA3DAD = (u32)&videoBuffer[OFFSET(sprite->row+r, sprite->col, 240)]; // This is always an address!
		REG_DMA3CNT = (sprite->width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
	}
}


/*
void reset()
{
	cact1.col = 240 - CACTUS1_WIDTH;
	cact2.col = 240 - CACTUS2_WIDTH;
	cact3.col = 240 - CACTUS3_WIDTH;
	cact4.col = 240 - CACTUS4_WIDTH;
	bird.col = 240 - BIRD1_WIDTH;
}

*/

// void drawScene(GAME_STATE *state)
// {
// 	if (state->cactusOnScreen)
// 	{
// 		drawSprite(40, 40)
// 	}
// }
