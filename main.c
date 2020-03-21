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
//#include <windowsx.h> 
//#include <mmsystem.h>
//Important C/C++ stuff
#include <iostream>
//#include <conio.h>
#include <cstdlib>
//#include <malloc.h>
#include <memory>
#include <cstring>
//#include <stdarg.h>
#include <cstdio>
#include <cmath>
#include <ios>
//#include <io.h>
//#include <fcntl.h>
//More includes:
#include <fstream>
#include "resource.h"
#include <SDL/SDL.h>
using namespace std;
#include "basefunc.cpp"
#include "errlog.cpp"
#include "tga.cpp"
//namespace dxmanse {

//Defines for windows
#define WINDOW_CLASS_NAME "WINCLASS1"
// DEFINES ////////////////////////////////////////////////
//Default screen size
#define SCREEN_WIDTH			640
#define SCREEN_HEIGHT			480
#define SCREEN_OFFSET_X           320//screen (a targa buffer) is offset to avoid clipping
#define SCREEN_OFFSET_Y           240
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
//#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct);}
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
typedef unsigned char	BYTE;
typedef unsigned char	BYTE;


// GLOBALS ////////////////////////////////////////////////
BYTE alphaLook[256][256][256]; //lookup for alpha result: ((Source-Dest)*alpha/256+Dest)

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
SDL_Surface *screen = NULL;
//Other globals:
DWORD								 start_clock_count = 0; // used for timing
LPTARGA				lptargaBackdrop=NULL;
LPTARGA				lptargaIntro=NULL;
LPTARGA				*lptargaAlien;
LPTARGA				*lptargaAlienBoss;
LPTARGA				*lptargaHero;
LPTARGA				*lptargaShot;
LPTARGA				*lptargaGameScreen;
LPTARGA             lptargaScreen;

char buffer[80];														 // general printing buffer
int explosionResult = 0;
int gameState = GAME_STATE_INIT;
int gameLevel = 1;					//3 Levels total
int gameStage = 1;					//3 Stages total, stages are within Levels
int index;		 // looping index
int numOfAliens = 0;


// PROTOTYPES	//////////////////////////////////////////////

