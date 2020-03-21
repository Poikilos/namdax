#ifndef MAIN_C
#define MAIN_C
//#include <windows.h>
//#define LINUX

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//#ifdef LINUX
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <math.h>
#include <SDL/SDL.h>
//#include <SDL/SDL_mixer.h>
//#include <SDL/SDL_ttf.h>
#include <al/al.h>
#include <al/alc.h>
#include <al/alu.h>
#include <al/alut.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
//next line is for OGG!
#define BUFFER_SIZE (4096 * 8)
*/
//#else
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
//#include <SDL/SDL_mixer.h>
//#include <SDL/SDL_ttf.h>
#include <AL/al.h>//<AL/al.h>
#include <AL/alc.h>//<AL/alc.h>
#include <AL/alut.h>//<AL/alut.h>
//TODO: finish this--uncomment: #include <ogg/ogg.h>
//TODO: finish this--uncomment: #include <vorbis/codec.h>
//TODO: finish this--uncomment: #include <vorbis/vorbisenc.h>
//TODO: finish this--use: #include <vorbis/vorbisfile.h>

//next line is for OGG!
#define BUFFER_SIZE (4096 * 8)

//#endif


#include <sprite.cpp>
#include <world.cpp>
#include <gfont32bgra.cpp>
#include <anim32bgra.cpp>
#include <gbuffer32bgra.cpp>
#include <gradient32bgra.cpp>
#include <targa.cpp>
#include <byter.cpp>
#include <base.cpp>
#include <frameworkdummy.cpp>

