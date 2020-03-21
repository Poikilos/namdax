#ifndef DXMAN_H
#define DXMAN_H

#include <anim32bgra.h>
#include <gfont32bgra.h>
#include "entity.h"
#include "camera.h"

///#region avoid SDL.h in files other than one with entry point
typedef float ALfloat;
typedef unsigned int ALuint;
///#endregion avoid SDL.h in files other than one with entry point

using namespace std;

namespace ExpertMultimediaBase {
	//typedef struct {int left; int right; int top; int bottom;} RECT;
	typedef unsigned short USHORT;
	typedef unsigned short WORD;
	typedef unsigned char BYTE;
	//typedef UInt32 DWORD;
	//typedef unsigned __int16 USHORT;
	//typedef unsigned __int8 BYTE;
	//typedef ULARGE_INTEGER __int64);

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
	#define SCREEN_WIDTH				640
	#define SCREEN_HEIGHT			480
	#define FSCREEN_WIDTH			640.0f
	#define FSCREEN_HEIGHT			480.0f
	#define BUFFER_WIDTH				1280
	#define BUFFER_HEIGHT			960
	#define BUFFER_PIXELS			1228800 //1280x960
	#define SCREEN_OFFSET_X			320 //screen (a GBuffer) is offset to avoid clipping
	#define SCREEN_OFFSET_Y			240
	#define FSCREEN_OFFSET_X		320.0f //screen (a GBuffer buffer) is offset to avoid clipping
	#define FSCREEN_OFFSET_Y		240.0f
	#define SCREEN_BPP				32	 //bits per pixel, GameInit tries 32 or 24 if one is not available
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

	#define SCREENITEM_ALIEN	1
	#define SCREENITEM_SHOT		2
	#define SCREENITEM_HERO		3
	#define SCREENITEM_3DCURSOR 4
	#define MAXSHOTS		50
	#define MAX_SCREENITEMS		58  //maximum number of objects to render per frame
	#define MAX_ZORDER		50   //maximum zOrder value - INCLUSIVE
	#define FMAX_ZORDER		50.0f
	//DEBUG: ZBUFFER_YRATIO MUST be 50/YRANGE
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
	#define ENTITY_TYPE_HERO  1
	#define ENTITY_TYPE_SHOT  2
	#define ENTITY_TYPE_ALIEN 3
	#define ENTITY_TYPE_BOSS  4

	#define FRADAR_ZOOM			1.0f
	#define FRADAR_ZOOMINNER	1.0f
	#define XPIX_RADAR  		564
	#define YPIX_RADAR			55
	#define RADAR_TOP           0
	#define RADAR_BOTTOM        110
	#define RADAR_LEFT          490
	#define RADAR_RIGHT         639


	typedef struct SCREENITEM_STRUCT {
		int iType;//entity type i.e. SCREENITEM_ALIEN
		int zOrder;
		int index;//entity index, irrelevant for hero etc
	} SCREENITEM, * LPSCREENITEM;



