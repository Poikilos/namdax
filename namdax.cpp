#ifndef MAIN_C
#define MAIN_C
//#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_TTF.h>

#include <sprite.cpp>
#include <world.cpp>
#include <anim32bgra.cpp>
#include <gbuffer32bgra.cpp>
#include <targa.cpp>
#include <byter.cpp>
#include <base.cpp>
#include <dotnetfake.cpp>

//#include "C:\My Documents\Projects-cpp\Base\sprite.cpp"
//#include "C:\My Documents\Projects-cpp\Base\world.cpp"
//#include "C:\My Documents\Projects-cpp\Base\anim32bgra.cpp"
//#include "C:\My Documents\Projects-cpp\Base\gbuffer32bgra.cpp"
//#include "C:\My Documents\Projects-cpp\Base\targa.cpp"
//#include "C:\My Documents\Projects-cpp\Base\byter.cpp"
//#include "C:\My Documents\Projects-cpp\Base\base.cpp"
//#include "C:\My Documents\Projects-cpp\Base\dotnetfake.cpp"

using namespace std;

namespace ExpertMultimediaBase {
	//	using namespace std;
	// TYPES //////////////////////////////////////////////////////
	//typedef struct {int left; int right; int top; int bottom;} RECT;
	typedef unsigned short USHORT;
	typedef unsigned short WORD;
	typedef unsigned char BYTE;
	//typedef UInt32 DWORD;
	//typedef unsigned __int16 USHORT;
	//typedef unsigned __int8 BYTE;
	//typedef ULARGE_INTEGER __int64);

	// DEFINES ////////////////////////////////////////////////
	#define BOUNDARY_BOUNCE 0
	#define BOUNDARY_STOP	1
	#define BOUNDARY_DIE		2
	#define FXMIN	-10.0f
	#define FXMAX	10.0f
	#define FYMIN	-10.0f
	#define FYMAX	10.0f
	#define FXRANGE 20.0f
	#define FYRANGE 20.0f
	//default screen size
	#define SCREEN_WIDTH			640
	#define SCREEN_HEIGHT			480
	#define FSCREEN_WIDTH			640.0f
	#define FSCREEN_HEIGHT			480.0f
	#define BUFFER_WIDTH			1280
	#define BUFFER_HEIGHT			960
	#define BUFFER_SIZE				1228800 //1280x960
	#define SCREEN_OFFSET_X			320 //screen (a GBuffer32BGRA) is offset to avoid clipping
	#define SCREEN_OFFSET_Y			240
	#define FSCREEN_OFFSET_X		320.0f //screen (a GBuffer32BGRA buffer) is offset to avoid clipping
	#define FSCREEN_OFFSET_Y		240.0f
	#define SCREEN_BPP				32	 //bits per pixel, GameInit tries 32 or 24 if one is not available
	#define CODE_THRESHOLD			90
	#define GRAVITY_METERS_PERSEC_PERFRAME	(.8f/30.0f) //meters per second per frame (positive) gravity
	//States for game loop
	#define GAMESTATE_INIT				0
	#define GAMESTATE_START_AREA		1
	#define GAMESTATE_START_ENCOUNTER	2
	#define GAMESTATE_RUN				3
	#define GAMESTATE_SHUTDOWN			4
	#define GAMESTATE_EXIT				5
	#define GAMESTATE_WIN_ENCOUNTER	6
	#define GAMESTATE_WIN_GAME			7
	#define GAMESTATE_YOU_LOSE			8
	#define GAMESTATE_LOSEGUY			9
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
	#define STATUS_RAPIDFIRE		256
	#define STATUS_AIMBOMBS			512 //AIM AI
	#define STATUS_VARIABLESHIELD	1024

	#define GAMEKEY_UP				1
	#define GAMEKEY_DOWN	 		2
	#define GAMEKEY_LEFT	 		4
	#define GAMEKEY_RIGHT			8
	#define GAMEKEY_FIRE	 		16
	#define GAMEKEY_JUMP	 		32
	#define GAMEKEY_EXIT	 		64
	#define GAMEKEY_RAPIDFIRE 		128
	#define GAMEKEY_DOUBLESPEED		512
	#define GAMEKEY_UP2				1024
	#define GAMEKEY_DOWN2	 		2048
	#define GAMEKEY_LEFT2			4096
	#define GAMEKEY_RIGHT2			8192
	UInt32 dwPressing=0;
	#define SCREENITEM_ALIEN	1
	#define SCREENITEM_SHOT		2
	#define SCREENITEM_HERO		3
	#define SCREENITEM_3DCURSOR 4
	#define MAXSHOTS			50
	#define MAX_SCREENITEMS		58  //maximum number of objects to render per frame
	#define MAX_ZORDER			50   //maximum zOrder value - INCLUSIVE
	#define FMAX_ZORDER			50.0f
	//DEBUG: ZBUFFER_YRATIO MUST be 50/YRANGE
	// STRUCTS ////////////////////////////////////////////////
	typedef struct SCREENITEM_STRUCT {
		int iType;//entity type i.e. SCREENITEM_ALIEN
		int zOrder;
		int index;//entity index, irrelevant for hero etc
	} SCREENITEM, * LPSCREENITEM;
	int iScreenItems=0;
	bool bInMenu=true;
	// MACROS /////////////////////////////////////////////////
	//uses limited ranges for more effect:
	#define SOUNDPAN_FROM3DX(x) (  (int)  ( (x<-6.0f) ? 0 : ((x>6.0f)?255:((x+6.0f)/12.0f*255)) )  )

	#define ZORDER_FROMY_FORCE(y) (   (  ( FYRANGE-((y)+(-FYMIN)) ) / FYRANGE )   *   FMAX_ZORDER   )
	#define ZORDER_FROMY(y) ( (int) (((y)<FYMIN) ? MAX_ZORDER : (((y)>FYMAX)?0:ZORDER_FROMY_FORCE(y))) )	 //get rendering z-order from y-value
	//Build a 32 bit color value in A.8.8.8 format (8-bit alpha mode):
	#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
	//24-bit specific, needed because of some video cards:
	#define _RGB24BIT(r,g,b) ((b) + ((g) << 8) + ((r) << 16))
	//audio panning:
	#define L_FROM_FACTOR(factorzerotoone) ((int)((1.0f-factorzerotoone)*255.0f))
	#define R_FROM_FACTOR(factorzerotoone) ((int)(factorzerotoone*255.0f))
	//Get the b,g,r or a of BGRA pixel:
	//#define _BYTEXOFPIXEL(m3dEnt.x,pixel) ((pixel >> 8*4(m3dEnt.x-1)) & 255)


	// GLOBALS ////////////////////////////////////////////////
	FPOINT3D p3dHero;
	bool bDebug=true;
	Mass3d m3dDebug[9];
	Mass3d m3dHeroEye, m3dCursor;
	//float xAimFlipper=-1.0f;
	//float yAimFlipper=-1.0f;
	//float zAimFlipper=-1.0f;
	bool bFlipYaw=false;
	bool bFlipPitch=false;
	float fBoss=0;//(Entity*)lparrAlien[iBoss]->fHealth;
	float fBossMax=1;//(Entity*)lparrAlien[iBoss]->fMaxHealth;
	int iBoss=-1;//global link to boss;
	bool bSDLQuitDone=false;
	Variables settings;
	Variables dat;
	bool bSDLQuitSent=false;
	bool bSplash=false;//if splash screen was drawn
	int iSplashTick;//time when splash screen appeared
	extern BYTE by3dAlphaLookup[256][256][256]; //lookup for alpha result: ((Source-Dest)*alpha/255+Dest)
	UInt32 dwarrEnergyGrad[256];//gradient for energy currency (ready attack) meter
	UInt32 dwarrHealthGrad[256];//gradient for health meter
	int xCursor=0, yCursor=0, xCursorDown=0, yCursorDown=0;
	bool bMouseDown=false;
	bool bMouseMove=false;
	bool bMouseDownR=false;
	float xfCursor=0.0f,yfCursor=0.0f,xfCursorDown=0.0f,yfCursorDown=0.0f;

	int xCursorCenter=24, yCursorCenter=24;
	int iGuysStart=3;
	int iGuys=iGuysStart;

	int iAreas=3,
		iBackdrops=iAreas,
		iFramesCursor=3,
		iEncounters=3;//TODO: finish this: change all usages to Anim32BGRA::GoToFrame or other Anim32BGRA methods

	int iFramesHero=60,
		iFramesHeroShadow=1,
		iFramesFlyer=60,
		iFramesFlyerShadow=2,
		iFramesBoss=3,
		iFramesBossShadow=1,
		iFramesShot=3,
		iFramesShotShadow=1,
		iFramesGameScreen=9,
		iBackDrops=iAreas;

	int iEscapeTime=0;
	int iErrorsSaved=0;
	int iFramesDropped=0;
	int REAL_BPP=32;
	int REAL_BYTEDEPTH=4;
	int keyDelay=0;
	int iDoubleCodeCounter=0; //player cheat code
	int iRapidFireCodeCounter=0;//player cheat code
	bool bBombed=false;
	Anim32BGRA				animBackdrop;
	GBuffer32BGRA			gbIntro;
	GBuffer32BGRA			gbIntroText;
	GBuffer32BGRA        gbSymbolShield;
	GBuffer32BGRA        gbSymbolBossHealth;
	GBuffer32BGRA        gbGuysDecor;
	Anim32BGRA				animFlyer;
	Anim32BGRA				animFlyerShadow;
	Anim32BGRA				animBoss;
	Anim32BGRA				animBossShadow;
	Anim32BGRA				animHero;
	Anim32BGRA				animHeroShadow;
	Anim32BGRA				animShot;
	Anim32BGRA				animShotShadow;
	Anim32BGRA				animGameScreen;
	Anim32BGRA				animGameScreenArea;
	Anim32BGRA				animGameScreenEncounter;
	Anim32BGRA				animCursor;
	// erieindianaerieindianaerieindiana
	GBuffer32BGRA				gbScreen;
	//LPSPRITE*           lpspritearr;//sprite array
	SCREENITEM			screenitemarr[MAX_SCREENITEMS];
	//char sTemp[800];								// general printing buffer
	int explosionResult=0;
	int iGameState=GAMESTATE_INIT;
	int iArea=1;		//reset later in GAME_STATE_INIT			//3 Areas total
	int iEncounter=1; //set to 1 again later in GAME_STATE_INIT					//3 Encounters total, encounters are in Areas
	int iAliens=0;
	int iMaxAliens=4;//changed later using ini file
	int iMaxAliensNow=iMaxAliens;//changes by level
	bool bPlayTrumpet=false;
	int iPlayBomb=-1;
	int iPlayLaserAlien=-1;
	int iPlayLaser=-1;
	int iPlayExplosion=-1;
	int iPlayOuchAlien=-1;
	int iPlayOuchZap=-1;
	int iPlayShieldZap=-1;
	int iPlayBlerrp=-1;
	int iPlayHitDirt=-1;
	int iPlayJump=-1;
	int iPlayScrapeGround=-1;
	int iPlayAngryAlien=-1;
	int iThruster=-1;
	Mix_Chunk *mcBomb=NULL;
	Mix_Chunk *mcLaserAlien=NULL;
	Mix_Chunk *mcLaser=NULL;
	Mix_Chunk *mcExplosion=NULL;
	Mix_Chunk *mcOuchAlien=NULL;
	Mix_Chunk *mcOuchZap=NULL;
	Mix_Chunk *mcShieldZap=NULL;
	Mix_Chunk *mcBlerrp=NULL;
	Mix_Chunk *mcHitDirt=NULL;
	Mix_Chunk *mcJump=NULL;
	Mix_Chunk *mcScrapeGround=NULL;
	Mix_Chunk *mcAngryAlien=NULL;
	Mix_Chunk *mcTrumpet=NULL;
	Mix_Chunk *mcThruster=NULL;
	int iChanBomb=-1;
	int iChanLaserAlien=-1;
	int iChanLaser=-1;
	int iChanExplosion=-1;
	int iChanOuchAlien=-1;
	int iChanOuchZap=-1;
	int iChanShieldZap=-1;
	int iChanBlerrp=-1;
	int iChanHitDirt=-1;
	int iChanJump=-1;
	int iChanScrapeGround=-1;
	int iChanAngryAlien=-1;
	int iChanThruster=-1;
	int iChanTrumpet=-1;

	// PROTOTYPES	//////////////////////////////////////////////
	bool GameInit();
	bool GameMain();
	bool GameShutdown();
	void ResetRand();
	float FRand();
	float FRand(float fMin, float fMax);
	int IRandPositive();
	int IRand(int iMin, int iMax);
	bool AddScreenItem(int iType, int zOrder, int iEntityIndex);
	void DrawRadarDot(float xPos, float yPos, float zPos, UInt32 dwPixel);
	void DrawRadarDot(float xPos, float yPos, float zPos, UInt32 dwPixel, bool bFilled);
	void DrawRadarRect(float left, float top, float bottom, float right, UInt32 dwPixel, bool bFilled);
	void DrawExclusiveRect(int left, int top, int bottom, int right, UInt32 dwPixel, bool bFilled);
	void DrawRadarField();
	void ShowDebugVars();
	bool SafeChunkUnload(Mix_Chunk* &mcToNull);
	void RadarDotAt(int &xPix, int &yPix, float xPos, float yPos);
	float RadarCenterX();
	float RadarCenterY();
	float RadarHeight();
	int PlaySounds();
	void ClearSounds();
	void Refresh3dCursor();
	void Draw3dCursor(byte byBrightness);
	void SayWhatIDrewIfFalse(bool bDrawFunctionReturn, int iEntityType, string sDescription);
	string EntityTypeToString(int iTypeX);
	float MetersToMove(float fMetersPerSecond, int iForThisManyMilliseconds);
	float DegreesToMove(float fDegreesPerSecond, int iForThisManyMilliseconds);
	void DrawCube(Mass3d &m3dNow, Pixel &pixelNear, Pixel &pixelFar);
	bool DrawScreen();
	inline void SleepWrapper(int iTicks);
	void LoadSequence(Anim32BGRA &animToLoad, string sFileBaseNameOnly, int iFramesExpectedElseError);
	void LoadImage(GBuffer32BGRA &gbToLoad, string sFile);
	bool GBuffer_FX_Scaled(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, int xFlat, int yFlat,
			float fOpacity, float fExplodedness, UInt32 dwStat, float fScale);

	////////////////////////////////// CLASSES /////////////////////////////////////

	////////////////////////////////// CAMERA //////////////////////////////////////

	class Camera {
	private:
	   	float fScreenRatio;
		float fApertureW;
		float fApertureH;
		float fAngleApertureTop;
		float fAngleApertureLeft;
	public:
	   	Mass3d m3dCam;
		Camera();
		void SetCamera(float x1, float y1, float z1, float xRot1, float yRot1, float zRot1, float fAperture);
		bool Point2dRatiosFrom3d(float &xReturn2d, float &yReturn2d, float x3d, float y3d, float z3d);
		float DestPixelsPerMeterAt(float fDistanceFromCamera);
	    bool Point2dFrom3d(FPOINT &pointReturn, Mass3d &point3d);
	    bool Point2dFrom3dWithScreenOffset(FPOINT &pointReturn, Mass3d &point3d);
		bool Point3dMoveAndRotateBy2d(Mass3d &pointOrig,float xRel,float yRel,float zRelFakePixelsPositiveIsAwayFromCamera, float xOrig, float yOrig);
		bool Point3dFrom2d(Mass3d &pointReturn,float xFlat,float yFlat,float fMetersFromCamera);
		bool Mass2dFrom3d(Mass2d &m2dWithPixPerMeter_ToModify, Mass2d &m2dShadow, Mass3d &m3dEnt);
	};

	Camera::Camera() {
	    fScreenRatio=FSCREEN_WIDTH/FSCREEN_HEIGHT; //i.e. 4/3 ratio
		SetCamera(0.0f,-14.836f,5.376f, 0.0f,-11.77,90.0f, 39.2025f);
		//i.e.  52.27f screen aperture width if 4:3 and 39.2025f height
	}
	bool Camera::Point2dFrom3dWithScreenOffset(FPOINT &pointReturn, Mass3d &point3d) {
		Point2dFrom3d(pointReturn, point3d);
		pointReturn.x+=FSCREEN_OFFSET_X;
		pointReturn.y+=FSCREEN_OFFSET_Y;
	}
	bool Camera::Point2dFrom3d(FPOINT &pointReturn, Mass3d &point3d) {
		bool bGood=true;
		try {
			static float xRightness, yDownness;
			Point2dRatiosFrom3d(xRightness,yDownness,point3d.x,point3d.y,point3d.z);
			pointReturn.x=xRightness*FSCREEN_WIDTH;
			pointReturn.y=yDownness*FSCREEN_HEIGHT;
		}
		catch (exception& exn) {
			ShowException(exn,"Point2dFrom3d");
		}
		catch (...) {
			ShowUnknownException("Point2dFrom3d");
		}
		return bGood;
	}
	bool Camera::Mass2dFrom3d(Mass2d &m2dWithPixPerMeter_ToModify, Mass2d &m2dReturnShadow, Mass3d &m3dEnt) {
		bool bGood=true;
		try {
			//debug yRot is the real Pitch NOT xrot!!!!!!!!
			static float xTranslate, yTranslate, xTestOffset, yTestOffset;
			static float xRightness, yDownness;
			Point2dRatiosFrom3d(xRightness,yDownness,m3dEnt.x,m3dEnt.y,m3dEnt.z);
			static float fScreenW=SCREEN_WIDTH, fScreenH=SCREEN_HEIGHT;
			xTranslate=xRightness*fScreenW;
			yTranslate=yDownness*fScreenH;
			//static float fSizer;//for getting m3dEnt.z-bounds in order to calculate scale
			//fSizer=m3dEnt.z+m3dEnt.zSize;
			//Point2dRatiosFrom3d(xTestOffset,yTestOffset,m3dEnt.x,m3dEnt.y,fSizer);
			//yTestOffset*=fScreenH;//makes it a pixel location (but xTestOffset is unused)
			float fDistFromCamera=DIST3D(m3dEnt,m3dCam);
			float fPixPerUnitHere=DestPixelsPerMeterAt(fDistFromCamera);
			m2dWithPixPerMeter_ToModify.fScale=fPixPerUnitHere/m2dWithPixPerMeter_ToModify.fPixelsPerMeter; //(yTranslate-yTestOffset)/(float)m2dWithPixPerMeter_ToModify.rectOriginal.bottom;//SET SCALE
			if (m2dWithPixPerMeter_ToModify.fScale<.05) m2dWithPixPerMeter_ToModify.fScale=.05;//prevents zero size after using scale
			m2dReturnShadow.fScale=m2dWithPixPerMeter_ToModify.fScale;
			float fWidthScaled=(float)(IROUNDF((float)m2dWithPixPerMeter_ToModify.rectOriginal.right*m2dWithPixPerMeter_ToModify.fScale))+1;//debug only: +1 shouldn't be needed
			float fHeightScaled=(float)(IROUNDF((float)m2dWithPixPerMeter_ToModify.rectOriginal.bottom*m2dWithPixPerMeter_ToModify.fScale))+1;//debug only: +1 shouldn't be needed
			float fHalfWidthScaled=FFLOOR(fWidthScaled/2.0f);
			float fHalfHeightScaled=FFLOOR(fHeightScaled/2.0f);
			m2dWithPixPerMeter_ToModify.rectRender.left=IROUNDF(xTranslate-m2dWithPixPerMeter_ToModify.FCenterXRelScaled());
			m2dWithPixPerMeter_ToModify.rectRender.top=IROUNDF(yTranslate-m2dWithPixPerMeter_ToModify.FCenterYRelScaled());
			m2dWithPixPerMeter_ToModify.rectRender.right=m2dWithPixPerMeter_ToModify.rectRender.left+IROUNDF(fWidthScaled);
			m2dWithPixPerMeter_ToModify.rectRender.bottom=m2dWithPixPerMeter_ToModify.rectRender.top+IROUNDF(fHeightScaled);

			Point2dRatiosFrom3d(xRightness,yDownness,m3dEnt.x,m3dEnt.y,0);
			m2dReturnShadow.rectRender.left=m2dWithPixPerMeter_ToModify.rectRender.left;//assumes top-down lighting
			m2dReturnShadow.rectRender.right=m2dReturnShadow.rectRender.left+(int)(m2dReturnShadow.fScale*(float)m2dReturnShadow.rectOriginal.right+.5f);//m2dWithPixPerMeter_ToModify.rectRender.right;
			m2dReturnShadow.rectRender.top=IROUNDF(yDownness*fScreenH-(float)m2dReturnShadow.rectOriginal.right/2.0f*m2dReturnShadow.fScale);
			m2dReturnShadow.rectRender.bottom=IROUNDF((float)m2dReturnShadow.rectRender.top+m2dReturnShadow.fScale*(float)m2dReturnShadow.rectOriginal.bottom);
		}
		catch (exception& exn) {
			ShowException(exn,"Mass2dFrom3d");
		}
		catch (...) {
			ShowUnknownException("Mass2dFrom3d");
		}
		return bGood;
	}//end Mass2dFrom3d
	float Camera::DestPixelsPerMeterAt(float fDistanceFromCamera) {
	    float fReturn=FSCREEN_WIDTH / FPDIST( FXOFRTHETA(fDistanceFromCamera,0),
									   FYOFRTHETA(fDistanceFromCamera,0),
									   FXOFRTHETA(fDistanceFromCamera,fApertureW),
									   FYOFRTHETA(fDistanceFromCamera,fApertureW) );
		if (fReturn<0.0f) fReturn=0.0f;
		return fReturn;
	}
	bool Camera::Point3dMoveAndRotateBy2d(Mass3d &pointOrig,float xRel,float yRel,float zRelFakePixelsPositiveIsAwayFromCamera, float xOrig, float yOrig) {
		bool bGood=true;
		static float fMetersFromCamera,
		    	fRatioFromLeft,
				fRatioFromTop,
				fDestPixPerUnitHere,
				fAltitude,
				fAzimuth,
				fDistance;
		try {
			fMetersFromCamera=DIST3D(pointOrig,m3dCam);
			fDestPixPerUnitHere=DestPixelsPerMeterAt(fMetersFromCamera);

			static FPOINT3D pointRel, pointTempForAltitude;
			pointRel.x=zRelFakePixelsPositiveIsAwayFromCamera; //xRel
			pointRel.y=-xRel; //zRelFakePixelsPositiveIsAwayFromCamera
			pointRel.z=-yRel; //-yRel
			Rotate(pointRel.y,pointRel.z,m3dCam.xRot);
			Rotate(pointRel.x,pointRel.z,m3dCam.yRot);
			Rotate(pointRel.x,pointRel.y,m3dCam.zRot);
			static float fSquare;
			fSquare=FSQUARED(pointRel.x) + FSQUARED(pointRel.y) + FSQUARED(pointRel.z);
			fDistance=(fSquare>0)?sqrt(fSquare):0;
			fAzimuth=SafeAngle360( THETAOFXY(pointRel.x,pointRel.y) );
			pointTempForAltitude.x=pointRel.x;
			pointTempForAltitude.y=pointRel.y;
			pointTempForAltitude.z=pointRel.z;
			Rotate(pointTempForAltitude.x,pointTempForAltitude.y,-fAzimuth);
			fAltitude=SafeAngle360( THETAOFXY(pointTempForAltitude.x,pointTempForAltitude.z) );
		//xRel, yRel, zRelFakePixelsPositiveIsAwayFromCamera, xOrig, yOrig

			//CONVERT TO METERS:
			fDistance/=fDestPixPerUnitHere;
			pointOrig.yRot=fAltitude;
			pointOrig.zRot=fAzimuth;
			Travel3d(pointOrig, pointOrig.yRot, pointOrig.zRot, fDistance);

			//self-moving test dest vars:
			//static float fTestAlt=90.0f, fTestAz=0.0f;
			//static float fTickLastTest=SDL_GetTicks();
			//float fTicksPassed=(float)SDL_GetTicks()-fTickLastTest;
			//static float fAzDegPerTick=360.0f/1000.0f; //360deg in 1s
			//static float fAltDegPerTick=-180.0f/20000.0f; //-180deg in 20s
			//fTickLastTest=(float)SDL_GetTicks();
			//fTestAlt+=fAltDegPerTick*fTicksPassed;
			//fTestAz+=fAzDegPerTick*fTicksPassed;
			//while (fTestAlt<=-90.0f) fTestAlt+=180.0f;
			//while (fTestAz>=360.0f) fTestAz-=360.0f;
			//Travel3d(pointOrig, fTestAlt, fTestAz, fDistance);

			//static int iTest=0;
			//if (iTest<100) {
			//	cout<<"{ fMetersFromCamera:"<<fMetersFromCamera<<"}"<<endl;
			//	cout<<"{ pointOfReferenceForDistanceOnly.x:"<<pointOfReferenceForDistanceOnly.x<<"; pointOfReferenceForDistanceOnly.y:"<<pointOfReferenceForDistanceOnly.y<<" (original value from Point3d by distance) }"<<endl;
			//	cout<<"{ pointReturn.x:"<<pointReturn.x<<"; pointReturn.y:"<<pointReturn.y<<" (original value from Point3d by distance) }"<<endl;
			//}
			//Travel3d(pointReturn,m3dCam,-zFakePixelsPositiveIsAwayFromCamera/DestPixelsPerMeterAt(fMetersFromCamera));
			//if (iTest<100) {
			//	cout<<"{ pointReturn.x:"<<pointReturn.x<<"; pointReturn.y:"<<pointReturn.y<<"; moved toward camera:"<<(zFakePixelsPositiveIsAwayFromCamera/DestPixelsPerMeterAt(fMetersFromCamera))<<"; DestPixelsPerMeterAt(fMetersFromCamera):"<<DestPixelsPerMeterAt(fMetersFromCamera)<<"; }"<<endl;
			//	iTest++;
			//}
		}
		catch (exception& exn) {
			ShowException(exn,"Point3dMoveAndRotateBy2d");
		}
		catch (...) {
			ShowUnknownException("Point3dMoveAndRotateBy2d");
		}
		return bGood;
	}//end Point3dMoveAndRotateBy2d
	bool Camera::Point3dFrom2d(Mass3d &pointReturn, float xFlat, float yFlat, float fMetersFromCamera) {
		bool bGood=true;
		static float fRatioFromLeft,
				fRatioFromTop,
				fYaw,
				fPitch;
		try {
			fRatioFromLeft=xFlat/FSCREEN_WIDTH;
			fRatioFromTop=yFlat/FSCREEN_HEIGHT;
			fYaw=fAngleApertureLeft-fRatioFromLeft*fApertureW;
			fPitch=fAngleApertureTop-fRatioFromTop*fApertureH;
			pointReturn.x=m3dCam.x;
			pointReturn.y=m3dCam.y;
			pointReturn.z=m3dCam.z;
			Travel3d(pointReturn,fPitch,fYaw,fMetersFromCamera);
			static int iTest=0;
		}
		catch (exception& exn) {
			ShowException(exn,"Point3dFrom2d");
		}
		catch (...) {
			ShowUnknownException("Point3dFrom2d");
		}
		return bGood;
	}//end Point3dFrom2d
	bool Camera::Point2dRatiosFrom3d(float &fReturnRightness, float &fReturnDownness, float x3d, float y3d, float z3d) {
	//gets screen point from 3d point
		bool bGood=true;
		try {
			//assumes there is no m3dCam.y rotation
			static float fAngleFromTop; //angle of point from top of aperture
			static float fAngleFromLeft; //angle of point from left of aperture
			static float xRel, yRel, zRel;//points relative to Camera;
			xRel=x3d-m3dCam.x;
			yRel=y3d-m3dCam.y;
			zRel=z3d-m3dCam.z;
			//the next line may assume camera is facing toward positive m3dCam.y
			fAngleFromTop=(fAngleApertureTop-(THETAOFXY(yRel,zRel)));
			fAngleFromLeft=(fAngleApertureLeft-(THETAOFXY(xRel,yRel)));
		    fReturnRightness=fAngleFromLeft/fApertureW;
		    fReturnDownness=fAngleFromTop/fApertureH;
		}
		catch (exception& exn) {
			ShowException(exn,"Point2dRatiosFrom3d");
		}
		catch (...) {
			ShowUnknownException("Point2dRatiosFrom3d");
		}//TODO: finish this: copy catch statements like this to the rest of them
		return bGood;//TODO: return false if not in camera bounds
	}//end Point2dRatiosFrom3d
	void Camera::SetCamera(float xTo, float yTo, float zTo, float xRotTo, float yRotTo, float zRotTo, float fApertureAngleHeight) {
		m3dCam.x=xTo;
		m3dCam.y=yTo;
		m3dCam.z=zTo;
		m3dCam.xRot=xRotTo;
		m3dCam.yRot=yRotTo;
		m3dCam.zRot=zRotTo;
		fApertureW=fApertureAngleHeight*fScreenRatio;
		fApertureH=fApertureAngleHeight;
		fAngleApertureTop=m3dCam.yRot+(fApertureH/2.0f);
		fAngleApertureLeft=m3dCam.zRot+(fApertureW/2.0f);
	}


