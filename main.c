#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <windows.h>
// TYPES //////////////////////////////////////////////////////
//typedef struct {int left; int right; int top; int bottom;} RECT;
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char BYTE;
//typedef unsigned __int8 DWORD;
//typedef unsigned __int16 USHORT;
//typedef unsigned __int8 BYTE;
//typedef ULARGE_INTEGER __int64);

//#include "filewrap.h"
#include "resource.h"
#include "basefunc.cpp"
#include "errlog.cpp"
#include "sprite.cpp"
// DEFINES ////////////////////////////////////////////////
//Default screen size
#define SCREEN_WIDTH			640
#define SCREEN_HEIGHT			480
#define BUFFER_WIDTH			1280
#define BUFFER_HEIGHT			960
#define SCREENBUFFER_SIZE		1228800 //1280x960
#define SCREEN_OFFSET_X			320 //screen (a targa buffer) is offset to avoid clipping
#define SCREEN_OFFSET_Y			240
#define SCREEN_BPP				32	 //bits per pixel, GameInit tries 32 or 24 if one is not available
#define FMAX_EXPLODEDNESS		10.0f //how much an entity is exploded before it is "dead"
#define MAX_EXPLODEDNESS		10 //how much an entity is exploded before it is "dead"
#define AIM_MIN					0.0f
#define AIM_MAX					0.1f // max m/s for zVel
#define AIM_RESOLUTION			0.01f
#define CODE_THRESHOLD          90
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
#define STATUS_RAPIDFIRE        256
#define MAXALIENS				4
#define MAXSHOTS				20

#define GAMEKEY_UP				1
#define GAMEKEY_DOWN	 		2
#define GAMEKEY_LEFT	 		4
#define GAMEKEY_RIGHT			8
#define GAMEKEY_FIRE	 		16
#define GAMEKEY_JUMP	 		32
#define GAMEKEY_EXIT	 		64
#define GAMEKEY_RAPIDFIRE 		128
#define GAMEKEY_PRECISIONAIM	256
#define GAMEKEY_DOUBLESPEED		512
DWORD dwPressing=0;

#define SCREENITEM_ALIEN	1
#define SCREENITEM_SHOT		2
#define SCREENITEM_HERO		3
#define MAX_SCREENITEMS		50  //maximum number of objects to render per frame
#define MAX_ZORDER               50   //maximum zOrder value - INCLUSIVE
#define ZBUFFER_YRATIO		5.55f //multiply yNearness by this to get z-order
// STRUCTS ////////////////////////////////////////////////
typedef struct SCREENITEM_STRUCT {
	int iType;//entity type i.e. SCREENITEM_ALIEN
	int zOrder;
	int index;//entity index, irrelevant for hero
} SCREENITEM, * LPSCREENITEM;
int iScreenItems=0;
// MACROS /////////////////////////////////////////////////
#define ZORDER_FROMY_FORCE(y) ((((y)-4.0f)*-1)*ZBUFFER_YRATIO)
#define ZORDER_FROMY(y) ((int)(y<-4.0f) ? 0 : ((y>5.0f)?MAX_ZORDER:ZORDER_FROMY_FORCE(y)))	 //get rendering z-order from y-value
//Build a 32 bit color value in A.8.8.8 format (8-bit alpha mode):
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
//24-bit specific, needed because of some video cards:
#define _RGB24BIT(r,g,b) ((b) + ((g) << 8) + ((r) << 16))
//audio panning:
#define L_FROM_FACTOR(factorzerotoone) ((int)((1.0f-factorzerotoone)*255.0f))
#define R_FROM_FACTOR(factorzerotoone) ((int)(factorzerotoone*255.0f))
//geometry:
#define APPROACH(start,toward,factor) ((start)-((start)-(toward))*(factor))
//Get the b,g,r or a of BGRA pixel:
//#define _BYTEXOFPIXEL(x,pixel) ((pixel >> 8*4(x-1)) & 255)

// GLOBALS ////////////////////////////////////////////////
BYTE alphaLook[256][256][256]; //lookup for alpha result: ((Source-Dest)*alpha/256+Dest)
	int iFramesHero=21,
		iFramesAlien=2,
		iFramesAlienBoss=3,
		iFramesShot=3,
		iFramesGameScreen=9;
int ix;//temp var for error#'s
int iEscapeTime=0;
int iErrorsSaved=0;
int iFramesDropped=0;
int REAL_BPP=32;
int REAL_BYTEDEPTH=4;
int keyDelay=0;
int doublecode=0; //player cheat code
int precisecode=0; //player cheat code (enabled by default now)
int rapidfirecode=0;//player cheat code
int bombed=0;
DWORD								 start_clock_count = 0; // used for timing
LPTARGA				lptargaBackdrop=NULL;
LPTARGA				lptargaIntro=NULL;
LPTARGA*			lptargaAlien;
LPTARGA*			lptargaAlienBoss;
LPTARGA*			lptargaHero;
LPTARGA*			lptargaShot;
LPTARGA*			lptargaGameScreen;
LPTARGA				lptargaScreen;
//LPSPRITE*           lpspritearr;//sprite array
SCREENITEM			screenitemarr[MAX_SCREENITEMS];
char buffer[80];								// general printing buffer
int explosionResult = 0;
int gameState = GAME_STATE_INIT;
int gameLevel = 1;					//3 Levels total
int gameStage = 1;					//3 Stages total, stages are within Levels
int numOfAliens = 0;

bool bPlayTrumpet=false;
int iPlayBomb=-1;
int iPlayLaserAlien=-1;
int iPlayLaser=-1;
int iPlayExplosion=-1;
int iPlayOuchAlien=-1;
int iPlayOuchZap=-1;
int iPlayShieldZap=-1;
int iThruster=-1;
Mix_Chunk *mcBomb=NULL;
Mix_Chunk *mcLaserAlien=NULL;
Mix_Chunk *mcLaser=NULL;
Mix_Chunk *mcExplosion=NULL;
Mix_Chunk *mcOuchAlien=NULL;
Mix_Chunk *mcOuchZap=NULL;
Mix_Chunk *mcShieldZap=NULL;
Mix_Chunk *mcTrumpet=NULL;
Mix_Chunk *mcThruster=NULL;
int iChanBomb=-1;
int iChanLaserAlien=-1;
int iChanLaser=-1;
int iChanExplosion=-1;
int iChanOuchAlien=-1;
int iChanOuchZap=-1;
int iChanShieldZap=-1;
int iChanThruster=-1;
int iChanTrumpet=-1;

// PROTOTYPES	//////////////////////////////////////////////
int GameInit();
int GameMain();
int GameShutdown();
int TargaStride(LPTARGA image);
void TargaToScreen_AutoCrop(LPTARGA image);
void TargaToScreen(LPTARGA image);
int TargaToTarga32_Alpha(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY);//, int opacityCap);
int TargaToTarga32_FX(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat);
int TargaToTarga32_FX_Scaled(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat, float scale);
int TargaToTarga32(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY);
//universal (24- or 32-bit mode) Targa renderer:
int TargaToTargaAlphaFX(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat);
bool AddScreenItem(int iType, int zOrder, int iEntityIndex);
void DrawRadarDot(float xPos, float yPos, float zPos, DWORD dwPixel);
void DrawExclusiveRect(int left, int top, int bottom, int right, DWORD dwPixel, bool bFilled);
void DrawRadarField();
//void Approach(float &xMoveMe, float &yMoveMe, float &xToward, float &yToward);
// CLASSES ////////////////////////////////////////////////
class Shot {
private:
	RECT rectRendered; //inclusive edges
	LPTARGA frames[3];
	int iFrameNow, iFrames;
	float xVel, yVel, zVel;
	float fBaseScale;
public:
	float wMeters;//width (spherical diameter) in meters
	float scale;
	int width, height;	//actual Targa size
	int isAlien, isBomb; //used as booleans
	float x,y,z; //centerpoint in meters-- placement of image is calculated

