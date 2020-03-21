// Author: J a k e	 G u s t a f s o n
// Title: "DX Man: Defeat the Aliens"
//
// Credits:
// Thanks to Andrew LaMoth's book for help with the simplified DirectDraw functions
// Thanks to NukeDX for ideas for alpha drawing
//
// Notes:
// -The size of hero00**.TGA's are 169x185
// -The size of alienf*.TGA's are 273x81
// -The size of 480intro.TGA is 480x480
// -All images are loaded as Targa structs except the background Targa which is loaded into a DDSTRUCT
//

//Known Safe order:
 //INCLUDES//
 //DEFINES// and //MACROS//
 //TYPES// and //STRUCTS
 //GLOBALS//
 //PROTOTYPES//
 //CLASSES// then //CLASS POINTERS// then //MORE CLASSES, THAT USE OTHERS' POINTERS//
 //FUNCTIONS//


// INCLUDES ///////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN	//tells windows.h to skip MFC
//#define RBE_DEBUG //tells ERRLOG.CPP to output function call titles even when there is no error.
#define INITGUID

//Important Windows stuff:
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
//Important C/C++ stuff
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
//More includes:
#include <fstream.h>
#include "resource.h"
#include "ddraw.h" // include directdraw
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "winmm.lib")

#include "_ENGINE.H"
#include "ERRLOG.CPP"
#include "BASEFUNC.CPP"
#include "TGA.CPP"

// DEFINES ////////////////////////////////////////////////

//Defines for windows
#define WINDOW_CLASS_NAME "WINCLASS1"
//Default screen size
#define SCREEN_WIDTH			640
#define SCREEN_HEIGHT			480
#define SCREEN_BPP				32	 //bits per pixel, GameInit tries 32 or 24 if one is not available
//#define MAX_COLORS_PALETTE		256
//States for game loop
#define GAME_STATE_INIT			0
#define GAME_STATE_START_LEVEL	1
#define GAME_STATE_START_STAGE	2
#define GAME_STATE_RUN			3
#define GAME_STATE_SHUTDOWN		4
#define GAME_STATE_EXIT			5
#define GAME_STATE_WIN_STAGE	6
#define GAME_STATE_WIN_GAME		7
#define GAME_STATE_YOU_LOSE		8
//Operations for Explosion
#define EXPLOSION_CHECK_COUNT	0
#define EXPLOSION_START			1
#define EXPLOSION_CONTINUE		2
#define PI						3.1416
//States of hero or enemies
#define STATUS_ALIVE			1
#define STATUS_SHIELD			2
#define STATUS_DOUBLESPEED		4
#define STATUS_ANTIGRAVITY		8
#define STATUS_SHOOTER			16
#define STATUS_BOMBER			32
#define STATUS_BOSS				64
#define STATUS_PRECISIONAIM		128
#define MAXALIENS				4
#define MAXSHOTS				20


// MACROS /////////////////////////////////////////////////

// tests if a key is up or down
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)	 ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
//Initialize a direct draw struct:
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct);}
//Build a 32 bit color value in A.8.8.8 format (8-bit alpha mode):
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
//24-bit specific, needed because of some video cards:
#define _RGB24BIT(r,g,b) ((b) + ((g) << 8) + ((r) << 16))
//Get the b,g,r or a of BGRA pixel:
//#define _BYTEXOFPIXEL(x,pixel) ((pixel >> 8*4(x-1)) & 255)

// TYPES //////////////////////////////////////////////////////

// basic unsigned types
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;


// GLOBALS ////////////////////////////////////////////////
UCHAR alphaLook[256][256][256]; //lookup for alpha result: ((Source-Dest)*alpha/256+Dest)

int ix;//temp var for error#'s
int iErrorsSaved=0;
int iFramesDropped=0;
int REAL_BPP=32;
int REAL_BYTEDEPTH=4;
int keyDelay=0;
int doublecode=0; //player cheat code
int precisecode=0; //player cheat code
int bombed=0;
//WinAPI globals:
HWND			hwndMain = NULL; // globally track main window
int			 window_closed			= 0;		// tracks if window is closed
HINSTANCE hinstance_app			= NULL; // globally track hinstance
//DirectDraw globals:
//LPDIRECTDRAWPALETTE	 lpddpal			= NULL;	 // a pointer to the created dd palette
LPDIRECTDRAW					lpdd				 = NULL;	 // dd object
LPDIRECTDRAW4				 lpdd4				= NULL;	 // dd4 object
LPDIRECTDRAWSURFACE4	lpddsPrimary = NULL;	 // dd primary surface
LPDIRECTDRAWSURFACE4	lpddsBack		= NULL;	 // dd back surface
LPDIRECTDRAWSURFACE4	lpddsBackdrop= NULL;	 // dd backdrop image surface
//LPDIRECTDRAWSURFACE4	lpddsExplosion= NULL;	// dd working surface where explosion outputs
LPDIRECTDRAWCLIPPER	 lpddClipper	= NULL;	 // dd clipper
DDSURFACEDESC2				ddsd;									// a direct draw surface description struct
DDBLTFX							 ddbltfx;							 // used to fill
DDSCAPS2							ddscaps;							 // a direct draw surface capabilities struct
HRESULT							 ddrval;								// result back from dd calls
DDPIXELFORMAT			ddpixel;				 // used to get real bit depth and all that
//Other globals:
DWORD								 start_clock_count = 0; // used for timing
LPTARGA				lptargaBack=NULL;
LPTARGA				lptargaIntro=NULL;
LPTARGA				*lptargaAlien;
LPTARGA				*lptargaAlienBoss;
LPTARGA				*lptargaHero;
LPTARGA				*lptargaShot;
LPTARGA				*lptargaGameScreen;
//LPTARGA				lptargaAlien[2]={NULL,NULL};
//LPTARGA				lptargaAlienBoss[3]={NULL,NULL,NULL};
//LPTARGA				lptargaHero[21]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
//LPTARGA				lptargaShot[2]={NULL,NULL};
//LPTARGA				lptargaGameScreen[9]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

char buffer[80];														 // general printing buffer
int explosionResult = 0;
int gameState = GAME_STATE_INIT;
int gameLevel = 1;					//3 Levels total
int gameStage = 1;					//3 Stages total, stages are within Levels
int index;		 // looping index
int numOfAliens = 0;


// PROTOTYPES	//////////////////////////////////////////////

//int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);
LPDIRECTDRAWSURFACE4 DDrawCreateSurface(int width, int height, int memflags);
LPDIRECTDRAWCLIPPER DDrawAttachClipper(LPDIRECTDRAWSURFACE4 lpdds,
											int numRects, LPRECT clipList);
int DDrawFillSurface(LPDIRECTDRAWSURFACE4 lpdds,int color);
int DDrawDrawSurface(LPDIRECTDRAWSURFACE4 source, int x, int y,
											int width, int height, LPDIRECTDRAWSURFACE4 dest);
int DDrawDrawSurfaceScaled(LPDIRECTDRAWSURFACE4 source, int x, int y,
											int width_src, int height_src, int width_dest, int height_dest,
											LPDIRECTDRAWSURFACE4 dest);
int _Targa2Surface32_Alpha(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY);//, int opacityCap);
int _Targa2Surface32_AlphaQuickEdge_FX(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat);
int _Targa2Surface32_AlphaQuickEdge_FX_Scaled(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat, float scale);
int Targa2Surface32(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY);
int _Targa2Surface24_Alpha(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY);//, int opacityCap);

//universal (24- or 32-bit mode) Targa renderer
int _Targa2SurfaceAlphaFX(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat);

int _Targa2Surface24_AlphaQuickEdge_FX(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat);
int _Targa2Surface24_AlphaQuickEdge_FX_Scaled(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat, float scale);
int Targa2Surface24(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY);
//UNUSED FUNCTION:
int _Targa2Surface32_AlphaQuickEdge(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY);

// CLASSES ////////////////////////////////////////////////
class Shot
{
private:
	LPTARGA frames[2];
	int width, height;	//actual Targa size
	int frameNow;
	int xVel, yVel, zVel; //zVelocity is ALWAYS divided by 10.0 when used because
							// z is only 4 levels, but the zVel is in a pixel-like measurement unit size
public:
	int isAlien, isBomb; //used as booleans
	int x,y; //these are centerpoints, placement of image is calculated
	float z;

	int dead;
	Shot();
	Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isAlien2, int isBomb2);
	void refresh();
	void redraw(register RECT &rectRendered, register float &scale);
	void setVars(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isAlien2, int isBomb2);

};
Shot::Shot()
{
	width = lptargaShot[0]->width; //set real size
	height = lptargaShot[0]->height;

	frames[0]=lptargaShot[0];
	frames[1]=lptargaShot[1];
	frameNow=rand()%2;
	x=SCREEN_WIDTH/2+width/2;
	y=SCREEN_HEIGHT/2+width/2;
	z=4;
	xVel=1;
	yVel=1;
	zVel=1;
	isAlien=0;
	isBomb=0;
	dead=0;
}
Shot::Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isAlien2, int isBomb2)
{
	width = lptargaShot[0]->width; //set real size
	height = lptargaShot[0]->height;

	frames[0]=lptargaShot[0];
	frames[1]=lptargaShot[1];
	frameNow=rand()%2;
	x=x2;
	y=y2;
	z=z2;
	xVel=xVel2;
	yVel=yVel2;
	zVel=zVel2;
	isAlien=isAlien2;
	isBomb=isBomb2;
	dead=0;
}
void Shot::refresh()
{
// ORDER: // (hit detect after draw is okay since the only hit detection here is edge hitting; alien and hero are better)
	//SET RECT//
	//DRAW//
	//MOVE//
	//HITDETECT// and //FIX POSITION (not needed, hit detection kills it at screen edge)//


	// SET RECT //
	register float scale = z/4.0f;
	register RECT rectRendered; //inclusive edges

	rectRendered.left=x-width/2*scale;
	rectRendered.right=rectRendered.left+scale*width;
	rectRendered.top=y-height/2*scale;
	rectRendered.bottom=rectRendered.top+scale*height;


	frameNow = (frameNow) ? 0 : 1; //advance frame

	//TO IMPLEMENT: DO NOT REDRAW if behind an object!:
	redraw(rectRendered, scale);

	x+=xVel;
	y+=yVel;
	z+=zVel/10.0;

	if (rectRendered.left<0 || rectRendered.right>=SCREEN_WIDTH || rectRendered.top<0 || rectRendered.bottom>=SCREEN_WIDTH
		|| z<.5)
		dead = 1;
}
void Shot::redraw(register RECT &rectRendered, register float &scale)
{
	//USES ALPHALOOK//_Targa2SurfaceAlphaFX(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-isAlien*127, isBomb, 0);

	//_Targa2Surface24_AlphaQuickEdge_FX(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-isAlien*127, isBomb, 0);

	if (REAL_BPP==24)
	{
		if (z==4)
			_Targa2Surface24_AlphaQuickEdge_FX(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-isAlien*127, isBomb, 0);
		else
			_Targa2Surface24_AlphaQuickEdge_FX_Scaled(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-isAlien*127, isBomb, 0, scale);
	}
	else
	{
		if (z==4)
			_Targa2Surface32_AlphaQuickEdge_FX(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-isAlien*127, isBomb, 0);
		else
			_Targa2Surface32_AlphaQuickEdge_FX_Scaled(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-isAlien*127, isBomb, 0, scale);
	}



}
void Shot::setVars(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isAlien2, int isBomb2)
{
	x=x2;
	y=y2;
	z=z2;
	xVel=xVel2;
	yVel=yVel2;
	zVel=zVel2;
	isAlien=isAlien2;
	isBomb=isBomb2;
}

// CLASS POINTERS GLOBAL TO CHARACTERS ////////////////////////////
Shot				*shots[MAXSHOTS];
////////////////////////////////////////////////////////////

class Alien
{
public:
	LPTARGA frames[4]; //frames of animation for cycle
	DWORD dwStatus;					//For special powers, color channels omitted etc.
	int iHP;						//Hit Points left
	int stunnedness;				//If true: slow down, flash red, and decrement this
	int x,y,z;												//CENTERPOINTS - Position of alien. z MUST be 1 to 4: multiplied by .25 to get scale
	int xVelocity;
	int yVelocity;
	//int verticle;
	int frameNow;							//Current frame of animation
	int frameDelay;
	int width;
	int height;										 //Size of surface
	int explodedBy;					//Size of explosion, incremented when dead
	int exploding; //used as boolean
	int shootDelay;

