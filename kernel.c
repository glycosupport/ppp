#include "printf.h"
#include "screen.h"
#include "include/pci.h"

#include "include/GameCore.h"
#include "include/Object.h"

#define LAND_H                   112
#define BACK_SPEED               1
#define ANIMATION_CREATE(frames) AllocatePool (sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL*) * frames)

//Global var
INTN gXCoordinate = 120;
INTN gYCoordinate = 250;
INTN gVelocity = 5;
INTN gBirdForwardSpeed = BACK_SPEED;

//Global obj var
AnimationObject birdObj;
Object          scoreObj[10];
Object          tutorObj;
Object          landObj[2];
Object          pipeObj[2];
Object          gameOverObj;

GameCore        *core;

EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BackgroundImg = NULL;
UINTN w, h;

VOID LoadResource();

VOID ObjectInit();

VOID MyGameLogic(
    EFI_INPUT_KEY Key,
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput
);

VOID OnBirdFlying(
    IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput,
    IN  AnimationObject *birdObj,
    IN  EFI_INPUT_KEY key,
    IN  UINTN frame,
    IN  INTN *istutorObjrial
);

UINT8 IsCollision(
    IN  Info object1,
    IN  Info object2
);

void main(void) {
    clear_screen();


    core = GameCoreInstance();
	// core->Initialize();
	// LoadResource();
	// ObjectInit();
	// core->GameLoop(&MyGameLogic);



    // DrawFractal();
}

UINT8
IsCollision(
    IN  Info object1,
    IN  Info object2
)
{
	object1.pos.x += 7;
	object1.pos.y += 7;
	object1.width -= 7;
	object1.height -= 7;

	if (object1.pos.x < object2.pos.x + (INTN)object2.width &&
	        object1.pos.x + (INTN)object1.width > object2.pos.x &&
	        object1.pos.y < object2.pos.y + (INTN)object2.height &&
	        (INTN)object1.height + object1.pos.y > object2.pos.y) {
		return 1;
	}
	else
		return 0;

}

VOID
ObjectInit()
{
	birdObj.info.pos.x = 120;
	birdObj.info.pos.y = 200;

	landObj[0].info.pos.x = 0;
	landObj[0].info.pos.y = 400;
	landObj[1] = landObj[0];
	landObj[1].info.pos.x = landObj[0].info.width;
	landObj[1].info.pos.y = 400;

	pipeObj[0].info.pos.x = 500;
	pipeObj[0].info.pos.y = -200;
	pipeObj[1].info.pos.x = 500;
	pipeObj[1].info.pos.y = pipeObj[0].info.pos.y + 500;

	gameOverObj.info.pos.x = 40;
	gameOverObj.info.pos.y = 100;

	tutorObj.info.pos.x = 100;
	tutorObj.info.pos.y = 300;
}