	int dead;
	Shot();
	Shot(float x2, float y2, float z2, float xVel2, float yVel2, float zVel2, int isAlien2, int isBomb2);
	void refresh();
	void Translate3D();
	void redraw();//void redraw(register RECT &rectRendered, register float &scale);
	void setVars(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isAlien2, int isBomb2);
};
Shot::Shot() {
	width = lptargaShot[0]->width; //set real size
	height = lptargaShot[0]->height;
	wMeters=.25;
	fBaseScale=1;
	iFrames=iFramesShot;
	frames[0]=lptargaShot[0];
	frames[1]=lptargaShot[1];
	frames[2]=lptargaShot[2];
	iFrameNow=rand()%iFrames;
	x=0;
	y=0;
	z=4;
	xVel=1;
	yVel=1;
	zVel=1;
	isAlien=0;
	isBomb=0;
	dead=0;
}
Shot::Shot(float x2, float y2, float z2, float xVel2, float yVel2, float zVel2, int isAlien2, int isBomb2) {
	width = lptargaShot[0]->width; //set real size
	height = lptargaShot[0]->height;
	wMeters=.25;
    fBaseScale=1;
	iFrames=iFramesShot;
	frames[0]=lptargaShot[0];
	frames[1]=lptargaShot[1];
	frames[2]=lptargaShot[2];
	iFrameNow=rand()%iFrames;
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
void Shot::Translate3D() {

	//ratios
	static float
		fScaleFarNearWidthRatio = 4.96460,//561.0f/113.0f;//calculated by hand from a backdrop
		fNearScale=APPROACH(561.0f,113.0f,.5f)/561.0f;//ratio of middle to near//1.25;//scale at yNear
	//screen pixel reference lines
	static int
		yZero=SCREEN_HEIGHT*0.616667,
		xZero=SCREEN_WIDTH/2,
		xFocalPoint2D=SCREEN_HEIGHT/2,
		yFocalPoint2D=SCREEN_HEIGHT*0.29375,
		yBelowFocus2D=SCREEN_HEIGHT-yFocalPoint2D,
		yAboveFocus2D=SCREEN_HEIGHT-yBelowFocus2D,
		xNearExtreme=SCREEN_WIDTH*fNearScale,//virtual pixel width of range, at near y
        fHalfScreenW=SCREEN_WIDTH/2,
        xNearScreenW=fNearScale*SCREEN_WIDTH;//width of grid is beyond screen width at near y
	//3D reference lines at parts of screen
	static float
		zTop=9, //top of screen when y==-4
		zBottom=0,
		zFocalPoint3D=4.5,
		yFar=4,//a minimum movement range for the game
		yNear=-4, //bottom of screen when z==0, top when z==zTop
		yRange=8,//range from far to near
		xEdge=6;//edge of screen when y==0

	//keep in mind that positive y is farther away
	//--negative y is closer


	//translate 2d coords to 3d
	int xTranslate, yTranslate;
	//float zFocused=z-zFocalPoint3D;
	float yFromFar=-(y-yFar);
	float yNearness=yFromFar/yRange;
	float yNearnessExp=(yFromFar*yFromFar)/(yRange*yRange);
	yTranslate = (z<zFocalPoint3D)//(zFocused<0)
		? (yFocalPoint2D+yNearnessExp*yBelowFocus2D)
		: (yFocalPoint2D-yNearnessExp*yAboveFocus2D);
	//xTranslate = (y>=0)  //remember +y is farther
	//	? APPROACH(x/xEdge*fHalfScreenW+fHalfScreenW, xFocalPoint2D, yNearnessExp)
	//	: (x/xEdge*fHalfScreenW*(yNearNessExp+))+fHalfScreenW
	xTranslate=APPROACH(x/xEdge*(fNearScale*fHalfScreenW)+fHalfScreenW,xFocalPoint2D,yNearnessExp);
	scale=fNearScale*(yFromFar/yRange);//SET SCALE
	scale*=fBaseScale;
	if (scale<.05) scale=.05;//prevents zero size after using scale
	rectRendered.left=xTranslate-width/2*scale;
	rectRendered.top=yTranslate-height/2*scale;
	rectRendered.right=rectRendered.left+scale*width;
	rectRendered.bottom=rectRendered.top+scale*height;

}
void Shot::refresh() {
// ORDER: // (hit detect after draw is okay since the only hit detection here is edge hitting; alien and hero are better)
	//SET RECT//
	//DRAW//
	//MOVE//
	//HITDETECT// and //FIX POSITION (not needed, hit detection kills it at screen edge)//


	// SET RECT //
	Translate3D();

	iFrameNow++;
	if (iFrameNow>=iFrames) iFrameNow=0;

	redraw();//redraw(rectRendered, scale);

	x+=xVel;
	y+=yVel;
	z+=zVel;

	if (rectRendered.left<0 || rectRendered.right>=SCREEN_WIDTH || rectRendered.top<0 || rectRendered.bottom>=SCREEN_WIDTH
		|| z<=0)
		dead = 1;
}
void Shot::redraw() {
	//USES ALPHALOOK//TargaToTargaAlphaFX(frames[iFrameNow], lptargaScreen, rectRendered.left, rectRendered.top, 255-isAlien*127, isBomb, 0);
	
	////if (z==4)
	////	TargaToTarga32_FX(frames[iFrameNow], lptargaScreen, SCREEN_OFFSET_X+rectRendered.left, SCREEN_OFFSET_Y+rectRendered.top, 255-isAlien*127, isBomb, 0);
	////else
		TargaToTarga32_FX_Scaled(frames[iFrameNow], lptargaScreen, SCREEN_OFFSET_X+rectRendered.left, SCREEN_OFFSET_Y+rectRendered.top, 255-isAlien*127, isBomb, 0, scale);
}
void Shot::setVars(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isAlien2, int isBomb2){
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

class Alien {
public:
	int iFrames;
	LPTARGA frames[4]; //frames of animation for cycle
	DWORD dwStatus;				//For special powers, color channels omitted etc.
	int iHP;					//Hit Points left
	float fFortitude;			//If < 1: slow down, flash red, and approach 1
	float x,y,z;			//3D CENTERPOINT
	int xVel;
	int yVel;
	int zVel;
	int iFrameNow;							//Current frame of animation
	int iFrameDelay;
	int iFrameDelayed;
	int width;
	int height;										 //Size of surface
	int explodedBy;					//Size of explosion, incremented when dead
	int exploding; //used as boolean
	int shootDelay;
    RECT rectRendered;
	float scale;
	float wMeters, dMeters, hMeters;//size in meters

	float fBaseScale;
	Alien();
	Alien(float x2, float y2, float z2);
	~Alien();
	void setAsBoss();
	void refresh();
	void hitDetect();
	void redraw();//void redraw(register RECT &rectRendered, register float &scale);
	void GetHit(int iShotIndex);
	void Translate3D();
private:
	void shoot();
	void bomb();
};
Alien::Alien() {
	iFrames=4;
    fBaseScale=1.0f;
	numOfAliens++;
	srand(SDL_GetTicks());
	dwStatus = STATUS_ALIVE;
	iHP = 30 * gameStage;
	fFortitude = 1.0f;
	x = 0;
	y = 0;
	z = 4;
	wMeters=2;
	dMeters=.5;
	hMeters=.5;
	xVel = -(.2 + (rand()%4)*.05) - gameStage*.05;
	yVel = .1;
	zVel = 0;
	iFrameNow = 0;
	iFrameDelay = 7;
	iFrameDelayed=0;
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
Alien::Alien(float x2, float y2, float z2) {
	iFrames=4;
    fBaseScale=1.0f;
	numOfAliens++;
	srand(SDL_GetTicks());
	dwStatus = STATUS_ALIVE;
	iHP = 30 * gameStage;
	fFortitude = 1.0f;
	x = x2;
	y = y2;
	z = z2;
	wMeters=2;
	dMeters=.5;
	hMeters=.5;
	xVel = -(2 + rand()%2) - gameStage;
	yVel = -1;
	zVel = 0;
	iFrameNow = 0;
	iFrameDelay = 7;
	iFrameDelayed=0;
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
Alien::~Alien(){
	numOfAliens--;
}
void Alien::Translate3D() {
	//ratios
	static float
		fScaleFarNearWidthRatio = 4.96460,//561.0f/113.0f;//calculated by hand from a backdrop
		fNearScale=APPROACH(561.0f,113.0f,.5f)/561.0f;//ratio of middle to near//1.25;//scale at yNear
	//screen pixel reference lines
	static int
		yZero=SCREEN_HEIGHT*0.616667,
		xZero=SCREEN_WIDTH/2,
		xFocalPoint2D=SCREEN_HEIGHT/2,
		yFocalPoint2D=SCREEN_HEIGHT*0.29375,
		yBelowFocus2D=SCREEN_HEIGHT-yFocalPoint2D,
		yAboveFocus2D=SCREEN_HEIGHT-yBelowFocus2D,
		xNearExtreme=SCREEN_WIDTH*fNearScale,//virtual pixel width of range, at near y
        fHalfScreenW=SCREEN_WIDTH/2,
        xNearScreenW=fNearScale*SCREEN_WIDTH;//width of grid is beyond screen width at near y
	//3D reference lines at parts of screen
	static float
		zTop=9, //top of screen when y==-4
		zBottom=0,
		zFocalPoint3D=4.5,
		yFar=4,//a minimum movement range for the game
		yNear=-4, //bottom of screen when z==0, top when z==zTop
		yRange=8,//range from far to near
		xEdge=6;//edge of screen when y==0

	//keep in mind that positive y is farther away
	//--negative y is closer


	//translate 2d coords to 3d
	int xTranslate, yTranslate;
	//float zFocused=z-zFocalPoint3D;
	float yFromFar=-(y-yFar);
	float yNearness=yFromFar/yRange;
	float yNearnessExp=(yFromFar*yFromFar)/(yRange*yRange);
	yTranslate = (z<zFocalPoint3D)//(zFocused<0)
		? (yFocalPoint2D+yNearnessExp*yBelowFocus2D)
		: (yFocalPoint2D-yNearnessExp*yAboveFocus2D);
	//xTranslate = (y>=0)  //remember +y is farther
	//	? APPROACH(x/xEdge*fHalfScreenW+fHalfScreenW, xFocalPoint2D, yNearnessExp)
	//	: (x/xEdge*fHalfScreenW*(yNearNessExp+))+fHalfScreenW
	xTranslate=APPROACH(x/xEdge*(fNearScale*fHalfScreenW)+fHalfScreenW,xFocalPoint2D,yNearnessExp);
	scale=fNearScale*(yFromFar/yRange);//SET SCALE
	scale*=fBaseScale;
	if (scale<.05) scale=.05;//prevents zero size after using scale
	rectRendered.left=xTranslate-width/2*scale;
	rectRendered.top=yTranslate-height/2*scale;
	rectRendered.right=rectRendered.left+scale*width;
	rectRendered.bottom=rectRendered.top+scale*height;
}
void Alien::setAsBoss(){
	width = lptargaAlienBoss[0]->width; // set real size
	height = lptargaAlienBoss[0]->height;

	frames[0] = lptargaAlienBoss[0];
	frames[1] = lptargaAlienBoss[1];
	frames[2] = lptargaAlienBoss[2];
	frames[3] = lptargaAlienBoss[1];

	dwStatus = STATUS_ALIVE | STATUS_BOMBER | STATUS_SHIELD | STATUS_BOSS;
	iHP=1000;
}
void Alien::refresh(){
	//scale = z/4.0f;

// ORDER: //

	//- UPDATE+MOVE//
	//- FIX POSITION// -get it ready for the next set rect and next render
	//- SET RECT//
	//- DRAW//
	//- HITDETECT&CHECK LIFE// -this way it is using the rectRendered for hit detection

	// UPDATE+MOVE //
	if (fFortitude<1.0f) fFortitude+=.1f;
	if (fFortitude>1.0f) fFortitude=1.0f;
	if (shootDelay) shootDelay--;
		// animate frames with frame delay
	if (++iFrameDelayed >= iFrameDelay) {
		iFrameDelayed = 0;
		iFrameNow++;
		if (iFrameNow>=iFrames) iFrameNow=0;
	}

	if (!(dwStatus & STATUS_BOSS)) {
		//Move non-boss at its given velocity
		x += xVel*fFortitude;// (xVel<0) ? (xVel - z + stunnedness) : (xVel + z + stunnedness);
		y+=yVel;
	}
	else {//Boss's speed not affected by stunnedness, but is by double speed status
		if (dwStatus & STATUS_DOUBLESPEED) x+=xVel*2;
		else x+=xVel;
	}




	// FIX POSITION //
	Translate3D();
	// right wrapping omitted because aliens always move left

	//if (rectRendered.right<0)
	//	x=int(SCREEN_WIDTH+width/2*scale);

	if (z<=0) {
		zVel=-zVel;
	}
	else if (rectRendered.top<=0) {
		zVel=-zVel;
	}

	//x=320;y=240;scale=1;//debug only

	// RESET RECT after fixing position //
	//scale = z/4.0f;
	//Translate3D();//update again after avoiding edges of screen

	// DRAW //
	if ( rectRendered.right>=0
		&& rectRendered.left<SCREEN_WIDTH
		&& y>-4.5) redraw();//redraw(rectRendered, scale);

	// AI ACTIONS //
	if (dwStatus & STATUS_SHOOTER)
		shoot();
	else if (dwStatus & STATUS_BOMBER)
		bomb();

	// HITDETECT //
	hitDetect();
	// CHECK LIFE //
	if (iHP<0) //died, stop shooting etc.
		dwStatus&=(STATUS_ALIVE|STATUS_SHOOTER|STATUS_BOMBER)^0xFFFFFFFF;

	if (!(dwStatus & STATUS_ALIVE))
	{
		if (!explodedBy)
		{
			iPlayExplosion=x;
			////PlaySound((const char*)SOUND_ID_EXPLOSION/*"explosion.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
			exploding=1;
		}
		explodedBy++;
	}
}
void Alien::redraw() {//register RECT &rectRendered, register float &scale)
	//USES ALPHALOOK//TargaToTargaAlphaFX(frames[iFrameNow], lptargaScreen, rectRendered.left, rectRendered.top, 255-stunnedness, explodedBy, dwStatus);
	
	//static int animation_seq[4] = {0,1,0,2};
	//if (z==4) TargaToTarga32_FX(frames[iFrameNow], lptargaScreen, SCREEN_OFFSET_X+rectRendered.left, SCREEN_OFFSET_Y+rectRendered.top, 255-stunnedness, explodedBy, dwStatus);
	//else
		//TargaToTarga32(frames[iFrameNow],lptargaScreen,SCREEN_OFFSET_X+rectRendered.left,SCREEN_OFFSET_Y+rectRendered.top);//debug only
		//TargaToTarga32(frames[iFrameNow],lptargaScreen,SCREEN_OFFSET_X+rectRendered.left,SCREEN_OFFSET_Y+rectRendered.top);//debug only
		TargaToTarga32_FX_Scaled(frames[iFrameNow], lptargaScreen, SCREEN_OFFSET_X+rectRendered.left, SCREEN_OFFSET_Y+rectRendered.top, 255*fFortitude, explodedBy, dwStatus, scale);
		//TargaToTarga32_FX_Scaled(frames[iFrameNow], lptargaScreen, SCREEN_OFFSET_X+rectRendered.left, SCREEN_OFFSET_Y+(363-(4-z)*52), 255-stunnedness, explodedBy, dwStatus, z/4.00);
}
void Alien::hitDetect() {
	//ranges are set to approximate meters
	//scale is NOT used since hit detection is pure 3D
	static float xMyRange=wMeters/2,
		yMyRange=dMeters/2,
		zMyRange=hMeters/2;
	for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
		if (shots[iShotNow]!=NULL) {//DISTANCE FORMULA: uses centerpoint (x,y,z)
			static float xOurRange=shots[iShotNow]->wMeters/2+xMyRange;
			static float yOurRange=shots[iShotNow]->wMeters/2+yMyRange;
			static float zOurRange=shots[iShotNow]->wMeters/2+zMyRange;
			if (abs(shots[iShotNow]->y-y) < yOurRange) {
				if (abs(shots[iShotNow]->x-x) < xOurRange) {
					if ( (abs(shots[iShotNow]->z-z) < zOurRange) && (!shots[iShotNow]->isAlien)) {
						GetHit(iShotNow);
					}
				}
			}
		}
	}
}
void Alien::GetHit(int iShotIndex) {
	if (dwStatus & STATUS_SHIELD) {
	//invincible with shield except boss
		if (dwStatus & STATUS_BOSS) dwStatus ^= STATUS_SHIELD; //only Boss loses shield, other shields are removed when boss dies
		//stunnedness=10;
		iHP+=8;
		delete shots[iShotIndex];
		shots[iShotIndex]=NULL;
		iPlayShieldZap=x;
		//PlaySound((const char*)SOUND_ID_SHIELDZAP/*"shieldzap.wav"*//*"shieldzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
	}
	else {
		fFortitude=.1;//stunnedness=10;
		iHP-=16;
		delete shots[iShotIndex];
		shots[iShotIndex]=NULL;
		iPlayOuchAlien=x;
		//PlaySound((const char*)SOUND_ID_OUCHALIEN/*"ouchalien.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
	}
}
void Alien::shoot(){
	if (!shootDelay) {
		shootDelay=rand()%30+10;
		float xEye = x-wMeters/2.0f;
		float shotSpeed=0.0333;//this per frame results in 1 m/s
		for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
			if (shots[iShotNow] == NULL) {
			//create a shot at the first available shot pointer
				iPlayLaserAlien=x;
				//Assume shoot left
				//Prototype: Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isRed2)
				shots[iShotNow] = new Shot(xEye, y, z, -shotSpeed, 0, 0, 1, 0);
				break;
			}
		}
	}
}
void Alien::bomb(){
	if (!shootDelay)
	{
		shootDelay=rand()%10+10;
		//float scale=z/4.0f;
		float yFrom=y-0.1f;
		int shotSpeed=8;
		for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
			if (shots[iShotNow] == NULL) {//create a shot at the first available shot pointer
				//Assume shoot left
				iPlayBomb=x;
				//Prototype: Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isAlien2 , int isBomb2)
				shots[iShotNow] = new Shot(x, yFrom, z, -shotSpeed, .2, -.1, 1, 1);
				break;
			}
		}
	}

}

// GLOBAL TO HERO /////////////////////////////////
Alien				*aliens[MAXALIENS];
///////////////////////////////////////////////////

//Hero Class
class Hero {
public:
	float fBaseScale;
	LPTARGA frames[21]; // 2 frames of animation for cycle
	float x,y,z; //z location 1 to 4 (2 to 4 is usage for game) mult by .25 for scale
	int moveDelay; //delays motion of z
    int aimDelay;
	float fAimZ; //multiplier for aim z velocity, could theoretically be negative and be okay
	int shootDelay; //
	int width, height;
	float wMeters, hMeters, dMeters;//size in meters
	DWORD dwStatus;
	int iHP;
	int stunnedness;
	int explodedBy;
	int iFrameNow; //used to calculate current rotation as well as frame number
	int direction; //-1=left 0=turning 1=right
	int rotationDir; //rotation: neg=left 0=done pos=right
	int xVel; //x velocity
	int yVel; 
	int zVel; //jumping upward velocity
	float gunPower; //gun power in percent, max=1
	int exploding;
	RECT rectRendered;
	float scale;

	Hero();
	void refresh();
	void turn(int dir);
	void jump();
	void move(int xDir, int yDir);
	void aim(int dir);
	void shoot();
	void doublespeed();
	void precisionaim();
	void rapidfire();
	void drawTarget(int alienNum);
	void redraw();
	void Translate3D();
private:
	void hitDetect();
	void drawMeters();
};
Hero::Hero() {
    fBaseScale=1.0f;
	exploding=0;
	for (int iFrame=0;iFrame<=20;iFrame++) {
		frames[iFrame]=lptargaHero[iFrame];
	}
	width=lptargaHero[0]->width;
	height=lptargaHero[0]->height;
	x=0;//(SCREEN_WIDTH/2 - width/2)-21;
	y=0;//(SCREEN_HEIGHT*.75 -height/2)-90;//y = SCREEN_HEIGHT*.75 - height/2;
	z=4; //starts falling from air
	wMeters=.2f;
	hMeters=1.6f;
	dMeters=.2f;
	fAimZ=0.0f;
	moveDelay = 0;
	aimDelay = 0;
	shootDelay = 0;
	dwStatus = STATUS_ALIVE | STATUS_PRECISIONAIM;// | STATUS_ANTIGRAVITY;
	iHP = 255;
	stunnedness = 10;
	explodedBy = 0;
	iFrameNow=9;
	direction=0;
	rotationDir=1;
	zVel = 0;
	xVel = 0;
	yVel = 0;
	gunPower = 1;
}
void Hero::turn(int dir) {
	//if (z==0)moveX(dir); //move horizontally
	if (dir==-1 || dir==1 || dir==2) { //else ignore bad arguments
		if (direction) { //if not turning
			if (dir==direction) {

			}
			else {
				direction=0; //set character in state of motion
				if (dir>direction) rotationDir=1;
				else if (dir<direction)	rotationDir=-1;
			}
		}
		else {//moving, so see if direction in argument is different than rotation direction
			//if (dir == 2 && (iFrameNow<=0 || iFrameNow>=20)) rotationDir = (iFrameNow<=0) ? -1 : 1;
			if (dir != rotationDir) rotationDir= -rotationDir;
		}
	}
}
void Hero::jump() {
	if (!moveDelay) {
		if ((z==0.0f) || (dwStatus & STATUS_ANTIGRAVITY)) {//don't unless not jumping or if ANTIGRAVITY status
			zVel=40.0f;//debug only, uncomment next line
			//zVel=(dwStatus & STATUS_ANTIGRAVITY) ? .07f :.42f;
			if (dwStatus & STATUS_DOUBLESPEED) zVel *= 2.0f;
			if (!(dwStatus & STATUS_ANTIGRAVITY)) //if no antigravity, delay movement in air
				moveDelay= (dwStatus & STATUS_DOUBLESPEED) ? 15 : 30;
		}
	}
}
void Hero::move(int xDir, int yDir) { //for up and down movement
	static float factor=.2f;
	if (!moveDelay) {
		if (dwStatus&STATUS_DOUBLESPEED) factor=.4f;
		if (yDir>0) {//move up (back, +y)
			if (y<4.0f) {
				y+=factor;
				yVel=factor*yDir;
			}
		}
		else if (yDir<0) {//move down (forward, -y)
			if (y>-4.0f) {
				y-=factor;
				yVel=factor*yDir;
			}
		}
		if (xDir<0) {//move left, -x
			if (x>-6.0f) {
				x-=factor;
				xVel=factor*xDir;
			}
		}
		else if (xDir>0) {//move right +x
			if (x<6.0f) {
				x+=factor;
				xVel=factor*xDir;
			}
		}
	}
}
void Hero::aim(int dir) {//for up and down aim
	if (!aimDelay) {
		if (dir<0 && fAimZ>AIM_MIN) fAimZ-=AIM_RESOLUTION;
		else if (fAimZ<AIM_MAX) fAimZ+=AIM_RESOLUTION;
		aimDelay = 1;
	}
}
void Hero::shoot() {
	if ((!shootDelay) && gunPower>=.45) {
		if (dwStatus&STATUS_RAPIDFIRE) {
			shootDelay=2;
		}
		else {
			gunPower-=.45;
			shootDelay=10;
		}
		float zEye = z+hMeters/2;
		float shotSpeed=0.03333;//meters per frame (1ms~.03333)
		float fTurnedness=((float)iFrameNow-10.0f)/10.0f;
		float fForwardness=(fTurnedness>0) ? 1.0f-fTurnedness : 1.0f+fTurnedness;
		float xEye = x+scale*fForwardness/100.0f; //position of hero's eye to shoot blast from
		for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
			if (shots[iShotNow] == NULL) {//create a shot at the first available shot pointer
				if (direction==-1) //shoot left
					//Prototype: Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isRed2)
					shots[iShotNow] = new Shot(xEye, y, zEye, direction*shotSpeed, 0, 0, 0, 0);
				else if (direction==1) //shoot right
					shots[iShotNow] = new Shot(xEye, y, zEye, direction*shotSpeed, 0, 0, 0, 0);
				else //shoot downward
					shots[iShotNow] = new Shot(xEye, y, zEye, shotSpeed*fTurnedness, shotSpeed*fForwardness, fAimZ, 0, 0);
					//shots[iShotNow] = new Shot(eyePosX-20*scale+scale*iFrameNow, eyePosY, z, ((iFrameNow-10)*6), fAimZ*20, shotSpeed, 0, 0);
					//new shot prototype: Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isAlien2, int isBomb2)
				iPlayLaser=x;
				break;
				//iChanLaser = Mix_PlayChannel(2, mcLaser, 0);//chan, sound, #loops
				//PlaySound((const char*)SOUND_ID_LASER/*"laser.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
				////PlaySound((const char*)SOUND_ID_LASER, hinstance_app, SND_RESOURCE | SND_ASYNC);
			}
		}
	}
}
void Hero::refresh() {
// ORDER: //
	//4. UPDATE+MOVE//
	//5. FIX POSITION// -get it ready for the next set rect and next render
	//1. SET RECT//
	//2. DRAW//
	//3. HITDETECT&CHECK LIFE// -this way it is using the rectRendered for hit detection

	// UPDATE //
	drawMeters();
	if (moveDelay) moveDelay--;
	if (aimDelay) aimDelay--;
	if (shootDelay) shootDelay--;
	if (stunnedness) stunnedness--;
	//Animate if not turned all the way
	if (!direction) {
		iFrameNow+=rotationDir * ((dwStatus & STATUS_DOUBLESPEED) ? 2 : 1);
		if (iFrameNow<0) {
			iFrameNow=0;
			rotationDir=0;
			direction=-1;
		}
		else if (iFrameNow>20) {
			iFrameNow=20;
			rotationDir=0;
			direction=1;
		}
	}
	if ((dwStatus & STATUS_PRECISIONAIM) && direction==0) direction = 2;
	//re-charge gun
	if (gunPower<1) {
		gunPower+= (dwStatus & STATUS_DOUBLESPEED) ? .066 : .033;
		if (gunPower>1) gunPower=1;
	}
	// MOVE //
	//(Calculate PHYSICS)
	//about 80 pixels per meter (actual hero is 150 pixels high inside the Targa)
	//30 frames per second
	//gravitational acceleration is 1.8 m/s/s
	z+=zVel;
	zVel-=.8/30.0f; //only .8m/s/s gravitational acceleration (.8/30perframe), for high jumping
	if (z<=0) {//hit ground
		zVel=0;
		z=0;
		xVel=0;
		yVel=0;
	}
	if (z>0) {
		x+=xVel;
		y+=yVel;
	}

	Translate3D();
	//scale=z/4.00;

	// FIX POSITION //
	if (rectRendered.top<=0) {
		zVel=0;
	}

	//will be rendered according to scale and this rect:
	Translate3D();
	redraw();
	//drawTarget;
	//if (boolShowTarget) drawTarget();

	// HITDETECT //
	if (!stunnedness) hitDetect();
	// CHECK LIFE //
	if (iHP<=0) dwStatus = 0; //died, stop other special abilities too
	if (!(dwStatus & STATUS_ALIVE)) {
		explodedBy++;
		if (!exploding) {
			iPlayExplosion=x;
			//PlaySound((const char*)SOUND_ID_EXPLOSION/*"explosion.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
			exploding=1;
		}
	}
}
void Hero::redraw() {
	//USES ALPHALOOK//TargaToTargaAlphaFX(frames[iFrameNow], lptargaScreen, x, y, 255-stunnedness, explodedBy, dwStatus);
	//if (z==4) TargaToTarga32_FX(frames[iFrameNow], lptargaScreen, SCREEN_OFFSET_X+x, SCREEN_OFFSET_Y+y, 255-stunnedness, explodedBy, dwStatus);
	//else
	TargaToTarga32_FX_Scaled(frames[iFrameNow], lptargaScreen, SCREEN_OFFSET_X+rectRendered.left, SCREEN_OFFSET_Y+rectRendered.top, 255-stunnedness, explodedBy, dwStatus, scale);
}
void Hero::Translate3D() {
	//ratios
	static float
		fScaleFarNearWidthRatio = 4.96460,//561.0f/113.0f;//calculated by hand from a backdrop
		fNearScale=APPROACH(561.0f,113.0f,.5f)/561.0f;//ratio of middle to near//1.25;//scale at yNear
	//screen pixel reference lines
	static int
		yZero=SCREEN_HEIGHT*0.616667,
		xZero=SCREEN_WIDTH/2,
		xFocalPoint2D=SCREEN_HEIGHT/2,
		yFocalPoint2D=SCREEN_HEIGHT*0.29375,
		yBelowFocus2D=SCREEN_HEIGHT-yFocalPoint2D,
		yAboveFocus2D=SCREEN_HEIGHT-yBelowFocus2D,
		xNearExtreme=SCREEN_WIDTH*fNearScale,//virtual pixel width of range, at near y
        fHalfScreenW=SCREEN_WIDTH/2,
        xNearScreenW=fNearScale*SCREEN_WIDTH;//width of grid is beyond screen width at near y
	//3D reference lines at parts of screen
	static float
		zTop=9, //top of screen when y==-4
		zBottom=0,
		zFocalPoint3D=4.5,
		yFar=4,//a minimum movement range for the game
		yNear=-4, //bottom of screen when z==0, top when z==zTop
		yRange=8,//range from far to near
		xEdge=6;//edge of screen when y==0

	//keep in mind that positive y is farther away
	//--negative y is closer


	//translate 2d coords to 3d
	int xTranslate, yTranslate;
	//float zFocused=z-zFocalPoint3D;
	float yFromFar=-(y-yFar);
	float yNearness=yFromFar/yRange;
	float yNearnessExp=(yFromFar*yFromFar)/(yRange*yRange);
	yTranslate = (z<zFocalPoint3D)//(zFocused<0)
		? (yFocalPoint2D+yNearnessExp*yBelowFocus2D)
		: (yFocalPoint2D-yNearnessExp*yAboveFocus2D);
	//xTranslate = (y>=0)  //remember +y is farther
	//	? APPROACH(x/xEdge*fHalfScreenW+fHalfScreenW, xFocalPoint2D, yNearnessExp)
	//	: (x/xEdge*fHalfScreenW*(yNearNessExp+))+fHalfScreenW
	xTranslate=APPROACH(x/xEdge*(fNearScale*fHalfScreenW)+fHalfScreenW,xFocalPoint2D,yNearnessExp);
	scale=fNearScale*(yFromFar/yRange);//SET SCALE
	scale*=fBaseScale;
	if (scale<.05) scale=.05;//prevents zero size after using scale
	rectRendered.left=xTranslate-width/2*scale;
	rectRendered.top=yTranslate-height/2*scale;
	rectRendered.right=rectRendered.left+scale*width;
	rectRendered.bottom=rectRendered.top+scale*height;
}
void Hero::drawMeters() {

	//register DWORD *lpdwDest = 
	register BYTE *byteBuffer = lptargaScreen->buffer;
	byteBuffer+=SCREEN_OFFSET_Y*lptargaScreen->width*4+SCREEN_OFFSET_X*4;
	static int iStride=TargaStride(lptargaScreen);
	//register DWORD pixel;// = _RGB32BIT(255,255,255,255); //remember this MACRO is ARGB unlike actual screen
	register int toX=2, toY=SCREEN_HEIGHT-3;
	for (int iLevel=0; iLevel<=gunPower*100; iLevel++)
	{
		//pixel=_RGB32BIT(255,55+iX*2,55+iX*2,155+iX); 
		for (register int offset=0; offset<=13; offset++)
		{
			//lpdwDest[toX + ((toY)*iStride >> 2)] = pixel;
			//lpdwDest[int(iX*100)] = pixel;
			for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
				byteBuffer[(toX + offset)*REAL_BYTEDEPTH + (toY-iLevel)*iStride + byteNow] = 255-130+offset*10-iLevel;
			
		}
	}

	toX=18;
	toY=SCREEN_HEIGHT-2;
/*
	pixel=_RGB32BIT(255,255,BYTE(iHP*.85),0);
	for (iX=0; iX<iHP; iX++)
	{
		lpdwDest[toX + iX + ((toY)*iStride >> 2)] = pixel;
		//for (byteNow
	}
*/

	for (int iLevel=0; iLevel<iHP; iLevel++) {
		for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
			byteBuffer[(toX+iLevel)*REAL_BYTEDEPTH + toY*iStride + byteNow] = (byteNow==3) ? 255 : (byteNow==2) ? 255 : iHP;
	}
}
void Hero::drawTarget(int alienNum) {
	//for (int alienNow = 0; alienNow < MAXALIENS; alienNow++)
	//{
		//if (aliens[alienNum] != NULL)
		int iX=alienNum;
		if (aliens[iX]==NULL) return;

		static float zOurRange=hMeters/2+aliens[iX]->hMeters/2;
		//I switched the right and left edges on purpose to be more inclusive
		//since screen offset prevents bad writes
		if (aliens[iX]->z-z<zOurRange && aliens[iX]->rectRendered.right>0 && aliens[iX]->rectRendered.left<SCREEN_WIDTH ) {
			//register DWORD *lpdwDest = 
			register BYTE *byteBuffer = lptargaScreen->buffer;
			static int iStride=TargaStride(lptargaScreen);
			//register DWORD pixel;// = _RGB32BIT(255,255,255,255); //remember this MACRO is ARGB unlike actual screen
			register int toX=aliens[iX]->rectRendered.left, toY=aliens[iX]->rectRendered.top;
			toX+=SCREEN_OFFSET_X;
			toY+=SCREEN_OFFSET_Y;
			for (int iX=0; iX<7; iX++) {
				for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++) {
				//draw crosshairs
					byteBuffer[(toX+iX)*REAL_BYTEDEPTH + (toY+3)*iStride + byteNow] = (byteNow==1)? 255 : 0; //across
					byteBuffer[(toX+3)*REAL_BYTEDEPTH + (toY+iX)*iStride + byteNow] = (byteNow==1)? 255 : 0; //down
				}
			}
			
/*			toX=aliens[iX]->x;
			toY=aliens[iX]->y-2;
			for (iX=0; iX<5; iX++)
			{
	
				for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
					byteBuffer[(toX)*REAL_BYTEDEPTH + (toY+iX)*iStride + byteNow] = (byteNow==1)? 255 : 0;
			}
*/			

		}
	//}
}
void Hero::hitDetect() {
	//ranges are set to approximate meters
	//scale is NOT used since hit detection is pure 3D
	static float xMyRange=wMeters/2,
		yMyRange=dMeters/2,
		zMyRange=hMeters/2;
	//HIT ALIEN:
	for (int iAlien=0; iAlien<MAXALIENS; iAlien++) {
		if (aliens[iAlien] != NULL) {
			static float xOurRange=aliens[iAlien]->wMeters/2+xMyRange;
			static float yOurRange=aliens[iAlien]->dMeters/2+yMyRange;
			static float zOurRange=aliens[iAlien]->hMeters/2+zMyRange;
		//CENTERPOINT DISTANCE-BASED
			if ( abs(aliens[iAlien]->x-x) < xOurRange) {
				if ( abs(aliens[iAlien]->y-y) < yOurRange) {
					if (aliens[iAlien]->z-z < zOurRange) {
						if (dwStatus & STATUS_SHIELD) {
							dwStatus ^= STATUS_SHIELD;
							iPlayShieldZap=x;
							//PlaySound((const char*)SOUND_ID_SHIELDZAP/*"shieldzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
							stunnedness=10;
							iHP+=50;
						}
						else {
							iPlayOuchZap=x;
							//PlaySound((const char*)SOUND_ID_OUCHZAP/*"ouchzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
							stunnedness=10;
							iHP-=50;
						}
					}
				}
			}
		
		//RECT-BASED
		//	int widthMod = 80*scale;
		//	if (aliens[iAlien]->rightEdge()>x+widthMod+widthMod)
		//		if (aliens[iAlien]->leftEdge()<x+width*scale-widthMod)
		//			if (aliens[iAlien]->y>y)
		//				if (aliens[iAlien]->y<y+height*scale)
		//					if (aliens[iAlien]->z==z)
		//					{
		//						if (dwStatus & STATUS_SHIELD)
		//						{
		//							dwStatus ^= STATUS_SHIELD;
		//							iPlayShieldZap=x;
		//							//PlaySound((const char*)SOUND_ID_SHIELDZAP/*"shieldzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
		//							stunnedness=10;
		//							iHP+=50;
		//						}
		//						else
		//						{
		//							iPlayOuchZap=x;
		//							//PlaySound((const char*)SOUND_ID_OUCHZAP/*"ouchzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
		//							stunnedness=10;
		//							iHP-=50;
		//						}
		//					}
		}//end if non-null alien
	}//end for aliens
	//HIT BULLET: edit the shot::hitDetect to do this instead maybe
	for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
		if (shots[iShotNow] != NULL) {//DISTANCE FORMULA: uses centerpoint (x,y,z)
			static float xOurRange=shots[iShotNow]->wMeters/2+xMyRange;
			static float yOurRange=shots[iShotNow]->wMeters/2+yMyRange;
			static float zOurRange=shots[iShotNow]->wMeters/2+zMyRange;
			if (abs(shots[iShotNow]->x-x) < xOurRange) {
				if (abs(shots[iShotNow]->y-y) < yOurRange) {
					if (abs(shots[iShotNow]->z-z) < zOurRange && (shots[iShotNow]->isAlien)) {
						if (dwStatus & STATUS_SHIELD) {
							dwStatus ^= STATUS_SHIELD;
							stunnedness=10;
							iHP+=15;
							delete shots[iShotNow];
							shots[iShotNow]=NULL;
							iPlayShieldZap=x;
							//PlaySound((const char*)SOUND_ID_OUCHZAP/*"ouchzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
						}
						else {
							stunnedness=10;
							iHP-=15;
							delete shots[iShotNow];
							shots[iShotNow]=NULL;
							iPlayOuchZap=x;
							//PlaySound((const char*)SOUND_ID_OUCHZAP/*"ouchzap.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
						}
					}
				}
			}
		}//end if shot not null
	}//end for iShotNow
}
void Hero::doublespeed() {
	if (!(dwStatus & STATUS_DOUBLESPEED))
	{
	dwStatus |= STATUS_DOUBLESPEED;
			 bPlayTrumpet=true;
	//PlaySound((const char*)SOUND_ID_TRUMPET/*"trumpet.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
	}
}
void Hero::precisionaim() {
	if (!(dwStatus & STATUS_PRECISIONAIM))
	{
	dwStatus |= STATUS_PRECISIONAIM;
			 bPlayTrumpet=true;
	//PlaySound((const char*)SOUND_ID_TRUMPET/*"trumpet.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
	}
}
void Hero::rapidfire() {
	if (!(dwStatus & STATUS_RAPIDFIRE)) {
	dwStatus |= STATUS_RAPIDFIRE;
			 bPlayTrumpet=true;
	//PlaySound((const char*)SOUND_ID_TRUMPET/*"trumpet.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
	}
}