	Alien();
	Alien(int x2, int y2, int z2);
	~Alien();
	void setAsBoss();
	void refresh();
	void hitDetect(float &scaler);
	int yCenter();
	int leftEdge();
	int rightEdge();
private:
	void redraw(register RECT &rectRendered, register float &scale);
	void shoot();
	void bomb();
};
Alien::Alien()
{
	numOfAliens++;
	srand(GetTickCount());
	dwStatus = STATUS_ALIVE;
	iHP = 30 * gameStage;
	stunnedness = 0;
	x = SCREEN_WIDTH*.75;
	y = SCREEN_HEIGHT*.75;
	z = 4;
	xVelocity = -(2 + rand()%2) - gameStage;
	yVelocity = -1;
	//verticle = -1;
	frameNow = 0;
	frameDelay = 0;
	shootDelay = 20;
	width = lptargaAlien[0]->width; // set real size
	height = lptargaAlien[0]->height;
	explodedBy = 0;
	exploding = 0;
	//scale = 1; // scale from 0.1 to 2.0
	frames[0] = lptargaAlien[0];
	frames[1] = lptargaAlien[1];
	frames[2] = lptargaAlien[0];
	frames[3] = lptargaAlien[1];
}
Alien::Alien(int x2, int y2, int z2)
{
	numOfAliens++;
	srand(GetTickCount());
	dwStatus = STATUS_ALIVE;
	iHP = 30 * gameStage;
	stunnedness = 0;
	x = x2;
	y = y2;
	z = z2;
	xVelocity = -(2 + rand()%2) - gameStage;
	yVelocity = -1;
	//verticle = -1;
	frameNow = 0;
	frameDelay = 0;
	shootDelay = 20;
	width = lptargaAlien[0]->width; // set real size
	height = lptargaAlien[0]->height;
	explodedBy = 0;
	exploding = 0;
	//scale = 1; // scale from 0.1 to 2.0
	frames[0] = lptargaAlien[0];
	frames[1] = lptargaAlien[1];
	frames[2] = lptargaAlien[0];
	frames[3] = lptargaAlien[1];

	if (gameLevel==2) dwStatus |= STATUS_SHOOTER;
	else if (gameLevel==3) dwStatus |= STATUS_BOMBER;
	if (gameLevel==3 && gameStage==3) dwStatus |= STATUS_SHIELD;

}
Alien::~Alien()
{
	numOfAliens--;
}
void Alien::setAsBoss()
{
	width = lptargaAlienBoss[0]->width; // set real size
	height = lptargaAlienBoss[0]->height;

	frames[0] = lptargaAlienBoss[0];
	frames[1] = lptargaAlienBoss[1];
	frames[2] = lptargaAlienBoss[2];
	frames[3] = lptargaAlienBoss[1];

	dwStatus = STATUS_ALIVE | STATUS_BOMBER | STATUS_SHIELD | STATUS_BOSS;
	iHP=1000;
}
void Alien::refresh()
{
	register RECT rectRendered; //inclusive edges
	register float scale = z/4.0f;

// ORDER: //

	//- UPDATE+MOVE//
	//- FIX POSITION// -get it ready for the next set rect and next render
	//- SET RECT//
	//- DRAW//
	//- HITDETECT&CHECK LIFE// -this way it is using the rectRendered for hit detection

	// UPDATE+MOVE //
	if (stunnedness) stunnedness--;
	if (shootDelay) shootDelay--;
		// animate frames with frame delay
	if (++frameDelay >= (8 - xVelocity))
	{
		// reset frameDelay for the animation frame
		frameDelay = 0;
		// advance to next frame
		frameNow++;
		if (frameNow>3) frameNow=0;
	}

	if (!(dwStatus & STATUS_BOSS))
	{
		//Move non-boss at its given velocity
		x += (xVelocity<0) ? (xVelocity - z + stunnedness) : (xVelocity + z + stunnedness);
		y+=yVelocity;
	}
	else
	{//Boss's speed not affected by stunnedness, but by double speed status
		if (dwStatus & STATUS_DOUBLESPEED) x+=xVelocity*2;
		else x+=xVelocity;
	}


	// FIX POSITION //
	//Update needed rectRendered members for hit detection (z omitted because z doesn't change)
	rectRendered.left=x-width/2*scale;
	rectRendered.right=rectRendered.left+scale*width;
	rectRendered.top=y-height/2*scale;
	rectRendered.bottom=rectRendered.top+scale*height;
	// right wrapping omitted because aliens always move left

	if (rectRendered.right<0)
		x=int(SCREEN_WIDTH+width/2*scale);

	if (rectRendered.bottom>SCREEN_HEIGHT/2)
	{
		yVelocity=-yVelocity;
		y=SCREEN_HEIGHT/2-height/2*scale;//bounce it out of the danger zone
	}
	else if (rectRendered.top<=0)
	{
		yVelocity=-yVelocity;
		y=height/2*scale; //bounce it out of the danger zone
	}

	// RESET RECT //
	scale = z/4.0f;
	rectRendered.left=x-width/2*scale;
	rectRendered.right=rectRendered.left+scale*width;
	rectRendered.top=y-height/2*scale;
	rectRendered.bottom=rectRendered.top+scale*height;


	// DRAW //
	redraw(rectRendered, scale);

	// AI ACTIONS //
	if (dwStatus & STATUS_SHOOTER)
		shoot();
	else if (dwStatus & STATUS_BOMBER)
		bomb();


	// HITDETECT //
	hitDetect(scale);
	// CHECK LIFE //
	if (iHP<0)
	{//died, stop shooting etc.
		if (dwStatus & STATUS_ALIVE) dwStatus ^= STATUS_ALIVE;
		if (dwStatus & STATUS_SHOOTER) dwStatus ^= STATUS_SHOOTER;
		if (dwStatus & STATUS_BOMBER) dwStatus ^= STATUS_BOMBER;
	}

	if (!(dwStatus & STATUS_ALIVE))
	{
		if (!explodedBy)
		{
			//PlaySound((const char*)SOUND_ID_EXPLOSION, hinstance_app, SND_RESOURCE | SND_ASYNC);
			exploding=1;
		}
		explodedBy++;
	}
}
void Alien::redraw(register RECT &rectRendered, register float &scale)
{
	//USES ALPHALOOK//_Targa2SurfaceAlphaFX(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-stunnedness, explodedBy, dwStatus);
	//_Targa2Surface24_AlphaQuickEdge_FX(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-stunnedness, explodedBy, dwStatus);

	//static int animation_seq[4] = {0,1,0,2};
	if (REAL_BPP==24)
	{
		if (z==4)
			 _Targa2Surface24_AlphaQuickEdge_FX(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-stunnedness, explodedBy, dwStatus);
		else _Targa2Surface24_AlphaQuickEdge_FX_Scaled(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-stunnedness, explodedBy, dwStatus, z/4.00);
	}
	else
	{
		if (z==4) _Targa2Surface32_AlphaQuickEdge_FX(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-stunnedness, explodedBy, dwStatus);
		else			_Targa2Surface32_AlphaQuickEdge_FX_Scaled(frames[frameNow], lpddsBack, rectRendered.left, rectRendered.top, 255-stunnedness, explodedBy, dwStatus, z/4.00);
	}

}
void Alien::hitDetect(float &scaler)
{
	//register float scaler=z/4.0f;
	for (int index=0; index<MAXSHOTS; index++)
	{
		if (shots[index] != NULL)
		{//DISTANCE FORMULA: uses centerpoints (x,y,z)
			if (abs(shots[index]->x-x) < width/2*scaler)
				if (abs(shots[index]->y-y) < height/2*scaler)
					if (abs(shots[index]->z-float(z)) < .25*scaler && (!shots[index]->isAlien))
					{
						if (dwStatus & STATUS_SHIELD)
						{//invincible with shield except boss
							if (dwStatus & STATUS_BOSS) dwStatus ^= STATUS_SHIELD; //only Boss loses shield, other shields are removed when boss dies
							//stunnedness=10;
							iHP+=8;
							delete shots[index];
							shots[index]=NULL;
							PlaySound((const char*)SOUND_ID_SHIELDZAP, hinstance_app, SND_RESOURCE | SND_ASYNC);
						}
						else
						{
							stunnedness=10;
							iHP-=16;
							delete shots[index];
							shots[index]=NULL;
							PlaySound((const char*)SOUND_ID_OUCHALIEN, hinstance_app, SND_RESOURCE | SND_ASYNC);
						}
					}
		}
	}
}
int Alien::yCenter()
{
	return (y);
}
int Alien::leftEdge()
{
	return (x-width/2*z/4.0f);
}
int Alien::rightEdge()
{
	return (x+width/2*z/4.0f);
}
void Alien::shoot()
{
	if (!shootDelay)
	{
		shootDelay=rand()%30+10;
		float scaler=z/4.0f;
		int eyePosX = x-10*scaler;
		int eyePosY = y;
		int maxX = SCREEN_WIDTH-eyePosX;
		int shotSpeed=16;
		for (int index=0; index<MAXSHOTS; index++)
		{
			if (shots[index] == NULL)
			{//create a shot at the first available shot pointer
				//Assume shoot left
				//Prototype: Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isRed2)
				shots[index] = new Shot(eyePosX, eyePosY, z, -shotSpeed, 0, 0, 1, 0);
				break;
			}
		}
	}
}
void Alien::bomb()
{
	if (!shootDelay)
	{
		shootDelay=rand()%10+10;
		float scaler=z/4.0f;
		int eyePosX = x-10*scaler;
		int eyePosY = y+height/2*scaler;
		int maxX = SCREEN_WIDTH-eyePosX;
		int shotSpeed=8;
		for (int index=0; index<MAXSHOTS; index++)
		{
			if (shots[index] == NULL)
			{//create a shot at the first available shot pointer
				//Assume shoot left
				//Prototype: Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isAlien2 , int isBomb2)
				shots[index] = new Shot(eyePosX, eyePosY, z, -shotSpeed, 4, -1, 1, 1);
				break;
			}
		}
	}

}

// GLOBAL TO HERO /////////////////////////////////
Alien				*aliens[MAXALIENS];
///////////////////////////////////////////////////

//Hero Class
class Hero
{
public:
	LPTARGA frames[21]; // 2 frames of animation for cycle
	int x, y, z; //z location 1 to 4 (2 to 4 is usage for game) mult by .25 for scale
	int moveDelay; //delays motion of z
	int shootDelay; //
	int width, height;
	DWORD dwStatus;
	int iHP;
	int stunnedness;
	int explodedBy;
	int frameNow; //used to calculate current rotation as well as frame number
	int direction; //-1=left 0=turning 1=right
	int rotationDir; //rotation: neg=left 0=done pos=right
	int verticle; //jumping variable added to y
	int v; //jumping upward velocity in pixels/s
	int xvel; //x velocity
	float gunPower; //gun power in percent, max=1
	int exploding;