	#define ENTITY_TYPE_HERO  1
	#define ENTITY_TYPE_SHOT  2
	#define ENTITY_TYPE_ALIEN 3
	#define ENTITY_TYPE_BOSS  4
	//global camera:
	Camera camera;
	///////////////////////////////////////////////////////////
	// void* ENTITY POINTERS GLOBAL TO OTHER ENTITIES ////////////////////////////
	void* lparrShot[MAXSHOTS];
	void** lparrAlien;
	///////////////////////////////////////////////////
	//Entity Class
	class Entity {
	public:
		float fShield;
		bool bHitSoundWasPlayed;
		bool bRotationTakesPrecedence;
		bool bCycleFrames;
		int iBoundaryAction;
		int iTickLastHitGroundSound;
		float fHealthMax;
		float fLaserSusceptibility;
		bool bUsePitchYaw; //only used for SHOT entities as of 2007
		float fMetersPerSecond;//only used if bMoveByMetric
		float fMetersPerSecondMax;
		Mass3d m3dEnt;
		Mass2d m2dEnt;
		Mass2d m2dShadow;
		float fPowerMax;
		float fPowerPerShot;
		bool bAimBomb;
		bool bAlien;
		bool bBomb;
		float fFortitude;
		float fSpeedMultiplier;
		bool bDraw;
		int iType;
		float fShadowOpacity;
		//float fFloatHeight;
		float zRotSpeed;
		float fShootMetersPerSec;
		Anim32BGRA* lpanimMain; // animation cycle (pointer to any global anim)
		Anim32BGRA* lpanimShadow;
		int iFramesMoveDelay; //delays motion by frames
		int iFramesAimDelay;
		int iFramesShootDelay;
		UInt32 dwStatus;
		float fHealth;
		float fExplodedness;
		float fPower; //gun power in percent, max=1
		bool bExploding;
		bool bGravity;
		bool bMustMove;

		void Init();
		Entity(int iTypeX);
		Entity(int iTypeX, Mass3d m3dDirectionAndLocation, float fMetersPerSec, bool IsAlien, bool IsBomb);
		Entity(int iTypeX, float x2, float y2, float z2, float xVel2, float yVel2, float zVel2, bool IsAlien, bool IsBomb);
		Entity(int iTypeX, float x2, float y2, float z2);
		~Entity();
		void Bomb(float xFlat, float yFlat, float zDest);
		void Bomb();
		void SetAsBoss();
		void CheckFrames(); //sets frame animation by direction if alien
		void Refresh();
		void TurnIncrement(bool bPositiveAngle);
		void Turn(float fDeg);
		void TurnPitchIncrement(bool bUp);
		void TurnPitch(float fDeg);
		void TurnAll(float fAltitude, float fAzimuth);
		void Jump();
		void Move(int xDir, int yDir);
		void TurnPitchByFlatPositiveCartesian(float fDeg);
		void Shoot();
		void Shoot(int iDir);
		void Shoot(float fPitch, float fYaw);
		void doublespeed();
		void rapidfire();
		void DrawTarget(int alienNum);
		void GetHit(int iShotIndex);
		void Draw();
		void Translate3D();
		float EyeX();
		float EyeY();
		float EyeZ();
	    void DeformTerrain();
		void SetVars(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, bool IsAlien, bool IsBomb);
		float MetersFromPixelsWhereIAm(float fPixels);
		void Stop();
		bool HasAttrib(UInt32 bit);
		void RemoveAttrib(UInt32 bit);
		void AddVariableShield(float fSetShieldTo);
	private:
		//shot vars:
		int iTickStart;
		int iTicksLife;
		bool bFirstRun;
		int iDir;
		int iTickLastRefresh;
		int iTickLastMetricMove; //only used for metric movement as of 2007