// GLOBAL CLASS POINTERS ////////////////////////////////////////////
Hero				*hero = NULL;


////////////////////////////////////////////////////////////////////////////////


bool bEvent=false;
/* The screen surface */
SDL_Surface *screen = NULL;
int xDir=0;
int yDir=0;
int audio_rate=44100;
Uint16 audio_format=AUDIO_S16; /* 16-bit stereo */
int audio_channels=2;
int audio_buffers=4096;
Mix_Music *music=NULL;
int iDirKeysDown=0;
int PickChan() {
	static int iPickChan=-1;
	iPickChan++;
	if (iPickChan>=8) iPickChan=0;
	return(iPickChan);
}
void MusicDone() { //runs when music is done
	//Mix_HaltMusic();
	//Mix_FreeMusic(music);
	//music = NULL;
}
inline void SetPan(int iChan, int iLocation) {
	 float fLoc=(float)iLocation/(float)SCREEN_WIDTH;
	 if (fLoc<0) fLoc=0;
	 else if (fLoc>1) fLoc=1;
	 Mix_SetPanning(iChan, L_FROM_FACTOR(fLoc), R_FROM_FACTOR(fLoc));
}
inline void UpdateThrustPan() {
	 SetPan(iChanThruster,hero->x);
	 //fThruster=(float)hero->x/(float)SCREEN_WIDTH;
	 //if (fThruster<0) fThruster=0;
	 //else if (fThruster>1) fThruster=1;
	 //Mix_SetPanning(iChanThruster, L_FROM_FACTOR(fThruster), R_FROM_FACTOR(fThruster));
}
void DirKeyDown() {
	 if ((hero!=NULL&&hero->z!=0.0f) && (hero->dwStatus & STATUS_ANTIGRAVITY)) {
		if (iDirKeysDown<=0) {
			iChanThruster = Mix_PlayChannel(-1, mcThruster, -1);//chan, sound, #loops
			//iReturn=Mix_FadeInChannelTimed(iChan, mcThruster,
			//			iLoops, iMillisecondsFadeIn, iTicksDuration)
		}
		UpdateThrustPan();
	 }
	 iDirKeysDown++;
}
void DirKeyUp() {
	 iDirKeysDown--;
	 if (iDirKeysDown<=0) {
		Mix_HaltChannel(iChanThruster);
		iChanThruster = -1;
		iDirKeysDown=0;
	 }
}
/* This function draws to the screen; replace this with your own code! */
static void draw ()
{

	static int xTest=2;
	static int yTest=2;
	static int direction = 0;
	static int value = 0;
	static int which = 0;
	//SDL_Rect rect;
	//Uint32 color;

	/* Create a black background */
	//color = SDL_MapRGB (screen->format, 0, 0, 0);
	//SDL_FillRect (screen, NULL, color);

	//rect.w = 1;
	//rect.h = 1;
	//rect.x = xTest;
	//rect.y = yTest;
	//color = SDL_MapRGB(screen->format, 255,255,255);
	//SDL_FillRect (screen, &rect, color);
	//now move point:
	xTest+=xDir;
	yTest+=yDir;
	//if (xTest>=screen->w) xDir*=-1;
	//else if (xTest<=0) xDir*=-1;
	//if (yTest>=screen->h) yDir*=-1;
	//else if (yTest<=0) yDir*=-1;


	/* Make sure everything is displayed on screen */
	SDL_Flip (screen);
	/* Don't run too fast */
	SDL_Delay (1);
}