	bool GameMain();
	bool GameInit();
	/////////////////////////////////////////////////////////////
	bool GameShutdown();
	int GetTicks_Absolute();
	Uint32 GetTicks_Relative();
	inline void SleepWrapper(int iTicks);
	void LoadSequence(Anim &animToLoad, string sFileBaseNameOnly, int iFramesExpectedElseError);
	void LoadImage(GBuffer &gbToLoad, string sFile);
	int PickChan();
	void InitSound();
	void InitMusic();
	void PlayMusic(string sFile, bool bLoop);
	void StopMusic();
	bool ShutdownMusic();
	bool ShutdownAudio();
	void MusicDone();
	//inline void SetPan(int iChan, int iLocation);
	inline void UpdateThrustPan();
	void DirKeyDown();
	void DirKeyUp();
	///////////////////////////////////////////////////////////////
	REAL Base_GetSeconds();
	REAL Base_SecondsSince(REAL rSecondCount);
	int IRandPositive();
	int IRand(int iMin, int iMax);
	void ResetRand();
	float FRand();
	float FRand(float fMin, float fMax);
	///////////////////////////////////////////////////////////////
	bool AddScreenItem(int iType, int zOrder, int iEntityIndex);
	////////////////////////////////////////////////////////////
	void QuitSafe();
	////////////////////////////////////////////////////////////////////////////////
	float RadarCenterX();
	float RadarCenterY();
	float RadarHeight();
	void RadarDotAt(int &xPixReturn, int &yPixReturn, float xPos, float yPos);
	void DrawRadarDot(float xPos, float yPos, float zPos, Uint32 u32Pixel);
	void DrawRadarDot(float xPos, float yPos, float zPos, Uint32 u32Pixel, bool bFilled);
	void DrawRadarRect(float left, float top, float bottom, float right, Uint32 u32Pixel, bool bFilled);
	void DrawRadarField();
	void DrawExclusiveRect(int left, int top, int bottom, int right, Uint32 u32Pixel, bool bFilled);
	////////////////////////////////////////////////////////////////////
	//bool SafeChunkUnload(Mix_Chunk* &mcToNull);
	void ClearSounds();
	void PlaySound(string sInstanceName, string sSoundName);
	void PlaySound(string sInstanceName, string sSoundName, Mass3d& m3dLoc);
	void PlaySound(string sInstanceName, string sSoundName, Mass3d& m3dLoc, Mass3d& m3dVel);
	void PlaySounds();
	////////////////////////////////////////////////////////////////////
	void Refresh3dCursor();
	void Draw3dCursor(byte byBrightness);
	void SayWhatIDrewIfFalse(bool bDrawFunctionReturn, int iEntityType, string sDescription);
	string EntityTypeToString(int iEntityType);
	float MetersToMove(float fMetersPerSecondX, int iForThisManyMilliseconds);
	float DegreesToMove(float fDegreesPerSecondX, int iForThisManyMilliseconds);
	//float MetersToMove(float fMetersPerSecondX, REAL rSeconds);
	//float DegreesToMove(float fDegreesPerSecondX, REAL rSeconds);
	void DrawCube(Mass3d &m3dNow, Pixel &pixelNear, Pixel &pixelFar);
	bool DrawScreen();
	bool GBuffer_FX_Scaled(GBuffer &gbDest, GBuffer &gbSrc, int xFlat, int yFlat,
			float fOpacity, float fExplodedness, UInt32 u32Stat, float fScale);

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

	class Sound {
	public:
		string sName;
		Sound();
	};

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


	//extern string sarrGameState[] = {"GAMESTATE_INIT","GAMESTATE_START_AREA","GAMESTATE_START_ENCOUNTER","GAMESTATE_RUN","GAMESTATE_SHUTDOWN","GAMESTATE_EXIT","GAMESTATE_WIN_ENCOUNTER","GAMESTATE_WIN_GAME","GAMESTATE_YOU_LOSE","GAMESTATE_LOSEGUY"};
	//extern long narrGameStateCount[] = {0,0,0,0,0,0,0,0,0,0};

	///#region globals as extern in h file
	extern bool bDoneShutdown;
	extern bool bShuttingDown;
	extern string sVerb_Engine;
	extern UInt32 u32Pressing;
	extern int iScreenItems;
	extern bool bInMenu;
		//TODO: finish this--uncomment: ogg_stream oggMusic;//Mix_Music *music=NULL;
	extern GFont gfontDefault;
	extern long narrGameStateCount[];
	//extern ALfloat arralfListenerPos[] = { 0.0, 0.0, 0.0 };
	//extern ALfloat arralfListenerVel[] = { 0.0, 0.0, 0.0 };
	//extern ALfloat arralfListenerRot[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };//(at,at,at,up,up,up)
	extern bool bClosedAudio;
	extern const REAL Base_r0;
	extern const REAL Base_r1000;
	extern FPOINT3D p3dHero;
	extern bool bDebug;
	extern Mass3d m3dDebug[9];
	extern Mass3d m3dHeroEye, m3dCursor;
	//extern float xAimFlipper;
	//extern float yAimFlipper;
	//extern float zAimFlipper;
	extern bool bFlipYaw;
	extern bool bFlipPitch;
	extern float fBoss;//(Entity*)arrpentAlien[iBoss]->fHealth;
	extern float fBossMax;//(Entity*)arrpentAlien[iBoss]->fMaxHealth;
	extern int iBoss;//global link to boss;
	extern bool bSDLQuitDone;
	extern Variables settings;
	extern Variables dat;
	extern bool bSDLQuitSent;
	extern bool bSplash;//if splash screen was drawn
	extern int iSplashTick;//time when splash screen appeared
	extern UInt32 arru32EnergyGrad[256];//gradient for energy currency (ready attack) meter
	extern UInt32 arru32HealthGrad[256];//gradient for health meter
	extern int xCursor, yCursor, xCursorDown, yCursorDown;
	extern bool bMouseDown;
	extern bool bMouseMove;
	extern bool bMouseDownR;
	extern float xfCursor,yfCursor,xfCursorDown,yfCursorDown;