		void DamageDetect();
		void DrawMeters();
		int FrameFromRot();
		void ShowDebugInfo();
		//float BottomOffsetHere();
	};
	void Entity::Init() {
		fShield=0.0f;
		bHitSoundWasPlayed=false;
	    bCycleFrames=false;
	    bGravity=false;
	    iBoundaryAction=BOUNDARY_BOUNCE;
	    iTickLastHitGroundSound=0;
		bExploding=false;
		fExplodedness=0.0f;
		bAlien=false;
		bBomb=false;
		iFramesMoveDelay=0;
		iFramesAimDelay=0;
		iFramesShootDelay=0;
		fExplodedness=0;
		//fFloatHeight=0.0f;
		fLaserSusceptibility=.05;
		iTickLastRefresh=SDL_GetTicks();
		iTickLastMetricMove=SDL_GetTicks();
		bFirstRun=true;
		bAimBomb=false;
		bDraw=true;
		fHealthMax=1.0f;//only used by hero as of 2007
		fPowerMax=1.0f;//changed below
		fPowerPerShot=.45f;
		bUsePitchYaw=false;
		fMetersPerSecond=4;//not used when bUsePitchYaw==false
		fMetersPerSecondMax=fMetersPerSecond;
		m2dEnt.Init(100,100,81.0f);
		bRotationTakesPrecedence=false;
		bMustMove=false;
		m3dEnt.SetRotMaxSpeed(180,180,180);
		m3dEnt.zMin=0.0f;
		fFortitude=1.0f;
		fSpeedMultiplier=1.0f;
	}//end Init()
	Entity::Entity(int iTypeX) {
		Init();
		iType=iTypeX;
		bool bGood=true;
		try {
			if (iType==ENTITY_TYPE_HERO) {
				try {
					m3dEnt.zMin=.8f;
					bRotationTakesPrecedence=true;
					bGravity=true;
					iBoundaryAction=BOUNDARY_STOP;
					//fFloatHeight=0.5f;
					fLaserSusceptibility=.059;//.196;
					dat.GetOrCreate(fLaserSusceptibility,"hero.fLaserSusceptibility");
					fHealthMax=1.0f;
					dat.GetOrCreate(fHealthMax,"hero.hp");
					//ShowError("test");
					//iErrors++;
					fPowerMax=2.0f;
					dat.GetOrCreate(fPowerMax,"hero.maxpow");
					m3dEnt.yRot=0.0f;//45.0f;
					m3dEnt.yRotDest=m3dEnt.yRot;
					m3dEnt.yRotMin=0.0f;//-60.0f; //UNUSED NOW 2007
					m3dEnt.yRotMax=360.0f;//90.0f; //UNUSED NOW 2007
					zRotSpeed=9.0f;
					m3dEnt.zRot=90.0f;//-180.0f;
					m3dEnt.zRotDest=m3dEnt.zRot;
					m3dEnt.zRotMin=0.0f;//-190.0f; //UNUSED NOW 2007
					m3dEnt.zRotMax=360.0f;//10.0f; //UNUSED NOW 2007
					fFortitude=0.2f;
					fShootMetersPerSec=6.6f;
					dat.GetOrCreate(fShootMetersPerSec, "hero.shot.meterspersec");
					m3dEnt.xMin=-6;
					m3dEnt.xMax=10;
					m3dEnt.yMin=-8;
					m3dEnt.yMax=8;
					m3dEnt.zMax=5.0f;
					dat.GetOrCreate(m3dEnt.xMin, "hero.bounds.xmin");
					dat.GetOrCreate(m3dEnt.xMax, "hero.bounds.xmax");
					dat.GetOrCreate(m3dEnt.yMin, "hero.bounds.ymin");
					dat.GetOrCreate(m3dEnt.yMax, "hero.bounds.ymax");
					dat.GetOrCreate(m3dEnt.zMax, "hero.bounds.zmax");
					m3dEnt.x=0;//(SCREEN_WIDTH/2 - m2dEnt.rectOriginal.right/2)-21;
					m3dEnt.y=m3dEnt.yMin;//(SCREEN_HEIGHT*.75 -height/2)-90;//m3dEnt.y=SCREEN_HEIGHT*.75 - height/2;
					m3dEnt.z=4; //starts falling from air
					dat.GetOrCreate(m3dEnt.x, "hero.start.x");
					dat.GetOrCreate(m3dEnt.y, "hero.start.y");
					dat.GetOrCreate(m3dEnt.z, "hero.start.z");
					m3dEnt.xSize=.4f;
					m3dEnt.ySize=.4f;
					m3dEnt.zSize=1.8f;//1.6f;
					dat.GetOrCreate(m3dEnt.xSize, "hero.size.x");
					dat.GetOrCreate(m3dEnt.ySize, "hero.size.y");
					dat.GetOrCreate(m3dEnt.zSize, "hero.size.z");
					dwStatus=STATUS_ALIVE;// | STATUS_ANTIGRAVITY;
					fHealth=fHealthMax;
					m3dEnt.zRotDest=m3dEnt.zRot;
					m3dEnt.zVel=0;
					m3dEnt.xVel=0;
					m3dEnt.yVel=0;
					fPower=fPowerMax;
					fShadowOpacity=.35;
					dat.GetOrCreate(fShadowOpacity,"hero.shadow.multiplier");

					lpanimMain=&animHero;
					m2dEnt.Init(lpanimMain->Width(),lpanimMain->Height(),81.0f);
					settings.GetOrCreate(m2dEnt.fPixelsPerMeter,"hero.size.pixelspermeter");
					m2dEnt.SetHitRect(27,100,179,128); //top, left, bottom, right
					m2dEnt.fScale=1.0f;
					m2dEnt.SetPixCenter(104,(int)m2dEnt.FCenterYRelScaled());

					lpanimShadow=&animHeroShadow;
					m2dShadow.Init(lpanimShadow->Width(),lpanimShadow->Height(),m2dEnt.fPixelsPerMeter);

					if (lpanimMain!=null) lpanimMain->GotoFrame(FrameFromRot());
				}
				catch (int iExn) { if (ShowError()) cerr<<"int exception in Entity constructor: "<<iExn<<endl; bGood=false; }
				catch (string sExn) { if (ShowError()) cerr<<"Exception error in Entity constructor: "<<sExn<<endl; bGood=false; }
				catch (...) { if (ShowError()) cerr<<"Unknown Exception in Entity constructor."<<endl; bGood=false; }
			}//end if ENTITY_TYPE_HERO
			else if (iType==ENTITY_TYPE_SHOT) {
				Console.Error.WriteLine("Created a useless shot using no constructor parameters.");
			}//end if ENTITY_TYPE_SHOT
			else if (iType==ENTITY_TYPE_ALIEN) {
				Console.Error.WriteLine("Created a useless alien using no constructor parameters.");
			}//end if ENTITY_TYPE_ALIEN
	  }
	  catch (string sExn) {
		if (ShowError()) cerr<<"Exception in entity constructor by type: "<<sExn<<endl;
	  }
	  catch (...) {
		if (ShowError()) cerr<<"Unknown Exception in entity constructor by type."<<endl;
	  }
	//TODO: this constructor isn't used except for hero.  move to an overload of init and make that the init overload that contains the init code
	  m3dEnt.zRotDest=m3dEnt.zRot;
	}//END CONSTRUCTOR BY iTypeX
	Entity::Entity(int iTypeX, Mass3d m3dDirectionAndLocation, float fMetersPerSec, bool IsAlien, bool IsBomb) {
		Init();
		bool bGood=true;
		iType=iTypeX;
		bUsePitchYaw=true;
		fMetersPerSecond=fMetersPerSec;
		fMetersPerSecondMax=fMetersPerSecond;
		m3dEnt.HardLocation(m3dDirectionAndLocation.x,m3dDirectionAndLocation.y,m3dDirectionAndLocation.z);
		m3dEnt.HardRotation(m3dDirectionAndLocation.xRot,m3dDirectionAndLocation.yRot,m3dDirectionAndLocation.zRot);
		if (iType==ENTITY_TYPE_SHOT) {
	        bCycleFrames=true;
	        iBoundaryAction=BOUNDARY_DIE;
			try {
				dwStatus=STATUS_ALIVE;
				fShadowOpacity=.25;
				m3dEnt.xMin=-10;
				m3dEnt.xMax=10;
				m3dEnt.yMin=-12;
				m3dEnt.yMax=30;
				m3dEnt.zMax=40.0f;
				iTickStart=SDL_GetTicks();
				iTicksLife=7000;

				m3dEnt.xSize=.25;
				m3dEnt.zSize=.25;
				m3dEnt.ySize=.25;

				lpanimMain=&animShot;
	 			m2dEnt.Init(lpanimMain->Width(),lpanimMain->Height(),400.0f);
				settings.GetOrCreate(m2dEnt.fPixelsPerMeter,"shot.size.pixelspermeter");
				//m2dEnt.SetHitRect(19,11,58,252);
				//m2dEnt.fScale=1.0f;
				//m2dEnt.SetPixCenter(104,(int)m2dEnt.FCenterYRelScaled());

				lpanimShadow=&animShotShadow;
	 			m2dShadow.Init(lpanimShadow->Width(),lpanimShadow->Height(),m2dEnt.fPixelsPerMeter);

				if (lpanimMain!=null) lpanimMain->GotoFrame(IRandPositive()%lpanimMain->IFrames());

				bAlien=IsAlien;
				bBomb=IsBomb;
				dat.GetOrCreate(fShadowOpacity,"shot.shadow.multiplier");
				dat.GetOrCreate(iTicksLife,"shot.life.msec");
			}
			catch (int iExn) { if (ShowError()) cerr<<"int exception in Entity constructor using Shot angle and Shot Variables: "<<iExn<<endl; bGood=false; }
			catch (string sExn) { if (ShowError()) cerr<<"Exception error in Entity constructor using Shot angle and Shot Variables: "<<sExn<<endl; bGood=false; }
			catch (...) { if (ShowError()) cerr<<"Unknown Exception in Entity constructor using Shot angle and Shot Variables."<<endl; bGood=false; }
		}//end if ENTITY_TYPE_SHOT
	  m3dEnt.zRotDest=m3dEnt.zRot;
	}//end shot constructor by Mass3d
	Entity::Entity(int iTypeX, float x2, float y2, float z2, float xVel2, float yVel2, float zVel2, bool IsAlien, bool IsBomb) {
		Init();
		iType=iTypeX;
		bool bGood=true;
		if (iType==ENTITY_TYPE_SHOT) {
	        bCycleFrames=true;
	        iBoundaryAction=BOUNDARY_DIE;
			try {
				dwStatus=STATUS_ALIVE;
				fShadowOpacity=.25;
				m3dEnt.xMin=-10;
				m3dEnt.xMax=10;
				m3dEnt.yMin=-12;
				m3dEnt.yMax=30;
				m3dEnt.zMax=40.0f;
				iTickStart=SDL_GetTicks();
				iTicksLife=7000;
				m3dEnt.xSize=.25;
				m3dEnt.zSize=.25;
				m3dEnt.ySize=.25;

				lpanimMain=&animShot;
	 			m2dEnt.Init(lpanimMain->Width(),lpanimMain->Height(),400.0f);
				settings.GetOrCreate(m2dEnt.fPixelsPerMeter,"shot.size.pixelspermeter");
				//m2dEnt.SetHitRect(19,11,58,252);
				//m2dEnt.fScale=1.0f;
				//m2dEnt.SetPixCenter(104,(int)m2dEnt.FCenterYRelScaled());

				lpanimShadow=&animShotShadow;
	 			m2dShadow.Init(lpanimShadow->Width(),lpanimShadow->Height(),m2dEnt.fPixelsPerMeter);

				if (lpanimMain!=null) lpanimMain->GotoFrame(IRandPositive()%lpanimMain->IFrames());
				m3dEnt.x=x2;
				m3dEnt.y=y2;
				m3dEnt.z=z2;
				m3dEnt.xVel=xVel2;
				m3dEnt.yVel=yVel2;
				m3dEnt.zVel=zVel2;
				bAlien=IsAlien;
				bBomb=IsBomb;
				dat.GetOrCreate(fShadowOpacity,"shot.shadow.multiplier");
				dat.GetOrCreate(iTicksLife,"shot.life.msec");
			}
			catch (int iExn) { if (ShowError()) cerr<<"int exception in Entity constructor using Shot angle and Shot Variables: "<<iExn<<endl; bGood=false; }
			catch (string sExn) { if (ShowError()) cerr<<"Exception error in Entity constructor using Shot angle and Shot Variables: "<<sExn<<endl; bGood=false; }
			catch (...) { if (ShowError()) cerr<<"Unknown Exception in Entity constructor using Shot angle and Shot Variables."<<endl; bGood=false; }
		}//end if ENTITY_TYPE_SHOT
	  m3dEnt.zRotDest=m3dEnt.zRot;
	}//END CONSTRUCTOR BY SHOT ANGLE AND SHOT VARIABLES
	Entity::Entity(int iTypeX, float x2, float y2, float z2) {
		Init();
		iType=iTypeX;
		bool bGood=true;
		if (iType==ENTITY_TYPE_ALIEN) {
			try {
				fHealthMax=.118;
				dat.GetOrCreate(fHealthMax, "flyer.hp");
				fHealthMax*=(float)iEncounter;
			    bAimBomb=false;
				fShootMetersPerSec=1;
				bAlien=true;
				fShadowOpacity=.39f;
				dat.GetOrCreate(fShadowOpacity,"flyer.shadow.multiplier");
				m3dEnt.xMin=-8;
				m3dEnt.xMax=12;
				m3dEnt.yMin=-8;
				m3dEnt.yMax=8;
				m3dEnt.zMax=6.0f;
				iAliens++;
				srand(SDL_GetTicks());
				dwStatus=STATUS_ALIVE;
				fHealth=fHealthMax;
				dat.GetOrCreate(fShootMetersPerSec, "flyer.shot.meterspersec");
				m3dEnt.x=x2;
				m3dEnt.y=y2;
				m3dEnt.z=z2;
	   			m3dEnt.LocationToLimits();
				m3dEnt.xSize=2.0f;
				m3dEnt.ySize=1.0f;
				m3dEnt.zSize=.25f;
				dat.GetOrCreate(m3dEnt.xSize, "flyer.size.x");
				dat.GetOrCreate(m3dEnt.ySize, "flyer.size.y");
				dat.GetOrCreate(m3dEnt.zSize, "flyer.size.z");
				//m3dEnt.xVel=(float)(iEncounter+1)*(float)(IRand()%3+1)/30.0f;
				//m3dEnt.yVel=-(float)(IRand()%4)*.01;
				//m3dEnt.zVel=-(float)(IRand()%4)*.01;
				for (int iX=0; iX<22; iX++) {
			    	IRandPositive();//iterate a couple times for extra "randomness"
				}
				fMetersPerSecondMax=(float)iEncounter * ((float)IRand(1,4)*.25f+2.0f);
				fMetersPerSecond=fMetersPerSecondMax;
				if (fMetersPerSecondMax<2.0f) fMetersPerSecondMax=2.0f;
				m3dEnt.yRot=(float)IRand(80,400);
				m3dEnt.yRot=m3dEnt.yRotDest;
				m3dEnt.xRot=0;
				m3dEnt.zRot=(float)IRand(0,360);
				m3dEnt.zRotDest=m3dEnt.zRot;
				bUsePitchYaw=true;

				iFramesShootDelay=20;


				lpanimMain=&animFlyer;
	 			m2dEnt.Init(lpanimMain->Width(),lpanimMain->Height(),150.0f);
				settings.GetOrCreate(m2dEnt.fPixelsPerMeter,"flyer.size.pixelspermeter");
				m2dEnt.SetHitRect(107,91,159,254); //TODO: finish this: set hit rect automatically using image,  EVERY TIME SetHitRect is called
				//m2dEnt.fScale=1.0f;
				//m2dEnt.SetPixCenter(104,(int)m2dEnt.FCenterYRelScaled());

				lpanimShadow=&animFlyerShadow;
	 			m2dShadow.Init(lpanimShadow->Width(),lpanimShadow->Height(),m2dEnt.fPixelsPerMeter);

				if (iArea==2) dwStatus |= STATUS_SHOOTER;
				else if (iArea==3) dwStatus |= STATUS_BOMBER;
				if (iArea==3 && iEncounter==3) dwStatus |= STATUS_SHIELD;
	        	fLaserSusceptibility=.0265;//.0265f;
				dat.GetOrCreate(fLaserSusceptibility,"flyer.fLaserSusceptibility");
			}
			catch (int iExn) { if (ShowError()) cerr<<"int exception in Entity constructor using location: "<<iExn<<endl; bGood=false; }
			catch (string sExn) { if (ShowError()) cerr<<"Exception error in Entity constructor using location: "<<sExn<<endl; bGood=false; }
			catch (...) { if (ShowError()) cerr<<"Unknown Exception in Entity constructor using location."<<endl; bGood=false; }
		}//end if ENTITY_TYPE_ALIEN
	  m3dEnt.zRotDest=m3dEnt.zRot;
	}//end constructor by location
	Entity::~Entity() {
	}
	////////////////////           end constructors          /////////////////////////////
	void Entity::SetAsBoss() {
		if (iType==ENTITY_TYPE_BOSS) {
			static bool bShown=false;
			if (!bShown) {
				if (ShowError()) cerr<<"Error in SetAsBoss--type is already boss."<<endl;
				bShown=true;
			}
			return;
		}
		iType=ENTITY_TYPE_BOSS;
		bCycleFrames=true;
		m3dEnt.xSize=4.7f;
		m3dEnt.ySize=4.7f;
		m3dEnt.zSize=2.0f;//1.2f;
		dat.GetOrCreate(m3dEnt.xSize, "boss.size.x");
		dat.GetOrCreate(m3dEnt.zSize, "boss.size.z");
		dat.GetOrCreate(m3dEnt.ySize, "boss.size.y");
		try {
			try {
				try {
					fHealthMax=4.0f;
					dat.GetOrCreate(fHealthMax,"boss.maxhealth");

					lpanimMain=&animBoss;
					m2dEnt.Init(lpanimMain->Width(),lpanimMain->Height(),80.0f);
					settings.GetOrCreate(m2dEnt.fPixelsPerMeter,"boss.size.pixelspermeter");
					m2dEnt.SetHitRect(5,15,121,282);
					//m2dEnt.fScale=1.0f;
					//m2dEnt.SetPixCenter(104,(int)m2dEnt.FCenterYRelScaled());

					lpanimShadow=&animBossShadow;
					m2dShadow.Init(lpanimShadow->Width(),lpanimShadow->Height(),m2dEnt.fPixelsPerMeter);

					dwStatus=STATUS_ALIVE | STATUS_BOMBER | STATUS_SHIELD | STATUS_BOSS | STATUS_SHOOTER | STATUS_AIMBOMBS;

					m3dEnt.xMin=-8.0f;
					m3dEnt.xMax=12.0f;
					m3dEnt.yMin=-8.0f;
					m3dEnt.yMax=12.0f;
					m3dEnt.zMax=7.0f;
					m3dEnt.x=0;m3dEnt.y=0;m3dEnt.z=m3dEnt.zMax;
					dat.GetOrCreate(m3dEnt.x, "boss.start.x");
					dat.GetOrCreate(m3dEnt.y, "boss.start.y");
					dat.GetOrCreate(m3dEnt.z, "boss.start.z");
					m3dEnt.xVel=.1f;
					m3dEnt.yVel=.04f;
					m3dEnt.zVel=-.0333f;
					fMetersPerSecond=4.0;
					bUsePitchYaw=false;//TODO: change vars and this
					dat.GetOrCreate(m3dEnt.xVel, "boss.start.velocity.x");
					dat.GetOrCreate(m3dEnt.yVel, "boss.start.velocity.y");
					dat.GetOrCreate(m3dEnt.zVel, "boss.start.velocity.z");
					fShootMetersPerSec=6.6f;
					dat.GetOrCreate(fShootMetersPerSec,"boss.shot.meterspersec");
					fHealth=fHealthMax;
					iPlayAngryAlien=SOUNDPAN_FROM3DX(m3dEnt.x);
					iFramesShootDelay=4;
					fLaserSusceptibility=.05;//.0265;
					dat.GetOrCreate(fLaserSusceptibility,"boss.fLaserSusceptibility");
					fShadowOpacity=.35;
					dat.GetOrCreate(fShadowOpacity,"boss.shadow.multiplier");
					settings.Save();
				}
				catch (string sExn) {
					if (ShowError()) cerr<<"Exception trying to initialize boss: "<<sExn<<endl;
				}
				catch (...) {
					if (ShowError()) cerr<<"Exception trying to initialize boss"<<endl;
				}
			}
			catch (string sExn) {
				if (ShowError()) cerr<<"Exception trying to create boss: "<<sExn<<endl;
			}
			catch (...) {
				if (ShowError()) cerr<<"Exception trying to create boss"<<endl;
			}
		}
		catch (string sExn) {
			if (ShowError()) cerr<<"Exception trying to enable boss: "<<sExn<<endl;
		}
		catch (...) {
			if (ShowError()) cerr<<"Unknown Exception trying to enable boss."<<endl;
		}
	}//end SetAsBoss
	void Entity::CheckFrames() {
		if (!bRotationTakesPrecedence && !bUsePitchYaw) {
			m3dEnt.zRot=THETAOFXY(m3dEnt.xVel,m3dEnt.yVel);
			m3dEnt.zRotDest=m3dEnt.zRot;
		}
		if (!bCycleFrames) {
			if (lpanimMain!=null) lpanimMain->GotoFrame(FrameFromRot());
		}
	}
	void TurnAll(float fAltitude, float fAzimuth) {
		//TODO: finish this
	}
	int Entity::FrameFromRot() {
		int iFrameTest;
		float fDeg=SafeAngle360(m3dEnt.zRot);
		if (fDeg<=90.0f) iFrameTest=(int)APPROACH(0,15,fDeg/90.0f);
		else if (fDeg<=180.0f) iFrameTest=(int)APPROACH(15,30,(fDeg-90.0f)/90.0f);
		else if (fDeg<=270.0f) iFrameTest=(int)APPROACH(30,45,(fDeg-180.0f)/90.0f);
		else {
			iFrameTest=(int)APPROACH(45,60,(fDeg-270.0f)/90.0f);
		}
		if (iFrameTest<0) iFrameTest=0;
		else if (lpanimMain==null || iFrameTest>=lpanimMain->IFrames()) iFrameTest=0;
		//static int iTest=0;
		//if (iTest<1000 && iType==ENTITY_TYPE_ALIEN) {
		//	cout<<"FrameFromRot "<<m3dEnt.zRot<<" (cropped to "<<fDeg<<") is "<<iFrameTest<<"."<<endl;
		//	iTest++;
		//}
		return iFrameTest;
	}
	void Entity::TurnIncrement(bool bPositiveAngle) {
		if (bPositiveAngle) m3dEnt.zRotDest=m3dEnt.zRot+DegreesToMove(m3dEnt.zRotVel,SDL_GetTicks()-iTickLastRefresh);
		else m3dEnt.zRotDest=m3dEnt.zRot-DegreesToMove(m3dEnt.zRotVel,SDL_GetTicks()-iTickLastRefresh);
	}
	void Entity::Turn(float fDeg) {
		m3dEnt.zRotDest=SafeAngle360(fDeg);
	}
	void Entity::TurnPitchIncrement(bool bUp) {
		if (bUp) m3dEnt.yRotDest=m3dEnt.yRot+DegreesToMove(m3dEnt.yRotVel,SDL_GetTicks()-iTickLastRefresh);
		else m3dEnt.yRotDest=m3dEnt.yRot-DegreesToMove(m3dEnt.yRotVel,SDL_GetTicks()-iTickLastRefresh);
	}
	void Entity::TurnPitch(float fDeg) {
		m3dEnt.yRotDest=SafeAngle360(fDeg);
		if (m3dEnt.yRotDest<m3dEnt.yRotMin)m3dEnt.yRotDest=m3dEnt.yRotMin;
		else if (m3dEnt.yRotDest>m3dEnt.yRotMax)m3dEnt.yRotDest=m3dEnt.yRotMax;
	}
	void Entity::Jump() {
		if (!iFramesMoveDelay) {
			if ((m3dEnt.z<=m3dEnt.zMin) || (dwStatus & STATUS_ANTIGRAVITY)) {//don't unless not jumping or if ANTIGRAVITY status
				m3dEnt.zVel=(dwStatus & STATUS_ANTIGRAVITY) ? .07f :.42f;//m3dEnt.zVel=40.0f;//debug this
				m3dEnt.z+=m3dEnt.zVel;//push us out of the way of ground detection
				if (dwStatus & STATUS_DOUBLESPEED) m3dEnt.zVel *= 2.0f;
				if (!(dwStatus & STATUS_ANTIGRAVITY)) {//if no antigravity, delay movement in air
					iFramesMoveDelay= (dwStatus & STATUS_DOUBLESPEED) ? 15 : 30;
	               	iPlayJump=SOUNDPAN_FROM3DX(m3dEnt.x);
				}
			}
		}
	}
	void Entity::Move(int xDir, int yDir) {
		static float fDist=.2f;
		if (!iFramesMoveDelay) {
			if (dwStatus&STATUS_DOUBLESPEED) fDist=.4f;
			if (yDir>0) {//Move up (back, +m3dEnt.y)
				if (m3dEnt.y<m3dEnt.yMax) {
					m3dEnt.y+=fDist;
					m3dEnt.yVel=fDist*yDir;
				}
			}
			else if (yDir<0) {//Move down (forward, -m3dEnt.y)
				if (m3dEnt.y>m3dEnt.yMin) {
					m3dEnt.y-=fDist;
					m3dEnt.yVel=fDist*yDir;
				}
			}
			if (xDir<0) {//Move left, -m3dEnt.x
				if (m3dEnt.x>m3dEnt.xMin) {
					m3dEnt.x-=fDist;
					m3dEnt.xVel=fDist*xDir;
				}
			}
			else if (xDir>0) {//Move right +m3dEnt.x
				if (m3dEnt.x<m3dEnt.xMax) {
					m3dEnt.x+=fDist;
					m3dEnt.xVel=fDist*xDir;
				}
			}
		}
	}
	void Entity::TurnPitchByFlatPositiveCartesian(float fAngle) {//for up and down Aim
		static float fAngleReflect, fAngleLimited;
		static bool bLeft;
		fAngleLimited=fAngle;
		if (fAngleLimited>180.0f) fAngleLimited=fAngleLimited-360.0f;
		fAngleReflect=fAngleLimited;
		if (fAngleReflect>90.0f) {fAngleReflect=180.0f-fAngleReflect;bLeft=true;}
		else if (fAngleReflect<-90) {fAngleReflect=-180.0f-fAngleReflect;bLeft=true;}
		else bLeft=false;

		if (fAngleReflect<0) fAngleReflect=0;//pointing downward, so point outward instead
		TurnPitch(fAngleReflect);
		//if (fDeg<m3dEnt.yRot) TurnPitch(false);
		//else  TurnPitch(true);
	}
	void Entity::Shoot() {
		Shoot(m3dEnt.yRot,m3dEnt.zRot);
	}
	void Entity::Shoot(int iDir) {
		if (iType==ENTITY_TYPE_HERO) {
			if ((!iFramesShootDelay) && fPower>=fPowerPerShot) {
				if (dwStatus&STATUS_RAPIDFIRE) {
					iFramesShootDelay=2;
				}
				else {
					fPower-=fPowerPerShot;
					iFramesShootDelay=10;
				}
				//float fTurnedness=(m3dEnt.zRot+90.0f)/90.0f;
				//fTurnedness*=fTurnedness;//make it exponential for better forward Aim
				//float fForwardness=(fTurnedness>0) ? 1.0f-fTurnedness : 1.0f+fTurnedness;
				//float fAimUpness=m3dEnt.yRot/90.0f;
				//float fDownness=1.0f-fAimUpness;
				//if (fDownness<0.0f) fDownness=-fDownness;//okay since fDownness isn't used for m3dEnt.zVel
				//else if (fDownness>1.0f) fDownness=1.0f;
				static Mass3d m3dWeapon;
				m3dWeapon.x=EyeX();
				m3dWeapon.y=EyeY();
				m3dWeapon.z=EyeZ();
				m3dWeapon.zRot=m3dEnt.zRot;
				m3dWeapon.yRot=m3dEnt.yRot;
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (lparrShot[iShotNow] == NULL) {//create a shot at the first available shot pointer
						lparrShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, false, false);
						iPlayLaser=SOUNDPAN_FROM3DX(m3dWeapon.x);
						break;
						//iChanLaser=Mix_PlayChannel(2, mcLaser, 0);//chan, sound, #loops
					}
				}
			}//end if able to shoot
		}//end if ENTITY_TYPE_HERO
		else if (iType==ENTITY_TYPE_ALIEN) {
			if (!iFramesShootDelay) {
				iFramesShootDelay=IRandPositive()%30+10;
				static Mass3d m3dWeapon;
				m3dWeapon.x=m3dEnt.x-m3dEnt.xSize/2.0f;
				m3dWeapon.y=m3dEnt.y;
				m3dWeapon.z=m3dEnt.z;
				m3dWeapon.zRot=(iDir==-1)?180.0f:0;
				m3dWeapon.yRot=0;
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (lparrShot[iShotNow] == NULL) {
						iPlayLaserAlien=SOUNDPAN_FROM3DX(m3dEnt.x);
						lparrShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, bAlien, false);
						break;
					}
				}
			}
		}//end if ENTITY_TYPE_ALIEN

	}
	void Entity::Shoot(float fPitch, float fYaw) {
		if (iType==ENTITY_TYPE_HERO) {
			if ((!iFramesShootDelay) && fPower>=fPowerPerShot) {
				if (dwStatus&STATUS_RAPIDFIRE) {
					iFramesShootDelay=2;
				}
				else {
					fPower-=fPowerPerShot;
					iFramesShootDelay=10;
				}
				//float fTurnedness=(m3dEnt.zRot+90.0f)/90.0f;
				//fTurnedness*=fTurnedness;//make it exponential for better forward Aim
				//float fForwardness=(fTurnedness>0) ? 1.0f-fTurnedness : 1.0f+fTurnedness;
				//float fAimUpness=m3dEnt.yRot/90.0f;
				//float fDownness=1.0f-fAimUpness;
				//if (fDownness<0.0f) fDownness=-fDownness;//okay since fDownness isn't used for m3dEnt.zVel
				//else if (fDownness>1.0f) fDownness=1.0f;

				//static float xVel,yVel,zVel,fDistTarget;
				//fDistTarget=DIST3D(m3dEnt,m3dDest);
				//static Mass3d pointAsVelocity;
				//pointAsVelocity.x=m3dEnt.x;
				//pointAsVelocity.y=m3dEnt.y;
				//pointAsVelocity.z=m3dEnt.z;
				//Travel3d(pointAsVelocity,m3dDest,fShootMetersPerSec/fDistTarget);
				//pointAsVelocity.x-=m3dEnt.x;
				//pointAsVelocity.y-=m3dEnt.y;
				//pointAsVelocity.z-=m3dEnt.z;
				//xNess=fShotSpeed*(FANGLEDIFFPOSITIVE(180,zRotDir)/180.0f*2.0f - 1.0f);
				//yNess=fShotSpeed*(FANGLEDIFFPOSITIVE(90,zRotDir)/1800.0f*2.0f - 1.0f);
				//if (zRotDir>=90.0f&&zRotDir<=270.0f) xNess*=-1.0f;
				static Mass3d m3dWeapon;
				m3dWeapon.x=EyeX();
				m3dWeapon.y=EyeY();
				m3dWeapon.z=EyeZ();
				m3dWeapon.yRot=fPitch;
				m3dWeapon.zRot=fYaw;
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (lparrShot[iShotNow] == NULL) {//create a shot at the first available shot pointer
						lparrShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, false, false);
						iPlayLaser=SOUNDPAN_FROM3DX(m3dWeapon.x);
						break;
						//iChanLaser=Mix_PlayChannel(2, mcLaser, 0);//chan, sound, #loops
					}
				}
			}//end if able to shoot
		}//end if ENTITY_TYPE_HERO
	}
	void Entity::Refresh() {
		// ORDER: //
			//4. MOVE//
			//3. HITDETECT&CHECK LIFE// -this way it is using the m2dEnt.rectRender for hit detection
			//1. SET RECT//
			//2. DRAW//
			//TODO: need to account for pausing in iTicksLastRefresh
		if (true) {//!bFirstRun) {
			// PHYSICS //
			if (bGravity) m3dEnt.zVel-=GRAVITY_METERS_PERSEC_PERFRAME; //only .8m/s/s gravitational acceleration (.8/30perframe), for high jumping //earth gravitational acceleration is 1.8 m/s/s
			if ((SDL_GetTicks()-iTickLastRefresh)>15)
				m3dEnt.RotateTowardDest(SDL_GetTicks()-iTickLastRefresh);
			//m3dEnt.zRot=m3dEnt.zRotDest;
			//if (m3dEnt.yRot!=m3dEnt.yRotDest) m3dEnt.yRot=m3dEnt.yRotDest;
			//if (bRotationTakesPrecedence && m3dEnt.zRot!=m3dEnt.zRotDest) {
			//	 m3dEnt.zRot=m3dEnt.zRotDest;
			//}

			float fSpeedMultNow=fFortitude*fSpeedMultiplier;
			if (HasAttrib(STATUS_BOSS)) fSpeedMultNow=1.0f;


			if (bUsePitchYaw) {
				if ( (bMustMove)||((SDL_GetTicks()-iTickLastMetricMove)>30) ) {//helps division accuracy on modern (fast) processors
					int iTest=0;
					float fDist;
					fDist=MetersToMove(fMetersPerSecond,SDL_GetTicks()-iTickLastMetricMove);//this is right, it is the tick not the diff
					Travel3d(m3dEnt, m3dEnt.yRot, m3dEnt.zRot, fDist*fSpeedMultNow);
					iTickLastMetricMove=SDL_GetTicks();
					bMustMove=false;
				}//end if move (if enough ticks)
			}
			else {
				m3dEnt.x+=m3dEnt.xVel*fSpeedMultNow;
				m3dEnt.y+=m3dEnt.yVel*fSpeedMultNow;
				m3dEnt.z+=m3dEnt.zVel*fSpeedMultNow;
			}


			// HIT EDGES //

			bool bHitEdge=false,bHitMinX=false,bHitMinY=false,bHitMinZ=false,bHitMaxX=false,bHitMaxY=false,bHitMaxZ=false;

			if      (m3dEnt.x<m3dEnt.xMin) { bHitMinX=true; bHitEdge=true; }
			else if (m3dEnt.x>m3dEnt.xMax) { bHitMaxX=true; bHitEdge=true; }
			if      (m3dEnt.y<m3dEnt.yMin) { bHitMinY=true; bHitEdge=true; }
			else if (m3dEnt.y>m3dEnt.yMax) { bHitMaxY=true; bHitEdge=true; }
			if      (m3dEnt.z<m3dEnt.zMin) { bHitMinZ=true; bHitEdge=true; }
			else if (m3dEnt.z>m3dEnt.zMax) { bHitMaxZ=true; bHitEdge=true; }
	 		m3dEnt.LocationToLimits();

			if (bHitEdge) {
				bMustMove=true;
				if (bHitMinZ&&bGravity) {
					m3dEnt.zVel=0;
				}
				switch (iBoundaryAction) {
					case BOUNDARY_STOP:
						if (bHitMinZ) {
							if (!bHitSoundWasPlayed) {
						       	if ((SDL_GetTicks()-iTickLastHitGroundSound)>300) {
									iPlayHitDirt=SOUNDPAN_FROM3DX(m3dEnt.x);
									bHitSoundWasPlayed=true;
									iTickLastHitGroundSound=SDL_GetTicks();
								}
								m3dEnt.z+=(fabs(m3dEnt.z-m3dEnt.zMin)/1.5f);
							}
							m3dEnt.xVel*=.5;
							m3dEnt.yVel*=.5;
							m3dEnt.zVel=0;//stop gravity if on ground
						}
						else {
							bHitSoundWasPlayed=false;
							if (bHitMaxZ) m3dEnt.zVel=0;
						} //TODO: m3dEnt.zVel+=zThrust; //zThrust=GRAVITY_METERS_PERSEC_PERFRAME
						if (bHitMinX) m3dEnt.xVel=0;
						else if (bHitMaxX) m3dEnt.xVel=0;
						if (bHitMinY) m3dEnt.yVel=0;
						else if (bHitMaxY) m3dEnt.yVel=0;
						break;
					case BOUNDARY_BOUNCE:
						/*
						if (bUsePitchYaw) {
							m3dEnt.xVel=0;
							m3dEnt.yVel=0;
							m3dEnt.zVel=0;
							//set the velocity vars using the metric vars -- using them will make bouncing easier
							Travel3d(m3dEnt.xVel, m3dEnt.yVel, m3dEnt.zVel, m3dEnt.yRot, m3dEnt.zRot, fMetersPerSecond);
						}
						*/
						if (bUsePitchYaw) {
							float fDistFlatToHero=FPDIST(p3dHero.x,p3dHero.y,m3dEnt.x,m3dEnt.y);
							float fHeightDiff=p3dHero.z-m3dEnt.z;
							float fPitchToHero=THETAOFXY(fDistFlatToHero,fHeightDiff);
							if (bHitMinZ) {
								//m3dEnt.yRot*=-1;
								m3dEnt.yRotDest=IRand(40,55);
								static int iTest=0;
								if (m3dEnt.yRotDest<40 || m3dEnt.yRotDest>55) if (iTest<100) cerr<<"Hit bottom but new yRot is "<<m3dEnt.yRot<<endl; //debug only
								iTest++;
								//m3dEnt.z=m3dEnt.zMin+.02;//debug only --shouldn't be needed//MetersToMove(SDL_GetTicks()-fMetersPerSecond,iTickLastRefresh);
								//if (m3dEnt.yRot<0.0f)m3dEnt.yRot+=360.0f;
								//if (m3dEnt.yRot>180.0f) m3dEnt.yRot=360.0f-m3dEnt.yRot;
							}
							else if (bHitMaxZ) {
								m3dEnt.yRotDest=fPitchToHero;
							}
							else {
								m3dEnt.yRotDest=fPitchToHero;
								m3dEnt.zRotDest=AngleToward(p3dHero.x, p3dHero.y, m3dEnt.x, m3dEnt.y);
								//TODO: fix this--the following lines should not be needed
								if (bHitMinY && m3dEnt.zRotDest>180) {m3dEnt.zRotDest=360.0f-m3dEnt.zRotDest; SafeAngle360ByRef(m3dEnt.zRotDest);}
								if (bHitMaxY && m3dEnt.zRotDest<180) {m3dEnt.zRotDest=360.0f-m3dEnt.zRotDest; SafeAngle360ByRef(m3dEnt.zRotDest);}
								if (bHitMinX && (m3dEnt.zRotDest>90.0f&&m3dEnt.zRotDest<270)) m3dEnt.zRotDest=MirrorOnYLine(m3dEnt.zRotDest);
								if (bHitMaxX && (m3dEnt.zRotDest<90.0f||m3dEnt.zRotDest>270)) m3dEnt.zRotDest=MirrorOnYLine(m3dEnt.zRotDest);
								/*
								if (bHitMinY||bHitMaxY) {
		                            m3dEnt.zRot-=180.0f-m3dEnt.zRot;
									//m3dEnt.zRot*=-1;//m3dEnt.zRot-(180.0f+m3dEnt.zRot);
								}
								if (bHitMinX||bHitMaxX) {
									m3dEnt.zRot-=180.0f-m3dEnt.zRot; //i.e. minx with 225 becomes 135
								}
								*/
							}
							if (bHitMinZ) {
						        iPlayScrapeGround=SOUNDPAN_FROM3DX(m3dEnt.x);
								if (iType!=ENTITY_TYPE_BOSS) fSpeedMultiplier=0.3;
							}
						}
						else {
							if (bHitMinZ) {
						        iPlayScrapeGround=SOUNDPAN_FROM3DX(m3dEnt.x);
								m3dEnt.zVel=-m3dEnt.zVel;
								if (iType!=ENTITY_TYPE_BOSS) fSpeedMultiplier=0.3;
							}
							else if (bHitMaxZ) m3dEnt.zVel=-m3dEnt.zVel;
							if (bHitMinX) m3dEnt.xVel=-m3dEnt.xVel; //TODO: deform terrain here
							else if (bHitMaxX) m3dEnt.xVel=-m3dEnt.xVel;
							if (bHitMinY) m3dEnt.yVel=-m3dEnt.yVel;
							else if (bHitMaxY) m3dEnt.yVel=-m3dEnt.yVel;
						}
						/*
						if (bUsePitchYaw) {
							m3dEnt.zRot=THETAOFXY(m3dEnt.xVel,m3dEnt.yVel);
							m3dEnt.zRotDest=m3dEnt.zRot;
							m3dEnt.yRotDest=THETAOFXY(m3dEnt.xVel,m3dEnt.zVel);
							m3dEnt.xRotDest=THETAOFXY(m3dEnt.yVel,m3dEnt.zVel);
						}
						*/
						break;
					case BOUNDARY_DIE:
						if (bHitEdge) {
							Stop();
							if (bHitMinZ && HasAttrib(STATUS_ALIVE)) {
								DeformTerrain();
								iPlayHitDirt=SOUNDPAN_FROM3DX(m3dEnt.x);
							}
							RemoveAttrib(STATUS_ALIVE);
						}
						break;
					default:
						break;
				}//end switch
			}//end if bHitEdge
			if (bCycleFrames) {
				if (lpanimMain!=null) {
					if (lpanimMain->LastFrame()) lpanimMain->GotoFrame(0);
					else lpanimMain->GotoNextFrame();
				}
			}
			else CheckFrames(); //DOES set iFrame using FrameFromRot
		}//end if not firstrun
		else bFirstRun=false;
		if (fMetersPerSecond<fMetersPerSecondMax) fMetersPerSecond=APPROACH(fMetersPerSecond,fMetersPerSecondMax,.5);


		//entity-type-specific statements

		if (iType==ENTITY_TYPE_HERO) {
			// UPDATE //
			DrawMeters();
			if (iFramesMoveDelay) iFramesMoveDelay--;
			if (iFramesMoveDelay<0) iFramesMoveDelay=0;
			if (iFramesAimDelay) iFramesAimDelay--;
			if (iFramesShootDelay) iFramesShootDelay--;
			if (fFortitude<1.0f) fFortitude+=.1f;
			if (fFortitude>1.0f) fFortitude=1.0f;
			if (fSpeedMultiplier<1.0f) fSpeedMultiplier+=.1f;
			if (fSpeedMultiplier>1.0f) fSpeedMultiplier=1.0f;

			//re-charge gun
			float fPowerMaxNow=fPowerMax;
			float fChargeAddNow=.033f;
			if (dwStatus & STATUS_DOUBLESPEED) {
				fPowerMaxNow*=2.0f;
				fChargeAddNow*=2.0f;
			}
			fPower+=(fPower<fPowerMaxNow*.66f)?fChargeAddNow:fChargeAddNow*.66f;
			if (fPower>fPowerMaxNow) fPower=fPowerMaxNow;

			// HITDETECT //
			if (fFortitude>=1.0f) DamageDetect();
			// CHECK LIFE //
			if (iType==ENTITY_TYPE_HERO && fHealth<=0) dwStatus=0; //died, stop other special abilities too
			if (!(dwStatus & STATUS_ALIVE)) {
				if (!bExploding) {
					iPlayExplosion=SOUNDPAN_FROM3DX(m3dEnt.x);
					bExploding=true;
				}
				else fExplodedness+=.1f;
				if (fExplodedness>1.0f) fExplodedness=1.0f;
			}
		}//end if ENTITY_TYPE_HERO
		else if (iType==ENTITY_TYPE_ALIEN||iType==ENTITY_TYPE_BOSS) {
			// HITDETECT //
			if (fFortitude<1.0f) fFortitude+=.03f;
			if (fFortitude>1.0f) fFortitude=1.0f;
			if (fSpeedMultiplier<1.0f) fSpeedMultiplier+=.03f;
			if (fSpeedMultiplier>1.0f) fSpeedMultiplier=1.0f;
			if (iFramesShootDelay) iFramesShootDelay--;
			// AI ACTIONS //
			if (dwStatus & STATUS_SHOOTER)
				Shoot();
			else if (dwStatus & STATUS_BOMBER) {
				if (dwStatus&STATUS_AIMBOMBS) bAimBomb=true;
				else Bomb();
			}
			else if (dwStatus & STATUS_BOSS) {//Shoot again other way
				if (dwStatus & STATUS_SHOOTER)
					Shoot();
			}

			// HITDETECT //
			DamageDetect();
			// CHECK LIFE //
			if (fHealth<=0) //died, stop shooting etc.
				dwStatus&=(STATUS_ALIVE|STATUS_SHOOTER|STATUS_BOMBER)^0xFFFFFFFF;

			if (!HasAttrib(STATUS_ALIVE)) {
				if (!bExploding) {
					iPlayExplosion=SOUNDPAN_FROM3DX(m3dEnt.x);
					if (iType==ENTITY_TYPE_ALIEN||iType==ENTITY_TYPE_BOSS) iPlayOuchAlien=SOUNDPAN_FROM3DX(m3dEnt.x);
					bExploding=true;
				}
				else fExplodedness+=.1f;
				if (fExplodedness>1.0f) fExplodedness=1.0f;
			}
		}//end if ENTITY_TYPE_ALIEN
		else if (iType==ENTITY_TYPE_SHOT) {
			if ((iTicksLife>0)&&(SDL_GetTicks()-iTickStart>iTicksLife)) {
		        	RemoveAttrib(STATUS_ALIVE);
			}
		} //end if ENTITY_TYPE_SHOT
		Translate3D();
		iTickLastRefresh=SDL_GetTicks();
		if (iType==ENTITY_TYPE_HERO) {
		    p3dHero.x=m3dEnt.x;
		    p3dHero.y=m3dEnt.y;
		    p3dHero.z=m3dEnt.z;
		}
	}//end Refresh
	void Entity::Draw() {
		bool bShadow=false;
		bool bGood=true;
		try {
			if (lpanimShadow!=NULL
		      && m2dShadow.rectRender.left+SCREEN_OFFSET_X>=0
			  && m2dShadow.rectRender.bottom+SCREEN_OFFSET_Y<BUFFER_HEIGHT
			  && m2dShadow.rectRender.right+SCREEN_OFFSET_X<BUFFER_WIDTH
			  && m2dShadow.rectRender.top+SCREEN_OFFSET_Y>0)
				bShadow=true;
			else bShadow=false;
			if ( m2dEnt.rectRender.left+SCREEN_OFFSET_X>=0
			  && m2dEnt.rectRender.bottom+SCREEN_OFFSET_Y<BUFFER_HEIGHT
			  && m2dEnt.rectRender.right+SCREEN_OFFSET_X<BUFFER_WIDTH
			  && m2dEnt.rectRender.top+SCREEN_OFFSET_Y>0)
				bDraw=true;
			else bDraw=false;
			//the next 2 lines are different for shot
			if (iType==ENTITY_TYPE_SHOT) {
				if (bShadow) {
					SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpanimShadow->gbFrame, SCREEN_OFFSET_X+m2dShadow.rectRender.left, SCREEN_OFFSET_Y+m2dShadow.rectRender.top, fShadowOpacity, 0, 0, m2dShadow.fScale),
						iType,
						"shadow");
				}
				if (bDraw) {
					SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpanimMain->gbFrame, SCREEN_OFFSET_X+m2dEnt.rectRender.left, SCREEN_OFFSET_Y+m2dEnt.rectRender.top, (bAlien?0.5f:1.0f), 0, (bBomb?STATUS_SHIELD:0), m2dEnt.fScale),
						iType,
						"self");
				}
				else dwStatus&=(STATUS_ALIVE^0xFFFFFFFF);
			}
			else if (iType==ENTITY_TYPE_ALIEN||iType==ENTITY_TYPE_BOSS) {
				float fShadowOpacityNow;
				fShadowOpacityNow=(float)fShadowOpacity*(1.2f-(m3dEnt.z/m3dEnt.zMax));
				if (fShadowOpacityNow>fShadowOpacity) fShadowOpacityNow=fShadowOpacity;
				else if (fShadowOpacityNow<0.0f) fShadowOpacityNow=0.0f;
				if (bShadow) {
					SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpanimShadow->gbFrame, SCREEN_OFFSET_X+m2dShadow.rectRender.left, SCREEN_OFFSET_Y+m2dShadow.rectRender.top, fShadowOpacityNow, fExplodedness, 0, m2dShadow.fScale),
							iType,
							"shadow");
				}
				if (bDraw) {
					SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpanimMain->gbFrame, SCREEN_OFFSET_X+m2dEnt.rectRender.left, SCREEN_OFFSET_Y+m2dEnt.rectRender.top, fFortitude*fSpeedMultiplier, fExplodedness, dwStatus, m2dEnt.fScale),
							iType,
							"self");
				}
			}//end if iType==ENTITY_TYPE_ALIEN||iType==ENTITY_TYPE_BOSS
			else if (iType==ENTITY_TYPE_HERO) {
				float fShadowOpacityNow;
				fShadowOpacityNow=(float)fShadowOpacity*(1.2f-(m3dEnt.z/m3dEnt.zMax));
				if (fShadowOpacityNow>fShadowOpacity) fShadowOpacityNow=fShadowOpacity;
				else if (fShadowOpacityNow<0.0f) fShadowOpacityNow=0.0f;
				if (bShadow) {
					//debug only:
					SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpanimShadow->gbFrame, SCREEN_OFFSET_X+m2dShadow.rectRender.left, SCREEN_OFFSET_Y+m2dShadow.rectRender.top, fShadowOpacityNow, 0.0f, 0, m2dShadow.fScale),
							iType,
							"shadow");
					//commented for debug only (crashes WITHOUT warning or output):
					//SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpanimShadow->gbFrame], SCREEN_OFFSET_X+m2dShadow.rectRender.left, SCREEN_OFFSET_Y+m2dShadow.rectRender.top, fShadowOpacityNow, fExplodedness, 0, m2dShadow.fScale),
					//		iType,
					//		"shadow");
				}
				//if (bShadow) {
				//	SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpgbHeroShadow, SCREEN_OFFSET_X+m2dShadow.rectRender.left, SCREEN_OFFSET_Y+m2dShadow.rectRender.top, 1.0f, 0, 0, m2dEnt.fScale),
				//			iType,
				//			"shadow");
				//}
				if (bDraw) {
					SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpanimMain->gbFrame, SCREEN_OFFSET_X+m2dEnt.rectRender.left, SCREEN_OFFSET_Y+m2dEnt.rectRender.top, fFortitude*fSpeedMultiplier, fExplodedness, dwStatus, m2dEnt.fScale),
							iType,
							"self");
				}
			}

			if (bDebug) {
				ShowDebugInfo();
			}
		}
		catch (int iExn) { if (ShowError()) cerr<<"int exception in Entity::Draw: "<<iExn<<endl; bGood=false; }
		catch (string sExn) { if (ShowError()) cerr<<"Exception error in Entity::Draw: "<<sExn<<endl; bGood=false; }
		catch (...) { if (ShowError()) cerr<<"Unknown Exception in Entity::Draw."<<endl; bGood=false; }
		//catch (exception& exn) {
		//	FakeException("Exception Error in Entity::Draw");
		//}
	}//end Draw
	float Entity::EyeX() {
		//float fFrontEccentricityPixels=11;
	   	SafeAngle360ByRef(m3dEnt.zRot);//commented for debug only ////m3dEnt.zRot-=FFLOOR(m3dEnt.zRot/360.0f)*360.0f;
		float fApparentDegFromOrthagonalY=FANGLEDIFFPOSITIVE(90.0f,m3dEnt.zRot);
		float fOutness;
		if (fApparentDegFromOrthagonalY>90.0f) fApparentDegFromOrthagonalY=180.0f-fApparentDegFromOrthagonalY;
		fOutness=fApparentDegFromOrthagonalY;
		if (m3dEnt.zRot>90.0f && m3dEnt.zRot<270.0f) fOutness*=-1;

		if (bDebug) {
			Pixel pixelNow;
			pixelNow.Set(64,0,0,255);
			gbScreen.DrawSubpixelArc(FSCREEN_WIDTH/2+64+SCREEN_OFFSET_X, FSCREEN_HEIGHT/2-64+FSCREEN_OFFSET_Y,
					40, 1, 0,
					0, m3dEnt.zRot,
					pixelNow,
					3,0);
			pixelNow.r=128;
			//pixelNow.g=0;
			//pixelNow.b=0;
			gbScreen.DrawSubpixelArc(FSCREEN_WIDTH/2+64+SCREEN_OFFSET_X, FSCREEN_HEIGHT/2-64+FSCREEN_OFFSET_Y,
					30, 1, 0,
					0, fOutness,
					pixelNow,
					3,0);
			pixelNow.r=0;
			pixelNow.g=128;
			pixelNow.b=0;
			gbScreen.DrawSubpixelArc(FSCREEN_WIDTH/2+64+SCREEN_OFFSET_X, FSCREEN_HEIGHT/2-64+FSCREEN_OFFSET_Y,
					20, 1, 0,
					0, FANGLEDIFFPOSITIVE(m3dEnt.zRot,90.0f),
					pixelNow,
					3,0);
			pixelNow.g=0;
			pixelNow.b=128;
			gbScreen.DrawSubpixelArc( FSCREEN_WIDTH/2+64+SCREEN_OFFSET_X, FSCREEN_HEIGHT/2-64+FSCREEN_OFFSET_Y,
					10, 1, 0,
					0, fApparentDegFromOrthagonalY,
					pixelNow,
					3,0);
		}
		fOutness/=90.0f;
		//if (fOutness>1.0f) fOutness=1.0f;//this should never happen
		float xQuads1And2Modifier=0.0f;
		if ( (m3dEnt.zRot<180&&m3dEnt.zRot>0.0f) || (m3dEnt.zRot<0.0f&&m3dEnt.zRot>-180.0f) ) {
	        xQuads1And2Modifier=MetersFromPixelsWhereIAm(20)*(1.0f-fOutness);
			//TODO: finish this--fix location when pointing "away" from screen
		}
		float fHeadRange=MetersFromPixelsWhereIAm(72.0f);//=m2dEnt.fScale*m3dEnt.xSize*1.0;
		float fEye=m3dEnt.x+(fHeadRange/2.0f)*fOutness+xQuads1And2Modifier;//=m3dEnt.x+fHeadRange*fOutness-.1;
		//fEye=FANGLEDIFFPOSITIVE(90.0f,m3dEnt.zRot)/90.0f;//debug only
		//static int iTest=0; //debug only
		//if (iTest<100) {
		//	cout<<"{"
		//	<<"fApparentDegFromOrthagonalY:"<<fApparentDegFromOrthagonalY<<"; "
		//	<<"fOutness:"<<fOutness<<"; "
		//	<<"fHeadRange:"<<fHeadRange<<"; "
		//	<<"EyeX():"<<fEye<<"; "
		//	<<"}"<<endl;
		//   iTest++;
		//}
		//fEye=m3dEnt.x+xHeadRange*fOutness;
		//fEye=m3dEnt.x;
		return fEye;
	}
	float Entity::EyeY() {
		float fEye;
		fEye=m3dEnt.y;
		return fEye;
	}
	float Entity::EyeZ() {
		return (m3dEnt.z+m3dEnt.zSize/2.0f-MetersFromPixelsWhereIAm(10.0f))+MetersFromPixelsWhereIAm(54.0f);
	}
	void Entity::Translate3D() {
		camera.Mass2dFrom3d(m2dEnt, m2dShadow, m3dEnt);
	}
	void Entity::DrawMeters() {
		//TODO: improve this, don't use low-level writing
		register UInt32* lpdwDest=(UInt32*)gbScreen.byarrData;
		int iStride=gbScreen.iStride;
		register int xFlat=32+SCREEN_OFFSET_X, yFlat=SCREEN_HEIGHT-3+SCREEN_OFFSET_Y;
		register int iNotch;
		float fRatio;
		Pixel pixelNow;
		float fMax,fMaxNow;
		int iMaxNow;
		fMax=fPowerMax*100.0f;
		fMaxNow=(fPower/fPowerMax)*100.0f;
		iMaxNow=IROUNDF(fMaxNow);
		for (iNotch=0; iNotch<=iMaxNow; iNotch++) {
			//pixel=_RGB32BIT(255,55+iX*2,55+iX*2,155+iX);
			for (register int xNow=0; xNow<=13; xNow++) {
				//lpdwDest[xFlat + ((yFlat)*iStride >> 2)]=pixel;
				//lpdwDest[int(iX*100)]=pixel;
				int iVal;
				//for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++){
				    iVal=255-130+xNow*10-iNotch;
				    if (iVal<0) iVal=0;
				    if (iVal>255) iVal=255;
				    pixelNow.Set(dwarrEnergyGrad[iVal]);
				    fRatio=(float)iNotch/fMax+.2;
				    if (fRatio>1.0f) fRatio=1.0f;
				    pixelNow.a=SafeByte(192*fRatio);
				    gbScreen.DrawSubpixelDot(xFlat+xNow,yFlat-iNotch,pixelNow);
					//lpdwDest[(xFlat + xNow) + (yFlat-iNotch)*BUFFER_WIDTH]=dwarrEnergyGrad[iVal];
				//}
			}
		}

		xFlat+=32;
		fMax=fHealthMax*100.0f;
		fMaxNow=(fHealth/fHealthMax)*100.0f;
		iMaxNow=IROUNDF(fMaxNow);
		for (iNotch=0; iNotch<=fMaxNow; iNotch++) {
			//pixel=_RGB32BIT(255,55+iX*2,55+iX*2,155+iX);
			for (register int xNow=0; xNow<=13; xNow++) {
				//lpdwDest[xFlat + ((yFlat)*iStride >> 2)]=pixel;
				//lpdwDest[int(iX*100)]=pixel;
				int iVal;
				//for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++){
				    iVal=255-130+xNow*10-iNotch;
				    if (iVal<0) iVal=0;
				    if (iVal>255) iVal=255;
				    pixelNow.Set(dwarrHealthGrad[iVal]);
				    fRatio=(float)iNotch/fMax+.2;
				    if (fRatio>1.0f) fRatio=1.0f;
				    pixelNow.a=SafeByte(192*fRatio);
				    gbScreen.DrawSubpixelDot(xFlat+xNow,yFlat-iNotch,pixelNow);
					//lpdwDest[(xFlat + xNow) + (yFlat-iNotch)*BUFFER_WIDTH]=dwarrHealthGrad[iVal];
				//}
			}
		}
		int yStart=yFlat-gbSymbolShield.iHeight;
		xFlat+=32;
		if (HasAttrib(STATUS_VARIABLESHIELD)) {
			//TODO: make a non-scaled function for next line
			//GBuffer_FX(gbScreen,gbSymbolShield,xFlat, yStart, ( (fShield<1.0f) ? ((fShield>0.0f)?fShield:0.0f) : 1.0f ), DrawModeBlendAlpha );
			GBuffer_FX_Scaled(gbScreen,gbSymbolShield,xFlat, yStart, ( (fShield<1.0f) ? ((fShield>0.0f)?fShield:0.0f) : 1.0f ), 0.0f, 0, 1.0f);
			xFlat+=gbSymbolShield.iWidth+(int)( (float)gbSymbolShield.iWidth/2.0f );
		}
		try {
			if (iBoss>=0) {
				yStart=yFlat-gbSymbolBossHealth.iHeight;
				float fPixMin=xFlat+32;
				float fPixMax=FSCREEN_WIDTH-fPixMin;
				float yMax=FSCREEN_HEIGHT-10;
				float yMin=FSCREEN_HEIGHT-20;
				fRatio=fBoss/fBossMax;
				//TODO: make a non-scaled function for next line
				//GBuffer_FX(gbScreen, gbSymbolBossHealth, xFlat, yStart, ( (fShield<1.0f) ? ((fRatio>0.0f)?fRatio:0.0f) : 1.0f ), 0.0f, 0, 1.0f );
				GBuffer_FX_Scaled(gbScreen, gbSymbolBossHealth, xFlat, yStart, ( (fShield<1.0f) ? ((fRatio>0.0f)?fRatio:0.0f) : 1.0f ), 0.0f, 0, 1.0f );
				/*
				pixelNow.a=255;
				Pixel pixelEnd;
				pixelEnd.r=pixelNow.r;
				pixelEnd.g=pixelNow.g;
				pixelEnd.b=pixelNow.b;
				pixelEnd.a=pixelNow.a;
				pixelNow.g=SafeByte(192*fRatio);
				pixelNow.b=SafeByte(202*fRatio);
				pixelNow.r=55;
				//pixelEnd.g=128*fRatio;
				gbScreen.DrawSubpixelArc( 128+SCREEN_OFFSET_X, FSCREEN_HEIGHT-64+FSCREEN_OFFSET_Y,
						32, 1, 0,
						0, 360*fRatio,
						pixelNow,
						3,0);
				*/
				//for (float yPix=FSCREEN_HEIGHT-49; yPix<FSCREEN_HEIGHT-30; yPix+=1.0f) {
				//	pixelEnd.a-=10;
				//	gbScreen.DrawSubpixelLine(96+FSCREEN_OFFSET_X,
				//								   yPix+FSCREEN_HEIGHT-32+FSCREEN_OFFSET_Y,
				//								   96+(fRatio*(FSCREEN_WIDTH-128))+FSCREEN_OFFSET_X,
				//								   yPix+FSCREEN_HEIGHT-32+FSCREEN_OFFSET_Y,
				//								   pixelNow,NULL,
				//								   1);
				//}
				/*
				float fRatioRel=0;
				float fModifiedRatio=fPixMax*fRatio;
				for (float xNow=0; xNow<fModifiedRatio; xNow+=1.0f) {
					fRatioRel=xNow/fModifiedRatio;
					pixelNow.r=255*(1.0f-fRatioRel);
					pixelNow.g=0;
					for (float yNow=yMax; yNow>yMin; yNow-=1.0f) {
						pixelNow.b=255.0f*(1.0f-(yMax-yNow)/(yMax-yMin));
						pixelNow.a=192;//((pixelNow.b>192)?192:pixelNow.b)+32;
		    			gbScreen.DrawSubpixelDot(
							xNow+fPixMin+FSCREEN_OFFSET_X, yNow+fPixMin+FSCREEN_OFFSET_Y,
							pixelNow);
					}
				}
				*/
			}//if there is a boss
		}
		catch (...) {
		}
	}
	void Entity::DrawTarget(int alienNum) {
		//for (int alienNow=0; alienNow < iMaxAliens; alienNow++)
		//{
			//if (lparrAlien[alienNum] != NULL)
			//TODO: remove or fix calling function for new aiming
			int iX=alienNum;
			if (lparrAlien[iX]==NULL) return;
			Entity* lpAlienNow=(Entity*)lparrAlien[iX];
			float zOurRange=m3dEnt.zSize/2+lpAlienNow->m3dEnt.zSize/2;
			//I switched the right and left edges on purpose to be more inclusive
			//since screen offset prevents bad writes
			if (lpAlienNow->m3dEnt.z-m3dEnt.z<zOurRange && lpAlienNow->m2dEnt.rectRender.right>0 && lpAlienNow->m2dEnt.rectRender.left<SCREEN_WIDTH ) {
				//register UInt32 *lpdwDest =
				register BYTE *byteBuffer=gbScreen.byarrData;//TODO: avoid low-level operations
				static int iStride=gbScreen.iStride;
				//register UInt32 pixel;//=_RGB32BIT(255,255,255,255); //remember this MACRO is ARGB unlike actual screen
				register int xFlat=lpAlienNow->m2dEnt.rectRender.left, yFlat=lpAlienNow->m2dEnt.rectRender.top;
				xFlat+=SCREEN_OFFSET_X;
				yFlat+=SCREEN_OFFSET_Y;
				for (int iX=0; iX<7; iX++) {
					for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++) {
					//draw crosshairs
						byteBuffer[(xFlat+iX)*REAL_BYTEDEPTH + (yFlat+3)*iStride + byteNow]=(byteNow==1)? 255 : 0; //across
						byteBuffer[(xFlat+3)*REAL_BYTEDEPTH + (yFlat+iX)*iStride + byteNow]=(byteNow==1)? 255 : 0; //down
					}
				}

	/*			xFlat=lpAlienNow->m3dEnt.x;
				yFlat=lpAlienNow->m3dEnt.y-2;
				for (iX=0; iX<5; iX++)
				{

					for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
						byteBuffer[(xFlat)*REAL_BYTEDEPTH + (yFlat+iX)*iStride + byteNow]=(byteNow==1)? 255 : 0;
				}
	*/

			}
		//}
	} //end drawtarget
	//TODO: make bombs be able to get hit (btw ignore shield)
	void Entity::GetHit(int iShotIndex) {
		if (iType==ENTITY_TYPE_ALIEN || iType==ENTITY_TYPE_BOSS) {
			if (HasAttrib(STATUS_SHIELD)) {
			//invincible with shield except boss
				if (HasAttrib(STATUS_BOSS)) dwStatus &= (0xffffffff^STATUS_SHIELD); //only Boss loses shield, other shields are removed when boss dies
				fFortitude=.1;
				//fHealth+=0.03f;
				delete (Entity*)lparrShot[iShotIndex];
				lparrShot[iShotIndex]=NULL;
				if (!bExploding) iPlayShieldZap=SOUNDPAN_FROM3DX(m3dEnt.x);
			}
			else {
				fFortitude=.1;
				fHealth-=fLaserSusceptibility;
				delete (Entity*)lparrShot[iShotIndex];
				lparrShot[iShotIndex]=NULL;
				if (!bExploding) iPlayOuchAlien=SOUNDPAN_FROM3DX(m3dEnt.x);
			}
		} //end if ENTITY_TYPE_ALIEN
	}

	void Entity::DamageDetect() {
		try {
			if (iType==ENTITY_TYPE_HERO) {
				//ranges are set to approximate meters
				//scale is NOT used since hit detection is pure 3D
				float xMyRange=m3dEnt.xSize/2,
					yMyRange=m3dEnt.ySize/2,
					zMyRange=m3dEnt.zSize/2;
				//hitdetect
				//HIT ALIEN:
		        Entity* lpAlienNow=NULL;
				for (int iAlien=0; iAlien<iMaxAliensNow; iAlien++) {
					if (lparrAlien[iAlien] != NULL) {
		        		lpAlienNow=(Entity*)lparrAlien[iAlien];
						float xOurRange=lpAlienNow->m3dEnt.xSize/2+xMyRange;
						float yOurRange=lpAlienNow->m3dEnt.ySize/2+yMyRange;
						float zOurRange=lpAlienNow->m3dEnt.zSize/2+zMyRange;
					//CENTERPOINT DISTANCE-BASED
						if ( abs(lpAlienNow->m3dEnt.x-m3dEnt.x) < xOurRange) {
							if ( abs(lpAlienNow->m3dEnt.y-m3dEnt.y) < yOurRange) {
								if (abs(lpAlienNow->m3dEnt.z-m3dEnt.z) < zOurRange) {
									if (HasAttrib(STATUS_SHIELD)) {
										RemoveAttrib(STATUS_SHIELD);
										//make alien blurrp as well as us zap
										fFortitude=.1f;//prevents multiple hits
										iPlayBlerrp=SOUNDPAN_FROM3DX(m3dEnt.x);
										iPlayShieldZap=SOUNDPAN_FROM3DX(m3dEnt.x);
										//fHealth+=fLaserSusceptibility*3.0f;//.196;
									}
									else if (HasAttrib(STATUS_VARIABLESHIELD)) {
										RemoveAttrib(STATUS_SHIELD);
										//make alien blurrp as well as us zap
										iPlayBlerrp=SOUNDPAN_FROM3DX(m3dEnt.x);
										iPlayShieldZap=SOUNDPAN_FROM3DX(m3dEnt.x);
										fFortitude=.1f;//prevents multiple hits
										fShield-=fLaserSusceptibility*3.0f;//.196
										if (fShield<=0.0f) RemoveAttrib(STATUS_VARIABLESHIELD);
									}
									else {
										iPlayBlerrp=SOUNDPAN_FROM3DX(m3dEnt.x);
										iPlayOuchZap=SOUNDPAN_FROM3DX(m3dEnt.x);
										fFortitude=.1f;
										fHealth-=fLaserSusceptibility*3.0f;//.196
									}
								}
							}
						}
					}//end if non-null alien
				}//end for lparrAlien
				//HIT BULLET: edit the shot::DamageDetect to do this instead maybe
				Entity* lpShotNow=NULL;
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (lparrShot[iShotNow] != NULL) {//DISTANCE FORMULA: uses centerpoint (m3dEnt.x,m3dEnt.y,m3dEnt.z)
						lpShotNow=(Entity*)lparrShot[iShotNow];
						float xOurRange=lpShotNow->m3dEnt.xSize/2.0f+xMyRange;
						float yOurRange=lpShotNow->m3dEnt.ySize/2.0f+yMyRange;
						float zOurRange=lpShotNow->m3dEnt.zSize/2.0f+zMyRange;
						if (abs(lpShotNow->m3dEnt.x-m3dEnt.x) < xOurRange) {
							if (abs(lpShotNow->m3dEnt.y-m3dEnt.y) < yOurRange) {
								if (abs(lpShotNow->m3dEnt.z-m3dEnt.z) < zOurRange && (lpShotNow->bAlien)) {
									if (HasAttrib(STATUS_SHIELD)) {
										RemoveAttrib(STATUS_SHIELD);
										fFortitude=.1f; //prevents multiple hits
										//fHealth+=fLaserSusceptibility;//.059;
										delete (Entity*)lparrShot[iShotNow];
										lparrShot[iShotNow]=NULL;
										iPlayShieldZap=SOUNDPAN_FROM3DX(m3dEnt.x);
									}
									else if (HasAttrib(STATUS_VARIABLESHIELD)) {
										fFortitude=.1f; //prevents multiple hits
										fShield-=fLaserSusceptibility*3.0f;//.196//fHealth+=fLaserSusceptibility;//.059;
										if (fShield<=0.0f) RemoveAttrib(STATUS_VARIABLESHIELD);
										delete (Entity*)lparrShot[iShotNow];
										lparrShot[iShotNow]=NULL;
										iPlayShieldZap=SOUNDPAN_FROM3DX(m3dEnt.x);
									}
									else {
										fFortitude=.1f;
										fHealth-=fLaserSusceptibility;//.059;
										delete (Entity*)lparrShot[iShotNow];
										lparrShot[iShotNow]=NULL;
										iPlayOuchZap=SOUNDPAN_FROM3DX(m3dEnt.x);
									}
								}
							}
						}
					}//end if shot not null
				}//end for iShotNow
			}//end if ENTITY_TYPE_HERO
			else if (iType==ENTITY_TYPE_ALIEN || iType==ENTITY_TYPE_BOSS) {
				float xMyRange=m3dEnt.xSize/2,
					yMyRange=m3dEnt.ySize/2,
					zMyRange=m3dEnt.zSize/2;
				Entity* lpShotNow=NULL;
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (lparrShot[iShotNow]!=NULL) {//DISTANCE FORMULA: uses centerpoint (m3dEnt.x,m3dEnt.y,m3dEnt.z)
					    lpShotNow=(Entity*)lparrShot[iShotNow];
						float xOurRange=lpShotNow->m3dEnt.xSize/2.0f+xMyRange;
						float yOurRange=lpShotNow->m3dEnt.ySize/2.0f+yMyRange;
						float zOurRange=lpShotNow->m3dEnt.zSize/2.0f+zMyRange;
						if (abs(lpShotNow->m3dEnt.y-m3dEnt.y) < yOurRange) {
							if (abs(lpShotNow->m3dEnt.x-m3dEnt.x) < xOurRange) {
								if ( (abs(lpShotNow->m3dEnt.z-m3dEnt.z) < zOurRange) && (!lpShotNow->bAlien)) {
									GetHit(iShotNow);
								}
							}
						}
					}
				}
			}//end if ENTITY_TYPE_ALIEN
		}
		catch (string sExn) {
			FakeException("Exception in damage detect\n");
			string sNow=EntityTypeToString(iType);
			if (iErrors<=iMaxErrors) {
				cerr<< sExn << endl << "  --damage detect " << sNow << endl;
				iErrors++;
			}
			else if (iErrors==iMaxErrors) {
				cerr<< sExn << endl << "  (too many errors, this is the last that will be shown)--damage detect " << sNow << endl;
				iErrors++;
			}
		}
		catch (...) {
		}
	}//END DAMAGE DETECT
	void Entity::doublespeed() {
		if (!(dwStatus & STATUS_DOUBLESPEED)) {
			dwStatus |= STATUS_DOUBLESPEED;
			bPlayTrumpet=true;
			m3dEnt.zRotVel*=2.0f;
			m3dEnt.yRotVel*=2.0f;
			fShootMetersPerSec*=2.0f;
		}
	}
	void Entity::rapidfire() {
		if (!(dwStatus & STATUS_RAPIDFIRE)) {
		dwStatus |= STATUS_RAPIDFIRE;
				 bPlayTrumpet=true;
		}
	}
					//TODO: set m3dEnt.SetRotMaxSpeed in Entity::Init
	void Entity::Bomb(float xFlat, float yFlat, float zDest) {
		if (!iFramesShootDelay) {
			if (dwStatus&STATUS_BOSS) iFramesShootDelay=IRand(5,15);
	  		else iFramesShootDelay=IRand(10,20);
			float xEye=m3dEnt.x-m3dEnt.xSize/2.0f;
			if (fShootMetersPerSec<6.0f) fShootMetersPerSec=6.0f;
			float xSpeed,ySpeed,zSpeed;
			float fArrivalFrames=60.0f;
			if (dwStatus&STATUS_BOSS) fArrivalFrames=(float)(IRand(30,60));
			xSpeed=(xFlat-m3dEnt.x)/fArrivalFrames;
			ySpeed=(yFlat-m3dEnt.y)/fArrivalFrames;
			zSpeed=(zDest-m3dEnt.z)/fArrivalFrames;
			for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
				if (lparrShot[iShotNow] == NULL) {
				//create a shot at the first available shot pointer
					iPlayBomb=SOUNDPAN_FROM3DX(m3dEnt.x);
					lparrShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, xEye, m3dEnt.y, m3dEnt.z, xSpeed, ySpeed, zSpeed, bAlien, false);
					break;
				}
			}
		}
	}
	void Entity::Bomb(){
		if (!iFramesShootDelay) {
			if (dwStatus&STATUS_BOSS) iFramesShootDelay=IRand(5,15);
	  		else iFramesShootDelay=IRand(10,20);
			float xEye=m3dEnt.x-m3dEnt.xSize/2.0f;
			for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
				if (lparrShot[iShotNow] == NULL) {
				//create a shot at the first available shot pointer
					iPlayBomb=SOUNDPAN_FROM3DX(m3dEnt.x);
					//Assume Shoot left
					//Prototype: Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isRed2)
	                fShootMetersPerSec=2.0f*(float)(IRand(1,4));
					Mass3d m3dWeapon;
					m3dWeapon.x=m3dEnt.x;
					m3dWeapon.y=m3dEnt.y;
					m3dWeapon.z=m3dEnt.z;
					m3dWeapon.zRotDest=(float)(IRand(0,360));
					m3dWeapon.yRotDest=-(float)(IRand(0,45));
					lparrShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, bAlien, false);
					break;
				}
			}
		}
	}//end Bomb()
	void Entity::DeformTerrain() {
		bool bGood=true;
		try {
			if (lpanimShadow->gbFrame.byarrData==NULL) return;
			static float fScaler=3.0f;
			m2dShadow.fScale*=fScaler;
			int xPushOut;
			xPushOut=(int)((float)(m2dEnt.rectRender.right-m2dEnt.rectRender.left)*fScaler/2.0f);//must be exact
			m2dEnt.rectRender.top-=xPushOut;
			m2dEnt.rectRender.left-=xPushOut;
			m2dEnt.rectRender.bottom=m2dEnt.rectRender.top+(int)(m2dShadow.rectOriginal.bottom*m2dShadow.fScale);//add extra for safety
			m2dEnt.rectRender.right=m2dEnt.rectRender.left+(int)(m2dShadow.rectOriginal.right*m2dShadow.fScale);
			if ( m2dEnt.rectRender.left>=0
			  && m2dEnt.rectRender.bottom<animBackdrop.Height()
			  && m2dEnt.rectRender.right<animBackdrop.Width()
			  && m2dEnt.rectRender.top>=0)
				bDraw=true;
			else bDraw=false;
			if (lpanimShadow->gbFrame.byarrData==NULL) bDraw=false;
			if (bDraw) {
				SayWhatIDrewIfFalse(GBuffer_FX_Scaled(animBackdrop.gbFrame, lpanimShadow->gbFrame, m2dEnt.rectRender.left, m2dEnt.rectRender.top, .078, 0, 0, m2dShadow.fScale),
						iType,
						"turf damage");
			}
		}
		catch (int iExn) { if (ShowError()) cerr<<"int exception in DeformTerrain: "<<iExn<<endl; bGood=false; }
		catch (string sExn) { if (ShowError()) cerr<<"Exception error in DeformTerrain: "<<sExn<<endl; bGood=false; }
		catch (...) { if (ShowError()) cerr<<"Unknown Exception in DeformTerrain."<<endl; bGood=false; }
		//catch (exception& exn) {
		//	FakeException("Exception Error in DeformTerrain");
		//}
	}//end DeformTerrain
	void Entity::SetVars(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, bool IsAlien, bool IsBomb){
		m3dEnt.x=x2;
		m3dEnt.y=y2;
		m3dEnt.z=z2;
		m3dEnt.xVel=xVel2;
		m3dEnt.yVel=yVel2;
		m3dEnt.zVel=zVel2;
		bAlien=IsAlien;
		bBomb=IsBomb;
	}//end SetVars
	void Entity::ShowDebugInfo() {
		Pixel pixelNear, pixelFar;
		pixelNear.Set(0,0,255,200);
		pixelFar.Set(0,0,64,145);
		DrawCube(m3dEnt, pixelNear, pixelFar);

		Mass3d m3dDest;
		m3dDest.x=m3dEnt.x;
		m3dDest.y=m3dEnt.y;
		m3dDest.z=m3dEnt.z;
		if (bUsePitchYaw) {
			Travel3d(m3dDest, m3dEnt.yRot, m3dEnt.zRot, fMetersPerSecond);
			pixelNear.Set(255,0,0,0);
			pixelFar.Set(255,0,0,255);
		}
		else {
			pixelNear.Set(255,255,0,0);
			pixelFar.Set(255,255,0,255);
			m3dDest.x+=m3dEnt.xVel;
			m3dDest.y+=m3dEnt.yVel;
			m3dDest.z+=m3dEnt.zVel;
		}
		FPOINT pointSrc, pointDest;
		camera.Point2dFrom3dWithScreenOffset(pointSrc,m3dEnt);
		camera.Point2dFrom3dWithScreenOffset(pointDest,m3dDest);
		gbScreen.DrawSubpixelLine( pointSrc, pointDest,
			pixelNear, &pixelFar, 1);
	}
	float Entity::MetersFromPixelsWhereIAm(float fPixels) {
		return fPixels/(m2dEnt.fPixelsPerMeter/m2dEnt.fScale);
	}
	//float Entity::BottomOffsetHere() {
	//	return m2dEnt.BottomOffsetRatio() * m3dEnt.zSize * m2dEnt.fScale; //commented for debug only: m2dEnt.BottomOffsetRatio() * m3dEnt.zSize * m2dEnt.fScale;
	//}
	void Entity::Stop() {
		m3dEnt.xVel=0;
		m3dEnt.yVel=0;
		m3dEnt.zVel=0;
	}
	bool Entity::HasAttrib(UInt32 bit) {
		return (dwStatus&bit)!=0;
	}
	void Entity::RemoveAttrib(UInt32 bit) {
	    dwStatus&=(bit^0xFFFFFFFF);
	}
	void Entity::AddVariableShield(float fSetShieldTo) {
		dwStatus |= STATUS_VARIABLESHIELD;
		fShield=fSetShieldTo;
	}
	//TODO: improve boss rect alignment
	//end entity
	//////////////////////      end class entity        //////////////////////////

	// GLOBAL CLASS POINTERS ////////////////////////////////////////////
	Entity* hero=NULL;

	////////////////////////////////////////////////////////////////////////////////


	/* The screen surface */
	SDL_Surface *screen=NULL;
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
	void InitMusic() {
	}
	void StopMusic() {
		//TODO: if (settings.GetForcedBool("music.openal",true) {
		//}
		//else {
			if (music!=NULL) {
				Mix_HaltMusic();
				Mix_FreeMusic(music);
				music=NULL;
			}
		//}
	}
	void PlayMusic(string sFile, int iRepeatsZeroFor1Neg1ForInf) {
		try {
			//TODO: if (settings.GetForcedBool("music.openal",true) {
			//}
			//else {
				music=Mix_LoadMUS(sFile.c_str());
				Mix_PlayMusic(music, iRepeatsZeroFor1Neg1ForInf);
			//}
		}
		catch (...) {
			ShowError("Exception","PlayMusic");
		}
	}
	bool ShutdownMusic() {
		bool bGood=false;
		try {//end of main event loop
			//TODO: if (settings.GetForcedBool("music.openal",true) {
			//}
			//else {
				static bool bClosedAudio=false;
				if (music!=NULL) {
					Mix_HaltMusic();
					Mix_FreeMusic(music);
					music=NULL;
					Console.WriteLine("Stopping Music...done");
				}
			   	else Console.WriteLine("Stopping Music...finished.");
				if (!bClosedAudio) {
					Mix_CloseAudio();
		   			Console.WriteLine("Closing Audio...done.");
		   			bClosedAudio=true;
				}
				else Console.WriteLine("Closing Audio...finished");
			//}
			bGood=true;
		}
		catch (int iExn) { if (ShowError()) cerr<<"int exception in main: "<<iExn<<endl; bGood=false; }
		catch (string sExn) { if (ShowError()) cerr<<"Exception error in main: "<<sExn<<endl; bGood=false; }
		catch (...) { if (ShowError()) cerr<<"Unknown Exception in main."<<endl; bGood=false; }
		return bGood;
	}//end ShutDownMusic wrapper

	void MusicDone() { //runs when music is done
		//TODO: if (settings.GetForcedBool("music.openal",true) {
		//}
		//else {
			if (music!=NULL) {
				//Mix_HaltMusic();
				//Mix_FreeMusic(music);
				//music=NULL;
			}
		//}
	}
	inline void SetPan(int iChan, int iLocation) {
		 float fLoc=(float)iLocation/(float)SCREEN_WIDTH;
		 if (fLoc<0) fLoc=0;
		 else if (fLoc>1) fLoc=1;
		 Mix_SetPanning(iChan, L_FROM_FACTOR(fLoc), R_FROM_FACTOR(fLoc));
	}
	inline void UpdateThrustPan() {
		 SetPan(iChanThruster,SOUNDPAN_FROM3DX(hero->m3dEnt.x));
		 //fThruster=(float)hero->m3dEnt.x/(float)SCREEN_WIDTH;
		 //if (fThruster<0) fThruster=0;
		 //else if (fThruster>1) fThruster=1;
		 //Mix_SetPanning(iChanThruster, L_FROM_FACTOR(fThruster), R_FROM_FACTOR(fThruster));
	}
	void DirKeyDown() {
		 if ((hero!=NULL&&hero->m3dEnt.z<=hero->m3dEnt.zMin) && (hero->dwStatus & STATUS_ANTIGRAVITY)) {
			if (iDirKeysDown<=0) {
				iChanThruster=Mix_PlayChannel(-1, mcThruster, -1);//chan, sound, #loops
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
			iChanThruster=-1;
			iDirKeysDown=0;
		 }
	}
	bool bDone=false;
	SDL_Event event;
	// FUNCTIONS ////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////
	int IRandPositive() {
		static int iMoreRandom=SDL_GetTicks();
		iMoreRandom=SafeAddWrappedPositiveOnly(iMoreRandom,SDL_GetTicks());
		iMoreRandom=SafeAddWrappedPositiveOnly(iMoreRandom,1);
		static int iFibbo=1;
		static int iFibboPrev=0;
		int iReturn=iFibbo;
		int iFibboTemp=iFibbo;
		iFibbo=SafeAddWrappedPositiveOnly(iFibboTemp,iFibboPrev);
		iFibboPrev=iFibboTemp;
		iMoreRandom=SafeAddWrappedPositiveOnly(iMoreRandom,1);
		iReturn=SafeAddWrappedPositiveOnly(iReturn,iMoreRandom);
		return iReturn;
	}
	int IRand(int iMin, int iMax) {
		int iRange=(iMax-iMin);
		int iReturn=(IRandPositive()%(iRange+1));
		if (iReturn<0) iReturn*=-1;
		if (iReturn>iRange) iReturn%=(iRange+1);
		iReturn+=iMin;
		return iReturn;
	}
	float fFibbo=1;
	float fFibboPrev=0;
	void ResetRand() {
		fFibbo=1;
		fFibboPrev=0;
	}
	float FRand() {
		float fReturn=fFibbo;
		float fFibboTemp=fFibbo;
		fFibbo=fFibboTemp+fFibboPrev;
		fFibboPrev=fFibboTemp;
		return fReturn;
	}
	float FRand(float fMin, float fMax) {
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
		//return (IRandPositive()%1)?fMin:fMax;//debug re-implement this
	}
	///////////////////////////////////////////////////////////////



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

	bool GameMain() {
		bool bGood=true;
		try {
		if (bDone) return(0);
		// for now test if user is hitting ESC
		if (iGameState!=GAMESTATE_EXIT) {
			if ((dwPressing&GAMEKEY_EXIT)&&(SDL_GetTicks()-iEscapeTime)>1000) {//int answer=0;
				iEscapeTime=SDL_GetTicks();
				animGameScreen.GotoFrame(2);
				animGameScreen.DrawToLargerWithoutCropElseCancel(gbScreen, (int)( FSCREEN_WIDTH/2.0f-animGameScreen.Width()/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-(float)animGameScreen.Height()/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha);
				DrawScreen();
				iEscapeTime=SDL_GetTicks();
				bool bContinue=true;
				while (bContinue) { //mini polling loop ONE OF FOUR! (confirm exit)
					if(SDL_PollEvent (&event)) {
						if (event.type==SDL_MOUSEBUTTONDOWN
						  || event.key.keysym.sym==SDLK_DELETE
	                      || event.key.keysym.sym==SDLK_UP
	                      || event.key.keysym.sym==SDLK_DOWN
	                      || event.key.keysym.sym==SDLK_LEFT
						  || event.key.keysym.sym==SDLK_RIGHT
						  ) {
							iGameState=GAMESTATE_EXIT;
							bContinue=false;
						}
						else if (event.key.keysym.sym==SDLK_ESCAPE) {
							if((SDL_GetTicks() - iEscapeTime) > 1000) {
								dwPressing&=GAMEKEY_EXIT^0xFFFFFFFF;
								iGameState=GAMESTATE_RUN;
								bContinue=false;
							}
						}
						else if (event.type==SDL_MOUSEMOTION) {
							xCursor=event.motion.x;
							yCursor=event.motion.y;
							xfCursor=xCursor;
							yfCursor=yCursor;
							bMouseMove=true;
							//TODO: DON'T draw cursor since DOESN'T allow click yes/no (fix?)
							//animCursor.GotoFrame(0);
							//animCursor.DrawToLargerWithoutCropElseCancel(gbScreen, xCursor+SCREEN_OFFSET_X-xCursorCenter, yCursor+SCREEN_OFFSET_Y-yCursorCenter, DrawModeBlendAlpha);
						}
					}//end if key is pressed

					SleepWrapper(100);
				}//end while continue to ask confirmation
			}//END IF PRESSING EXIT
		}//end if not already exiting


		if (iGameState == GAMESTATE_INIT) {
			DrawExclusiveRect(0,0,SCREEN_HEIGHT,SCREEN_WIDTH,0x00000000,true);
			gbIntro.DrawToLargerWithoutCropElseCancel(gbScreen,(int)( FSCREEN_OFFSET_X+(FSCREEN_WIDTH/2.0f-(float)gbIntro.iWidth/2.0f) ),SCREEN_OFFSET_Y,DrawModeCopyAlpha);
			DrawScreen();
			Uint32 dwStartTick=SDL_GetTicks();
			//while((SDL_GetTicks() - dwStartTick) < 1000) {
				SleepWrapper(1500);
			//}
			bool bStart=false;
			while (!bStart) { //mini polling loop TWO OF FOUR! (show intro)
				if (SDL_PollEvent (&event)) {
					if (event.key.keysym.sym==SDLK_DELETE
					  || event.key.keysym.sym==SDLK_UP
					  || event.key.keysym.sym==SDLK_DOWN
					  || event.key.keysym.sym==SDLK_LEFT
					  || event.key.keysym.sym==SDLK_RIGHT
					  || event.type==SDL_MOUSEBUTTONDOWN) {
						bStart=true;
						break;
				 	}
				}//end if key is pressed
				SleepWrapper(100);
			}//end while
			DrawExclusiveRect(0,0,SCREEN_HEIGHT,SCREEN_WIDTH,0x00000000,true);

			gbIntroText.DrawToLargerWithoutCropElseCancel(gbScreen, (int)( FSCREEN_OFFSET_X+(FSCREEN_WIDTH/2.0f-(float)gbIntroText.iWidth/2.0f) ), SCREEN_OFFSET_Y, DrawModeCopyAlpha);
			DrawScreen();
			dwStartTick=SDL_GetTicks();
			SleepWrapper(1000);//while((SDL_GetTicks() - dwStartTick) < 2000) ;
			bStart=false;//reset to false in order to show intro text
			while (!bStart) { //mini polling loop THREE OF FOUR! (show intro text)
				if (SDL_PollEvent (&event)) {
					if (event.key.keysym.sym==SDLK_DELETE
	                  || event.key.keysym.sym==SDLK_UP
	                  || event.key.keysym.sym==SDLK_DOWN
	                  || event.key.keysym.sym==SDLK_LEFT
					  || event.key.keysym.sym==SDLK_RIGHT
					  || event.type==SDL_MOUSEBUTTONDOWN) {
						bStart=true;
						break;
					}
					else if (event.type==SDL_MOUSEMOTION) {
						xCursor=event.motion.x;
						yCursor=event.motion.y;
						xfCursor=xCursor;
						yfCursor=yCursor;
						bMouseMove=true;
						//TODO: DON'T draw cursor since DOESN'T allow click yes/no (fix?)
						//animCursor.GotoFrame(0);
						//animCursor.DrawToLargerWithoutCropElseCancel(gbScreen, xCursor+SCREEN_OFFSET_X-xCursorCenter, yCursor+SCREEN_OFFSET_Y-yCursorCenter, DrawModeBlendAlpha);
					}
				}//end if key is pressed
				SleepWrapper(100);
			}//end while showing intro text
			if (iGameState==GAMESTATE_EXIT) {cerr<<"iGameState was exit at start of game"; return(0);}
			try {
				SDL_ShowCursor(SDL_DISABLE);
			}
			catch (int param) { FakeException("int exception hiding cursor in INIT state"); }
			catch (char param) { ShowError(param,"hiding cursor in INIT state (exception)");}
			catch (...) { FakeException("default exception hiding cursor in INIT state"); }
			//catch (exception& exn) {
			//	FakeException("Exception error hiding cursor in INIT state");
			//}
			if (hero!=NULL) {
				delete hero;
				hero=NULL;
			}
			try {
				for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++) {
					if (lparrAlien[iAliensNow]!=NULL) {
						if (iAliensNow==iBoss) iBoss=-1;
						delete (Entity*)lparrAlien[iAliensNow];
						lparrAlien[iAliensNow]=NULL;
					}
				}
			}
			catch (int iExn) { if (ShowError()) cerr<<"int exception initializing alien flyers in INIT state: "<<iExn<<endl; bGood=false; }
			catch (char* sExn) { if (ShowError()) cerr<<"Exception error initializing alien flyers in INIT state: "<<sExn<<endl; bGood=false; }
			catch (...) { if (ShowError()) cerr<<"Unknown Exception initializing alien flyers in INIT state."<<endl; bGood=false; }
			try {
				hero=new Entity(ENTITY_TYPE_HERO);
			}
			catch (int iExn) { if (ShowError()) cerr<<"int exception creating hero in INIT state: "<<iExn<<endl; bGood=false; }
			catch (char* sExn) { if (ShowError()) cerr<<"Exception error creating hero in INIT state: "<<sExn<<endl; bGood=false; }
			catch (...) { if (ShowError()) cerr<<"Unknown Exception creating hero in INIT state."<<endl; bGood=false; }
			iGameState=GAMESTATE_START_AREA;
			iArea=1;
			iEncounter=1;
			dat.GetOrCreate(iArea,"start.area");
			dat.GetOrCreate(iEncounter,"start.encounter");
		}//end if GAMESTATE_INIT
		else if (iGameState == GAMESTATE_START_AREA) {//START AREA
			try {
				animBackdrop.GotoFrame(iArea-1);
			}
			catch (int iExn) { if (ShowError()) cerr<<"int exception in (getting animBackdrop) START_AREA state: "<<iExn<<endl; bGood=false; }
			catch (char* sExn) { if (ShowError()) cerr<<"Exception error in (getting animBackdrop) START_AREA state: "<<sExn<<endl; bGood=false; }
			catch (...) { if (ShowError()) cerr<<"Unknown Exception in (getting animBackdrop) START_AREA state."<<endl; bGood=false; }
			//catch (exception& exn) {
			//	FakeException("error getting animBackdrop");
			//}
			hero->fHealth=1.0f;
			if (iArea==2) hero->dwStatus |= STATUS_ANTIGRAVITY;
			iGameState=GAMESTATE_START_ENCOUNTER;
		}
		else if (iGameState == GAMESTATE_LOSEGUY) {
			SleepWrapper(1000);

			StopMusic();
			iGuys--;
			if (hero==NULL) hero=new Entity(ENTITY_TYPE_HERO);

			animBackdrop.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
			gbGuysDecor.DrawToLargerWithoutCropElseCancel(gbScreen,(int) ( FSCREEN_WIDTH/2.0f-(float)gbGuysDecor.iWidth/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-(float)gbGuysDecor.iHeight+FSCREEN_OFFSET_Y ),DrawModeBlendAlpha);
			int iTotalWidth=SCREEN_WIDTH;
			float fScaleGuy=.5f; //the "0-eth" value
			int iOffsetter;
			while (iTotalWidth>=SCREEN_WIDTH) {
				fScaleGuy*=.5f;
				iOffsetter=(int)((float)hero->m2dEnt.rectOriginal.right*fScaleGuy+(float)hero->m2dEnt.rectOriginal.right*fScaleGuy/2.0f);
				iOffsetter/=2;
				dat.GetOrCreate(iOffsetter,"hero.lives.counter.offsetpersymbol");
				iTotalWidth=iOffsetter*iGuys;
			}
			int iOffset=(SCREEN_WIDTH-iTotalWidth)/2+iOffsetter/2;
			iOffset-=iOffsetter; //shifts if over a bit
			dat.GetOrCreate(iOffset,"hero.lives.counter.offset.x");
			static int yOffset=SCREEN_HEIGHT/2;
			dat.GetOrCreate(yOffset,"hero.lives.counter.offset.y");
			animHero.GotoFrame(0);
			for (int iGuy=0; iGuy<iGuys; iGuy++) {
				GBuffer_FX_Scaled(gbScreen, animHero.gbFrame, SCREEN_OFFSET_X+iOffset, SCREEN_OFFSET_Y+yOffset, 255, 0, 0, fScaleGuy);
				iOffset+=iOffsetter;
			}
			iEncounter=1;//go back to first encounter of this area
			DrawScreen();
			Uint32 dwStartTick=SDL_GetTicks();
			SleepWrapper(4000);//while((SDL_GetTicks() - dwStartTick) < 5000);
			iGameState=GAMESTATE_START_ENCOUNTER;
		}
		else if (iGameState == GAMESTATE_START_ENCOUNTER) {//START ENCOUNTER
			animGameScreenArea.GotoFrame(iArea-1);
			animGameScreenEncounter.GotoFrame(iEncounter-1);
			iMaxAliensNow=iMaxAliens+iArea-1;
			bBombed=false;
			StopMusic();
			ClearSounds();
			iPlayExplosion=128;
			PlaySounds();
			animBackdrop.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
			animGameScreenArea.gbFrame.DrawToLargerWithoutCropElseCancel( gbScreen,
					(int)( FSCREEN_WIDTH/2.0f-animGameScreenArea.Width()/2.0f+FSCREEN_OFFSET_X ),
					(int)( FSCREEN_HEIGHT/2.0f-animGameScreenArea.Height()+FSCREEN_OFFSET_Y ),DrawModeBlendAlpha );
			animGameScreenEncounter.gbFrame.DrawToLargerWithoutCropElseCancel( gbScreen,
				(int)( FSCREEN_WIDTH/2.0f-animGameScreenEncounter.Width()/2.0f+FSCREEN_OFFSET_X ),
				(int)( FSCREEN_HEIGHT/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha );
			DrawScreen();
			Uint32 dwStartScreen=SDL_GetTicks();
			SleepWrapper(3000);//while((SDL_GetTicks() - dwStartScreen) < 3000);

			if (settings.GetForcedBool("music")) {
				PlayMusic("music/Orangejuice-DXMan-Invasion.ogg",-1);
			}
			for (int index=0; index<iMaxAliensNow; index++) {
				//lparrAlien[index]=new Alien(3.0f, (float)index-2.0f, 3.9f);
				lparrAlien[index]=new Entity(ENTITY_TYPE_ALIEN, 3.0f, (float)index-1.0f, 3.9f);
				//lparrAlien[index]->m3dEnt.x=SCREEN_WIDTH;
				//lparrAlien[index]->m3dEnt.y += index*(SCREEN_HEIGHT/64);
				//lparrAlien[index]->m3dEnt.z=index+1;
				//if (iArea==2) lparrAlien[index]->dwStatus |= STATUS_SHOOTER;
				//else if (iArea==3) lparrAlien[index]->dwStatus |= STATUS_BOMBER;
				//if (iArea==3 && iEncounter==3) lparrAlien[index]->dwStatus |= STATUS_SHIELD;
			}

			if (iArea==3) {
				hero->AddVariableShield(1.0f);
				if (iEncounter!=3) ((Entity*)lparrAlien[0])->dwStatus|=STATUS_AIMBOMBS;
			}
			if (iEncounter==3 && iArea==3) {
	            iBoss=( (iMaxAliensNow>=3)?2:(iMaxAliensNow-1) );
				((Entity*)lparrAlien[iBoss])->SetAsBoss();//Set a boss if last encounter of last area
			}
			Console.WriteLine("About to do GAMESTATE_RUN at area "+ToString(iArea)+" encounter "+ToString(iEncounter));
			iGameState=GAMESTATE_RUN;
		}

		else if (iGameState == GAMESTATE_RUN) {
			//if KEYDOWN(65/* a */) hero->dwStatus |= STATUS_ANTIGRAVITY;
			//if KEYUP(65) hero->dwStatus ^= STATUS_ANTIGRAVITY;

			if (dwPressing&GAMEKEY_UP) { hero->Move(0,1); }
			if (dwPressing&GAMEKEY_DOWN) { hero->Move(0,-1);	}
			if (dwPressing&GAMEKEY_LEFT) { hero->Move(-1,0);}
			if (dwPressing&GAMEKEY_RIGHT) { hero->Move(1,0);}
			if (dwPressing&GAMEKEY_UP2) { hero->TurnPitchIncrement(true); }
			if (dwPressing&GAMEKEY_DOWN2) { hero->TurnPitchIncrement(false);	}
			if (dwPressing&GAMEKEY_LEFT2) {
				hero->TurnIncrement(true);
				//if (hero->m3dEnt.yRot< -hero->zRotSpeed) hero->TurnPitchIncrement(true);
				//else if (hero->m3dEnt.yRot> hero->zRotSpeed) hero->TurnPitchIncrement(false);
				//else hero->TurnPitch(0.0f);//this is the destination for the whole "if" clause
			}
			if (dwPressing&GAMEKEY_RIGHT2) {
				hero->TurnIncrement(false);
				//if (hero->m3dEnt.yRot< -hero->zRotSpeed) hero->TurnPitchIncrement(true);
				//else if (hero->m3dEnt.yRot> hero->zRotSpeed) hero->TurnPitchIncrement(false);
				//else hero->TurnPitchIncrement(0.0f);//this is the destination for the whole "if" clause
			}
			if (dwPressing&GAMEKEY_JUMP) { hero->Jump(); }
			if (!(dwPressing&GAMEKEY_FIRE)) hero->iFramesShootDelay=0;
			if (dwPressing&GAMEKEY_FIRE) hero->Shoot();

			if (dwPressing&GAMEKEY_DOUBLESPEED) iDoubleCodeCounter++;//holding '2' activates STATUS_DOUBLESPEED
			else iDoubleCodeCounter=0;

			if (dwPressing&GAMEKEY_RAPIDFIRE) iRapidFireCodeCounter++; //holding 'r' activates STATUS_RAPIDFIRE
			else iRapidFireCodeCounter=0;

			//if KEYUP(80/* p */) keyDelay=0;



			if (iDoubleCodeCounter>=CODE_THRESHOLD && hero->dwStatus ^ STATUS_DOUBLESPEED) hero->doublespeed();
			if (iRapidFireCodeCounter>=CODE_THRESHOLD && hero->dwStatus ^ STATUS_RAPIDFIRE) hero->rapidfire();

			animBackdrop.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
			//DrawRadarField();

			//Update all the existing lparrAlien, delete the rest
			if (hero!=NULL) {
				if (iBoss>=0) {
					try {
			   			fBoss=((Entity*)lparrAlien[iBoss])->fHealth;
				    	fBossMax=((Entity*)lparrAlien[iBoss])->fHealthMax;
					}
					catch (...) {
					}
				}
				hero->Refresh();
				AddScreenItem(SCREENITEM_HERO,ZORDER_FROMY(hero->m3dEnt.y),0);
				//DrawRadarDot(hero->m3dEnt.x, hero->m3dEnt.y, hero->m3dEnt.z+1, 0xFFFFFFFF);
				float
					fHalfH=hero->m3dEnt.ySize/2.0f,
					fHalfW=hero->m3dEnt.xSize/2.0f;
	        	DrawRadarRect(hero->m3dEnt.x-fHalfW,
							hero->m3dEnt.y+fHalfH,
							hero->m3dEnt.y-fHalfH,
							hero->m3dEnt.x+fHalfW,0xFFFFFFFF, true);//electric_blue:0xFF8800FF
				Pixel pixelNow;
				pixelNow.Set(192,192,192,64);
	            gbScreen.DrawSubpixelArc(RadarCenterX()+FSCREEN_OFFSET_X ,RadarCenterY()+FSCREEN_OFFSET_Y,
	                        RadarHeight()/2.0f-2.0f,1,0,
	                        0,360,
							pixelNow,
							.2,0);
				//DrawExclusiveRect(hero->m2dEnt.rectRender.left, hero->m2dEnt.rectRender.top, hero->m2dEnt.rectRender.bottom, hero->m2dEnt.rectRender.right, 0xFFFFFFFF, false);
			}
			int iNulls;
			iNulls=0;
			Entity* lpAlienNow=NULL;
			for (int iAlien=0; iAlien<iMaxAliensNow; iAlien++) {
				if (lparrAlien[iAlien] != NULL) {
					lpAlienNow=(Entity*)lparrAlien[iAlien];
					lpAlienNow->Refresh();
					if (lpAlienNow->bAimBomb && hero)
						lpAlienNow->Bomb(hero->m3dEnt.x,hero->m3dEnt.y,hero->m3dEnt.z);
						//TODO: fix bomb function to use metric speed(?)
					lpAlienNow->bAimBomb=false;

					AddScreenItem(SCREENITEM_ALIEN,ZORDER_FROMY(lpAlienNow->m3dEnt.y),iAlien);
					////hero->DrawTarget(iAlien);
					if (lpAlienNow->dwStatus & STATUS_BOSS) {
						//DrawRadarDot(lpAlienNow->m3dEnt.x, lpAlienNow->m3dEnt.y, lpAlienNow->m3dEnt.z,
						//0x00000000);
						//prototype: void DrawRadarRect(float left, float top, float bottom, float right, Uint32 dwPixel)
						float fHalfH, fHalfW;
						fHalfH=lpAlienNow->m3dEnt.ySize/2.0f;
						fHalfW=lpAlienNow->m3dEnt.xSize/2.0f;
	                	DrawRadarRect(lpAlienNow->m3dEnt.x-fHalfW,
									lpAlienNow->m3dEnt.y+fHalfH,
									lpAlienNow->m3dEnt.y-fHalfH,
									lpAlienNow->m3dEnt.x+fHalfW,0xFF8800FF, false);//electric_blue:0xFF8800FF
					}
					else { //DrawRadarDot(lpAlienNow->m3dEnt.x, lpAlienNow->m3dEnt.y, lpAlienNow->m3dEnt.z,
					  //0xFFFF0000);
					//((lpAlienNow->dwStatus&STATUS_BOSS)?0x00000000:0xFFFF0000) );
						float fHalfH, fHalfW;
						fHalfH=lpAlienNow->m3dEnt.ySize/2.0f;
						fHalfW=lpAlienNow->m3dEnt.xSize/2.0f;
	                	DrawRadarRect(lpAlienNow->m3dEnt.x-fHalfW,
									lpAlienNow->m3dEnt.y+fHalfH,
									lpAlienNow->m3dEnt.y-fHalfH,
									lpAlienNow->m3dEnt.x+fHalfW,0xFFFF0000, true);//electric_blue:0xFF8800FF
					}

					if (lpAlienNow->fExplodedness>=1.0f) {
						if (lpAlienNow->dwStatus & STATUS_BOSS) {
							for (int iAlienToDeShield=0; iAlienToDeShield < iMaxAliensNow; iAlienToDeShield++) {
								if (lparrAlien[iAlienToDeShield]) {
									if (((Entity*)lparrAlien[iAlienToDeShield])->dwStatus & STATUS_SHIELD) {
										((Entity*)lparrAlien[iAlienToDeShield])->dwStatus ^= STATUS_SHIELD;
										((Entity*)lparrAlien[iAlienToDeShield])->fHealth=0;
									}
								}
							}
						}
						iAliens--;
						delete (Entity*)lparrAlien[iAlien];
						if (iAlien==iBoss) iBoss=-1;
						lparrAlien[iAlien]=NULL;
					}
				}
				else iNulls++;
			}

			if (hero->fExplodedness>=1.0f) {
				delete hero;
				hero=NULL;
				if (iGuys<=0) iGameState=GAMESTATE_YOU_LOSE;
				else iGameState=GAMESTATE_LOSEGUY;
			}
			for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
				if (lparrShot[iShotNow] != NULL) {
					((Entity*)lparrShot[iShotNow])->Refresh();
					((Entity*)lparrShot[iShotNow])->Draw();
					AddScreenItem(SCREENITEM_SHOT,ZORDER_FROMY(((Entity*)lparrShot[iShotNow])->m3dEnt.y),iShotNow);
					DrawRadarDot( ((Entity*)lparrShot[iShotNow])->m3dEnt.x, ((Entity*)lparrShot[iShotNow])->m3dEnt.y, 1,
					  ((((Entity*)lparrShot[iShotNow])->bAlien)?0xFF800000:0xFFFFFFFF),false);//A,R,G,B //electric_blue:0xFF8800FF
					if ((((Entity*)lparrShot[iShotNow])->dwStatus&STATUS_ALIVE)==0) {
						delete (Entity*)lparrShot[iShotNow];
						lparrShot[iShotNow]=NULL;
					}
				}
			}
			if (bDebug) {
				Pixel pixelNear;
				Pixel pixelFar;
				pixelNear.Set(0,0,255,150);
				pixelFar.Set(0,0,128,100);
				for (int iNow=0; iNow<9; iNow++) {
			        DrawCube(m3dDebug[iNow], pixelNear, pixelFar);
				}
			}

			Refresh3dCursor();
			int zNow=0;
			int iScreenItemsNow=iScreenItems;
	        //Draw ScreenItems:
			while (iScreenItemsNow>0 && zNow<=MAX_ZORDER) {
				for (int iItem=0; iItem<iScreenItems; iItem++) {
					if (screenitemarr[iItem].zOrder==zNow) {
						switch (screenitemarr[iItem].iType) {
							case SCREENITEM_ALIEN:
								if (NULL!=lparrAlien[ screenitemarr[iItem].index ]) ((Entity*)lparrAlien[ screenitemarr[iItem].index ])->Draw();
								iScreenItemsNow--;
								break;
							case SCREENITEM_SHOT:
								if (NULL!=lparrShot[ screenitemarr[iItem].index ]) ((Entity*)lparrShot[ screenitemarr[iItem].index ])->Draw();
								iScreenItemsNow--;
								break;
							case SCREENITEM_HERO:
								if (NULL!=hero) hero->Draw();
								iScreenItemsNow--;
								break;
							case SCREENITEM_3DCURSOR:
								Draw3dCursor(screenitemarr[iItem].index);
								//iScreenItemsNow--;//commented for debug only
								break;
							default:
								break;
						}
					}//end if matching zOrder
				}//end for iItem
				zNow++;
			}
			iScreenItems=0;
			/*
			if(calculateExplosion(gbScreen, 100, 100, 20, 0, EXPLOSION_CHECK_COUNT)==0)
			{
				explosionResult=calculateExplosion(gbScreen, 100, 100, 20, 30, EXPLOSION_START);
			}
			else
				explosionResult=calculateExplosion(gbScreen, 100, 100, 20, 30, EXPLOSION_CONTINUE);
			//cerr<< "\nResult of explosionCalculate=" << explosionResult;
			*/

			// flip pages:
			DrawScreen();
			//ShowDebugVars();

			if (iAliens<=0 || iNulls==iMaxAliensNow) iGameState=GAMESTATE_WIN_ENCOUNTER;
		}//end GAMESTATE_RUN
		else if (iGameState == GAMESTATE_SHUTDOWN) {
		}
		else if (iGameState == GAMESTATE_WIN_ENCOUNTER) {
			StopMusic();

			if (settings.GetForcedBool("music")) {
				PlayMusic("sound/trumpet.wav", 0);
			}


			for (int iX=0; iX<MAXSHOTS; iX++) {
				if (lparrShot[iX]!=NULL)
				{
					delete (Entity*)lparrShot[iX];
					lparrShot[iX]=NULL;
				}
			}
			int iScreenNow=1;

			if (iArea<3) {
				if (iEncounter<3) {//WIN ENCOUNTER
					iEncounter++;
					iScreenNow=iEncounter+1;//DISPLAY: iEncounter..
					iGameState=GAMESTATE_START_ENCOUNTER;
				}
				else {//WIN AREA

					iArea++;
					iScreenNow=iArea+3;//DISPLAY: iArea.
					iEncounter=1;
					iGameState=GAMESTATE_START_AREA;
				}
			}
			else {
				if (iEncounter<3) {//WIN ENCOUNTER
					iEncounter++;
					iScreenNow=iEncounter+1;
					iGameState=GAMESTATE_START_ENCOUNTER;
				}
				else {//WIN GAME
					iScreenNow=1;
					iGameState=GAMESTATE_WIN_GAME;
				}
			}

			animGameScreen.GotoFrame(iScreenNow);

			if (iScreenNow==1) {
				iGameState=GAMESTATE_WIN_GAME;
			}
			else {
				Uint32 dwStartScreen;
				animGameScreen.GotoFrame(iScreenNow);
				animGameScreen.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, (int)( FSCREEN_WIDTH/2.0f-animGameScreen.Width()/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-animGameScreen.Height()/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha);
				DrawScreen();
				dwStartScreen=SDL_GetTicks();
				SleepWrapper(5000);//while((SDL_GetTicks() - dwStartScreen) < 5000);
			}
		}
		else if (iGameState == GAMESTATE_WIN_GAME) {
			int iScreenNow=1;
			if (iDoubleCodeCounter>=CODE_THRESHOLD || (hero&&hero->dwStatus&STATUS_DOUBLESPEED)) {
				iScreenNow=7;
			}
			if (iRapidFireCodeCounter>=CODE_THRESHOLD|| (hero&&hero->dwStatus&STATUS_RAPIDFIRE)) iScreenNow=8;

			animGameScreen.GotoFrame(iScreenNow);
			animGameScreen.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, (int)( FSCREEN_WIDTH/2.0f-animGameScreen.Width()/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-animGameScreen.Height()/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha);
			DrawScreen();

			StopMusic();

			if (settings.GetForcedBool("music"),true) {
				PlayMusic("music/Orangejuice-DXMan-Ending.ogg",-1);
			}

			Uint32 dwStartTick=SDL_GetTicks();
			SleepWrapper(20000);//while((SDL_GetTicks() - dwStartTick) < 3000);
			bool bExit=false;
			while(!bExit) { //mini polling loop FOUR OF FOUR! (show ending screen)
				if (SDL_PollEvent (&event)) {
					if (event.key.keysym.sym==SDLK_ESCAPE
						|| event.key.keysym.sym==SDLK_DELETE
						|| event.key.keysym.sym==SDLK_UP
						|| event.key.keysym.sym==SDLK_DOWN
						|| event.key.keysym.sym==SDLK_LEFT
						|| event.key.keysym.sym==SDLK_RIGHT
						|| event.type==SDL_MOUSEBUTTONDOWN) {
						bExit=true;
					}
				}
				else if (event.type==SDL_MOUSEMOTION) {
					xCursor=event.motion.x;
					yCursor=event.motion.y;
					xfCursor=xCursor;
					yfCursor=yCursor;
					bMouseMove=true;
					//TODO: DON'T draw cursor since DOESN'T allow click yes/no (fix?)
					//animCursor.GotoFrame(0);
					//animCursor.DrawToLargerWithoutCropElseCancel(gbScreen, xCursor+SCREEN_OFFSET_X-xCursorCenter, yCursor+SCREEN_OFFSET_Y-yCursorCenter, DrawModeBlendAlpha);
				}
			}
			iGameState=GAMESTATE_EXIT;
		}
		else if (iGameState == GAMESTATE_YOU_LOSE) {
			iGuys=iGuysStart;

			StopMusic();

			if (settings.GetForcedBool("music")) {
				PlayMusic("music/Orangejuice-DXMan-Intro.ogg", -1);
			}
			animGameScreen.GotoFrame(0);
			animGameScreen.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, (int)( FSCREEN_WIDTH/2.0f-animGameScreen.Width()/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-animGameScreen.Height()/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha);
			DrawScreen();
			Uint32 dwStartTick=SDL_GetTicks();
			SleepWrapper(5000);//while((SDL_GetTicks() - dwStartTick) < 5000);
			//PostMessage(hwndMain,WM_CLOSE,0,0);
			//PostMessage(hwndMain,WM_QUIT,0,0);
			//window_closed=1;
			iGameState=GAMESTATE_INIT;
		}
		else if (iGameState == GAMESTATE_EXIT) {
	        //TODO: finish this why isn't quit working
			try {
				if (!bSDLQuitDone) {
					SDL_Quit(); //TODO: finish this why isn't quit working
	                bSDLQuitDone=true;
				}
				Console.WriteLine("SDL_Quit...done");
			}
			catch (int iExn) { if (ShowError()) cerr<<"int exception in (trying to run SDL_Quit) EXIT state: "<<iExn<<endl; bGood=false; }
			catch (char* sExn) { if (ShowError()) cerr<<"Exception error in (trying to run SDL_Quit) EXIT state: "<<sExn<<endl; bGood=false; }
			catch (...) { if (ShowError()) cerr<<"Unknown Exception in (trying to run SDL_Quit) EXIT state."<<endl; bGood=false; }
			if (!bDone) {
				Console.WriteLine("GAMESTATE_EXIT...done");
			}
		}//end if GAMESTATE_EXIT
	  }
	  catch (char* sExn) {
			ShowError("sExn","GameMain");
	  }
	  catch (...) {
			ShowError("Unknown exception.","GameMain");
	  }
		// return success or failure or your own return code here
		return(bGood);
	} // end GameMain

	////////////////////////////////////////////////////////////
	void QuitSafe() {
		Console.Write("Running QuitSafe...");
	    if (!bSDLQuitDone) {
			SDL_Quit();
			bSDLQuitDone=true;
			Console.Write("done.");
		}
		else Console.Write("Nothing more to do.");
	}
	bool GameInit() {
		iSplashTick=SDL_GetTicks();//use non-sdl for safety since SDL isn't necessarily initialized
		bool bGood=true;
		Console.Write("Loading debug points...");
		try {
			for (int iNow=0; iNow<9; iNow++) {
		        m3dDebug[iNow].x=0;
		        m3dDebug[iNow].y=0;
		        m3dDebug[iNow].z=0;
		        m3dDebug[iNow].xSize=1.0f;
		        m3dDebug[iNow].ySize=1.0f;
		        m3dDebug[iNow].zSize=1.0f;
			}
			static float fTestRad=4.0f;
			static float fTestDiam=fTestRad*2.0f;
			m3dDebug[0].x-=fTestRad; //see image 1.box-indeces.png
			m3dDebug[0].y+=fTestRad; //see image 1.box-indeces.png
			m3dDebug[0].z+=fTestDiam;
			m3dDebug[1].x+=fTestRad;
			m3dDebug[1].y+=fTestRad;
			m3dDebug[1].z+=fTestDiam;
			m3dDebug[2].x+=fTestRad;
			m3dDebug[2].y-=fTestRad;
			m3dDebug[2].z+=fTestDiam;
			m3dDebug[3].x-=fTestRad;
			m3dDebug[3].y-=fTestRad;
			m3dDebug[3].z+=fTestDiam;
			m3dDebug[4].x-=fTestRad; //see image 1.box-indeces.png
			m3dDebug[4].y+=fTestRad; //see image 1.box-indeces.png
			m3dDebug[5].x+=fTestRad;
			m3dDebug[5].y+=fTestRad;
			m3dDebug[6].x+=fTestRad;
			m3dDebug[6].y-=fTestRad;
			m3dDebug[7].x-=fTestRad; //see image 1.box-indeces.png
			m3dDebug[7].y-=fTestRad; //see image 1.box-indeces.png
			Console.WriteLine("done.");

			//settings.bDebug=true;
			//dat.bDebug=true;
			Console.Write("Loading settings...");
			settings.Load("settings.txt");
			Console.WriteLine("done");
			Console.Write("Loading game data...");
			dat.Load("dat");
			Console.WriteLine("done.");
			Console.Write("Getting vars...");
				//VARS THAT CURRENTLY REQUIRE RESTART IF settings OBJECT IS MODIFIED
			//TODO: (make them not so, by using a Get-set function or other way to cache them.
			dat.GetOrCreate(iMaxAliens,"minaliens");
			if (iMaxAliens<1) iMaxAliens=1;
				//VARS THAT MIGHT NEED TO BE GRABBED IN REAL TIME
			settings.CreateOrIgnore("music","yes");
			settings.CreateOrIgnore("sound","yes");
	        settings.CreateOrIgnore("aim.flip_pitch","yes");
			bFlipPitch=settings.GetForcedBool("aim.flip_pitch");
	        settings.CreateOrIgnore("aim.flip_yaw","yes");
			bFlipYaw=settings.GetForcedBool("aim.flip_yaw");
	        settings.CreateOrIgnore("debugmode","no");
	        bDebug=settings.GetForcedBool("debugmode");
	        //settings.CreateOrIgnore("aim.flip_x","no");
	        //settings.CreateOrIgnore("aim.flip_y","no");
	        //settings.CreateOrIgnore("aim.flip_z","no");
			//xAimFlipper=(settings.GetForcedBool("aim.flip_x"))?-1.0f:1.0f;
			//yAimFlipper=(settings.GetForcedBool("aim.flip_y"))?-1.0f:1.0f;
			//zAimFlipper=(settings.GetForcedBool("aim.flip_z"))?-1.0f:1.0f;

				//VARS THAT DON'T NEED TO BE MODIFIED IN REAL TIME
			int iProgramLoadsCount=dat.GetForcedInt("programloadscount");
			iProgramLoadsCount++;
			dat.SetOrCreate("programloadscount",iProgramLoadsCount);
			dat.GetOrCreate(iGuysStart,"start.lives");
			iGuys=iGuysStart;

			settings.Save();
			dat.Save();
			Console.WriteLine("done.");


			Console.Write("Setting up arrays...");
			lparrAlien=(void**)malloc(sizeof(Entity*)*iMaxAliens);
			string sMsg;
			Console.WriteLine("done");
							/// Initialize SDL ///
			Console.Write("Initializing video...");
			if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
				sMsg="Couldn't initialize SDL:\n";
				sMsg+=ToString(SDL_GetError());
				cerr<<"In GameInit -- "<<sMsg<<endl;
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //re-implement this w/o windows api
				bGood=false;
				return bGood;
			}
			Console.WriteLine("done.");
			atexit(QuitSafe);
			Console.Write("Initializing audio...");
			if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
				sMsg="Couldn't open Audio: \n";
				sMsg+=ToString(SDL_GetError());
				cerr<<"In GameInit -- "<<sMsg<<endl;
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //re-implement this w/o windows api
				return false;
			}
			Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);//what we got
			Console.WriteLine("done.");

			/* Set 640x480 32-bits video mode */
			Console.Write("Setting video mode...");
			screen=SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF);
			if (screen == NULL) {
				sMsg="Couldn't set 640x480x32 video mode: \n";
				sMsg+=ToString(SDL_GetError());
				cerr<<"In GameInit -- "<<sMsg<<endl;
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
				bGood=false;
				return bGood;
			}
			SDL_WM_SetCaption ("DXMan", NULL);
			Console.WriteLine("done.");
			Console.Write("Loading splash screen...");
			GBuffer32BGRA gbSplash;
			if (!gbSplash.Load("images/intro-splash.tga")) {
				sMsg="Failed to load splash screen";
				cerr<<"In GameInit -- "<<sMsg<<endl;
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
			}
			bool bSplashDrawn=false;
			Console.WriteLine("done.");
			iSplashTick=SDL_GetTicks();//use non-sdl for safety since SDL isn't necessarily initialized
			Console.Write("Creating screen...");
			gbScreen.InitNull();
			//if (!
			gbScreen.Init(SCREEN_WIDTH*2, SCREEN_HEIGHT*2,4,true);
			//) {
			//	sMsg="Failed to create screen sprite buffer";
			//	Console.Error.WriteLine("In GameInit -- "+sMsg);
			//	iErrors++;
			//	//MessageBox (0, sMsg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
			//	bGood=false;
			//	return bGood;
			//}
			//else {
			DrawExclusiveRect(0,0,SCREEN_HEIGHT,SCREEN_WIDTH,0xFFFFFFFF,true);
			if ( gbSplash.DrawToLargerWithoutCropElseCancel(gbScreen, (int)( FSCREEN_WIDTH/2.0f-(float)gbSplash.iWidth/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-(float)gbSplash.iHeight/2.0f+FSCREEN_OFFSET_Y ), DrawModeCopyAlpha) ) {
				bSplashDrawn=true;
			}
			//}
			Console.WriteLine("done.");
			Console.Write("Drawing screen...");
			DrawScreen();
			if (bSplashDrawn) bSplash=true;
			Console.WriteLine( ToString("done (") + ((bSplash)?"with":"without") + ToString(" splash).") );
			iSplashTick=SDL_GetTicks();//use non-sdl for safety since SDL isn't necessarily initialized
			//DONE LOADING SPLASH SCREEN
			float fGrad;
			float r1,g1,b1,r2,g2,b2;
			r2=232;	g2=49;	b2=0;
			r1=126;	g1=97;	b1=200;
			for (int iGrad=0; iGrad<256; iGrad++) {
				fGrad=(float)iGrad/255.0f;
				dwarrHealthGrad[iGrad]=_RGB32BIT(255,(BYTE)APPROACH(r2,r1,fGrad),(BYTE)APPROACH(g2,g1,fGrad),(BYTE)APPROACH(b2,b1,fGrad)); //A,R,G,B,
			}
			r1=64;	g1=64;	b1=64;
			r2=126; g2=97;	b2=200;
			for (int iGrad=0; iGrad<256; iGrad++) {
				fGrad=(float)iGrad/255.0f;
				dwarrEnergyGrad[iGrad]=_RGB32BIT(255,(BYTE)APPROACH(r2,r1,fGrad),(BYTE)APPROACH(g2,g1,fGrad),(BYTE)APPROACH(b2,b1,fGrad)); //A,R,G,B,
			}
			Console.Write("Loading sounds...");

			if (settings.GetForcedBool("music")) {
				PlayMusic("music/Orangejuice-DXMan-Intro.ogg", -1);
			}


			mcBomb=Mix_LoadWAV("sound/bomb.wav");
			mcLaserAlien=Mix_LoadWAV("sound/laser-alien.wav");
			mcLaser=Mix_LoadWAV("sound/laser.wav");
			mcExplosion=Mix_LoadWAV("sound/explosion.wav");
			mcOuchAlien=Mix_LoadWAV("sound/ouchalien.wav");
			mcOuchZap=Mix_LoadWAV("sound/ouchzap.wav");
			mcShieldZap=Mix_LoadWAV("sound/shieldzap.wav");
			mcBlerrp=Mix_LoadWAV("sound/blerrp.wav");
			mcHitDirt=Mix_LoadWAV("sound/hitdirt.wav");
			mcJump=Mix_LoadWAV("sound/jump.wav");
			mcScrapeGround=Mix_LoadWAV("sound/scrapeground.wav");
			mcAngryAlien=Mix_LoadWAV("sound/angryalien.wav");

			mcTrumpet=Mix_LoadWAV("sound/trumpet.wav");

			mcThruster=Mix_LoadWAV("sound/thruster.wav");
			Mix_VolumeChunk(mcThruster,40);
			mcLaser=Mix_LoadWAV("sound/laser.wav");
			Mix_VolumeChunk(mcLaser,100);

			//Mix_HookMusicFinished(MusicDone);
			//cout<<("Starting GameInit");

			/*
			*/

			//Load alpha lookup table from file
			//int file_handle;
			Console.WriteLine("done.");

			Console.Write("Loading files in data folder...");
			ifstream ifData("data/lookup-alpha.raw", ios::out | ios::in | ios::binary);//OFSTRUCT file_data; //the file data information
			bool bLookupFile=ifData.is_open();
			if (!bLookupFile) {// ((file_handle=OpenFile("by3dAlphaLookup.raw",&file_data,OF_READ))==-1) //open the file if it exists
			//if can't open file:
				cerr<< "\nERROR: Unable to open alpha lookup file!";
				iErrors++;
			}

			ifData.read((char*)by3dAlphaLookup, 256*256*256); //_lread(file_handle, by3dAlphaLookup,256*256*256);
			ifData.close();//_lclose(file_handle);

			if (!bLookupFile) {
				//Generate alpha lookup table if needed
				cerr<< "\nNeed to generate alpha lookup table..."<<flush;
				int iSource=0;
				int iDest;
				int iAlpha;//can't use bytes because the loop will never end (never >255!)
				for (float source=0.0f; source<=255.0f; source+=1.0f, iSource++) {
					iDest=0;
					for (float dest=0.0f; dest<=255.0f; dest+=1.0f, iDest++) {
						iAlpha=0;
						for (float alpha=0.0f; alpha<=255.0f; alpha+=1.0f, iAlpha++) {
							by3dAlphaLookup[iSource][iDest][iAlpha]=(BYTE)((source-dest)*alpha/255.0f+dest);
						}
					}
				}
				//TODO: ofstream ofData("data/lookup-alpha.raw");
				cerr<< "Done."<<endl;
				//TODO: save it?
			}
			Console.WriteLine( ToString("done (") + ToString((bLookupFile)?"from":"generated") + " lookup file)." );

			Console.Write("Loading image files...");
			for (int shotsNow=0; shotsNow<MAXSHOTS; shotsNow++)
				lparrShot[shotsNow]=NULL;
			for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++)
				lparrAlien[iAliensNow]=NULL;

			// this is called once after the initial window is created and
			// before the main event loop is entered, do all your initialization
			// here
			//lpspritearr=(LPSPRITE*)malloc(MAX_SPRITES*sizeof(LPSPRITE));
			//memset(lpspritearr,0,MAX_SPRITES*sizeof(LPSPRITE));

			//RECT explosionRect={0,0,200,200};
			IRECT screenRect= {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

			REAL_BPP=32;
			REAL_BYTEDEPTH=4;

			REAL_BPP=32;
			REAL_BYTEDEPTH=4;
			LoadSequence(animBackdrop, "images/area", iBackdrops);
			LoadSequence(animHero, "images/hero", iFramesHero);
			LoadSequence(animFlyer, "images/flyer", iFramesFlyer);
			LoadSequence(animFlyerShadow, "images/flyer-shad", iFramesFlyerShadow);
			LoadSequence(animBossShadow, "images/boss-shad", iFramesBossShadow);
			LoadSequence(animShotShadow, "images/shot-shad", iFramesShotShadow);
			LoadSequence(animHeroShadow, "images/hero-shad", iFramesHeroShadow);
			LoadSequence(animBoss, "images/boss", iFramesBoss);
			LoadSequence(animShot, "images/shotani", iFramesShot);
			LoadSequence(animGameScreen, "images/gamescreen", iFramesGameScreen);
			LoadSequence(animGameScreenArea, "images/gamescreenarea", iAreas);
			LoadSequence(animGameScreenEncounter, "images/gamescreenencounter", iEncounters);
			LoadSequence(animCursor, "images/cursor", iFramesCursor);
			LoadImage(gbIntro, "images/intro-title.tga");
			LoadImage(gbIntroText, "images/intro-text.tga");
			LoadImage(gbSymbolShield, "images/symbol-shield.tga");
			LoadImage(gbSymbolBossHealth, "images/symbol-bosshealth.tga");
			LoadImage(gbGuysDecor, "images/guys.tga");

			settings.bSaveEveryChange=true;
			dat.bSaveEveryChange=true;
			Console.WriteLine("done (end of GameInit).");
		}//end try
		catch (int iExn) { if (ShowError()) cerr<<"int exception in GameInit: "<<iExn<<endl; bGood=false; }
		catch (char* sExn) { if (ShowError()) cerr<<"Exception error in GameInit: "<<sExn<<endl; bGood=false; }
		catch (...) { if (ShowError()) cerr<<"Unknown Exception in GameInit."<<endl; bGood=false; }
		return(bGood);
	} // end GameInit

	/////////////////////////////////////////////////////////////

	bool GameShutdown() {
		//This is called after the game is exited and the main event
		// loop while is exited, do all you cleanup and shutdown here
		bool bGood=true;
			//Release palette if 8-bit was used
		//if (lpddpal){lpddpal->Release(); lpddpal=NULL;} //
		static bool bDoneShutdown=false;
		if (!bDoneShutdown) {
		try {
		    if (SDL_ShowCursor(SDL_QUERY)==SDL_DISABLE) {
	            SDL_ShowCursor(SDL_ENABLE);
			}
			SafeChunkUnload(mcBomb);
			SafeChunkUnload(mcLaserAlien);
			SafeChunkUnload(mcLaser);
			SafeChunkUnload(mcExplosion);
			SafeChunkUnload(mcOuchAlien);
			SafeChunkUnload(mcOuchZap);
			SafeChunkUnload(mcShieldZap);
			SafeChunkUnload(mcBlerrp);
			SafeChunkUnload(mcHitDirt);
			SafeChunkUnload(mcJump);
			SafeChunkUnload(mcScrapeGround);
			SafeChunkUnload(mcAngryAlien);
			SafeChunkUnload(mcTrumpet);
			SafeChunkUnload(mcThruster);
			SafeChunkUnload(mcLaser);
			SafeChunkUnload(mcThruster);
			ShutdownMusic();
			if (lparrAlien!=NULL) {
				try {
					for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++) {
						if (lparrAlien[iAliensNow]!=NULL) {
							if (iAliensNow==iBoss) iBoss=-1;
							delete (Entity*)lparrAlien[iAliensNow];
							lparrAlien[iAliensNow]=NULL;
						}
					}
					free(lparrAlien);
				}
				catch (...) { if (ShowError()) cerr<<"Unknown Exception terminating alien flyers in GameShutdown."<<endl; bGood=false; }
	        	//SafeFree(lparrAlien);
			}
		}
		catch (int iExn) { if (ShowError()) cerr<<"int exception in GameShutdown: "<<iExn<<endl; bGood=false; }
		catch (char* sExn) { if (ShowError()) cerr<<"Exception error in GameShutdown: "<<sExn<<endl; bGood=false; }
		catch (...) { if (ShowError()) cerr<<"Unknown Exception in GameShutdown."<<endl; bGood=false; }
		bDoneShutdown=true;
	  }//end if !bDoneShutDown
	  else Console.WriteLine("Already done shutdown!");
		return(bGood);
	} // end GameShutdown

	////////////////////////////////////////////////////////////////////////////////
	#define FRADAR_ZOOM			1.0f
	#define FRADAR_ZOOMINNER	1.0f
	#define XPIX_RADAR  		564
	#define YPIX_RADAR			55
	#define RADAR_TOP           0
	#define RADAR_BOTTOM        110
	#define RADAR_LEFT          490
	#define RADAR_RIGHT         639
	float RadarCenterX() {
		return XPIX_RADAR;
	}
	float RadarCenterY() {
		return YPIX_RADAR;
	}
	float RadarHeight() {
		return RADAR_BOTTOM - RADAR_TOP;
	}
	void RadarDotAt(int &xPixReturn, int &yPixReturn, float xPos, float yPos) {
		bool bGood=true;
		try {
			if (hero) {
				xPos=xPos-hero->m3dEnt.x;
				yPos=yPos-hero->m3dEnt.y;
			}
			if (xPos>0) xPixReturn=(int)APPROACH(XPIX_RADAR,RADAR_RIGHT,xPos/FXMAX);
			else xPixReturn=(int)APPROACH(XPIX_RADAR,RADAR_LEFT,xPos/FXMIN);
			if (yPos>0) yPixReturn=(int)APPROACH(YPIX_RADAR,RADAR_TOP,yPos/FYMAX);
			else yPixReturn=(int)APPROACH(YPIX_RADAR,RADAR_BOTTOM,yPos/FYMIN);
			if (yPixReturn<RADAR_TOP) yPixReturn=RADAR_TOP;
			else if (yPixReturn>RADAR_BOTTOM) yPixReturn=RADAR_BOTTOM;
			if (xPixReturn<RADAR_LEFT) xPixReturn=RADAR_LEFT;
			else if (xPixReturn>RADAR_RIGHT) xPixReturn=RADAR_RIGHT;
		}
		catch (int iExn) { if (ShowError()) cerr<<"int exception in RadarDotAt function: "<<iExn<<endl; bGood=false; }
		catch (char* sExn) { if (ShowError()) cerr<<"Exception error in RadarDotAt function: "<<sExn<<endl; bGood=false; }
		catch (...) { if (ShowError()) cerr<<"Unknown Exception in RadarDotAt function."<<endl; bGood=false; }
	}//end RadarDotAt
	void DrawRadarDot(float xPos, float yPos, float zPos, Uint32 dwPixel) {
		DrawRadarDot(xPos,yPos,zPos,dwPixel,true);
	}
	void DrawRadarDot(float xPos, float yPos, float zPos, Uint32 dwPixel, bool bFilled) {
		bool bGood;
		try {
			int xCenter,yCenter;
			RadarDotAt(xCenter, yCenter, xPos, yPos);
			int xOffset=(int)(zPos);
			int yOffset=(int)(zPos);
			int left=xCenter-xOffset, right=xCenter+xOffset, top=yCenter-yOffset, bottom=yCenter+yOffset;
			DrawExclusiveRect(left, top, bottom+1, right+1, dwPixel, bFilled);
		}
		catch (int iExn) { if (ShowError()) cerr<<"int exception in DrawRadarDot: "<<iExn<<endl; bGood=false; }
		catch (char* sExn) { if (ShowError()) cerr<<"Exception error in DrawRadarDot: "<<sExn<<endl; bGood=false; }
		catch (...) { if (ShowError()) cerr<<"Unknown Exception in DrawRadarDot."<<endl; bGood=false; }
	}//end DrawRadarDot
	void DrawRadarRect(float left, float top, float bottom, float right, Uint32 dwPixel, bool bFilled) {
		bool bGood=true;
		try {
			int iLeft=0, iTop=0, iBottom=0, iRight=0;
			RadarDotAt(iLeft,iTop,left,top);
			RadarDotAt(iRight,iBottom,right,bottom);
			DrawExclusiveRect(iLeft, iTop, iBottom+1, iRight+1, dwPixel, bFilled);
		}
		catch (int iExn) { if (ShowError()) cerr<<"int exception in DrawRadarRect: "<<iExn<<endl; bGood=false; }
		catch (char* sExn) { if (ShowError()) cerr<<"Exception error in DrawRadarRect: "<<sExn<<endl; bGood=false; }
		catch (...) { if (ShowError()) cerr<<"Unknown Exception in DrawRadarRect."<<endl; bGood=false; }
		//catch (exception& exn) {
		//	FakeException("Exception error during DrawRadarRect");
		//}
	}//end DrawRadarRect
	void DrawRadarField() {
		static float FRADAR_XRAD=FXMAX-FXMIN;
		static float FRADAR_YRAD=FYMAX-FYMIN;
		static Uint32 dwPixel=0x004400FF;
		static int xCenter=SCREEN_WIDTH-(int)(FRADAR_XRAD+6.0f);
		static int yCenter=(int)((float)FRADAR_YRAD+6.0f);
		static int xOffset=(int)(FRADAR_ZOOM*FRADAR_XRAD);
		static int yOffset=(int)(FRADAR_ZOOM*FRADAR_YRAD);
		static int left=xCenter-xOffset, right=xCenter+xOffset, top=yCenter-yOffset, bottom=yCenter+yOffset;
		DrawExclusiveRect(left, top, bottom, right, dwPixel, false);
	}
	void DrawExclusiveRect(int left, int top, int bottom, int right, Uint32 dwPixel, bool bFilled) {
		if (left<0||right>=BUFFER_WIDTH||top<0||bottom>=BUFFER_HEIGHT)//error checking
			return;
		left+=SCREEN_OFFSET_X;
		right+=SCREEN_OFFSET_X;
		top+=SCREEN_OFFSET_Y;
		bottom+=SCREEN_OFFSET_Y;
		bool bGood=true;
		Uint32* lpdwDest=(Uint32*)gbScreen.byarrData; //TODO: avoid low-level operations
		int iScreenW=gbScreen.iWidth;
		int iScreenH=gbScreen.iHeight;
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
		int iStart=top*iScreenW+left;//no stride since using Uint32* to buffer
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
	TTF_Font *font;         //Pointer to the font we will be using to render the score.
	SDL_Surface *scoretext; //Surface containing the text rendered by SDL_ttf that has the score.
	bool bTTFStarted=false;
	bool bTTFError=false;
	void ShowDebugVars() {//debug doesn't work, isn't used
		if (!bTTFStarted) {
			if (TTF_Init() == -1) { //Initialize SDL_ttf and if it fails, report the error.
				//printf("Unable to initialize SDL_ttf: ", TTF_GetError(), endl);
	            bTTFError=true;
			}
	 		font=TTF_OpenFont("arial.ttf", 8);
			if (font==NULL) {
				//printf("TTF_OpenFont: ", TTF_GetError(), endl);
				bTTFError=true;
				cerr<<"TTF_OpenFont: "<<TTF_GetError()<<endl;
			}
			bTTFStarted=true;
		}
		else if (!bTTFError) {
			SDL_Color fontcolor={0,0,0,255};       //The color that the text will be rendered in by SDL_ttf.
			char fonttext[800];                     //The text that is rendered by SDL_ttf.
			sprintf(fonttext, "Entity.top: %i, Entity.bottom: %i,  Entity.left: %i, Entity.right: %i", hero->m2dEnt.rectRender.top, hero->m2dEnt.rectRender.bottom,hero->m2dEnt.rectRender.left,hero->m2dEnt.rectRender.right);
			scoretext=TTF_RenderText_Solid(font, fonttext, fontcolor);
	        SDL_Surface *srcimg=scoretext;
			int sx=0,sy=0,sw=SCREEN_WIDTH,sh=SCREEN_HEIGHT/6,dx=0,dy=0,alpha=255;
			if ((!srcimg) || (alpha==0)) return; //If there's no image, or its 100% transparent.
			SDL_Rect rectSrc, rectDest;                     //The two rectangles are filled with the information passed to the function.
			rectSrc.x=sx;  rectSrc.y=sy;  rectSrc.w=sw;  rectSrc.h=sh;
			rectDest.x=dx;  rectDest.y=dy;  rectDest.w=rectSrc.w;  rectDest.h=rectSrc.h;
			if (alpha != 255) SDL_SetAlpha(srcimg, SDL_SRCALPHA, alpha); //Make SDL aware of the desired Alpha transparency in the source image.
			SDL_BlitSurface(srcimg, &rectSrc, screen, &rectDest);                 //Finally Blit the source on to the destination surface.
		}
	}//end ShowDebugVars
	////////////////////////////////////////////////////////////////////
	bool SafeChunkUnload(Mix_Chunk* &mcToNull) {
		if (mcToNull!=NULL) {
	        Mix_FreeChunk(mcToNull);
	        mcToNull=NULL;
		}
	}
	void ClearSounds() {
		bPlayTrumpet=false;
		iPlayBomb=-1;
		iPlayLaserAlien=-1;
		iPlayLaser=-1;
		iPlayExplosion=-1;
		iPlayOuchAlien=-1;
		iPlayOuchZap=-1;
		iPlayShieldZap=-1;
		iPlayBlerrp=-1;
		iPlayHitDirt=-1;
		iPlayJump=-1;
		iPlayScrapeGround=-1;
		iPlayAngryAlien=-1;
		iThruster=-1;
	}
	int PlaySounds() {
		int iPlayed=0;
		//only do one at a time, so use else if
		if (!settings.GetForcedBool("sound")) {
			return iPlayed;
		}
		if (bPlayTrumpet) {
			bPlayTrumpet=false;
			iChanTrumpet=Mix_PlayChannel(PickChan(), mcTrumpet, 0); //chan, sound, #loops
		}
		else if (iPlayLaserAlien>-1) {
			iPlayed++;
			iChanLaserAlien=Mix_PlayChannel(PickChan(), mcLaserAlien, 0); //chan, sound, #loops
			SetPan(iChanLaserAlien, iPlayLaserAlien);
			iPlayLaserAlien=-1;
		}
		else if (iPlayLaser>-1) {
			iPlayed++;
			iChanLaser=Mix_PlayChannel(PickChan(), mcLaser, 0); //chan, sound, #loops
			SetPan(iChanLaser, iPlayLaser);
			iPlayLaser=-1;
		}
		else if (iPlayExplosion>-1) {
			iPlayed++;
			iChanExplosion=Mix_PlayChannel(PickChan(), mcExplosion, 0); //chan, sound, #loops
			SetPan(iChanExplosion, iPlayExplosion);
			iPlayExplosion=-1;
		}
		else if (iPlayOuchAlien>-1) {
			iPlayed++;
			iChanOuchAlien=Mix_PlayChannel(PickChan(), mcOuchAlien, 0); //chan, sound, #loops
			SetPan(iChanOuchAlien, iPlayOuchAlien);
			iPlayOuchAlien=-1;
		}
		else if (iPlayOuchZap>-1) {
			iPlayed++;
			iChanOuchZap=Mix_PlayChannel(PickChan(), mcOuchZap, 0); //chan, sound, #loops
			SetPan(iChanOuchZap, iPlayOuchZap);
			iPlayOuchZap=-1;
		}
		else if (iPlayShieldZap>-1) {
			iPlayed++;
			iChanShieldZap=Mix_PlayChannel(PickChan(), mcShieldZap, 0); //chan, sound, #loops
			SetPan(iChanShieldZap, iPlayShieldZap);
			iPlayShieldZap=-1;
		}
		else if (iPlayBlerrp>-1) {
			iPlayed++;
			iChanBlerrp=Mix_PlayChannel(PickChan(), mcBlerrp, 0); //chan, sound, #loops
			SetPan(iChanBlerrp, iPlayBlerrp);
			iPlayBlerrp=-1;
		}
		else if (iPlayHitDirt>-1) {
			iPlayed++;
			iChanHitDirt=Mix_PlayChannel(PickChan(), mcHitDirt, 0); //chan, sound, #loops
			SetPan(iChanHitDirt, iPlayHitDirt);
			iPlayHitDirt=-1;
		}
		else if (iPlayJump>-1) {
			iPlayed++;
			iChanJump=Mix_PlayChannel(PickChan(), mcJump, 0); //chan, sound, #loops
			SetPan(iChanJump, iPlayJump);
			iPlayJump=-1;
		}
		else if (iPlayScrapeGround>-1) {
			iPlayed++;
			iChanScrapeGround=Mix_PlayChannel(PickChan(), mcScrapeGround, 0); //chan, sound, #loops
			SetPan(iChanScrapeGround, iPlayScrapeGround);
			iPlayScrapeGround=-1;
		}
		else if (iPlayAngryAlien>-1) {
			iPlayed++;
			iChanAngryAlien=Mix_PlayChannel(PickChan(), mcAngryAlien, 0); //chan, sound, #loops
			SetPan(iChanAngryAlien, iPlayAngryAlien);
			iPlayAngryAlien=-1;
		}
		return iPlayed;
	}//
	////////////////////////////////////////////////////////////////////
	void Refresh3dCursor() {
		bool bGood=true;
		try {
			if (hero) {
				float fAltitude, fAzimuth,
							xCursorMin=FSCREEN_WIDTH*.1f, yCursorMin=FSCREEN_HEIGHT*.1f,
							xCursorMax=FSCREEN_WIDTH-xCursorMin, yCursorMax=FSCREEN_WIDTH-yCursorMin,
							xCursorRange=xCursorMax-xCursorMin, yCursorRange=yCursorMax-yCursorMin,
							xCursorness, yCursorness,
							fAimerDistance=3;
				xCursorness=( ((xfCursor<xCursorMin)?xCursorMin:((xfCursor>xCursorMax)?xCursorMax:xfCursor)) - xCursorMin )  /  xCursorRange;
				yCursorness=( ((yfCursor<yCursorMin)?yCursorMin:((yfCursor>yCursorMax)?yCursorMax:yfCursor)) - yCursorMin )  /  yCursorRange;
				if (bFlipYaw) xCursorness=1.0f-xCursorness;
				if (bFlipPitch) yCursorness=1.0f-yCursorness;
				fAzimuth=xCursorness*360.0f-90.0f;
				fAltitude=yCursorness*180.0f-90.0f;
				byte byBrightness=(byte)( (FANGLEDIFFPOSITIVE(fAzimuth,90)/180.0f) * 128.0f + 128.0f);
				if (byBrightness<128) byBrightness=255;//fixes byte integer looping
				if (bMouseDown) {
					hero->Shoot(fAltitude,fAzimuth); //p3dCursorAbs is an absolute position
				}
				if (bMouseMove) {
					hero->Turn(fAzimuth);
				}
				m3dHeroEye.x=hero->EyeX();
				m3dHeroEye.y=hero->EyeY();
				m3dHeroEye.z=hero->EyeZ();
				m3dCursor.x=m3dHeroEye.x;
				m3dCursor.y=m3dHeroEye.y;
				m3dCursor.z=m3dHeroEye.z;
				Travel3d(m3dCursor, fAltitude, fAzimuth, fAimerDistance);
				AddScreenItem(SCREENITEM_3DCURSOR,ZORDER_FROMY(m3dCursor.y),byBrightness);
				/*
				Mass3d p3dHeroEye;
				float fAimRadNear=100.0f;
				float fAimRadFar=fAimRadNear*2.0f;
				float fAimRadSquared=fAimRadNear*fAimRadNear;
				float
						xRelNear,yRelNear, //screen location relative to hero
						xCursorNearFlatAbs,yCursorNearFlatAbs, //absolute screen location of MODIFIED cursor
						fAngleFlat, //angle using screen (cartesian-ized) relative coords
						fDistFlatFarther, //absolute cursor distance
						fDistFlatFar, //absolute cursor distance limited
						fDistFlatNear, //absolute cursor distance looped backward to zero as goes beyond fAimRadNear toward fAimRadFar
						zFake, //distance "from screen" -- positive is "out"
						fAngleFlatNonCartesian; //angle using screen (flipped) relative coords
				bool bBack, bTurn;
				p3dHeroEye.x=hero->EyeX();
				p3dHeroEye.y=hero->EyeY();
				p3dHeroEye.z=hero->EyeZ();
				FPOINT fpHeroEye;
				camera.Point2dRatiosFrom3d(fpHeroEye.x,fpHeroEye.y,p3dHeroEye.x,p3dHeroEye.y,p3dHeroEye.z);
				fpHeroEye.x=fpHeroEye.x*SCREEN_WIDTH;
				fpHeroEye.y=fpHeroEye.y*SCREEN_HEIGHT;
				xRelNear=xfCursor-fpHeroEye.x; yRelNear=yfCursor-fpHeroEye.y;

				fAngleFlat=SafeAngle360(THETAOFXY(xRelNear,-yRelNear));
				fAngleFlatNonCartesian=SafeAngle360(THETAOFXY(xRelNear,yRelNear));
				fDistFlatFarther=ROFXY(xRelNear,yRelNear); //ONLY OK since before they aren't "near" yet now
				fDistFlatFar=fDistFlatFarther;
				if (fDistFlatFar>fAimRadFar) fDistFlatFar=fAimRadFar;

				xCursorNearFlatAbs=xfCursor; yCursorNearFlatAbs=yfCursor;
				if (fDistFlatFar>fAimRadNear) {
					float fBackBy;
					bBack=true;
					fBackBy=(fDistFlatFarther-fAimRadNear)/fAimRadNear;
					if (fBackBy>1.0f) fBackBy=1.0f;
					xCursorNearFlatAbs=APPROACH(xCursorNearFlatAbs,fpHeroEye.x,fBackBy);
					yCursorNearFlatAbs=APPROACH(yCursorNearFlatAbs,fpHeroEye.y,fBackBy);
					fDistFlatNear=APPROACH(fDistFlatFar,0,fBackBy);
				}
				else {
					bBack=false;
					fDistFlatNear=fDistFlatFar;
				}
				xRelNear=xCursorNearFlatAbs-fpHeroEye.x;
				yRelNear=yCursorNearFlatAbs-fpHeroEye.y;
				const float fCircleSize=.02*(float)SCREEN_HEIGHT;
				float fSizeModifier;
				float fProduct=fAimRadSquared-fDistFlatNear*fDistFlatNear;
				zFake=(fProduct<=0)?0.0f:sqrt(fProduct);//flipped thag theorem to get fake straight "distance from the screen"
				if (bBack) {
					fSizeModifier=fDistFlatNear/fAimRadNear-1.0f;
					//fSizeModifier=1.0f-(fSizeModifier*fSizeModifier);//to make it immitate a sphere
				}
				else {
					zFake*=-1.0f;
					fSizeModifier=1.0f-fDistFlatNear/fAimRadNear;
					//fSizeModifier=(fSizeModifier*fSizeModifier);//to make it immitate a sphere
				}
				xRelNear*=xAimFlipper;
				yRelNear*=yAimFlipper;
				zFake*=zAimFlipper;
				xCursorNearFlatAbs=fpHeroEye.x+xRelNear;
				yCursorNearFlatAbs=fpHeroEye.y+yRelNear;
				//gbScreen.DrawSubpixelLine(fpHeroEye.x+xRelNear+FSCREEN_OFFSET_X, //debug only
				//							   fpHeroEye.y+yRelNear+FSCREEN_OFFSET_Y,
				//							   fpHeroEye.x+xRelNear+FSCREEN_OFFSET_X-zFake,
				//							   fpHeroEye.y+yRelNear+FSCREEN_OFFSET_Y+zFake,
				//							   255,0,0,
				//							   33,.2);
				//float fFrontnessFlatFake=((zFake*-1.0f+fAimRadNear)/fAimRadFar);
				//byte byValue=(byte)( 63.0f+192*fFrontnessFlatFake);
				//gbScreen.DrawSubpixelArc(xCursorNearFlatAbs+FSCREEN_OFFSET_X,
				//							yCursorNearFlatAbs+FSCREEN_OFFSET_Y,
				//							4.5,1,0,
				//							0,SafeAngle360(THETAOFXY(xRelNear,zFake)),
				//							byValue,0,0,128,
				//							5,0);
				fSizeModifier*=fCircleSize*.3f;
				float fCircleResult;

			   	//fSizeModifier*=fSizeModifier;
				//if (bBack) {
	            //   fSizeModifier*=-1;
				//}
				fCircleResult=fCircleSize+fSizeModifier;
				//fCircleResult*=fCircleResult;

				//gbScreen.DrawSubpixelLine(32+FSCREEN_OFFSET_X,32+FSCREEN_OFFSET_Y,xfCursor+FSCREEN_OFFSET_X,yfCursor+FSCREEN_OFFSET_Y,0,255,0,255,.2);
				//gbScreen.DrawSubpixelLine(fpHeroEye.x+FSCREEN_OFFSET_X, fpHeroEye.y+FSCREEN_OFFSET_Y, xCursorNearFlatAbs+FSCREEN_OFFSET_X, yCursorNearFlatAbs+FSCREEN_OFFSET_Y,
				//		0,(bBack?127:255),0,255,32*fDistFlatNear/fAimRadNear);
				//gbScreen.DrawSubpixelArc(xCursorNearFlatAbs+FSCREEN_OFFSET_X, yCursorNearFlatAbs+FSCREEN_OFFSET_Y, fCircleResult, 1.0f-(fDistFlatNear/fAimRadNear), fAngleFlatNonCartesian, 0, 360,
				//		0,(bBack?127:255),0,255,.5,0);
				//float fBackness=0, fFrontness=0;
				//if (zFake<0) {
				//	fFrontness=-1.0f*zFake/fAimRadNear;
				//}
				//else {
				//	fBackness=zFake/fAimRadNear;
				//}
				//gbScreen.DrawSubpixelLine(32+FSCREEN_OFFSET_X,32+FSCREEN_OFFSET_Y,xfCursor+FSCREEN_OFFSET_X,yfCursor+FSCREEN_OFFSET_Y,
				//(byte)(255.0f*fBackness),0,(byte)(255.0f*fFrontness),
				//255,.2);
				bTurn=bMouseMove;
				Mass3d p3dCursorAbs, p3dCursorRel;
				p3dCursorAbs.x=p3dHeroEye.x;
				p3dCursorAbs.y=p3dHeroEye.y;
				p3dCursorAbs.z=p3dHeroEye.z;

				camera.Point3dMoveAndRotateBy2d(p3dCursorAbs,xRelNear,yRelNear,zFake,fpHeroEye.x,fpHeroEye.y);
				p3dCursorRel.x=p3dCursorAbs.x-p3dHeroEye.x;
				p3dCursorRel.y=p3dCursorAbs.y-p3dHeroEye.y;
				p3dCursorRel.z=p3dCursorAbs.z-p3dHeroEye.z;
				float fForwardness=-1.0f*zFake/fAimRadNear;// *-1.0 since more forward if negative
				FPOINT fpCursor;

				camera.Point2dFrom3d(fpCursor,p3dCursorAbs);
				bBack=(p3dCursorAbs.y>p3dHeroEye.y);//debug does not account for non-z=90 camera rotations
				float fFrontness;//=(bBack?127:255);
				float fAimRadMeters=DIST3D(p3dHeroEye,p3dCursorAbs); //always the same since radius is constant
				float fAimDiameterMeters=fAimRadMeters*2.0f; //always the same since constant radius
				fFrontness=fabs(p3dCursorAbs.y-(p3dHeroEye.y+fAimRadMeters))/fAimDiameterMeters; //debug does not account for other camera rotation
				Crop(fFrontness,0,1);
				pixelNow.Set(0,(byte)((fFrontness*192.0f)+63.0f),0,255);
				gbScreen.DrawSubpixelArc(fpCursor.x+FSCREEN_OFFSET_X, fpCursor.y+FSCREEN_OFFSET_Y, fCircleResult, 1.0f-(fDistFlatNear/fAimRadNear), fAngleFlatNonCartesian, 0, 360,
					pixelNow,.5,0);
				//gbScreen.DrawSubpixelLine(fpHeroEye.x+FSCREEN_OFFSET_X, fpHeroEye.y+FSCREEN_OFFSET_Y, xCursorNearFlatAbs+FSCREEN_OFFSET_X, yCursorNearFlatAbs+FSCREEN_OFFSET_Y,
				//		(bBack?64:128),0,0,(bBack?64:128),.3);
				float fAlpha=191+fForwardness*128;
				if (fAlpha>255.0f) fAlpha=255.0f;
				Pixel pixelEnd;
				pixelEnd.Set(0,(byte)(191.0f+fForwardness*64),0,(byte)fAlpha);
				pixelNow.Set(pixelEnd.r,pixelEnd.g,pixelEnd.b,0);
				gbScreen.DrawSubpixelLine(fpHeroEye.x+FSCREEN_OFFSET_X, fpHeroEye.y+FSCREEN_OFFSET_Y, fpCursor.x+FSCREEN_OFFSET_X, fpCursor.y+FSCREEN_OFFSET_Y,
						pixelNow,&pixelEnd,.9);
				//bool bFirstRun=true;
				//if (bTurn||bFirstRun) {//commented for debug only
					//float zRealRot;
					//zRealRot=THETAOFXY(xRelNear,zFake);
					hero->Turn(p3dCursorAbs.zRot);
				//	bFirstRun=false;//commented for debug only
				///}
				if (bMouseDown) {
					hero->Shoot(p3dCursorAbs.yRot,p3dCursorAbs.zRot); //p3dCursorAbs is an absolute position
				}
				*/
				//int iTest=0;
				//if (iTest<100 && bMouseMove) {
				//	//cerr<<"Draw3dCursor {fpHeroEye.x:"<<fpHeroEye.x<<"; fpHeroEye.y:"<<fpHeroEye.y<<"; xCursorNearFlatAbs:"<<xCursorNearFlatAbs<<"; yCursorNearFlatAbs:"<<yCursorNearFlatAbs<<"; hEyeX():"<<p3dHeroEye.x<<"; EyeY():"<<p3dHeroEye.y<<"; EyeZ():"<<p3dHeroEye.z<<"}"<<endl;
				//	cerr<<"Draw3dCursor {fpHeroEye.x:"<<fpHeroEye.x<<";\tfpHeroEye.y:"<<fpHeroEye.y<<";\tp3dCursorAbs.x:"<<p3dCursorAbs.x<<";\tp3dCursorAbs.y:"<<p3dCursorAbs.y<<";\tp3dCursorAbs.z:"<<p3dCursorAbs.z<<";\txCursorNearFlatAbs:"<<xCursorNearFlatAbs<<";\tyCursorNearFlatAbs:"<<yCursorNearFlatAbs<<";\tzFake:"<<zFake<<";\tEyeX():"<<p3dHeroEye.x<<";\tEyeY():"<<p3dHeroEye.y<<";\tEyeZ():"<<p3dHeroEye.z<<"}"<<endl;
				//	iTest++;
				//}
				bMouseMove=false;
			}//end if hero
			else { //else show a 2d cursor since hero not present
				animCursor.GotoFrame(0);
				animCursor.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, xCursor+SCREEN_OFFSET_X-xCursorCenter, yCursor+SCREEN_OFFSET_Y-yCursorCenter, DrawModeBlendAlpha);
			}
		}
		catch (int iExn) { if (ShowError()) cerr<<"int exception in Refresh3dCursor: "<<iExn<<endl; bGood=false; }
		catch (char* sExn) { if (ShowError()) cerr<<"Exception error in Refresh3dCursor: "<<sExn<<endl; bGood=false; }
		catch (...) { if (ShowError()) cerr<<"Unknown Exception in Refresh3dCursor."<<endl; bGood=false; }
	}//end Refresh3dCursor
	void Draw3dCursor(byte byBrightness) {
		Pixel pixelNow;
		FPOINT fpStart,//this point is first used as the hero weapon
			fpEnd;//this point is first used as the 3d cursor
		Pixel pixelEnd, pixelStart;
		camera.Point2dFrom3d(fpStart,m3dHeroEye);
		camera.Point2dFrom3d(fpEnd,m3dCursor);
		pixelStart.Set(0,byBrightness,0,0);
		pixelEnd.Set(0,byBrightness,0,255);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelNow,&pixelEnd,.9);
		//draw screen-edge crosshairs:
		pixelStart.Set(255,0,0,255);
		pixelEnd.Set(255,0,0,255);
		float fLenPix=16;
		//top:
		fpStart.x=xfCursor+FSCREEN_OFFSET_X;
		fpStart.y=FSCREEN_OFFSET_Y;
		fpEnd.x=fpStart.x;
		fpEnd.y=fpStart.y+fLenPix;
		gbScreen.DrawSubpixelLine(fpStart.x, fpStart.y, fpEnd.x, fpEnd.y,
				pixelNow,&pixelEnd,.9);
		//bottom:
		fpStart.x=xfCursor+FSCREEN_OFFSET_X;
		fpStart.y=FSCREEN_OFFSET_Y+FSCREEN_HEIGHT;
		fpEnd.x=fpStart.x;
		fpEnd.y=fpStart.y-fLenPix;
		gbScreen.DrawSubpixelLine(fpStart.x, fpStart.y, fpEnd.x, fpEnd.y,
				pixelNow,&pixelEnd,.9);
		//left:
		fpStart.x=FSCREEN_OFFSET_X;
		fpStart.y=yfCursor+FSCREEN_OFFSET_Y;
		fpEnd.x=fpStart.x+fLenPix;
		fpEnd.y=fpStart.y;
		gbScreen.DrawSubpixelLine(fpStart.x, fpStart.y, fpEnd.x, fpEnd.y,
				pixelNow,&pixelEnd,.9);
		//right:
		fpStart.x=FSCREEN_OFFSET_X+FSCREEN_WIDTH;
		fpStart.y=yfCursor+FSCREEN_OFFSET_Y;
		fpEnd.x=fpStart.x-fLenPix;
		fpEnd.y=fpStart.y;
		gbScreen.DrawSubpixelLine(fpStart.x, fpStart.y, fpEnd.x, fpEnd.y,
				pixelNow,&pixelEnd,.9);

	}
	void SayWhatIDrewIfFalse(bool bDrawFunctionReturn, int iEntityType, string sDescription) {
		if (!bDrawFunctionReturn) {
			try {
				if (ShowError()) {
					string sNow=EntityTypeToString(iEntityType);
					cerr<< sNow << " failed to draw " << sDescription << "." << endl;
				}
			}
			catch (...) {
				FakeException("Exception error in SayWhatIDrewIfFalse.");
			}
		}
	}
	string EntityTypeToString(int iEntityType) {
		string sReturn="";
		switch (iEntityType) {
			case ENTITY_TYPE_HERO:
	            sReturn="Hero";
				break;
			case ENTITY_TYPE_ALIEN:
	            sReturn="Alien";
				break;
			case ENTITY_TYPE_BOSS:
	            sReturn="Boss";
				break;
			case ENTITY_TYPE_SHOT:
	            sReturn="Shot";
				break;
			default:
	            sReturn+="(unknown entity type #";
				sReturn+=iEntityType;
				sReturn+=")";
				break;
		}
		return sReturn;
	}
	float MetersToMove(float fMetersPerSecondX, int iForThisManyMilliseconds) {
	    return (float)(  (double)fMetersPerSecondX*((double)(iForThisManyMilliseconds)) / 1000.0  );
	}
	float DegreesToMove(float fDegreesPerSecondX, int iForThisManyMilliseconds) {
	    return (float)(  (double)fDegreesPerSecondX*((double)(iForThisManyMilliseconds)) / 1000.0  );
	}
	void DrawCube(Mass3d &m3dNow, Pixel &pixelNear, Pixel &pixelFar) {
		FPOINT fpStart, fpEnd;
		Mass3d m3darrBox[8]; //(see drawing 1.box-indeces.png)
		float
			xSizeHalf=m3dNow.xSize/2.0f,
			ySizeHalf=m3dNow.ySize/2.0f,
			zSizeHalf=m3dNow.zSize/2.0f;
		m3darrBox[0].x=m3dNow.x-xSizeHalf;
		m3darrBox[0].y=m3dNow.y+ySizeHalf;
		m3darrBox[0].z=m3dNow.z+zSizeHalf;
		m3darrBox[1].x=m3dNow.x+xSizeHalf;
		m3darrBox[1].y=m3dNow.y+ySizeHalf;
		m3darrBox[1].z=m3dNow.z+zSizeHalf;
		m3darrBox[2].x=m3dNow.x+xSizeHalf;
		m3darrBox[2].y=m3dNow.y-ySizeHalf;
		m3darrBox[2].z=m3dNow.z+zSizeHalf;
		m3darrBox[3].x=m3dNow.x-xSizeHalf;
		m3darrBox[3].y=m3dNow.y-ySizeHalf;
		m3darrBox[3].z=m3dNow.z+zSizeHalf;

		m3darrBox[4].x=m3dNow.x-xSizeHalf;
		m3darrBox[4].y=m3dNow.y+ySizeHalf;
		m3darrBox[4].z=m3dNow.z-zSizeHalf;
		m3darrBox[5].x=m3dNow.x+xSizeHalf;
		m3darrBox[5].y=m3dNow.y+ySizeHalf;
		m3darrBox[5].z=m3dNow.z-zSizeHalf;
		m3darrBox[6].x=m3dNow.x+xSizeHalf;
		m3darrBox[6].y=m3dNow.y-ySizeHalf;
		m3darrBox[6].z=m3dNow.z-zSizeHalf;
		m3darrBox[7].x=m3dNow.x-xSizeHalf;
		m3darrBox[7].y=m3dNow.y-ySizeHalf;
		m3darrBox[7].z=m3dNow.z-zSizeHalf;
		float fSubPixAccuracy=3;
		camera.Point2dFrom3d(fpStart,m3darrBox[4]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[5]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelFar,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[4]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[7]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[5]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[6]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[4]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[0]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelFar,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[5]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[1]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelFar,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[0]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[1]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelFar,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[0]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[3]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[1]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[2]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[7]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[6]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelNear,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[7]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[3]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelNear,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[6]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[2]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelNear,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[3]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[2]);
		gbScreen.DrawSubpixelLine(fpStart.x+FSCREEN_OFFSET_X, fpStart.y+FSCREEN_OFFSET_Y, fpEnd.x+FSCREEN_OFFSET_X, fpEnd.y+FSCREEN_OFFSET_Y,
				pixelNear,&pixelNear,fSubPixAccuracy);
	}
	bool DrawScreen() {
		bool bGood=true;
		static bool bFirstRun=true;
		if (SDL_MUSTLOCK(screen)!=0) {
			if (SDL_LockSurface(screen)<0) {
				bGood=false;
				ShowError("Can't lock screen surface","DrawScreen");
			}
		}
		int iDestBytesPP=0;
		int iDestStride=0;
		int iDestWidth=SCREEN_WIDTH;
		int iDestHeight=SCREEN_HEIGHT;
		int iSrcStart;
		int iSrcStride=0;
		int iStrideMin=0;
		if (bGood) {
			iDestBytesPP=screen->format->BytesPerPixel;
			iDestStride=screen->pitch;
			iSrcStride=gbScreen.iStride;
			iSrcStart=iSrcStride*SCREEN_OFFSET_Y+gbScreen.iBytesPP*SCREEN_OFFSET_X;
			iStrideMin=(iSrcStride<iDestStride)?iSrcStride:iDestStride;
			if (gbScreen.BytePtrStart()==null) {
				ShowError("Tried to draw uninitialized GBuffer32BGRA!","DrawScreen");
				bGood=false;
			}
		}
		try {
			byte* lpSrcLine;
			byte* lpSrcPix;
			Uint8* lpDestPix;
			Uint8* lpDestLine;
			if (bGood) {
	            lpSrcLine=gbScreen.BytePtrStart();
	            lpSrcLine+=iSrcStart;//byte offset for cropped gbScreen's top left corner
				if (bFirstRun) Console.Write("debug test...");

				//Uint8* lpbyScreen=(Uint8*)screen->pixels;//debug only
				//*((Uint32*)lpbyScreen)=0xFFFFFF; //debug only
				//*(Uint32*)(&lpbyScreen[32*iDestStride+32*iDestBytesPP])=0xFFFFFF; //debug only
				//gbScreen.Fill(255);//debug only

				if (bFirstRun) Console.Write("copy...");
				lpDestLine=(Uint8*)screen->pixels;//8bit in order to use stride
				switch (iDestBytesPP) {
					case 1:
						for (int y=0; y<iDestHeight; y++) {
							lpDestPix=lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int x=0; x<iDestWidth; x++) {
								*lpDestPix = (Uint8)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
								lpSrcPix+=4;//assumes 32-bit GBuffer32BGRA
								lpDestPix++;
							}
							lpDestLine+=iDestStride;
							lpSrcLine+=iSrcStride;
						}
						break;
					case 2:
						Uint16* lpwDestPix;
						for (int y=0; y<iDestHeight; y++) {
							lpwDestPix=(Uint16*)lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int x=0; x<iDestWidth; x++) {
								*lpwDestPix = (Uint16)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
								lpSrcPix+=4;//assumes 32-bit GBuffer32BGRA
								lpwDestPix++;//ok since Uint16 ptr
							}
							lpDestLine+=iDestStride;
							lpSrcLine+=iSrcStride;
						}
						break;
					case 3:
						Uint32 color;
						if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
							for (int y=0; y<iDestHeight; y++) {
								lpDestPix=lpDestLine;
								lpSrcPix=lpSrcLine;
								for (int x=0; x<iDestWidth; x++) {
									color = (Uint32)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
									*lpDestPix=(byte)color;
									lpDestPix[1]=(byte)(color>>8);
									lpDestPix[2]=(byte)(color>>16);
									lpSrcPix+=4;//assumes 32-bit GBuffer32BGRA
									lpDestPix+=3;
								}
								lpDestLine+=iDestStride;
								lpSrcLine+=iSrcStride;
							}
						}
						else {
							for (int y=0; y<iDestHeight; y++) {
								lpDestPix=lpDestLine;
								lpSrcPix=lpSrcLine;
								for (int x=0; x<iDestWidth; x++) {
									color = (Uint32)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
									lpDestPix[2]=(byte)color;
									lpDestPix[1]=(byte)(color>>8);
									*lpDestPix=(byte)(color>>16);
									lpSrcPix+=4;//assumes 32-bit GBuffer32BGRA
									lpDestPix+=3;
								}
								lpDestLine+=iDestStride;
								lpSrcLine+=iSrcStride;
							}
						}
						break;
					case 4:
						Uint32* lpdwDestPix;
						if (bFirstRun) Console.Write("32-bit from "+ToString(gbScreen.iWidth)+"x"+ToString(gbScreen.iHeight)+"x"+ToString(gbScreen.iBytesPP*8)+"...");
						for (int y=0; y<iDestHeight; y++) {
							lpdwDestPix=(Uint32*)lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int x=0; x<iDestWidth; x++) {
								//*lpdwDestPix=*((Uint32*)lpSrcPix);
								*lpdwDestPix = (Uint32)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
								lpSrcPix+=4;//assumes 32-bit GBuffer32BGRA
								lpdwDestPix++;//ok since Uint32*
							}
							lpDestLine+=iDestStride;
							lpSrcLine+=iSrcStride;
						}
						/* TODO:? SDL_LIL_ENDIAN?:
						if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
							for (int iLine=0; iLine<iDestHeight; iLine++) {
								memcpy(lpDestLine,lpSrcLine,iStrideMin);
								lpDestLine+=iDestStride;
								lpSrcLine+=iSrcStride;
							}
						}
						*/
						break;
				}//end switch
				if (SDL_MUSTLOCK(screen)!=0) {
					SDL_UnlockSurface(screen);
				}
				SDL_Flip(screen);
			}//end if bGood
			//else already showed an error
		}
		catch (exception& exn) {
			ShowException(exn,"DrawScreen()");
			bGood=false;
		}
		catch (...) {
			ShowUnknownException("DrawScreen()");
			bGood=false;
		}
		if (bFirstRun) Console.WriteLine(bGood?"success.":"fail!");
		bFirstRun=false;
		return bGood;
	}//end DrawScreen

	inline void SleepWrapper(int iTicks) {
		SDL_Delay(iTicks);
	}

	void LoadSequence(Anim32BGRA &animToLoad, string sFileBaseNameOnly, int iFramesExpectedElseError) {
		int iFramesFound;
		bool bGood=animToLoad.LoadSeq(sFileBaseNameOnly+"*.tga");
		if (!bGood)	ShowError(sFileBaseNameOnly+"*.tga failed to load","GameInit");
		iFramesFound=animToLoad.IFrames();
		if (bGood && iFramesFound!=iFramesExpectedElseError) {
			bGood=false;
			ShowError("not all "+sFileBaseNameOnly+ToString("*.tga frames loaded."));
		}
	}
	void LoadImage(GBuffer32BGRA &gbToLoad, string sFile) {
		bool bGood=gbToLoad.Load(sFile);
		if (!bGood)	ShowError(sFile+".tga failed to load","GameInit");
	}
	bool GBuffer_FX_Scaled(GBuffer32BGRA &gbDest, GBuffer32BGRA &gbSrc, int xFlat, int yFlat,
			float fOpacity, float fExplodedness, UInt32 dwStat, float fScale) {
		bool bGood=true;
		bool bShowVars=false;
		float fInverseScale;
		UInt32* lpdwDest;
		UInt32* lpdwSrc;
		int iStride;
		int iDestW, iDestH;
		UInt32* lpdwDestNow;

		int iSrcPixels;
		int iDestPixels;
		BYTE* lpbyDest;
		BYTE* lpbySrc;
		float bSrc,gSrc,rSrc,aSrc, bDest,gDest,rDest, bDestModLoc,gDestModLoc,rDestModLoc, bCooked,gCooked,rCooked;
		BYTE bySrcB,bySrcG,bySrcR,bySrcA;
		UInt32 dwPixel;
		int xExploder, xSrcNow;
		int yExploder, ySrcNow;
		bool bExplode;
		int iDestNow, iDestModLoc;
		float fExplodeExp;
		float fExplodeLog;
		float fRemains;
		float fRemainsExp;
		float fRemainsLog;
		//exp example: .75=0.5625
		//log example: .75 =
		int iSrcW, iSrcH;
		int iScaledW;
		int iScaledH;
		int iScaledRight;
		int iScaledBottom;
		int iSrcStride;
		int yMaxExploder, xMaxExploder, xOffScaledOnly,yOffScaledOnly,
			iHalfWidth, iHalfHeight;//explosion vars
		float fX, fY;
		float fDestW, fDestH;
		float xfFlat, yfFlat;
		float fSrcW, fSrcH;
		float fSrcStride;
		int iLimiter;
		int xDestRel,yDestRel;
		int yDestNow;
		int xDestNow;
		int iSrcNow;
	    float fCookedAlpha;
		float rMax, gMax, bMax;
		UInt32 dwPixelMax;
		float fMiddleOfExplosion, fMaxExploder, fDistFromEnds;
		float fInvMiddle;
		int iExploder, iOffset, iDestExp;
		int iDestStart;
		long double iDestEnd;
		int iDestBytes;

		try {
			if (gbDest.byarrData==NULL) {
				if (ShowError()) {
					cerr<<"Null dest image in GBuffer_FX_Scaled"<<endl;
					bShowVars=true;
				}
				bGood=false;
			}
			else if (gbSrc.byarrData==NULL) {
				if (ShowError()) {
					cerr<<"Null source image in GBuffer_FX_Scaled"<<endl;
					bShowVars=true;
				}
				bGood=false;
			}
			else { //else OK
				fX=0; fY=0;
				fDestW=(float)gbDest.iWidth; fDestH=(float)gbDest.iHeight;
				xfFlat=(float)xFlat; yfFlat=(float)yFlat;
				fSrcW=(float)gbSrc.iWidth;
				fSrcH=(float)gbSrc.iHeight;
				iSrcW=(int)gbSrc.iWidth;
				iSrcH=(int)gbSrc.iHeight;
				ResetRand();//makes explosion uniform (?)
				if (fScale<.05f) fScale=.05;
				else if (fScale>10) fScale=10;//debug only
				fInverseScale=1.0f/fScale;
				if (fOpacity>1.0f) fOpacity=1.0f;
				lpdwDest=(UInt32*)gbDest.byarrData;
				lpdwSrc=(UInt32*)gbSrc.byarrData;
				iStride=gbDest.iStride;
				iDestBytes=iStride*(int)gbDest.iHeight;
				iDestStart=xFlat + (yFlat*iDestW);
				iDestEnd=(long double)iDestStart+((long double)(gbSrc.iStride)*fScale*(long double)gbSrc.iHeight*(long double)fScale);
				iDestW=gbDest.iWidth;
				iDestH=gbDest.iHeight;
				if (iDestStart<0||(iDestEnd>=(long double)iDestBytes)||xfFlat<0.0f||yfFlat<0.0f||((int)((xfFlat+fSrcW*fScale)+.5f)>=iDestW)||((int)((yfFlat+fSrcH*fScale)+.5f)>=iDestH)) {
					if (ShowError()) {
						cerr<<"Can't draw "<<(int)(fSrcW*fScale)<<"x"<<(int)(fSrcH*fScale)<<" image at "<<"("<<xFlat<<","<<yFlat<<") (spans to ("<<xFlat+(int)((float)iSrcW*fScale+.5f)<<","<<yFlat+(int)((float)iSrcH*fScale+.5f)<<") ) (was "<<fSrcW<<"x"<<fSrcH<<" scaled by "<<fScale<<").  Limit is ("<<fDestW<<","<<fDestH<<")"<<endl;
						bShowVars=true;
					}
					bGood=false;
				}
			}// end else good
			if (bGood) {
				lpdwDestNow=&lpdwDest[iDestStart];
				iSrcPixels=gbSrc.iWidth*gbSrc.iHeight;
				iDestPixels=gbDest.iWidth*gbDest.iHeight;
				lpbyDest=(BYTE*)lpdwDestNow;
				lpbySrc;//=image.buffer;
				bExplode=(fExplodedness>0.0f)?true:false;
				iDestNow=0; iDestModLoc=0;

				if (bExplode) fExplodedness-=.25f; //start explosion just as a red dude, not blown up yet at frame 1
				if (fExplodedness>1.0f) fExplodedness=1.0f;
				else if (fExplodedness<0.0f) fExplodedness=0.0f;

				fExplodeExp=fExplodedness*fExplodedness;
				fExplodeLog=(fExplodedness>0.0f)?sqrt(fExplodedness):0.0f;
				fRemains=(1.0f-fExplodedness);
				fRemainsExp=fRemains*fRemains;
				fRemainsLog=1.0f-fExplodeLog;
				//exp example: .75=0.5625
				//log example: .75 =
				iScaledW=(int)((float)gbSrc.iWidth*fScale);
				iScaledH=(int)((float)gbSrc.iHeight*fScale);
				iScaledRight=xFlat+iScaledW;
				iScaledBottom=yFlat+iScaledH;
				iSrcStride=gbSrc.iWidth*4;
				iHalfWidth=iScaledW/2; iHalfHeight=iScaledH/2;//explosion vars
				fY=0.0f, fX;
				yMaxExploder=40;
				xMaxExploder=40;
				fSrcStride=(float)iSrcStride;
				iLimiter=gbDest.iHeight-1;
				xDestRel; yDestRel=0;

				for (yDestNow=yFlat; yDestNow<iScaledBottom; yDestNow++, fY+=1.0f, yDestRel++) {
					if (yDestNow>=iLimiter) break;
					///yMaxExploder=(yDestNow<iHalfHeight)?iHalfHeight-yDestNow:yDestNow-iHalfHeight;
					fX=0.0f;
					xDestRel=0;

					for (register int xDestNow=xFlat; xDestNow<iScaledRight; xDestNow++, fX+=1.0f, xDestRel++) {
						yExploder=0;
						xExploder=0;
						//xMaxExploder=(xDestNow<iHalfWidth)?iHalfWidth-xDestNow:xDestNow-iHalfWidth;
						ySrcNow=(int)(fY*fInverseScale);
						xSrcNow=(int)(fX*fInverseScale);
						iSrcNow=ySrcNow*iSrcW + xSrcNow;
						if (iSrcNow>=iSrcPixels||iSrcNow<0) {
							if (ShowError()) {
								cerr<<"Can't Draw: Out of bounds of source image in GBuffer_FX_Scaled."<<endl;
								bShowVars=true;
							}
							bGood=false;
						}
						if (bGood) {
							lpbySrc=(BYTE*)&lpdwSrc[iSrcNow];
							bSrc=*lpbySrc++;
							gSrc=*lpbySrc++;
							rSrc=*lpbySrc++;
							aSrc=*lpbySrc; //++;
							if (fOpacity<1.0f) { //this use of opacity is unique to DXMan; makes object red when transparent
								gSrc/=2.0f;
								bSrc/=4.0f;
							}
							if (bExplode) {
								if (fExplodedness<=.5f) {
									rSrc=255.0f;//APPROACH(255.0f,255.0f,1.0f-fExplodedness*2.0f);
									gSrc=APPROACH(255.0f,128.0f,fExplodedness/.5f);
									bSrc=APPROACH(255.0f,0.0f,fExplodedness/.5f);
								}
								else {
									rSrc=APPROACH(255.0f,0.0f,(fExplodedness-.5f)/.5f);
									gSrc=APPROACH(128.0f,0.0f,(fExplodedness-.5f)/.5f);
									bSrc=APPROACH(0.0f,0.0f,(fExplodedness-.5f)/.5f);
								}
							}

							aSrc*=fOpacity;

							iDestNow=xDestNow + ((yDestNow)*iDestW);
							if (bExplode) {
								//uses -= for implosion
								if (xDestRel<iHalfWidth) xExploder-=(int)(FRand(-xMaxExploder,0)*fExplodeLog);
								else xExploder-=(int)(FRand(0,xMaxExploder)*fExplodeLog);
								if (yDestRel<iHalfHeight) yExploder=(int)(FRand(-yMaxExploder,0)*fExplodeLog);
								else yExploder=(int)(FRand(0,yMaxExploder)*fExplodeLog);
								aSrc*=fRemainsLog;
								iDestModLoc=xDestNow+xExploder + ((yDestNow+yExploder)*iDestW);
							}
							else iDestModLoc=iDestNow;

							if (iDestNow>=iDestPixels || iDestNow<0) {
								if (ShowError()) {
									cerr<<"Out of bounds of destination in GBuffer_FX_Scaled."<<endl;
									bShowVars=true;
								}
								bGood=false;
							}
							else if (iDestModLoc>=iDestPixels || iDestModLoc<0) {
								if (ShowError()) {
									cerr<<"Out of modified bounds of destination in GBuffer_FX_Scaled."<<endl;
									bShowVars=true;
								}
								bGood=false;
							}
							if (bGood) {
								if ((dwStat & STATUS_SHIELD)&&aSrc<192.0f) {//blue outline
									bSrc=192.0f;
									gSrc=64.0f;
								}
								fCookedAlpha=(float)aSrc/255.0f;
								static BYTE by255=255;//avoids type conversions from double constant (?)
								lpbyDest=(BYTE*)&lpdwDest[iDestNow];//first get original location's color
								bDest=*lpbyDest++;
								gDest=*lpbyDest++;
								rDest=*lpbyDest++;
								lpbyDest=(BYTE*)&lpdwDest[iDestModLoc];//now get exploded location's color
								bDestModLoc=*lpbyDest++;
								gDestModLoc=*lpbyDest++;
								rDestModLoc=*lpbyDest++;
								bCooked=APPROACH(bDestModLoc,bSrc,fCookedAlpha);//by3dAlphaLookup[blue][*lpbyDest][bySrcA];
								gCooked=APPROACH(gDestModLoc,gSrc,fCookedAlpha);//by3dAlphaLookup[green][*lpbyDest][bySrcA];
								rCooked=APPROACH(rDestModLoc,rSrc,fCookedAlpha);//by3dAlphaLookup[red][*lpbyDest][bySrcA];
								dwPixel=_RGB32BIT(by255,(BYTE)rCooked,(BYTE)gCooked,(BYTE)bCooked);
								if (iDestModLoc<BUFFER_SIZE) {
									if (bExplode) {
										//also paint second pixel (image-shaped) for explosion
										rMax=APPROACH(rDest,APPROACH(255.0f,rSrc,fExplodeExp),fCookedAlpha);
										gMax=APPROACH(gDest,APPROACH(255.0f,gSrc,fExplodeExp),fCookedAlpha);
										bMax=APPROACH(bDest,APPROACH(200.0f,bSrc,fExplodeExp),fCookedAlpha);
										dwPixelMax=_RGB32BIT(255,(BYTE)rMax,(BYTE)gMax,(BYTE)bMax);
										lpdwDest[iDestNow]=dwPixelMax;
										fMiddleOfExplosion=.95, fMaxExploder=3.0f, fDistFromEnds;
										fInvMiddle=(1.0f-fMiddleOfExplosion);
										if (fInvMiddle<=0.0f) fInvMiddle=.1f;
										fDistFromEnds=(fExplodedness<fMiddleOfExplosion)
											?(1.0f-fExplodedness/fMiddleOfExplosion)
											:(1.0f-((fExplodedness-fMiddleOfExplosion)/fInvMiddle));
										fDistFromEnds=(fDistFromEnds>0.0f)?sqrt(fDistFromEnds):0;//logorithmic smoothing
										iExploder=(int)(APPROACH(0.0f,fMaxExploder,fDistFromEnds)+.5) +1;
										iDestExp=iDestNow-(int)(iExploder/2)-(int)(iExploder/2)*iDestW;//go to top left of particle
										iOffset=iDestW-iExploder;
										//commented for debug only:
										//for (register int ySubPix=0; ySubPix<iExploder; ySubPix++) {
				                        //    for (register int xSubPix=0; xSubPix<iExploder; xSubPix++) {
										//		if (iDestExp<0||iDestExp>=BUFFER_SIZE) {ySubPix=iExploder;break;}
										//		lpdwDest[iDestExp]=dwPixel;//_RGB32BIT(255,(BYTE)rMax,(BYTE)gMax,(BYTE)bMax);
										//		iDestExp++;
										//	}
										//	iDestExp+=iOffset;
										//}
				 					}// end if bExplode
				 					else lpdwDest[iDestModLoc]=dwPixel;
								}
							}// end if good
						}// end if good
						else {
							break;
						}
					}// end for xDestNow
					if (iDestNow>=BUFFER_SIZE) break;
					if (!bGood) break;
				}// end for yDestNow
			}// end if good (bounds)
		}// end try
		catch (int iExn) { if (ShowError()) cerr<<"int exception in GBuffer_FX_Scaled: "<<iExn<<endl; bShowVars=true; bGood=false; }
		catch (string sExn) { if (ShowError()) cerr<<"Exception error in GBuffer_FX_Scaled: "<<sExn<<endl; bShowVars=true; bGood=false; }
		catch (...) { if (ShowError()) cerr<<"Unknown Exception in GBuffer_FX_Scaled"<<endl; bShowVars=true; bGood=false; }
		if (bShowVars) {
			cerr<<"  { iDestNow:"<<iDestNow<<"; source.w:"<<gbSrc.iWidth<<"; source.h:"<<gbSrc.iHeight<<"; xFlat:"<<xFlat<<"; yFlat:"<<yFlat<<"; fX:"<<fX<<"; fY:"<<fY<<"; xDestNow:"<<xDestNow<<"; yDestNow:"<<yDestNow<<"; fInverseScale:"<<fInverseScale<<"; fScale:"<<fScale<<"; fExplodedness:"<<fExplodedness<<"; }"<<endl;
		}
		return(bGood);
	} //end GBuffer_FX_Scaled
}//end namespace