	Hero();
	void refresh();
	void turn(int dir);
	void jump();
	void move(int dir);
	void moveX(int dir);
	void shoot();
	void doublespeed();
	void precisionaim();
	void drawTarget(int alienNum);
private:
	void redraw();
	void hitDetect();
	void drawMeters();
};
Hero::Hero()
{
	exploding=0;
	for (int index=0;index<=20;index++)
	{
		frames[index]=lptargaHero[index];
	}
	width=lptargaHero[0]->width;
	height=lptargaHero[0]->height;
	x = SCREEN_WIDTH/2 - width/2;
	y = SCREEN_HEIGHT*.75 - height/2;
	z = 4;
	moveDelay = 0;
	shootDelay = 0;
	dwStatus = STATUS_ALIVE;// | STATUS_ANTIGRAVITY;
	iHP = 255;
	stunnedness = 10;
	explodedBy = 0;
	frameNow=9;
	direction=0;
	rotationDir=1;
	verticle = 100*z/4.00;
	v = 0;
	xvel = 0;
	gunPower = 1;
}
void Hero::refresh()
{
// ORDER: //
	//4. UPDATE+MOVE//
	//5. FIX POSITION// -get it ready for the next set rect and next render
	//1. SET RECT//
	//2. DRAW//
	//3. HITDETECT&CHECK LIFE// -this way it is using the rectRendered for hit detection

	// UPDATE //
	//drawMeters();
	if (moveDelay) moveDelay--;
	if (shootDelay) shootDelay--;
	if (stunnedness) stunnedness--;
	//Animate if not turned all the way
	if (!direction)
	{
		frameNow+=rotationDir * ((dwStatus & STATUS_DOUBLESPEED) ? 2 : 1);
		if (frameNow<0)
		{
			frameNow=0;
			rotationDir=0;
			direction=-1;
		}
		else if (frameNow>20)
		{
			frameNow=20;
			rotationDir=0;
			direction=1;
		}
	}
	if ((dwStatus & STATUS_PRECISIONAIM) && direction==0) direction = 2;
	//re-charge gun
	if (gunPower<1)
	{
		gunPower+= (dwStatus & STATUS_DOUBLESPEED) ? .066 : .033;
		if (gunPower>1) gunPower=1;
	}
	// MOVE //
	//(Calculate PHYSICS)
	//about 80 pixels per meter (actual hero is 150 pixels high inside the Targa)
	//30 frames per second
	//gravitational acceleration is 1.8 m/s/s
	verticle += float(v) * z/4.0;
	v-=.8*80/30; //only .8 gravitational acceleration, for high jumping (/pixel/m /frame/s)
	if (verticle<=0)
	{//hit ground
		v=0; //reset velocity
		verticle=0; //make sure we aren't below ground
		xvel=0;
	}
	if (verticle)
	{
		x+=xvel;
	}


	// FIX POSITION //
	if (verticle>200*z/4.00)//(y-verticle < 0)
	{//make sure you don't fly too high
		verticle=200*z/4.00;
		v=0;
	}
	if (x<0) x=0;
	if (x>SCREEN_WIDTH-width*z/4) x=(SCREEN_WIDTH-width*z/4);


	redraw();
	//drawTarget;
	//if (boolShowTarget) drawTarget();

	// HITDETECT //
	if (!stunnedness) hitDetect();
	// CHECK LIFE //
	if (iHP<=0) dwStatus = 0; //died, stop other special abilities too
	if (!(dwStatus & STATUS_ALIVE))
	{
		explodedBy++;
		if (!exploding)
		{
			PlaySound((const char*)SOUND_ID_EXPLOSION, hinstance_app, SND_RESOURCE | SND_ASYNC);
			exploding=1;
		}
	}
}
void Hero::turn(int dir)
{
					if (!verticle)moveX(dir);
	if (dir==-1 || dir==1 || dir==2) //ignore bad arguments
	{
		if (direction) //if not turning
		{
			if (dir==direction)
			{

			}
			else
			{
				direction=0; //set character in state of motion
				if (dir>direction) rotationDir=1;
				else if (dir<direction)	rotationDir=-1;
			}
		}
		else //moving, so see if direction in argument is different than rotation direction
		{
			//if (dir == 2 && (frameNow<=0 || frameNow>=20)) rotationDir = (frameNow<=0) ? -1 : 1;
			if (dir != rotationDir) rotationDir= -rotationDir;
		}
	}
}
void Hero::jump()
{
	if (!moveDelay)
	{
		if ((!verticle) || (dwStatus & STATUS_ANTIGRAVITY)) //don't unless not jumping or if ANTIGRAVITY status
		{
			v=(dwStatus & STATUS_ANTIGRAVITY) ? 5 :30;
			if (dwStatus & STATUS_DOUBLESPEED) v *= 2;
			if (!(dwStatus & STATUS_ANTIGRAVITY)) //if no antigravity, delay movement in air
				moveDelay= (dwStatus & STATUS_DOUBLESPEED) ? 15 : 30;
		}
	}
}
void Hero::move(int dir)
{
	if (!moveDelay)
	{
		if (dir<0)
		{
			if (z>1)
			{	x+=17;
				y-=z*10;
				z--;
			}
		}
		else
		{
			if (z<4)
			{	z++;
				y+=z*10;
				x-=17;
			}
		}
		moveDelay = 3;
	}
}
void Hero::moveX(int dir)
{
	x += (dwStatus & STATUS_DOUBLESPEED) ? dir*10+z*2*dir : dir*5+z*2*dir;
	xvel = (dwStatus & STATUS_DOUBLESPEED) ? dir*10+z*2*dir : dir*5+z*2*dir;
	//moveDelay = (dwStatus & STATUS_DOUBLESPEED)? 1 : 2;
}
void Hero::shoot()
{
	if ((!shootDelay) && gunPower>=.45)
	{
		gunPower-=.45;
		shootDelay=10;
		float scaler=z/4.0f;
		int eyePosX = x+97-(4-z)*25; //position of hero's eye to shoot blast from
		int eyePosY = y+24-(4-z)*5 - verticle -abs(10-frameNow)/4*scaler + 4*scaler - (4-z);
		int maxX = SCREEN_WIDTH-eyePosX;
		int shotSpeed=16;
		for (index=0; index<MAXSHOTS; index++)
		{
			if (shots[index] == NULL)
			{//create a shot at the first available shot pointer
				if (direction==-1) //shoot left
					//Prototype: Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isRed2)
					shots[index] = new Shot(eyePosX-20*scaler, eyePosY, z, direction*shotSpeed, 0, 0, 0, 0);
				else if (direction==1) //shoot right
					shots[index] = new Shot(eyePosX, eyePosY, z, direction*shotSpeed, 0, 0, 0, 0);
				else //shoot downward
					shots[index] = new Shot(eyePosX-20*scaler+scaler*frameNow, eyePosY, z, (frameNow-10)*6, 20, shotSpeed, 0, 0);
				break;
				//PlaySound((const char*)SOUND_ID_LASER, hinstance_app, SND_RESOURCE | SND_ASYNC);
			}
		}
	}
}
void Hero::redraw()
{
	//USES ALPHALOOK//_Targa2SurfaceAlphaFX(frames[frameNow], lpddsBack, x, y-verticle, 255-stunnedness, explodedBy, dwStatus);
	//_Targa2Surface24_AlphaQuickEdge_FX(frames[frameNow], lpddsBack, x, y-verticle, 255-stunnedness, explodedBy, dwStatus);

	if (REAL_BPP==24)
	{
		if (z==4)
			 _Targa2Surface24_AlphaQuickEdge_FX(frames[frameNow], lpddsBack, x, y-verticle, 255-stunnedness, explodedBy, dwStatus);
		else _Targa2Surface24_AlphaQuickEdge_FX_Scaled(frames[frameNow], lpddsBack, x, y-verticle, 255-stunnedness, explodedBy, dwStatus, z/4.00);
	}
	else
	{
		if (z==4) _Targa2Surface32_AlphaQuickEdge_FX(frames[frameNow], lpddsBack, x, y-verticle, 255-stunnedness, explodedBy, dwStatus);
		else		_Targa2Surface32_AlphaQuickEdge_FX_Scaled(frames[frameNow], lpddsBack, x, y-verticle, 255-stunnedness, explodedBy, dwStatus, z/4.00);
	}

}
void Hero::drawMeters()
{
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	lpddsBack->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);

	//register DWORD *this_buffer = (DWORD *)ddsd.lpSurface;
	register UCHAR *byteBuffer = (UCHAR *)ddsd.lpSurface;
	//register DWORD pixel;// = _RGB32BIT(255,255,255,255); //remember this MACRO is ARGB unlike actual screen
	register int toX=2, toY=SCREEN_HEIGHT-3;
	for (int index=0; index<=gunPower*100; index++)
	{
		//pixel=_RGB32BIT(255,55+index*2,55+index*2,155+index);
		for (register int offset=0; offset<=13; offset++)
		{
			//this_buffer[toX + ((toY)*ddsd.lPitch >> 2)] = pixel;
			//this_buffer[int(index*100)] = pixel;
			for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
				byteBuffer[(toX + offset)*REAL_BYTEDEPTH + (toY-index)*ddsd.lPitch + byteNow] = 255-130+offset*10-index;

		}
	}

	toX=18;
	toY=SCREEN_HEIGHT-2;
/*
	pixel=_RGB32BIT(255,255,UCHAR(iHP*.85),0);
	for (index=0; index<iHP; index++)
	{
		this_buffer[toX + index + ((toY)*ddsd.lPitch >> 2)] = pixel;
		//for (byteNow
	}
*/
	for (index=0; index<iHP; index++)
	{

		for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
			byteBuffer[(toX+index)*REAL_BYTEDEPTH + toY*ddsd.lPitch + byteNow] = (byteNow==3) ? 255 : (byteNow==2) ? 255 : iHP;
	}

	lpddsBack->Unlock(NULL);
}
void Hero::drawTarget(int alienNum)
{
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	lpddsBack->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);

	//for (int alienNow = 0; alienNow < MAXALIENS; alienNow++)
	//{
		//if (aliens[alienNum] != NULL)

		if (aliens[index]->z==z) if (aliens[index]->x>5) if (aliens[index]->x<SCREEN_WIDTH-6)
		{

			//register DWORD *this_buffer = (DWORD *)ddsd.lpSurface;
			register UCHAR *byteBuffer = (UCHAR *)ddsd.lpSurface;
			//register DWORD pixel;// = _RGB32BIT(255,255,255,255); //remember this MACRO is ARGB unlike actual screen
			register int toX=aliens[index]->x-3, toY=aliens[index]->y-3;

			for (int index=0; index<7; index++)
			{

				for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
				{//draw a crosshairs
					byteBuffer[(toX+index)*REAL_BYTEDEPTH + (toY+3)*ddsd.lPitch + byteNow] = (byteNow==1)? 255 : 0; //across
					byteBuffer[(toX+3)*REAL_BYTEDEPTH + (toY+index)*ddsd.lPitch + byteNow] = (byteNow==1)? 255 : 0; //down
				}
			}

/*			toX=aliens[index]->x;
			toY=aliens[index]->y-2;
			for (index=0; index<5; index++)
			{

				for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
					byteBuffer[(toX)*REAL_BYTEDEPTH + (toY+index)*ddsd.lPitch + byteNow] = (byteNow==1)? 255 : 0;
			}
*/

		}
	//}


	lpddsBack->Unlock(NULL);
}
void Hero::hitDetect()
{
	register float scaler=z/4.0f;
	int xCenter=x+width/2*scaler;
	int yCenter=y-verticle+height/2*scaler;

	//HIT ALIEN:
	for (int index=0; index<MAXALIENS; index++)
	{

		if (aliens[index] != NULL)
		{//CENTERPOINT DISTANCE-BASED
		/*
			if ( abs(aliens[index]->x-50-xCenter) < 100) //100 IS SLOPPY BUT USSIVL DIST -50 SO HIT IS NEAR FRONT
				if ( abs(aliens[index]->y-yCenter) < 80) //80 IS SLOPPY BUT USSIVL DIST
					if (aliens[index]->z==z)
					{
						PlaySound((const char*)SOUND_ID_OUCHZAP, hinstance_app, SND_RESOURCE | SND_ASYNC);
						stunnedness=10;
						iHP-=50;
					}
		*/
		//RECT-BASED
			int widthMod = 80*scaler;
			if (aliens[index]->rightEdge()>x+widthMod+widthMod)
				if (aliens[index]->leftEdge()<x+width*scaler-widthMod)
					if (aliens[index]->yCenter()>y-verticle)
						if (aliens[index]->yCenter()<y-verticle+height*scaler)
							if (aliens[index]->z==z)
							{
								if (dwStatus & STATUS_SHIELD)
								{
									dwStatus ^= STATUS_SHIELD;
									PlaySound((const char*)SOUND_ID_OUCHZAP, hinstance_app, SND_RESOURCE | SND_ASYNC);
									stunnedness=10;
									iHP+=50;
								}
								else
								{
									PlaySound((const char*)SOUND_ID_OUCHZAP, hinstance_app, SND_RESOURCE | SND_ASYNC);
									stunnedness=10;
									iHP-=50;
								}
							}
		}
	}
	//HIT BULLET: edit the shot::hitDetect to do this instead maybe
	for (index=0; index<MAXSHOTS; index++)
	{
		if (shots[index] != NULL)
		{//DISTANCE FORMULA: uses centerpoints (x,y,z)
			if (abs(shots[index]->x-xCenter) < width/2*scaler)
				if (abs(shots[index]->y-yCenter) < height/2*scaler)
					if (abs(shots[index]->z-float(z)) < .25*scaler && (shots[index]->isAlien))
					{
						if (dwStatus & STATUS_SHIELD)
						{
							dwStatus ^= STATUS_SHIELD;
							stunnedness=10;
							iHP+=15;
							delete shots[index];
							shots[index]=NULL;
							PlaySound((const char*)SOUND_ID_OUCHZAP, hinstance_app, SND_RESOURCE | SND_ASYNC);
						}
						else
						{
							stunnedness=10;
							iHP-=15;
							delete shots[index];
							shots[index]=NULL;
							PlaySound((const char*)SOUND_ID_OUCHZAP, hinstance_app, SND_RESOURCE | SND_ASYNC);
						}
					}
		}
	}
}
void Hero::doublespeed()
{
	if (!(dwStatus & STATUS_DOUBLESPEED))
	{
	dwStatus |= STATUS_DOUBLESPEED;
	PlaySound((const char*)SOUND_ID_TRUMPET, hinstance_app, SND_RESOURCE | SND_ASYNC);
	}
}
void Hero::precisionaim()
{
	if (!(dwStatus & STATUS_PRECISIONAIM))
	{
	dwStatus |= STATUS_PRECISIONAIM;
	PlaySound((const char*)SOUND_ID_TRUMPET, hinstance_app, SND_RESOURCE | SND_ASYNC);
	}
}

// GLOBAL CLASS POINTERS ////////////////////////////////////////////
Hero				*hero = NULL;

// FUNCTIONS ////////////////////////////////////////////////
LPDIRECTDRAWSURFACE4 DDrawCreateSurface(int width, int height, int memflags)
{
	DDSURFACEDESC2 ddsd;				 // working description
	LPDIRECTDRAWSURFACE4 lpdds;	// temporary surface

		// set to access caps, width, and height
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize	= sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;// | DDSD_ALPHABITDEPTH;
	//ddsd.dwAlphaBitDepth = 8;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;
	// set surface to offscreen plain
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | memflags;


	if (FAILED(lpdd4->CreateSurface(&ddsd, &lpdds, NULL)))
		return(0);

	return(lpdds);
}

//////////////////////////////////////////////////////////////////////