	extern int xCursorCenter, yCursorCenter;
	extern int iGuysStart;
	extern int iGuys;

	extern int iAreas,
		iBackdrops,
		iFramesCursor,
		iEncounters;

	extern int iFramesHero,
		iFramesHeroShadow,
		iFramesFlyer,
		iFramesFlyerShadow,
		iFramesBoss,
		iFramesBossShadow,
		iFramesShot,
		iFramesShotShadow,
		iFramesGameScreen,
		iBackDrops;

	extern int iEscapeTime;
	extern int iErrorsSaved;
	extern int iFramesDropped;
	extern int REAL_BPP;
	extern int REAL_BYTEDEPTH;
	extern int keyDelay;
	extern int iDoubleCodeCounter; //player cheat code
	extern int iRapidFireCodeCounter;//player cheat code
	extern bool bBombed;
	extern Anim				animBackdrop;
	extern GBuffer			gbIntro;
	extern GBuffer			gbIntroText;
	extern GBuffer			gbSymbolShield;
	extern GBuffer			gbSymbolBossHealth;
	extern GBuffer			gbGuysDecor;
	extern Anim				animFlyer;
	extern Anim				animFlyerShadow;
	extern Anim				animBoss;
	extern Anim				animBossShadow;
	extern Anim				animHero;
	extern Anim				animHeroShadow;
	extern Anim				animShot;
	extern Anim				animShotShadow;
	extern Anim				animGameScreen;
	extern Anim				animGameScreenArea;
	extern Anim				animGameScreenEncounter;
	extern Anim				animCursor;
	extern GBuffer			gbScreen;
	//extern LPSPRITE*				lpspritearr;//sprite array
	extern SCREENITEM				screenitemarr[MAX_SCREENITEMS];
	extern int explosionResult;
	extern int iGameStateWas;
	extern int iGameState;
	extern int iArea;		//reset later in GAME_STATE_INIT			//3 Areas total
	extern int iEncounter; //set to 1 again later in GAME_STATE_INIT					//3 Encounters total, encounters are in Areas
	extern int iAliens;
	extern int iMaxAliens;//changed later using ini file
	extern int iMaxAliensNow;//changes by level
	extern bool bPlayTrumpet;
	extern string sMusic;
	extern bool bLoopMusic;
	// Buffers hold sound data.
	extern ALuint aluiBuffer;

	// Sources are points emitting sound.
	extern ALuint aluiSource;
	/*
	extern int iChanBomb;
	extern int iChanLaserAlien;
	extern int iChanLaser;
	extern int iChanExplosion;
	extern int iChanOuchAlien;
	extern int iChanOuchZap;
	extern int iChanShieldZap;
	extern int iChanBlerrp;
	extern int iChanHitDirt;
	extern int iChanJump;
	extern int iChanScrapeGround;
	extern int iChanAngryAlien;
	extern int iChanThruster;
	extern int iChanTrumpet;
	*/
	extern bool bSounder;
	extern Camera camera;
	extern Sounder sounder;
	extern Entity* arrpentShot[MAXSHOTS];
	extern Entity** arrpentAlien;
	//TODO: improve boss rect alignment
	extern Entity* hero;
	/* The screen surface */
	extern SDL_Surface* screen;
	extern int iDirKeysDown;
	extern bool bDone;
	///#endregion globals as extern in h file

}//end namespace

#endif