int TargaStride(LPTARGA image);
void TargaToScreen_AutoCrop(LPTARGA image);
void TargaToScreen(LPTARGA image);
int TargaToTarga32_Alpha(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY);//, int opacityCap);
int TargaToTarga32_AlphaQuickEdge_FX(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat);
int TargaToTarga32_AlphaQuickEdge_FX_Scaled(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat, float scale);
int TargaToTarga32(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY);
//universal (24- or 32-bit mode) Targa renderer:
int TargaToTargaAlphaFX(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat);

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
	//USES ALPHALOOK//TargaToTargaAlphaFX(frames[frameNow], lptargaScreen, rectRendered.left, rectRendered.top, 255-isAlien*127, isBomb, 0);
	
	
	if (z==4)
		TargaToTarga32_AlphaQuickEdge_FX(frames[frameNow], lptargaScreen, rectRendered.left, rectRendered.top, 255-isAlien*127, isBomb, 0);
	else
	    TargaToTarga32_AlphaQuickEdge_FX_Scaled(frames[frameNow], lptargaScreen, rectRendered.left, rectRendered.top, 255-isAlien*127, isBomb, 0, scale);
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
			////PlaySound((const char*)SOUND_ID_EXPLOSION/*"explosion.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
			exploding=1;
		}
		explodedBy++;
	}
}
void Alien::redraw(register RECT &rectRendered, register float &scale)
{
	//USES ALPHALOOK//TargaToTargaAlphaFX(frames[frameNow], lptargaScreen, rectRendered.left, rectRendered.top, 255-stunnedness, explodedBy, dwStatus);
	
	//static int animation_seq[4] = {0,1,0,2};
	if (z==4) TargaToTarga32_AlphaQuickEdge_FX(frames[frameNow], lptargaScreen, rectRendered.left, rectRendered.top, 255-stunnedness, explodedBy, dwStatus);
	else			TargaToTarga32_AlphaQuickEdge_FX_Scaled(frames[frameNow], lptargaScreen, rectRendered.left, rectRendered.top, 255-stunnedness, explodedBy, dwStatus, z/4.00);
	
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
							//PlaySound((const char*)SOUND_ID_SHIELDZAP/*"shieldzap.wav"*//*"shieldzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
						}
						else
						{
							stunnedness=10;
							iHP-=16;
							delete shots[index];
							shots[index]=NULL;
							//PlaySound((const char*)SOUND_ID_OUCHALIEN/*"ouchalien.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
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
			//PlaySound((const char*)SOUND_ID_EXPLOSION/*"explosion.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
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
				//PlaySound((const char*)SOUND_ID_LASER/*"laser.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
				////PlaySound((const char*)SOUND_ID_LASER, hinstance_app, SND_RESOURCE | SND_ASYNC);
			}
		}
	}
}
void Hero::redraw()
{
	//USES ALPHALOOK//TargaToTargaAlphaFX(frames[frameNow], lptargaScreen, x, y-verticle, 255-stunnedness, explodedBy, dwStatus);
	if (z==4) TargaToTarga32_AlphaQuickEdge_FX(frames[frameNow], lptargaScreen, x, y-verticle, 255-stunnedness, explodedBy, dwStatus);
	else		TargaToTarga32_AlphaQuickEdge_FX_Scaled(frames[frameNow], lptargaScreen, x, y-verticle, 255-stunnedness, explodedBy, dwStatus, z/4.00);
}
void Hero::drawMeters()
{

	//register DWORD *this_buffer = 
	register BYTE *byteBuffer = lptargaScreen->buffer;
	static int iStride=TargaStride(lptargaScreen);
	//register DWORD pixel;// = _RGB32BIT(255,255,255,255); //remember this MACRO is ARGB unlike actual screen
	register int toX=2, toY=SCREEN_HEIGHT-3;
	for (int index=0; index<=gunPower*100; index++)
	{
		//pixel=_RGB32BIT(255,55+index*2,55+index*2,155+index); 
		for (register int offset=0; offset<=13; offset++)
		{
			//this_buffer[toX + ((toY)*iStride >> 2)] = pixel;
			//this_buffer[int(index*100)] = pixel;
			for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
				byteBuffer[(toX + offset)*REAL_BYTEDEPTH + (toY-index)*iStride + byteNow] = 255-130+offset*10-index;
			
		}
	}

	toX=18;
	toY=SCREEN_HEIGHT-2;
/*
	pixel=_RGB32BIT(255,255,BYTE(iHP*.85),0);
	for (index=0; index<iHP; index++)
	{
		this_buffer[toX + index + ((toY)*iStride >> 2)] = pixel;
		//for (byteNow
	}
*/
    
	for (index=0; index<iHP; index++)
	{

		for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
			byteBuffer[(toX+index)*REAL_BYTEDEPTH + toY*iStride + byteNow] = (byteNow==3) ? 255 : (byteNow==2) ? 255 : iHP;
	}
}
void Hero::drawTarget(int alienNum)
{

	//for (int alienNow = 0; alienNow < MAXALIENS; alienNow++)
	//{
		//if (aliens[alienNum] != NULL)
	    
		if (aliens[index]->z==z) if (aliens[index]->x>5) if (aliens[index]->x<SCREEN_WIDTH-6)
		{
			//register DWORD *this_buffer = 
			register BYTE *byteBuffer = lptargaScreen->buffer;
			static int iStride=TargaStride(lptargaScreen);
			//register DWORD pixel;// = _RGB32BIT(255,255,255,255); //remember this MACRO is ARGB unlike actual screen
			register int toX=aliens[index]->x-3, toY=aliens[index]->y-3;

			for (int index=0; index<7; index++)
			{
				for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
				{//draw a crosshairs
					byteBuffer[(toX+index)*REAL_BYTEDEPTH + (toY+3)*iStride + byteNow] = (byteNow==1)? 255 : 0; //across
					byteBuffer[(toX+3)*REAL_BYTEDEPTH + (toY+index)*iStride + byteNow] = (byteNow==1)? 255 : 0; //down
				}
			}
			
/*			toX=aliens[index]->x;
			toY=aliens[index]->y-2;
			for (index=0; index<5; index++)
			{
	
				for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
					byteBuffer[(toX)*REAL_BYTEDEPTH + (toY+index)*iStride + byteNow] = (byteNow==1)? 255 : 0;
			}
*/			

		}
	//}
}
void Hero::hitDetect()
{
	register float scaler=z/4.0f;
	int xCenter=x+width/2*scaler;
	int yCenter=y-verticle+height/2*scaler;

	//HIT ALIEN:
	for (int index=0; index<MAXALIENS; index++)
	{
		
		if (aliens[index] != NULL) {
                          
        //CENTERPOINT DISTANCE-BASED
		//	if ( abs(aliens[index]->x-50-xCenter) < 100) //100 IS SLOPPY BUT USSIVL DIST -50 SO HIT IS NEAR FRONT
		//		if ( abs(aliens[index]->y-yCenter) < 80) //80 IS SLOPPY BUT USSIVL DIST
		//			if (aliens[index]->z==z)
		//   		{
		//				PlaySound((const char*)SOUND_ID_OUCHZAP/*"ouchzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
		//       		stunnedness=10;
		//				iHP-=50;
		//			}
		
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
									//PlaySound((const char*)SOUND_ID_SHIELDZAP/*"shieldzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
									stunnedness=10;
									iHP+=50;
								}
								else
								{
									//PlaySound((const char*)SOUND_ID_OUCHZAP/*"ouchzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
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
							//PlaySound((const char*)SOUND_ID_OUCHZAP/*"ouchzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
						}
						else
						{
							stunnedness=10;
							iHP-=15;
							delete shots[index];
							shots[index]=NULL;
							//PlaySound((const char*)SOUND_ID_OUCHZAP/*"ouchzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
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
	//PlaySound((const char*)SOUND_ID_TRUMPET/*"trumpet.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
	}
}
void Hero::precisionaim()
{
	if (!(dwStatus & STATUS_PRECISIONAIM))
	{
	dwStatus |= STATUS_PRECISIONAIM;
	//PlaySound((const char*)SOUND_ID_TRUMPET/*"trumpet.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
	}
}

// GLOBAL CLASS POINTERS ////////////////////////////////////////////
Hero				*hero = NULL;

// FUNCTIONS ////////////////////////////////////////////////
int TargaStride(LPTARGA image) {
    if (image==NULL) return 0;
    return image->width*(image->byBitDepth/8);
}

void TargaToScreen_AutoCrop(LPTARGA image) {
		//assumes 32-bit screen?
		//for (int iLine=0; iLine<SCREEN_WIDTH; iLine++) {
        //}
        //debug SLOW
        SDL_Rect rect;
        Uint32 color;
    
        
        //color = SDL_MapRGB (screen->format, 0, 0, 0);
        //SDL_FillRect (screen, NULL, color);// Create a black background 
        rect.w=1;
        rect.h=1;
        rect.x=0;
        rect.y=0;
        BYTE *lpbyNow=image->buffer;
        lpbyNow+=SCREEN_OFFSET_Y*lptargaScreen->width+SCREEN_OFFSET_X;
        int iMarginsOffset=SCREEN_WIDTH*4;
        for (int yPix=0; yPix<SCREEN_HEIGHT; yPix++, rect.y++, rect.x=0) {
            for (int xPix=0; xPix<SCREEN_WIDTH; xPix++, rect.x++) {
                //color=(Uint32)*lpbyNow;
                color=SDL_MapRGB(screen->format,lpbyNow[2],lpbyNow[1],*lpbyNow);
                SDL_FillRect (screen, &rect, color);
                lpbyNow+=4;
                //lptargaNew->buffer = (BYTE *)malloc(lptarga1->width * lptarga1->height * iByteDepth);
            }
            lpbyNow+=iMarginsOffset;
        }
        SDL_Flip (screen);
}

///////////////////////////////////////////////////////////	 

void TargaToScreen(LPTARGA image) {
		//assumes 32-bit screen?
		//for (int iLine=0; iLine<SCREEN_WIDTH; iLine++) {
        //}
        //debug SLOW
        SDL_Rect rect;
        Uint32 color;
    
        //color = SDL_MapRGB (screen->format, 0, 0, 0);
        //SDL_FillRect (screen, NULL, color);// Create a black background 
        rect.w=1;
        rect.h=1;
        rect.x=0;
        rect.y=0;
        BYTE *lpbyNow=image->buffer;
        lpbyNow+=SCREEN_OFFSET_Y*lptargaScreen->width+SCREEN_OFFSET_X;
        for (int yPix=0; yPix<SCREEN_HEIGHT; yPix++, rect.y++, rect.x=0) {
            for (int xPix=0; xPix<SCREEN_WIDTH; xPix++, rect.x++, lpbyNow+=4) {
                //color=(Uint32)*lpbyNow;
                color=SDL_MapRGB(screen->format,lpbyNow[2],lpbyNow[1],*lpbyNow);
                SDL_FillRect (screen, &rect, color);
                //lptargaNew->buffer = (BYTE *)malloc(lptarga1->width * lptarga1->height * iByteDepth);
            }
        }
        SDL_Flip (screen);
}

///////////////////////////////////////////////////////////	 

int TargaToTarga32_Alpha(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY)//, int opacityCap)
{
	// copy the Targa image to the primary buffer line by line
	//	direct draw surface description 
	// set size of the structure

	register DWORD *this_buffer = (DWORD *)lptargaThis1->buffer;
    int iStride=TargaStride(lptargaThis1);

	register BYTE *dPtr = (BYTE*)&this_buffer[toX + (toY*iStride >> 2)]; //dest pointer
	register BYTE *sPtr = image->buffer;														 //source pointer
	
	register BYTE alpha,blue,green,red;
	register DWORD pixel;
	//int adddPos=iStride-nWidth*4;
	//register int adddPos=iStride >> 2;//-image->width*4; //in case not linear
	//int addsPos=lPitch-nWidth*4;
	//register int addsPos=1 ;//assume linear

	// process each line and copy it into the primary buffer
	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			/*
			//Get BGR values and alpha
			BYTE blue	= (image->buffer[index_y*image->width*4 + index_x*4 + 0]),
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
								this_buffer[toX + index_x + ((index_y+toY)*iStride >> 2)] = pixel;

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
								this_buffer[toX + index_x + ((index_y+toY)*iStride >> 2)] = pixel;
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
								this_buffer[toX + index_x + ((index_y+toY)*iStride >> 2)] = pixel;
			}

		} // end for index_x
		dPtr+=iStride-image->width*4;
	} // end for index_y

	return(1);
} // end TargaToTarga32_Alpha

///////////////////////////////////////////////////////////////

int TargaToTarga32_AlphaQuickEdge_FX(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat)
{
	register DWORD *this_buffer = (DWORD *)lptargaThis1->buffer;
	int iStride=TargaStride(lptargaThis1);
	register BYTE *dPtr = (BYTE*)&this_buffer[toX + (toY*iStride >> 2)];
	register BYTE *sPtr = image->buffer;
	register BYTE blue,green,red,alpha;
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
								this_buffer[index_x + toX + ((index_y + toY)*iStride >> 2)] = pixel;
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
								this_buffer[index_x + toX + ((index_y + toY)*iStride >> 2)] = pixel;
							}
			}
		}
		dPtr+=iStride-image->width*4;
	}
	return(1);
} // end TargaToTarga32_AlphaQuickEdge_FX

////////////////////////////////////////////////////////////////////////////////

int TargaToTarga32_AlphaQuickEdge_FX_Scaled(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat, float scale)
{
	register DWORD *this_buffer = (DWORD *)lptargaThis1;
	int iStride=TargaStride(lptargaThis1);
	register BYTE *dPtr = (BYTE*)&this_buffer[toX + (toY*iStride >> 2)];
	register BYTE *sPtr = image->buffer;
	register BYTE blue,green,red,alpha;
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
								this_buffer[toX + scaledX + ((scaledY+toY)*iStride >> 2)] = pixel;
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
								this_buffer[toX + scaledX + ((scaledY+toY)*iStride >> 2)] = pixel;
							}
			}
		}
		dPtr+=iStride-image->width*4;
	}
	return(1);
} // end TargaToTarga32_AlphaQuickEdge_FX_Scaled

///////////////////////////////////////////////////////////////////////////////

int TargaToTarga32(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY)
{
	DWORD *this_buffer = (DWORD *)lptargaThis1->buffer;
    int iStride=TargaStride(lptargaThis1);
	// process each line and copy it into the primary buffer
	for (int index_y = 0; index_y < image->height; index_y++)
	{
		for (int index_x = 0; index_x < image->width; index_x++)
		{
			// get BGR values
			BYTE blue	= (image->buffer[index_y*image->width*4 + index_x*4 + 0]),
				green = (image->buffer[index_y*image->width*4 + index_x*4 + 1]),
				red	 = (image->buffer[index_y*image->width*4 + index_x*4 + 2]),
				alpha = (image->buffer[index_y*image->width*4 + index_x*4 + 3]);

			// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
			DWORD pixel = _RGB32BIT(alpha,red,green,blue);

			// write the pixel
			this_buffer[toX + index_x + ((index_y+toY)*iStride >> 2)] = pixel;

		} // end for index_x

	} // end for index_y

	return(1);
} //end TargaToTarga32

////////////////////////////////////////////////////////////////////////////////

int TargaToTargaAlphaFX(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat)
{//uses alpha lookup
	//register DWORD *this_buffer = (DWORD *)lptargaThis1->buffer;
    int iStride=TargaStride(lptargaThis1);
	register BYTE *dPtr = lptargaThis1->buffer;//(BYTE*)&this_buffer[toX + (toY*iStride >> 2)]; //dest pointer
	register BYTE *sPtr = (BYTE *)image->buffer;														 //source pointer
	register BYTE *sPtrAlpha = (BYTE *)image->buffer;
	sPtrAlpha+=3; //move pointer to position of alpha byte
	register int draw=0;
	register float cookedAlpha=1;
	
	
	
	//register BYTE alpha,blue,green,red;
	//register DWORD pixel;

	//int adddPos=iStride-nWidth*4;
	//register int adddPos=iStride >> 2;//-image->width*4; //in case not linear
	//int addsPos=lPitch-nWidth*4;
	//register int addsPos=1 ;//assume linear

	//Move the dest pointer to the starting pixel
	dPtr+=toX*REAL_BYTEDEPTH;
	dPtr+=toY*iStride;
	int offset = iStride - image->width*REAL_BYTEDEPTH;

	//int scalerAdder=5-scale*4 //ADD THIS TO indexES INSTEAD OF ++, AND MULTIPLY USES OF width BY scale
	
	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			draw=0;	//for clipping
			/*
			//Get BGR values and alpha
			BYTE blue	= (image->buffer[index_y*image->width*4 + index_x*4 + 0]),
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
	return(1);
} // end TargaToTarga_Alpha

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
			
			TargaToTarga32_Alpha(lptargaGameScreen[2], lptargaScreen,
				SCREEN_WIDTH/2-lptargaGameScreen[2]->width/2,
				SCREEN_HEIGHT/2-lptargaGameScreen[2]->height/2);
            TargaToScreen_AutoCrop(lptargaScreen);
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
		TargaToTarga32(lptargaIntro, lptargaScreen, SCREEN_WIDTH/2-lptargaIntro->width/2,0);
		TargaToScreen_AutoCrop(lptargaScreen);
		//PlaySound((const char*)SOUND_ID_INTRO/*"Orangejuice-DXMan-Intro.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC | SND_LOOP);
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
			//errlog.AddPrecedent("About to call TargaUnload(lptargaBackdrop)", "START_LEVEL 1");
			if (lptargaBackdrop) TargaUnload(lptargaBackdrop);
			errlog.Add(iLastErr, "TargaUnload lptargaBackdrop", "START_LEVEL");
			//errlog.AddPrecedent("About to call TargaLoad(\"level1Back.tga\")", "START_LEVEL 1");
			lptargaBackdrop=TargaLoad("level1Back.tga");
			errlog.Add(iLastErr, "TargaLoad level1Back.tga", "START_LEVEL");
			if (iLastErr)
			{
				iErrorsSaved++;
				return (0);
			}

			/*
			//32-bit backdrop surface
			if (!TargaToTarga32(lptargaBackdrop, lptargaScreendrop, 0, 0))
			{	
				iErrorsSaved++;
				error_txt << "\nUnable to render Targa to 32-bit Backdrop in START_LEVEL 1";
				return(0);
			}
			
            */
			//errlog.AddPrecedent("About to call TargaUnload(lptargaBackdrop)","START_LEVEL 1");
			//TargaUnload(lptargaBackdrop);
			//errlog.Add(iLastErr, "unload lptargaBackdrop","START_LEVEL");
		}
		else if (gameLevel==2)
		{
			//errlog.AddPrecedent("Calling TargaUnload(lptargaBackdrop)", "START_LEVEL 2);
			if (lptargaBackdrop) TargaUnload(lptargaBackdrop);
			errlog.Add(iLastErr, "unload lptargaBackdrop","START_LEVEL 2");
			//errlog.AddPrecedent("TargaLoad(\"level2Back.tga\")", "START_LEVEL 2");
			lptargaBackdrop=TargaLoad("level2Back.tga");
			errlog.Add(iLastErr, "level2Back.tga","START_LEVEL 2");
			if (iLastErr)
			{
				iErrorsSaved++;
				return (0);
			}
/*
			//32-bit backdrop surface
			if (!TargaToTarga32(lptargaBackdrop, lptargaScreendrop, 0, 0))
			{	
				error_txt << "\nUnable to render Targa to 32-bit Backdrop in START_LEVEL 2";
				iErrorsSaved++;
				return(0);
			}
			

			//errlog.AddPrecedent("Calling TargaUnload(lptargaBackdrop)","START_LEVEL 2");
			TargaUnload(lptargaBackdrop);
			errlog.Add(iLastErr, "unload lptargaBackdrop", "START_LEVEL 2");
			*/
		}
		else if (gameLevel==3)
		{
			//errlog.AddPrecedent("Calling TargaUnload(lptargaBackdrop)");
			if (lptargaBackdrop) TargaUnload(lptargaBackdrop);
			errlog.Add(iLastErr, "unload lptargaBackdrop", "START_LEVEL 3");
			lptargaBackdrop=TargaLoad("level3Back.tga");
			errlog.Add(iLastErr, "level3Back.tga", "START_LEVEL 3");
			if (iLastErr)
			{
				iErrorsSaved++;
				return (0);
			}
/*
			if (REAL_BPP == 24)
			{
				if (!TargaToTarga32(lptargaBackdrop, lptargaScreendrop, 0, 0))//ix)
				{
					error_txt << "\nUnable to render targaBack to 24-bit surface";
					iErrorsSaved++;
					return(0);
				}
			}
			else
			{
				if (!TargaToTarga32(lptargaBackdrop, lptargaScreendrop, 0, 0))
				{
					error_txt << "\nUnable to render targaBack to 32-bit surface";
					iErrorsSaved++;
					return(0);
				}
			}

			//errlog.AddPrecedent("Calling TargaUnload(lptargaBackdrop)", "START_LEVEL 3");
			TargaUnload(lptargaBackdrop);
			errlog.Add(iLastErr, "unload lptargaBackdrop", "START_LEVEL 3");
			*/
		}


		hero->iHP=255;
		if (gameLevel==2) hero->dwStatus |= STATUS_ANTIGRAVITY;
		gameState = GAME_STATE_START_STAGE;
	}

	else if (gameState == GAME_STATE_START_STAGE)
	{//START STAGE
		bombed=0;
		//PlaySound((const char*)SOUND_ID_INVASION/*"Orangejuice-DXMan-Invasion.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC | SND_LOOP);
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

		TargaToTarga32(lptargaBackdrop, lptargaScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y);
        //DDrawDrawSurface(lptargaScreendrop, 0, 0, SCREEN_WIDTH+1, SCREEN_HEIGHT+1, lptargaScreen);

		//Update all the existing aliens, delete the rest
		for (index=0; index < MAXALIENS; index++)
		{//Prototype is (LPTARGA image, LPTARGA lptargaThis1, int toX, int toY_)
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


		//DDrawDrawSurface(lptargaExplosion, 100,100,300,300,lptargaScreen);
		
		/*
		if(calculateExplosion(lptargaScreen, 100, 100, 20, 0, EXPLOSION_CHECK_COUNT) == 0)
		{
			//DDrawFillSurface(lptargaScreen,0);
			explosionResult = calculateExplosion(lptargaScreen, 100, 100, 20, 30, EXPLOSION_START);
		}
		else
			explosionResult = calculateExplosion(lptargaScreen, 100, 100, 20, 30, EXPLOSION_CONTINUE);
		//error_txt << "\nResult of explosionCalculate = " << explosionResult;
		*/

		// flip pages:
		TargaToScreen_AutoCrop(lptargaScreen);
		
		if (numOfAliens<=0) gameState = GAME_STATE_WIN_STAGE;
	}
	else if (gameState == GAME_STATE_SHUTDOWN)
	{
	}
	else if (gameState == GAME_STATE_WIN_STAGE)
	{
		//PlaySound((const char*)SOUND_ID_TRUMPET/*"trumpet.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
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
			//
			//	TargaToTarga32_Alpha(lptargaGameScreen[screenNow], lptargaScreen,
			//		SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2,
			//		SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2);
			//
			TargaToTarga32_Alpha(lptargaGameScreen[screenNow],lptargaScreen,
					SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2,
					SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2);
			TargaToScreen_AutoCrop(lptargaScreen);
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

		//	TargaToTarga32_Alpha(lptargaGameScreen[screenNow], lpddsPrimary,
		//		SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2,
		//		SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2);
		TargaToTarga32_Alpha(lptargaGameScreen[screenNow], lptargaScreen,
				SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2,
				SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2);

		//PlaySound((const char*)SOUND_ID_ENDING/*"Orangejuice-DXMan-Ending.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		DWORD startTime=GetTickCount();
		while((GetTickCount() - startTime) < 300);
		while(KEYUP(VK_ESCAPE));
		gameState = GAME_STATE_EXIT;
	}
	else if (gameState == GAME_STATE_YOU_LOSE)
	{
		//PlaySound((const char*)SOUND_ID_THRUST/*"thrust.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
		TargaToTarga32_Alpha(lptargaGameScreen[0], lptargaScreen,
			SCREEN_WIDTH/2-lptargaGameScreen[0]->width/2,
			SCREEN_HEIGHT/2-lptargaGameScreen[0]->height/2);
		TargaToScreen_AutoCrop(lptargaScreen);
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
	//errlog.AddTitle("Starting GameInit");

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
		for (BYTE source=0; source<256; source++)
		{
		for (BYTE dest; dest<256; dest++)
		{
			for (BYTE alpha; alpha<256; alpha++)
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
	lptargaScreen=TargaNew(SCREEN_WIDTH*2, SCREEN_HEIGHT*2,32,TGATYPE_UTC);

	//RECT explosionRect={0,0,200,200};
	RECT screenRect= {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

	REAL_BPP=32;
	REAL_BYTEDEPTH=4;

    char *msg;
    int done;

    /* Initialize SDL */
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
        sprintf (msg, "Couldn't initialize SDL: %s\n", SDL_GetError ());
        MessageBox (0, msg, "Error", MB_ICONHAND); 
        free (msg);
        exit (1);
    }
    atexit (SDL_Quit);

    /* Set 640x480 16-bits video mode */
    screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_WIDTH, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        sprintf (msg, "Couldn't set video mode: %s\n",
          SDL_GetError ());
        MessageBox (0, msg, "Error", MB_ICONHAND); 
        free (msg);
        exit (2);
    }
    SDL_WM_SetCaption ("DXMan", NULL);
    
   REAL_BPP=32;
   REAL_BYTEDEPTH=4;

	if (lptargaBackdrop) TargaUnload(lptargaBackdrop);
	errlog.Add(iLastErr, "unload lptargaBackdrop", "GameInit");
	lptargaBackdrop=TargaLoad("level1Back.tga");
	errlog.Add(iLastErr, "level1Back.tga", "GameInit");
	if (iLastErr)
	{
		iErrorsSaved++;
		return (0);
	}
	//lptargaScreendrop = DDrawCreateSurface(lptargaBackdrop->width,lptargaBackdrop->height,0);	
	//	if (!TargaToTarga32(lptargaBackdrop, lptargaScreendrop, 0, 0))
	//	{
	//		error_txt <<"\nCould not load targaBack to 32-bit BackDrop in GameInit";
	//		iErrorsSaved++;
	//		return(0);
	//	}
	//TargaUnload(lptargaBackdrop);
	errlog.Add(iLastErr, "unload lptargaBackdrop", "GameInit");


	//lptargaExplosion = DDrawCreateSurface(300,300,0); //Not used yet
	//DDrawFillSurface(lptargaExplosion,0);
	int iFramesHero=21,
		iFramesAlien=2,
		iFramesAlienBoss=3,
		iFramesShot=2,
		iFramesGameScreen=9;
	int iFramesFound;

	iFramesFound=iFramesHero;
	//errlog.AddPrecedent("Loading heroani","GameInit");
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


    TargaUnload(lptargaScreen);
	errlog.Add(iLastErr, "lptargaScreen", "GameShutDown");
    TargaUnload(lptargaBackdrop);
	errlog.Add(iLastErr, "lptargaBackdrop", "GameShutDown");
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
	//winclass.hIcon			= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hCursor		= LoadCursor(hinstance, MAKEINTRESOURCE(IDC_CROSSHAIRS)); 
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
//}//end namespace