LPDIRECTDRAWCLIPPER DDrawAttachClipper(LPDIRECTDRAWSURFACE4 lpdds,
																				 int numRects,
																				 LPRECT clipList)
{
	// this function creates a clipper from the sent clip list and attaches
	// it to the sent surface

	//int index;												 // looping var
	LPDIRECTDRAWCLIPPER lpddClipper;	 // pointer to the newly created dd clipper
	LPRGNDATA regionData;						 // pointer to the region data that contains
																		 // the header and clip list

	// first create the direct draw clipper
	if (FAILED(lpdd4->CreateClipper(0,&lpddClipper,NULL)))
		 return(NULL);

	// now create the clip list from the sent data

	// first allocate memory for region data
	regionData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+numRects*sizeof(RECT));

	// now copy the rects into region data
	memcpy(regionData->Buffer, clipList, sizeof(RECT)*numRects);

	// set up fields of header
	regionData->rdh.dwSize					= sizeof(RGNDATAHEADER);
	regionData->rdh.iType					 = RDH_RECTANGLES;
	regionData->rdh.nCount					= numRects;
	regionData->rdh.nRgnSize				= numRects*sizeof(RECT);

	regionData->rdh.rcBound.left		=	64000;
	regionData->rdh.rcBound.top		 =	64000;
	regionData->rdh.rcBound.right	 = -64000;
	regionData->rdh.rcBound.bottom	= -64000;

	// find bounds of all clipping regions
	for (index=0; index<numRects; index++)
			{
			// test if the next rectangle unioned with the current bound is larger
			if (clipList[index].left < regionData->rdh.rcBound.left)
				 regionData->rdh.rcBound.left = clipList[index].left;

			if (clipList[index].right > regionData->rdh.rcBound.right)
				 regionData->rdh.rcBound.right = clipList[index].right;

			if (clipList[index].top < regionData->rdh.rcBound.top)
				 regionData->rdh.rcBound.top = clipList[index].top;

			if (clipList[index].bottom > regionData->rdh.rcBound.bottom)
				 regionData->rdh.rcBound.bottom = clipList[index].bottom;

			} // end for index

	// now we have computed the bounding rectangle region and set up the data
	// now let's set the clipping list

	if (FAILED(lpddClipper->SetClipList(regionData, 0)))
		 {
		 // release memory and return error
		 free(regionData);
		 return(NULL);
		 } // end if

	// now attach the clipper to the surface
	if (FAILED(lpdds->SetClipper(lpddClipper)))
		 {
		 // release memory and return error
		 free(regionData);
		 return(NULL);
		 } // end if

	// all is well, so release memory and send back the pointer to the new clipper
	free(regionData);

	return(lpddClipper);
}

///////////////////////////////////////////////////////////

int DDrawFillSurface(LPDIRECTDRAWSURFACE4 lpdds,int color)
{
	DDBLTFX ddbltfx; // this contains the DDBLTFX structure

	// clear out the structure and set the size field
	DDRAW_INIT_STRUCT(ddbltfx);




	// ready to blt to surface
	lpdds->Blt(NULL,			 // ptr to dest rectangle
					 NULL,			 // ptr to source surface, NA
					 NULL,			 // ptr to source rectangle, NA
					 DDBLT_COLORFILL | DDBLT_WAIT,	 // fill and wait
					 &ddbltfx);	// ptr to DDBLTFX structure

	// return success
	return(1);
} // end DDrawFillSurface

//////////////////////////////////////////////////////////////

int DDrawDrawSurface(LPDIRECTDRAWSURFACE4 source, // source surface to draw
											int x, int y,								 // position to draw at
											int width, int height,				// size of source surface
											LPDIRECTDRAWSURFACE4 dest)		// surface to draw the surface on
{
	// draw the surface at the x,y defined by dest, note that we are sending
	// the size of the surface, we could query for it, but that takes time
	// basically, we are really lacking datastructure as this point, since
	// you would create a datastructure that keep important info about the
	// surface, so you did't have to query it from directdraw
	//DDBLTFX	ddbltfx; // this contains the DDBLTFX structure

	// clear out the structure and set the size field
	//DDRAW_INIT_STRUCT(ddbltfx);
	//ddbltfx.dwDDFX = DDBLT_ALPHADEST | DDBLT_ALPHASRC;

	RECT dest_rect,	 // the destination rectangle
		source_rect; // the source rectangle

	// fill in the destination rect
	dest_rect.left	 = x;
	dest_rect.top		= y;
	dest_rect.right	= x+width-1;
	dest_rect.bottom = y+height-1;

	// fill in the source rect
	source_rect.left		= 0;
	source_rect.top		 = 0;
	source_rect.right	 = width-1;
	source_rect.bottom	= height-1;

	//This is a plain Blt without FX
	if (FAILED(dest->Blt(&dest_rect, source,
						&source_rect,(DDBLT_WAIT),
						NULL)))
		return(0);





/*	//Code for key color:
	// test transparency flag
	if (transparent)
	{
		// enable color key blit
		// blt to destination surface
		if (FAILED(dest->Blt(&dest_rect, source,
							&source_rect,(DDBLT_WAIT | DDBLT_KEYSRC),
																	NULL)))
			return(0);

	} // end if
	else
	{
		// perform blit without color key
		// blt to destination surface
		if (FAILED(dest->Blt(&dest_rect, source,
							&source_rect,(DDBLT_WAIT),
														NULL)))
			return(0);

	} // end if
*/
	// return success
	return(1);

} // end DDrawDrawSurface

//////////////////////////////////////////////////////////////

int DDrawDrawSurfaceScaled(LPDIRECTDRAWSURFACE4 source, // source surface to draw
											int x, int y,								 // position to draw at
											int width_src, int height_src,// size of source surface
											int width_dest, int height_dest,// size of dest surface
											LPDIRECTDRAWSURFACE4 dest)		// surface to draw the surface on
{
// draw the surface at the x,y defined by dest, send both the original
// source size of surface, along with the desired size, if they are
// different then directdraw will scale the bitmap for you
// note that we are sending
// the size of the surface, we could query for it, but that takes time
// basically, we are really lacking datastructure as this point, since
// you would create a datastructure that keep important info about the
// surface, so you did't have to query it from directdraw


	RECT dest_rect,	 // the destination rectangle
		source_rect; // the source rectangle

	// fill in the destination rect
	dest_rect.left	 = x;
	dest_rect.top		= y;
	dest_rect.right	= x+width_dest-1;
	dest_rect.bottom = y+height_dest-1;

	// fill in the source rect
	source_rect.left		= 0;
	source_rect.top		 = 0;
	source_rect.right	 = width_src-1;
	source_rect.bottom	= height_src-1;

	// perform blit without color key
	// blt to destination surface
	if (FAILED(dest->Blt(&dest_rect, source,
										 &source_rect,(DDBLT_WAIT),
										 NULL)))
		return(0);

	// return success
	return(1);

} // end DDrawDrawSurfaceScaled

//////////////////////////////////////////////////////////////

int _Targa2Surface32_Alpha(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY)//, int opacityCap)
{
	// copy the Targa image to the primary buffer line by line
	DDSURFACEDESC2 ddsd;	//	direct draw surface description
	// set size of the structure
	ddsd.dwSize = sizeof(ddsd);

	// lock the primary surface
	lpddsThis1->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);

	// get video pointer to primary surfce
	register DWORD *this_buffer = (DWORD *)ddsd.lpSurface;

	register UCHAR *dPtr = (UCHAR*)&this_buffer[toX + (toY*ddsd.lPitch >> 2)]; //dest pointer
	register UCHAR *sPtr = image->buffer;														 //source pointer

	register UCHAR alpha,blue,green,red;
	register DWORD pixel;
	//int adddPos=ddsd.lPitch-nWidth*4;
	//register int adddPos=ddsd.lPitch >> 2;//-image->width*4; //in case not linear
	//int addsPos=lPitch-nWidth*4;
	//register int addsPos=1 ;//assume linear

	// process each line and copy it into the primary buffer
	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			/*
			//Get BGR values and alpha
			UCHAR blue	= (image->buffer[index_y*image->width*4 + index_x*4 + 0]),
				green = (image->buffer[index_y*image->width*4 + index_x*4 + 1]),
				red	 = (image->buffer[index_y*image->width*4 + index_x*4 + 2]),
				alpha = (image->buffer[index_y*image->width*4 + index_x*4 + 3]);
			*/


			blue=*sPtr++;
			green=*sPtr++;
			red=*sPtr++;
			alpha=*sPtr++;

			//if (alpha>opacityCap) alpha=opacityCap; //"Cap" opacity off at opacityCap to create transparency if not 255

			if (alpha==0)
			{
				dPtr+=4;
			}
			else if (alpha==127)
			{
				//Do quick average
				blue = (*dPtr + blue)/2;
				dPtr++;
				green = (*dPtr + green)/2;
				dPtr++;
				red = (*dPtr + red)/2;
				dPtr+=2; //increment past alpha
				pixel = _RGB32BIT(256,red,green,blue);
				//Manual clip and set pixel:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								this_buffer[toX + index_x + ((index_y+toY)*ddsd.lPitch >> 2)] = pixel;

			}
			else if (alpha==255)
			{
				dPtr+=4;
				pixel = _RGB32BIT(256,red,green,blue);
				//Manual clip and set pixel:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								this_buffer[toX + index_x + ((index_y+toY)*ddsd.lPitch >> 2)] = pixel;
			}
			else
			{
				//Do alpha, FORMULA: ((Source-Dest)*alpha/256+Dest)
				register float cookedAlpha = alpha/256.00;
				blue = ((blue - *dPtr) * cookedAlpha + *dPtr);
				dPtr++;
				green = ((green - *dPtr) * cookedAlpha + *dPtr);
				dPtr++;
				red = ((red - *dPtr) * cookedAlpha + *dPtr);
				dPtr+=2; //increment past alpha
				pixel = _RGB32BIT(256,red,green,blue);
				//DWORD alphaPix = _RGB32BIT(1,alpha/255,alpha/255,alpha/255);
				//Manual clip and set pixel:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								this_buffer[toX + index_x + ((index_y+toY)*ddsd.lPitch >> 2)] = pixel;
			}

		} // end for index_x
		dPtr+=ddsd.lPitch-image->width*4;
	} // end for index_y

	// now unlock the surface
	if (FAILED(lpddsThis1->Unlock(NULL)))
		return(0);
	return(1);
} // end _Targa2Surface32_Alpha

///////////////////////////////////////////////////////////////

int _Targa2Surface32_AlphaQuickEdge_FX(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat)
{
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	lpddsThis1->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	register DWORD *this_buffer = (DWORD *)ddsd.lpSurface;
	register UCHAR *dPtr = (UCHAR*)&this_buffer[toX + (toY*ddsd.lPitch >> 2)];
	register UCHAR *sPtr = image->buffer;
	register UCHAR blue,green,red,alpha;
	register DWORD pixel;
	register int tox1 = toX; //save original
	register int explod1 = explodedness; //save original

/*
	if (explodedness)
	{//this keeps the exploded object red but solid until after frame 3
		if (explodedness<=3)
			explodedness=0;
		else
			explodedness-=3;
	}
*/
	if (explodedness) explodedness--; //start explosion just as a red dude, not blown up yet at 1

	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		if (index_y%2) //splits object into squares
			toY+=explodedness;
		toX=tox1; //reset offset before exploding next line of image

		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			if (index_x%2) //splits object into squares
				toX+=explodedness;

			blue=*sPtr++;
			green=*sPtr++;
			red=*sPtr++;
			alpha=*sPtr++;
			if (explod1 || 255-opacityCap) //this use of opacity cap is unique to DXMan, makes it red when transparent
			{
				green>>=1;
				blue>>=2;
			}

			if (alpha>opacityCap) alpha=opacityCap; //"Cap" opacity off at opacityCap to create transparency if not 255

			if (alpha<85)
			{
				dPtr+=4;
			}
			else if (alpha>171)
			{	dPtr+=4;

				//register int scaledX = (scale==1) ? index_x : index_x*scale,
				//			 scaledY = (scale==1) ? index_y : index_y*scale;
				//Manual clip	and set pixel:
				if (index_x + toX<SCREEN_WIDTH)
					if (index_x + toX>=0)
						if (index_y + toY>=0)
							if (index_y + toY<SCREEN_HEIGHT)
							{
								pixel = _RGB32BIT(256,red,green,blue);
								this_buffer[index_x + toX + ((index_y + toY)*ddsd.lPitch >> 2)] = pixel;
							}
			}
			else
			{//blend
				if (dwStat & STATUS_SHIELD)
				{//blue outline
					blue = 192;
					green = 64;
				}
				else
				{//actual blend
					blue = (*dPtr + blue)/2;
					dPtr++;
					green = (*dPtr + green)/2;
					dPtr++;
					red = (*dPtr + red)/2;
					dPtr+=2;
				}

				//register int scaledX = (scale==1) ? index_x : index_x*scale,
				//			 scaledY = (scale==1) ? index_y : index_y*scale;
				//Manual clip and set pixel:
				if (index_x + toX<SCREEN_WIDTH)
					if (index_x + toX>=0)
						if (index_y + toY>=0)
							if (index_y + toY<SCREEN_HEIGHT)
							{
								pixel = _RGB32BIT(256,red,green,blue);
								this_buffer[index_x + toX + ((index_y + toY)*ddsd.lPitch >> 2)] = pixel;
							}
			}
		}
		dPtr+=ddsd.lPitch-image->width*4;
	}
	if (FAILED(lpddsThis1->Unlock(NULL)))
		return(0);
	return(1);
} // end _Targa2Surface32_AlphaQuickEdge_FX

////////////////////////////////////////////////////////////////////////////////

