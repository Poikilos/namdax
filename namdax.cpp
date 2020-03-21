#ifndef DXMAN_CPP
#define DXMAN_CPP
//#include <windows.h>
//#define LINUX

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <base.h>
#include <RFont_bgra32.h>
#include <RAnim_bgra32.h>
#include "entity.h"
#include "camera.h"
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
*/
//#else
//next line is for OGG!
#define BUFFER_SIZE (4096 * 8)
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
//ALUT from <http://connect.creativelabs.com/openal/Downloads/Forms/AllItems.aspx> 2009-09-04
#include <AL/al.h>//<AL/al.h>
#include <AL/alc.h>//<AL/alc.h>
#include <AL/alut.h>//<AL/alut.h>
#include "dxman.h"
//TODO: finish this--uncomment: #include <ogg/ogg.h>
//TODO: finish this--uncomment: #include <vorbis/codec.h>
//TODO: finish this--uncomment: #include <vorbis/vorbisenc.h>
//TODO: finish this--use: #include <vorbis/vorbisfile.h>

//next line is for OGG!
//#define BUFFER_SIZE (4096 * 8)

//#endif

using namespace std;

namespace ExpertMultimediaBase {

	//extern string EntityTypeToString(int iTypeX);
	//typedef struct {int left; int right; int top; int bottom;} RECT;
	//typedef unsigned short USHORT;
	//typedef unsigned short WORD;
	//typedef unsigned char BYTE;
	//typedef unsigned __int8 DWORD;
	//typedef unsigned __int16 USHORT;
	//typedef unsigned __int8 BYTE;
	//typedef ULARGE_INTEGER __int64);

	string sarrGameState[] = {"GAMESTATE_INIT","GAMESTATE_START_AREA","GAMESTATE_START_ENCOUNTER","GAMESTATE_RUN","GAMESTATE_SHUTDOWN","GAMESTATE_EXIT","GAMESTATE_WIN_ENCOUNTER","GAMESTATE_WIN_GAME","GAMESTATE_YOU_LOSE","GAMESTATE_LOSEGUY"};
	long narrGameStateCount[] = {0,0,0,0,0,0,0,0,0,0};
	//int iTicksDisplayHelpMessage1=0;
	//int iTickLastDisplayHelpMessage1=-1;
	///#region globals as extern in h file
	GFont gfontDefault;
	bool bDoneShutdown=false;
	bool bShuttingDown=false;
	string sVerb_Engine="before initialization";
	UInt32 u32Pressing=0;
	//DEBUG: ZBUFFER_YRATIO MUST be 50/YRANGE
	int iScreenItems=0;
	bool bInMenu=true;
	Uint32 dwAbsoluteTickLastGameStateState_Run=0;
	//TODO: finish this--uncomment: ogg_stream oggMusic;//Mix_Music *music=NULL;
	ALfloat arralfListenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat arralfListenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat arralfListenerRot[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };//(at,at,at,up,up,up)
	bool bClosedAudio=false;
	FPOINT3D p3dHero;
	//bDebug=true;
	//bMegaDebug=true; //true for debug only!
	long iMainEventLoopCount=0;//counts iterations of main event loop
	Mass3d m3dDebug[9];
	Mass3d m3dHeroEye, m3dCursor;
	//float xAimFlipper=-1.0f;
	//float yAimFlipper=-1.0f;
	//float zAimFlipper=-1.0f;
	bool bFlipYaw=false;
	bool bFlipPitch=false;
	float fBoss=0;//arrpentAlien[iBoss]->fHealth;
	float fBossMax=1;//arrpentAlien[iBoss]->fMaxHealth;
	int iBoss=-1;//global link to boss;
	bool bSDLQuitDone=false;
	Variables settings;
	Variables dat;
	bool bSDLQuitSent=false;
	bool bSplash=false;//if splash screen was drawn
	int iSplashTick;//time when splash screen appeared
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
		iEncounters=3;

	int iFramesHero=60,
		iFramesHeroShadow=1,
		iFramesFlyer=60,
		iFramesFlyerShadow=2,
		iFramesBoss=3,
		iFramesBossShadow=1,
		iFramesShot=3,
		iFramesShotShadow=1,
		iFramesBurn=4,
		iFramesExpArt=16,
		iFramesGameScreen=9,
		iFramesCursor=3;//TODO: actually use these instead of hard-coding frames
						//may be okay as long as they're reset to iFoundFrames upon TargaLoadSeq
	int iTickAbsoluteEscapeLastPress=0;
	int iErrorsSaved=0;
	int iFramesDropped=0;
	int REAL_BPP=32;
	int REAL_BYTEDEPTH=4;
	int keyDelay=0;
	int iDoubleCodeCounter=0; //player cheat code
	int iRapidFireCodeCounter=0;//player cheat code
	bool bBombed=false;
	Anim		animBackdrop;
	GBuffer		gbScreen;
	GBuffer		gbIntro;
	GBuffer		gbIntroTextBack;
	GBuffer		gbIntroText;
	GBuffer		gbSymbolShield;
	GBuffer		gbSymbolBossHealth;
	GBuffer		gbLives;
	Anim		animHero;
	Anim		animHeroShadow;
	Anim		animFlyer;
	Anim		animFlyerShadow;
	Anim		animBoss;
	Anim		animBossShadow;
	Anim		animShot;
	Anim		animBurn;
	Anim		animExpArt;//TODO: implement this
	Anim		animShotShadow;
	Anim		animGameScreen;
	Anim		animGameScreenArea;
	Anim		animGameScreenEncounter;
	Anim		animCursor;
	//LPSPRITE*	lpspritearr;//sprite array
	SCREENITEM	screenitemarr[MAX_SCREENITEMS];
	int explosionResult=0;
	int iGameState=GAMESTATE_INIT;
	int iGameStateWas=iGameState;
	int iArea=1;		//reset later in GAMESTATE_INIT			//3 Areas total
	int iEncounter=1; //set to 1 again later in GAMESTATE_INIT					//3 Encounters total, encounters are in Areas
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
	bool bSounder=false;
	Sounder sounder;
	Camera camera;
	Entity* arrpentShot[MAXSHOTS];
	Entity** arrpentAlien;
	Entity* hero=NULL;
	//TODO: improve boss rect alignment
	/* The screen surface */
	SDL_Surface *screen=NULL;
	int iDirKeysDown=0;
	bool bDone=false;
	///#endregion globals defined as extern in h file
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
	int audio_rate=44100;
	Uint16 audio_format=AUDIO_S16; /* 16-bit stereo */
	int audio_channels=2;
	int audio_buffers=4096;
	/*
	//Mix_Music *music=NULL;
	Mix_Music *musicIntro=NULL;
	Mix_Music *musicInvasion=NULL;
	Mix_Music *musicEnding=NULL;
	bool bStartedIntroMusic=false;

	bool MusicThreadFunction_bStarted=false;

	string MusicFileName_Intro="music/Orangejuice-DXMan-Intro.ogg";
	string MusicFileName_Invasion="music/Orangejuice-DXMan-Invasion.ogg";
	string MusicFileName_Ending="music/Orangejuice-DXMan-Ending.ogg";

	string MusicThread_Play_MusicFileName="";
	int MusicThread_Play_MusicLoopCount=0;
	string MusicThread_Play_MusicFileNameing="";
	int MusicThread_LastKeepAliveAbsoluteTick=0;

	bool MusicThread_bClosedAudio=false;
	bool MusicThread_bCloseAudio=false;
	bool MusicThread_bReturn=false;
	int MusicThread_iIterationDelayTicks=100;

	SDL_Thread* psdl_threadMusicThread=NULL;
	*/

	SDL_Event event;

	float fFibbo=1;
	float fFibboPrev=0;
	//TTF_Font *font;		 //Pointer to the font we will be using to render the score.
	//SDL_Surface *scoretext; //Surface containing the text rendered by SDL_ttf that has the score.
	//bool bTTFStarted=false;
	//bool bTTFError=false;
	//void ShowDebugVars();

	bool bFirstRunGameMain=true;