VOID
LoadResource()
{

	birdObj.Image = ANIMATION_CREATE(3);
	// LoadFromPCX(landPcx, sizeof(landPcx), &landObj[0].info.width, &landObj[0].info.height, &landObj[0].Image);
	LoadFromRGB(land, (UINTN)150528, &landObj[0].info.width, &landObj[0].info.height, &landObj[0].Image);
	landObj[0].info.width = 336;
	landObj[0].info.height = 112;

	LoadFromRGB(bird, (UINTN)9216, &birdObj.info.width, &birdObj.info.height, &birdObj.Image[0]);
	LoadFromRGB(bird1, (UINTN)9216, &birdObj.info.width, &birdObj.info.height, &birdObj.Image[1]);
	LoadFromRGB(bird2, (UINTN)9216, &birdObj.info.width, &birdObj.info.height, &birdObj.Image[2]);
	birdObj.info.width = 48;
	birdObj.info.height = 48;
	
	// LoadFromPCX(bird0_0, sizeof(bird0_0), &birdObj.info.width, &birdObj.info.height, &birdObj.Image[0]);
	// LoadFromPCX(bird0_1, sizeof(bird0_1), &birdObj.info.width, &birdObj.info.height, &birdObj.Image[1]);
	// LoadFromPCX(bird0_2, sizeof(bird0_2), &birdObj.info.width, &birdObj.info.height, &birdObj.Image[2]);
	LoadFromRGB(pipeDown, (UINTN)65650, &pipeObj[0].info.width, &pipeObj[0].info.height, &pipeObj[0].Image);
	LoadFromRGB(pipeUp, (UINTN)65650, &pipeObj[1].info.width, &pipeObj[1].info.height, &pipeObj[1].Image);
	pipeObj[0].info.width = 52;
	pipeObj[0].info.height = 320;
	pipeObj[1].info.width = 52;
	pipeObj[1].info.height = 320;
	// LoadFromPCX(font_048, sizeof(font_048), &scoreObj[0].info.width, &scoreObj[0].info.height, &scoreObj[0].Image);
	// LoadFromPCX(font_049, sizeof(font_049), &scoreObj[1].info.width, &scoreObj[1].info.height, &scoreObj[1].Image);
	// LoadFromPCX(font_050, sizeof(font_050), &scoreObj[2].info.width, &scoreObj[2].info.height, &scoreObj[2].Image);
	// LoadFromPCX(font_051, sizeof(font_051), &scoreObj[3].info.width, &scoreObj[3].info.height, &scoreObj[3].Image);
	// LoadFromPCX(font_052, sizeof(font_052), &scoreObj[4].info.width, &scoreObj[4].info.height, &scoreObj[4].Image);
	// LoadFromPCX(font_053, sizeof(font_053), &scoreObj[5].info.width, &scoreObj[5].info.height, &scoreObj[5].Image);
	// LoadFromPCX(font_054, sizeof(font_054), &scoreObj[6].info.width, &scoreObj[6].info.height, &scoreObj[6].Image);
	// LoadFromPCX(font_055, sizeof(font_055), &scoreObj[7].info.width, &scoreObj[7].info.height, &scoreObj[7].Image);
	// LoadFromPCX(font_056, sizeof(font_056), &scoreObj[8].info.width, &scoreObj[8].info.height, &scoreObj[8].Image);
	// LoadFromPCX(font_057, sizeof(font_057), &scoreObj[9].info.width, &scoreObj[9].info.height, &scoreObj[9].Image);
	// LoadFromPCX(tutorial, sizeof(tutorObj),  &tutorObj.info.width, &tutorObj.info.height, &tutorObj.Image);
	// LoadFromPCX(text_game_over, sizeof(text_game_over),  &gameOverObj.info.width, &gameOverObj.info.height, &gameOverObj.Image);

	//Load from bitmap
	//Status = LoadBitmapFile (L"text_game_over.bmp", &gameOver.info.width, &gameOver.info.height, &gameOver.Image);

}