int done=0;
SDL_Event event;
int main (int argc, char *argv[])
{
	GameInit();
	//main event loop:
	while (!done) {
			DWORD startTime=SDL_GetTicks();//GetTickCount();
		//Check for events
		bEvent=false;
		while (SDL_PollEvent (&event)) {
				bEvent=true;
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym==SDLK_a) {
					DirKeyDown();
					dwPressing|=GAMEKEY_LEFT;
					xDir=-1;
				}
				else if (event.key.keysym.sym==SDLK_d) {
					DirKeyDown();
					dwPressing|=GAMEKEY_RIGHT;
					xDir=1;
				}
				else if (event.key.keysym.sym==SDLK_w) {
					DirKeyDown();
					dwPressing|=GAMEKEY_UP;
					yDir=-1;
				}
				else if (event.key.keysym.sym==SDLK_s) {
					DirKeyDown();
					dwPressing|=GAMEKEY_DOWN;
					yDir=1;
				}
				else if (event.key.keysym.sym==SDLK_DELETE) {
					//laser keydown
					dwPressing|=GAMEKEY_FIRE;
				}
				else if (event.key.keysym.sym==SDLK_END) {
					dwPressing|=GAMEKEY_JUMP;
				}
				else if (event.key.keysym.sym==SDLK_ESCAPE) {
					dwPressing|=GAMEKEY_EXIT;
				}
				else if (event.key.keysym.sym==SDLK_2) {
					dwPressing|=GAMEKEY_DOUBLESPEED;
				}
				else if (event.key.keysym.sym==SDLK_p) {
					dwPressing|=GAMEKEY_PRECISIONAIM;
				}
				else if (event.key.keysym.sym==SDLK_r) {
					dwPressing|=GAMEKEY_RAPIDFIRE;
				}
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym==SDLK_a) {
					DirKeyUp();
					dwPressing&=(GAMEKEY_LEFT^0xFFFFFFFF);
					xDir=0;
				}
				else if (event.key.keysym.sym==SDLK_d) {
					DirKeyUp();
					dwPressing&=(GAMEKEY_RIGHT^0xFFFFFFFF);
					xDir=0;
				}
				else if (event.key.keysym.sym==SDLK_w) {
					DirKeyUp();
					dwPressing&=(GAMEKEY_UP^0xFFFFFFFF);
					yDir=0;
				}
				else if (event.key.keysym.sym==SDLK_s) {
					DirKeyUp();
					dwPressing&=(GAMEKEY_DOWN^0xFFFFFFFF);
					yDir=0;
				}
				else if (event.key.keysym.sym==SDLK_DELETE) {
					//iChanLaser = Mix_PlayChannel(2, mcLaser, 0);//chan, sound, #loops
					dwPressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_END) {
					dwPressing&=(GAMEKEY_JUMP^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_ESCAPE) {
					dwPressing&=(GAMEKEY_EXIT^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_2) {
					dwPressing&=GAMEKEY_DOUBLESPEED^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_p) {
					dwPressing&=GAMEKEY_PRECISIONAIM^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_r) {
					dwPressing&=GAMEKEY_RAPIDFIRE^0xFFFFFFFF;
				}
				break;
			case SDL_QUIT:
				if (!done) {
					gameState=GAME_STATE_SHUTDOWN;//makes GameMain do nothing
					done=1;
					GameShutdown();
				}
				break;
			default:
				break;
			}
		}//end while checking keyboard event
		
		if (!done) {
			while((SDL_GetTicks() - startTime) < 28); //lock to approx 30fps
			GameMain();
					//only do one at a time, so use else if
			if (bPlayTrumpet) {
				bPlayTrumpet=false;
				iChanTrumpet = Mix_PlayChannel(PickChan(), mcTrumpet, 0); //chan, sound, #loops
			}
			else if (iPlayLaserAlien>-1) {
				iChanLaserAlien=Mix_PlayChannel(PickChan(), mcLaserAlien, 0); //chan, sound, #loops
				SetPan(iChanLaserAlien, iPlayLaserAlien);
				iPlayLaserAlien=-1;
			}
			else if (iPlayLaser>-1) {
				iChanLaser=Mix_PlayChannel(PickChan(), mcLaser, 0); //chan, sound, #loops
				SetPan(iChanLaser, iPlayLaser);
				iPlayLaser=-1;
			}
			else if (iPlayExplosion>-1) {
				iChanExplosion=Mix_PlayChannel(PickChan(), mcExplosion, 0); //chan, sound, #loops
				SetPan(iChanExplosion, iPlayExplosion);
				iPlayExplosion=-1;
			}
			else if (iPlayOuchAlien>-1) {
				iChanOuchAlien=Mix_PlayChannel(PickChan(), mcOuchAlien, 0); //chan, sound, #loops
				SetPan(iChanOuchAlien, iPlayOuchAlien);
				iPlayOuchAlien=-1;
			}
			else if (iPlayOuchZap>-1) {
				iChanOuchZap=Mix_PlayChannel(PickChan(), mcOuchZap, 0); //chan, sound, #loops
				SetPan(iChanOuchZap, iPlayOuchZap);
				iPlayOuchZap=-1;
			}
			else if (iPlayShieldZap>-1) {
				iChanShieldZap=Mix_PlayChannel(PickChan(), mcShieldZap, 0); //chan, sound, #loops
				SetPan(iChanShieldZap, iPlayShieldZap);
				iPlayShieldZap=-1;
			}
		}//end if not done do normal stuff
	}//end while not done continue main event loop
	try {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music=NULL;
		Mix_CloseAudio();
	}
	catch (char* sExn) {
		try {
			error_txt<<"Exception closing audio device: "<<sExn;
		}
		catch (char* sExn) {
		}
	}
	return 0;
}
// FUNCTIONS ////////////////////////////////////////////////
int TargaStride(LPTARGA image) {
	if (image==NULL) return 0;
	return image->width*(image->byBitDepth/8);
}