//#include "C:\My Documents\Projects-cpp\Base\sprite.cpp"
//#include "C:\My Documents\Projects-cpp\Base\world.cpp"
//#include "C:\My Documents\Projects-cpp\Base\anim32bgra.cpp"
//#include "C:\My Documents\Projects-cpp\Base\gbuffer32bgra.cpp"
//#include "C:\My Documents\Projects-cpp\Base\targa.cpp"
//#include "C:\My Documents\Projects-cpp\Base\byter.cpp"
//#include "C:\My Documents\Projects-cpp\Base\base.cpp"
//#include "C:\My Documents\Projects-cpp\Base\frameworkdummy.cpp"

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
	#define SOUND_BUFFERS 15
	#define SOUND_SOURCES 20
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
	#define BUFFER_PIXELS			1228800 //1280x960
	#define SCREEN_OFFSET_X			320 //screen (a GBuffer) is offset to avoid clipping
	#define SCREEN_OFFSET_Y			240
	#define FSCREEN_OFFSET_X		320.0f //screen (a GBuffer buffer) is offset to avoid clipping
	#define FSCREEN_OFFSET_Y		240.0f
	#define SCREEN_BPP			32	 //bits per pixel, GameInit tries 32 or 24 if one is not available
	#define CODE_THRESHOLD			90
	//States for game loop
	#define GAMESTATE_INIT			0
	#define GAMESTATE_START_AREA		1
	#define GAMESTATE_START_ENCOUNTER	2
	#define GAMESTATE_RUN			3
	#define GAMESTATE_SHUTDOWN		4
	#define GAMESTATE_EXIT			5
	#define GAMESTATE_WIN_ENCOUNTER		6
	#define GAMESTATE_WIN_GAME		7
	#define GAMESTATE_YOU_LOSE		8
	#define GAMESTATE_LOSEGUY		9
	//Operations for Explosion
	#define EXPLOSION_CHECK_COUNT		0
	#define EXPLOSION_START			1
	#define EXPLOSION_CONTINUE		2
	#define PI				3.1416
	//States of hero or enemies
	#define STATUS_ALIVE			1
	#define STATUS_SHIELD			2
	#define STATUS_DOUBLESPEED		4
	#define STATUS_ANTIGRAVITY		8
	#define STATUS_SHOOTER			16
	#define STATUS_BOMBER			32
	#define STATUS_BOSS			64
	#define STATUS_RAPIDFIRE		256
	#define STATUS_AIMBOMBS			512 //AIM AI
	#define STATUS_VARIABLESHIELD		1024

	#define GAMEKEY_UP			1
	#define GAMEKEY_DOWN	 		2
	#define GAMEKEY_LEFT	 		4
	#define GAMEKEY_RIGHT			8
	#define GAMEKEY_FIRE	 		16
	#define GAMEKEY_JUMP	 		32
	#define GAMEKEY_EXIT	 		64
	#define GAMEKEY_RAPIDFIRE 		128
	#define GAMEKEY_DOUBLESPEED		512
	#define GAMEKEY_UP2			1024
	#define GAMEKEY_DOWN2	 		2048
	#define GAMEKEY_LEFT2			4096
	#define GAMEKEY_RIGHT2			8192
	UInt32 u32Pressing=0;
	#define SCREENITEM_ALIEN	1
	#define SCREENITEM_SHOT		2
	#define SCREENITEM_HERO		3
	#define SCREENITEM_3DCURSOR 4
	#define MAXSHOTS		50
	#define MAX_SCREENITEMS		58  //maximum number of objects to render per frame
	#define MAX_ZORDER		50   //maximum zOrder value - INCLUSIVE
	#define FMAX_ZORDER		50.0f
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
	//TODO: finish this--uncomment: ogg_stream oggMusic;//Mix_Music *music=NULL;
	GFont gfontMain;
	ALfloat arralfListenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat arralfListenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat arralfListenerRot[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };//(at,at,at,up,up,up)
	bool bClosedAudio=false;
	const REAL Base_r0=(REAL)0;
	const REAL Base_r1000=(REAL)1000.0;
	FPOINT3D p3dHero;
	bool bDebug=true;
	Mass3d m3dDebug[9];
	Mass3d m3dHeroEye, m3dCursor;
	//float xAimFlipper=-1.0f;
	//float yAimFlipper=-1.0f;
	//float zAimFlipper=-1.0f;
	bool bFlipYaw=false;
	bool bFlipPitch=false;
	float fBoss=0;//(Entity*)arrpentAlien[iBoss]->fHealth;
	float fBossMax=1;//(Entity*)arrpentAlien[iBoss]->fMaxHealth;
	int iBoss=-1;//global link to boss;
	bool bSDLQuitDone=false;
	Variables settings;
	Variables dat;
	bool bSDLQuitSent=false;
	bool bSplash=false;//if splash screen was drawn
	extern BYTE by3dAlphaLookup[256][256][256]; //lookup for alpha result: ((Source-Dest)*alpha/255+Dest)
	UInt32 arru32EnergyGrad[256];//gradient for energy currency (ready attack) meter
	UInt32 arru32HealthGrad[256];//gradient for health meter
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
		iEncounters=3;

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
	Anim				animBackdrop;
	GBuffer			gbIntro;
	GBuffer			gbIntroText;
	GBuffer			gbSymbolShield;
	GBuffer			gbSymbolBossHealth;
	GBuffer			gbGuysDecor;
	Anim				animFlyer;
	Anim				animFlyerShadow;
	Anim				animBoss;
	Anim				animBossShadow;
	Anim				animHero;
	Anim				animHeroShadow;
	Anim				animShot;
	Anim				animShotShadow;
	Anim				animGameScreen;
	Anim				animGameScreenArea;
	Anim				animGameScreenEncounter;
	Anim				animCursor;
	GBuffer			gbScreen;
	//LPSPRITE*				lpspritearr;//sprite array
	SCREENITEM				screenitemarr[MAX_SCREENITEMS];
	int explosionResult=0;
	int iGameState=GAMESTATE_INIT;
	int iArea=1;		//reset later in GAME_STATE_INIT			//3 Areas total
	int iEncounter=1; //set to 1 again later in GAME_STATE_INIT					//3 Encounters total, encounters are in Areas
	int iAliens=0;
	int iMaxAliens=4;//changed later using ini file
	int iMaxAliensNow=iMaxAliens;//changes by level
	bool bPlayTrumpet=false;
	string sMusic="";
	bool bLoopMusic=false;
	// Buffers hold sound data.
	ALuint aluiBuffer;
	
	// Sources are points emitting sound.
	ALuint aluiSource;
	/*
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
	*/
	/*
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
	*/
	

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
	void DrawRadarDot(float xPos, float yPos, float zPos, UInt32 u32Pixel);
	void DrawRadarDot(float xPos, float yPos, float zPos, UInt32 u32Pixel, bool bFilled);
	void DrawRadarRect(float left, float top, float bottom, float right, UInt32 u32Pixel, bool bFilled);
	void DrawExclusiveRect(int left, int top, int bottom, int right, UInt32 u32Pixel, bool bFilled);
	void DrawRadarField();
	void ShowDebugVars();
	//bool SafeChunkUnload(Mix_Chunk* &mcToNull);
	void RadarDotAt(int &xPix, int &yPix, float xPos, float yPos);
	float RadarCenterX();
	float RadarCenterY();
	float RadarHeight();
	void PlaySound(string sInstanceName, string sSoundName);
	void PlaySound(string sInstanceName, string sSoundName, Mass3d& m3dLoc);
	void PlaySound(string sInstanceName, string sSoundName, Mass3d& m3dLoc, Mass3d& m3dVel);
	void PlaySounds();
	void ClearSounds();
	void Refresh3dCursor();
	void Draw3dCursor(byte byBrightness);
	void SayWhatIDrewIfFalse(bool bDrawFunctionReturn, int iEntityType, string sDescription);
	string EntityTypeToString(int iTypeX);
	void DrawCube(Mass3d &m3dNow, Pixel &pixelNear, Pixel &pixelFar);
	bool DrawScreen();
	inline void SleepWrapper(int iTicks);
	void LoadSequence(Anim &animToLoad, string sFileBaseNameOnly, int iFramesExpectedElseError);
	void LoadImage(GBuffer &gbToLoad, string sFile);
	bool GBuffer_FX_Scaled(GBuffer &gbDest, GBuffer &gbSrc, int xFlat, int yFlat,
			float fOpacity, float fExplodedness, UInt32 u32Stat, float fScale);
	REAL Base_GetSeconds();
	REAL Base_SecondsSince(REAL rSecondCount);
	
	////////////////////////////////// CLASSES /////////////////////////////////////

	////////////////////////////////// CAMERA //////////////////////////////////////
	class SoundInstance {
	public:
		ALfloat arralfPos[3];
		ALfloat arralfVel[3];
		string sName;
		int iSound;
		void SetLoc(Mass3d& m3dLoc);
		void SetVel(Mass3d& m3dLoc);
	private:
	};
	void SoundInstance::SetLoc(Mass3d& m3dLoc) {
		//TODO: finish this: adjust these
		arralfPos[0]=m3dLoc.x;
		arralfPos[1]=m3dLoc.y;
		arralfPos[2]=m3dLoc.z;
	}
	void SoundInstance::SetVel(Mass3d& m3dVel) {
		//TODO: finish this: adjust these if OpenAl is not cartesian
		arralfPos[0]=m3dVel.x;
		arralfPos[1]=m3dVel.y;
		arralfPos[2]=m3dVel.z;
	}
	class Sound {
	public:
		string sName;
		Sound();
	};
	Sound::Sound() {
		sName="";
	}
	bool bSounder=false;
	class Sounder {
	public:
		Sounder();
		void Refresh();
		void Load(string sName, string sFile);
		void SetOrCreateInstance(string sName, string sSoundName, Mass3d& m3dLoc);
		void SetOrCreateInstance(string sName, string sSoundName, Mass3d& m3dLoc,Mass3d& m3dVel);
		void StopInstance(string sName);
		void Clear();
		int GetFreeInstanceIndex();
		int IndexOfSound(string sName);
		int IndexOfInstance(string sName);
		void Close();
	private:
		Sound soundarr[SOUND_BUFFERS];
		ALuint aluiarrBuffer[SOUND_BUFFERS];//corresponds to soundarr
		SoundInstance instancearr[SOUND_SOURCES];
		ALuint aluiarrSource[SOUND_SOURCES];//corresponds to instancearr
		int iSoundarrLen;
		int iInstancearrLen;
		int iSounds;
		void Init();
	};
	Sounder::Sounder() {
	}
	void Sounder::Init() {
		iSoundarrLen=SOUND_BUFFERS;
		iInstancearrLen=SOUND_SOURCES;
		int iNow;
		for (iNow=0; iNow<iSoundarrLen; iNow) {
			soundarr[iNow].sName="";
		}
		iSounds=0;
		//TODO: finish this--uncomment: alGenBuffers(SOUND_BUFFERS, aluiarrBuffer);
		//TODO: finish this--uncomment: alGenSources(SOUND_SOURCES, aluiarrSource);

		if (!bSounder) {
			try {
				//Init OpenAL and clear error bit.
				//TODO: finish this--uncomment: alutInit(NULL, 0);
				//TODO: finish this--uncomment: alGetError();//TODO: report the result
				//if (LoadALData() == AL_FALSE)
				//	return 0;
				//TODO: finish this--uncomment: alListenerfv(AL_POSITION, arralfListenerPos);
				//TODO: finish this--uncomment: alListenerfv(AL_VELOCITY, arralfListenerVel);
				//TODO: finish this--uncomment: alListenerfv(AL_ORIENTATION, arralfListenerRot);
				bSounder=true;
			}
			catch (exception& exn) {
				ShowException(exn,"Sounder Init");
			}
		}
		else ShowErr("Sounder was opened.");
	}//end Init
	void Sounder::Load(string sFile, string sName) {
		ALenum format;
		ALsizei size;
		ALvoid* data;
		ALsizei freq;
		ALboolean loop;
		const char* szFile=sFile.c_str();
		//TODO: finish this--uncomment: alutLoadWAVFile((ALbyte*)szFile, &format, &data, &size, &freq, &loop);
		//TODO: finish this--uncomment: alBufferData(aluiarrBuffer[iSounds], format, data, size, freq);
		//TODO: finish this--uncomment: alutUnloadWAV(format, data, size, freq);
		soundarr[iSounds].sName=sName;
		iSounds++;
	}
	void Sounder::Close() {
		if (bSounder) {
			//TODO: finish this--uncomment: alDeleteBuffers(SOUND_BUFFERS, &aluiarrBuffer[0]);
			//TODO: finish this--uncomment: alDeleteSources(SOUND_SOURCES, &aluiarrSource[0]);
			//TODO: finish this--uncomment: alutExit();
			bSounder=false;
		}
		else ShowErr("Sounder was closed.");
	}
	void Sounder::Refresh() {
	}
	int Sounder::IndexOfSound(string sName) {
		int iReturn=-1;
		for (int iNow=0; iNow<iSoundarrLen; iNow++) {
			if (soundarr[iNow].sName==sName && soundarr[iNow].sName!="") {
				iReturn=iNow;
				break;
			}
		}
		return iReturn;
	}
	int Sounder::IndexOfInstance(string sName) {
		int iReturn=-1;
		for (int iNow=0; iNow<iInstancearrLen; iNow++) {
			if (instancearr[iNow].sName==sName && instancearr[iNow].sName!="") {
				iReturn=iNow;
				break;
			}
		}
		return iReturn;
	}
	int Sounder::GetFreeInstanceIndex() {
		int iReturn=-1;
		for (int iNow=0; iNow<iInstancearrLen; iNow++) {
			if (instancearr[iNow].sName=="") {
				iReturn=iNow;
				break;
			}
		}
		return iReturn;
	}
	void Sounder::StopInstance(string sName) {
		//TODO: finish this
	}
	void Sounder::SetOrCreateInstance(string sName, string sSoundName, Mass3d& m3dLoc) {
		Mass3d m3dTemp;
		SetOrCreateInstance(sName, sSoundName, m3dLoc, m3dTemp);
	}
	void Sounder::SetOrCreateInstance(string sName, string sSoundName, Mass3d& m3dLoc, Mass3d& m3dVel) {
		bool bGood=false;
		bool bPlay=false;
		int iAt=IndexOfInstance(sName);
		if (iAt<0) {
			iAt=GetFreeInstanceIndex();
			bPlay=true;
		}
		if (iAt>=0) { //if room
			instancearr[iAt].sName=sName;
			instancearr[iAt].iSound=IndexOfSound(sSoundName);
			instancearr[iAt].SetLoc(m3dLoc);
			instancearr[iAt].SetVel(m3dVel);
			//TODO: finish this--uncomment: alSourcei (aluiarrSource[iAt], AL_BUFFER,   aluiarrBuffer[instancearr[iAt].iSound]);
			//TODO: finish this--uncomment: alSourcef (aluiarrSource[iAt], AL_PITCH,    1.0f);
			//TODO: finish this--uncomment: alSourcef (aluiarrSource[iAt], AL_GAIN,     1.0f);
			//TODO: finish this--uncomment: alSourcefv(aluiarrSource[iAt], AL_POSITION, instancearr[iAt].arralfPos);
			//TODO: finish this--uncomment: alSourcefv(aluiarrSource[iAt], AL_VELOCITY, instancearr[iAt].arralfVel);
			//TODO: finish this--uncomment: alSourcei (aluiarrSource[iAt], AL_LOOPING,  AL_FALSE);
			if (bPlay) {
				//ALint state;
				//TODO: finish this--uncomment: alGetSourcei(aluiarrSource[iAt], AL_SOURCE_STATE, &state);
				//TODO: finish this--uncomment: if (state==AL_PLAYING) bPlay=false;
			}
			//TODO: finish this--uncomment: if (bPlay) alSourcePlay(aluiarrSource[iAt]);
			bGood=true;
		}
		//TODO: finish this--uncomment: if(alGetError() != AL_NO_ERROR)
		//TODO: finish this--uncomment: 	bGood=false;
	}
	void Sounder::Clear() {
		
	}

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
	    bool PointIsInView(float x3d, float y3d, float z3d);
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
			bGood=false;
			ShowException(exn,"Point2dFrom3d");
		}
		catch (...) {
			bGood=false;
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
			bGood=false;
			ShowException(exn,"Mass2dFrom3d");
		}
		catch (...) {
			bGood=false;
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

			//CONVERTS TO METERS:
			fDistance/=fDestPixPerUnitHere;
			pointOrig.yRot=fAltitude;
			pointOrig.zRot=fAzimuth;
			Travel3d(pointOrig, pointOrig.yRot, pointOrig.zRot, fDistance);

			////TODO:? self-moving test dest vars:
			//static float fTestAlt=90.0f, fTestAz=0.0f;
			//static float fSecLastTest=(float)Base_GetSeconds();
			//float fSecondsPassed=(float)Base_GetSeconds()-fSecLastTest;
			//static float fAzDegPerSec=360.0f;
			//static float fAltDegPerSec=-180.0f/20.0f; //-180deg in 20s
			//fTickLastTest=(float)Base_GetSeconds();
			//fTestAlt+=fAltDegPerSec*fSecondsPassed;
			//fTestAz+=fAzDegPerSec*fSecondsPassed;
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
			bGood=false;
			ShowException(exn,"Point3dMoveAndRotateBy2d");
		}
		catch (...) {
			bGood=false;
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
			bGood=false;
			ShowException(exn,"Point3dFrom2d");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("Point3dFrom2d");
		}
		return bGood;
	}//end Point3dFrom2d
	bool Camera::Point2dRatiosFrom3d(float &fReturnRightness, float &fReturnDownness, float x3d, float y3d, float z3d) {
	//gets screen point from 3d point
		bool bGood=true;
		try {
			//debug assumes there is no m3dCam.y rotation
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
			bGood=false;
			ShowException(exn,"Point2dRatiosFrom3d");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("Point2dRatiosFrom3d");
		}
		return bGood;
	}//end Point2dRatiosFrom3d
	bool Camera::PointIsInView(float x3d, float y3d, float z3d) {
		bool bGood=false;
		bool bReturn=false; //whether 3d point is on screen
		try {
			//debug assumes there is no m3dCam.y rotation
			static float fAngleFromTop; //angle of point from top of aperture
			static float fAngleFromLeft; //angle of point from left of aperture
			static float xRel, yRel, zRel;//points relative to Camera;
			xRel=x3d-m3dCam.x;
			yRel=y3d-m3dCam.y;
			zRel=z3d-m3dCam.z;
			//the next line may assume camera is facing toward positive m3dCam.y
			fAngleFromTop=(fAngleApertureTop-(THETAOFXY(yRel,zRel)));
			if (fAngleFromTop<0.0f) return false;
			else if (fAngleFromTop>=fApertureH);
			fAngleFromLeft=(fAngleApertureLeft-(THETAOFXY(xRel,yRel)));
			if (fAngleFromLeft<0.0f) return false;
			else if (fAngleFromLeft>=fApertureW);
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"PointIsInView");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("PointIsInView");
		}
		return bGood;
	}//end PointIsInView
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
	Sounder sounder;
	///////////////////////////////////////////////////////////
	// void* ENTITY POINTERS GLOBAL TO OTHER ENTITIES ////////////////////////////
	void* arrpentShot[MAXSHOTS];
	void** arrpentAlien;
	///////////////////////////////////////////////////
	//Entity Class
	class Entity {
	public:
		float fShield;
		bool bHitSoundWasPlayed;
		bool bRotationTakesPrecedence;
		bool bCycleFrames;
		int iBoundaryAction;
		REAL rSecLastHitGroundSound;
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
		REAL rFortitude;
		float fSpeedMultiplier;
		bool bDraw;
		int iType;
		float fShadowOpacity;
		//float fFloatHeight;
		float zRotSpeed;
		float fShootMetersPerSec;
		Anim* lpanimMain; // animation cycle (pointer to any global anim)
		Anim* lpanimShadow;
		REAL rSecondsMoveDelay;
		REAL rSecondsAimDelay;
		REAL rSecondsShootDelay;
		UInt32 u32Status;
		float fHealth;
		float fExplodedness;
		float fPower; //gun power in percent, max=1
		bool bExploding;
		bool bGravity;
		bool bMustMove;

		void InitNull();
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
		void SetDoubleSpeed();
		void SetRapidFire();
		void DrawTarget(int alienNum);
		void GetHit(int iShotIndex);
		void DamageDetect();
		void Draw();
		void Translate3D();
		float EyeX();
		float EyeY();
		float EyeZ();
		int iIndex;//my index in entity array
		void DamageTurf();
		void SetVars(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, bool IsAlien, bool IsBomb);
		float MetersFromPixelsWhereIAm(float fPixels);
		void Stop();
		bool HasAttrib(UInt32 bit);
		void RemoveAttrib(UInt32 bit);
		void AddVariableShield(float fSetShieldTo);
	private:
		//shot vars:
		REAL rSecStart;
		REAL rSecondsLife;
		bool bFirstRun;
		int iDir;
		REAL rSecLastRefresh;
		REAL rSecLastMetricMove; //only used for metric movement as of 2007

		void DrawMeters();
		int FrameFromRot();
		void ShowDebugInfo();
		//float BottomOffsetHere();
	};
	void Entity::InitNull() {
		lpanimMain=NULL;
		lpanimShadow=NULL;
		fShield=0.0f;
		bHitSoundWasPlayed=false;
	    bCycleFrames=false;
	    bGravity=false;
	    iBoundaryAction=BOUNDARY_BOUNCE;
	    rSecLastHitGroundSound=Base_GetSeconds();
		bExploding=false;
		fExplodedness=0.0f;
		bAlien=false;
		bBomb=false;
		rSecondsMoveDelay=0;
		rSecondsAimDelay=0;
		rSecondsShootDelay=0;
		fExplodedness=0;
		//fFloatHeight=0.0f;
		fLaserSusceptibility=.05;
		rSecLastRefresh=Base_GetSeconds();
		rSecLastMetricMove=Base_GetSeconds();
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
		rFortitude=1.0f;
		fSpeedMultiplier=1.0f;
		rSecondsLife=0.0f;
		rSecStart=Base_GetSeconds();
		
	}//end Init()
	Entity::Entity(int iTypeX) {
		InitNull();
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
					rFortitude=0.2f;
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
					u32Status=STATUS_ALIVE;// | STATUS_ANTIGRAVITY;
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
				catch (exception& exn) {
					ShowException(exn,"Entity constructor");
				}
				catch (...) {
					ShowUnknownException("Entity constructor");
				}
			}//end if ENTITY_TYPE_HERO
			else if (iType==ENTITY_TYPE_SHOT) {
				Console.Error.WriteLine("Created a useless shot using no constructor parameters.");
			}//end if ENTITY_TYPE_SHOT
			else if (iType==ENTITY_TYPE_ALIEN) {
				Console.Error.WriteLine("Created a useless alien using no constructor parameters.");
			}//end if ENTITY_TYPE_ALIEN
		}
		catch (exception& exn) {
			ShowException(exn,"Entity constructor by type");
		}
		catch (...) {
			ShowUnknownException("Entity constructor by type");
		}
		//TODO: this constructor isn't used except for hero.  move to an overload of init and make that the init overload that contains the init code
		m3dEnt.zRotDest=m3dEnt.zRot;
	}//END CONSTRUCTOR BY iTypeX
	Entity::Entity(int iTypeX, Mass3d m3dDirectionAndLocation, float fMetersPerSec, bool IsAlien, bool IsBomb) {
		InitNull();
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
				u32Status=STATUS_ALIVE;
				fShadowOpacity=.25;
				m3dEnt.xMin=-10;
				m3dEnt.xMax=10;
				m3dEnt.yMin=-12;
				m3dEnt.yMax=30;
				m3dEnt.zMax=40.0f;
				rSecondsLife=7.0f;

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
				dat.GetOrCreate(rSecondsLife,"shot.life.seconds");
			}
			catch (exception& exn) {
				ShowException(exn,"Entity constructor using Shot angle and Shot Variables");
			}
			catch (...) {
				ShowUnknownException("Entity constructor using Shot angle and Shot Variables");
			}
		}//end if ENTITY_TYPE_SHOT
	  m3dEnt.zRotDest=m3dEnt.zRot;
	}//end shot constructor by Mass3d
	Entity::Entity(int iTypeX, float x2, float y2, float z2, float xVel2, float yVel2, float zVel2, bool IsAlien, bool IsBomb) {
		InitNull();
		iType=iTypeX;
		bool bGood=true;
		if (iType==ENTITY_TYPE_SHOT) {
	        bCycleFrames=true;
	        iBoundaryAction=BOUNDARY_DIE;
			try {
				u32Status=STATUS_ALIVE;
				fShadowOpacity=.25;
				m3dEnt.xMin=-10;
				m3dEnt.xMax=10;
				m3dEnt.yMin=-12;
				m3dEnt.yMax=30;
				m3dEnt.zMax=40.0f;
				rSecondsLife=7.0f;
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
				dat.GetOrCreate(rSecondsLife,"shot.life.seconds");
			}
			catch (exception& exn) {
				ShowException(exn,"Entity constructor using Shot angle and Shot Variables");
			}
			catch (...) {
				ShowUnknownException("Entity constructor using Shot angle and Shot Variables");
			}
		}//end if ENTITY_TYPE_SHOT
	  m3dEnt.zRotDest=m3dEnt.zRot;
	}//END CONSTRUCTOR BY SHOT ANGLE AND SHOT VARIABLES
	Entity::Entity(int iTypeX, float x2, float y2, float z2) {
		InitNull();
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
				u32Status=STATUS_ALIVE;
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
				rSecondsShootDelay=.66;
				lpanimMain=&animFlyer;
	 			m2dEnt.Init(lpanimMain->Width(),lpanimMain->Height(),150.0f);
				settings.GetOrCreate(m2dEnt.fPixelsPerMeter,"flyer.size.pixelspermeter");
				m2dEnt.SetHitRect(107,91,159,254); //TODO: finish this: set hit rect automatically using image, EVERY TIME SetHitRect is called since frame changes it
				//m2dEnt.fScale=1.0f;
				//m2dEnt.SetPixCenter(104,(int)m2dEnt.FCenterYRelScaled());

				lpanimShadow=&animFlyerShadow;
	 			m2dShadow.Init(lpanimShadow->Width(),lpanimShadow->Height(),m2dEnt.fPixelsPerMeter);

				if (iArea==2) u32Status |= STATUS_SHOOTER;
				else if (iArea==3) u32Status |= STATUS_BOMBER;
				if (iArea==3 && iEncounter==3) u32Status |= STATUS_SHIELD;
	        	fLaserSusceptibility=.0265;//.0265f;
				dat.GetOrCreate(fLaserSusceptibility,"flyer.fLaserSusceptibility");
			}
			catch (exception& exn) {
				ShowException(exn,"Entity constructor using location");
			}
			catch (...) {
				ShowUnknownException("Entity constructor using location");
			}
		}//end if ENTITY_TYPE_ALIEN
	  m3dEnt.zRotDest=m3dEnt.zRot;
	}//end constructor by location
	Entity::~Entity() {
	}
	////////////////////           end constructors          /////////////////////////////
	void Entity::SetAsBoss() {
		bool bGood=false;
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

					u32Status=STATUS_ALIVE | STATUS_BOMBER | STATUS_SHIELD | STATUS_BOSS | STATUS_SHOOTER | STATUS_AIMBOMBS;

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
					//sound names: bomb, laser-alien, laser, explosion, ouchalien, ouchzap, shieldzap
					//	blerrp, hitdirt, jump, scrapeground, angryalien, trumpet, thruster, laser
					PlaySound("a.angry"+ToString(iIndex),"angryalien",m3dEnt);
					rSecondsShootDelay=.1333;
					fLaserSusceptibility=.05;//.0265;
					dat.GetOrCreate(fLaserSusceptibility,"boss.fLaserSusceptibility");
					fShadowOpacity=.35;
					bGood=true;
					dat.GetOrCreate(fShadowOpacity,"boss.shadow.multiplier");
					settings.Save();
				}
				catch (exception& exn) {
					bGood=false;
					ShowException(exn,"initializing boss");
				}
				catch (...) {
					bGood=false;
					ShowUnknownException("initializing boss");
				}
			}
			catch (exception& exn) {
				bGood=false;
				ShowException(exn,"loading boss");
			}
			catch (...) {
				bGood=false;
				ShowUnknownException("loading boss");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"enabling boss");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("enabling boss");
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
	void Entity::TurnAll(float fAltitude, float fAzimuth) {
		//TODO: finish this
	}
	int Entity::FrameFromRot() {
		int iFrameTest;
		//if (!bUsePitchYaw && iType!=ENTITY_TYPE_HERO) m3dEnt.zRot=THETAOFXY(m3dEnt.xVel,m3dEnt.yVel);
		float fDeg=SafeAngle360(m3dEnt.zRot);
		if (fDeg<=90.0f) iFrameTest=(int)APPROACH(0,15,fDeg/90.0f);
		else if (fDeg<=180.0f) iFrameTest=(int)APPROACH(15,30,(fDeg-90.0f)/90.0f);
		else if (fDeg<=270.0f) iFrameTest=(int)APPROACH(30,45,(fDeg-180.0f)/90.0f);
		else iFrameTest=(int)APPROACH(45,60,(fDeg-270.0f)/90.0f);
		
		if (iFrameTest<0) iFrameTest=0;
		else if (lpanimMain==null || iFrameTest>=lpanimMain->IFrames()) {
			if (ShowErr()) cerr<<"Warning: FrameFromRot("<<fDeg<<") (was "<<m3dEnt.zRot<<") is "<<iFrameTest<<endl;
			iFrameTest=0;
		}
		//static REAL rSecondsSinceLastShow=Base_GetSeconds();
		//if (Base_SecondsSince(rSecondsSinceLastShow)>=1) {
		//	rSecondsSinceLastShow=Base_GetSeconds();
		//	cerr<<"FrameFromRot() "<<m3dEnt.zRot<<" is "<<iFrameTest<<endl;
		//}
		//static int iTest=0;
		//if (iTest<1000 && iType==ENTITY_TYPE_ALIEN) {
		//	cout<<"FrameFromRot "<<m3dEnt.zRot<<" (cropped to "<<fDeg<<") is "<<iFrameTest<<"."<<endl;
		//	iTest++;
		//}
		return iFrameTest;
	}
	void Entity::TurnIncrement(bool bPositiveAngle) {
		if (bPositiveAngle) m3dEnt.zRotDest=m3dEnt.zRot+m3dEnt.zRotVel*(Base_SecondsSince(rSecLastRefresh));
		else m3dEnt.zRotDest=m3dEnt.zRot-m3dEnt.zRotVel*(Base_SecondsSince(rSecLastRefresh));
	}
	void Entity::Turn(float fDeg) {
		m3dEnt.zRotDest=SafeAngle360(fDeg);
	}
	void Entity::TurnPitchIncrement(bool bUp) {
		if (bUp) m3dEnt.yRotDest=m3dEnt.yRot+(m3dEnt.yRotVel*(Base_GetSeconds()-rSecLastRefresh));
		else m3dEnt.yRotDest=m3dEnt.yRot-(m3dEnt.yRotVel*(Base_GetSeconds()-rSecLastRefresh));
	}
	void Entity::TurnPitch(float fDeg) {
		m3dEnt.yRotDest=SafeAngle360(fDeg);
		if (m3dEnt.yRotDest<m3dEnt.yRotMin)m3dEnt.yRotDest=m3dEnt.yRotMin;
		else if (m3dEnt.yRotDest>m3dEnt.yRotMax)m3dEnt.yRotDest=m3dEnt.yRotMax;
	}
	void Entity::Jump() {
		if (rSecondsMoveDelay<=Base_r0) {
			if ((m3dEnt.z<=m3dEnt.zMin) || (u32Status & STATUS_ANTIGRAVITY)) {//don't unless not jumping or if ANTIGRAVITY status
				m3dEnt.zVel=(u32Status & STATUS_ANTIGRAVITY) ? .07f :.42f;//m3dEnt.zVel=40.0f;//debug this
				m3dEnt.z+=m3dEnt.zVel;//push us out of the way of ground detection
				if (u32Status & STATUS_DOUBLESPEED) m3dEnt.zVel *= 2.0f;
				if (!(u32Status & STATUS_ANTIGRAVITY)) {//if no antigravity, delay movement in air
					rSecondsMoveDelay= (u32Status & STATUS_DOUBLESPEED) ? .5 : 1.0;
					PlaySound("h.thruster","thruster",m3dEnt);
				}
			}
		}
	}
	void Entity::Move(int xDir, int yDir) {
		static float fDist=.2f;
		if (rSecondsMoveDelay<=Base_r0) {
			if (u32Status&STATUS_DOUBLESPEED) fDist=.4f;
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
			if ((rSecondsShootDelay<=Base_r0) && fPower>=fPowerPerShot) {
				if (u32Status&STATUS_RAPIDFIRE) {
					rSecondsShootDelay=.0667;
				}
				else {
					fPower-=fPowerPerShot;
					rSecondsShootDelay=.3333;
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
					if (arrpentShot[iShotNow] == NULL) {//create a shot at the first available shot pointer
						arrpentShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, false, false);
						((Entity*)arrpentShot[iShotNow])->iIndex=iShotNow;
						PlaySound("h.laser"+ToString(iShotNow),"laser",m3dWeapon);
						break;
					}
				}
			}//end if able to shoot
		}//end if ENTITY_TYPE_HERO
		else if (iType==ENTITY_TYPE_ALIEN) {
			if (rSecondsShootDelay<=Base_r0) {
				rSecondsShootDelay=((REAL)(IRandPositive()%30+10))/(REAL)30.0;
				static Mass3d m3dWeapon;
				m3dWeapon.x=m3dEnt.x-m3dEnt.xSize/2.0f;
				m3dWeapon.y=m3dEnt.y;
				m3dWeapon.z=m3dEnt.z;
				m3dWeapon.zRot=(iDir==-1)?180.0f:0;
				m3dWeapon.yRot=0;
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (arrpentShot[iShotNow] == NULL) {
						PlaySound("a.laser"+ToString(iShotNow),"laser-alien",m3dEnt);
						arrpentShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, bAlien, false);
						((Entity*)arrpentShot[iShotNow])->iIndex=iShotNow;
						break;
					}
				}
			}
		}//end if ENTITY_TYPE_ALIEN

	}
	void Entity::Shoot(float fPitch, float fYaw) {
		if (iType==ENTITY_TYPE_HERO) {
			if ((rSecondsShootDelay<=Base_r0) && fPower>=fPowerPerShot) {
				if (u32Status&STATUS_RAPIDFIRE) {
					rSecondsShootDelay=.0667;
				}
				else {
					fPower-=fPowerPerShot;
					rSecondsShootDelay=.3333;
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
					if (arrpentShot[iShotNow] == NULL) {//create a shot at the first available shot pointer
						arrpentShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, false, false);
						((Entity*)arrpentShot[iShotNow])->iIndex=iShotNow;
						PlaySound("h.laser","laser",m3dEnt);
						break;
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

			//TODO: need to account for pausing in rSecLastRefresh
		REAL rNow=Base_GetSeconds();
		REAL rSecondsSinceLastRefresh=rNow-rSecLastRefresh;

		if (true) { //!bFirstRun) {
			// PHYSICS //
			if (bGravity) m3dEnt.zVel-=(float)(rSecondsSinceLastRefresh*(REAL).8); //only .8m/s/s gravitational acceleration for high jumping //earth gravitational acceleration is 1.8 m/s/s
			//if ((rSecondsSinceLastRefresh)>.3f) //increases granularity to improve division accuracy
				m3dEnt.RotateTowardDest(rSecondsSinceLastRefresh);
			//m3dEnt.zRot=m3dEnt.zRotDest;
			//if (m3dEnt.yRot!=m3dEnt.yRotDest) m3dEnt.yRot=m3dEnt.yRotDest;
			//if (bRotationTakesPrecedence && m3dEnt.zRot!=m3dEnt.zRotDest) {
			//	 m3dEnt.zRot=m3dEnt.zRotDest;
			//}

			float fSpeedMultNow=rFortitude*fSpeedMultiplier;
			if (HasAttrib(STATUS_BOSS)) fSpeedMultNow=1.0f;


			if (bUsePitchYaw) {
				if ( (bMustMove)||((rNow-rSecLastMetricMove)>.01f) ) {//minimum difference helps division accuracy on high frame rates
					int iTest=0;
					float fDist;
					fDist=fMetersPerSecond*(rNow-rSecLastMetricMove);
					Travel3d(m3dEnt, m3dEnt.yRot, m3dEnt.zRot, fDist*fSpeedMultNow);
					rSecLastMetricMove=rNow;
					bMustMove=false;
				}//end if move (if enough time passed since last time to allow division accuracy)
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
								if ((rNow-rSecLastHitGroundSound)>.3f) {
									PlaySound("hitdirt"+ToString(iIndex),"hitdirt",m3dEnt);
									bHitSoundWasPlayed=true;
									rSecLastHitGroundSound=rNow;
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
						} //TODO: m3dEnt.zVel+=zThrust;
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
								//m3dEnt.z=m3dEnt.zMin+.02;//debug only --shouldn't be needed//fMetersPerSecond*rSecondsSinceLastRefresh);
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
								PlaySound("scrape"+ToString(iIndex),"scrapeground",m3dEnt);
								if (iType!=ENTITY_TYPE_BOSS) fSpeedMultiplier=0.3;
							}
						}
						else {
							if (bHitMinZ) {
							PlaySound("scrape"+ToString(iIndex),"scrapeground",m3dEnt);
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
								DamageTurf();
								PlaySound("hitdirt"+ToString(iIndex),"hitdirt",m3dEnt);
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
			else CheckFrames(); //DOES set iFrame using FrameFromRot else cycles frames
		}//end if not firstrun
		else bFirstRun=false;
		if (fMetersPerSecond<fMetersPerSecondMax) fMetersPerSecond=APPROACH(fMetersPerSecond,fMetersPerSecondMax,.5);


		//entity-type-specific statements

		if (iType==ENTITY_TYPE_HERO) {
			// UPDATE //
			DrawMeters();
			if (rSecondsMoveDelay>Base_r0) rSecondsMoveDelay-=rSecondsSinceLastRefresh;
			if (rSecondsMoveDelay<0) rSecondsMoveDelay=0;
			if (rSecondsAimDelay) rSecondsAimDelay-=rSecondsSinceLastRefresh;
			if (rSecondsAimDelay<0) rSecondsAimDelay=0;
			if (rSecondsShootDelay) rSecondsShootDelay-=rSecondsSinceLastRefresh;
			if (rSecondsShootDelay<0) rSecondsShootDelay=0;
			if (rFortitude<1.0f) rFortitude+=1.0*rSecondsSinceLastRefresh;
			if (rFortitude>1.0f) rFortitude=1.0f;
			if (fSpeedMultiplier<1.0f) fSpeedMultiplier+=1.0*rSecondsSinceLastRefresh;
			if (fSpeedMultiplier>1.0f) fSpeedMultiplier=1.0f;

			//re-charge gun
			float fPowerMaxNow=fPowerMax;
			
			float fChargeAddNow=.3333*rSecondsSinceLastRefresh;//=.0011*rSecondsSinceLastRefresh;
			if (u32Status & STATUS_DOUBLESPEED) {
				fPowerMaxNow*=2.0f;
				fChargeAddNow*=2.0f;
			}
			fPower+=(fPower<fPowerMaxNow*.66f)?fChargeAddNow:fChargeAddNow*.66f;
			if (fPower>fPowerMaxNow) fPower=fPowerMaxNow;

			// HITDETECT //
			if (rFortitude>=1.0f) DamageDetect();
			// CHECK LIFE //
			if (iType==ENTITY_TYPE_HERO && fHealth<=0) u32Status=0; //died, stop other special abilities too
			if (!(u32Status & STATUS_ALIVE)) {
				if (!bExploding) {
					PlaySound("explosion"+ToString(iIndex),"explosion",m3dEnt);
					bExploding=true;
				}
				else fExplodedness+=.9*rSecondsSinceLastRefresh;
				if (fExplodedness>1.0f) fExplodedness=1.0f;
			}
		}//end if ENTITY_TYPE_HERO
		else if (iType==ENTITY_TYPE_ALIEN||iType==ENTITY_TYPE_BOSS) {
			// HITDETECT //
			if (rFortitude<1.0f) rFortitude+=.859*rSecondsSinceLastRefresh;
			if (rFortitude>1.0f) rFortitude=1.0f;
			if (fSpeedMultiplier<1.0f) fSpeedMultiplier+=.001*rSecondsSinceLastRefresh;
			if (fSpeedMultiplier>1.0f) fSpeedMultiplier=1.0f;
			if (rSecondsShootDelay>Base_r0) rSecondsShootDelay-=rSecondsSinceLastRefresh;
			if (rSecondsShootDelay<Base_r0) rSecondsShootDelay=Base_r0;
			// AI ACTIONS //
			if (HasAttrib(STATUS_ALIVE)) {
				if (u32Status & STATUS_SHOOTER)
					Shoot();
				else if (u32Status & STATUS_BOMBER) {
					if (u32Status&STATUS_AIMBOMBS) bAimBomb=true;
					else Bomb();
				}
				else if (u32Status & STATUS_BOSS) {//Shoot again other way
					if (u32Status & STATUS_SHOOTER)
						Shoot();
				}
			}
			// HITDETECT //
			if (rFortitude>=1.0f) DamageDetect();
			// CHECK LIFE //
			if (fHealth<=0.0f) RemoveAttrib(STATUS_ALIVE);

			if (!HasAttrib(STATUS_ALIVE)) {
				if (!bExploding) {
					PlaySound("explosion"+ToString(iIndex),"explosion",m3dEnt);
					if (iType==ENTITY_TYPE_ALIEN||iType==ENTITY_TYPE_BOSS) {
						PlaySound("ouchalien"+ToString(iIndex),"ouchalien",m3dEnt);
					}
					bExploding=true;
				}
				else fExplodedness+=.3333*rSecondsSinceLastRefresh;
				if (fExplodedness>1.0f) fExplodedness=1.0f;
			}
		}//end if ENTITY_TYPE_ALIEN
		//else if (iType==ENTITY_TYPE_SHOT) {//TODO: ok to remove iType check since rSecondsLife compared to zero below
		if ((rSecondsLife>0.0)&&(rNow-rSecStart>rSecondsLife)) {
			RemoveAttrib(STATUS_ALIVE);
		}
		//} //end if ENTITY_TYPE_SHOT
		Translate3D();
		rSecLastRefresh=rNow;
		if (iType==ENTITY_TYPE_HERO) {
			p3dHero.x=m3dEnt.x;
			p3dHero.y=m3dEnt.y;
			p3dHero.z=m3dEnt.z;
			IPoint ipDest;
			ipDest.x=m2dEnt.rectRender.left+SCREEN_OFFSET_X;
			ipDest.y=m2dEnt.rectRender.top+SCREEN_OFFSET_Y;
			gfontMain.TypeFast(gbScreen, ipDest, "hero:"+m3dEnt.ToString());
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
				else RemoveAttrib(STATUS_ALIVE);
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
					SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpanimMain->gbFrame, SCREEN_OFFSET_X+m2dEnt.rectRender.left, SCREEN_OFFSET_Y+m2dEnt.rectRender.top, rFortitude*fSpeedMultiplier, fExplodedness, u32Status, m2dEnt.fScale),
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
					SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpanimMain->gbFrame, SCREEN_OFFSET_X+m2dEnt.rectRender.left, SCREEN_OFFSET_Y+m2dEnt.rectRender.top, rFortitude*fSpeedMultiplier, fExplodedness, u32Status, m2dEnt.fScale),
							iType,
							"self");
				}
			}

			if (bDebug) {
				ShowDebugInfo();
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"Entity::Draw");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("Entity::Draw");
		}
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
		bool bGood=false;
		//TODO: improve this, don't use low-level writing
		register UInt32* lpu32Dest=(UInt32*)gbScreen.arrbyData;
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
				//lpu32Dest[xFlat + ((yFlat)*iStride >> 2)]=pixel;
				//lpu32Dest[int(iX*100)]=pixel;
				int iVal;
				//for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++){
				    iVal=255-130+xNow*10-iNotch;
				    if (iVal<0) iVal=0;
				    if (iVal>255) iVal=255;
				    pixelNow.Set(arru32EnergyGrad[iVal]);
				    fRatio=(float)iNotch/fMax+.2;
				    if (fRatio>1.0f) fRatio=1.0f;
				    pixelNow.a=SafeByte(192*fRatio);
				    gbScreen.DrawSubpixelDot(xFlat+xNow,yFlat-iNotch,pixelNow);
					//lpu32Dest[(xFlat + xNow) + (yFlat-iNotch)*BUFFER_WIDTH]=arru32EnergyGrad[iVal];
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
				//lpu32Dest[xFlat + ((yFlat)*iStride >> 2)]=pixel;
				//lpu32Dest[int(iX*100)]=pixel;
				int iVal;
				//for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++){
				    iVal=255-130+xNow*10-iNotch;
				    if (iVal<0) iVal=0;
				    if (iVal>255) iVal=255;
				    pixelNow.Set(arru32HealthGrad[iVal]);
				    fRatio=(float)iNotch/fMax+.2;
				    if (fRatio>1.0f) fRatio=1.0f;
				    pixelNow.a=SafeByte(192*fRatio);
				    gbScreen.DrawSubpixelDot(xFlat+xNow,yFlat-iNotch,pixelNow);
					//lpu32Dest[(xFlat + xNow) + (yFlat-iNotch)*BUFFER_WIDTH]=arru32HealthGrad[iVal];
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
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"entity.DrawMeters");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("entity.DrawMeters");
		}
	}//end DrawMeters
	void Entity::DrawTarget(int alienNum) {
		//for (int alienNow=0; alienNow < iMaxAliens; alienNow++)
		//{
			//if (arrpentAlien[alienNum] != NULL)
			//TODO: remove or fix calling function for new aiming
			int iX=alienNum;
			if (arrpentAlien[iX]==NULL) return;
			Entity* lpAlienNow=(Entity*)arrpentAlien[iX];
			float zOurRange=m3dEnt.zSize/2+lpAlienNow->m3dEnt.zSize/2;
			//I switched the right and left edges on purpose to be more inclusive
			//since screen offset prevents bad writes
			if (lpAlienNow->m3dEnt.z-m3dEnt.z<zOurRange && lpAlienNow->m2dEnt.rectRender.right>0 && lpAlienNow->m2dEnt.rectRender.left<SCREEN_WIDTH ) {
				//register UInt32 *lpu32Dest =
				register BYTE *byteBuffer=gbScreen.arrbyData;//TODO: avoid low-level operations
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
				if (HasAttrib(STATUS_BOSS)) RemoveAttrib(STATUS_SHIELD); //only Boss loses shield, other shields are removed when boss dies
				rFortitude=.1;
				//fHealth+=0.03f;
				delete (Entity*)arrpentShot[iShotIndex];
				arrpentShot[iShotIndex]=NULL;
				if (!bExploding) {
					PlaySound("shieldzap"+ToString(iIndex),"shieldzap",m3dEnt);
				}
			}
			else {
				rFortitude=.1;
				fHealth-=fLaserSusceptibility;
				delete (Entity*)arrpentShot[iShotIndex];
				arrpentShot[iShotIndex]=NULL;
				if (!bExploding) {
					PlaySound("ouchalien"+ToString(iIndex),"ouchalien",m3dEnt);
				}
			}
		} //end if ENTITY_TYPE_ALIEN
	}//end GetHit(int iShotIndex)

	void Entity::DamageDetect() {
		bool bGood=false;
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
					if (arrpentAlien[iAlien] != NULL) {
		        		lpAlienNow=(Entity*)arrpentAlien[iAlien];
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
										rFortitude=.1f;//prevents multiple hits
										PlaySound("blerrp"+ToString(iIndex),"blerrp",m3dEnt);
										PlaySound("shieldzap"+ToString(iIndex),"shieldzap",m3dEnt);
									}
									else if (HasAttrib(STATUS_VARIABLESHIELD)) {
										RemoveAttrib(STATUS_SHIELD);
										//make alien blurrp as well as us zap
										PlaySound("blerrp"+ToString(iIndex),"blerrp",m3dEnt);
										PlaySound("shieldzap"+ToString(iIndex),"shieldzap",m3dEnt);
										rFortitude=.1f;//prevents multiple hits
										fShield-=fLaserSusceptibility*3.0f;//.196
										if (fShield<=0.0f) RemoveAttrib(STATUS_VARIABLESHIELD);
									}
									else {
										PlaySound("blerrp"+ToString(iIndex),"blerrp",m3dEnt);
										PlaySound("ouchzap"+ToString(iIndex),"ouchzap",m3dEnt);
										rFortitude=.1f;
										fHealth-=fLaserSusceptibility*3.0f;//.196
									}
								}
							}
						}
					}//end if non-null alien
				}//end for arrpentAlien
				//HIT BULLET: edit the shot::DamageDetect to do this instead maybe
				Entity* lpShotNow=NULL;
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (arrpentShot[iShotNow] != NULL) {//DISTANCE FORMULA: uses centerpoint (m3dEnt.x,m3dEnt.y,m3dEnt.z)
						lpShotNow=(Entity*)arrpentShot[iShotNow];
						float xOurRange=lpShotNow->m3dEnt.xSize/2.0f+xMyRange;
						float yOurRange=lpShotNow->m3dEnt.ySize/2.0f+yMyRange;
						float zOurRange=lpShotNow->m3dEnt.zSize/2.0f+zMyRange;
						if (abs(lpShotNow->m3dEnt.x-m3dEnt.x) < xOurRange) {
							if (abs(lpShotNow->m3dEnt.y-m3dEnt.y) < yOurRange) {
								if (abs(lpShotNow->m3dEnt.z-m3dEnt.z) < zOurRange && (lpShotNow->bAlien)) {
									if (HasAttrib(STATUS_SHIELD)) {
										RemoveAttrib(STATUS_SHIELD);
										rFortitude=.1f; //prevents multiple hits
										//fHealth+=fLaserSusceptibility;//.059;
										delete (Entity*)arrpentShot[iShotNow];
										arrpentShot[iShotNow]=NULL;
										PlaySound("shieldzap"+ToString(iIndex),"shieldzap",m3dEnt);
									}
									else if (HasAttrib(STATUS_VARIABLESHIELD)) {
										rFortitude=.1f; //prevents multiple hits
										fShield-=fLaserSusceptibility*3.0f;//.196//fHealth+=fLaserSusceptibility;//.059;
										if (fShield<=0.0f) RemoveAttrib(STATUS_VARIABLESHIELD);
										delete (Entity*)arrpentShot[iShotNow];
										arrpentShot[iShotNow]=NULL;
										PlaySound("shieldzap"+ToString(iIndex),"shieldzap",m3dEnt);
									}
									else {
										rFortitude=.1f;
										fHealth-=fLaserSusceptibility;//.059;
										delete (Entity*)arrpentShot[iShotNow];
										arrpentShot[iShotNow]=NULL;
										PlaySound("ouchzap"+ToString(iIndex),"ouchzap",m3dEnt);
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
					if (arrpentShot[iShotNow]!=NULL) {//DISTANCE FORMULA: uses centerpoint (m3dEnt.x,m3dEnt.y,m3dEnt.z)
					    lpShotNow=(Entity*)arrpentShot[iShotNow];
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
				}//end for shot
			}//end if ENTITY_TYPE_ALIEN || ENTITY_TYPE_BOSS
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"damage detect");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("damage detect");
		}
	}//END DAMAGE DETECT
	void Entity::SetDoubleSpeed() {
		if (0==(u32Status & STATUS_DOUBLESPEED)) {
			u32Status |= STATUS_DOUBLESPEED;
			bPlayTrumpet=true;
			m3dEnt.zRotVel*=2.0f;
			m3dEnt.yRotVel*=2.0f;
			fShootMetersPerSec*=2.0f;
		}
	}
	void Entity::SetRapidFire() {
		if (0==(u32Status & STATUS_RAPIDFIRE)) {
			u32Status |= STATUS_RAPIDFIRE;
			bPlayTrumpet=true;
		}
	}
					//TODO: set m3dEnt.SetRotMaxSpeed in Entity::Init
	void Entity::Bomb(float xFlat, float yFlat, float zDest) {
		if (rSecondsShootDelay<=Base_r0) {
			if (u32Status&STATUS_BOSS) rSecondsShootDelay=(REAL)IRand(5,15)/(REAL)30.0;
	  		else rSecondsShootDelay=(REAL)IRand(10,20)/(REAL)30.0;
			float xEye=m3dEnt.x-m3dEnt.xSize/2.0f;
			if (fShootMetersPerSec<6.0f) fShootMetersPerSec=6.0f;
			float xSpeed,ySpeed,zSpeed;
			float fArrivalFrames=60.0f;
			if (u32Status&STATUS_BOSS) fArrivalFrames=(float)(IRand(30,60));
			xSpeed=(xFlat-m3dEnt.x)/fArrivalFrames;
			ySpeed=(yFlat-m3dEnt.y)/fArrivalFrames;
			zSpeed=(zDest-m3dEnt.z)/fArrivalFrames;
			for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
				if (arrpentShot[iShotNow] == NULL) {
				//create a shot at the first available shot pointer
					PlaySound("bomb"+ToString(iIndex),"bomb",m3dEnt);
					arrpentShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, xEye, m3dEnt.y, m3dEnt.z, xSpeed, ySpeed, zSpeed, bAlien, false);
					((Entity*)arrpentShot[iShotNow])->iIndex=iShotNow;
					break;
				}
			}
		}
	}
	void Entity::Bomb(){
		if (rSecondsShootDelay<Base_r0) {
			if (u32Status&STATUS_BOSS) rSecondsShootDelay=(REAL)IRand(5,15)/(REAL)30;
	  		else rSecondsShootDelay=(REAL)IRand(10,20)/(REAL)30;
			float xEye=m3dEnt.x-m3dEnt.xSize/2.0f;
			for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
				if (arrpentShot[iShotNow] == NULL) {
				//create a shot at the first available shot pointer
					PlaySound("bomb"+ToString(iIndex),"bomb",m3dEnt);
					//Assume Shoot left
					//Prototype: Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isRed2)
					fShootMetersPerSec=2.0f*(float)(IRand(1,4));
					Mass3d m3dWeapon;
					m3dWeapon.x=m3dEnt.x;
					m3dWeapon.y=m3dEnt.y;
					m3dWeapon.z=m3dEnt.z;
					m3dWeapon.zRotDest=(float)(IRand(0,360));
					m3dWeapon.yRotDest=-(float)(IRand(0,45));
					arrpentShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, bAlien, false);
					((Entity*)arrpentShot[iShotNow])->iIndex=iShotNow;
					break;
				}
			}
		}
	}//end Bomb()
	void Entity::DamageTurf() {
		bool bGood=true;
		try {
			if (lpanimShadow->gbFrame.arrbyData==NULL) return;
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
			if (lpanimShadow->gbFrame.arrbyData==NULL) bDraw=false;
			if (bDraw) {
				SayWhatIDrewIfFalse(GBuffer_FX_Scaled(animBackdrop.gbFrame, lpanimShadow->gbFrame, m2dEnt.rectRender.left, m2dEnt.rectRender.top, .078, 0, 0, m2dShadow.fScale),
						iType,
						"turf damage");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"DamageTurf");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("DamageTurf");
		}
	}//end DamageTurf
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
		return (u32Status&bit)!=0;
	}
	void Entity::RemoveAttrib(UInt32 bit) {
	    u32Status&=(bit^0xFFFFFFFF);
	}
	void Entity::AddVariableShield(float fSetShieldTo) {
		u32Status |= STATUS_VARIABLESHIELD;
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
		//TODO: finish this--uncomment: oggMusic.release();
		//if (music!=NULL) {
		//	Mix_HaltMusic();
		//	Mix_FreeMusic(music);
		//	music=NULL;
		//}
	}
	void InitSound() {
		try {
			sounder.Load("bomb","sound/bomb.wav");//mcBomb=Mix_LoadWAV("sound/bomb.wav");
			sounder.Load("laser-alien","sound/laser-alien.wav");//mcLaserAlien=Mix_LoadWAV("sound/laser-alien.wav");
			sounder.Load("laser","sound/laser.wav");//mcLaser=Mix_LoadWAV("sound/laser.wav");
			sounder.Load("explosion","sound/explosion.wav");//mcExplosion=Mix_LoadWAV("sound/explosion.wav");
			sounder.Load("ouchalien","sound/ouchalien.wav");//mcOuchAlien=Mix_LoadWAV("sound/ouchalien.wav");
			sounder.Load("ouchzap","sound/ouchzap.wav");//mcOuchZap=Mix_LoadWAV("sound/ouchzap.wav");
			sounder.Load("shieldzap","sound/shieldzap.wav");//mcShieldZap=Mix_LoadWAV("sound/shieldzap.wav");
			sounder.Load("blerrp","sound/blerrp.wav");//mcBlerrp=Mix_LoadWAV("sound/blerrp.wav");
			sounder.Load("hitdirt","sound/hitdirt.wav");//mcHitDirt=Mix_LoadWAV("sound/hitdirt.wav");
			sounder.Load("jump","sound/jump.wav");//mcJump=Mix_LoadWAV("sound/jump.wav");
			sounder.Load("scrapeground","sound/scrapeground.wav");//mcScrapeGround=Mix_LoadWAV("sound/scrapeground.wav");
			sounder.Load("angryalien","sound/angryalien.wav");//mcAngryAlien=Mix_LoadWAV("sound/angryalien.wav");
			sounder.Load("trumpet","sound/trumpet.wav");//mcTrumpet=Mix_LoadWAV("sound/trumpet.wav");
			sounder.Load("thruster","sound/thruster.wav");//mcThruster=Mix_LoadWAV("sound/thruster.wav");
			sounder.Load("laser","sound/laser.wav");//mcLaser=Mix_LoadWAV("sound/laser.wav");
		}
		catch (exception& exn) {
			ShowException(exn,"InitSound");
		}
		//note: must increase SOUND_BUFFERS if adding more samples!
		//Mix_VolumeChunk(mcThruster,40);
		//Mix_VolumeChunk(mcLaser,100);
		bClosedAudio=false;
	}
	void PlayMusic(string sFile, bool bLoop) {
		bool bGood=false;
		try {
			sMusic=sFile;
			bLoopMusic=bLoop;
			//TODO: finish this--uncomment: oggMusic.open(sMusic);
			//TODO: finish this--uncomment: oggMusic.playback();
			//music=Mix_LoadMUS(sFile.c_str());
			//Mix_PlayMusic(music, iRepeatsZeroFor1Neg1ForInf);
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"PlayMusic");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("PlayMusic");
		}
	}//end PlayMusic
	bool ShutdownMusic() {
		bool bGood=false;
		try {//end of main event loop
			//if (music!=NULL) {
			//	Mix_HaltMusic();
			//	Mix_FreeMusic(music);
			//	music=NULL;
			//	Console.WriteLine("Stopping Music...done");
			//}
			//else Console.WriteLine("Stopping Music...was finished.");
			//TODO: finish this--uncomment: oggMusic.release();
			//bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"ShutDownMusic");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("ShutDownMusic");
		}
		return bGood;
	}//end ShutDownMusic wrapper
	bool ShutdownAudio() {
		bool bGood=false;
		try {//end of main event loop
			//SafeChunkUnload(mcBomb);
			//SafeChunkUnload(mcLaserAlien);
			//SafeChunkUnload(mcLaser);
			//SafeChunkUnload(mcExplosion);
			//SafeChunkUnload(mcOuchAlien);
			//SafeChunkUnload(mcOuchZap);
			//SafeChunkUnload(mcShieldZap);
			//SafeChunkUnload(mcBlerrp);
			//SafeChunkUnload(mcHitDirt);
			//SafeChunkUnload(mcJump);
			//SafeChunkUnload(mcScrapeGround);
			//SafeChunkUnload(mcAngryAlien);
			//SafeChunkUnload(mcTrumpet);
			//SafeChunkUnload(mcThruster);
			//SafeChunkUnload(mcLaser);
			//SafeChunkUnload(mcThruster);
			if (!bClosedAudio) {
				sounder.Close();
			//	Mix_CloseAudio();
			//	Console.WriteLine("Closing Audio...done.");
				bClosedAudio=true;
			}
			else Console.WriteLine("Closing Audio...was finished");
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"ShutdownAudio");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("ShutdownAudio");
		}
		return bGood;
	}
	void MusicDone() { //runs when music is done
		//if (music!=NULL) {
			//Mix_HaltMusic();
			//Mix_FreeMusic(music);
			//music=NULL;
		//}
	}
	//inline void SetPan(int iChan, int iLocation) {
	//	 float fLoc=(float)iLocation/(float)SCREEN_WIDTH;
	//	 if (fLoc<0) fLoc=0;
	//	 else if (fLoc>1) fLoc=1;
	//	 Mix_SetPanning(iChan, L_FROM_FACTOR(fLoc), R_FROM_FACTOR(fLoc));
	//}
	inline void UpdateThrustPan() {
		sounder.SetOrCreateInstance("h.thruster","thruster",hero->m3dEnt);
		 //SetPan(iChanThruster,SOUNDPAN_FROM3DX(hero->m3dEnt.x));
		 //fThruster=(float)hero->m3dEnt.x/(float)SCREEN_WIDTH;
		 //if (fThruster<0) fThruster=0;
		 //else if (fThruster>1) fThruster=1;
		 //Mix_SetPanning(iChanThruster, L_FROM_FACTOR(fThruster), R_FROM_FACTOR(fThruster));
	}
	void DirKeyDown() {
		 if ((hero!=NULL&&hero->m3dEnt.z<=hero->m3dEnt.zMin) && (hero->u32Status & STATUS_ANTIGRAVITY)) {
			if (iDirKeysDown<=0) {
				sounder.StopInstance("h.thruster");
				//iChanThruster=Mix_PlayChannel(-1, mcThruster, -1);//chan, sound, #loops
				//iReturn=Mix_FadeInChannelTimed(iChan, mcThruster,
				//			iLoops, iMillisecondsFadeIn, rSecondsDuration)
			}
			else UpdateThrustPan();
		 }
		 iDirKeysDown++;
	}
	void DirKeyUp() {
		 iDirKeysDown--;
		 if (iDirKeysDown<=0) {
			sounder.StopInstance("h.thruster");
			//Mix_HaltChannel(iChanThruster);
			//iChanThruster=-1;
			iDirKeysDown=0;
		 }
	}
	bool bDone=false;
	SDL_Event event;
	// FUNCTIONS ////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////
	REAL Base_GetSeconds() {
		return ((REAL)SDL_GetTicks())/Base_r1000;
	}
	REAL Base_SecondsSince(REAL rSecondCount) {
		REAL (Base_GetSeconds()-rSecondCount);
	}
	int IRandPositive() {
		static int iMoreRandom=2;//keep as constant value to make it deterministic
		iMoreRandom=SafeAddWrappedPositiveOnly(iMoreRandom,iMoreRandom/2);
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
				if ((u32Pressing&GAMEKEY_EXIT)&&(SDL_GetTicks()-iEscapeTime)>1000) {//int answer=0;
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
									u32Pressing&=GAMEKEY_EXIT^0xFFFFFFFF;
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
				SleepWrapper(1500);
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
				SleepWrapper(1000);
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
				catch (exception& exn) {
					ShowException(exn,"INIT state", "hiding cursor");
				}
				catch (...) {
					ShowUnknownException("INIT state", "hiding cursor");
				}
				if (hero!=NULL) {
					delete hero;
					hero=NULL;
				}
				try {
					for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++) {
						if (arrpentAlien[iAliensNow]!=NULL) {
							if (iAliensNow==iBoss) iBoss=-1;
							delete (Entity*)arrpentAlien[iAliensNow];
							arrpentAlien[iAliensNow]=NULL;
						}
					}
				}
				catch (exception& exn) {
					ShowException(exn,"INIT state","initializing alien flyers");
				}
				catch (...) {
					ShowUnknownException("INIT state","initializing alien flyers");
				}
				try {
					hero=new Entity(ENTITY_TYPE_HERO);
					hero->iIndex=0;
				}
				catch (exception& exn) {
					ShowException(exn,"INIT state","creating hero");
				}
				catch (...) {
					ShowUnknownException("INIT state","creating hero");
				}
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
				catch (exception& exn) {
					ShowException(exn,"START_AREA state","getting animBackdrop");
				}
				catch (...) {
					ShowUnknownException("START_AREA state","getting animBackdrop");
				}
				hero->fHealth=1.0f;
				if (iArea==2) hero->u32Status |= STATUS_ANTIGRAVITY;
				iGameState=GAMESTATE_START_ENCOUNTER;
			}
			else if (iGameState == GAMESTATE_LOSEGUY) {
				SleepWrapper(1000);
	
				StopMusic();
				iGuys--;
				if (hero==NULL) {
					hero=new Entity(ENTITY_TYPE_HERO);
					hero->iIndex=0;
				}
	
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
				SleepWrapper(4000);
				iGameState=GAMESTATE_START_ENCOUNTER;
			}
			else if (iGameState == GAMESTATE_START_ENCOUNTER) {//START ENCOUNTER
				animGameScreenArea.GotoFrame(iArea-1);
				animGameScreenEncounter.GotoFrame(iEncounter-1);
				iMaxAliensNow=iMaxAliens+iArea-1;
				bBombed=false;
				StopMusic();
				ClearSounds();
				PlaySounds();
				animBackdrop.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
				animGameScreenArea.gbFrame.DrawToLargerWithoutCropElseCancel( gbScreen,
						(int)( FSCREEN_WIDTH/2.0f-animGameScreenArea.Width()/2.0f+FSCREEN_OFFSET_X ),
						(int)( FSCREEN_HEIGHT/2.0f-animGameScreenArea.Height()+FSCREEN_OFFSET_Y ),DrawModeBlendAlpha );
				animGameScreenEncounter.gbFrame.DrawToLargerWithoutCropElseCancel( gbScreen,
					(int)( FSCREEN_WIDTH/2.0f-animGameScreenEncounter.Width()/2.0f+FSCREEN_OFFSET_X ),
					(int)( FSCREEN_HEIGHT/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha );
				DrawScreen();
				SleepWrapper(3000);
				if (settings.GetForcedBool("music")) {
					PlayMusic("music/Orangejuice-DXMan-Invasion.ogg",true);
				}
				for (int index=0; index<iMaxAliensNow; index++) {
					//arrpentAlien[index]=new Alien(3.0f, (float)index-2.0f, 3.9f);
					arrpentAlien[index]=new Entity(ENTITY_TYPE_ALIEN, 3.0f, (float)index-1.0f, 3.9f);
					((Entity*)arrpentAlien[index])->iIndex=index;
					//arrpentAlien[index]->m3dEnt.x=SCREEN_WIDTH;
					//arrpentAlien[index]->m3dEnt.y += index*(SCREEN_HEIGHT/64);
					//arrpentAlien[index]->m3dEnt.z=index+1;
					//if (iArea==2) arrpentAlien[index]->u32Status |= STATUS_SHOOTER;
					//else if (iArea==3) arrpentAlien[index]->u32Status |= STATUS_BOMBER;
					//if (iArea==3 && iEncounter==3) arrpentAlien[index]->u32Status |= STATUS_SHIELD;
				}
	
				if (iArea==3) {
					hero->AddVariableShield(1.0f);
					if (iEncounter!=3) ((Entity*)arrpentAlien[0])->u32Status|=STATUS_AIMBOMBS;
				}
				if (iEncounter==3 && iArea==3) {
						iBoss=( (iMaxAliensNow>=3)?2:(iMaxAliensNow-1) );
					((Entity*)arrpentAlien[iBoss])->SetAsBoss();//Set a boss if last encounter of last area
				}
				Console.WriteLine("About to do GAMESTATE_RUN at area "+ToString(iArea)+" encounter "+ToString(iEncounter));
				iGameState=GAMESTATE_RUN;
			}
	
			else if (iGameState == GAMESTATE_RUN) {
				//if KEYDOWN(65/* a */) hero->u32Status |= STATUS_ANTIGRAVITY;
				//if KEYUP(65) hero->u32Status ^= STATUS_ANTIGRAVITY;
	
				if (u32Pressing&GAMEKEY_UP) { hero->Move(0,1); }
				if (u32Pressing&GAMEKEY_DOWN) { hero->Move(0,-1);	}
				if (u32Pressing&GAMEKEY_LEFT) { hero->Move(-1,0);}
				if (u32Pressing&GAMEKEY_RIGHT) { hero->Move(1,0);}
				if (u32Pressing&GAMEKEY_UP2) { hero->TurnPitchIncrement(true); }
				if (u32Pressing&GAMEKEY_DOWN2) { hero->TurnPitchIncrement(false);	}
				if (u32Pressing&GAMEKEY_LEFT2) {
					hero->TurnIncrement(true);
					//if (hero->m3dEnt.yRot< -hero->zRotSpeed) hero->TurnPitchIncrement(true);
					//else if (hero->m3dEnt.yRot> hero->zRotSpeed) hero->TurnPitchIncrement(false);
					//else hero->TurnPitch(0.0f);//this is the destination for the whole "if" clause
				}
				if (u32Pressing&GAMEKEY_RIGHT2) {
					hero->TurnIncrement(false);
					//if (hero->m3dEnt.yRot< -hero->zRotSpeed) hero->TurnPitchIncrement(true);
					//else if (hero->m3dEnt.yRot> hero->zRotSpeed) hero->TurnPitchIncrement(false);
					//else hero->TurnPitchIncrement(0.0f);//this is the destination for the whole "if" clause
				}
				if (u32Pressing&GAMEKEY_JUMP) { hero->Jump(); }
				if (!(u32Pressing&GAMEKEY_FIRE)) hero->rSecondsShootDelay=0;
				if (u32Pressing&GAMEKEY_FIRE) hero->Shoot();
	
				if (u32Pressing&GAMEKEY_DOUBLESPEED) iDoubleCodeCounter++;//holding '2' activates STATUS_DOUBLESPEED
				else iDoubleCodeCounter=0;
	
				if (u32Pressing&GAMEKEY_RAPIDFIRE) iRapidFireCodeCounter++; //holding 'r' activates STATUS_RAPIDFIRE
				else iRapidFireCodeCounter=0;
	
				//if KEYUP(80/* p */) keyDelay=0;
	
	
	
				if (iDoubleCodeCounter>=CODE_THRESHOLD && hero->u32Status ^ STATUS_DOUBLESPEED) hero->SetDoubleSpeed();
				if (iRapidFireCodeCounter>=CODE_THRESHOLD && hero->u32Status ^ STATUS_RAPIDFIRE) hero->SetRapidFire();
	
				animBackdrop.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
				//DrawRadarField();
	
				//Update all the existing arrpentAlien, delete the rest
				if (hero!=NULL) {
					if (iBoss>=0) {
						try {
								fBoss=((Entity*)arrpentAlien[iBoss])->fHealth;
							fBossMax=((Entity*)arrpentAlien[iBoss])->fHealthMax;
						}
						catch (exception& exn) {
							ShowException(exn,"RUN state","getting boss vars");
						}
						catch (...) {
							ShowUnknownException("RUN state","getting boss vars");
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
					if (arrpentAlien[iAlien] != NULL) {
						lpAlienNow=(Entity*)arrpentAlien[iAlien];
						lpAlienNow->Refresh();
						if (lpAlienNow->bAimBomb && hero)
							lpAlienNow->Bomb(hero->m3dEnt.x,hero->m3dEnt.y,hero->m3dEnt.z);
							//TODO: fix bomb function to use metric speed(?)
						lpAlienNow->bAimBomb=false;
	
						AddScreenItem(SCREENITEM_ALIEN,ZORDER_FROMY(lpAlienNow->m3dEnt.y),iAlien);
						////hero->DrawTarget(iAlien);
						if (lpAlienNow->u32Status & STATUS_BOSS) {
							//DrawRadarDot(lpAlienNow->m3dEnt.x, lpAlienNow->m3dEnt.y, lpAlienNow->m3dEnt.z,
							//0x00000000);
							//prototype: void DrawRadarRect(float left, float top, float bottom, float right, Uint32 u32Pixel)
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
						//((lpAlienNow->u32Status&STATUS_BOSS)?0x00000000:0xFFFF0000) );
							float fHalfH, fHalfW;
							fHalfH=lpAlienNow->m3dEnt.ySize/2.0f;
							fHalfW=lpAlienNow->m3dEnt.xSize/2.0f;
								DrawRadarRect(lpAlienNow->m3dEnt.x-fHalfW,
										lpAlienNow->m3dEnt.y+fHalfH,
										lpAlienNow->m3dEnt.y-fHalfH,
										lpAlienNow->m3dEnt.x+fHalfW,0xFFFF0000, true);//electric_blue:0xFF8800FF
						}
	
						if (lpAlienNow->fExplodedness>=1.0f) {
							if (lpAlienNow->u32Status & STATUS_BOSS) {
								for (int iAlienToDeShield=0; iAlienToDeShield < iMaxAliensNow; iAlienToDeShield++) {
									if (arrpentAlien[iAlienToDeShield]) {
										if (((Entity*)arrpentAlien[iAlienToDeShield])->u32Status & STATUS_SHIELD) {
											((Entity*)arrpentAlien[iAlienToDeShield])->u32Status ^= STATUS_SHIELD;
											((Entity*)arrpentAlien[iAlienToDeShield])->fHealth=0;
										}
									}
								}
							}
							iAliens--;
							delete (Entity*)arrpentAlien[iAlien];
							if (iAlien==iBoss) iBoss=-1;
							arrpentAlien[iAlien]=NULL;
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
					if (arrpentShot[iShotNow] != NULL) {
						((Entity*)arrpentShot[iShotNow])->Refresh();
						((Entity*)arrpentShot[iShotNow])->Draw();
						AddScreenItem(SCREENITEM_SHOT,ZORDER_FROMY(((Entity*)arrpentShot[iShotNow])->m3dEnt.y),iShotNow);
						DrawRadarDot( ((Entity*)arrpentShot[iShotNow])->m3dEnt.x, ((Entity*)arrpentShot[iShotNow])->m3dEnt.y, 1,
						((((Entity*)arrpentShot[iShotNow])->bAlien)?0xFF800000:0xFFFFFFFF),false);//A,R,G,B //electric_blue:0xFF8800FF
						if ((((Entity*)arrpentShot[iShotNow])->u32Status&STATUS_ALIVE)==0) {
							delete (Entity*)arrpentShot[iShotNow];
							arrpentShot[iShotNow]=NULL;
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
									if (NULL!=arrpentAlien[ screenitemarr[iItem].index ]) ((Entity*)arrpentAlien[ screenitemarr[iItem].index ])->Draw();
									iScreenItemsNow--;
									break;
								case SCREENITEM_SHOT:
									if (NULL!=arrpentShot[ screenitemarr[iItem].index ]) ((Entity*)arrpentShot[ screenitemarr[iItem].index ])->Draw();
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
					PlayMusic("sound/trumpet.wav", false);
				}
	
	
				for (int iX=0; iX<MAXSHOTS; iX++) {
					if (arrpentShot[iX]!=NULL)
					{
						delete (Entity*)arrpentShot[iX];
						arrpentShot[iX]=NULL;
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
					animGameScreen.GotoFrame(iScreenNow);
					animGameScreen.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, (int)( FSCREEN_WIDTH/2.0f-animGameScreen.Width()/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-animGameScreen.Height()/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha);
					DrawScreen();
					SleepWrapper(5000);
				}
			}
			else if (iGameState == GAMESTATE_WIN_GAME) {
				int iScreenNow=1;
				if (iDoubleCodeCounter>=CODE_THRESHOLD || (hero&&hero->u32Status&STATUS_DOUBLESPEED)) {
					iScreenNow=7;
				}
				if (iRapidFireCodeCounter>=CODE_THRESHOLD|| (hero&&hero->u32Status&STATUS_RAPIDFIRE)) iScreenNow=8;
	
				animGameScreen.GotoFrame(iScreenNow);
				animGameScreen.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, (int)( FSCREEN_WIDTH/2.0f-animGameScreen.Width()/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-animGameScreen.Height()/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha);
				DrawScreen();
	
				StopMusic();
	
				if (settings.GetForcedBool("music"),true) {
					PlayMusic("music/Orangejuice-DXMan-Ending.ogg",true);
				}
	
				SleepWrapper(20000);
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
					PlayMusic("music/Orangejuice-DXMan-Intro.ogg", true);
				}
				animGameScreen.GotoFrame(0);
				animGameScreen.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, (int)( FSCREEN_WIDTH/2.0f-animGameScreen.Width()/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-animGameScreen.Height()/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha);
				DrawScreen();
				SleepWrapper(5000);
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
				catch (exception& exn) {
					ShowException(exn,"EXIT state","trying to run SDL_Quit");
				}
				catch (...) {
					ShowUnknownException("EXIT state","trying to run SDL_Quit");
				}
				if (!bDone) {
					Console.WriteLine("GAMESTATE_EXIT...done");
				}
			}//end if GAMESTATE_EXIT
		}
		catch (exception& exn) {
			ShowException(exn,"GameMain");
		}
		catch (...) {
			ShowUnknownException("GameMain");
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
			arrpentAlien=(void**)malloc(sizeof(Entity*)*iMaxAliens);
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
			//Console.Write("Initializing audio...");
			//if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
			//	sMsg="Couldn't open Audio: \n";
			//	sMsg+=ToString(SDL_GetError());
			//	cerr<<"In GameInit -- "<<sMsg<<endl;
			//	iErrors++;
			//	//MessageBox (0, sMsg, "Error", MB_ICONHAND); //re-implement this w/o windows api
			//	return false;
			//}
			//Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);//what we got
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
			GBuffer gbSplash;
			if (!gbSplash.Load("images/intro-splash.tga")) {
				sMsg="Failed to load splash screen";
				cerr<<"In GameInit -- "<<sMsg<<endl;
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
			}
			bool bSplashDrawn=false;
			Console.WriteLine("done.");
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
			//DONE LOADING SPLASH SCREEN
			float fGrad;
			float r1,g1,b1,r2,g2,b2;
			r2=232;	g2=49;	b2=0;
			r1=126;	g1=97;	b1=200;
			for (int iGrad=0; iGrad<256; iGrad++) {
				fGrad=(float)iGrad/255.0f;
				//TODO: make the next line use a byte array instead of the macro.
				arru32HealthGrad[iGrad]=_RGB32BIT(255,(BYTE)APPROACH(r2,r1,fGrad),(BYTE)APPROACH(g2,g1,fGrad),(BYTE)APPROACH(b2,b1,fGrad)); //A,R,G,B,
			}
			r1=64;	g1=64;	b1=64;
			r2=126; g2=97;	b2=200;
			for (int iGrad=0; iGrad<256; iGrad++) {
				fGrad=(float)iGrad/255.0f;
				//TODO: make the next line use a byte array instead of the macro.
				arru32EnergyGrad[iGrad]=_RGB32BIT(255,(BYTE)APPROACH(r2,r1,fGrad),(BYTE)APPROACH(g2,g1,fGrad),(BYTE)APPROACH(b2,b1,fGrad)); //A,R,G,B,
			}
			Console.Write("Loading sounds...");

			if (settings.GetForcedBool("music")) {
				PlayMusic("music/Orangejuice-DXMan-Intro.ogg", true);
			}

			InitSound();

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
			Console.Write("Loading font...");
			bool bTest=gfontMain.FromImageValue("images/font.tga", 16, 24, 16, 16);
			//Console.Write(bTest?"FromImageValue succeeded.":"FromImageValue failed!");
			cout<<flush;
			Console.Write("Loading image files...");
			cout<<flush;
			for (int shotsNow=0; shotsNow<MAXSHOTS; shotsNow++)
				arrpentShot[shotsNow]=NULL;
			for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++)
				arrpentAlien[iAliensNow]=NULL;

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
			LoadSequence(animShot, "images/shot", iFramesShot);
			LoadSequence(animGameScreen, "images/gamescreen", iFramesGameScreen);
			LoadSequence(animGameScreenArea, "images/gamescreenarea", iAreas);
			LoadSequence(animGameScreenEncounter, "images/gamescreenencounter", iEncounters);
			LoadSequence(animCursor, "images/cursor", iFramesCursor);
			LoadImage(gbIntro, "images/intro-title.tga");
			LoadImage(gbIntroText, "images/intro-text.tga");
			LoadImage(gbSymbolShield, "images/symbol-shield.tga");
			LoadImage(gbSymbolBossHealth, "images/symbol-bosshealth.tga");
			LoadImage(gbGuysDecor, "images/lives.tga");

			settings.bSaveEveryChange=true;
			dat.bSaveEveryChange=true;
			Console.WriteLine("done (end of GameInit).");
		}//end try
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"GameInit");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("GameInit");
		}
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
			ShutdownMusic();
			ShutdownAudio();
			if (arrpentAlien!=NULL) {
				try {
					for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++) {
						if (arrpentAlien[iAliensNow]!=NULL) {
							if (iAliensNow==iBoss) iBoss=-1;
							delete (Entity*)arrpentAlien[iAliensNow];
							arrpentAlien[iAliensNow]=NULL;
						}
					}
					free(arrpentAlien);
				}
				catch (exception& exn) {
					bGood=false;
					ShowException(exn,"GameShutdown","terminating alien flyers");
				}
				catch (...) {
					bGood=false;
					ShowUnknownException("GameShutdown","terminating alien flyers");
				}
	        	//SafeFree(arrpentAlien);
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"GameShutdown");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("GameShutdown");
		}
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
		bool bGood=false;
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
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"RadarDotAt");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("RadarDotAt");
		}
	}//end RadarDotAt
	void DrawRadarDot(float xPos, float yPos, float zPos, Uint32 u32Pixel) {
		DrawRadarDot(xPos,yPos,zPos,u32Pixel,true);
	}
	void DrawRadarDot(float xPos, float yPos, float zPos, Uint32 u32Pixel, bool bFilled) {
		bool bGood=false;
		try {
			int xCenter,yCenter;
			RadarDotAt(xCenter, yCenter, xPos, yPos);
			int xOffset=(int)(zPos);
			int yOffset=(int)(zPos);
			int left=xCenter-xOffset, right=xCenter+xOffset, top=yCenter-yOffset, bottom=yCenter+yOffset;
			DrawExclusiveRect(left, top, bottom+1, right+1, u32Pixel, bFilled);
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"DrawRadarDot");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("DrawRadarDot");
		}
	}//end DrawRadarDot
	void DrawRadarRect(float left, float top, float bottom, float right, Uint32 u32Pixel, bool bFilled) {
		bool bGood=false;
		try {
			int iLeft=0, iTop=0, iBottom=0, iRight=0;
			RadarDotAt(iLeft,iTop,left,top);
			RadarDotAt(iRight,iBottom,right,bottom);
			DrawExclusiveRect(iLeft, iTop, iBottom+1, iRight+1, u32Pixel, bFilled);
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"DrawRadarRect");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("DrawRadarRect");
		}
	}//end DrawRadarRect
	void DrawRadarField() {
		static float FRADAR_XRAD=FXMAX-FXMIN;
		static float FRADAR_YRAD=FYMAX-FYMIN;
		static Uint32 u32Pixel=0x004400FF;
		static int xCenter=SCREEN_WIDTH-(int)(FRADAR_XRAD+6.0f);
		static int yCenter=(int)((float)FRADAR_YRAD+6.0f);
		static int xOffset=(int)(FRADAR_ZOOM*FRADAR_XRAD);
		static int yOffset=(int)(FRADAR_ZOOM*FRADAR_YRAD);
		static int left=xCenter-xOffset, right=xCenter+xOffset, top=yCenter-yOffset, bottom=yCenter+yOffset;
		DrawExclusiveRect(left, top, bottom, right, u32Pixel, false);
	}
	void DrawExclusiveRect(int left, int top, int bottom, int right, Uint32 u32Pixel, bool bFilled) {
		if (left<0||right>=BUFFER_WIDTH||top<0||bottom>=BUFFER_HEIGHT)//error checking
			return;
		left+=SCREEN_OFFSET_X;
		right+=SCREEN_OFFSET_X;
		top+=SCREEN_OFFSET_Y;
		bottom+=SCREEN_OFFSET_Y;
		bool bGood=true;
		Uint32* lpu32Dest=(Uint32*)gbScreen.arrbyData; //TODO: avoid low-level operations
		int iScreenW=gbScreen.iWidth;
		int iScreenH=gbScreen.iHeight;
		if (bottom<=top) {//if bad, draw vert line
			bottom=top+6;
			right=left+1;
			DrawExclusiveRect(left,top,bottom,right,u32Pixel,false);
			if (right<=left) {//if bad, draw horz line too
				top+=2;
				bottom=top+1;
				left-=2;
				right=left+6;
				DrawExclusiveRect(left,top,bottom,right,u32Pixel,false);
			}
			bGood=false;
		}
		else if (right<=left) {//if bad, draw horz line
			bottom=top+1;
			right=left+6;
	        DrawExclusiveRect(left,top,bottom,right,u32Pixel,false);
			bGood=false;
		}
		int iStart=top*iScreenW+left;//no stride since using Uint32* to buffer
		lpu32Dest+=iStart;
		int iSkipEdge=iScreenW-(right-left);
		if (bGood) {
			if (bFilled) {
				for (int yNow=top; yNow<bottom; yNow++) {
					for (int xNow=left; xNow<right; xNow++) {
						*lpu32Dest=u32Pixel;
						lpu32Dest++;
					}
					lpu32Dest+=iSkipEdge;
				}
			}//end if bFilled
			else {//only draw outline
				for (int yNow=top; yNow<bottom; yNow++) {
					for (int xNow=left; xNow<right; xNow++) {
						if (yNow==top||yNow==bottom-1||xNow==left||xNow==right-1)
							*lpu32Dest=u32Pixel;
						lpu32Dest++;
					}
					lpu32Dest+=iSkipEdge;
				}
			}
		}//end if bGood
	}

	////////////////////////////////////////////////////////////////////////////////
	//TTF_Font *font;         //Pointer to the font we will be using to render the score.
	//SDL_Surface *scoretext; //Surface containing the text rendered by SDL_ttf that has the score.
	//bool bTTFStarted=false;
	//bool bTTFError=false;