	///#region functions
	bool GameMain() {
		bool bGood=true;
		bool bFirstRunOfCurrentState=(narrGameStateCount[iGameState]<1);
		iGameStateWas=iGameState;
		sVerb_Engine="running "+sarrGameState[iGameState];
		if (bMegaDebug) Console::Error.WriteLine("Running "+sarrGameState[iGameState]+" ("+RString_ToString(narrGameStateCount[iGameState])+" so far)");
		try {
			if (bDone) return(false);
			// for now test if user is hitting ESC
			if (iGameState!=GAMESTATE_EXIT) {
				if ((u32Pressing&GAMEKEY_EXIT)&&(SDL_GetTicks()-iTickAbsoluteEscapeLastPress)>1000) {//int answer=0;
					iTickAbsoluteEscapeLastPress=SDL_GetTicks();
					animGameScreen.GotoFrame(2);
					animGameScreen.DrawToLargerWithoutCropElseCancel(gbScreen, (int)( FSCREEN_WIDTH/2.0f-animGameScreen.Width()/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-(float)animGameScreen.Height()/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha);
					if (bMegaDebug) {
						Console::Error.Write("About to show screen in GAMESTATE_EXIT...");
						Console::Error.Flush();
					}
					DrawScreen();
					if (bMegaDebug) Console::Error.WriteLine("done showing screen in GAMESTATE_EXIT.");
					iTickAbsoluteEscapeLastPress=SDL_GetTicks();
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
								if((SDL_GetTicks() - iTickAbsoluteEscapeLastPress) > 1000) {
									u32Pressing&=GAMEKEY_EXIT^0xFFFFFFFF;
									iGameState=GAMESTATE_RUN;
									bContinue=false;//TODO: should this be TRUE (since no exiting now)???
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
			}//end GAMESTATE_EXIT if not already exiting


			if (iGameState==GAMESTATE_INIT) {
				DrawExclusiveRect(0,0,SCREEN_HEIGHT,SCREEN_WIDTH,0x00000000,true);
				gbIntro.DrawToLargerWithoutCropElseCancel(gbScreen,(int)( FSCREEN_OFFSET_X+(FSCREEN_WIDTH/2.0f-(float)gbIntro.iWidth/2.0f) ),SCREEN_OFFSET_Y,DrawModeCopyAlpha);
				if (bMegaDebug) {
					Console::Error.Write("About to show screen in GAMESTATE_INIT...");
					Console::Error.Flush();
				}
				DrawScreen();
				if (bMegaDebug) {
					Console::Error.Write("done showing screen in GAMESTATE_INIT.");
					Console::Error.Flush();
				}
				SleepWrapper(1500);
				bool bStart=false;
				if (bMegaDebug) Console::Error.WriteLine("Waiting for user to press key/button at Intro Screen in GAMESTATE_INIT.");
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
				}//end while showing intro-title
				if (bMegaDebug) Console::Error.WriteLine("Clearing screen in GAMESTATE_INIT.");
				DrawExclusiveRect(0,0,SCREEN_HEIGHT,SCREEN_WIDTH,0x00000000,true);
				bStart=false;//reset to false in order to show intro text
				Uint32 dwAbsoluteTickStartIntroText=SDL_GetTicks();
				float yIntroTextF=(float)(SCREEN_OFFSET_Y+SCREEN_HEIGHT);
				float fOpacityIntroText=.1f;
				//if (bMegaDebug) Console::Error.WriteLine("Initializing music in GAMESTATE_INIT.");
				//InitMusic();
				if (bMegaDebug) Console::Error.WriteLine("Playing music in GAMESTATE_INIT.");
				//if (settings.GetForcedBool("music")) {
				//	if (!bStartedIntroMusic) VirtualMusicPlay(MusicFileName_Intro, -1);
				//	bStartedIntroMusic=true;
				//}
				if (bMegaDebug) Console::Error.WriteLine("Waiting for user to press key/button at Intro Text scroll in GAMESTATE_INIT.");
				int xIntroTextI=SCREEN_OFFSET_X+(SCREEN_WIDTH/2-gbIntroText.iWidth/2);
				while (!bStart) { //mini polling loop THREE OF FOUR! (show intro text)
					if ((int)yIntroTextF+gbIntroText.iHeight <= SCREEN_OFFSET_Y+(SCREEN_HEIGHT/3) ) fOpacityIntroText*=.9f;
					else if (fOpacityIntroText<1.0f) {
							fOpacityIntroText+=fOpacityIntroText*(1.0f-fOpacityIntroText);//logorithmic gain
							if (fOpacityIntroText>1.0f) fOpacityIntroText=1.0f;
					}

					if ((int)yIntroTextF+gbIntroText.iHeight <= SCREEN_OFFSET_Y) break;
					if (bMegaDebug) {
						Console::Error.WriteLine("B");
						Console::Error.Flush();
					}
					gbIntroTextBack.DrawToLargerWithoutCropElseCancel(gbScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y, DrawModeCopyAlpha);
					if (IROUNDF(yIntroTextF)>(-1*(gbIntroText.iHeight))) {
						if (bMegaDebug) {
							Console::Error.WriteLine("F");
							Console::Error.Flush();
						}
						gbIntroText.DrawToWithClipping(gbScreen, xIntroTextI,IROUNDF(yIntroTextF), fOpacityIntroText);
					}
					if (bMegaDebug) {
						Console::Error.Write("D("+RString_ToString(xIntroTextI)+","+RString_ToString(yIntroTextF)+")");
						Console::Error.Flush();
					}
					DrawScreen();
					yIntroTextF-=1.0f;

					if (SDL_PollEvent (&event)) {
						if (SDL_GetTicks()-dwAbsoluteTickStartIntroText > 1000) {
							if (event.key.keysym.sym==SDLK_DELETE
							  || event.key.keysym.sym==SDLK_UP
							  || event.key.keysym.sym==SDLK_DOWN
							  || event.key.keysym.sym==SDLK_LEFT
							  || event.key.keysym.sym==SDLK_RIGHT
							  || event.type==SDL_MOUSEBUTTONDOWN) {
								bStart=true;
								break;
							}
						}
						//else if (event.type==SDL_MOUSEMOTION) {
						//	xCursor=event.motion.x;
						//	yCursor=event.motion.y;
						//	xfCursor=xCursor;
						//	yfCursor=yCursor;
						//	bMouseMove=true;
						//}
						//try { TargaToTarga32_Alpha(animCursor[0], gbScreen, xCursor+SCREEN_OFFSET_X-xCursorCenter, yCursor+SCREEN_OFFSET_Y-yCursorCenter); }
						//catch (...) {}
					}//end if key is pressed
					SleepWrapper(16);//~60fps
				}//end while showing intro text
				if (iGameState==GAMESTATE_EXIT) {Console::Error.WriteLine("iGameState was GAMESTATE_EXIT at start of game!"); return(false);}
				try {
					SDL_ShowCursor(SDL_DISABLE);
				}
				catch (exception& exn) {
					ShowExn(exn,"GameMain(){state:INIT}", "hiding cursor");
				}
				catch (...) {
					ShowUnknownExn("GameMain(){state:INIT}", "hiding cursor");
				}
				if (bMegaDebug) Console::Error.WriteLine("Setting up entity objects in GAMESTATE_INIT.");
				if (hero!=NULL) {
					delete hero;
					hero=NULL;
				}
				try {
					for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++) {
						if (arrpentAlien[iAliensNow]!=NULL) {
							if (iAliensNow==iBoss) iBoss=-1;
							delete arrpentAlien[iAliensNow];
							arrpentAlien[iAliensNow]=NULL;
						}
					}
				}
				catch (exception& exn) {
					ShowExn(exn,"GameMain(){state:INIT}","initializing alien flyers");
				}
				catch (...) {
					ShowUnknownExn("GameMain(){state:INIT}","initializing alien flyers");
				}
				try {
					hero=new Entity(ENTITY_TYPE_HERO);
					hero->iIndex=0;
				}
				catch (exception& exn) {
					ShowExn(exn,"GameMain(){state:INIT}","creating hero");
				}
				catch (...) {
					ShowUnknownExn("GameMain(){state:INIT}","creating hero");
				}
				iGameState=GAMESTATE_START_AREA;
				iArea=1;
				iEncounter=1;
				dat.GetOrCreate(iArea,"start.area");
				dat.GetOrCreate(iEncounter,"start.encounter");
			}//end if GAMESTATE_INIT
			else if (iGameState==GAMESTATE_START_AREA) {//START AREA
				try {
					animBackdrop.GotoFrame(iArea-1);
				}
				catch (exception& exn) {
					ShowExn(exn,"START_AREA state","getting animBackdrop");
					bGood=false;
				}
				catch (...) {
					ShowUnknownExn("START_AREA state","getting animBackdrop");
					bGood=false;
				}
				hero->fHealth=1.0f;
				if (iArea==2) hero->u32Status |= STATUS_ANTIGRAVITY;
				iGameState=GAMESTATE_START_ENCOUNTER;
			}//end GAMESTATE_START_AREA
			else if (iGameState==GAMESTATE_LOSEGUY) {
				SleepWrapper(1000);

				StopMusic();
				iGuys--;
				if (hero==NULL) {
					hero=new Entity(ENTITY_TYPE_HERO);
					hero->iIndex=0;
				}

				animBackdrop.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
				gbLives.DrawToLargerWithoutCropElseCancel(gbScreen,(int) ( FSCREEN_WIDTH/2.0f-(float)gbLives.iWidth/2.0f+FSCREEN_OFFSET_X ), (int)( FSCREEN_HEIGHT/2.0f-(float)gbLives.iHeight+FSCREEN_OFFSET_Y ),DrawModeBlendAlpha);
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
				dat.GetOrCreate(iOffset,"hero.lives.counter.offset.X");
				static int yOffset=SCREEN_HEIGHT/2;
				dat.GetOrCreate(yOffset,"hero.lives.counter.offset.Y");
				animHero.GotoFrame(0);
				for (int iGuy=0; iGuy<iGuys; iGuy++) {
					GBuffer_FX_Scaled(gbScreen, animHero.gbFrame, SCREEN_OFFSET_X+iOffset, SCREEN_OFFSET_Y+yOffset, 255.0f, 0.0f, 0x00000000, fScaleGuy);
					iOffset+=iOffsetter;
				}
				iEncounter=1;//go back to first encounter of this area
				DrawScreen();
				SleepWrapper(4000);
				iGameState=GAMESTATE_START_ENCOUNTER;
			}//end GAMESTATE_LOSEGUY
			else if (iGameState==GAMESTATE_START_ENCOUNTER) {//START ENCOUNTER
				animGameScreenArea.GotoFrame(iArea-1);
				animGameScreenEncounter.GotoFrame(iEncounter-1);
				iMaxAliensNow=iMaxAliens+iArea-1;
				bBombed=false;
				sVerb_Engine="stopping music";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				StopMusic();
				sVerb_Engine="clearing sounds";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				ClearSounds();
				//iPlayExplosion=128;//TODO: change this to different sound effect (start encounter sound)
				sVerb_Engine="playing sounds";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				PlaySounds();
				sVerb_Engine="rendering backdrop";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				//TODO:? //if (bFirstRunOfCurrentState) bShowOnce_TargaToTarga32_ANY_Debug=true;
				animBackdrop.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
				sVerb_Engine="rendering area "+RString_ToString(iArea-1)+" gamescreen";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				animGameScreenArea.gbFrame.DrawToLargerWithoutCropElseCancel( gbScreen,
						(int)( FSCREEN_WIDTH/2.0f-animGameScreenArea.Width()/2.0f+FSCREEN_OFFSET_X ),
						(int)( FSCREEN_HEIGHT/2.0f-animGameScreenArea.Height()+FSCREEN_OFFSET_Y ),DrawModeBlendAlpha );
				sVerb_Engine="rendering encounter "+RString_ToString(iEncounter-1)+" gamescreen";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				animGameScreenEncounter.gbFrame.DrawToLargerWithoutCropElseCancel( gbScreen,
					(int)( FSCREEN_WIDTH/2.0f-animGameScreenEncounter.Width()/2.0f+FSCREEN_OFFSET_X ),
					(int)( FSCREEN_HEIGHT/2.0f+FSCREEN_OFFSET_Y ), DrawModeBlendAlpha );
				sVerb_Engine="drawing backbuffer";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				DrawScreen();
				sVerb_Engine="pausing to show gamescreen";
				SleepWrapper(3000);
				sVerb_Engine="checking for music enabled setting";
				if (bFirstRunOfCurrentState) {
					Console::Error.Write("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
					Console::Error.Flush();
				}
				if (settings.GetForcedBool("music")) {
					sVerb_Engine="playing music";
					if (bFirstRunOfCurrentState) {
						Console::Error.WriteLine(sVerb_Engine+"...");
						Console::Error.Flush();
					}
					PlayMusic("music/Orangejuice-DXMan-Invasion.ogg",true);
					if (bFirstRunOfCurrentState) Console::Error.WriteLine("finished setting music.");
				}
				else {
					sVerb_Engine="skipping music";
					if (bFirstRunOfCurrentState) Console::Error.WriteLine(sVerb_Engine+".");
				}
				sVerb_Engine="creating "+RString_ToString(iMaxAliensNow)+" aliens";
				if (bFirstRunOfCurrentState) {
					Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine);
					Console::Error.Flush();
				}
				for (int index=0; index<iMaxAliensNow; index++) {
					//arrpentAlien[index]=new Alien(3.0f, (float)index-2.0f, 3.9f);
					arrpentAlien[index]=new Entity(ENTITY_TYPE_ALIEN, 3.0f, (float)index-1.0f, 3.9f);
					(arrpentAlien[index])->iIndex=index;
					if (bFirstRunOfCurrentState) Console::Error.WriteLine(".");
					//arrpentAlien[index]->m3dEnt.X=SCREEN_WIDTH;
					//arrpentAlien[index]->m3dEnt.Y += index*(SCREEN_HEIGHT/64);
					//arrpentAlien[index]->m3dEnt.Z=index+1;
					//if (iArea==2) arrpentAlien[index]->u32Status |= STATUS_SHOOTER;
					//else if (iArea==3) arrpentAlien[index]->u32Status |= STATUS_BOMBER;
					//if (iArea==3 && iEncounter==3) arrpentAlien[index]->u32Status |= STATUS_SHIELD;
				}
				sVerb_Engine="creating hero";
				if (bFirstRunOfCurrentState) {
					Console::Error.WriteLine(sVerb_Engine+"...");
					Console::Error.Flush();
				}
				//if (iEncounter==1&&iArea==1&&bFirstRunGameMain) {
				//	iTicksDisplayHelpMessage1=3000;
				//	iTickLastDisplayHelpMessage1=-1;
				//}
				if (iArea==3) {
					hero->AddVariableShield(1.0f);
					if (iEncounter!=3) (arrpentAlien[0])->u32Status|=STATUS_AIMBOMBS;
				}
				if (iEncounter==3 && iArea==3) {
					sVerb_Engine="creating boss";
					if (bFirstRunOfCurrentState) {
						Console::Error.WriteLine(sVerb_Engine+"...");
						Console::Error.Flush();
					}
					iBoss=( (iMaxAliensNow>=3)?2:(iMaxAliensNow-1) );
					(arrpentAlien[iBoss])->SetAsBoss();//Set a boss if last encounter of last area
				}
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("start encounter tasks finished.");
				Console::Error.WriteLine("About to do GAMESTATE_RUN at area "+RString_ToString(iArea)+" encounter "+RString_ToString(iEncounter));
				iGameState=GAMESTATE_RUN;
			}//end GAMESTATE_START_ENCOUNTER

			else if (iGameState==GAMESTATE_RUN) {
				//if KEYDOWN(65/* a */) hero->u32Status |= STATUS_ANTIGRAVITY;
				//if KEYUP(65) hero->u32Status ^= STATUS_ANTIGRAVITY;
				sVerb_Engine="reading hero controller";
				if (bFirstRunOfCurrentState)  Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("GameMain {bFirstRunOfCurrentState:true}: keys...");
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



				sVerb_Engine="processing powerups";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				if (iDoubleCodeCounter>=CODE_THRESHOLD && hero->u32Status ^ STATUS_DOUBLESPEED) hero->SetDoubleSpeed();
				if (iRapidFireCodeCounter>=CODE_THRESHOLD && hero->u32Status ^ STATUS_RAPIDFIRE) hero->SetRapidFire();

				sVerb_Engine="rendering background";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				animBackdrop.gbFrame.DrawToLargerWithoutCropElseCancel(gbScreen, SCREEN_OFFSET_X, SCREEN_OFFSET_Y, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
				//DrawRadarField();

				//Update all the existing arrpentAlien, delete the rest
				if (bFirstRunOfCurrentState) {
					Console::Error.Write("GameMain {bFirstRunOfCurrentState:true}: processing hero...");
					Console::Error.Flush();
				}
				if (hero!=NULL) {
					sVerb_Engine="checking for enemy meter variables";
					if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
					if (iBoss>=0) {
						if (bFirstRunOfCurrentState) {
							Console::Error.Write("processing boss health before hero actions...");
							Console::Error.Flush();
						}
						try {
							fBoss=(arrpentAlien[iBoss])->fHealth;
							fBossMax=(arrpentAlien[iBoss])->fHealthMax;
						}
						catch (exception& exn) {
							ShowExn(exn,"GameInit(){state:RUN}","getting boss vars");
						}
						catch (...) {
							ShowUnknownExn("GameInit(){state:RUN}","getting boss vars");
						}
					}
					sVerb_Engine="refreshing hero status";
					if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
					hero->Refresh();
					sVerb_Engine="adding hero to render buffer";
					if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
					AddScreenItem(SCREENITEM_HERO,ZORDER_FROMY(hero->m3dEnt.Y),0);
					//DrawRadarDot(hero->m3dEnt.X, hero->m3dEnt.Y, hero->m3dEnt.Z+1, 0xFFFFFFFF);
					if (bFirstRunOfCurrentState) {
						Console::Error.Write("get hero location...");
						Console::Error.Flush();
					}
					float
						fHalfH=hero->m3dEnt.ySize/2.0f,
						fHalfW=hero->m3dEnt.xSize/2.0f;
					sVerb_Engine="drawing radar";
					if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
					DrawRadarRect(hero->m3dEnt.X-fHalfW,
								hero->m3dEnt.Y+fHalfH,
								hero->m3dEnt.Y-fHalfH,
								hero->m3dEnt.X+fHalfW,0xFFFFFFFF, true);//electric_blue:0xFF8800FF
					Pixel pixelNow;
					sVerb_Engine="drawing radar arc";
					if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
					pixelNow.Set(192,192,192,64);
					gbScreen.DrawSubpixelArc(RadarCenterX()+FSCREEN_OFFSET_X ,RadarCenterY()+FSCREEN_OFFSET_Y,
									RadarHeight()/2.0f-2.0f,1,0,
									0,360,
							pixelNow,
							.2,0);
					//DrawExclusiveRect(hero->m2dEnt.rectRender.left, hero->m2dEnt.rectRender.top, hero->m2dEnt.rectRender.bottom, hero->m2dEnt.rectRender.right, 0xFFFFFFFF, false);
					if (bFirstRunOfCurrentState) {
						Console::Error.WriteLine("done processing hero.");
					}
				}
				int iNulls;
				iNulls=0;
				Entity* lpAlienNow=NULL;
				sVerb_Engine="refreshing alien status";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				for (int iAlien=0; iAlien<iMaxAliensNow; iAlien++) {
					if (arrpentAlien[iAlien] != NULL) {
						lpAlienNow=arrpentAlien[iAlien];
						lpAlienNow->Refresh();
						if (lpAlienNow->bAimBomb && hero)
							lpAlienNow->Bomb(hero->m3dEnt.X,hero->m3dEnt.Y,hero->m3dEnt.Z);
							//TODO: fix bomb function to use metric speed(?)
						lpAlienNow->bAimBomb=false;

						AddScreenItem(SCREENITEM_ALIEN,ZORDER_FROMY(lpAlienNow->m3dEnt.Y),iAlien);
						////hero->DrawTarget(iAlien);
						if (lpAlienNow->u32Status & STATUS_BOSS) {
							//DrawRadarDot(lpAlienNow->m3dEnt.X, lpAlienNow->m3dEnt.Y, lpAlienNow->m3dEnt.Z,
							//0x00000000);
							//prototype: void DrawRadarRect(float left, float top, float bottom, float right, Uint32 u32Pixel)
							float fHalfH, fHalfW;
							fHalfH=lpAlienNow->m3dEnt.ySize/2.0f;
							fHalfW=lpAlienNow->m3dEnt.xSize/2.0f;
							DrawRadarRect(lpAlienNow->m3dEnt.X-fHalfW,
										lpAlienNow->m3dEnt.Y+fHalfH,
										lpAlienNow->m3dEnt.Y-fHalfH,
										lpAlienNow->m3dEnt.X+fHalfW,0xFF8800FF, false);//electric_blue:0xFF8800FF
						}
						else { //DrawRadarDot(lpAlienNow->m3dEnt.X, lpAlienNow->m3dEnt.Y, lpAlienNow->m3dEnt.Z,
						//0xFFFF0000);
						//((lpAlienNow->u32Status&STATUS_BOSS)?0x00000000:0xFFFF0000) );
							float fHalfH, fHalfW;
							fHalfH=lpAlienNow->m3dEnt.ySize/2.0f;
							fHalfW=lpAlienNow->m3dEnt.xSize/2.0f;
							DrawRadarRect(lpAlienNow->m3dEnt.X-fHalfW,
										lpAlienNow->m3dEnt.Y+fHalfH,
										lpAlienNow->m3dEnt.Y-fHalfH,
										lpAlienNow->m3dEnt.X+fHalfW,0xFFFF0000, true);//electric_blue:0xFF8800FF
						}

						if (lpAlienNow->fExplodedness>=1.0f) {
							if (lpAlienNow->u32Status & STATUS_BOSS) {
								for (int iAlienToDeShield=0; iAlienToDeShield < iMaxAliensNow; iAlienToDeShield++) {
									if (arrpentAlien[iAlienToDeShield]) {
										if ((arrpentAlien[iAlienToDeShield])->u32Status & STATUS_SHIELD) {
											(arrpentAlien[iAlienToDeShield])->u32Status ^= STATUS_SHIELD;
											(arrpentAlien[iAlienToDeShield])->fHealth=0;
										}
									}
								}
							}
							iAliens--;
							delete arrpentAlien[iAlien];
							if (iAlien==iBoss) iBoss=-1;
							arrpentAlien[iAlien]=NULL;
						}
					}
					else iNulls++;
				}
				sVerb_Engine="checking for hero fatality";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				if (hero->fExplodedness>=1.0f) {
					delete hero;
					hero=NULL;
					if (iGuys<=0) iGameState=GAMESTATE_YOU_LOSE;
					else iGameState=GAMESTATE_LOSEGUY;
				}
				sVerb_Engine="refreshing shot status";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (arrpentShot[iShotNow] != NULL) {
						(arrpentShot[iShotNow])->Refresh();
						(arrpentShot[iShotNow])->Draw();
						AddScreenItem(SCREENITEM_SHOT,ZORDER_FROMY((arrpentShot[iShotNow])->m3dEnt.Y),iShotNow);
						DrawRadarDot( (arrpentShot[iShotNow])->m3dEnt.X, (arrpentShot[iShotNow])->m3dEnt.Y, 1,
							(((arrpentShot[iShotNow])->bAlien)?0xFF800000:0xFFFFFFFF),false);//A,R,G,B //electric_blue:0xFF8800FF
						if (((arrpentShot[iShotNow])->u32Status&STATUS_ALIVE)==0) {
							delete arrpentShot[iShotNow];
							arrpentShot[iShotNow]=NULL;
						}
					}
				}
				if (bDebug) {
					sVerb_Engine="drawing movement boundary cube for debug";
					if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
					Pixel pixelNear;
					Pixel pixelFar;
					pixelNear.Set(0,0,255,150);
					pixelFar.Set(0,0,128,100);
					for (int iNow=0; iNow<9; iNow++) {
						DrawCube(m3dDebug[iNow], pixelNear, pixelFar);
					}
				}
				sVerb_Engine="refreshing 3d cursor status";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				Refresh3dCursor();
				int zNow=0;
				int iScreenItemsNow=iScreenItems;
				sVerb_Engine=RString_ToString("drawing (")+RString_ToString(iScreenItems)+RString_ToString(") render buffer items");
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				//Draw ScreenItems:
				while (iScreenItemsNow>0 && zNow<=MAX_ZORDER) {
					for (int iItem=0; iItem<iScreenItems; iItem++) {
						if (screenitemarr[iItem].zOrder==zNow) {
							switch (screenitemarr[iItem].iType) {
								case SCREENITEM_ALIEN:
									if (NULL!=arrpentAlien[ screenitemarr[iItem].index ]) {
										(arrpentAlien[ screenitemarr[iItem].index ])->Draw();
										if (bDebug) DrawAxis((arrpentAlien[ screenitemarr[iItem].index ])->m3dEnt);
									}
									iScreenItemsNow--;
									break;
								case SCREENITEM_SHOT:
									if (NULL!=arrpentShot[ screenitemarr[iItem].index ]) (arrpentShot[ screenitemarr[iItem].index ])->Draw();
									iScreenItemsNow--;
									break;
								case SCREENITEM_HERO:
									if (NULL!=hero) {
										hero->Draw();
										if (bDebug) DrawAxis(hero->m3dEnt);
									}
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
				if (bDebug) {
					Mass3d m3dAxisCenter;
					DrawAxis(m3dAxisCenter);//draw the origin
				}
				iScreenItems=0;
				/*
				if(calculateExplosion(gbScreen, 100, 100, 20, 0, EXPLOSION_CHECK_COUNT)==0)
				{
					explosionResult=calculateExplosion(gbScreen, 100, 100, 20, 30, EXPLOSION_START);
				}
				else
					explosionResult=calculateExplosion(gbScreen, 100, 100, 20, 30, EXPLOSION_CONTINUE);
				//Console::Error.WriteLine( "\nResult of explosionCalculate=" + explosionResult;
				*/
/*
				if (iTicksDisplayHelpMessage1>0) {
					if (iTickLastDisplayHelpMessage1<0) {
						iTickLastDisplayHelpMessage1=Base_GetTicks_Relative();
					}
					else {
						iTicksDisplayHelpMessage1-=Base_GetTicks_Relative()-iTickLastDisplayHelpMessage1;
						iTickLastDisplayHelpMessage1=Base_GetTicks_Relative();
					}
					if (iTicksDisplayHelpMessage1>0) { //if STILL >0 after decrementing
						static string sMsg1="Shoot those aliens!";
						//int iWidthTheoretical=gfontDefault.MeasureString(sMsg1);
						static SizeF sizeText;
						static IPoint ipDest;
						static bool DisplayHelpMessage1_bFirstRun=true;
						static bool DisplayHelpMessage1_bMeasuredActual=false;
						static int iYStart=0;
						if (DisplayHelpMessage1_bFirstRun||!DisplayHelpMessage1_bMeasuredActual) {
							if (DisplayHelpMessage1_bFirstRun) {
								ipDest.X=gbScreen.iWidth/2;//this assumes the same amount of wrapping is needed for starting at center, but is fixed after first run
								ipDest.Y=0;
								gfontDefault.MeasureTextByRef_UsingTypeFast(sizeText, gbScreen, ipDest, sMsg1, GFont_GlyphTypePlain);
							}
							else DisplayHelpMessage1_bMeasuredActual=true; //was wrapped starting at final X location (below) instead of only test location (above)
							ipDest.X=gbScreen.iWidth/2-(sizeText.Width/2);
							ipDest.Y=gbScreen.iHeight/2;
							iYStart=ipDest.Y;
							DisplayHelpMessage1_bFirstRun=false;
						}
						static int iSlideMS=750;
						if (iTicksDisplayHelpMessage1<iSlideMS) {
							ipDest.Y=iYStart-(int) ( (double)(iSlideMS-iTicksDisplayHelpMessage1)/(double)iSlideMS * ((double)gbScreen.iHeight/2.0) );
						}
						gfontDefault.TypeFast(sizeText,gbScreen, ipDest, sMsg1,GFont_GlyphTypePlain,DrawModeBlendAlpha,DisplayHelpMessage1_bFirstRun);
					}
				}
*/
				sVerb_Engine="drawing backbuffer";
				// flip pages:
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				DrawScreen();
				//ShowDebugVars();
				if (iAliens<=0 || iNulls==iMaxAliensNow) iGameState=GAMESTATE_WIN_ENCOUNTER;
				sVerb_Engine="finishing run";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+".");
			}//end GAMESTATE_RUN
			else if (iGameState==GAMESTATE_SHUTDOWN) {
			}//end GAMESTATE_SHUTDOWN
			else if (iGameState==GAMESTATE_WIN_ENCOUNTER) {
				StopMusic();

				if (settings.GetForcedBool("music")) {
					PlayMusic("sound/trumpet.wav", false);
				}


				for (int iX=0; iX<MAXSHOTS; iX++) {
					if (arrpentShot[iX]!=NULL)
					{
						delete arrpentShot[iX];
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
			}//end GAMESTATE_WIN_ENCOUNTER
			else if (iGameState==GAMESTATE_WIN_GAME) {
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
			else if (iGameState==GAMESTATE_YOU_LOSE) {
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
			}//end GAMESTATE_YOU_LOSE
			else if (iGameState==GAMESTATE_EXIT) {
				//TODO: finish this why isn't quit working
				try {
					if (!bSDLQuitDone) {
						SDL_Quit(); //TODO: finish this why isn't quit working
						bSDLQuitDone=true;
					}
					Console::Error.WriteLine("SDL_Quit...done");
				}
				catch (exception& exn) {
					ShowExn(exn,"GameMain(){state:EXIT}","trying to run SDL_Quit");
				}
				catch (...) {
					ShowUnknownExn("GameMain(){state:EXIT}","trying to run SDL_Quit");
				}
				if (!bDone) {
					Console::Error.WriteLine("GameMain(){state:EXIT}...done");
				}
			}//end if GAMESTATE_EXIT
		}
		catch (exception& exn) {
			ShowExn(exn,"GameMain");
		}
		catch (...) {
			ShowUnknownExn("GameMain");
		}
		narrGameStateCount[iGameStateWas]++;
		if (bMegaDebug) {
			Console::Error.Write("Ended "+sarrGameState[iGameStateWas]+"--ready to go to "+sarrGameState[iGameState]);
			Console::Error.Flush();
			if (iGameState==iGameStateWas) {
				Console::Error.Write(" again.");
				Console::Error.Flush();
			}
			else {
				Console::Error.Write(".");
				Console::Error.Flush();
			}
		}
		if (bFirstRunGameMain) Console::Error.WriteLine("GameMain FirstRun: ended "+RString_ToString(bGood?"ok.":"with error!"));
		bFirstRunGameMain=false;
		// return success or failure or your own return code here
		sVerb_Engine="finishing "+sarrGameState[iGameStateWas];
		return(bGood);
	} //end GameMain

	////////////////////////////////////////////////////////////
	bool GameInit() {
		bool bGood=true;
		Console::Error.Write("Loading debug points...");
		Console::Error.Flush();
		try {
			for (int iNow=0; iNow<9; iNow++) {
				m3dDebug[iNow].X=0;
				m3dDebug[iNow].Y=0;
				m3dDebug[iNow].Z=0;
				m3dDebug[iNow].xSize=1.0f;
				m3dDebug[iNow].ySize=1.0f;
				m3dDebug[iNow].zSize=1.0f;
			}
			static float fTestRad=4.0f;
			static float fTestDiam=fTestRad*2.0f;
			m3dDebug[0].X-=fTestRad; //see image 1.box-indeces.png
			m3dDebug[0].Y+=fTestRad; //see image 1.box-indeces.png
			m3dDebug[0].Z+=fTestDiam;
			m3dDebug[1].X+=fTestRad;
			m3dDebug[1].Y+=fTestRad;
			m3dDebug[1].Z+=fTestDiam;
			m3dDebug[2].X+=fTestRad;
			m3dDebug[2].Y-=fTestRad;
			m3dDebug[2].Z+=fTestDiam;
			m3dDebug[3].X-=fTestRad;
			m3dDebug[3].Y-=fTestRad;
			m3dDebug[3].Z+=fTestDiam;
			m3dDebug[4].X-=fTestRad; //see image 1.box-indeces.png
			m3dDebug[4].Y+=fTestRad; //see image 1.box-indeces.png
			m3dDebug[5].X+=fTestRad;
			m3dDebug[5].Y+=fTestRad;
			m3dDebug[6].X+=fTestRad;
			m3dDebug[6].Y-=fTestRad;
			m3dDebug[7].X-=fTestRad; //see image 1.box-indeces.png
			m3dDebug[7].Y-=fTestRad; //see image 1.box-indeces.png
			Console::Error.WriteLine("done.");

			//settings.bDebug=true;
			//dat.bDebug=true;
			Console::Error.Write("Loading settings...");
			Console::Error.Flush();
			settings.Load("settings.txt");
			Console::Error.WriteLine("Done (settings.txt)");
			Console::Error.Write("Loading game data...");
			Console::Error.Flush();
			dat.Load("dat");
			Console::Error.WriteLine("done.");
			Console::Error.Write("Getting vars...");
			Console::Error.Flush();
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
			Console::Error.WriteLine("debugmode:"+RString_ToString(bDebug));
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
			dat.GetOrCreate(iGuysStart,"hero.lives.start");
			iGuys=iGuysStart;

			settings.Save();
			dat.Save();
			Console::Error.WriteLine("done.");


			Console::Error.Write("Setting up arrays...");
			Console::Error.Flush();
			arrpentAlien=(Entity**)malloc(sizeof(Entity*)*iMaxAliens);
			string sMsg;
			Console::Error.WriteLine("Done (iMaxAliens:"+RString_ToString(iMaxAliens)+")");
							/// Initialize SDL ///
			Console::Error.Write("Initializing video...");
			Console::Error.Flush();
			if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
				sMsg="Couldn't initialize SDL:\n";
				sMsg+=RString_ToString(SDL_GetError());
				Console::Error.WriteLine("In GameInit -- "+sMsg);
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //re-implement this w/o windows api
				bGood=false;
				return bGood;
			}
			Console::Error.WriteLine("done.");
			atexit(QuitSafe);
			//Console::Error.Write("Initializing audio...");
			//Console::Error.Flush();
			//if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
			//	sMsg="Couldn't open Audio: \n";
			//	sMsg+=RString_ToString(SDL_GetError());
			//	Console::Error.WriteLine("In GameInit -- "+sMsg);
			//	iErrors++;
			//	//MessageBox (0, sMsg, "Error", MB_ICONHAND); //re-implement this w/o windows api
			//	return false;
			//}
			//Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);//what we got
			Console::Error.WriteLine("done.");

			/* Set 640x480 32-bits video mode */
			Console::Error.Write("Setting video mode...");
			Console::Error.Flush();
			screen=SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF);
			if (screen == NULL) {
				sMsg="Couldn't set 640x480x32 video mode: \n";
				sMsg+=RString_ToString(SDL_GetError());
				Console::Error.WriteLine("In GameInit -- "+sMsg);
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
				bGood=false;
				return bGood;
			}
			SDL_WM_SetCaption ("DXMan", NULL);
			Console::Error.WriteLine("done.");
			Console::Error.Write("Loading splash screen...");
			Console::Error.Flush();
			GBuffer gbSplash;
			if (!gbSplash.Load("images/intro-splash.tga")) {
				sMsg="Failed to load splash screen";
				Console::Error.WriteLine("In GameInit -- "+sMsg);
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
			}
			bool bSplashDrawn=false;
			Console::Error.WriteLine("done.");
			iSplashTick=Base_GetTicks_Absolute();//use non-sdl for safety since SDL isn't necessarily initialized
			Console::Error.Write("Creating screen buffer...");
			Console::Error.Flush();
			gbScreen.InitNull();
			//if (!
			gbScreen.Init(SCREEN_WIDTH*2, SCREEN_HEIGHT*2,4,true);
			//) {
			//	sMsg="Failed to create screen sprite buffer";
			//	Console::Error.WriteLine("In GameInit -- "+sMsg);
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
			Console::Error.WriteLine("done.");
			Console::Error.Write("Drawing screen...");
			Console::Error.Flush();
			DrawScreen();
			if (bSplashDrawn) bSplash=true;
			Console::Error.WriteLine( RString_ToString("done (") + ((bSplash)?"with":"without") + RString_ToString(" splash).") );
			//DONE LOADING SPLASH SCREEN
			float fGrad;
			float r1,g1,b1,r2,g2,b2;
			r2=232;	g2=49;	b2=0;
			r1=126;	g1=97;	b1=200;
			for (int iGrad=0; iGrad<256; iGrad++) {
				fGrad=(float)iGrad/255.0f;
				//TODO: make the next line use a byte array instead of the macro.
				arru32HealthGrad[iGrad]=_RGB32BIT(255,(byte)APPROACH(r2,r1,fGrad),(byte)APPROACH(g2,g1,fGrad),(byte)APPROACH(b2,b1,fGrad)); //A,R,G,B,
			}
			r1=64;	g1=64;	b1=64;
			r2=126; g2=97;	b2=200;
			for (int iGrad=0; iGrad<256; iGrad++) {
				fGrad=(float)iGrad/255.0f;
				//TODO: make the next line use a byte array instead of the macro.
				arru32EnergyGrad[iGrad]=_RGB32BIT(255,(byte)APPROACH(r2,r1,fGrad),(byte)APPROACH(g2,g1,fGrad),(byte)APPROACH(b2,b1,fGrad)); //A,R,G,B,
			}
			Console::Error.Write("Loading sounds...");
			Console::Error.Flush();


			InitSound();

			//Mix_HookMusicFinished(MusicDone);
			//Console::Error.WriteLine("Starting GameInit");

			/*
			*/

			//Load alpha lookup table from file
			//int file_handle;
			Console::Error.WriteLine("done.");

			Console::Error.Write("Loading files in data folder...");
			Console::Error.Flush();
			ifstream ifData("data/lookup-alpha.raw", ios::out | ios::in | ios::binary);//OFSTRUCT file_data; //the file data information
			bool bLookupFile=ifData.is_open();
			if (!bLookupFile) {// ((file_handle=OpenFile("by3dAlphaLookup.raw",&file_data,OF_READ))==-1) //open the file if it exists
			//if can't open file:
				Console::Error.Write( "\nERROR: Unable to open alpha lookup file!");
				Console::Error.Flush();
				iErrors++;
			}

			ifData.read((char*)by3dAlphaLookup, 256*256*256); //_lread(file_handle, by3dAlphaLookup,256*256*256);
			ifData.close();//_lclose(file_handle);

			if (!bLookupFile) { //TODO: put this in base.cpp
				//Generate alpha lookup table if needed
				Console::Error.Write( "\nNeed to generate alpha lookup table...");
				Console::Error.Flush();
				int iSource=0;
				int iDest;
				int iAlpha;//can't use bytes because the loop will never end (never >255!)
				for (float source=0.0f; source<=255.0f; source+=1.0f, iSource++) {
					iDest=0;
					for (float dest=0.0f; dest<=255.0f; dest+=1.0f, iDest++) {
						iAlpha=0;
						for (float alpha=0.0f; alpha<=255.0f; alpha+=1.0f, iAlpha++) {
							by3dAlphaLookup[iSource][iDest][iAlpha]=(byte)((source-dest)*alpha/255.0f+dest);
						}
					}
				}
				//TODO: ofstream ofData("data/lookup-alpha.raw");
				Console::Error.WriteLine("Done.");
				//TODO: save it?
			}
			Console::Error.WriteLine( RString_ToString("done (") + RString_ToString((bLookupFile)?"from":"generated") + " lookup file)." );
			Console::Error.Write("Loading font...");
			string sFontFile_RelName="images/font.tga";
			bool bTest=false;
			Targa targaTest;
			bTest=targaTest.Load(sFontFile_RelName);
			Rectangle rectFontMap;
			targaTest.ToRect(rectFontMap);
			int iCharW=rectFontMap.Width/16;
			int iCharH=rectFontMap.Height/16;
			bTest=gfontDefault.FromImageAsPredefinedColorFont(sFontFile_RelName, iCharW, iCharH, 16, 16);//.FromImageValue("images/font.tga", 16, 24, 16, 16);
			Console::Error.Write(bTest?"gfontDefault.FromImageAsPredefinedColorFont...OK":"gfontDefault.FromImageAsPredefinedColorFont...FAILED");
			Console::Error.Flush();
			Console::Error.Write("Loading image files...");
			Console::Error.Flush();
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
			//IRECT screenRect= {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
			REAL_BPP=32;
			REAL_BYTEDEPTH=4;

			LoadImage(gbIntro, "images/intro-title.tga");
			LoadImage(gbIntroTextBack, "images/intro-text-back.tga");
			LoadImage(gbIntroText, "images/intro-text.tga");
			LoadImage(gbSymbolShield, "images/symbol-shield.tga");
			LoadImage(gbSymbolBossHealth, "images/symbol-bosshealth.tga");
			LoadImage(gbLives, "images/lives.tga");
			LoadSequence(animBackdrop, "images/area", iBackdrops);
			LoadSequence(animHero, "images/hero", iFramesHero);
			LoadSequence(animHeroShadow, "images/hero-shad", iFramesHeroShadow);
			LoadSequence(animFlyer, "images/flyer", iFramesFlyer);
			LoadSequence(animFlyerShadow, "images/flyer-shad", iFramesFlyerShadow);
			LoadSequence(animBoss, "images/boss", iFramesBoss);
			LoadSequence(animBossShadow, "images/boss-shad", iFramesBossShadow);
			LoadSequence(animShot, "images/shot", iFramesShot);
			LoadSequence(animShotShadow, "images/shot-shad", iFramesShotShadow);
			LoadSequence(animBurn, "images/burn", iFramesShotShadow);
			LoadSequence(animExpArt, "images/expart", iFramesShotShadow);
			LoadSequence(animGameScreen, "images/gamescreen", iFramesGameScreen);
			LoadSequence(animGameScreenArea, "images/gamescreenarea", iAreas);
			LoadSequence(animGameScreenEncounter, "images/gamescreenencounter", iEncounters);
			LoadSequence(animCursor, "images/cursor", iFramesCursor);

			settings.bSaveEveryChange=true;
			dat.bSaveEveryChange=true;
			Console::Error.WriteLine("done (end of GameInit).");
		}//end try
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"GameInit");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("GameInit");
		}
		return(bGood);
	} // end GameInit

	/////////////////////////////////////////////////////////////

	bool GameShutdown() {
		bool bGood=false;
		if (!bShuttingDown) {
			bGood=true;
			bShuttingDown=true;
			//This is called after the game is exited and the main event
			// loop while is exited, do all you cleanup and shutdown here
			sVerb_Engine="shutting down game";
			if (bMegaDebug) Console::Error.WriteLine(sVerb_Engine+"...");
			//Release palette if 8-bit was used
			//if (lpddpal){lpddpal->Release(); lpddpal=NULL;} //
			if (!bDoneShutdown) {
				try {
					if (SDL_ShowCursor(SDL_QUERY)==SDL_DISABLE) {
						SDL_ShowCursor(SDL_ENABLE);
					}
					ShutdownMusic();
					ShutdownAudio();
					Console::Error.Write("Freeing alien array...");
					Console::Error.Flush();
					if (bDebug) Console::Error.WriteLine();
					if (arrpentAlien!=NULL) {
						try {
							for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++) {
								if (arrpentAlien[iAliensNow]!=NULL) {
									if (iAliensNow==iBoss) iBoss=-1;
									if (bDebug) {
										Console::Error.WriteLine("arrpentAlien["+RString_ToString(iAliensNow)+"].m3dEnt={");
										///float X,Y,Z;
										Console::Error.WriteLine("\tLocation:"+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.X)+","+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.Y)+","+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.Z)+";");
										//float xMin,yMin,zMin;
										//float xMax,yMax,zMax;
										///float xVel,yVel,zVel;
										Console::Error.WriteLine("\tVelocity:"+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.xVel)+","+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.yVel)+","+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.zVel)+";");
										///float xRot,yRot,zRot;
										Console::Error.WriteLine("\tRotation:"+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.xRot)+","+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.yRot)+","+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.zRot)+";");
										//float xRotMin,yRotMin,zRotMin;
										//float xRotMax,yRotMax,zRotMax;
										//float xRotVelDegreesPerSec,yRotVelDegreesPerSec,zRotVelDegreesPerSec;
										///float xRotDest,yRotDest,zRotDest;
										Console::Error.WriteLine("\tRotDest:"+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.xRotDest)+","+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.yRotDest)+","+RString_ToString(arrpentAlien[iAliensNow]->m3dEnt.zRotDest)+";");
										//float xSize,ySize,zSize;
										Console::Error.WriteLine("}");
									}
									delete arrpentAlien[iAliensNow];
									arrpentAlien[iAliensNow]=NULL;
								}
								else {
									if (bDebug) {
										Console::Error.WriteLine("arrpentAlien["+RString_ToString(iAliensNow)+"]=null");
									}
								}
							}
							free(arrpentAlien);
						}
						catch (exception& exn) {
							bGood=false;
							ShowExn(exn,"GameShutdown","terminating alien flyers");
						}
						catch (...) {
							bGood=false;
							ShowUnknownExn("GameShutdown","terminating alien flyers");
						}
						//SafeFree(arrpentAlien);
						Console::Error.Write("done Freeing alien array");
					}
					else Console::Error.Write("NOTHING TO DO for freeing alien array.");
					Console::Error.Flush();
				}
				catch (exception& exn) {
					bGood=false;
					ShowExn(exn,"GameShutdown");
				}
				catch (...) {
					bGood=false;
					ShowUnknownExn("GameShutdown");
				}
				bDoneShutdown=true;
			}//end if !bDoneShutDown
			else Console::Error.WriteLine("Already done shutdown!");
		}//end if !bShuttingDown
		return(bGood);
	} // end GameShutdown
	/*inline*/ void SleepWrapper(int iTicks) {
		SDL_Delay(iTicks);
	}
	void LoadSequence(Anim &animToLoad, string sFileBaseNameOnly, int iFramesExpectedElseError) {
		int iFramesFound;
		bool bGood=animToLoad.LoadSeq(sFileBaseNameOnly+"*.tga");
		if (!bGood)	ShowError(sFileBaseNameOnly+"*.tga failed to load","GameInit");
		iFramesFound=animToLoad.IFrames();
		if (bGood && iFramesFound!=iFramesExpectedElseError) {
			bGood=false;
			ShowError("not all "+sFileBaseNameOnly+RString_ToString("*.tga frames loaded."));
		}
	}
	void LoadImage(GBuffer &gbToLoad, string sFile) {
		bool bGood=gbToLoad.Load(sFile);
		if (!bGood)	ShowError(sFile+".tga failed to load","GameInit");
	}
	int PickChan() {
		static int iPickChan=-1;
		iPickChan++;
		if (iPickChan>=8) iPickChan=0;
		return(iPickChan);
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
			ShowExn(exn,"InitSound");
		}
		//note: must increase SOUND_BUFFERS if adding more samples!
		//Mix_VolumeChunk(mcThruster,40);
		//Mix_VolumeChunk(mcLaser,100);
		bClosedAudio=false;
	}//end InitSound
	bool ShutdownAudio() {
		bool bGood=false;
		try {//end of main event loop
			sVerb_Engine="unloading sounds";
			if (bMegaDebug) Console::Error.WriteLine(sVerb_Engine+"...");
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
			//	Console::Error.WriteLine("Closing Audio...done.");
				bClosedAudio=true;
			}
			else Console::Error.WriteLine("Closing Audio...was finished");
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"ShutdownAudio");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("ShutdownAudio");
		}
		return bGood;
	}//end ShutdownAudio
	///*inline*/ void SetPan(int iChan, int iLocation) {
	//	 float fLoc=(float)iLocation/(float)SCREEN_WIDTH;
	//	 if (fLoc<0) fLoc=0;
	//	 else if (fLoc>1) fLoc=1;
	//	 if (bMegaDebug) Console::Error.WriteLine("SetPan..."<<flush;
	//	 Mix_SetPanning(iChan, L_FROM_FACTOR(fLoc), R_FROM_FACTOR(fLoc));
	//	 if (bMegaDebug) Console::Error.WriteLine("done..."<<flush;
	//}
	/*inline*/ void UpdateThrustPan() {
		sounder.SetOrCreateInstance("h.thruster","thruster",hero->m3dEnt);
		 //SetPan(iChanThruster,SOUNDPAN_FROM3DX(hero->m3dEnt.X));
		 //fThruster=(float)hero->m3dEnt.X/(float)SCREEN_WIDTH;
		 //if (fThruster<0) fThruster=0;
		 //else if (fThruster>1) fThruster=1;
		 //Mix_SetPanning(iChanThruster, L_FROM_FACTOR(fThruster), R_FROM_FACTOR(fThruster));
		if (bMegaDebug) {
			Console::Error.WriteLine("done...");
			Console::Error.Flush();
		}
	}
	void DirKeyDown() {
		 if ((hero!=NULL&&hero->m3dEnt.Z<=hero->m3dEnt.zMin) && (hero->u32Status & STATUS_ANTIGRAVITY)) {
			if (iDirKeysDown<=0) {
				if (bMegaDebug) {
					Console::Error.Write("DirKeyDown{iDirKeysDown<=0; action:thruster} Mix_PlayChannel...");
					Console::Error.Flush();
				}
				sounder.StopInstance("h.thruster");
				//iChanThruster=Mix_PlayChannel(-1, mcThruster, -1);//chan, sound, #loops
				//iReturn=Mix_FadeInChannelTimed(iChan, mcThruster,
				//			iLoops, iMillisecondsFadeIn, rSecondsDuration)
				if (bMegaDebug) {
					Console::Error.WriteLine("done...");
					Console::Error.Flush();
				}
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
	///////////////////////////////////////////////////////////////

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
		//if (iReturn<0) Console::Error.WriteLine("SafeAddWrappedPositiveOnly returned negative for iReturn!");
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
	bool AddScreenItem(int iType, int zOrder, int iEntityIndex) {
		bool bGood=false;
		if (iScreenItems<MAX_SCREENITEMS) {
			screenitemarr[iScreenItems].zOrder=zOrder;
			screenitemarr[iScreenItems].index=iEntityIndex;
			screenitemarr[iScreenItems].iType=iType;
			iScreenItems++;
			bGood=true;
		}
		return bGood;
	}
	///////////////////////////////////////////////////////////////
	void QuitSafe() {
		Console::Error.Write("Running QuitSafe...");
		Console::Error.Flush();
		if (!bSDLQuitDone) {
			SDL_Quit();//this is required--cleans up SDL window.  In some situations, SDL_Quit can be the atexit parameter.
			bSDLQuitDone=true;
			Console::Error.WriteLine("done.");
		}
		else Console::Error.WriteLine("Nothing more to do.");
	}
	////////////////////////////////////////////////////////////////////////////////
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
				xPos=xPos-hero->m3dEnt.X;
				yPos=yPos-hero->m3dEnt.Y;
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
			ShowExn(exn,"RadarDotAt");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("RadarDotAt");
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
			ShowExn(exn,"DrawRadarDot");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("DrawRadarDot");
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
			ShowExn(exn,"DrawRadarRect");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("DrawRadarRect");
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
	}//end DrawRadarField
	void DrawExclusiveRect(int left, int top, int bottom, int right, Uint32 u32Pixel, bool bFilled) {
		if (left<0||right>=BUFFER_WIDTH||top<0||bottom>=BUFFER_HEIGHT)//error checking
			return;
		left+=SCREEN_OFFSET_X;
		right+=SCREEN_OFFSET_X;
		top+=SCREEN_OFFSET_Y;
		bottom+=SCREEN_OFFSET_Y;
		bool bGood=true;
		Uint32* lpu32Dest=(UInt32*)gbScreen.arrbyData; //TODO: avoid low-level operations
		int iScreenW=gbScreen.iWidth;
		//int iScreenH=gbScreen.iHeight;
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
		int iStart=top*iScreenW+left;//no stride since using UInt32* to buffer
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
	}//end DrawExclusiveRect
	void DrawAxis(Mass3d& m3dAxisCenter) {
		Mass3d m3dAxisFarX;
		m3dAxisFarX.X+=1.0;
		Mass3d m3dAxisFarY;
		m3dAxisFarY.Y+=1.0;
		Mass3d m3dAxisFarZ;
		m3dAxisFarZ.Z+=1.0;
		//start rotation
		Rotate(m3dAxisFarX.X,m3dAxisFarX.Y,m3dAxisCenter.zRot);
		Rotate(m3dAxisFarX.X,m3dAxisFarX.Z,m3dAxisCenter.yRot);
		Rotate(m3dAxisFarX.Y,m3dAxisFarX.Z,m3dAxisCenter.xRot);
		Rotate(m3dAxisFarY.X,m3dAxisFarY.Y,m3dAxisCenter.zRot);
		Rotate(m3dAxisFarY.X,m3dAxisFarY.Z,m3dAxisCenter.yRot);
		Rotate(m3dAxisFarY.Y,m3dAxisFarY.Z,m3dAxisCenter.xRot);
		Rotate(m3dAxisFarZ.X,m3dAxisFarZ.Y,m3dAxisCenter.zRot);
		Rotate(m3dAxisFarZ.X,m3dAxisFarZ.Z,m3dAxisCenter.yRot);
		Rotate(m3dAxisFarZ.Y,m3dAxisFarZ.Z,m3dAxisCenter.xRot);
		//end rotation
		m3dAxisCenter.OffsetSomethingByMyLocation(m3dAxisFarX);
		m3dAxisCenter.OffsetSomethingByMyLocation(m3dAxisFarY);
		m3dAxisCenter.OffsetSomethingByMyLocation(m3dAxisFarZ);

		Draw3DLine(m3dAxisCenter,m3dAxisFarX,255,0,0,255,0);
		Draw3DLine(m3dAxisCenter,m3dAxisFarY,0,255,0,255,0);
		Draw3DLine(m3dAxisCenter,m3dAxisFarZ,0,0,255,255,0);
		//FPOINT p2dOrig,p2dFarNow;
		//camera.Point2dFrom3dWithScreenOffset(p2dOrig,m3dAxisCenter);
		//Pixel pixelNow;
		//pixelNow.Set(255,0,0,255);
		//camera.Point2dFrom3dWithScreenOffset(p2dFarNow,m3dAxisFarX);
		//gbScreen.DrawSubpixelLine(p2dOrig, p2dFarNow, pixelNow, null, 1);
		//pixelNow.Set(0,255,0,255);
		//camera.Point2dFrom3dWithScreenOffset(p2dFarNow,m3dAxisFarY);
		//gbScreen.DrawSubpixelLine(p2dOrig, p2dFarNow, pixelNow, null, 1);
		//pixelNow.Set(0,0,255,255);
		//camera.Point2dFrom3dWithScreenOffset(p2dFarNow,m3dAxisFarZ);
		//gbScreen.DrawSubpixelLine(p2dOrig, p2dFarNow, pixelNow, null, 1);
	}
	void Draw3DLine(Mass3d& m3dStart, Mass3d& m3dEnd, byte r, byte g, byte b, byte a, byte a_FadeFarthestPoint_To) {
		float fOpacityOfEnd=ANGLEDIFFPOSITIVE(m3dStart.yRot,0);//always near RELATIVE to m3d, not necessarily to camera (see below for swap when not)
		if (fOpacityOfEnd>180) {
			Console::Error.WriteLine("Math usage error: ANGLEDIFFPOSITIVE returned greater than 180 {ANGLEDIFFPOSITIVE("+RString_ToString(m3dStart.yRot)+",0):"+RString_ToString(fOpacityOfEnd)+"}");
			//fOpacityOfEnd=fOpacityOfEnd-180;
		}
		if (fOpacityOfEnd>90) {
			fOpacityOfEnd=180-fOpacityOfEnd;
		}
		//if (fOpacityOfEnd<0) fOpacityOfEnd=180+fOpacityOfEnd;
		fOpacityOfEnd=(fOpacityOfEnd)/90.0f;//the closer it is to zero (angled toward horizon) the less opaque End pixel will be (unless flipped when point End from object is closer to camera)
		byte byFirstAlpha=a;//=0;
		byte byLastAlpha=APPROACH(a,a_FadeFarthestPoint_To,1.0f-fOpacityOfEnd);//SafeByRoundF((fOpacityOfEnd)*255.0f);
		if (DIST3D(camera.m3dCam,m3dEnd)<DIST3D(camera.m3dCam,m3dStart)) {
			byte bySwap=byFirstAlpha;
			byFirstAlpha=byLastAlpha;
			byLastAlpha=bySwap;
		}
		static Pixel pixelStart,pixelEnd;
		pixelStart.Set(r,g,b,byFirstAlpha);
		pixelEnd.Set(r,g,b,byLastAlpha);
		static FPOINT pointSrc, pointDest;
		camera.Point2dFrom3dWithScreenOffset(pointSrc,m3dStart);
		camera.Point2dFrom3dWithScreenOffset(pointDest,m3dEnd);
		gbScreen.DrawSubpixelLine( pointSrc, pointDest, pixelStart, &pixelEnd, 1);
	}
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
				Console::Error.WriteLine("TTF_OpenFont: "+TTF_GetError());
			}
			bTTFStarted=true;
		}
		else if (!bTTFError) {
			SDL_Color fontcolor={0,0,0,255};	   //The color that the text will be rendered in by SDL_ttf.
			char fonttext[800];					 //The text that is rendered by SDL_ttf.
			sprintf(fonttext, "Entity.top: %i, Entity.bottom: %i,  Entity.left: %i, Entity.right: %i", hero->m2dEnt.rectRender.top, hero->m2dEnt.rectRender.bottom,hero->m2dEnt.rectRender.left,hero->m2dEnt.rectRender.right);
			scoretext=TTF_RenderText_Solid(font, fonttext, fontcolor);
			SDL_Surface *srcimg=scoretext;
			int sx=0,sy=0,sw=SCREEN_WIDTH,sh=SCREEN_HEIGHT/6,dx=0,dy=0,alpha=255;
			if ((!srcimg) || (alpha==0)) return; //If there's no image, or its 100% transparent.
			SDL_Rect rectSrc, rectDest;					 //The two rectangles are filled with the information passed to the function.
			rectSrc.X=sx;  rectSrc.Y=sy;  rectSrc.w=sw;  rectSrc.h=sh;
			rectDest.X=dx;  rectDest.Y=dy;  rectDest.w=rectSrc.w;  rectDest.h=rectSrc.h;
			if (alpha != 255) SDL_SetAlpha(srcimg, SDL_SRCALPHA, alpha); //Make SDL aware of the desired Alpha transparency in the source image.
			SDL_BlitSurface(srcimg, &rectSrc, screen, &rectDest);				 //Finally Blit the source on to the destination surface.
		}
	}//end ShowDebugVars