void TargaToScreen_AutoCrop(LPTARGA image) {
	 BYTE *lpbyNow=image->buffer;
	 static int iScreenCorner=SCREEN_OFFSET_Y*lptargaScreen->width*4+SCREEN_OFFSET_X*4;
	 lpbyNow+=iScreenCorner;
	 SDL_LockSurface(screen);
	 int iStrideScreen=screen->pitch;
	 static int iStrideMini=4*SCREEN_WIDTH;
	 int iStrideBig=4*image->width;
	 if (screen->format->BytesPerPixel==4) {
		BYTE* buffer=(BYTE*)screen->pixels;
		for (int iLine=0; iLine<SCREEN_HEIGHT; iLine++) {
			memcpy(buffer,lpbyNow,iStrideMini);
			lpbyNow+=iStrideBig;
			buffer+=iStrideScreen;
		}
		SDL_UnlockSurface(screen);
	 }
	 else {
		SDL_UnlockSurface(screen);
		//debug SLOW
		SDL_Rect rect;
		Uint32 color;
		rect.w=1;
		rect.h=1;
		rect.x=0;
		rect.y=0;
		BYTE *lpbyNow=image->buffer;
		static int iScreenCorner=SCREEN_OFFSET_Y*lptargaScreen->width*4+SCREEN_OFFSET_X*4;
		lpbyNow+=iScreenCorner;
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
		static int iScreenCorner=SCREEN_OFFSET_Y*lptargaScreen->width*4+SCREEN_OFFSET_X*4;

		lpbyNow+=iScreenCorner;
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

int TargaToTarga32_Alpha(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY) {
	// copy the Targa image to the primary buffer line by line
	//	direct draw surface description 
	// set size of the structure

	register DWORD *lpdwDest = (DWORD *)lptargaThis1->buffer;
	int iStride=TargaStride(lptargaThis1);

	register BYTE *dPtr = (BYTE*)&lpdwDest[toX + (toY*iStride >> 2)]; //dest pointer
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
			
			//Get BGR values and alpha
			//BYTE blue	= (image->buffer[index_y*image->width*4 + index_x*4 + 0]),
			//	green = (image->buffer[index_y*image->width*4 + index_x*4 + 1]),
			//	red	 = (image->buffer[index_y*image->width*4 + index_x*4 + 2]),
			//	alpha = (image->buffer[index_y*image->width*4 + index_x*4 + 3]);
			
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
				//if (index_x+toX<SCREEN_WIDTH)
				//	if (index_x+toX>=0)
				//		if (index_y+toY>=0)
				//			if (index_y+toY<SCREEN_HEIGHT)
								lpdwDest[toX + index_x + ((index_y+toY)*iStride >> 2)] = pixel;

			}
			else if (alpha==255)
			{
				dPtr+=4;
				pixel = _RGB32BIT(256,red,green,blue);
				//Manual clip and set pixel:
				//if (index_x+toX<SCREEN_WIDTH)
				//	if (index_x+toX>=0)
				//		if (index_y+toY>=0)
				//			if (index_y+toY<SCREEN_HEIGHT)
								lpdwDest[toX + index_x + ((index_y+toY)*iStride >> 2)] = pixel;
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
				//if (index_x+toX<SCREEN_WIDTH)
				//	if (index_x+toX>=0)
				//		if (index_y+toY>=0)
				//			if (index_y+toY<SCREEN_HEIGHT)
								lpdwDest[toX + index_x + ((index_y+toY)*iStride >> 2)] = pixel;
			}

		} // end for index_x
		dPtr+=iStride-image->width*4;
	} // end for index_y

	return(1);
} // end TargaToTarga32_Alpha

///////////////////////////////////////////////////////////////
int IRand() {
	static int iFibbo=1;
	static int iFibboPrev=0;
	int iReturn=iFibbo;
	int iFibboTemp=iFibbo;
	iFibbo=iFibboTemp+iFibboPrev;
	iFibboPrev=iFibboTemp;
	return iReturn;
}

float FRand() {
	static float fFibbo=1;
	static float fFibboPrev=0;
	float fReturn=fFibbo;
	float fFibboTemp=fFibbo;
	fFibbo=fFibboTemp+fFibboPrev;
	fFibboPrev=fFibboTemp;
	return fReturn;
}