int _Targa2Surface32_AlphaQuickEdge_FX_Scaled(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat, float scale)
{
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	lpddsThis1->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	register DWORD *this_buffer = (DWORD *)ddsd.lpSurface;
	register UCHAR *dPtr = (UCHAR*)&this_buffer[toX + (toY*ddsd.lPitch >> 2)];
	register UCHAR *sPtr = image->buffer;
	register UCHAR blue,green,red,alpha;
	register DWORD pixel;
	register int tox1 = toX; //save original
	register int explod1 = explodedness; //save original

	if (explodedness) explodedness--; //start explosion just as a red dude, not blown up yet at frame 1

	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		if (index_y%2) //splits object into squares
			toY+=explodedness;
		toX=tox1; //reset offset before exploding next line of image

		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			if (index_x%2) //splits object into squares
				toX+=explodedness;

			blue=*sPtr++;
			green=*sPtr++;
			red=*sPtr++;
			alpha=*sPtr++;
			if (explod1 || 255-opacityCap) //this use of opacity cap is unique to DXMan, makes it red when transparent
			{
				green>>=1;
				blue>>=2;
			}

			if (alpha>opacityCap) alpha=opacityCap; //"Cap" opacity off at opacityCap to create transparency if not 255

			if (alpha<85)
			{//transparent
				dPtr+=4;
			}
			else if (alpha>171)
			{//opaque
				dPtr+=4;

				register int scaledX = (scale==1) ? index_x : index_x*scale,
							 scaledY = (scale==1) ? index_y : index_y*scale;
				//Manual clip	and set pixel:
				if (scaledX+toX<SCREEN_WIDTH)
					if (scaledX+toX>=0)
						if (scaledY+toY>=0)
							if (scaledY+toY<SCREEN_HEIGHT)
							{
								pixel = _RGB32BIT(256,red,green,blue);
								this_buffer[toX + scaledX + ((scaledY+toY)*ddsd.lPitch >> 2)] = pixel;
							}
			}
			else
			{//blend
				if (dwStat & STATUS_SHIELD)
				{//blue outline
					blue = 192;
					green = 64;
				}
				else
				{//actual blend
					blue = (*dPtr + blue)/2;
					dPtr++;
					green = (*dPtr + green)/2;
					dPtr++;
					red = (*dPtr + red)/2;
					dPtr+=2;
				}

				register int scaledX = (scale==1) ? index_x : index_x*scale,
							 scaledY = (scale==1) ? index_y : index_y*scale;
				//Manual clip and set pixel:
				if (scaledX+toX<SCREEN_WIDTH)
					if (scaledX+toX>=0)
						if (scaledY+toY>=0)
							if (scaledY+toY<SCREEN_HEIGHT)
							{
								pixel = _RGB32BIT(256,red,green,blue);
								this_buffer[toX + scaledX + ((scaledY+toY)*ddsd.lPitch >> 2)] = pixel;
							}
			}
		}
		dPtr+=ddsd.lPitch-image->width*4;
	}
	if (FAILED(lpddsThis1->Unlock(NULL)))
		return(0);
	return(1);
} // end _Targa2Surface32_AlphaQuickEdge_FX_Scaled

///////////////////////////////////////////////////////////////////////////////

int Targa2Surface32(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY)
{
	// copy the Targa image to the primary buffer line by line
	DDSURFACEDESC2 ddsd;	//	direct draw surface description
	// set size of the structure
	ddsd.dwSize = sizeof(ddsd);

	// lock the primary surface
	lpddsThis1->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);

	// get video pointer to primary surfce
	DWORD *this_buffer = (DWORD *)ddsd.lpSurface;

	// process each line and copy it into the primary buffer
	for (int index_y = 0; index_y < image->height; index_y++)
	{
		for (int index_x = 0; index_x < image->width; index_x++)
		{
			// get BGR values
			UCHAR blue	= (image->buffer[index_y*image->width*4 + index_x*4 + 0]),
				green = (image->buffer[index_y*image->width*4 + index_x*4 + 1]),
				red	 = (image->buffer[index_y*image->width*4 + index_x*4 + 2]),
				alpha = (image->buffer[index_y*image->width*4 + index_x*4 + 3]);

			// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
			DWORD pixel = _RGB32BIT(alpha,red,green,blue);

			// write the pixel
			this_buffer[toX + index_x + ((index_y+toY)*ddsd.lPitch >> 2)] = pixel;

		} // end for index_x

	} // end for index_y

	// now unlock the surface
	if (FAILED(lpddsThis1->Unlock(NULL)))
		return(0);
	return(1);
} //end Targa2Surface32

////////////////////////////////////////////////////////////////////////////////

int _Targa2Surface24_Alpha(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY)//, int opacityCap)
{
	DDSURFACEDESC2 ddsd;	//	direct draw surface description
	ddsd.dwSize = sizeof(ddsd);
	lpddsThis1->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	// get video pointer to primary surfce
	//register DWORD *this_buffer = (DWORD *)ddsd.lpSurface;

	register UCHAR *dPtr = (UCHAR *)ddsd.lpSurface;//(UCHAR*)&this_buffer[toX + (toY*ddsd.lPitch >> 2)]; //dest pointer
	register UCHAR *sPtr = (UCHAR *)image->buffer;														 //source pointer
	register UCHAR *sPtrAlpha = (UCHAR *)image->buffer;
	sPtrAlpha+=3; //move pointer to position of alpha byte
	register int draw=0;
	register float cookedAlpha=1;



	//register UCHAR alpha,blue,green,red;
	//register DWORD pixel;

	//int adddPos=ddsd.lPitch-nWidth*4;
	//register int adddPos=ddsd.lPitch >> 2;//-image->width*4; //in case not linear
	//int addsPos=lPitch-nWidth*4;
	//register int addsPos=1 ;//assume linear

	//Move the dest pointer to the starting pixel
	dPtr+=toX*3;
	dPtr+=toY*ddsd.lPitch;

	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			draw=0;
			/*
			//Get BGR values and alpha
			UCHAR blue	= (image->buffer[index_y*image->width*4 + index_x*4 + 0]),
				green = (image->buffer[index_y*image->width*4 + index_x*4 + 1]),
				red	 = (image->buffer[index_y*image->width*4 + index_x*4 + 2]),
				alpha = (image->buffer[index_y*image->width*4 + index_x*4 + 3]);
			*/

			//if (alpha>opacityCap) alpha=opacityCap; //"Cap" opacity off at opacityCap to create transparency if not 255

			if (*sPtrAlpha==0)
			{//skip
				dPtr+=3;
				sPtr+=4;
				sPtrAlpha+=4;
			}
			else if (*sPtrAlpha==127)
			{//half-transparent
				//Manual clip:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								draw=1;
				if (draw)
				{
					//Do quick average
					*dPtr = (*dPtr + *sPtr)/2;
					dPtr++;
					sPtr++;
					*dPtr = (*dPtr + *sPtr)/2;
					dPtr++;
					sPtr++;
					*dPtr = (*dPtr + *sPtr)/2;
					dPtr++;
					sPtr+=2; //increment past alpha

				}
				else
				{
					dPtr+=3;
					sPtr+=4;
				}

				sPtrAlpha+=4;
			}
			else if (*sPtrAlpha==255)
			{//Solid
				//Manual clip:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								draw=1;
				if (draw)
				{
					*dPtr = *sPtr;
					dPtr++;
					sPtr++;
					*dPtr = *sPtr;
					dPtr++;
					sPtr++;
					*dPtr = *sPtr;
					dPtr++;
					sPtr++;
					sPtr++; //skip alpha
				}
				else
				{
					dPtr+=3;
					sPtr+=4;
				}

				sPtrAlpha+=4;
			}
			else
			{//Do Alpha Formula
				//Manual clip:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								draw=1;
				if (draw)
				{
					cookedAlpha = *sPtrAlpha/255.00;
					*dPtr = ((*sPtr - *dPtr) * cookedAlpha + *dPtr);
					dPtr++;	sPtr++;
					*dPtr = ((*sPtr - *dPtr) * cookedAlpha + *dPtr);
					dPtr++; sPtr++;
					*dPtr = ((*sPtr - *dPtr) * cookedAlpha + *dPtr);
					dPtr++;
					sPtr+=2;
				}
				else
				{
					dPtr+=3;
					sPtr+=4;
				}

				sPtrAlpha+=4;
			}
		} // end for index_x
		dPtr+=ddsd.lPitch - image->width*3;// - toX*3;
	} // end for index_y

	if (FAILED(lpddsThis1->Unlock(NULL)))
		return(0);
	return(1);
} // end _Targa2Surface24_Alpha
////////////////////////////////////////////////////////////////////////////////

int _Targa2SurfaceAlphaFX(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat)
{//uses alpha lookup
	DDSURFACEDESC2 ddsd;	//	direct draw surface description
	ddsd.dwSize = sizeof(ddsd);
	lpddsThis1->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	// get video pointer to primary surfce
	//register DWORD *this_buffer = (DWORD *)ddsd.lpSurface;

	register UCHAR *dPtr = (UCHAR *)ddsd.lpSurface;//(UCHAR*)&this_buffer[toX + (toY*ddsd.lPitch >> 2)]; //dest pointer
	register UCHAR *sPtr = (UCHAR *)image->buffer;														 //source pointer
	register UCHAR *sPtrAlpha = (UCHAR *)image->buffer;
	sPtrAlpha+=3; //move pointer to position of alpha byte
	register int draw=0;
	register float cookedAlpha=1;



	//register UCHAR alpha,blue,green,red;
	//register DWORD pixel;

	//int adddPos=ddsd.lPitch-nWidth*4;
	//register int adddPos=ddsd.lPitch >> 2;//-image->width*4; //in case not linear
	//int addsPos=lPitch-nWidth*4;
	//register int addsPos=1 ;//assume linear

	//Move the dest pointer to the starting pixel
	dPtr+=toX*REAL_BYTEDEPTH;
	dPtr+=toY*ddsd.lPitch;
	int offset = ddsd.lPitch - image->width*REAL_BYTEDEPTH;

	//int scalerAdder=5-scale*4 //ADD THIS TO indexES INSTEAD OF ++, AND MULTIPLY USES OF width BY scale

	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			draw=0;	//for clipping
			/*
			//Get BGR values and alpha
			UCHAR blue	= (image->buffer[index_y*image->width*4 + index_x*4 + 0]),
				green = (image->buffer[index_y*image->width*4 + index_x*4 + 1]),
				red	 = (image->buffer[index_y*image->width*4 + index_x*4 + 2]),
				alpha = (image->buffer[index_y*image->width*4 + index_x*4 + 3]);
			*/

			//if (alpha>opacityCap) alpha=opacityCap; //"Cap" opacity off at opacityCap to create transparency if not 255

			if (*sPtrAlpha==0)
			{//skip
				dPtr+=REAL_BYTEDEPTH;
				sPtr+=4;
				sPtrAlpha+=4;
			}
			else if (*sPtrAlpha==255)
			{//Solid
				//Manual clip:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								draw=1;
				if (draw)
				{
					*dPtr = *sPtr;
					dPtr++;
					sPtr++;
					*dPtr = *sPtr;
					dPtr++;
					sPtr++;
					*dPtr = *sPtr;
					dPtr++;
					sPtr++;

					sPtr++; //skip alpha
					if (REAL_BYTEDEPTH==4) dPtr++; //if exist, skip alpha

				}
				else
				{
					dPtr+=REAL_BYTEDEPTH;
					sPtr+=4;
				}

				sPtrAlpha+=4;
			}
			else
			{//Do Alpha Formula (use lookup instead of calculating)
				//Manual clip:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								draw=1;
				if (draw)
				{
				/*
					//Calculate value by alpha
					cookedAlpha = *sPtrAlpha/255.00;
					*dPtr = ((*sPtr - *dPtr) * cookedAlpha + *dPtr);
					dPtr++;	sPtr++;
					*dPtr = ((*sPtr - *dPtr) * cookedAlpha + *dPtr);
					dPtr++; sPtr++;
					*dPtr = ((*sPtr - *dPtr) * cookedAlpha + *dPtr);
					dPtr++;	if (REAL_BYTEDEPTH==4) dPtr++; sPtr+=2;
				*/

					//Lookup value by alpha
					*dPtr = alphaLook[int(*sPtr)][int(*dPtr)][int(*sPtrAlpha)];
					dPtr++;	sPtr++;
					*dPtr = alphaLook[int(*sPtr)][int(*dPtr)][int(*sPtrAlpha)];
					dPtr++;	sPtr++;
					*dPtr = alphaLook[int(*sPtr)][int(*dPtr)][int(*sPtrAlpha)];
					dPtr++;	if (REAL_BYTEDEPTH==4) dPtr++; sPtr+=2;

				}
				else
				{
					dPtr+=REAL_BYTEDEPTH;
					sPtr+=4;
				}

				sPtrAlpha+=4;
			}
		} // end for index_x
		dPtr+=offset;
	} // end for index_y

	if (FAILED(lpddsThis1->Unlock(NULL)))
		return(0);
	return(1);
} // end _Targa2Surface_Alpha

///////////////////////////////////////////////////////////////