VOID MyGameLogic(
    EFI_INPUT_KEY Key,
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput)
{
	// EFI_STATUS Status = EFI_SUCCESS;
	static UINT8    scoreObjNum = 0;
	static UINT8    isGameOver = 0;
	static INTN     istutorObjrial = 1;
	static UINTN    frame = 0;
	static UINT8    l = 0;
	static INTN     X = 0;
	static INTN     X2 = CANVAS_W;

	if (Key.ScanCode == SCAN_END)
		return;

	// DrawGraphic (GraphicsOutput, BackgroundImg, X, 0, w, h, NULL);
	// DrawGraphic (GraphicsOutput, BackgroundImg, X2, 0, w, h, NULL);
	DrawGraphic (GraphicsOutput, pipeObj[0].Image, pipeObj[0].info.pos.x, pipeObj[0].info.pos.y, pipeObj[0].info.width, pipeObj[0].info.height, NULL);
	DrawGraphic (GraphicsOutput, pipeObj[1].Image, pipeObj[1].info.pos.x, pipeObj[1].info.pos.y, pipeObj[1].info.width, pipeObj[1].info.height, NULL);
	DrawGraphic (GraphicsOutput, landObj[0].Image, landObj[0].info.pos.x, landObj[0].info.pos.y, landObj[0].info.width, landObj[0].info.height, NULL);
	DrawGraphic (GraphicsOutput, landObj[1].Image, landObj[1].info.pos.x, landObj[1].info.pos.y, landObj[1].info.width, landObj[1].info.height, NULL);
	// DrawGraphic (GraphicsOutput, scoreObj[scoreObjNum % 10].Image, 145, 20, scoreObj[scoreObjNum % 10].info.width, scoreObj[scoreObjNum % 10].info.height, NULL);
	// DrawGraphic (GraphicsOutput, scoreObj[scoreObjNum / 10].Image, 120, 20, scoreObj[scoreObjNum / 10].info.width, scoreObj[scoreObjNum / 10].info.height, NULL);

	if (!isGameOver  && !istutorObjrial)
	{

		if (frame % 10 == 0) {
			X -= gBirdForwardSpeed;
			X2 -= gBirdForwardSpeed;
		}
		pipeObj[0].info.pos.x -= gBirdForwardSpeed;
		pipeObj[1].info.pos.x -= gBirdForwardSpeed;

		landObj[0].info.pos.x -= gBirdForwardSpeed;
		landObj[1].info.pos.x -= gBirdForwardSpeed;

		if (landObj[0].info.pos.x <= -(INTN)landObj[0].info.width)
			landObj[0].info.pos.x = 0;

		if (landObj[1].info.pos.x <= 0)
			landObj[1].info.pos.x = landObj[0].info.width;

		if (X <= -CANVAS_W)
			X = 0;
		if (X2 <= 0)
			X2 = CANVAS_W;

		if (pipeObj[0].info.pos.x <= -100) {
			pipeObj[0].info.pos.x = CANVAS_W;
			pipeObj[1].info.pos.x = CANVAS_W;
			l = 0;

			INTN nSeed;
			nSeed = core->RandomInitSeed();
			pipeObj[0].info.pos.y = nSeed % 100  - 200;
			pipeObj[1].info.pos.y = pipeObj[0].info.pos.y + 500;

		}
		if (pipeObj[0].info.pos.x < birdObj.info.pos.x && !l)
		{
			l = 1;
			scoreObjNum++;
		}
		if (IsCollision(birdObj.info , pipeObj[0].info) || IsCollision(birdObj.info , pipeObj[1].info) || IsCollision(birdObj.info , landObj[0].info) || IsCollision(birdObj.info , landObj[1].info))
		{
			isGameOver = 1;
			gBirdForwardSpeed = 0;
			gVelocity = 10;
		}
		if (frame % 1000 == 0)
		{

			if (gBirdForwardSpeed < 10)
				gBirdForwardSpeed++;
		}
	}

	if (isGameOver)
	{
		DrawGraphic (GraphicsOutput, gameOverObj.Image, gameOverObj.info.pos.x, gameOverObj.info.pos.y, gameOverObj.info.width, gameOverObj.info.height, NULL);
		if (Key.ScanCode == SCAN_UP)
		{
			ObjectInit();
			isGameOver = 0;
			gVelocity = 5;
			gBirdForwardSpeed = 1;
			scoreObjNum = 0;
			frame = 0;
			istutorObjrial = 1;
			Key.ScanCode = SCAN_NULL;
		}
	}

	OnBirdFlying(GraphicsOutput, &birdObj, Key, frame, &istutorObjrial);

	if (Key.ScanCode == SCAN_F1)
	{
		ObjectInit();
		isGameOver = 0;
		gVelocity = 5;
		gBirdForwardSpeed = 1;
		scoreObjNum = 0;
		frame = 0;
		istutorObjrial = 1;
	}

	if (istutorObjrial)
	{
		DrawGraphic (GraphicsOutput, tutorObj.Image, tutorObj.info.pos.x, tutorObj.info.pos.y, tutorObj.info.width, tutorObj.info.height, NULL);
		OnBirdFlying(GraphicsOutput, &birdObj, Key, frame, &istutorObjrial);
	}

	frame++;
}

VOID
OnBirdFlying(
    IN  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput,
    IN  AnimationObject *birdObj,
    IN  EFI_INPUT_KEY key,
    IN  UINTN frame,
    IN  INTN *istutorObjrial)
{
	static UINT8 birdFrame = 0;
	if (frame % 10 == 0) {
		gVelocity += 2;
		//gBirdForwardSpeed++;
		birdFrame ++;
		if (birdFrame == 3)
			birdFrame = 0;
	}

	if (key.ScanCode == 4)gXCoordinate--;
	if (key.ScanCode == 3)gXCoordinate++;
	if (key.ScanCode == 1)
	{
		*istutorObjrial = 0;
		gVelocity = -5;
	}
	if (key.ScanCode == 2)gYCoordinate++;

	if (!*istutorObjrial)
		birdObj->info.pos.y += gVelocity;

	if (birdObj->info.pos.y >= 400)
		birdObj->info.pos.y = 400;

	/*if (v > 5)
		Rotate(GraphicsOutput, birdObj, 90);
	else
		Rotate(GraphicsOutput, birdObj, 30);*/

	DrawGraphic (GraphicsOutput, birdObj->Image[birdFrame], birdObj->info.pos.x, birdObj->info.pos.y, birdObj->info.width, birdObj->info.height, NULL);
}