float FRand(float fMin, float fMax) {
	static float fFibbo=1;
	static float fFibboPrev=0;
	float fReturn=fFibbo;
	float fFibboTemp=fFibbo;
	if (fFibboTemp+fFibboPrev+1>=(65534.0f)) {//prevent overflow
		fFibboTemp=1;
		fFibbo=1;
		fFibboPrev=0;
	}
	fFibbo=fFibboTemp+fFibboPrev;
	fFibboPrev=fFibboTemp;
	if (fMax<=fMin) fMax=fMin+1.0f;//prevent crashes
	//srand(SDL_GetTicks());
	float fRange=fMax-fMin;
	//fReturn=(int)(fReturn)%(int)(fRange);
	//while (fReturn>fRange) fReturn--;
	//fReturn+=fMin;
	return (float)((int)(fReturn)%(int)(fRange))+fMin;
	
	//return (rand()%1)?fMin:fMax;//debug re-implement this
}
///////////////////////////////////////////////////////////////
bool bQuickEdge=false;
int TargaToTarga32_FX(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat)
{
	//if (explodedness>7) explodedness=7;
	register DWORD *lpdwDest = (DWORD *)lptargaThis1->buffer;
	int iStride=TargaStride(lptargaThis1);
	register BYTE *dPtr = (BYTE*)&lpdwDest[toX + (toY*iStride >> 2)];
	register BYTE *sPtr = image->buffer;
	register BYTE blue,green,red,alpha;
	register DWORD pixel;
	register int xOffset;
	register int yOffset;
	register int explod1 = explodedness; //save original


	//if (explodedness)
	//{//this keeps the exploded object red but solid until after frame 3
	//	if (explodedness<=3)
	//		explodedness=0;
	//	else
	//		explodedness-=3;
	//}

	if (explodedness) explodedness--; //start explosion just as a red object, not blown up yet at 1
	 float fExplode=(float)explodedness/7.0f;
	 if (fExplode>1.0f) fExplode=1.0f;
	 register int iHalfHeight=image->height/2;
	 register int iHalfWidth=image->height/2;
	 register int yMaxEx,xMaxEx;
	 float fRemains=1.0f-fExplode;
	 float fExplodeLog=sqrt(fExplode);
	 float fRemainsExp=fRemains*fRemains;
	 float fRemainsLog=(fRemains*fRemains)/fRemains;
	for (register int index_y = 0; index_y < image->height; index_y++)
	{
		//if (index_y%2) //splits object into squares
		//	toY+=explodedness;
		yOffset = toY;
		yMaxEx=(index_y<iHalfHeight)?iHalfHeight-index_y:index_y-iHalfHeight;

		for (register int index_x = 0; index_x < image->width; index_x++)
		{
			xMaxEx=(index_x<iHalfWidth)?iHalfWidth-index_x:index_x-iHalfWidth;
			//if (index_x%2) //splits object into squares
			//	toX+=explodedness;
			xOffset = toX;

			blue=*sPtr++;
			green=*sPtr++;
			red=*sPtr++;
			alpha=*sPtr++;
			if (explod1 || 255-opacityCap) //this use of opacity cap is unique to DXMan, makes it red when transparent
			{
				green>>=1;
				blue>>=2;
			}
			
			if (explodedness) {
								//uses -= for implosion
				 if (index_x<iHalfWidth) xOffset-=(int)(FRand(-xMaxEx,0)*fExplodeLog);
				 else xOffset-=(int)(FRand(0,xMaxEx)*fExplodeLog);
				 if (index_y<iHalfHeight) yOffset=toY+(int)(FRand(-yMaxEx,0)*fExplodeLog);
				 else yOffset=toY+(int)(FRand(0,yMaxEx)*fExplodeLog);
				 green*=fRemainsExp;
				 red*=fRemainsExp;
				 green*=fRemainsExp;
				 alpha*=fRemains;
			}


			if (alpha>opacityCap) alpha=opacityCap; //"Cap" opacity off at opacityCap to create transparency if not 255
			if (bQuickEdge) {
				if (alpha<85)
				{
					dPtr+=4;
				}
				else if (alpha>171)
				{	dPtr+=4;
					
					//register int scaledX = (scale==1) ? index_x : index_x*scale,
					//			 scaledY = (scale==1) ? index_y : index_y*scale;
					//Manual clip	and set pixel:
					//if (index_x + toX<SCREEN_WIDTH)
					//	if (index_x + toX>=0)
						//	if (index_y + toY>=0)
								//if (index_y + toY<SCREEN_HEIGHT)
								//{
									pixel = _RGB32BIT(256,red,green,blue);
									lpdwDest[index_x + xOffset + ((index_y + yOffset)*iStride >> 2)] = pixel;
								//}
				}
				else
				{//blend
					if ((dwStat & STATUS_SHIELD)&&alpha<172)
					{//blue outline
						blue = 192;
						green = 64;
					}
					blue = alphaLook[blue][*dPtr][alpha];
					dPtr++;
					green = alphaLook[green][*dPtr][alpha];
					dPtr++;
					red = alphaLook[red][*dPtr][alpha];
					dPtr+=2;
					
	
					//register int scaledX = (scale==1) ? index_x : index_x*scale,
					//			 scaledY = (scale==1) ? index_y : index_y*scale;
					//Manual clip and set pixel:
					//if (index_x + toX<SCREEN_WIDTH)
					//	if (index_x + toX>=0)
						//	if (index_y + toY>=0)
							//	if (index_y + toY<SCREEN_HEIGHT)
								//{
									pixel = _RGB32BIT(256,red,green,blue);
									lpdwDest[index_x + xOffset + ((index_y + yOffset)*iStride >> 2)] = pixel;
								//}
				}//end else solid
			}//end if quickedge
			else {
					if ((dwStat & STATUS_SHIELD)&&alpha<172)
					{//blue outline
						blue = 192;
						green = 64;
					}
					blue = alphaLook[blue][*dPtr][alpha];
					dPtr++;
					green = alphaLook[green][*dPtr][alpha];
					dPtr++;
					red = alphaLook[red][*dPtr][alpha];
					dPtr+=2;
					pixel = _RGB32BIT(255,red,green,blue);
					lpdwDest[index_x + xOffset + ((index_y + yOffset)*iStride >> 2)] = pixel;
			}//end else not bQuickEdge
		}
		dPtr+=iStride-image->width*4;
	}
	return(1);
} // end TargaToTarga32_FX

////////////////////////////////////////////////////////////////////////////////

int TargaToTarga32_FX_Scaled(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat, float scale) {
	register DWORD* lpdwDest = (DWORD*)lptargaThis1->buffer;
	register DWORD* lpdwSrc = (DWORD*)image->buffer;
	register int iStride=TargaStride(lptargaThis1);
	register int iDestW=lptargaThis1->width;
	register BYTE* dPtr = (BYTE*)&lpdwDest[toX + (toY*iDestW)];
	register BYTE* sPtr;// = image->buffer;
	//memcpy(dPtr,lpdwDest,lptargaThis1->width*4);//debug only
	register BYTE blue,green,red,alpha;
	register DWORD pixel;
	//register int tox1 = toX; //save original
	register int xOffset;
	register int yOffset;
	register int explod1 = explodedness; //save original
	register int iScale=(int)(scale+0.5f);
	register int iDestNow=0;
	if (explodedness) explodedness--; //start explosion just as a red dude, not blown up yet at frame 1
	register float fExplode=((float)explodedness/FMAX_EXPLODEDNESS);
	if (fExplode>1.0f) fExplode=1.0f;
	register float fExplodeExp=fExplode*fExplode;
	register float fExplodeLog=sqrt(fExplode);
	register float fRemains=(1.0f-fExplode);
	register float fRemainsExp=fRemains*fRemains;
	//exp example: .75 = 0.5625
	//log example: .75 = 
	register int iScaledW=(int)((float)image->width*scale);
	register int iScaledH=(int)((float)image->height*scale);
	register int iSrcStride=image->width*4;
	//register float fRemainsExp=fRemains*fRemains;//exponential decay
	//register int iSubStride=iStride-(iScale*4);//only for scale>1 subloop
	register int yMaxEx, xMaxEx, iHalfHeight=iScaledH/2, iHalfWidth=iScaledW/2;//explosion vars
	register float fY=0.0f, fX;
	register float fSrcStride=(float)iSrcStride;
	register int iLimiter=lptargaThis1->height-1;
	if (scale<=0) scale=1;//prevent crash
	register float fInverseScale=1.0f/scale;
	for (register int index_y = 0; index_y < iScaledH; index_y++, fY+=1.0f) {
		if (index_y>=iLimiter) break;
		//if (index_y%2) //splits object into squares
		//	toY+=explodedness;
		//toX=tox1; //reset offset before exploding next line of image
		yMaxEx=(index_y<iHalfHeight)?iHalfHeight-index_y:index_y-iHalfHeight;
		yOffset=toY;
		fX=0.0f;
		for (register int index_x = 0; index_x < iScaledW; index_x++, fX+=1.0f) {
			//if (index_x>lptargaThis1->width) break;
			//if (index_x%2) //splits object into squares
			//	toX+=explodedness;
			xOffset=toX;
			xMaxEx=(index_x<iHalfWidth)?iHalfWidth-index_x:index_x-iHalfWidth;
			sPtr=(BYTE*)&lpdwSrc[(int)(fY*fInverseScale)*image->width + (int)(fX*fInverseScale)];
			blue=*sPtr++;
			green=*sPtr++;
			red=*sPtr++;
			alpha=*sPtr; //++;
			if (explod1 || 255-opacityCap) { //this use of opacity cap is unique to DXMan, makes it red when transparent
				green>>=1;
				blue>>=2;
			}

			if (alpha>opacityCap) alpha=opacityCap; //"Cap" opacity off at opacityCap to create transparency if not 255

			if (explodedness) {
								//uses -= for implosion
				 if (index_x<iHalfWidth) xOffset-=(int)(FRand(-xMaxEx,0)*fExplodeLog);
				 else xOffset-=(int)(FRand(0,xMaxEx)*fExplodeLog);
				 if (index_y<iHalfHeight) yOffset=toY+(int)(FRand(-yMaxEx,0)*fExplodeLog);
				 else yOffset=toY+(int)(FRand(0,yMaxEx)*fExplodeLog);
				 green*=fRemains;
				 red*=fRemains;
				 green*=fRemains;
				 alpha*=fRemainsExp;
			}
			
			register int iDest1=xOffset + index_x + ((index_y+yOffset)*iDestW);
			dPtr=(BYTE*)&lpdwDest[iDest1];
					if ((dwStat & STATUS_SHIELD)&&alpha<172)
					{//blue outline
						blue = 192;
						green = 64;
					}
					blue = alphaLook[blue][*dPtr][alpha];
					dPtr++;
					green = alphaLook[green][*dPtr][alpha];
					dPtr++;
					red = alphaLook[red][*dPtr][alpha];
					//dPtr+=2;
					pixel=_RGB32BIT(255,red,green,blue);
					if (iDest1<SCREENBUFFER_SIZE) {
						lpdwDest[iDest1] = pixel;
					}
		}//end for index_x
		if (iDestNow>=SCREENBUFFER_SIZE) break;
		//dPtr+=iStride-iScaledW*4;
	}//end for index_y
	return(1);
} // end TargaToTarga32_FX_Scaled

///////////////////////////////////////////////////////////////////////////////

int TargaToTarga32(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY)
{
	DWORD *lpdwDest = (DWORD *)lptargaThis1->buffer;
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
			lpdwDest[toX + index_x + ((index_y+toY)*iStride >> 2)] = pixel;

		} // end for index_x

	} // end for index_y

	return(1);
} //end TargaToTarga32

////////////////////////////////////////////////////////////////////////////////