int _Targa2Surface24_AlphaQuickEdge_FX(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat)
{
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	lpddsThis1->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	//register DWORD *this_buffer = (DWORD *)ddsd.lpSurface;
	register UCHAR *dPtr = (UCHAR *)ddsd.lpSurface;//(UCHAR*)&this_buffer[toX + (toY*ddsd.lPitch >> 2)];
	register UCHAR *sPtr = image->buffer;
	register UCHAR *sPtrAlpha = image->buffer;
	sPtrAlpha+=3;
	//register UCHAR blue,green,red,alpha;
	//register DWORD pixel;
	register int tox1 = toX; //save original
	register int toy1 = toY;
	register int explod1 = explodedness; //save original
	register int draw=0;

/*
	if (explodedness)
	{//this keeps the exploded object red but solid until after frame 3
		if (explodedness<=3)
			explodedness=0;
		else
			explodedness-=3;
	}
*/
	if (explodedness) explodedness--; //start explosion just as a red dude, not blown up yet at 1

	dPtr+=toX*3;
	dPtr+=toY*ddsd.lPitch;
	register UCHAR *dPtrLineStart=dPtr;

	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		dPtrLineStart=dPtr;
		if (index_y%2) //splits object into squares
			toY+=explodedness;
		toX=tox1; //reset offset before exploding next line of image

		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			if (index_x%2) //splits object into squares
				toX+=explodedness;
			draw=0;
			/*
			blue=*sPtr++;
			green=*sPtr++;
			red=*sPtr++;
			alpha=*sPtr++;
			*/
			/*
			if (explod1 || 255-opacityCap) //this use of opacity cap is unique to DXMan, makes it red when transparent
			{
				green>>=1;
				blue>>=2;
			}//find some way to implement this
			*/

			if (*sPtrAlpha>opacityCap) *sPtrAlpha=opacityCap; //"Cap" opacity off at opacityCap to create transparency if not 255

			if (*sPtrAlpha<85)
			{//skip
				sPtr+=4;
				sPtrAlpha+=4;
				dPtr+=3;
			}
			else if (*sPtrAlpha>171)
			{//Solid
				//Manual clip:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								draw=1;
				if (draw)
				{
					//dPtr+=index_x*3 + toX*3;// + index_x*3;// + toX*3;
					//dPtr=dPtrLineStart;
					dPtr=(UCHAR*)ddsd.lpSurface;
					dPtr+=(index_y+toY)*ddsd.lPitch;
					dPtr+=(index_x+toX)*3;//+(index_y+toY-toy1)*ddsd.lPitch;
					//dPtr+=explOffset;//+ddsd.lPitch*int(explodedness/2);
					*dPtr = (explod1 || 255-opacityCap) ? *sPtr>>2 : *sPtr; //b
					dPtr++;
					sPtr++;
					*dPtr = (explod1 || 255-opacityCap) ? *sPtr>>1 : *sPtr; //g
					dPtr++;
					sPtr++;
					*dPtr = (explod1)? *sPtr-explodedness*30 : *sPtr; //r
					dPtr++;
					sPtr+=2;
				}
				else
				{
					dPtr+=3;
					sPtr+=4;
				}

				sPtrAlpha+=4;
			}
			else
			{//Half transparent
				//Manual clip:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								draw=1;

				if (draw)
				{
					if (dwStat & STATUS_SHIELD)
					{//blue outline
							*dPtr = 192;
							dPtr++;
							sPtr++;
							*dPtr = 64;
							dPtr++;
							sPtr++;
							*dPtr = *sPtr;
							dPtr++;
							sPtr+=2;
					}
					else
					{//actual blend
						//dPtr=(UCHAR*)ddsd.lpSurface + toX*3 + toY*ddsd.lPitch;
						*dPtr = (*dPtr + *sPtr)/2;
						dPtr++;
						sPtr++;
						*dPtr = (*dPtr + *sPtr)/2;
						dPtr++;
						sPtr++;
						*dPtr = (*dPtr + *sPtr)/2;
						dPtr++;
						sPtr+=2; //increment past alpha
					}
				}
				else
				{
					dPtr+=3;
					sPtr+=4;
				}


				sPtrAlpha+=4;
			}
		}
		dPtrLineStart+=ddsd.lPitch;
		dPtr+=ddsd.lPitch - image->width*3;
	}
	if (FAILED(lpddsThis1->Unlock(NULL)))
		return(0);
	return(1);
} // end _Targa2Surface24_AlphaQuickEdge_FX

////////////////////////////////////////////////////////////////////////////////

int _Targa2Surface24_AlphaQuickEdge_FX_Scaled(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat, float scale)
{
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	lpddsThis1->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	//register DWORD *this_buffer = (DWORD *)ddsd.lpSurface;
	register UCHAR *dPtr = (UCHAR *)ddsd.lpSurface;//(UCHAR*)&this_buffer[toX + (toY*ddsd.lPitch >> 2)];
	register UCHAR *sPtr = image->buffer;
	register UCHAR *sPtrAlpha = image->buffer;
	sPtrAlpha+=3;
	//register UCHAR blue,green,red,alpha;
	//register DWORD pixel;
	register int tox1 = toX; //save original
	register int explod1 = explodedness; //save original
	register int draw=0;

	if (explodedness) explodedness--; //start explosion just as a red dude, not blown up yet at frame 1

	dPtr+=toX*3;
	dPtr+=toY*ddsd.lPitch;

	register UCHAR *
	dPtrLineStart=dPtr;

	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		dPtr=dPtrLineStart;
		if (index_y%2) //splits object into squares
			toY+=explodedness;
		toX=tox1; //reset offset before exploding next line of image

		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			draw=0;
			if (index_x%2) //splits object into squares
				toX+=explodedness;

			if (*sPtrAlpha>opacityCap) *sPtrAlpha=opacityCap; //"Cap" opacity off at opacityCap to create transparency if not 255

			if (*sPtrAlpha<85)
			{//TRANSPARENT
				dPtr+=3;
				sPtr+=4;
				sPtrAlpha+=4;
			}
			else if (*sPtrAlpha>171)
			{//SOLID
				register int scaledX = (scale==1) ? index_x : index_x*scale,
							 scaledY = (scale==1) ? index_y : index_y*scale;
				//Manual clip	and set pixel:
				if (scaledX+toX<SCREEN_WIDTH)
					if (scaledX+toX>=0)
						if (scaledY+toY>=0)
							if (scaledY+toY<SCREEN_HEIGHT)
							{
								draw=1;
								//pixel = _RGB32BIT(256,red,green,blue);
								//this_buffer[toX + scaledX + ((scaledY+toY)*ddsd.lPitch >> 2)] = pixel;
							}
				if (draw)
				{
					dPtr=(UCHAR*)ddsd.lpSurface+(toX+scaledX)*3+(toY+scaledY)*ddsd.lPitch;
					*dPtr=(explod1 || 255-opacityCap) ? *sPtr>>2 : *sPtr; //b
					dPtr++;
					sPtr++;
					*dPtr=(explod1 || 255-opacityCap) ? *sPtr>>1 : *sPtr; //g
					dPtr++;
					sPtr++;
					*dPtr=(explod1)? *sPtr-explodedness*30 : *sPtr; //r;
					dPtr++;
					sPtr+=2; //skip alpha
				}
				else
				{
					dPtr+=3;
					sPtr+=4;
				}
				sPtrAlpha+=4;
			}
			else
			{//AVERAGE (or "shield" effect)
				register int scaledX = (scale==1) ? index_x : index_x*scale,
							 scaledY = (scale==1) ? index_y : index_y*scale;
				//Manual clip	and set pixel:
				if (scaledX+toX<SCREEN_WIDTH)
					if (scaledX+toX>=0)
						if (scaledY+toY>=0)
							if (scaledY+toY<SCREEN_HEIGHT)
							{
								draw=1;
								//pixel = _RGB32BIT(256,red,green,blue);
								//this_buffer[toX + scaledX + ((scaledY+toY)*ddsd.lPitch >> 2)] = pixel;
							}
				if (draw)
				{
					if (dwStat & STATUS_SHIELD)
					{//do shield
						dPtr=(UCHAR*)ddsd.lpSurface+(toX+scaledX)*3+(toY+scaledY)*ddsd.lPitch;
						*dPtr=192;
						dPtr++;
						sPtr++;
						*dPtr=64;
						dPtr++;
						sPtr++;
						*dPtr=*sPtr;
						dPtr++;
						sPtr+=2;
					}
					else
					{//do shield
						dPtr=(UCHAR*)ddsd.lpSurface+(toX+scaledX)*3+(toY+scaledY)*ddsd.lPitch;
						*dPtr=(*dPtr + *sPtr)/2;
						dPtr++;
						sPtr++;
						*dPtr=(*dPtr + *sPtr)/2;
						dPtr++;
						sPtr++;
						*dPtr=(*dPtr + *sPtr)/2;
						dPtr++;
						sPtr+=2;
					}
				}
				else
				{
					dPtr+=3;
					sPtr+=4;
				}
				sPtrAlpha+=4;
			}
		}
		dPtr+=ddsd.lPitch - image->width*3;
	}
	if (FAILED(lpddsThis1->Unlock(NULL)))
		return(0);
	return(1);
} // end _Targa2Surface24_AlphaQuickEdge_FX_Scaled

///////////////////////////////////////////////////////////////////////////////


int Targa2Surface24(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY)
{
	// copy the Targa image to the primary buffer line by line
	DDSURFACEDESC2 ddsd;	//	direct draw surface description
	// set size of the structure
	ddsd.dwSize = sizeof(ddsd);

	// lock the primary surface
	lpddsThis1->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);

	// get video pointer to primary surfce
	//DWORD *this_buffer = (DWORD *)ddsd.lpSurface;
	UCHAR *dPtr = (UCHAR *)ddsd.lpSurface;
	UCHAR *sPtr = (UCHAR *)image->buffer;

	// process each line and copy it into the primary buffer
	for (int index_y = 0; index_y < image->height; index_y++)
	{
		dPtr+=toX*3;
		dPtr+=toY*ddsd.lPitch;
		for (int index_x = 0; index_x < image->width; index_x++)
		{
			// get BGR values
			//UCHAR blue	= (image->buffer[index_y*image->width*4 + index_x*4 + 0]),
				//green = (image->buffer[index_y*image->width*4 + index_x*4 + 1]),
				//red	 = (image->buffer[index_y*image->width*4 + index_x*4 + 2]),
				//alpha = (image->buffer[index_y*image->width*4 + index_x*4 + 3]);
			*dPtr = *sPtr;
			dPtr++;
			sPtr++;
			*dPtr = *sPtr;
			dPtr++;
			sPtr++;
			*dPtr = *sPtr;
			dPtr++;
			sPtr++;
			sPtr++; //skip alpha

			// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
			//DWORD pixel = _RGB32BIT(alpha,red,green,blue);

			// write the pixel
			//this_buffer[toX + index_x + ((index_y+toY)*ddsd.lPitch >> 2)] = pixel;

		} // end for index_x
		dPtr+=ddsd.lPitch - image->width*3 - toX*3;// /REAL_BPP - SCREEN_WIDTH*REAL_BPP/8;// - SCREEN_WIDTH * REAL_BPP/8;
	} // end for index_y

	// now unlock the surface
	if (FAILED(lpddsThis1->Unlock(NULL)))
		return(0);
	return(1);
}

////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//UNUSED FUNCTION:
int _Targa2Surface32_AlphaQuickEdge(LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY)//, int opacityCap)
{
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	lpddsThis1->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	register DWORD *this_buffer = (DWORD *)ddsd.lpSurface; //dest pixel pointer
	register UCHAR *dPtr = (UCHAR*)&this_buffer[toX + (toY*ddsd.lPitch >> 2)]; //dest byte pointer
	register UCHAR *sPtr = image->buffer;														 //source byte pointer
	register UCHAR alpha,blue,green,red;
	register DWORD pixel;
	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			blue=*sPtr++;
			green=*sPtr++;
			red=*sPtr++;
			alpha=*sPtr++;
			if (alpha<85) dPtr+=4;//transparent
			else if (alpha>171)
			{//solid
				dPtr+=4;
				pixel = _RGB32BIT(256,red,green,blue);
				//Manual clip, and set pixel in this_buffer:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								this_buffer[toX + index_x + ((index_y+toY)*ddsd.lPitch >> 2)] = pixel;
			}
			else
			{//half transparent, average
				blue = (*dPtr + blue)/2;
				dPtr++;
				green = (*dPtr + green)/2;
				dPtr++;
				red = (*dPtr + red)/2;
				dPtr+=2; //increment past alpha
				pixel = _RGB32BIT(256,red,green,blue);
				//Manual clip and set pixel:
				if (index_x+toX<SCREEN_WIDTH)
					if (index_x+toX>=0)
						if (index_y+toY>=0)
							if (index_y+toY<SCREEN_HEIGHT)
								this_buffer[toX + index_x + ((index_y+toY)*ddsd.lPitch >> 2)] = pixel;
			}
		}
		dPtr+=ddsd.lPitch - image->width*4;
	}
	if (FAILED(lpddsThis1->Unlock(NULL)))
		return(0);
	return(1);
} // end _Targa2Surface_AlphaQuickEdge

///////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hwnd,
								UINT msg,
														WPARAM wparam,
														LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT		ps;		// used in WM_PAINT
	HDC				hdc;	// handle to a device context
	char buffer[80];				// used to print strings

	// what is the message
	switch(msg)
	{
	case WM_CREATE:
				{
		// do initialization stuff here
				// return success
		return(0);
		} break;

	case WM_PAINT:
		{
		// simply validate the window
				 hdc = BeginPaint(hwnd,&ps);

				// end painting
				EndPaint(hwnd,&ps);

				// return success
		return(0);
			 } break;

	case WM_DESTROY:
		{

		// kill the application, this sends a WM_QUIT message
		PostQuitMessage(0);

				// return success
		return(0);
		} break;

	default:break;

		} // end switch

	// process any messages that we didn't take care of
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

///////////////////////////////////////////////////////////