using namespace ExpertMultimediaBase;
//int SDL_main();
//int main(int argc, char *argv[]) {
//int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR
//	lpCmdLine, unsigned int nCmdShow ) {
int main(int iArgs, char** lpsArg) {
    Console.WriteLine("GameInit:");
    p3dHero.x=0;
    p3dHero.y=0;
    p3dHero.z=0;
	bool bOK=GameInit();
	int iMainReturn=0;//start good
	if (!bOK) {
    	ShowError("GameInit...Failed!");
		//re-implement this w/o using win32 api: MessageBox (0, "GameInit Failed, reinstalling game may fix the error", "Failed to Load Game", MB_ICONHAND);
		iErrors++;
		bDone=true;
		//iMainReturn=1;//"bad...bad program :spank:"
	}
	else if (bSplash) {
		//while (SDL_GetTicks()-iSplashTick<2000) {
			//SleepWrapper(100);
		//}
    	Console.WriteLine("GameInit Done.");
		SleepWrapper(3000);
	}
	else  {
		Console.WriteLine("GameInit Done (did not try to load splash screen, since bSplash is turned off).");
	}
	//main event loop:
   	if (!bDone) Console.WriteLine("Running main event loop.");
   	else ShowError("Bypassing main loop, due to failed init.");
	UInt32 dwLastRefresh=SDL_GetTicks();
	while (!bDone) {
		//Check for events
		while (SDL_PollEvent (&event)) { //main polling loop
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetRelativeMouseState(&xCursorDown, &yCursorDown);//SDL_GetMouseState(&m3dEnt.x, &m3dEnt.y);
				if (event.button.button == SDL_BUTTON_LEFT) {
					xfCursorDown=xCursorDown;
					yfCursorDown=yCursorDown;
					bMouseDown=true;
				}
				else {
					bMouseDownR=true;
					dwPressing|=GAMEKEY_JUMP;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					bMouseDown=false;
				}
				else {
					bMouseDownR=false;
					dwPressing&=GAMEKEY_JUMP^0xFFFFFFFF;
				}
				break;
            case SDL_MOUSEMOTION:
				xCursor=event.motion.x;
				yCursor=event.motion.y;
				xfCursor=xCursor;
				yfCursor=yCursor;
				bMouseMove=true;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym==SDLK_a) {
					DirKeyDown();
					dwPressing|=GAMEKEY_LEFT;
				}
				else if (event.key.keysym.sym==SDLK_d) {
					DirKeyDown();
					dwPressing|=GAMEKEY_RIGHT;
				}
				else if (event.key.keysym.sym==SDLK_w) {
					DirKeyDown();
					dwPressing|=GAMEKEY_UP;
				}
				else if (event.key.keysym.sym==SDLK_s) {
					DirKeyDown();
					dwPressing|=GAMEKEY_DOWN;
				}
				else if (event.key.keysym.sym==SDLK_DELETE) {
					//laser keydown
					dwPressing|=GAMEKEY_FIRE;
				}
				else if (event.key.keysym.sym==SDLK_END) {
					dwPressing|=GAMEKEY_JUMP;
				}
				else if (event.key.keysym.sym==SDLK_ESCAPE) {
					dwPressing|=GAMEKEY_EXIT; //commented for debug only
				}
				else if (event.key.keysym.sym==SDLK_LEFT) {
					dwPressing|=GAMEKEY_FIRE;
					dwPressing|=GAMEKEY_LEFT2;
				}
				else if (event.key.keysym.sym==SDLK_RIGHT) {
					dwPressing|=GAMEKEY_FIRE;
					dwPressing|=GAMEKEY_RIGHT2;
				}
				else if (event.key.keysym.sym==SDLK_UP) {
					dwPressing|=GAMEKEY_FIRE;
					dwPressing|=GAMEKEY_UP2;
				}
				else if (event.key.keysym.sym==SDLK_DOWN) {
					dwPressing|=GAMEKEY_FIRE;
					dwPressing|=GAMEKEY_DOWN2;
				}
				else if (event.key.keysym.sym==SDLK_2) {
					dwPressing|=GAMEKEY_DOUBLESPEED;
				}
				else if (event.key.keysym.sym==SDLK_r) {
					dwPressing|=GAMEKEY_RAPIDFIRE;
				}/*
				else if (event.key.keysym.sym==SDLK_KP_PLUS) {
				//This case is only for testing--smart Bomb to skip level!!
					if (!bBombed) {
						bBombed=true;
						for (int index=0; index<iMaxAliensNow; index++) {
							if (lparrAlien[index] != NULL) {
								delete (Entity*)lparrAlien[index];
								lparrAlien[index]=NULL;
							}
						}
					}
				}*/
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym==SDLK_a) {
					DirKeyUp();
					dwPressing&=(GAMEKEY_LEFT^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_d) {
					DirKeyUp();
					dwPressing&=(GAMEKEY_RIGHT^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_w) {
					DirKeyUp();
					dwPressing&=(GAMEKEY_UP^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_s) {
					DirKeyUp();
					dwPressing&=(GAMEKEY_DOWN^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_DELETE) {
					//iChanLaser=Mix_PlayChannel(2, mcLaser, 0);//chan, sound, #loops
					dwPressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_END) {
					dwPressing&=(GAMEKEY_JUMP^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_ESCAPE) {
					dwPressing&=(GAMEKEY_EXIT^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_LEFT) {
					dwPressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
					dwPressing&=GAMEKEY_LEFT2^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_RIGHT) {
					dwPressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
					dwPressing&=GAMEKEY_RIGHT2^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_UP) {
					dwPressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
					dwPressing&=GAMEKEY_UP2^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_DOWN) {
					dwPressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
					dwPressing&=GAMEKEY_DOWN2^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_2) {
					dwPressing&=GAMEKEY_DOUBLESPEED^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_r) {
					dwPressing&=GAMEKEY_RAPIDFIRE^0xFFFFFFFF;
				}
				break;
			case SDL_QUIT:
				if (!bDone) {
					iGameState=GAMESTATE_SHUTDOWN;//makes GameMain do nothing
					bDone=true;
					Console.Write("GameShutDown...");
					if (GameShutdown()) Console.WriteLine("Success.");
					else Console.WriteLine("Failed!"); //OK since gameshutdown uses cerr
				}
				break;
			default:
				break;
			}//end switch event.type
			if (bDone) break;
		}//end while checking keyboard event

		if (!bDone) {
			//while((SDL_GetTicks() - dwLastRefresh) < 28) { //TODO: improve this and remove loop
				SleepWrapper(1);//TODO: remove to avoid missing keystrokes??
			//}
			//SleepWrapper( (SDL_GetTicks()-dwLastRefresh) + 28)

			//TODO: separate Redraw from Refresh GameMain below

			if ( (SDL_GetTicks()-dwLastRefresh) >= 28 ) {//lock to approx 30fps
				dwLastRefresh=SDL_GetTicks();
				GameMain();
			}
			
			PlaySounds();
			if (bSDLQuitDone) bDone=true;
		}//end if not bDone do normal stuff
	}//end while not bDone continue main event loop
   	Console.Write("Exited main event loop--Stopping Remaining processes...");
	ShutdownMusic();
	if (!bSDLQuitDone) {
		SDL_Quit();
		bSDLQuitDone=true;
	}
	return iMainReturn;
}//end main(...)
#endif