int TargaToTargaAlphaFX(LPTARGA image, LPTARGA lptargaThis1, int toX, int toY, int opacityCap, int explodedness, DWORD dwStat)
{//uses alpha lookup
	//register DWORD *lpdwDest = (DWORD *)lptargaThis1->buffer;
	int iStride=TargaStride(lptargaThis1);
	register BYTE *dPtr = lptargaThis1->buffer;//(BYTE*)&lpdwDest[toX + (toY*iStride >> 2)]; //dest pointer
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

	for (register int index_y = 0; index_y < image->height; index_y++) {
		for (register int index_x = 0; index_x < image->width; index_x++) {
			draw=0;	//for clipping
			
			//Get BGR values and alpha
			//BYTE blue	= (image->buffer[index_y*image->width*4 + index_x*4 + 0]),
			//	green = (image->buffer[index_y*image->width*4 + index_x*4 + 1]),
			//	red	 = (image->buffer[index_y*image->width*4 + index_x*4 + 2]),
			//	alpha = (image->buffer[index_y*image->width*4 + index_x*4 + 3]);
			

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
				//if (index_x+toX<SCREEN_WIDTH)
				//	if (index_x+toX>=0)
				//		if (index_y+toY>=0)
				//			if (index_y+toY<SCREEN_HEIGHT)
								draw=1;
				if (draw) {
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
				//if (index_x+toX<SCREEN_WIDTH)
				//	if (index_x+toX>=0)
				//		if (index_y+toY>=0)
				//			if (index_y+toY<SCREEN_HEIGHT)
								draw=1;
				if (draw) {
				
					//Calculate value by alpha
					//cookedAlpha = *sPtrAlpha/255.00;
					//*dPtr = ((*sPtr - *dPtr) * cookedAlpha + *dPtr);
					//dPtr++;	sPtr++;
					//*dPtr = ((*sPtr - *dPtr) * cookedAlpha + *dPtr);
					//dPtr++; sPtr++;
					//*dPtr = ((*sPtr - *dPtr) * cookedAlpha + *dPtr);
					//dPtr++;	if (REAL_BYTEDEPTH==4) dPtr++; sPtr+=2;
				
				
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
bool AddScreenItem(int iType, int zOrder, int iEntityIndex) {
	bool bGood=false;
	if (iScreenItems<MAX_SCREENITEMS) {
		screenitemarr[iScreenItems].zOrder=zOrder;
        screenitemarr[iScreenItems].index=iEntityIndex;
		screenitemarr[iScreenItems].iType=iType;
		iScreenItems++;
		bGood=true;
	}
}
///////////////////////////////////////////////////////////////

int GameMain()
{
	if (done) return(0);
	// for now test if user is hitting ESC
	if ((dwPressing&GAMEKEY_EXIT)&&(SDL_GetTicks()-iEscapeTime)>1000) {//int answer = 0;
		iEscapeTime=SDL_GetTicks();
			TargaToTarga32_Alpha(lptargaGameScreen[2], lptargaScreen,
		SCREEN_WIDTH/2-lptargaGameScreen[2]->width/2+SCREEN_OFFSET_X,
		SCREEN_HEIGHT/2-lptargaGameScreen[2]->height/2+SCREEN_OFFSET_Y);
		TargaToScreen_AutoCrop(lptargaScreen);
		iEscapeTime=SDL_GetTicks();

		while (1) {
			if(SDL_PollEvent (&event)) {
				if (event.key.keysym.sym==SDLK_DELETE) {
					gameState=GAME_STATE_EXIT;
					break;
				}
				else if (event.key.keysym.sym==SDLK_ESCAPE) {
					if((SDL_GetTicks() - iEscapeTime) > 1000) {
						dwPressing&=GAMEKEY_EXIT^0xFFFFFFFF;
						gameState=GAME_STATE_RUN;
						break;
					}
				}
			}
		}
		
		//gameState=GAME_STATE_YOU_LOSE;
	}


	if (gameState == GAME_STATE_INIT) {
		TargaToTarga32(lptargaIntro, lptargaScreen, SCREEN_OFFSET_X+(SCREEN_WIDTH/2-lptargaIntro->width/2),SCREEN_OFFSET_Y);
		TargaToScreen_AutoCrop(lptargaScreen);
		//PlaySound((const char*)SOUND_ID_INTRO/*"Orangejuice-DXMan-Intro.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		DWORD startTime=SDL_GetTicks();
			while((SDL_GetTicks() - startTime) < 1000);
		bool bStart=false;
		while (!bStart) {
			if (SDL_PollEvent (&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					 if (event.key.keysym.sym==SDLK_DELETE) {
						bStart=true;
						break;
					 }
				default:break;
			 	}
			}
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

			
			//32-bit backdrop surface
			//if (!TargaToTarga32(lptargaBackdrop, lptargaScreendrop, 0, 0))
			//{	
			//	iErrorsSaved++;
			//	error_txt << "\nUnable to render Targa to 32-bit Backdrop in START_LEVEL 1";
			//	return(0);
			//}
			
			
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

			//32-bit backdrop surface
			//if (!TargaToTarga32(lptargaBackdrop, lptargaScreendrop, 0, 0))
			//{	
			//	error_txt << "\nUnable to render Targa to 32-bit Backdrop in START_LEVEL 2";
			//	iErrorsSaved++;
			//	return(0);
			//}
			

			////errlog.AddPrecedent("Calling TargaUnload(lptargaBackdrop)","START_LEVEL 2");
			//TargaUnload(lptargaBackdrop);
			//errlog.Add(iLastErr, "unload lptargaBackdrop", "START_LEVEL 2");
			
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
			
			//if (!TargaToTarga32(lptargaBackdrop, lptargaScreendrop, 0, 0)) {
			//	error_txt << "\nUnable to render targaBack to 32-bit surface";
			//	iErrorsSaved++;
			//	return(0);
			//}
			//TargaUnload(lptargaBackdrop);
			//errlog.Add(iLastErr, "unload lptargaBackdrop", "START_LEVEL 3");
			
		}


		hero->iHP=255;
		if (gameLevel==2) hero->dwStatus |= STATUS_ANTIGRAVITY;
		gameState = GAME_STATE_START_STAGE;
	}

	else if (gameState == GAME_STATE_START_STAGE)
	{//START STAGE
		bombed=0;
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
		music = Mix_LoadMUS("Orangejuice-DXMan-Invasion.ogg");
		Mix_PlayMusic(music, -1); //0 for once, -1 for infinite
		//PlaySound((const char*)SOUND_ID_INVASION/*"Orangejuice-DXMan-Invasion.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		for (int index=0; index<MAXALIENS; index++) {
			aliens[index] = new Alien(3, index-2, 4);
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

	else if (gameState == GAME_STATE_RUN) {
		//if KEYDOWN(65/* a */) hero->dwStatus |= STATUS_ANTIGRAVITY;
		//if KEYUP(65) hero->dwStatus ^= STATUS_ANTIGRAVITY;

		if (dwPressing&GAMEKEY_RIGHT) { hero->turn(1); hero->move(1,0);}
		if (dwPressing&GAMEKEY_LEFT) { hero->turn(-1); hero->move(-1,0);}
		if (dwPressing&GAMEKEY_JUMP) { hero->jump(); }
		if (dwPressing&GAMEKEY_UP) { hero->aim(-1); }
		if (dwPressing&GAMEKEY_DOWN) { hero->aim(1);	}
		if (dwPressing&GAMEKEY_UP) { hero->move(0,1); }
		if (dwPressing&GAMEKEY_DOWN) { hero->move(0,-1);	}
		if (!(dwPressing&GAMEKEY_FIRE)) hero->shootDelay=0;
		if (dwPressing&GAMEKEY_FIRE) hero->shoot();

		if (dwPressing&GAMEKEY_DOUBLESPEED) doublecode++;//holding '2' activates STATUS_DOUBLESPEED
		else doublecode=0;

		if (dwPressing&GAMEKEY_PRECISIONAIM) precisecode++;
		else precisecode=0;

		if (dwPressing&GAMEKEY_RAPIDFIRE) rapidfirecode++; //holding 'r' activates STATUS_RAPIDFIRE
		else rapidfirecode=0;

		//if KEYUP(80/* p */) keyDelay=0;

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
		

		if (doublecode>=CODE_THRESHOLD && hero->dwStatus ^ STATUS_DOUBLESPEED) hero->doublespeed();
		if (precisecode>=CODE_THRESHOLD && hero->dwStatus ^ STATUS_PRECISIONAIM) hero->precisionaim();
		if (rapidfirecode>=CODE_THRESHOLD && hero->dwStatus ^ STATUS_RAPIDFIRE) hero->rapidfire();

		TargaToTarga32(lptargaBackdrop, lptargaScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y);
		DrawRadarField();

		//Update all the existing aliens, delete the rest
		if (hero!=NULL) {
			hero->refresh();
			AddScreenItem(SCREENITEM_HERO,ZORDER_FROMY(hero->y),0);
			DrawRadarDot(hero->x, hero->y, hero->z, 0xFFFFFFFF);
		}

		for (int iAlien=0; iAlien < MAXALIENS; iAlien++) {
			if (aliens[iAlien] != NULL) {
				aliens[iAlien]->refresh();
				AddScreenItem(SCREENITEM_ALIEN,ZORDER_FROMY(aliens[iAlien]->y),iAlien);
				////hero->drawTarget(iAlien);
				DrawRadarDot(aliens[iAlien]->x, aliens[iAlien]->y, aliens[iAlien]->z, 0x0000FFFF);
				if (aliens[iAlien]->explodedBy>MAX_EXPLODEDNESS) {
					if (aliens[iAlien]->dwStatus & STATUS_BOSS) {
						for (int iAlienToDeShield=0; iAlienToDeShield < MAXALIENS; iAlienToDeShield++) {
							if (aliens[iAlienToDeShield]->dwStatus & STATUS_SHIELD)
								aliens[iAlienToDeShield]->dwStatus ^= STATUS_SHIELD;
						}
					}

					delete aliens[iAlien];
					aliens[iAlien] = NULL;
				}
			}
		}

		if (hero->explodedBy>MAX_EXPLODEDNESS) {
			delete hero;
			hero=NULL;
			gameState=GAME_STATE_YOU_LOSE;
		}
		for (int iX=0; iX<MAXSHOTS; iX++) {
			if (shots[iX] != NULL) {
				shots[iX]->refresh();
				AddScreenItem(SCREENITEM_SHOT,ZORDER_FROMY(shots[iX]->y),iX);
				DrawRadarDot(shots[iX]->x, shots[iX]->y, shots[iX]->z, 0xFF8800FF);
				if (shots[iX]->dead) {
					delete shots[iX];
					shots[iX] = NULL;
				}
			}
		}

		int zNow=MAX_SCREENITEMS-1;
		int iScreenItemsNow=iScreenItems;
        //Draw ScreenItems:
/*
		while (iScreenItems && zNow>=0) {
			for (int iItem=0; iItem<iScreenItems; iItem++) {
				if (screenitemarr[iItem].zOrder==zNow) {
					switch (screenitemarr[iItem].iType) {
						case SCREENITEM_ALIEN:
							aliens[ screenitemarr[iItem].index ]->redraw();
							iScreenItems--;
							break;
						case SCREENITEM_SHOT:
							shots[ screenitemarr[iItem].index ]->redraw();
							iScreenItems--;
							break;
						case SCREENITEM_HERO:
							hero->redraw();
							iScreenItems--;
							break;
						default:
							break;
					}
				}
			}
			zNow--;
		}
		iScreenItems=0; //not needed?
*/
		/*
		if(calculateExplosion(lptargaScreen, 100, 100, 20, 0, EXPLOSION_CHECK_COUNT) == 0)
		{
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
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
		music = Mix_LoadMUS("trumpet.wav");
		Mix_PlayMusic(music, 0); //0 for once, -1 for infinite
		//PlaySound((const char*)SOUND_ID_TRUMPET/*"trumpet.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
		for (int iX=0; iX<MAXSHOTS; iX++)
		{
			if (shots[iX]!=NULL)
			{
				delete shots[iX];
				shots[iX]=NULL;
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
					SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2+SCREEN_OFFSET_X,
					SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2+SCREEN_OFFSET_Y);
			TargaToScreen_AutoCrop(lptargaScreen);
			DWORD startTime=SDL_GetTicks();
			while((SDL_GetTicks() - startTime) < 3000);
		}
	}
	else if (gameState == GAME_STATE_WIN_GAME)
	{
		int screenNow=1;
		if (doublecode>=CODE_THRESHOLD)
		{
			screenNow=7;
			if (rapidfirecode>=CODE_THRESHOLD) screenNow++;//if (precisecode>=20) screenNow++;
		}

		//	TargaToTarga32_Alpha(lptargaGameScreen[screenNow], lpddsPrimary,
		//		SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2,
		//		SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2);
		TargaToTarga32_Alpha(lptargaGameScreen[screenNow], lptargaScreen,
				SCREEN_WIDTH/2-lptargaGameScreen[screenNow]->width/2+SCREEN_OFFSET_X,
				SCREEN_HEIGHT/2-lptargaGameScreen[screenNow]->height/2+SCREEN_OFFSET_Y);

		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
		music = Mix_LoadMUS("Orangejuice-DXMan-Ending.ogg");
		Mix_PlayMusic(music, -1); //0 for once, -1 for infinite
		//PlaySound((const char*)SOUND_ID_ENDING/*"Orangejuice-DXMan-Ending.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		DWORD startTime=SDL_GetTicks();
		while((SDL_GetTicks() - startTime) < 300);
		bool bExit=false;
			while(!bExit) {
				if (SDL_PollEvent (&event)) {
					switch (event.type) {
					case SDL_KEYDOWN:
					 if (event.key.keysym.sym==SDLK_ESCAPE) {
						bExit=true;
					 }
					break;
					default:break;
					}
				}
			}
		gameState = GAME_STATE_EXIT;
	}
	else if (gameState == GAME_STATE_YOU_LOSE) {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
		music = Mix_LoadMUS("Orangejuice-DXMan-Intro.ogg");
		Mix_PlayMusic(music, -1); //0 for once, -1 for infinite
		//PlaySound((const char*)SOUND_ID_THRUST/*"thrust.wav"*/, hinstance_app, SND_RESOURCE | SND_ASYNC);
		TargaToTarga32_Alpha(lptargaGameScreen[0], lptargaScreen,
			SCREEN_WIDTH/2-lptargaGameScreen[0]->width/2+SCREEN_OFFSET_X,
			SCREEN_HEIGHT/2-lptargaGameScreen[0]->height/2+SCREEN_OFFSET_Y);
		TargaToScreen_AutoCrop(lptargaScreen);
		DWORD startTime=SDL_GetTicks();
		while((SDL_GetTicks() - startTime) < 5000);
		//PostMessage(hwndMain,WM_CLOSE,0,0);
		//PostMessage(hwndMain,WM_QUIT,0,0);
		//window_closed = 1;
		gameState=GAME_STATE_INIT;
	}
	else if (gameState == GAME_STATE_EXIT) {
		if (!done) {
			try {
				SDL_Quit();
			}
			catch (char* sExn) {
				try {
					error_txt<<sExn;
				}
				catch (char* sExn) {
				}
			}
		}
	}

	// return success or failure or your own return code here
	return(1);

} // end GameMain

////////////////////////////////////////////////////////////

int GameInit() {
	//errlog.SetMessageDest(hwndMain);
	char *msg;
	/* Initialize SDL */
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		sprintf (msg, "Couldn't initialize SDL: %s\n", SDL_GetError ());
		//MessageBox (0, msg, "Error", MB_ICONHAND); //re-implement this w/o windows api
		free (msg);
		exit (1);
	}
	atexit(SDL_Quit);
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		//sprintf("Unable to open audio!\n");
		exit(1);
	}
	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);//what we got
	
	/* Set 640x480 32-bits video mode */
	screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF);
	if (screen == NULL) {
		sprintf (msg, "Couldn't set 640x480x32 video mode: %s\n",
			SDL_GetError ());
		//MessageBox (0, msg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
		free (msg);
		exit (2);
	}
	SDL_WM_SetCaption ("SDL MultiMedia Application", NULL);

	music = Mix_LoadMUS("Orangejuice-DXMan-Intro.ogg");

//Mix_Chunk *mcBomb=NULL;
//Mix_Chunk *mcLaserAlien=NULL;
//Mix_Chunk *mcLaser=NULL;
//Mix_Chunk *mcExplosion=NULL;
//Mix_Chunk *mcOuchAlien=NULL;
//Mix_Chunk *mcOuchZap=NULL;
//Mix_Chunk *mcShieldZap=NULL;
//Mix_Chunk *mcThruster=NULL;
	mcBomb=Mix_LoadWAV("bomb.wav");
	mcLaserAlien=Mix_LoadWAV("laser-alien.wav");
	mcLaser=Mix_LoadWAV("laser.wav");
	mcExplosion=Mix_LoadWAV("explosion.wav");
	mcOuchAlien=Mix_LoadWAV("ouchalien.wav");
	mcOuchZap=Mix_LoadWAV("ouchzap.wav");
	mcShieldZap=Mix_LoadWAV("shieldzap.wav");
	mcTrumpet=Mix_LoadWAV("trumpet.wav");

	mcThruster = Mix_LoadWAV("thruster.wav");
	Mix_VolumeChunk(mcThruster,40);
	mcLaser = Mix_LoadWAV("laser.wav");
	Mix_VolumeChunk(mcLaser,100);

	Mix_PlayMusic(music, -1); //0 for once, -1 for infinite
	//Mix_HookMusicFinished(MusicDone);
	//errlog.AddTitle("Starting GameInit");

	/*
	*/
	

	//Load alpha lookup table from file
	//int file_handle;
	ifstream ifData("alphalook.raw", ios::out | ios::in | ios::binary);//OFSTRUCT file_data; //the file data information
	
	if (0==ifData.is_open()){// ((file_handle = OpenFile("alphalook.raw",&file_data,OF_READ))==-1) //open the file if it exists
	//if can't open file:
		error_txt << "\nERROR: Unable to open alpha lookup file!";
		iErrorsSaved++;
		return(0);	
	}
	ifData.read((char*)alphaLook, 256*256*256); //_lread(file_handle, alphaLook,256*256*256);
	ifData.close();//_lclose(file_handle);

	if (iErrorsSaved)
	{
		//Generate alpha lookup table if needed
		error_txt << "\nNeed to generate alpha lookup table..";
		int iSource=0;
		int iDest;
		int iAlpha;//can't use bytes because the loop will never end (never >255!)
		for (float source=0.0f; source<=255.0f; source+=1.0f, iSource++) {
			iDest=0;
			for (float dest=0.0f; dest<=255.0f; dest+=1.0f, iDest++) {
				iAlpha=0;
				for (float alpha=0.0f; alpha<=255.0f; alpha+=1.0f, iAlpha++) {
							alphaLook[iSource][iDest][iAlpha]=(BYTE)((source-dest)*alpha/255.0f+dest);
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
	//lpspritearr=(LPSPRITE*)malloc(MAX_SPRITES*sizeof(LPSPRITE));
	//memset(lpspritearr,0,MAX_SPRITES*sizeof(LPSPRITE));

	//RECT explosionRect={0,0,200,200};
	RECT screenRect= {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

	REAL_BPP=32;
	REAL_BYTEDEPTH=4;

	//char *msg;
	//int done;

	//if (SDL_Init (SDL_INIT_VIDEO) < 0)
	//{
	//	sprintf (msg, "Couldn't initialize SDL: %s\n", SDL_GetError ());
	//	//MessageBox (0, msg, "Error", MB_ICONHAND); //TODO: re-implement this w/o windows api
	//	free (msg);
	//	exit (1);
	//}
	//atexit (SDL_Quit);
	//show splash screen here
	
	/* Set 640x480 32-bits video mode */
	// screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_WIDTH, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF);
	//if (screen == NULL) {
	//	sprintf (msg, "Couldn't set video mode: %s\n",
	//		SDL_GetError ());
	//	MessageBox (0, msg, "Error", MB_ICONHAND); 
	//	free (msg);
	//	exit (2);
	//}
	//SDL_WM_SetCaption ("DXMan", NULL);
	
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
	//	if (!TargaToTarga32(lptargaBackdrop, lptargaScreendrop, 0, 0))
	//	{
	//		error_txt <<"\nCould not load targaBack to 32-bit BackDrop in GameInit";
	//		iErrorsSaved++;
	//		return(0);
	//	}
	//TargaUnload(lptargaBackdrop);
	errlog.Add(iLastErr, "unload lptargaBackdrop", "GameInit");


	//DDrawFillSurface(lptargaExplosion,0);
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
	lptargaAlienBoss=TargaLoadSeq("alienboss", &iFramesFound);
	errlog.Add(iLastErr,"alienboss*.tga", "GameInit");
	if (iFramesFound!=iFramesAlienBoss) error_txt << "not all AlienBoss frames loaded."<<endl;
	iFramesFound=iFramesShot;
	lptargaShot=TargaLoadSeq("shotAni", &iFramesFound);
	errlog.Add(iLastErr,"shotani*.tga", "GameInit");
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
	//if (lpspritearr) {
	//	for (int iSpriteX=0; iSpriteX=MAX_SPRITES; iSpriteX++) {
	//		SpriteUnload(lpspritearr[iSpriteX]);
	//	}
	//	free(lpspritearr);
	//}


	//Return success or failure or return code
	return(1);

} // end GameInit

/////////////////////////////////////////////////////////////

int GameShutdown()
{
	//This is called after the game is exited and the main event
	// loop while is exited, do all you cleanup and shutdown here

		//Release palette if 8-bit was used
	//if (lpddpal){lpddpal->Release(); lpddpal = NULL;} //
	try {
		//Unload Targas
		TargaUnloadSeq(lptargaAlien, 2);
		errlog.Add(iLastErr, "lptargaAlien", "GameShutDown");
		TargaUnloadSeq(lptargaAlienBoss, 3);
		errlog.Add(iLastErr, "lptargaAlienBoss", "GameShutDown");
		TargaUnloadSeq(lptargaHero, 21);
		errlog.Add(iLastErr, "lptargaHero", "GameShutDown");
		TargaUnloadSeq(lptargaShot, 3);
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
	}
    catch(char* sExn) {
		try {
			error_txt<<"Exception in GameShutdown: "<<sExn;
		}
		catch (char* sExn) {
		}
	}
	return(1);

} // end GameShutdown

////////////////////////////////////////////////////////////////////////////////
#define FRADAR_ZOOM  5.0f
void DrawRadarDot(float xPos, float yPos, float zPos, DWORD dwPixel) {

	int xCenter=SCREEN_WIDTH-(int)((float)SCREEN_WIDTH*.1f);
	xCenter+=xPos*FRADAR_ZOOM;
	int yCenter=(int)((float)SCREEN_HEIGHT*.1f);
	yCenter+=yPos*FRADAR_ZOOM;
	int xOffset=(int)(zPos);
	int yOffset=(int)(zPos);
	int left=xCenter-xOffset, right=xCenter+xOffset, top=yCenter-yOffset, bottom=yCenter+yOffset;


	DrawExclusiveRect(left, top, bottom+1, right+1, dwPixel, true);
}
void DrawRadarField() {
	static DWORD dwPixel=0x004400FF;
	static int xCenter=SCREEN_WIDTH-(int)((float)SCREEN_WIDTH*.1f);
	static int yCenter=(int)((float)SCREEN_HEIGHT*.1f);
	static int xOffset=(int)(FRADAR_ZOOM*6.0f);
	static int yOffset=(int)(FRADAR_ZOOM*4.0f);
	static int left=xCenter-xOffset, right=xCenter+xOffset, top=yCenter-yOffset, bottom=yCenter+yOffset;
	DrawExclusiveRect(left, top, bottom, right, dwPixel, false);
}
void DrawExclusiveRect(int left, int top, int bottom, int right, DWORD dwPixel, bool bFilled) {
	if (left<0||right>=BUFFER_WIDTH||top<0||bottom>=BUFFER_HEIGHT)//error checking
		return;
	left+=SCREEN_OFFSET_X;
	right+=SCREEN_OFFSET_X;
	top+=SCREEN_OFFSET_Y;
	bottom+=SCREEN_OFFSET_Y;
	bool bGood=true;
	DWORD *lpdwDest=(DWORD*)lptargaScreen->buffer;
	int iScreenW=lptargaScreen->width;
	int iScreenH=lptargaScreen->height;
	if (bottom<=top) {//if bad, draw vert line
		bottom=top+6;
		right=left+1;
		DrawExclusiveRect(left,top,bottom,right,dwPixel,false);
		if (right<=left) {//if bad, draw horz line too
			top+=2;
			bottom=top+1;
			left-=2;
			right=left+6;
			DrawExclusiveRect(left,top,bottom,right,dwPixel,false);
		}
		bGood=false;
	}
	else if (right<=left) {//if bad, draw horz line
		bottom=top+1;
		right=left+6;
        DrawExclusiveRect(left,top,bottom,right,dwPixel,false);
		bGood=false;
	}
	int iStart=top*iScreenW+left;//no stride since using DWORD* to buffer
	lpdwDest+=iStart;
	int iSkipEdge=iScreenW-(right-left);
	if (bGood) {
		if (bFilled) {
			for (int yNow=top; yNow<bottom; yNow++) {
				for (int xNow=left; xNow<right; xNow++) {
					*lpdwDest=dwPixel;
					lpdwDest++;
				}
				lpdwDest+=iSkipEdge;
			}
		}//end if bFilled
		else {//only draw outline
			for (int yNow=top; yNow<bottom; yNow++) {
				for (int xNow=left; xNow<right; xNow++) {
					if (yNow==top||yNow==bottom-1||xNow==left||xNow==right-1)
						*lpdwDest=dwPixel;
					lpdwDest++;
				}
				lpdwDest+=iSkipEdge;
			}
		}
	}//end if bGood
}

////////////////////////////////////////////////////////////////////////////////