int GameMain(void *parms = NULL, int num_parms = 0)
{
	// this is the main loop of the game, do all your processing
	// here


	// make sure this isn't executed again
	if (window_closed)
		return(0);

	// for now test if user is hitting ESC
	if (KEYDOWN(VK_ESCAPE))
	{//int answer = 0;

			if (REAL_BPP==24)
				_Targa2Surface24_Alpha(lptargaGameScreen[2], lpddsPrimary,
					SCREEN_WIDTH/2-lptargaGameScreen[2]->width/2,
					SCREEN_HEIGHT/2-lptargaGameScreen[2]->height/2);
			else
				_Targa2Surface32_Alpha(lptargaGameScreen[2], lpddsPrimary,
					SCREEN_WIDTH/2-lptargaGameScreen[2]->width/2,
					SCREEN_HEIGHT/2-lptargaGameScreen[2]->height/2);

			DWORD startTime=GetTickCount();
			while((GetTickCount() - startTime) < 300);

		while (1)
		{
			if (KEYDOWN(0x59)) //HEX: 59 //LETTER:Y
			{
				gameState=GAME_STATE_EXIT;
				break;
			}
			else if (KEYDOWN(0x4E)) //HEX: 4E //LETTER:N
			{
				gameState=GAME_STATE_RUN;
				break;
			}
		}

		//gameState=GAME_STATE_YOU_LOSE;
	}


	if (gameState == GAME_STATE_INIT)
	{
		DDrawFillSurface(lpddsPrimary,0);
		if (REAL_BPP == 24)
			Targa2Surface24(lptargaIntro, lpddsPrimary, SCREEN_WIDTH/2-lptargaIntro->width/2,0);
		else Targa2Surface32(lptargaIntro, lpddsPrimary, SCREEN_WIDTH/2-lptargaIntro->width/2,0);
		PlaySound((const char*)SOUND_ID_INTRO, hinstance_app, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		DWORD startTime=GetTickCount();
			while((GetTickCount() - startTime) < 1000);
		while (KEYUP(VK_RETURN))
		{
		}

		if (hero != NULL)
		{
			delete hero;
			hero=NULL;
		}
		for (int aliensNow = 0; aliensNow < MAXALIENS; aliensNow++)
		{
			if (aliens[aliensNow] != NULL)
			{
				delete aliens[aliensNow];
				aliens[aliensNow]=NULL;
			}
		}
		hero = new Hero;
		gameState = GAME_STATE_START_LEVEL;
		gameLevel = 1;
		gameStage = 1;
	}

	else if (gameState == GAME_STATE_START_LEVEL)
	{//START LEVEL

		if (gameLevel==1)
		{
			errlog.AddPrecedent("About to call TargaUnload(lptargaBack)", "START_LEVEL 1");
			if (lptargaBack) TargaUnload(lptargaBack);
			errlog.Add(iLastErr, "TargaUnload lptargaBack", "START_LEVEL");
			errlog.AddPrecedent("About to call TargaLoad(\"level1Back.tga\")", "START_LEVEL 1");
			lptargaBack=TargaLoad("level1Back.tga");
			errlog.Add(iLastErr, "TargaLoad level1Back.tga", "START_LEVEL");
			if (iLastErr)
			{
				iErrorsSaved++;
				return (0);
			}

			if (REAL_BPP==24)
			{
				if (!Targa2Surface24(lptargaBack, lpddsBackdrop, 0, 0))
				{
					iErrorsSaved++;
					//error_txt << "\nUnable to render Targa to 24-bit Backdrop in START_LEVEL";
					return(0);
				}
			}
			else
			{//32-bit backdrop surface
				if (!Targa2Surface32(lptargaBack, lpddsBackdrop, 0, 0))
				{
					iErrorsSaved++;
					error_txt << "\nUnable to render Targa to 32-bit Backdrop in START_LEVEL 1";
					return(0);
				}
			}

			errlog.AddPrecedent("About to call TargaUnload(lptargaBack)","START_LEVEL 1");
			TargaUnload(lptargaBack);
			errlog.Add(iLastErr, "unload lptargaBack","START_LEVEL");
		}
		else if (gameLevel==2)
		{
			errlog.AddPrecedent("Calling TargaUnload(lptargaBack)", "START_LEVEL 2);
			if (lptargaBack) TargaUnload(lptargaBack);
			errlog.Add(iLastErr, "unload lptargaBack","START_LEVEL 2");
			errlog.AddPrecedent("TargaLoad(\"level2Back.tga\")", "START_LEVEL 2");
			lptargaBack=TargaLoad("level2Back.tga");
			errlog.Add(iLastErr, "level2Back.tga","START_LEVEL 2");
			if (iLastErr)
			{
				iErrorsSaved++;
				return (0);
			}

			if (REAL_BPP==24)
			{
				if (!Targa2Surface24(lptargaBack, lpddsBackdrop, 0, 0))
				{
					iErrorsSaved++;
					error_txt << "\nUnable to render Targa to 24-bit Backdrop in START_LEVEL 2";
					return(0);
				}
			}
			else
			{//32-bit backdrop surface
				if (!Targa2Surface32(lptargaBack, lpddsBackdrop, 0, 0))
				{
					error_txt << "\nUnable to render Targa to 32-bit Backdrop in START_LEVEL 2";
					iErrorsSaved++;
					return(0);
				}
			}

			errlog.AddPrecedent("Calling TargaUnload(lptargaBack)","START_LEVEL 2");
			TargaUnload(lptargaBack);
			errlog.Add(iLastErr, "unload lptargaBack", "START_LEVEL 2");
		}
		else if (gameLevel==3)
		{
			errlog.AddPrecedent("Calling TargaUnload(lptargaBack)");
			if (lptargaBack) TargaUnload(lptargaBack);
			errlog.Add(iLastErr, "unload lptargaBack", "START_LEVEL 3");
			lptargaBack=TargaLoad("level3Back.tga");
			errlog.Add(iLastErr, "level3Back.tga", "START_LEVEL 3");
			if (iLastErr)
			{
				iErrorsSaved++;
				return (0);
			}

			if (REAL_BPP == 24)
			{
				if (!Targa2Surface32(lptargaBack, lpddsBackdrop, 0, 0))//ix)
				{
					error_txt << "\nUnable to render targaBack to 24-bit surface";
					iErrorsSaved++;
					return(0);
				}
			}
			else
			{
				if (!Targa2Surface32(lptargaBack, lpddsBackdrop, 0, 0))
				{
					error_txt << "\nUnable to render targaBack to 32-bit surface";
					iErrorsSaved++;
					return(0);
				}
			}

			errlog.AddPrecedent("Calling TargaUnload(lptargaBack)", "START_LEVEL 3");
			TargaUnload(lptargaBack);
			errlog.Add(iLastErr, "unload lptargaBack", "START_LEVEL 3");
		}


		hero->iHP=255;
		if (gameLevel==2) hero->dwStatus |= STATUS_ANTIGRAVITY;
		gameState = GAME_STATE_START_STAGE;
	}

	else if (gameState == GAME_STATE_START_STAGE)
	{//START STAGE
		bombed=0;
		PlaySound((const char*)SOUND_ID_INVASION, hinstance_app, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		for (index=0; index<MAXALIENS; index++)
		{
			aliens[index] = new Alien(SCREEN_WIDTH, index*(SCREEN_HEIGHT/8), index+1);
			//aliens[index]->x = SCREEN_WIDTH;
			//aliens[index]->y += index*(SCREEN_HEIGHT/64);
			//aliens[index]->z = index+1;
			//if (gameLevel==2) aliens[index]->dwStatus |= STATUS_SHOOTER;
			//else if (gameLevel==3) aliens[index]->dwStatus |= STATUS_BOMBER;
			//if (gameLevel==3 && gameStage==3) aliens[index]->dwStatus |= STATUS_SHIELD;
		}

		if (gameLevel==3) hero->dwStatus |= STATUS_SHIELD;
		if (gameStage==3 && gameLevel==3) aliens[2]->setAsBoss();//Set a boss if last stage of last level
		gameState = GAME_STATE_RUN;
	}

	else if (gameState == GAME_STATE_RUN)
	{
		//if KEYDOWN(65/* a */) hero->dwStatus |= STATUS_ANTIGRAVITY;
		//if KEYUP(65) hero->dwStatus ^= STATUS_ANTIGRAVITY;

		if KEYDOWN(68/* d */) hero->turn(1);//if KEYDOWN(VK_RIGHT) hero->turn(1);
		if KEYDOWN(65/* a */) { hero->turn(-1);	doublecode=0;}//if KEYDOWN(VK_LEFT) hero->turn(-1);
		if KEYDOWN(VK_HOME) { hero->jump();		doublecode=0;}
		if KEYDOWN(87/* w */) { hero->move(-1);	doublecode=0;}//if KEYDOWN(VK_UP) hero->move(-1);
		if KEYDOWN(83/* s */) { hero->move(1);	 doublecode=0;}//if KEYDOWN(VK_DOWN) hero->move(1);
		if KEYUP(VK_INSERT) hero->shootDelay=0;
		if KEYDOWN(VK_INSERT) hero->shoot();
		if KEYDOWN(50/* 2 */)
		{//pressing '2' 10 times activates STATUS_DOUBLESPEED
			if (!keyDelay)
			{keyDelay=1;
				doublecode++;}
		}
		if KEYUP(50/* 2 */) keyDelay=0;
		if KEYDOWN(80/* p */)
		{//pressing 'p' 20 times activates STATUS_PRECISIONAIM
			if (!keyDelay)
			{keyDelay=1;
				precisecode++;}
		}
		if KEYUP(80/* p */) keyDelay=0;

		/*
		if KEYDOWN(65)
		{// "a" key: debug level skip "smart bomb"
			if (!bombed)
			{
				bombed=1;
				for (index=0; index<MAXALIENS; index++)
				{
					if (aliens[index] != NULL)
					{
						delete aliens[index];
						aliens[index] = NULL;
					}
				}
			}
		}
		*/


		if (doublecode>=10 && hero->dwStatus ^ STATUS_DOUBLESPEED) hero->doublespeed();
		if (precisecode>=20 && hero->dwStatus ^ STATUS_PRECISIONAIM) hero->precisionaim();

		DDrawDrawSurface(lpddsBackdrop,	0,	 0,	 SCREEN_WIDTH+1, SCREEN_HEIGHT+1 ,lpddsBack);

		//Update all the existing aliens, delete the rest
		for (index=0; index < MAXALIENS; index++)
		{//Prototype is (LPTARGA image, LPDIRECTDRAWSURFACE4 lpddsThis1, int toX, int toY_)
			if (hero!=NULL && (index == hero->z-1))//(calculate layer to put hero in)
				hero->refresh();
			if (aliens[index] != NULL)
			{
				aliens[index]->refresh();
				//hero->drawTarget(index);
				if (aliens[index]->explodedBy>6)
				{
					if (aliens[index]->dwStatus & STATUS_BOSS)
					{
						for (int alienNow=0; alienNow < MAXALIENS; alienNow++)
						{
							if (aliens[alienNow]->dwStatus & STATUS_SHIELD)
								aliens[alienNow]->dwStatus ^= STATUS_SHIELD;
						}
					}

					delete aliens[index];
					aliens[index] = NULL;
				}
			}
		}

		if (hero->explodedBy>10)
		{
			delete hero;
			hero=NULL;
			gameState=GAME_STATE_YOU_LOSE;
		}


		for (index=0; index<MAXSHOTS; index++)
		{
			if (shots[index] != NULL)
			{
				shots[index]->refresh();
				if (shots[index]->dead)
				{
					delete shots[index];
					shots[index] = NULL;
				}
			}
		}


		//DDrawDrawSurface(lpddsExplosion, 100,100,300,300,lpddsBack);

		/*
		if(calculateExplosion(lpddsBack, 100, 100, 20, 0, EXPLOSION_CHECK_COUNT) == 0)
		{
			//DDrawFillSurface(lpddsBack,0);
			explosionResult = calculateExplosion(lpddsBack, 100, 100, 20, 30, EXPLOSION_START);
		}
		else
			explosionResult = calculateExplosion(lpddsBack, 100, 100, 20, 30, EXPLOSION_CONTINUE);
		//error_txt << "\nResult of explosionCalculate = " << explosionResult;
		*/

		// flip pages
		while (FAILED(lpddsPrimary->Flip(NULL, DDFLIP_WAIT)));

		if (numOfAliens<=0) gameState = GAME_STATE_WIN_STAGE;
	}
	else if (gameState == GAME_STATE_SHUTDOWN)
	{
	}
	else if (gameState == GAME_STATE_WIN_STAGE)
	{
		PlaySound((const char*)SOUND_ID_TRUMPET, hinstance_app, SND_RESOURCE | SND_ASYNC);
		for (index=0; index<MAXSHOTS; index++)
		{
			if (shots[index]!=NULL)
			{
				delete shots[index];
				shots[index]=NULL;
			}
		}
		int screenNow=1;

		if (gameLevel<3)
		{
			if (gameStage<3)
			{//WIN STAGE


				gameStage++;
				screenNow=gameStage+1;//DISPLAY: Stage gameStage..
				gameState=GAME_STATE_START_STAGE;
			}
			else
			{//WIN LEVEL

				gameLevel++;
				screenNow=gameLevel+3;//DISPLAY: Level gameLevel.
				gameStage=1;
				gameState=GAME_STATE_START_LEVEL;
			}
		}
		else
		{
			if (gameStage<3)
			{//WIN STAGE
				gameStage++;
				screenNow=gameStage+1;//DISPLAY: You won stage gameStage of Level 3!
				gameState=GAME_STATE_START_STAGE;
			}
			else
			{//WIN GAME
				screenNow=1;
				gameState=GAME_STATE_WIN_GAME;
			}
		}
		if (screenNow==1)
		{
			gameState = GAME_STATE_WIN_GAME;
		}
		else
		{
			if (REAL_BPP==24)
			{
				_Targa2Surface24_Alpha(lptargaGameScreen[screenNow], lpddsPrimary,
					SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2,
					SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2);
			}
			else
			{
				_Targa2Surface32_Alpha(lptargaGameScreen[screenNow], lpddsPrimary,
					SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2,
					SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2);
			}

			DWORD startTime=GetTickCount();
			while((GetTickCount() - startTime) < 3000);
		}
	}
	else if (gameState == GAME_STATE_WIN_GAME)
	{
		int screenNow=1;
		if (doublecode>=10)
		{
			screenNow=7;
			if (precisecode>=20) screenNow++;
		}

		if (REAL_BPP==24)
		{
			_Targa2Surface24_Alpha(lptargaGameScreen[screenNow], lpddsPrimary,
				SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2,
				SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2);
		}
		else
		{
			_Targa2Surface32_Alpha(lptargaGameScreen[screenNow], lpddsPrimary,
				SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2,
				SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2);
		}

		PlaySound((const char*)SOUND_ID_ENDING, hinstance_app, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		DWORD startTime=GetTickCount();
		while((GetTickCount() - startTime) < 300);
		while(KEYUP(VK_ESCAPE));
		gameState = GAME_STATE_EXIT;
	}
	else if (gameState == GAME_STATE_YOU_LOSE)
	{
		PlaySound((const char*)SOUND_ID_THRUST, hinstance_app, SND_RESOURCE | SND_ASYNC);
		if (REAL_BPP==24)
		{
			_Targa2Surface24_Alpha(lptargaGameScreen[0], lpddsPrimary,
				SCREEN_WIDTH/2-lptargaGameScreen[0]->width/2,
				SCREEN_HEIGHT/2-lptargaGameScreen[0]->height/2);
		}
		else
		{
			_Targa2Surface32_Alpha(lptargaGameScreen[0], lpddsPrimary,
				SCREEN_WIDTH/2-lptargaGameScreen[0]->width/2,
				SCREEN_HEIGHT/2-lptargaGameScreen[0]->height/2);
		}
		DWORD startTime=GetTickCount();
		while((GetTickCount() - startTime) < 5000);
		//PostMessage(hwndMain,WM_CLOSE,0,0);
		//PostMessage(hwndMain,WM_QUIT,0,0);
		//window_closed = 1;
		gameState=GAME_STATE_INIT;
	}
	else if (gameState == GAME_STATE_EXIT)
	{
		PostMessage(hwndMain,WM_QUIT,0,0);
		window_closed = 1;
	}

	// return success or failure or your own return code here
	return(1);

} // end GameMain

////////////////////////////////////////////////////////////

int GameInit(void *parms = NULL, int num_parms = 0)
{
	errlog.SetMessageDest(hwndMain);
	errlog.AddTitle("Starting GameInit");

	/*
	*/


	//Load alpha lookup table from file
	int file_handle;
	OFSTRUCT file_data; //the file data information

	if ((file_handle = OpenFile("alphalook.raw",&file_data,OF_READ))==-1) //open the file if it exists
	{//If can't open file:
		error_txt << "\nERROR: Unable to open alpha lookup file!";
		iErrorsSaved++;
		return(0);
	}
	_lread(file_handle, alphaLook,256*256*256);
	_lclose(file_handle);

	if (iErrorsSaved)
	{
		//Generate alpha lookup table if needed
		error_txt << "\nNeed to generate alpha lookup table..";
		for (UCHAR source=0; source<256; source++)
		{
		for (UCHAR dest; dest<256; dest++)
		{
			for (UCHAR alpha; alpha<256; alpha++)
			{
						alphaLook[source][dest][alpha]=((source-dest)*alpha/255+dest);
			}
		}
		}
		error_txt << "Done";
	}

	for (int shotsNow = 0; shotsNow<MAXSHOTS; shotsNow++)
		shots[shotsNow]=NULL;
	for (int aliensNow = 0; aliensNow<MAXALIENS; aliensNow++)
		aliens[aliensNow]=NULL;


	// this is called once after the initial window is created and
	// before the main event loop is entered, do all your initialization
	// here

	// first create base IDirectDraw interface
	if (FAILED(DirectDrawCreate(NULL, &lpdd, NULL)))
		return(0);

	// now query for IDirectDraw4
	if (FAILED(lpdd->QueryInterface(IID_IDirectDraw4,
															 (LPVOID *)&lpdd4)))
	return(0);

	// set cooperation to full screen
	if (FAILED(lpdd4->SetCooperativeLevel(hwndMain,
										DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX |
										DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
	return(0);

	// set display mode
	if (FAILED(lpdd4->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,0,0)))
		if (FAILED(lpdd4->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, (SCREEN_BPP==32) ? 24 : 32,0,0)))
		{
			error_txt << "\nAborting: Unable to set ColorDepth to 24 or 32";
			iErrorsSaved++;
			return (0);
		}


	// clear ddsd and set size
	DDRAW_INIT_STRUCT(ddsd);
	// enable valid fields
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	// set the backbuffer count field to 1, use 2 for triple buffering
	ddsd.dwBackBufferCount = 1;
	// request a complex, flippable
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
	// create the primary surface
	if (FAILED(lpdd4->CreateSurface(&ddsd, &lpddsPrimary, NULL)))
		return(0);
	// this line is needed by the call
	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	// set clipper up on back buffer since that's where well clip
	// get the attached back buffer surface
	if (FAILED(lpddsPrimary->GetAttachedSurface(&ddsd.ddsCaps, &lpddsBack)))
		return(0);
	//RECT explosionRect={0,0,200,200};
	//lpddClipper = DDrawAttachClipper(lpddsExplosion,1,&explosionRect);
	RECT screenRect= {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	lpddClipper = DDrawAttachClipper(lpddsBack,1,&screenRect);
	// clean the surfaces
	DDrawFillSurface(lpddsPrimary,0);
	DDrawFillSurface(lpddsBack,0);

	DDRAW_INIT_STRUCT(ddsd);

	//Good time to get the pixel format:
	memset(&ddpixel, 0, sizeof(ddpixel));
	ddpixel.dwSize = sizeof(ddpixel);
	lpddsPrimary->GetPixelFormat(&ddpixel);
	REAL_BPP=ddpixel.dwRGBBitCount;
	REAL_BYTEDEPTH=REAL_BPP/8;


	if (lptargaBack) TargaUnload(lptargaBack);
	errlog.Add(iLastErr, "unload lptargaBack", "GameInit");
	lptargaBack=TargaLoad("level1Back.tga");
	errlog.Add(iLastErr, "level1Back.tga", "GameInit");
	if (iLastErr)
	{
		iErrorsSaved++;
		return (0);
	}
	lpddsBackdrop = DDrawCreateSurface(lptargaBack->width,lptargaBack->height,0);
	if (REAL_BPP == 32)
	{
		if (!Targa2Surface32(lptargaBack, lpddsBackdrop, 0, 0))
		{
			error_txt <<"\nCould not load targaBack to 32-bit BackDrop in GameInit";
			iErrorsSaved++;
			return(0);
		}
	}
	else
	{
		if (!Targa2Surface24(lptargaBack, lpddsBackdrop, 0,0))
		{
			error_txt <<"\nCould not load targaBack to 24-bit BackDrop in GameInit";
			iErrorsSaved++;
			return(0);
		}

	}
	TargaUnload(lptargaBack); //Dump this targa--already stored as lpddsBackdrop
	errlog.Add(iLastErr, "unload lptargaBack", "GameInit");


	//lpddsExplosion = DDrawCreateSurface(300,300,0); //Not used yet
	//DDrawFillSurface(lpddsExplosion,0);
	int iFramesHero=21,
		iFramesAlien=2,
		iFramesAlienBoss=3,
		iFramesShot=2,
		iFramesGameScreen=9;
	int iFramesFound;

	iFramesFound=iFramesHero;
	errlog.AddPrecedent("Loading heroani","GameInit");
	lptargaHero=TargaLoadSeq("heroani", &iFramesFound);
	errlog.Add(iLastErr, "heroani*.tga", "GameInit");
	if (iFramesFound!=iFramesHero) error_txt << "not all Hero frames loaded."<<endl;
	iFramesFound=iFramesAlien;
	lptargaAlien=TargaLoadSeq("alienf", &iFramesFound);
	errlog.Add(iLastErr, "alienf*.tga", "GameInit");
	if (iFramesFound!=iFramesAlien) error_txt << "not all Alien frames loaded."<<endl;
	iFramesFound=iFramesAlienBoss;
	lptargaAlienBoss=TargaLoadSeq("alienBoss", &iFramesFound);
	errlog.Add(iLastErr,"alienBoss*.tga", "GameInit");
	if (iFramesFound!=iFramesAlienBoss) error_txt << "not all AlienBoss frames loaded."<<endl;
	iFramesFound=iFramesShot;
	lptargaShot=TargaLoadSeq("shotAni", &iFramesFound);
	errlog.Add(iLastErr,"shotAni*.tga", "GameInit");
	if (iFramesFound!=iFramesShot) error_txt << "not all Shot frames loaded."<<endl;
	iFramesFound=iFramesGameScreen;
	lptargaGameScreen=TargaLoadSeq("gamescreen",&iFramesFound);
	errlog.Add(iLastErr,"gamescreen*.tga", "GameInit");
	if (iFramesFound!=iFramesGameScreen) error_txt << "not all GameScreen frames loaded."<<endl;

	lptargaIntro=TargaLoad("480intro.tga");
	errlog.Add(iLastErr,"480intro.tga", "GameInit");
	if (iLastErr)
	{
		iErrorsSaved++;
	}


	//Return success or failure or return code
	return(1);

} // end GameInit

/////////////////////////////////////////////////////////////

int GameShutdown(void *parms = NULL, int num_parms = 0)
{
	//This is called after the game is exited and the main event
	// loop while is exited, do all you cleanup and shutdown here

		//Release palette if 8-bit was used
	//if (lpddpal){lpddpal->Release(); lpddpal = NULL;} //

	//Unload Targas
	TargaUnloadSeq(lptargaAlien, 2);
	errlog.Add(iLastErr, "lptargaAlien", "GameShutDown");
	TargaUnloadSeq(lptargaAlienBoss, 3);
	errlog.Add(iLastErr, "lptargaAlienBoss", "GameShutDown");
	TargaUnloadSeq(lptargaHero, 21);
	errlog.Add(iLastErr, "lptargaHero", "GameShutDown");
	TargaUnloadSeq(lptargaShot, 2);
	errlog.Add(iLastErr, "lptargaShot", "GameShutDown");
	TargaUnloadSeq(lptargaGameScreen, 9);
	errlog.Add(iLastErr, "lptargaGameScreen", "GameShutDown");
	TargaUnload(lptargaIntro);
	errlog.Add(iLastErr, "lptargaIntro", "GameShutDown");


	//Release primary surface
	if (lpddsPrimary)	{lpddsPrimary->Release();	lpddsPrimary = NULL;}

	//Release other surfaces//
	if (lpddsBackdrop)	{lpddsBackdrop->Release();	lpddsBackdrop = NULL;}
	//if (lpddsExplosion)	{lpddsExplosion->Release();	lpddsExplosion = NULL;}

	// now blow away the IDirectDraw4 interface
	if (lpdd4){	lpdd4->Release(); lpdd4 = NULL;} // end if

	// return success or failure or your own return code here
	return(1);

} // end GameShutdown

// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

	WNDCLASSEX winclass; // this will hold the class we create
	HWND		 hwnd;	 // generic window handle
	MSG			 msg;		 // generic message
	HDC				hdc;			// graphics device context

	// first fill in the window class stucture
	winclass.cbSize				 = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC |
													CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hCursor		= LoadCursor(hinstance, MAKEINTRESOURCE(IDC_CURSOR1));
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	winclass.hIconSm				= LoadIcon(NULL, IDI_APPLICATION);

	// save hinstance in global
	hinstance_app = hinstance;

	// register the window class
	if (!RegisterClassEx(&winclass))
		return(0);

	// create the window
	if (!(hwnd = CreateWindowEx(NULL,									// extended style
														WINDOW_CLASS_NAME,		 // class
								"DXMan SE ([TGA] Skinned Edition)", // title
								WS_POPUP | WS_VISIBLE,
								 0,0,		// initial x,y
								SCREEN_WIDTH,SCREEN_HEIGHT,	// initial width, height
								NULL,		// handle to parent
								NULL,		// handle to menu
								hinstance,// instance of this application
								NULL)))	// extra creation parms
		return(0);

	//save main window handle
	hwndMain = hwnd;
	errlog.SetMessageDest(hwnd);

	// initialize game here
	GameInit();


	while(TRUE)
	{//Main event Loop
		DWORD startTime=GetTickCount();

		// test if there is a message in queue, if so get it
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			// test if this is a quit
			if (msg.message == WM_QUIT)
				break;

			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		}
		//Main game processing goes here
		GameMain();

		//if ((GetTickCount() - startTime) >= 33){ iFramesDropped++; iErrorsSaved++;};
		while((GetTickCount() - startTime) < 33); //lock to approx 30fps
	} //Exit main event loop

	// closedown game here
	GameShutdown();

	//error_txt << "\n(" << iFramesDropped << ") frames dropped";

	if (!iErrorsSaved) DeleteFile("!errors.txt");

	// return to Windows like this
	return(msg.wParam);

} // end WinMain