/*
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
*/
	////////////////////////////////////////////////////////////////////
	//bool SafeChunkUnload(Mix_Chunk* &mcToNull) {
	//	if (mcToNull!=NULL) {
	//		Mix_FreeChunk(mcToNull);
	//		mcToNull=NULL;
	//	}
	//}
	void ClearSounds() {
		sounder.Clear();
	}
	void PlaySound(string sInstanceName, string sSoundName) {
		Mass3d m3dTemp;
		PlaySound(sInstanceName,sSoundName,m3dTemp);
	}
	void PlaySound(string sInstanceName, string sSoundName, Mass3d& m3dLoc) {
		Mass3d m3dTemp;
		PlaySound(sInstanceName,sSoundName,m3dLoc,m3dTemp);
	}
	void PlaySound(string sInstanceName, string sSoundName, Mass3d& m3dLoc, Mass3d& m3dVel) {
		sounder.SetOrCreateInstance(sInstanceName,sSoundName,m3dLoc,m3dVel);
	}
	void PlaySounds() {
		//int iPlayed=0;
		try {
			//TODO: finish this--uncomment: if (!oggMusic.update()) {
			//TODO: finish this--uncomment: 	if (bLoopMusic && sMusic!="") {
			//TODO: finish this--uncomment: 		oggMusic.open(sMusic);
			//TODO: finish this--uncomment: 		oggMusic.playback();
			//TODO: finish this--uncomment: 	}
			//TODO: finish this--uncomment: }
			//TODO: finish this--uncomment: else oggMusic.release();
			sounder.Refresh();
		}
		catch (exception& exn) {
			ShowException(exn,"PlaySounds");
		}
		//return iPlayed;
	}//
	////////////////////////////////////////////////////////////////////
	void Refresh3dCursor() {
		bool bGood=false;
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
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowException(exn,"Refresh3dCursor");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("Refresh3dCursor");
		}
	}//end Refresh3dCursor
	void Draw3dCursor(byte byBrightness) {
		Pixel pixelNow;
		FPOINT fpStart,//this point is first used as the hero weapon
			fpEnd;//this 2d point is first used as the 3d cursor
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
			catch (exception& exn) {
				ShowException(exn,"SayWhatIDrewIfFalse");
			}
			catch (...) {
				ShowUnknownException("SayWhatIDrewIfFalse");
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
	//float MetersToMove(float fMetersPerSecondX, REAL rSeconds) {
	//    return fMetersPerSecondX*rSeconds;//return (float)(  (double)fMetersPerSecondX*((double)(rSeconds)) / 1000.0  );
	//}
	//float DegreesToMove(float fDegreesPerSecondX, REAL rSeconds) {
	//    return fDegreesPerSecondX*rSeconds;//return (float)(  (double)fDegreesPerSecondX*((double)(rSeconds)) / 1000.0  );
	//}
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
				ShowError("Tried to draw uninitialized GBuffer!","DrawScreen");
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
								lpSrcPix+=4;//assumes 32-bit GBuffer
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
								lpSrcPix+=4;//assumes 32-bit GBuffer
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
									lpSrcPix+=4;//assumes 32-bit GBuffer
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
									lpSrcPix+=4;//assumes 32-bit GBuffer
									lpDestPix+=3;
								}
								lpDestLine+=iDestStride;
								lpSrcLine+=iSrcStride;
							}
						}
						break;
					case 4:
						Uint32* lpu32DestPix;
						if (bFirstRun) Console.Write("32-bit from "+ToString(gbScreen.iWidth)+"x"+ToString(gbScreen.iHeight)+"x"+ToString(gbScreen.iBytesPP*8)+"...");
						for (int y=0; y<iDestHeight; y++) {
							lpu32DestPix=(Uint32*)lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int x=0; x<iDestWidth; x++) {
								//*lpu32DestPix=*((Uint32*)lpSrcPix);
								*lpu32DestPix = (Uint32)SDL_MapRGB(screen->format, lpSrcPix[2], lpSrcPix[1], *lpSrcPix);
								lpSrcPix+=4;//assumes 32-bit GBuffer
								lpu32DestPix++;//ok since Uint32*
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
			bGood=false;
			ShowException(exn,"DrawScreen");
		}
		catch (...) {
			bGood=false;
			ShowUnknownException("DrawScreen");
		}
		if (bFirstRun) Console.WriteLine(bGood?"success.":"fail!");
		bFirstRun=false;
		return bGood;
	}//end DrawScreen

	inline void SleepWrapper(int iTicks) {
		SDL_Delay(iTicks);
	}

	void LoadSequence(Anim &animToLoad, string sFileBaseNameOnly, int iFramesExpectedElseError) {
		int iFramesFound;
		bool bGood=animToLoad.LoadSeq(sFileBaseNameOnly+"*.tga");
		if (!bGood)	ShowError(sFileBaseNameOnly+"*.tga failed to load","GameInit");
		iFramesFound=animToLoad.IFrames();
		if (bGood && iFramesFound!=iFramesExpectedElseError) {
			bGood=false;
			ShowError("not all "+sFileBaseNameOnly+ToString("*.tga frames loaded."));
		}
	}
	void LoadImage(GBuffer &gbToLoad, string sFile) {
		bool bGood=gbToLoad.Load(sFile);
		if (!bGood)	ShowError(sFile+".tga failed to load","GameInit");
	}
	bool GBuffer_FX_Scaled(GBuffer &gbDest, GBuffer &gbSrc, int xFlat, int yFlat,
			float fOpacity, float fExplodedness, UInt32 u32Stat, float fScale) {
		bool bGood=true;
		byte* arrbyDest=null;
		bool bShowVars=false;
		float fInverseScale;
		UInt32* lpu32Dest;
		UInt32* lpu32Src;
		int iStride;
		int iDestW, iDestH;
		UInt32* lpu32DestNow;

		int iSrcPixels;
		int iDestPixels;
		BYTE* lpbyDest;
		BYTE* lpbySrc;
		float bSrc,gSrc,rSrc,aSrc, bDest,gDest,rDest, bAtScaledExplodedDest,gAtScaledExplodedDest,rAtScaledExplodedDest, bCooked,gCooked,rCooked;
		BYTE bySrcB,bySrcG,bySrcR,bySrcA;
		UInt32 u32Pixel;
		int xExploder, xSrcNow;
		int yExploder, ySrcNow;
		bool bExplode;
		int iDestScaled, iDestScaledExploded;
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
	    float fAlphaness;
		float rExploded, gExploded, bExploded;
		UInt32 u32PixelMax;
		float fMiddleOfExplosion, fMaxExploder, fDistFromEnds;
		float fInvMiddle;
		int iExploder, iOffset, iDestExp;
		int iDestStart;
		long double iDestEnd;
		int iDestBytes;

		try {
			if (gbDest.arrbyData==NULL) {
				if (ShowError()) {
					cerr<<"Null dest image in GBuffer_FX_Scaled"<<endl;
					bShowVars=true;
				}
				bGood=false;
			}
			else if (gbSrc.arrbyData==NULL) {
				if (ShowError()) {
					cerr<<"Null source image in GBuffer_FX_Scaled"<<endl;
					bShowVars=true;
				}
				bGood=false;
			}
			else { //else OK
				fX=0; fY=0;
				iDestW=gbDest.iWidth;
				iDestH=gbDest.iHeight;
				fDestW=(float)iDestW;
				fDestH=(float)iDestH;
				xfFlat=(float)xFlat;
				yfFlat=(float)yFlat;
				fSrcW=(float)gbSrc.iWidth;
				fSrcH=(float)gbSrc.iHeight;
				iSrcW=(int)gbSrc.iWidth;
				iSrcH=(int)gbSrc.iHeight;
				ResetRand();//makes explosion uniform (?)
				if (fScale<.05f) fScale=.05;
				else if (fScale>10) fScale=10;//debug only
				fInverseScale=1.0f/fScale;
				if (fOpacity>1.0f) fOpacity=1.0f;
				lpu32Dest=(UInt32*)gbDest.arrbyData;
				lpu32Src=(UInt32*)gbSrc.arrbyData;
				iStride=gbDest.iStride;
				iDestBytes=iStride*(int)gbDest.iHeight;
				iDestStart=xFlat + (yFlat*iDestW);
				iDestEnd=(long double)iDestStart+((long double)(gbSrc.iStride)*fScale*(long double)gbSrc.iHeight*(long double)fScale);
				if (iDestStart<0||(iDestEnd>=(long double)iDestBytes)||xfFlat<0.0f||yfFlat<0.0f||((int)((xfFlat+fSrcW*fScale)+.5f)>=iDestW)||((int)((yfFlat+fSrcH*fScale)+.5f)>=iDestH)) {
					if (ShowError()) {
						cerr<<"Can't draw "<<(int)(fSrcW*fScale)<<"x"<<(int)(fSrcH*fScale)<<" image at index "<<iDestStart<<" "<<"("<<xFlat<<","<<yFlat<<") (spans to index "<<iDestEnd<<" ("<<xFlat+(int)((float)iSrcW*fScale+.5f)<<","<<yFlat+(int)((float)iSrcH*fScale+.5f)<<") of "<<iDestBytes<<") (was "<<fSrcW<<"x"<<fSrcH<<" scaled by "<<fScale<<").  Limit is ("<<fDestW<<","<<fDestH<<")"<<endl;
						bShowVars=true;
					}
					bGood=false;
				}
			}// end else good
			if (bGood) {
				lpu32DestNow=&lpu32Dest[iDestStart];
				iSrcPixels=gbSrc.iWidth*gbSrc.iHeight;
				iDestPixels=gbDest.iWidth*gbDest.iHeight;
				lpbyDest=(BYTE*)lpu32DestNow;
				lpbySrc;//=image.buffer;
				bExplode=(fExplodedness>0.0f)?true:false;
				iDestScaled=0; iDestScaledExploded=0;

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
				static BYTE by255=255;//avoids type conversions from double constant (?)

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
							lpbySrc=(BYTE*)&lpu32Src[iSrcNow];
							bSrc=*lpbySrc;
							gSrc=lpbySrc[1];
							rSrc=lpbySrc[2];
							aSrc=lpbySrc[3];
							aSrc*=fOpacity; //changes alpha according to param
							if (fOpacity<1.0f) { //debug this use of opacity is unique to DXMan; makes object red when transparent
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

							iDestScaled=xDestNow + ((yDestNow)*iDestW);
							if (bExplode) {
								//uses -= for implosion
								if (xDestRel<iHalfWidth) xExploder-=(int)(FRand(-xMaxExploder,0)*fExplodeLog);
								else xExploder-=(int)(FRand(0,xMaxExploder)*fExplodeLog);
								if (yDestRel<iHalfHeight) yExploder=(int)(FRand(-yMaxExploder,0)*fExplodeLog);
								else yExploder=(int)(FRand(0,yMaxExploder)*fExplodeLog);
								aSrc*=fRemainsLog;
								iDestScaledExploded=xDestNow+xExploder + ((yDestNow+yExploder)*iDestW);
							}
							else iDestScaledExploded=iDestScaled;

							if (iDestScaled>=iDestPixels || iDestScaled<0) {
								if (ShowError()) {
									cerr<<"Out of bounds of destination in GBuffer_FX_Scaled."<<endl;
									bShowVars=true;
								}
								bGood=false;
							}
							else if (iDestScaledExploded>=iDestPixels || iDestScaledExploded<0) {
								//if (ShowError()) {
								//	cerr<<"Out of exploded bounds of destination in GBuffer_FX_Scaled."<<endl;
								//	bShowVars=true;
								//}
								bGood=false;
							}
							if (bGood) {								//fAlphaness=aSrc/255.0f; //done below, only if blend
								if ((u32Stat & STATUS_SHIELD)&&aSrc<192.0f) {//debug: blue outline is only for DXMan
									bSrc=192.0f;
									gSrc=64.0f;
								}
					
								
								if (aSrc<15) {//transparent (draw nothing)
								}
								else if (aSrc>241) {//solid
									if (iDestScaledExploded<BUFFER_PIXELS) {
										if (bExplode) {
											//also paint second pixel (image-shaped) for explosion
											rExploded=APPROACH(255.0f,rDest,fExplodeExp);//rExploded=APPROACH(rDest,APPROACH(255.0f,rSrc,fExplodeExp),fAlphaness);
											gExploded=APPROACH(255.0f,gDest,fExplodeExp);//gExploded=APPROACH(gDest,APPROACH(255.0f,gSrc,fExplodeExp),fAlphaness);
											bExploded=APPROACH(255.0f,bDest,fExplodeExp);//bExploded=APPROACH(bDest,APPROACH(200.0f,bSrc,fExplodeExp),fAlphaness);
											//u32PixelMax=_RGB32BIT(255,(BYTE)rExploded,(BYTE)gExploded,(BYTE)bExploded);

											//lpu32Dest[iDestScaled]=u32PixelMax;
					 						arrbyDest=(byte*)&lpu32Dest[iDestScaled];
					 						*arrbyDest=(byte)bExploded;
					 						arrbyDest[1]=(byte)gExploded;
					 						arrbyDest[2]=(byte)rExploded;
					 						//arrbyDest[3]=(byte)aExploded;

											//commented for debug only:
											//fMiddleOfExplosion=.95, fMaxExploder=3.0f, fDistFromEnds;
											//fInvMiddle=(1.0f-fMiddleOfExplosion);
											//if (fInvMiddle<=0.0f) fInvMiddle=.1f;
											//fDistFromEnds=(fExplodedness<fMiddleOfExplosion)
											//	?(1.0f-fExplodedness/fMiddleOfExplosion)
											//	:(1.0f-((fExplodedness-fMiddleOfExplosion)/fInvMiddle));
											//fDistFromEnds=(fDistFromEnds>0.0f)?sqrt(fDistFromEnds):0;//logorithmic smoothing
											//iExploder=(int)(APPROACH(0.0f,fMaxExploder,fDistFromEnds)+.5) +1;
											//iDestExp=iDestScaled-(int)(iExploder/2)-(int)(iExploder/2)*iDestW;//go to top left of particle
											//iOffset=iDestW-iExploder;
											
											//for (register int ySubPix=0; ySubPix<iExploder; ySubPix++) {
					                        //	for (register int xSubPix=0; xSubPix<iExploder; xSubPix++) {
											//		if (iDestExp<0||iDestExp>=BUFFER_PIXELS) {ySubPix=iExploder;break;}
											//		lpu32Dest[iDestExp]=u32Pixel;//_RGB32BIT(255,(BYTE)rExploded,(BYTE)gExploded,(BYTE)bExploded);
											//		iDestExp++;
											//	}
											//	iDestExp+=iOffset;
											//}
					 					}// end if bExplode
					 					else {//lpu32Dest[iDestScaledExploded]=_RGB32BIT(by255,rSrc,gSrc,bSrc);
					 						arrbyDest=(byte*)&lpu32Dest[iDestScaledExploded];
					 						*arrbyDest=(byte)bSrc;
					 						arrbyDest[1]=(byte)gSrc;
					 						arrbyDest[2]=(byte)rSrc;
					 						//arrbyDest[3]=(byte)aSrc;
										}//else non-exploded solid pixel
									}//if iDestScaledExploded<BUFFER_PIXELS
								}//end if solid
								else {//blend alpha
									fAlphaness=(float)aSrc/255.0f;
									lpbyDest=(byte*)&lpu32Dest[iDestScaled];//first get original location's color
									bDest=*lpbyDest;
									gDest=lpbyDest[1];
									rDest=lpbyDest[2];
									lpbyDest=(BYTE*)&lpu32Dest[iDestScaledExploded];//now get exploded location's color
									bAtScaledExplodedDest=*lpbyDest;
									gAtScaledExplodedDest=lpbyDest[1];
									rAtScaledExplodedDest=lpbyDest[2];
									bCooked=APPROACH(bAtScaledExplodedDest,bSrc,fAlphaness);//by3dAlphaLookup[blue][*lpbyDest][bySrcA];
									gCooked=APPROACH(gAtScaledExplodedDest,gSrc,fAlphaness);//by3dAlphaLookup[green][*lpbyDest][bySrcA];
									rCooked=APPROACH(rAtScaledExplodedDest,rSrc,fAlphaness);//by3dAlphaLookup[red][*lpbyDest][bySrcA];
									//u32Pixel=_RGB32BIT(by255,(BYTE)rCooked,(BYTE)gCooked,(BYTE)bCooked);
									if (iDestScaledExploded<BUFFER_PIXELS) {
										if (bExplode) {
											//also paint second pixel (image-shaped) for explosion
											rExploded=APPROACH(rDest,APPROACH(255.0f,rSrc,fExplodeExp),fAlphaness);
											gExploded=APPROACH(gDest,APPROACH(255.0f,gSrc,fExplodeExp),fAlphaness);
											bExploded=APPROACH(bDest,APPROACH(200.0f,bSrc,fExplodeExp),fAlphaness);
											
											//u32PixelMax=_RGB32BIT(255,(BYTE)rExploded,(BYTE)gExploded,(BYTE)bExploded);
											//lpu32Dest[iDestScaled]=u32PixelMax;
					 						arrbyDest=(byte*)&lpu32Dest[iDestScaled];
					 						*arrbyDest=(byte)bExploded;
					 						arrbyDest[1]=(byte)gExploded;
					 						arrbyDest[2]=(byte)rExploded;
					 						//arrbyDest[3]=aExploded;
											
											//commented for debug only:
											//fMiddleOfExplosion=.95, fMaxExploder=3.0f, fDistFromEnds;
											//fInvMiddle=(1.0f-fMiddleOfExplosion);
											//if (fInvMiddle<=0.0f) fInvMiddle=.1f;
											//fDistFromEnds=(fExplodedness<fMiddleOfExplosion)
											//	?(1.0f-fExplodedness/fMiddleOfExplosion)
											//	:(1.0f-((fExplodedness-fMiddleOfExplosion)/fInvMiddle));
											//fDistFromEnds=(fDistFromEnds>0.0f)?sqrt(fDistFromEnds):0;//logorithmic smoothing
											//iExploder=(int)(APPROACH(0.0f,fMaxExploder,fDistFromEnds)+.5) +1;
											//iDestExp=iDestScaled-(int)(iExploder/2)-(int)(iExploder/2)*iDestW;//go to top left of particle
											//iOffset=iDestW-iExploder;
											//commented for debug only:
											//for (register int ySubPix=0; ySubPix<iExploder; ySubPix++) {
					                        //    for (register int xSubPix=0; xSubPix<iExploder; xSubPix++) {
											//		if (iDestExp<0||iDestExp>=BUFFER_PIXELS) {ySubPix=iExploder;break;}
											//		lpu32Dest[iDestExp]=u32Pixel;//_RGB32BIT(255,(BYTE)rExploded,(BYTE)gExploded,(BYTE)bExploded);
											//		iDestExp++;
											//	}
											//	iDestExp+=iOffset;
											//}
					 					}// end if bExplode
					 					else {//lpu32Dest[iDestScaledExploded]=u32Pixel;
					 						arrbyDest=(byte*)&lpu32Dest[iDestScaledExploded];
					 						*arrbyDest=(byte)bCooked;
					 						arrbyDest[1]=(byte)gCooked;
					 						arrbyDest[2]=(byte)rCooked;
					 						//arrbyDest[3]=aSrc;
										}//end else don't explode
									}//if iDestScaledExploded<BUFFER_PIXELS
								}//end if blend alpha
							}// end if good
						}// end if good
						else {
							break;
						}
					}// end for xDestNow
					if (iDestScaled>=BUFFER_PIXELS) break;
					if (!bGood) break;
				}// end for yDestNow
			}// end if SOURCE in bounds (bGood)
		}// end try
		catch (exception& exn) {
			bGood=false;
			bShowVars=true;
			ShowException(exn,"GBuffer_FX_Scaled");
		}
		catch (...) {
			bGood=false;
			bShowVars=true;
			ShowUnknownException("GBuffer_FX_Scaled");
		}
		if (bShowVars) {
			ShowError("  { iDestScaled:"+ToString(iDestScaled)+"; source.w:"+ToString(gbSrc.iWidth)+"; source.h:"+ToString(gbSrc.iHeight)+"; xFlat:"+ToString(xFlat)+"; yFlat:"+ToString(yFlat)+"; fX:"+ToString(fX)+"; fY:"+ToString(fY)+"; xDestNow:"+ToString(xDestNow)+"; yDestNow:"+ToString(yDestNow)+"; fInverseScale:"+ToString(fInverseScale)+"; fScale:"+ToString(fScale)+"; fExplodedness:"+ToString(fExplodedness)+"; }");
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
    	Console.WriteLine("GameInit Done.");
		SleepWrapper(3000);
	}
	else  {
		Console.WriteLine("GameInit Done (did not try to load splash screen, since bSplash is turned off).");
	}
	//main event loop:
   	if (!bDone) Console.WriteLine("Running main event loop.");
   	else ShowError("Bypassing main loop, due to failed init.");
	UInt32 u32LastRefresh=SDL_GetTicks();//TODO: change to seconds??
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
					u32Pressing|=GAMEKEY_JUMP;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					bMouseDown=false;
				}
				else {
					bMouseDownR=false;
					u32Pressing&=GAMEKEY_JUMP^0xFFFFFFFF;
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
					u32Pressing|=GAMEKEY_LEFT;
				}
				else if (event.key.keysym.sym==SDLK_d) {
					DirKeyDown();
					u32Pressing|=GAMEKEY_RIGHT;
				}
				else if (event.key.keysym.sym==SDLK_w) {
					DirKeyDown();
					u32Pressing|=GAMEKEY_UP;
				}
				else if (event.key.keysym.sym==SDLK_s) {
					DirKeyDown();
					u32Pressing|=GAMEKEY_DOWN;
				}
				else if (event.key.keysym.sym==SDLK_DELETE) {
					//laser keydown
					u32Pressing|=GAMEKEY_FIRE;
				}
				else if (event.key.keysym.sym==SDLK_END) {
					u32Pressing|=GAMEKEY_JUMP;
				}
				else if (event.key.keysym.sym==SDLK_ESCAPE) {
					u32Pressing|=GAMEKEY_EXIT; //commented for debug only
				}
				else if (event.key.keysym.sym==SDLK_LEFT) {
					u32Pressing|=GAMEKEY_FIRE;
					u32Pressing|=GAMEKEY_LEFT2;
				}
				else if (event.key.keysym.sym==SDLK_RIGHT) {
					u32Pressing|=GAMEKEY_FIRE;
					u32Pressing|=GAMEKEY_RIGHT2;
				}
				else if (event.key.keysym.sym==SDLK_UP) {
					u32Pressing|=GAMEKEY_FIRE;
					u32Pressing|=GAMEKEY_UP2;
				}
				else if (event.key.keysym.sym==SDLK_DOWN) {
					u32Pressing|=GAMEKEY_FIRE;
					u32Pressing|=GAMEKEY_DOWN2;
				}
				else if (event.key.keysym.sym==SDLK_2) {
					u32Pressing|=GAMEKEY_DOUBLESPEED;
				}
				else if (event.key.keysym.sym==SDLK_r) {
					u32Pressing|=GAMEKEY_RAPIDFIRE;
				}/*
				else if (event.key.keysym.sym==SDLK_KP_PLUS) {
				//This case is only for testing--smart Bomb to skip level!!
					if (!bBombed) {
						bBombed=true;
						for (int index=0; index<iMaxAliensNow; index++) {
							if (arrpentAlien[index] != NULL) {
								delete (Entity*)arrpentAlien[index];
								arrpentAlien[index]=NULL;
							}
						}
					}
				}*/
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym==SDLK_a) {
					DirKeyUp();
					u32Pressing&=(GAMEKEY_LEFT^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_d) {
					DirKeyUp();
					u32Pressing&=(GAMEKEY_RIGHT^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_w) {
					DirKeyUp();
					u32Pressing&=(GAMEKEY_UP^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_s) {
					DirKeyUp();
					u32Pressing&=(GAMEKEY_DOWN^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_DELETE) {
					//iChanLaser=Mix_PlayChannel(2, mcLaser, 0);//chan, sound, num of loops
					u32Pressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_END) {
					u32Pressing&=(GAMEKEY_JUMP^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_ESCAPE) {
					u32Pressing&=(GAMEKEY_EXIT^0xFFFFFFFF);
				}
				else if (event.key.keysym.sym==SDLK_LEFT) {
					u32Pressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
					u32Pressing&=GAMEKEY_LEFT2^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_RIGHT) {
					u32Pressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
					u32Pressing&=GAMEKEY_RIGHT2^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_UP) {
					u32Pressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
					u32Pressing&=GAMEKEY_UP2^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_DOWN) {
					u32Pressing&=(GAMEKEY_FIRE^0xFFFFFFFF);
					u32Pressing&=GAMEKEY_DOWN2^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_2) {
					u32Pressing&=GAMEKEY_DOUBLESPEED^0xFFFFFFFF;
				}
				else if (event.key.keysym.sym==SDLK_r) {
					u32Pressing&=GAMEKEY_RAPIDFIRE^0xFFFFFFFF;
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
			//SleepWrapper(1);
			//SleepWrapper( (SDL_GetTicks()-u32LastRefresh) + 28)
			//TODO: separate Redraw from Refresh GameMain below
			if ( (SDL_GetTicks()-u32LastRefresh) >= 28 ) {//lock to approx 30fps
				u32LastRefresh=SDL_GetTicks();
				GameMain();
			}
			else SleepWrapper(1);
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