*/
	////////////////////////////////////////////////////////////////////
/*	bool SafeChunkUnload(Mix_Chunk* &mcToNull) {
		bool bGood=false;
		try {
			if (mcToNull!=NULL) {
				Mix_FreeChunk(mcToNull);
				mcToNull=NULL;
			}
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn, "SafeChunkUnload");
		}
		return bGood;
	}//end SafeChunkUnload
*/
	void ClearSounds() {
		sounder.Clear();
	}
	void PlaySounds() {
		//int iPlayed=0;
		if (bMegaDebug) {
			Console::Error.WriteLine("PlaySounds...");
			Console::Error.Flush();
		}
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
			ShowExn(exn,"PlaySounds", "setting up before loop");
		}
		if (bMegaDebug) {
			Console::Error.Write("PlaySounds {iPlayed:"+RString_ToString(iPlayed)+"} done...");
			Console::Error.Flush();
		}
		//return iPlayed;
	}//end PlaySounds
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
				byte byBrightness=SafeByte( (ANGLEDIFFPOSITIVE(fAzimuth,90.0f)/180.0f) * 128.0f + 128.0f);
				if (byBrightness<128) byBrightness=255;//fixes byte integer looping
				if (bMouseDown) {
					hero->Shoot(fAltitude,fAzimuth); //p3dCursorAbs is an absolute position
				}
				if (bMouseMove) {
					hero->Turn(fAzimuth);
				}
				m3dHeroEye.X=hero->EyeX();
				m3dHeroEye.Y=hero->EyeY();
				m3dHeroEye.Z=hero->EyeZ();
				m3dCursor.X=m3dHeroEye.X;
				m3dCursor.Y=m3dHeroEye.Y;
				m3dCursor.Z=m3dHeroEye.Z;
				Travel3d(m3dCursor, fAltitude, fAzimuth, fAimerDistance);
				AddScreenItem(SCREENITEM_3DCURSOR,ZORDER_FROMY(m3dCursor.Y),byBrightness);
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
				p3dHeroEye.X=hero->EyeX();
				p3dHeroEye.Y=hero->EyeY();
				p3dHeroEye.Z=hero->EyeZ();
				FPOINT fptHeroeye;
				camera.Point2dMultipliersFrom3d(fptHeroeye.X,fptHeroeye.Y,p3dHeroEye.X,p3dHeroEye.Y,p3dHeroEye.Z);
				fptHeroeye.X=fptHeroeye.X*SCREEN_WIDTH;
				fptHeroeye.Y=fptHeroeye.Y*SCREEN_HEIGHT;
				xRelNear=xfCursor-fptHeroeye.X; yRelNear=yfCursor-fptHeroeye.Y;

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
					xCursorNearFlatAbs=APPROACH(xCursorNearFlatAbs,fptHeroeye.X,fBackBy);
					yCursorNearFlatAbs=APPROACH(yCursorNearFlatAbs,fptHeroeye.Y,fBackBy);
					fDistFlatNear=APPROACH(fDistFlatFar,0,fBackBy);
				}
				else {
					bBack=false;
					fDistFlatNear=fDistFlatFar;
				}
				xRelNear=xCursorNearFlatAbs-fptHeroeye.X;
				yRelNear=yCursorNearFlatAbs-fptHeroeye.Y;
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
				xCursorNearFlatAbs=fptHeroeye.X+xRelNear;
				yCursorNearFlatAbs=fptHeroeye.Y+yRelNear;
				//gbScreen.DrawSubpixelLine(fptHeroeye.X+xRelNear+FSCREEN_OFFSET_X, //debug only
				//							   fptHeroeye.Y+yRelNear+FSCREEN_OFFSET_Y,
				//							   fptHeroeye.X+xRelNear+FSCREEN_OFFSET_X-zFake,
				//							   fptHeroeye.Y+yRelNear+FSCREEN_OFFSET_Y+zFake,
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
				//gbScreen.DrawSubpixelLine(fptHeroeye.X+FSCREEN_OFFSET_X, fptHeroeye.Y+FSCREEN_OFFSET_Y, xCursorNearFlatAbs+FSCREEN_OFFSET_X, yCursorNearFlatAbs+FSCREEN_OFFSET_Y,
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
				p3dCursorAbs.X=p3dHeroEye.X;
				p3dCursorAbs.Y=p3dHeroEye.Y;
				p3dCursorAbs.Z=p3dHeroEye.Z;

				camera.Point3dMoveAndRotateBy2d(p3dCursorAbs,xRelNear,yRelNear,zFake,fptHeroeye.X,fptHeroeye.Y);
				p3dCursorRel.X=p3dCursorAbs.X-p3dHeroEye.X;
				p3dCursorRel.Y=p3dCursorAbs.Y-p3dHeroEye.Y;
				p3dCursorRel.Z=p3dCursorAbs.Z-p3dHeroEye.Z;
				float fForwardness=-1.0f*zFake/fAimRadNear;// *-1.0 since more forward if negative
				FPOINT fptCursor;

				camera.Point2dFrom3d(fptCursor,p3dCursorAbs);
				bBack=(p3dCursorAbs.Y>p3dHeroEye.Y);//debug does not account for non-Z=90 camera rotations
				float fFrontness;//=(bBack?127:255);
				float fAimRadMeters=DIST3D(p3dHeroEye,p3dCursorAbs); //always the same since radius is constant
				float fAimDiameterMeters=fAimRadMeters*2.0f; //always the same since constant radius
				fFrontness=fabs(p3dCursorAbs.Y-(p3dHeroEye.Y+fAimRadMeters))/fAimDiameterMeters; //debug does not account for other camera rotation
				Crop(fFrontness,0,1);
				pixelNow.Set(0,(byte)((fFrontness*192.0f)+63.0f),0,255);
				gbScreen.DrawSubpixelArc(fptCursor.X+FSCREEN_OFFSET_X, fptCursor.Y+FSCREEN_OFFSET_Y, fCircleResult, 1.0f-(fDistFlatNear/fAimRadNear), fAngleFlatNonCartesian, 0, 360,
					pixelNow,.5,0);
				//gbScreen.DrawSubpixelLine(fptHeroeye.X+FSCREEN_OFFSET_X, fptHeroeye.Y+FSCREEN_OFFSET_Y, xCursorNearFlatAbs+FSCREEN_OFFSET_X, yCursorNearFlatAbs+FSCREEN_OFFSET_Y,
				//		(bBack?64:128),0,0,(bBack?64:128),.3);
				float fAlpha=191+fForwardness*128;
				if (fAlpha>255.0f) fAlpha=255.0f;
				Pixel pixelEnd;
				pixelEnd.Set(0,(byte)(191.0f+fForwardness*64),0,(byte)fAlpha);
				pixelNow.Set(pixelEnd.r,pixelEnd.g,pixelEnd.b,0);
				gbScreen.DrawSubpixelLine(fptHeroeye.X+FSCREEN_OFFSET_X, fptHeroeye.Y+FSCREEN_OFFSET_Y, fptCursor.X+FSCREEN_OFFSET_X, fptCursor.Y+FSCREEN_OFFSET_Y,
						pixelNow,&pixelEnd,.9);
				//static bool Refresh3dCursor_bFirstRun=true;
				//if (bTurn||Refresh3dCursor_bFirstRun) {//commented for debug only
					//float zRealRot;
					//zRealRot=THETAOFXY(xRelNear,zFake);
					hero->Turn(p3dCursorAbs.zRot);
				//	Refresh3dCursor_bFirstRun=false;//commented for debug only
				///}
				if (bMouseDown) {
					hero->Shoot(p3dCursorAbs.yRot,p3dCursorAbs.zRot); //p3dCursorAbs is an absolute position
				}
				*/
				//int iTest=0;
				//if (iTest<100 && bMouseMove) {
				//	//Console::Error.WriteLine("Draw3dCursor {fptHeroeye.X:"+fptHeroeye.X+"; fptHeroeye.Y:"+fptHeroeye.Y+"; xCursorNearFlatAbs:"+xCursorNearFlatAbs+"; yCursorNearFlatAbs:"+yCursorNearFlatAbs+"; hEyeX():"+p3dHeroEye.X+"; EyeY():"+p3dHeroEye.Y+"; EyeZ():"+p3dHeroEye.Z+"}");
				//	Console::Error.WriteLine("Draw3dCursor {fptHeroeye.X:"+fptHeroeye.X+";\tfptHeroeye.Y:"+fptHeroeye.Y+";\tp3dCursorAbs.X:"+p3dCursorAbs.X+";\tp3dCursorAbs.Y:"+p3dCursorAbs.Y+";\tp3dCursorAbs.Z:"+p3dCursorAbs.Z+";\txCursorNearFlatAbs:"+xCursorNearFlatAbs+";\tyCursorNearFlatAbs:"+yCursorNearFlatAbs+";\tzFake:"+zFake+";\tEyeX():"+p3dHeroEye.X+";\tEyeY():"+p3dHeroEye.Y+";\tEyeZ():"+p3dHeroEye.Z+"}");
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
			ShowExn(exn,"Refresh3dCursor");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Refresh3dCursor");
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
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelNow,&pixelEnd,.9);
		//draw screen-edge crosshairs:
		pixelStart.Set(255,0,0,255);
		pixelEnd.Set(255,0,0,255);
		float fLenPix=16;
		//top:
		fpStart.X=xfCursor+FSCREEN_OFFSET_X;
		fpStart.Y=FSCREEN_OFFSET_Y;
		fpEnd.X=fpStart.X;
		fpEnd.Y=fpStart.Y+fLenPix;
		gbScreen.DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
				pixelNow,&pixelEnd,.9);
		//bottom:
		fpStart.X=xfCursor+FSCREEN_OFFSET_X;
		fpStart.Y=FSCREEN_OFFSET_Y+FSCREEN_HEIGHT;
		fpEnd.X=fpStart.X;
		fpEnd.Y=fpStart.Y-fLenPix;
		gbScreen.DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
				pixelNow,&pixelEnd,.9);
		//left:
		fpStart.X=FSCREEN_OFFSET_X;
		fpStart.Y=yfCursor+FSCREEN_OFFSET_Y;
		fpEnd.X=fpStart.X+fLenPix;
		fpEnd.Y=fpStart.Y;
		gbScreen.DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
				pixelNow,&pixelEnd,.9);
		//right:
		fpStart.X=FSCREEN_OFFSET_X+FSCREEN_WIDTH;
		fpStart.Y=yfCursor+FSCREEN_OFFSET_Y;
		fpEnd.X=fpStart.X-fLenPix;
		fpEnd.Y=fpStart.Y;
		gbScreen.DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
				pixelNow,&pixelEnd,.9);

	}//end Draw3dCursor
	void SayWhatIDrewIfFalse(bool bDrawFunctionReturn, int iEntityType, string sDescription) {
		if (!bDrawFunctionReturn) {
			try {
				if (ShowError()) {
					string sNow=EntityTypeToString(iEntityType);
					Console::Error.WriteLine(sNow+" failed to draw "+sDescription+".");
				}
			}
			catch (exception& exn) {
				ShowExn(exn,"SayWhatIDrewIfFalse");
			}
			catch (...) {
				ShowUnknownExn("SayWhatIDrewIfFalse");
			}
		}
	}//end SayWhatIDrewIfFalse
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
	}//end EntityTypeToString
	void DrawCube(Mass3d &m3dNow, Pixel &pixelNear, Pixel &pixelFar) {
		FPOINT fpStart, fpEnd;
		Mass3d m3darrBox[8]; //(see drawing 1.box-indeces.png)
		float
			xSizeHalf=m3dNow.xSize/2.0f,
			ySizeHalf=m3dNow.ySize/2.0f,
			zSizeHalf=m3dNow.zSize/2.0f;
		m3darrBox[0].X=m3dNow.X-xSizeHalf;
		m3darrBox[0].Y=m3dNow.Y+ySizeHalf;
		m3darrBox[0].Z=m3dNow.Z+zSizeHalf;
		m3darrBox[1].X=m3dNow.X+xSizeHalf;
		m3darrBox[1].Y=m3dNow.Y+ySizeHalf;
		m3darrBox[1].Z=m3dNow.Z+zSizeHalf;
		m3darrBox[2].X=m3dNow.X+xSizeHalf;
		m3darrBox[2].Y=m3dNow.Y-ySizeHalf;
		m3darrBox[2].Z=m3dNow.Z+zSizeHalf;
		m3darrBox[3].X=m3dNow.X-xSizeHalf;
		m3darrBox[3].Y=m3dNow.Y-ySizeHalf;
		m3darrBox[3].Z=m3dNow.Z+zSizeHalf;

		m3darrBox[4].X=m3dNow.X-xSizeHalf;
		m3darrBox[4].Y=m3dNow.Y+ySizeHalf;
		m3darrBox[4].Z=m3dNow.Z-zSizeHalf;
		m3darrBox[5].X=m3dNow.X+xSizeHalf;
		m3darrBox[5].Y=m3dNow.Y+ySizeHalf;
		m3darrBox[5].Z=m3dNow.Z-zSizeHalf;
		m3darrBox[6].X=m3dNow.X+xSizeHalf;
		m3darrBox[6].Y=m3dNow.Y-ySizeHalf;
		m3darrBox[6].Z=m3dNow.Z-zSizeHalf;
		m3darrBox[7].X=m3dNow.X-xSizeHalf;
		m3darrBox[7].Y=m3dNow.Y-ySizeHalf;
		m3darrBox[7].Z=m3dNow.Z-zSizeHalf;
		float fSubPixAccuracy=3;
		camera.Point2dFrom3d(fpStart,m3darrBox[4]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[5]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelFar,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[4]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[7]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[5]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[6]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[4]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[0]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelFar,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[5]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[1]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelFar,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[0]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[1]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelFar,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[0]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[3]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[1]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[2]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelFar,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[7]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[6]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelNear,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[7]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[3]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelNear,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[6]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[2]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelNear,&pixelNear,fSubPixAccuracy);
		camera.Point2dFrom3d(fpStart,m3darrBox[3]);
		camera.Point2dFrom3d(fpEnd,m3darrBox[2]);
		gbScreen.DrawSubpixelLine(fpStart.X+FSCREEN_OFFSET_X, fpStart.Y+FSCREEN_OFFSET_Y, fpEnd.X+FSCREEN_OFFSET_X, fpEnd.Y+FSCREEN_OFFSET_Y,
				pixelNear,&pixelNear,fSubPixAccuracy);
	}//end DrawCube
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
				if (bFirstRun) Console::Error.Write("debug test...");

				//Uint8* lpbyScreen=(Uint8*)screen->pixels;//debug only
				//*((Uint32*)lpbyScreen)=0xFFFFFF; //debug only
				//*(Uint32*)(&lpbyScreen[32*iDestStride+32*iDestBytesPP])=0xFFFFFF; //debug only
				//gbScreen.Fill(255);//debug only

				if (bFirstRun) Console::Error.Write("copy...");
				lpDestLine=(Uint8*)screen->pixels;//8bit in order to use stride
				switch (iDestBytesPP) {
					case 1:
						for (int Y=0; Y<iDestHeight; Y++) {
							lpDestPix=lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int X=0; X<iDestWidth; X++) {
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
						for (int Y=0; Y<iDestHeight; Y++) {
							lpwDestPix=(Uint16*)lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int X=0; X<iDestWidth; X++) {
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
							for (int Y=0; Y<iDestHeight; Y++) {
								lpDestPix=lpDestLine;
								lpSrcPix=lpSrcLine;
								for (int X=0; X<iDestWidth; X++) {
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
							for (int Y=0; Y<iDestHeight; Y++) {
								lpDestPix=lpDestLine;
								lpSrcPix=lpSrcLine;
								for (int X=0; X<iDestWidth; X++) {
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
						if (bFirstRun) Console::Error.Write("32-bit from "+RString_ToString(gbScreen.iWidth)+"X"+RString_ToString(gbScreen.iHeight)+"X"+RString_ToString(gbScreen.iBytesPP*8)+"...");
						for (int Y=0; Y<iDestHeight; Y++) {
							lpu32DestPix=(Uint32*)lpDestLine;
							lpSrcPix=lpSrcLine;
							for (int X=0; X<iDestWidth; X++) {
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
			ShowExn(exn,"DrawScreen");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("DrawScreen");
		}
		if (bFirstRun) Console::Error.WriteLine(bGood?"success.":"fail!");
		bFirstRun=false;
		return bGood;
	}//end DrawScreen

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
		byte* lpbyDest;
		byte* lpbySrc;
		float bSrc,gSrc,rSrc,aSrc, bDest,gDest,rDest, bAtScaledExplodedDest,gAtScaledExplodedDest,rAtScaledExplodedDest, bCooked,gCooked,rCooked;
		//byte bySrcB,bySrcG,bySrcR,bySrcA;
		//UInt32 u32Pixel;
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
		int yMaxExploder, xMaxExploder;//, xOffScaledOnly,yOffScaledOnly,
		int iHalfWidth, iHalfHeight;//explosion vars
		float fX, fY;
		float fDestW, fDestH;
		float xfFlat, yfFlat;
		float fSrcW, fSrcH;
		float fSrcStride;
		int iLimiter;
		int xDestRel,yDestRel;
		int yDestNow;
		int xDestNow=-1;//this is set later
		int iSrcNow;
		float fAlphaness;
		float rExploded, gExploded, bExploded;
		//UInt32 u32PixelMax;
		//float fMiddleOfExplosion, fMaxExploder, fDistFromEnds;
		//float fInvMiddle;
		//int iExploder, iOffset, iDestExp;
		int iDestStart;
		long double iDestEnd;
		int iDestBytes;

		try {
			if (gbDest.arrbyData==NULL) {
				if (ShowError()) {
					Console::Error.WriteLine("Null dest image in GBuffer_FX_Scaled");
					bShowVars=true;
				}
				bGood=false;
			}
			else if (gbSrc.arrbyData==NULL) {
				if (ShowError()) {
					Console::Error.WriteLine("Null source image in GBuffer_FX_Scaled");
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
						Console::Error.WriteLine("Can't draw "+RString_ToString((int)(fSrcW*fScale+.5f))+"X"+RString_ToString((int)(fSrcH*fScale+.5f))+" image at index "+RString_ToString(iDestStart)+" "+"("+RString_ToString(xFlat)+","+RString_ToString(yFlat)+") (spans to index "+RString_ToString(iDestEnd)+" ("+RString_ToString(xFlat)+RString_ToString((int)((float)iSrcW*fScale+.5f))+","+RString_ToString(yFlat)+RString_ToString((int)((float)iSrcH*fScale+.5f))+") of "+RString_ToString(iDestBytes)+") (was "+RString_ToString(fSrcW)+"X"+RString_ToString(fSrcH)+" scaled by "+RString_ToString(fScale)+").  Limit is ("+RString_ToString(fDestW)+","+RString_ToString(fDestH)+")");
						bShowVars=true;
					}
					bGood=false;
				}
			}// end else good
			if (bGood) {
				lpu32DestNow=&lpu32Dest[iDestStart];
				iSrcPixels=gbSrc.iWidth*gbSrc.iHeight;
				iDestPixels=gbDest.iWidth*gbDest.iHeight;
				lpbyDest=(byte*)lpu32DestNow;
				lpbySrc=NULL;//=image.buffer;
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
				fY=0.0f, fX=0.0f;
				yMaxExploder=40;
				xMaxExploder=40;
				fSrcStride=(float)iSrcStride;
				iLimiter=gbDest.iHeight-1;
				xDestRel=0; yDestRel=0;
				//static byte by255=255;//avoids type conversions from double constant (?)

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
								Console::Error.WriteLine("Can't Draw: Out of bounds of source image in GBuffer_FX_Scaled.");
								bShowVars=true;
							}
							bGood=false;
						}
						if (bGood) {
							lpbySrc=(byte*)&lpu32Src[iSrcNow];
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
									Console::Error.WriteLine("Out of bounds of destination in GBuffer_FX_Scaled.");
									bShowVars=true;
								}
								bGood=false;
							}
							else if (iDestScaledExploded>=iDestPixels || iDestScaledExploded<0) {
								//if (ShowError()) {
								//	Console::Error.WriteLine("Out of exploded bounds of destination in GBuffer_FX_Scaled.");
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
											//u32PixelMax=_RGB32BIT(255,(byte)rExploded,(byte)gExploded,(byte)bExploded);

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
											//		lpu32Dest[iDestExp]=u32Pixel;//_RGB32BIT(255,(byte)rExploded,(byte)gExploded,(byte)bExploded);
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
									lpbyDest=(byte*)&lpu32Dest[iDestScaledExploded];//now get exploded location's color
									bAtScaledExplodedDest=*lpbyDest;
									gAtScaledExplodedDest=lpbyDest[1];
									rAtScaledExplodedDest=lpbyDest[2];
									bCooked=APPROACH(bAtScaledExplodedDest,bSrc,fAlphaness);//by3dAlphaLookup[blue][*lpbyDest][bySrcA];
									gCooked=APPROACH(gAtScaledExplodedDest,gSrc,fAlphaness);//by3dAlphaLookup[green][*lpbyDest][bySrcA];
									rCooked=APPROACH(rAtScaledExplodedDest,rSrc,fAlphaness);//by3dAlphaLookup[red][*lpbyDest][bySrcA];
									//u32Pixel=_RGB32BIT(by255,(byte)rCooked,(byte)gCooked,(byte)bCooked);
									if (iDestScaledExploded<BUFFER_PIXELS) {
										if (bExplode) {
											//also paint second pixel (image-shaped) for explosion
											rExploded=APPROACH(rDest,APPROACH(255.0f,rSrc,fExplodeExp),fAlphaness);
											gExploded=APPROACH(gDest,APPROACH(255.0f,gSrc,fExplodeExp),fAlphaness);
											bExploded=APPROACH(bDest,APPROACH(200.0f,bSrc,fExplodeExp),fAlphaness);

											//u32PixelMax=_RGB32BIT(255,(byte)rExploded,(byte)gExploded,(byte)bExploded);
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
											//	for (register int xSubPix=0; xSubPix<iExploder; xSubPix++) {
											//		if (iDestExp<0||iDestExp>=BUFFER_PIXELS) {ySubPix=iExploder;break;}
											//		lpu32Dest[iDestExp]=u32Pixel;//_RGB32BIT(255,(byte)rExploded,(byte)gExploded,(byte)bExploded);
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
			ShowExn(exn,"GBuffer_FX_Scaled");
		}
		catch (...) {
			bGood=false;
			bShowVars=true;
			ShowUnknownExn("GBuffer_FX_Scaled");
		}
		if (bShowVars) {
			ShowError("  { iDestScaled:"+RString_ToString(iDestScaled)
							+"; source.w:"+RString_ToString(gbSrc.iWidth)
							+"; source.h:"+RString_ToString(gbSrc.iHeight)
							+"; xFlat:"+RString_ToString(xFlat)
							+"; yFlat:"+RString_ToString(yFlat)
							+"; fX:"+RString_ToString(fX)
							+"; fY:"+RString_ToString(fY)
							+"; xDestNow:"+RString_ToString(xDestNow)
							+"; yDestNow:"+RString_ToString(yDestNow)
							+"; fInverseScale:"+RString_ToString(fInverseScale)
							+"; fScale:"+RString_ToString(fScale)
							+"; fExplodedness:"+RString_ToString(fExplodedness)
							+"; }");
		}
		return(bGood);
	} //end GBuffer_FX_Scaled

	void InitMusic() {
	//	if (!MusicThreadFunction_bStarted) {
	//		psdl_threadMusicThread = SDL_CreateThread(MusicThreadFunction, NULL );
	//	}
		//start MusicThreadFunction thread here
	}//end InitMusic
	void StopMusic() {
		//TODO: finish this--uncomment: oggMusic.release();
		//if (music!=NULL) {
		//	Mix_HaltMusic();
		//	Mix_FreeMusic(music);
		//	music=NULL;
		//}
	}
	void PlayMusic(string sFile, bool bLoop) {
	//TODO: OpenAL PlayMusic
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
			ShowExn(exn,"PlayMusic");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("PlayMusic");
		}
	}//end PlayMusic

	void SoundInstance::SetLoc(Mass3d& m3dLoc) {
		//TODO: finish this: adjust these
		arralfPos[0]=m3dLoc.X;
		arralfPos[1]=m3dLoc.Y;
		arralfPos[2]=m3dLoc.Z;
	}
	void SoundInstance::SetVel(Mass3d& m3dVel) {
		//TODO: finish this: adjust these if OpenAl is not cartesian
		arralfPos[0]=m3dVel.X;
		arralfPos[1]=m3dVel.Y;
		arralfPos[2]=m3dVel.Z;
	}

	Sound::Sound() {
		sName="";
	}

	Sounder::Sounder() {
	}
	void Sounder::Init() {
		iSoundarrLen=SOUND_BUFFERS;
		iInstancearrLen=SOUND_SOURCES;
		int iNow;
		for (iNow=0; iNow<iSoundarrLen; iNow++) {
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
				ShowExn(exn,"Sounder Init");
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
			//TODO: finish this--uncomment: alSourcef (aluiarrSource[iAt], AL_PITCH,	1.0f);
			//TODO: finish this--uncomment: alSourcef (aluiarrSource[iAt], AL_GAIN,	 1.0f);
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


	bool ShutdownMusic() {
		bool bGood=false;
		try {//end of main event loop
			//if (music!=NULL) {
			//	Mix_HaltMusic();
			//	Mix_FreeMusic(music);
			//	music=NULL;
			//	Console::Error.WriteLine("Stopping Music...done");
			//}
			//else Console::Error.WriteLine("Stopping Music...was finished.");
			//TODO: finish this--uncomment: oggMusic.release();
			//bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"ShutDownMusic");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("ShutDownMusic");
		}
		return bGood;
	}//end ShutDownMusic wrapper
	void MusicDone() { //runs when music is done
		//if (music!=NULL) {
			//Mix_HaltMusic();
			//Mix_FreeMusic(music);
			//music=NULL;
		//}
	}
	///#endregion functions
}//end namespace

using namespace ExpertMultimediaBase;
//using namespace ProtoArmor;
//int SDL_main();
//int main(int argc, char *argv[]) {
//int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR
//	lpCmdLine, unsigned int nCmdShow ) {
int main(int iArgs, char *lpsArg[]) { //int main(int iArgs, char** lpsArg) {
	Console::Error.WriteLine("GameInit:");//NOTE: this will be written to stderr.txt because of SDL's trickery!!!
	sVerb_Engine="running GameInit (in main)";
	p3dHero.X=0;
	p3dHero.Y=0;
	p3dHero.Z=0;
	//Console::Error.WriteLine("Calling GameInit...");
	bool bOK=GameInit();
	//Console::Error.WriteLine("Calling GameInit...Done.");
	sVerb_Engine="checking GameInit status";
	int iMainReturn=0;//start good
	if (!bOK) {
		Console::Error.WriteLine("GameInit Failed!");
		ShowError("GameInit...Failed!");
		//re-implement this w/o using win32 api: MessageBox (0, "GameInit Failed, reinstalling game may fix the error", "Failed to Load Game", MB_ICONHAND);
		iErrors++;
		bDone=true;
		//iMainReturn=1;//"bad...bad program :spank:"
	}
	else if (bSplash) {
		Console::Error.WriteLine("GameInit:Done.");
		SleepWrapper(3000);
	}
	else  {
		Console::Error.WriteLine("GameInit:Done (did not try to load splash screen, since bSplash is turned off).");
	}
	//main event loop:
	if (!bDone) Console::Error.WriteLine("Running main event loop.");
	else ShowError("Bypassing main loop, due to failed init.");
	sVerb_Engine="entering main event loop";
	UInt32 u32LastRefresh=SDL_GetTicks();//TODO: change to seconds??
	bool bFirstRunOfMainEventLoop=true;
	while (!bDone) {
		//UInt32 dwAbsoluteTickStartMainEventLoop=SDL_GetTicks();//absolute ticks for framerate
		if (bFirstRunOfMainEventLoop) Console::Error.WriteLine("FirstRun: Checking for events...");
		//Check for events
		while (SDL_PollEvent (&event)) { //main polling loop
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetRelativeMouseState(&xCursorDown, &yCursorDown);//SDL_GetMouseState(&m3dEnt.X, &m3dEnt.Y);
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
					u32Pressing|=GAMEKEY_EXIT;
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
								delete arrpentAlien[index];
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
					Console::Error.Write("GameShutDown...");
					Console::Error.Flush();
					if (GameShutdown()) Console::Error.WriteLine("...Success (GameShutdown).");
					else Console::Error.WriteLine("Failed!");
				}
				break;
			default:
				break;
			}//end switch event.type
			//int iMusicThreadIterationBreakWarning=SDL_GetTicks();
			//if (SDL_GetTicks()-MusicThread_LastKeepAliveAbsoluteTick > (MusicThread_iIterationDelayTicks*10)) {
			//	if (SDL_GetTicks()-iMusicThreadIterationBreakWarning > (60000)) {//every 60 seconds
			//		iMusicThreadIterationBreakWarning=SDL_GetTicks();
			//		cerr)<<"WARNING: music thread has frozen for "<<((SDL_GetTicks()-MusicThread_LastKeepAliveAbsoluteTick)/1000)<<" seconds!");
			//	}
			//}
			if (bDone) break;
		}//end while checking for events
		sVerb_Engine="exiting";
		if (bFirstRunOfMainEventLoop) {
			Console::Error.Write("FirstRun: Checking whether user exited early...");
			Console::Error.Flush();
		}

		if (!bDone) {
			//SleepWrapper(1);
			//SleepWrapper( (SDL_GetTicks()-u32LastRefresh) + 28)
			//TODO: separate Redraw from Refresh GameMain below
			if (bFirstRunOfMainEventLoop) Console::Error.WriteLine("no.");
			if ( (SDL_GetTicks()-u32LastRefresh) >= 28 ) {//lock to approx 30fps
				u32LastRefresh=SDL_GetTicks();
				GameMain();
			}
			else SleepWrapper(1);
			if (iGameStateWas!=GAMESTATE_RUN) {
				if (narrGameStateCount[GAMESTATE_RUN]>0) {
					dwTicksAcquiredOutsideOfGameState_Run+=(SDL_GetTicks()-dwAbsoluteTickLastGameStateState_Run);
					//ok to use dwAbsoluteTickLastGameStateState_Run already
					// because for GAMESTATE_RUN to have run,
					// dwAbsoluteTickLastGameStateState_Run would have already
					// been set even though the line is below here in the loop.
				}
			}
			dwAbsoluteTickLastGameStateState_Run=SDL_GetTicks();
			PlaySounds();
			if (bSDLQuitDone) bDone=true;
			if (bFirstRunOfMainEventLoop) Console::Error.WriteLine("FirstRun: Finished main event loop successfully.");
		}//end if not bDone do normal stuff
		else if (bFirstRunOfMainEventLoop) Console::Error.WriteLine("yes.");
		bFirstRunOfMainEventLoop=false;
		iMainEventLoopCount++;
	}//end while not bDone continue main event loop
	if (bMegaDebug) Console::Error.WriteLine(RString_ToString("Main event loop ran ")+RString_ToString(iMainEventLoopCount)+RString_ToString(" times."));
	Console::Error.Write("Exiting main after main event loop--Stopping Remaining processes...");
	Console::Error.Flush();
	if (!bSDLQuitDone) {
		SDL_Quit();
		bSDLQuitDone=true;
	}
	if (!bShuttingDown && !bDoneShutdown) {
		Console::Error.Write("GameShutdown...");
		if (GameShutdown()) Console::Error.WriteLine("...OK (GameShutdown)");
		else Console::Error.WriteLine("...FAILED (GameShutdown)");
	}
	Console::Error.WriteLine("Returning from main.");
	return iMainReturn;
}//end main(...)
#endif
