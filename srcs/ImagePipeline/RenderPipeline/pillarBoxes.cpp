
#include "sprite.h"
#include "screen.h"
#include "commonTools.h"
#include "Textures.h"

#define ASPECT_RATIO 1.7778f

static bool boxesSet = false;
static GLSprite *box1 = NULL;
static GLSprite *box2 = NULL;

static void DeleteOwnSprites()
{
	if (box1 != NULL)
		delete box1;
	if (box2 != NULL)
		delete box2;
	box1 = NULL;
	box2 = NULL;
}

static void SetOwnLetterBoxes(Shader *shader)
{
	int width = __currentScreenWidth;
	int height = __currentScreenHeight;
	int targetHeight = rounding((float)width / ASPECT_RATIO);
	int removedPixels = height - targetHeight;
	removedPixels = rounding((float)removedPixels / 2.0f);
	float scale = 1.0f / (float)height;
	float poisitioning = scale * removedPixels;
	box1 = new GLSprite({-1.1f, 1.0f - poisitioning}, {2.2f, poisitioning + poisitioning}, gameTestTextures.everyColor.text, shader, 0);
	box2 = new GLSprite({-1.1f, -1.0f - poisitioning}, {2.2f, poisitioning + poisitioning}, gameTestTextures.everyColor.text, shader, 0);
	box1->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	box2->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
}

static void SetOwnPillarBoxes(Shader *shader)
{
	int width = __currentScreenWidth;
	int height = __currentScreenHeight;
	int targetHeight = rounding((float)width / ASPECT_RATIO);
	int removedPixels = height - targetHeight;
	removedPixels = rounding((float)removedPixels / 2.0f);
	float scale = 1.0f / (float)height;
	float poisitioning = scale * removedPixels;
	box1 = new GLSprite({0.0f, 0.0f}, {1.0f, 1.0f}, gameTestTextures.tile.text, shader, 0);
	box2 = new GLSprite({-1.1f, -1.0f - poisitioning}, {2.2f, poisitioning + poisitioning}, gameTestTextures.tile.text, shader, 0);
	box1->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	box2->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void SetPillarBoxes(Shader *shader)
{
	if (!__forceAspectRatio)
		return ;
	int width = __currentScreenWidth;
	int height = __currentScreenHeight;
	float ratio = (float)width / (float)height;
	if (FAlmostEqual(ASPECT_RATIO, ratio, 0.0001f))
		return ;
	boxesSet = true;
	DeleteOwnSprites();
	if (ratio < ASPECT_RATIO)
		SetOwnLetterBoxes(shader);
	else
		SetOwnPillarBoxes(shader);
}

void DrawPillarBoxes()
{
	if (!boxesSet)
		return ;
	if (box1 != NULL)
		box1->Draw();
	if (box2 != NULL)
		box2->Draw();
}

void ClearPillarBoxes()
{
	boxesSet = false;
	DeleteOwnSprites();
}
