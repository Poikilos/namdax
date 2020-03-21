#ifndef DXMAN_CPP
#define DXMAN_CPP

#include "dxman.h"

using namespace std;

namespace ExpertMultimediaBase {
	RSound rsound;
	//extern string EntityTypeToString(int iTypeX);
	//typedef struct {int left; int right; int top; int bottom;} RECT;
	//typedef unsigned short USHORT;
	//typedef unsigned short WORD;
	//typedef unsigned char BYTE;
	//typedef unsigned __int8 DWORD;
	//typedef unsigned __int16 USHORT;
	//typedef unsigned __int8 BYTE;
	//typedef ULARGE_INTEGER __int64);

	bool RememberTextCursorPosition=false;

	string sarrGameState[] = {"GAMESTATE_INIT","GAMESTATE_START_AREA","GAMESTATE_START_ENCOUNTER","GAMESTATE_RUN","GAMESTATE_SHUTDOWN","GAMESTATE_EXIT","GAMESTATE_WIN_ENCOUNTER","GAMESTATE_WIN_GAME","GAMESTATE_YOU_LOSE","GAMESTATE_LOSEGUY"};
	long narrGameStateCount[] = {0,0,0,0,0,0,0,0,0,0};
	bool bFirstRunRefresh3dCursor=true;
	int iTicksDisplayHelpMessage1=0;
	int iTickLastDisplayHelpMessage1=-1;
	//int iTickLastDebugInterval=0;
	//float fLine1DegDebugNow=0.0f;
	//float fLine2DegDebugNow=0.0f;
	IPoint debugtextcursor_Loc(0,0);//for text drawing
	IPoint statuscursor_Loc(0,0);//for text drawing
	SizeF debugtextcursor_LastTextBlockSize;
	SizeF statuscursor_LastTextBlockSize;
	///#region globals defined as extern in h file
	//int iLastIntersectionType=RMath::IntersectionNo;
	int iLastLineRelationshipType=RMath::LineRelationshipNone;

	GFont gfontDefault;
	bool bDoneShutdown=false;
	bool bShuttingDown=false;
	string sVerb_Engine="before initialization";
	UInt32 u32Pressing=0;
	//DEBUG: ZBUFFER_YRATIO MUST be 50/YRANGE
	int iScreenItemTypes=6;
	string sarrScreenItem_DONTUSEMEDIRECTLY[] = {"(invalid screenitem:0)","SCREENITEM_ALIEN","SCREENITEM_SHOT","SCREENITEM_HERO","SCREENITEM_3DCURSOR","SCREENITEM_CUBE"};
	int iScreenItems=0;
	bool bInMenu=true;
	Uint32 dwAbsoluteTickLastGameStateState_Run=0;
	bool bShowOnce_TargaToTarga32_ANY_Debug=true;
	bool bClosedAudio=false;
	FPOINT3D p3dHero;
	//bDebug=true;
	//bMegaDebug=true; //debug only--see base.cpp instead
	long iMainEventLoopCount=0;//counts iterations of main event loop
	Mass3d m3dTemp;
	Mass3d m3darrDebug[9]; //Mass3d[]
	Mass3d m3dHeroEye, m3dCursor, m3dFiringLineNearPointForDisplayOnly;
	Pixel px3DCursorNear,px3DCursorFar;
	Pixel pixelWhite(255,255,255,255);
	Pixel pixelRed(255,0,0,255);
	Pixel pixelDarkRed(64,0,0,255);
	Pixel pixelGreen(0,255,0,255);
	Pixel pixelBlue(0,0,255,255);
	Pixel pixelCyan(0,255,255,255);
	Pixel pixelMagenta(255,0,255,255);
	Pixel pixelYellow(255,255,0,255);
	Pixel pixelDarkYellow(64,64,0,255);
	Pixel pixelMediumDarkYellow(128,128,0,255);
	//float xAimFlipper=-1.0f;
	//float yAimFlipper=-1.0f;
	//float zAimFlipper=-1.0f;
	//bool bFlipYaw=false;
	//bool bFlipPitch=false;
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
	GBuffer*	screen_ptr=nullptr;
	GBuffer		gbIntro;
	GBuffer		gbIntroTextBack;
	GBuffer		gbIntroText;
	GBuffer*	shield_symbol_image_ptr=nullptr;
	GBuffer*	boss_health_symbol_image_ptr=nullptr;
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

	//	UseScreenW, UseScreenH, x1, 		y1, 	z1, 	xRot1, yRot1,  zRot1, fAperture
	Camera camera(640,480,		0.0f,		-14.836f,5.376f,0.0f,	-11.77,90.0f, 39.2025f,true,true);//make_orientation_Z_UP, make_orientation_X_FORWARD
	Entity* arrpentShot[MAXSHOTS];
	std::vector<Entity*> arrpentAlien(iMaxAliens); //Entity** arrpentAlien=nullptr;
	Entity* hero=nullptr;
	//TODO: improve boss rect alignment
	/* The screen surface */
	SDL_Surface *screen=nullptr;
	SDL_Window *sdlWindow=nullptr;
	SDL_Renderer *sdlRenderer=nullptr;
	SDL_Texture *sdlTexture=nullptr;
	int iDirKeysDown=0;
	bool bDone=false;
	///#endregion globals defined as extern in h file
	SDL_Event event;

	//TTF_Font *font;		 //Pointer to the font we will be using to render the score.
	//SDL_Surface *scoretext; //Surface containing the text rendered by SDL_ttf that has the score.
	//bool bTTFStarted=false;
	//bool bTTFError=false;
	//void ShowDebugVars();

	bool bFirstRunGameMain=true;
	bool bFirstRunOfCurrentState=true;

	///#region functions
	bool GameMain() {
		bool bGood=true;
		if (bFirstRunGameMain) {
			Console::Error.WriteLine();
			Console::Error.WriteLine();
			Console::Error.WriteLine();
		}
		debugtextcursor_Loc.Set(0,0);
		bFirstRunOfCurrentState=(narrGameStateCount[iGameState]<1);
		iGameStateWas=iGameState;
		sVerb_Engine="running "+sarrGameState[iGameState];
		if (bMegaDebug) Console::Error.WriteLine("Running "+sarrGameState[iGameState]+" ("+RString_ToString(narrGameStateCount[iGameState])+" so far)");
		try {
			if (bDone) return(false);
			// for now test if user is hitting ESC
			if (iGameState!=GAMESTATE_EXIT) {
				if ((u32Pressing&GAMEKEY_EXIT)&&(SDL_GetTicks()-iTickAbsoluteEscapeLastPress)>1000) {//int answer=0;
					iTickAbsoluteEscapeLastPress=SDL_GetTicks();
					animCursor.GotoFrame(2);
					animGameScreen.GotoFrame(2);
					animGameScreen.DrawToLargerWithoutCropElseCancel(screen_ptr, (int)( (float)screen_ptr->iWidth/2.0f-animGameScreen.Width()/2.0f ), (int)( (float)screen_ptr->iHeight/2.0f-(float)animGameScreen.Height()/2.0f ), DrawModeBlendAlpha);
					if (bMegaDebug) {
						Console::Error.Write("About to show screen in GAMESTATE_EXIT...");
						Console::Error.Flush();
					}
					DrawScreen();
					if (bMegaDebug) Console::Error.WriteLine("done showing screen in GAMESTATE_EXIT.");
					iTickAbsoluteEscapeLastPress=SDL_GetTicks();
					bool IsWaitingToConfirmExit=true;
					while (IsWaitingToConfirmExit) { //mini polling loop ONE OF FOUR! (confirm exit)
						bMouseMove=false;
						while (SDL_PollEvent (&event)) {
							if (event.type==SDL_MOUSEBUTTONDOWN
									|| event.key.keysym.sym==SDLK_DELETE
									|| event.key.keysym.sym==SDLK_UP
									|| event.key.keysym.sym==SDLK_DOWN
									|| event.key.keysym.sym==SDLK_LEFT
									|| event.key.keysym.sym==SDLK_RIGHT
								) {
								iGameState=GAMESTATE_EXIT;
								IsWaitingToConfirmExit=false;
							}
							else if (event.key.keysym.sym==SDLK_ESCAPE) {
								if((SDL_GetTicks() - iTickAbsoluteEscapeLastPress) > 1000) {
									u32Pressing&=GAMEKEY_EXIT^0xFFFFFFFF;
									iGameState=GAMESTATE_RUN;
									IsWaitingToConfirmExit=false;//TODO: should this be TRUE (since no exiting now)???
								}
							}
							else if (event.type==SDL_QUIT) {
								IsWaitingToConfirmExit=false;
							}
							else if (event.type==SDL_MOUSEMOTION) {
								xCursor=event.motion.x;
								yCursor=event.motion.y;
								xfCursor=xCursor;
								yfCursor=yCursor;
								bMouseMove=true;
							}
							//else if (event.type==SDL_MOUSEMOTION) {
							//	xCursor=event.motion.x;
							//	yCursor=event.motion.y;
							//	xfCursor=xCursor;
							//	yfCursor=yCursor;
							//	bMouseMove=true;
								//TODO: DON'T draw cursor since DOESN'T allow click yes/no (fix?)
								//animCursor.GotoFrame(0);
								//animCursor.DrawToLargerWithoutCropElseCancel(screen_ptr, xCursor-xCursorCenter, yCursor-yCursorCenter, DrawModeBlendAlpha);
							//}
						}
						if (animBackdrop.frame_ptr!=nullptr) animBackdrop.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, 0, 0, DrawModeBlendAlpha);
						if (animGameScreen.frame_ptr!=nullptr) animGameScreen.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, (int)( (float)screen_ptr->iWidth/2.0f-animGameScreen.Width()/2.0f ), (int)( (float)screen_ptr->iHeight/2.0f-animGameScreen.Height()/2.0f ), DrawModeBlendAlpha);
						animCursor.DrawToLargerWithoutCropElseCancel(screen_ptr, xCursor-xCursorCenter, yCursor-yCursorCenter, DrawModeBlendAlpha);

						SleepWrapper(33);
						DrawScreen();
					}//end while continue to ask confirmation
				}//END IF PRESSING EXIT
				animCursor.GotoFrame(0);
			}//end GAMESTATE_EXIT if not already exiting


			if (iGameState==GAMESTATE_INIT) {
				screen_ptr->DrawExclusiveRect(0,0, screen_ptr->iWidth,screen_ptr->iHeight,0x00000000,true);
				gbIntro.DrawToLargerWithoutCropElseCancel(screen_ptr,(int)( ((float)screen_ptr->iWidth/2.0f-(float)gbIntro.iWidth/2.0f) ),0,DrawModeCopyAlpha);
				if (bMegaDebug) {
					Console::Error.Write("About to show screen in GAMESTATE_INIT...");
					Console::Error.Flush();
				}
				DrawScreen();
				if (bMegaDebug) {
					Console::Error.Write("done showing screen in GAMESTATE_INIT.");
					Console::Error.Flush();
				}
				if (!bDebug) SleepAllowingEvents(1500);
				bool bStart=false;
				if (bMegaDebug) Console::Error.WriteLine("Waiting for user to press key/button at Intro Screen in GAMESTATE_INIT.");
				while (!bStart) { //mini polling loop TWO OF FOUR! (show intro)
					if (SDL_PollEvent (&event)) {
						if (event.type==SDL_KEYDOWN) {
							//if (event.key.keysym.sym==SDLK_DELETE
							//		|| event.key.keysym.sym==SDLK_a
							//		|| event.key.keysym.sym==SDLK_s
							//		|| event.key.keysym.sym==SDLK_d
							//		|| event.key.keysym.sym==SDLK_w
							//		|| event.key.keysym.sym==SDLK_RETURN
							//		|| event.key.keysym.sym==SDLK_SPACE
							//		|| event.key.keysym.sym==SDLK_UP
							//		|| event.key.keysym.sym==SDLK_DOWN
							//		|| event.key.keysym.sym==SDLK_LEFT
							//		|| event.key.keysym.sym==SDLK_RIGHT ) {
							bStart=true;
							break;
							//}
						}
						else if (event.type==SDL_MOUSEBUTTONDOWN) {
							bStart=true;
							break;
						}
						else if (event.type==SDL_QUIT) {
							bStart=true;
							break;
						}
					}//end if key is pressed
					DrawScreen();
					SleepWrapper(33);
				}//end while showing intro-title
				if (bMegaDebug) Console::Error.WriteLine("Clearing screen in GAMESTATE_INIT.");
				screen_ptr->DrawExclusiveRect(0,0,screen_ptr->iWidth,screen_ptr->iHeight,0x00000000,true);
				bStart=false;//reset to false in order to show intro text
				Uint32 dwAbsoluteTickStartIntroText=SDL_GetTicks();
				float yIntroTextF=(float)(screen_ptr->iHeight);
				float fOpacityIntroText=.1f;

				Console::Error.WriteLine("Initializing music objects in GAMESTATE_INIT.");
				Console::Error.Flush();

				rsound.LoadFile("Intro", "MixMystery - Namdax - 01 Intro",true);  // formerly "music/MixMystery - DXMan - 01 Intro.ogg",true);
				rsound.LoadFile("Ending","music/MixMystery - DXMan - 05 Ending.ogg",true);
				rsound.LoadFile("Invasion","music/MixMystery-DXMan - 02 Invasion.ogg",true);
				rsound.LoadFile("Do You Want to Know","music/MixMystery - DXMan - 03 Do You Want to Know.ogg",true);
				rsound.LoadFile("Would You Fight","music/MixMystery - DXMan - 04 Would You Fight.ogg",true);
				//rsound.LoadFile("Level Start","sound/levelstart.wav",true);  // elminate this in v2
				rsound.LoadFile("Victory1","sound/victory1.ogg",true);  // formerly victory-trumpets.wav "Victory Trumpets" formerly trumpet.wav
				rsound.LoadFile("Victory2","sound/victory2.ogg",true);  // differs for last level in FX Tracks v2
				Console::Error.WriteLine("done (loading music objects)");

				Console::Error.WriteLine("Initializing sound objects in GAMESTATE_INIT.");
				Console::Error.Flush();
				rsound.LoadFile("Bomb","sound/bomb.wav",false);
				rsound.LoadFile("EnemyLaser","sound/laser-enemy.wav",false); // v2 from lmms; formerly named LaserAlien
				//rsound.LoadFile("Laser","sound/laser.wav",false);  // replaced by Beam in v2
				rsound.LoadFile("Beam","sound/beam.wav",false);  // replaces for v2 (constant particle beam weapon instead of shot weapon)
				//rsound.LoadFile("Explosion","sound/explosion1b.wav",false); //for v2, see enemy-explosion1 instead (EnemyExplosion1)
				//rsound.LoadFile("OuchAlien","sound/ouchalien.wav",false);  // eliminate for v2
				//rsound.LoadFile("OuchZap","sound/ouchzap.wav",false);  // eliminate for v2
				rsound.LoadFile("ShieldZap","sound/shieldzap.wav",false);  // v2 is based on sound formerly known as (unused) shock00.wav
				//rsound.LoadFile("Blerrp","sound/blerrp.wav",false);  // eliminate this in v2
				//rsound.LoadFile("HitDirt","sound/hitdirt.wav",false);  // v2 eliminate player hitting ground sound; use DirtExplosion for enemy ordnance hitting ground (see DirtExplosion)
				rsound.LoadFile("DirtExplosion","sound/explosion-dirt.wav",false);
				rsound.LoadFile("EnemyExplosion1","sound/explosion-enemy1.wav",false); //implement in v2
				//rsound.LoadFile("Jump","sound/jump.wav",false);  // eliminate this in v2
				//rsound.LoadFile("ScrapeGround","sound/scrapeground.wav",false);  // eliminate this in v2
				//rsound.LoadFile("AngryAlien","sound/angryalien.wav",false);  // eliminate this in v2
				//rsound.LoadFile("Thruster","sound/thruster.wav",false);  // eliminate this in sound v2
				rsound.LoadFile("Yeshua","sound/yeshua.wav",false);  // new for v2 (assign to Yeshua's character)
				///TODO: set default volume of thruster to 40% //Mix_VolumeChunk(mcThruster,40);
				//rsound.LoadFile("Laser","sound/laser.wav",false); // this is redundant (discovered while making SoundFX tracks v2)

				///TODO:?? set volume of laser?? //Mix_VolumeChunk(mcLaser,100);
				Console::Error.WriteLine("done (loading sound objects)");

				Console::Error.WriteLine("Playing music in GAMESTATE_INIT.");
				if (settings.GetForcedBool("music")) {
					rsound.PlayAsMusic("Intro");
					//if (!bStartedIntroMusic) VirtualMusicPlay(MusicFileName_Intro, -1);
					//bStartedIntroMusic=true;
				}
				if (bMegaDebug) Console::Error.WriteLine("Waiting for user to press key/button at Intro Text scroll in GAMESTATE_INIT.");
				int xIntroTextI=(screen_ptr->iWidth/2-gbIntroText.iWidth/2);
				while (!bStart) { //mini polling loop THREE OF FOUR! (show intro text)
					if ((int)yIntroTextF+gbIntroText.iHeight <= (screen_ptr->iHeight/3) ) fOpacityIntroText*=.9f;
					else if (fOpacityIntroText<1.0f) {
							fOpacityIntroText+=fOpacityIntroText*(1.0f-fOpacityIntroText);//logorithmic gain
							if (fOpacityIntroText>1.0f) fOpacityIntroText=1.0f;
					}

					if ((int)yIntroTextF+gbIntroText.iHeight <= 0) break;
					if (bMegaDebug) {
						Console::Error.WriteLine("B");
						Console::Error.Flush();
					}
					gbIntroTextBack.DrawToLargerWithoutCropElseCancel(screen_ptr, 0, 0, DrawModeCopyAlpha);
					if (IROUNDF(yIntroTextF)>(-1*(gbIntroText.iHeight))) {
						if (bMegaDebug) {
							Console::Error.WriteLine("F");
							Console::Error.Flush();
						}
						gbIntroText.DrawToWithClipping(screen_ptr, xIntroTextI,IROUNDF(yIntroTextF), fOpacityIntroText);
					}
					if (bMegaDebug) {
						Console::Error.Write("D("+RString_ToString(xIntroTextI)+","+RString_ToString(yIntroTextF)+")");
						Console::Error.Flush();
					}
					DrawScreen();
					yIntroTextF-=1.0f;

					if (SDL_PollEvent (&event)) {
						if (SDL_GetTicks()-dwAbsoluteTickStartIntroText > 1000) {
							if (event.type==SDL_MOUSEBUTTONDOWN) {
								bStart=true;
								break;
							}
							else if (event.type==SDL_KEYDOWN) {
								//if (event.key.keysym.sym==SDLK_DELETE
								//  || event.key.keysym.sym==SDLK_UP
								//  || event.key.keysym.sym==SDLK_DOWN
								//  || event.key.keysym.sym==SDLK_LEFT
								//  || event.key.keysym.sym==SDLK_RIGHT) {
								bStart=true;
								break;
								//}
							}
							else if (event.type==SDL_QUIT) {
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
						//try { TargaToTarga32_Alpha(animCursor[0], screen_ptr, xCursor-xCursorCenter, yCursor-yCursorCenter); }
						//catch (...) {}
					}//end if key is pressed
					SleepWrapper(16);//~60fps
				}//end while showing intro text
				if (iGameState==GAMESTATE_EXIT) {
					Console::Error.WriteLine("iGameState was GAMESTATE_EXIT at start of game!");
					return(false);
				}
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
				if (hero!=nullptr) {
					delete hero;
					hero=nullptr;
				}
				try {
					for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++) {
						if (arrpentAlien[iAliensNow]!=nullptr) {
							if (iAliensNow==iBoss) iBoss=-1;
							delete arrpentAlien[iAliensNow];
							arrpentAlien[iAliensNow]=nullptr;
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
					hero=new Entity(ENTITY_TYPE_HERO,"hero");
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
				Console::Error.Write("GAMESTATE_LOSEGUY...");
				Console::Error.Out.Flush();
				SleepAllowingEvents(1000);
				Console::Error.Write("calling StopMusic...");
				Console::Error.Out.Flush();
				rsound.StopMusic();
				iGuys--;
				if (hero==nullptr) {
					Console::Error.Write("creating hero...");
					Console::Error.Out.Flush();
					hero=new Entity(ENTITY_TYPE_HERO,"hero");
					hero->iIndex=0;
				}
				Console::Error.Write("drawing backdrop...");
				Console::Error.Out.Flush();
				if (animBackdrop.frame_ptr!=nullptr) animBackdrop.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, 0, 0, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
				else Console::Error.WriteLine("animBackdrop.frame_ptr nullptr for frame "+RString_ToString(animBackdrop.get_frame_number()));
				Console::Error.Write("drawing lives...");
				Console::Error.Out.Flush();
				gbLives.DrawToLargerWithoutCropElseCancel(screen_ptr,(int) ( (float)screen_ptr->iWidth/2.0f-(float)gbLives.iWidth/2.0f ), (int)( (float)screen_ptr->iHeight/2.0f-(float)gbLives.iHeight ),DrawModeBlendAlpha);
				Console::Error.Write("accessing offset for life...");
				Console::Error.Out.Flush();
				int this_life_y=screen_ptr->iHeight/2;
				Console::Error.Write("setting hero anim frame...");
				Console::Error.Out.Flush();
				animHero.GotoFrame(0);
				float life_symbol_scale = get_scale_by_percentage_of_smallest_screen_axis(animHero.Width(), animHero.Height(), 0.1f, screen_ptr->iWidth, screen_ptr->iHeight);
				int life_symbol_width=(int)((float)screen_ptr->iWidth*life_symbol_scale);
				Console::Error.Write("life_symbol_scale:"+RString_ToString(life_symbol_scale)+"...");
				Console::Error.Write("life_symbol_width:"+RString_ToString(life_symbol_width)+"...");
				Console::Error.Out.Flush();
				int life_symbol_height=(int)((float)screen_ptr->iHeight*life_symbol_scale);
				int symbol_spacer_width=life_symbol_width/8;
				int symbol_subtractor=(int)((float)life_symbol_width*.8f); //account for blank part (crop image horizontally)
				int symbol_kerning_width=life_symbol_width+symbol_spacer_width-symbol_subtractor;
				if (symbol_kerning_width*iGuys>screen_ptr->iWidth) symbol_kerning_width=screen_ptr->iWidth/iGuys;
				int lives_total_width=symbol_kerning_width*iGuys;
				if (iGuys>1) lives_total_width-=symbol_spacer_width;
				int this_life_x=(screen_ptr->iWidth-lives_total_width)/2-symbol_subtractor/2;
				if (animHero.frame_ptr!=nullptr) {
					for (int iGuy=0; iGuy<iGuys; iGuy++) {
						GBuffer_FX_Scaled(screen_ptr, animHero.frame_ptr, this_life_x, this_life_y, 255.0f, 0.0f, false, life_symbol_scale, "Unit in Reserve", VisualDebugEnable?&gfontDefault:nullptr);
						this_life_x+=symbol_kerning_width;
					}
				}
				iEncounter=1;//go back to first encounter of this area
				Console::Error.Write("drawing screen...");
				Console::Error.Out.Flush();
				DrawScreen();
				Console::Error.Write("sleeping...");
				Console::Error.Out.Flush();
				SleepAllowingEvents(4000);
				Console::Error.Write("done (GAMESTATE_LOSEGUY).");
				Console::Error.Out.Flush();
				iGameState=GAMESTATE_START_ENCOUNTER;
			}//end GAMESTATE_LOSEGUY
			else if (iGameState==GAMESTATE_START_ENCOUNTER) {//START ENCOUNTER
				animGameScreenArea.GotoFrame(iArea-1);
				animGameScreenEncounter.GotoFrame(iEncounter-1);
				iMaxAliensNow=iMaxAliens+iArea-1;
				if (iMaxAliens<iMaxAliensNow) {
					arrpentAlien.reserve(iMaxAliensNow);
					iMaxAliens=iMaxAliensNow;
				}
				bBombed=false;
				sVerb_Engine="stopping music";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				rsound.StopMusic();
				sVerb_Engine="clearing sounds";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				rsound.ClearAllInstances();
				rsound.PlayAsMusic("Level Start",false,1.0); //iPlayExplosion=128;
				sVerb_Engine="playing sounds";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				///TODO: force rsound.Iterate?? //PlaySounds();
				sVerb_Engine="rendering backdrop";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				if (bFirstRunOfCurrentState) bShowOnce_TargaToTarga32_ANY_Debug=true;
				if (animBackdrop.frame_ptr!=nullptr) animBackdrop.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, 0, 0, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
				else Console::Error.WriteLine("animBackdrop.frame_ptr nullptr for frame "+RString_ToString(animBackdrop.get_frame_number()));
				sVerb_Engine="rendering area "+RString_ToString(iArea-1)+" gamescreen";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				if (animGameScreenArea.frame_ptr!=nullptr) animGameScreenArea.frame_ptr->DrawToLargerWithoutCropElseCancel( screen_ptr,
						(int)( (float)screen_ptr->iWidth/2.0f-animGameScreenArea.Width()/2.0f ),
						(int)( (float)screen_ptr->iHeight/2.0f-animGameScreenArea.Height() ),DrawModeBlendAlpha );
				else Console::Error.WriteLine("animGameScreenArea.frame_ptr nullptr for frame "+RString_ToString(animGameScreenArea.get_frame_number()));
				sVerb_Engine="rendering encounter "+RString_ToString(iEncounter-1)+" gamescreen";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				if (animGameScreenEncounter.frame_ptr!=nullptr) animGameScreenEncounter.frame_ptr->DrawToLargerWithoutCropElseCancel( screen_ptr,
					(int)( (float)screen_ptr->iWidth/2.0f-animGameScreenEncounter.Width()/2.0f ),
					(int)( (float)screen_ptr->iHeight/2.0f ), DrawModeBlendAlpha );
				else Console::Error.WriteLine("animGameScreenEncounter.frame_ptr nullptr for frame "+RString_ToString(animGameScreenEncounter.get_frame_number()));
				sVerb_Engine="drawing backbuffer";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: "+sarrGameState[iGameStateWas]+" "+sVerb_Engine+"...");
				DrawScreen();
				sVerb_Engine="pausing to show gamescreen";
				if (!bDebug) SleepAllowingEvents(3000);
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
					if (iArea==1) rsound.PlayAsMusic("Invasion"); //VirtualMusicPlay(MusicFileName_Invasion,-1);
					else if (iArea==2) rsound.PlayAsMusic("Do You Want to Know");
					else if (iArea==3) rsound.PlayAsMusic("Would You Fight");
					else rsound.PlayAsMusic("Invasion");
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
					arrpentAlien[index]=new Entity(ENTITY_TYPE_ALIEN, "Flyer "+RString_ToString(index), 3.0f, (float)index-1.0f, HITDETECTION2D_CONSTANT_Z_DBL);//3.9f);
					(arrpentAlien[index])->iIndex=index;
					if (bFirstRunOfCurrentState) Console::Error.WriteLine(".");
					//arrpentAlien[index]->m3dEnt.X=screen_ptr->iWidth;
					//arrpentAlien[index]->m3dEnt.Y += index*(screen_ptr->iHeight/64);
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
				if (iEncounter==1&&iArea==1&&bFirstRunGameMain) {
					iTicksDisplayHelpMessage1=3000;
					iTickLastDisplayHelpMessage1=-1;
				}
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
				if (u32Pressing&GAMEKEY_UP2) {
						//hero->TurnPitchIncrement(true);
				}
				if (u32Pressing&GAMEKEY_DOWN2) {
						//hero->TurnPitchIncrement(false);
				}
				if (u32Pressing&GAMEKEY_LEFT2) {
					//hero->TurnIncrement(true);
					////if (hero->m3dEnt.yRot< -hero->zRotSpeed) hero->TurnPitchIncrement(true);
					////else if (hero->m3dEnt.yRot> hero->zRotSpeed) hero->TurnPitchIncrement(false);
					////else hero->TurnPitch(0.0f);//this is the destination for the whole "if" clause
				}
				if (u32Pressing&GAMEKEY_RIGHT2) {
					//hero->TurnIncrement(false);
					////if (hero->m3dEnt.yRot< -hero->zRotSpeed) hero->TurnPitchIncrement(true);
					////else if (hero->m3dEnt.yRot> hero->zRotSpeed) hero->TurnPitchIncrement(false);
					////else hero->TurnPitchIncrement(0.0f);//this is the destination for the whole "if" clause
				}
				if (u32Pressing&GAMEKEY_JUMP) { hero->Jump(); }
				if (!(u32Pressing&GAMEKEY_FIRE)) hero->iFramesShootDelay=0;
				if (u32Pressing&GAMEKEY_FIRE) hero->Shoot();

				if (u32Pressing&GAMEKEY_DOUBLESPEED) iDoubleCodeCounter++;//holding '2' activates STATUS_DOUBLESPEED
				else iDoubleCodeCounter=0;

				if (u32Pressing&GAMEKEY_RAPIDFIRE) iRapidFireCodeCounter++; //holding 'r' activates STATUS_RAPIDFIRE
				else iRapidFireCodeCounter=0;

				//if KEYUP(80/* p */) keyDelay=0;



				sVerb_Engine="processing powerups";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				if ( (iDoubleCodeCounter>=CODE_THRESHOLD) && (0==(hero->u32Status&STATUS_DOUBLESPEED) )) { //&& hero->u32Status^STATUS_DOUBLESPEED
						hero->SetDoubleSpeed();
						rsound.PlayAsMusicOnceThenResumePrevious("Victory Trumpets",1.0); //bPlayTrumpet=true;
				}
				if ( (iRapidFireCodeCounter>=CODE_THRESHOLD) && (0==(hero->u32Status&STATUS_RAPIDFIRE))) { //&& hero->u32Status ^ STATUS_RAPIDFIRE
						hero->SetRapidFire();
						rsound.PlayAsMusicOnceThenResumePrevious("Victory Trumpets",1.0); //bPlayTrumpet=true;
				}

				sVerb_Engine="rendering background";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				if (animBackdrop.frame_ptr!=nullptr) animBackdrop.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, 0, 0, DrawModeBlendAlpha);//clear backdrop (formerly ClearBackdrop())
				else Console::Error.WriteLine("animBackdrop.frame_ptr nullptr for frame "+RString_ToString(animBackdrop.get_frame_number()));
				//DrawRadarField();

				//Update all the existing arrpentAlien, delete the rest
				if (bFirstRunOfCurrentState) {
					Console::Error.Write("GameMain {bFirstRunOfCurrentState:true}: processing hero...");
					Console::Error.Flush();
				}
				if (hero!=nullptr) {
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
							ShowExn(exn,"GameMain(){state:"+sarrGameState[iGameState]+"}","getting boss vars");
						}
						catch (...) {
							ShowUnknownExn("GameMain(){state:"+sarrGameState[iGameState]+"}","getting boss vars");
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
					screen_ptr->DrawSubpixelArc(RadarCenterX() ,RadarCenterY(),
									RadarHeight()/2.0f-2.0f,1,0,
									0,360,
							pixelNow,
							.2,0);
					//screen_ptr->DrawExclusiveRect(hero->m2dEnt.rectRender.left, hero->m2dEnt.rectRender.top, hero->m2dEnt.rectRender.right, hero->m2dEnt.rectRender.bottom, 0xFFFFFFFF, false);
					if (bFirstRunOfCurrentState) {
						Console::Error.WriteLine("done processing hero.");
					}
				}
				int iNulls;
				iNulls=0;
				Entity* lpAlienNow=nullptr;
				sVerb_Engine="refreshing alien status";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				for (int iAlien=0; iAlien<iMaxAliensNow; iAlien++) {
					if (arrpentAlien[iAlien] != nullptr) {
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
							arrpentAlien[iAlien]=nullptr;
						}
					}
					else iNulls++;
				}
				sVerb_Engine="checking for hero fatality";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				if (hero->fExplodedness>=1.0f) {
					delete hero;
					hero=nullptr;
					if (iGuys<=0) iGameState=GAMESTATE_YOU_LOSE; //game over
					else iGameState=GAMESTATE_LOSEGUY; //hero death ("lost a guy")
				}
				sVerb_Engine="refreshing shot status";
				if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (arrpentShot[iShotNow] != nullptr) {
						(arrpentShot[iShotNow])->Refresh();
						(arrpentShot[iShotNow])->Draw();
						AddScreenItem(SCREENITEM_SHOT,ZORDER_FROMY((arrpentShot[iShotNow])->m3dEnt.Y),iShotNow);
						DrawRadarDot( (arrpentShot[iShotNow])->m3dEnt.X, (arrpentShot[iShotNow])->m3dEnt.Y, 1,
							(((arrpentShot[iShotNow])->bAlien)?0xFF800000:0xFFFFFFFF),false);//A,R,G,B //electric_blue:0xFF8800FF
						if (((arrpentShot[iShotNow])->u32Status&STATUS_ALIVE)==0) {
							delete arrpentShot[iShotNow];
							arrpentShot[iShotNow]=nullptr;
						}
					}
				}
				if (VisualDebugEnable) {
					sVerb_Engine="drawing movement boundary cube for debug";
					if (bFirstRunOfCurrentState) Console::Error.WriteLine("FirstRun: GAMESTATE_RUN "+sVerb_Engine+"...");
					Pixel pixelNear;
					Pixel pixelFar;
					pixelNear.Set(0,0,255,150);//Blue
					pixelFar.Set(0,0,128,100);//DarkBlue
					for (int iNow=0; iNow<9; iNow++) {
						camera.DrawBox(screen_ptr, m3darrDebug[iNow], pixelNear, pixelFar);
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
				string sLastScreenItem="(none yet)";
				while (iScreenItemsNow>0 && zNow<=MAX_ZORDER) {
					for (int iItem=0; iItem<iScreenItems; iItem++) {
						if (screenitemarr[iItem].zOrder==zNow) {
							try {
								switch (screenitemarr[iItem].iType) {
									case SCREENITEM_ALIEN:
										sLastScreenItem="Alien";
										if (nullptr!=arrpentAlien[ screenitemarr[iItem].index ]) {
											(arrpentAlien[ screenitemarr[iItem].index ])->Draw();
											if (VisualDebugEnable) camera.DrawAxes(screen_ptr,(arrpentAlien[ screenitemarr[iItem].index ])->m3dEnt);
										}
										if (VisualDebugEnable) {
											static Mass3d m3dAlienBottom;
											if (m3dAlienBottom.name!="AlienBottom") m3dAlienBottom.name="AlienBottom";
											arrpentAlien[ screenitemarr[iItem].index ]->m3dEnt.CopyTo(m3dAlienBottom);
											m3dAlienBottom.HardLocation(m3dAlienBottom.X,m3dAlienBottom.Y, 0.0f);
											camera.Draw3dRegularPolygon(	screen_ptr,	m3dAlienBottom,  'Z',	arrpentAlien[ screenitemarr[iItem].index ]->m3dEnt.zRot ,						.5f,		3,		pixelRed);
										}
										iScreenItemsNow--;
										break;
									case SCREENITEM_SHOT:
										sLastScreenItem="Shot";
										if (nullptr!=arrpentShot[ screenitemarr[iItem].index ]) (arrpentShot[ screenitemarr[iItem].index ])->Draw();
										iScreenItemsNow--;
										break;
									case SCREENITEM_HERO:
										sLastScreenItem="Hero";
										if (VisualDebugEnable) {
											static Mass3d m3dHeroBottom;
											if (m3dHeroBottom.name!="HeroBottom") m3dHeroBottom.name="HeroBottom";
											hero->m3dEnt.CopyTo(m3dHeroBottom); //debug: crashes accessing reference sometimes--how is this possible??
											m3dHeroBottom.HardLocation(m3dHeroBottom.X,m3dHeroBottom.Y, 0.0f);
											camera.Draw3dRegularPolygon(	screen_ptr,	m3dHeroBottom,  'Z',	hero->m3dEnt.zRot ,						.5f,		3,		pixelWhite);
										}
										if (nullptr!=hero) {
											hero->Draw();
											if (VisualDebugEnable) camera.DrawAxes(screen_ptr,hero->m3dEnt);
										}
										iScreenItemsNow--;
										break;
									case SCREENITEM_3DCURSOR:
										sLastScreenItem="3DCursor";
										Draw3dCursor(screenitemarr[iItem].index);
										iScreenItemsNow--;
										break;
									case SCREENITEM_CUBE:
										sLastScreenItem="Cube";
										camera.DrawBox(screen_ptr, screenitemarr[iItem].m3dCube,px3DCursorNear,px3DCursorFar);
										iScreenItemsNow--;
										break;
									default:
										break;
								}
							}
							catch (exception& exn) {
								ShowExn(exn,"GameMain","Drawing Screen Item:"+sLastScreenItem);
								iScreenItemsNow--;
							}
							if (bFirstRunOfCurrentState) Console::Error.Write("["+Convert::ToString(iItem)+"]"+sLastScreenItem+".zOrder:"+Convert::ToString(zNow)+"...");
						}//end if matching zOrder
					}//end for iItem
					zNow++;
				}
				if (VisualDebugEnable) {
					////int iLineSpacing=(int)( (float)gfontDefault.arranimGlyphType[GFont_GlyphTypePlain].Height()*1.5f+.5f );
					Mass3d m3dAxisCenter;
					m3dAxisCenter.name="AxisCenter";
					camera.DrawAxes(screen_ptr,m3dAxisCenter);//draw the origin
					////static IPoint ipDest;
					////int iStayLeft=32;
					////ipDest.Set(iStayLeft,32);
					debugtextcursor_Loc.X=0.0f;
					//gfontDefault.TypeFast(debugtextcursor_LastTextBlockSize,screen_ptr,debugtextcursor_Loc,"Cursor@ConstZ:"+RMath::LineRelationshipToString(iLastLineRelationshipType),0,DrawModeCopyAlpha,false);
					////ipDest.X=iStayLeft;
					////ipDest.Y+=iLineSpacing;
					//debugtextcursor_Loc.X=0;
					//debugtextcursor_Loc.Y+=debugtextcursor_LastTextBlockSize.Height;
					//gfontDefault.TypeFast(debugtextcursor_LastTextBlockSize,screen_ptr,debugtextcursor_Loc,RMath::LineRelationshipToString(iLastLineRelationshipType),0,DrawModeBlendAlpha,false);
					////ipDest.X=iStayLeft;
					////ipDest.Y+=iLineSpacing;
					debugtextcursor_Loc.X=0.0f;
					debugtextcursor_Loc.Y+=debugtextcursor_LastTextBlockSize.Height;
					//gfontDefault.TypeFast(debugtextcursor_LastTextBlockSize,screen_ptr,debugtextcursor_Loc,"Cursor@ConstZ Cursor:"+m3dCursor.ToString(true),0,DrawModeBlendAlpha,false);
				}
				iScreenItems=0;
				/*
				if(calculateExplosion(screen_ptr, 100, 100, 20, 0, EXPLOSION_CHECK_COUNT)==0)
				{
					explosionResult=calculateExplosion(screen_ptr, 100, 100, 20, 30, EXPLOSION_START);
				}
				else
					explosionResult=calculateExplosion(screen_ptr, 100, 100, 20, 30, EXPLOSION_CONTINUE);
				//Console::Error.WriteLine( "\nResult of explosionCalculate=" + explosionResult;
				*/
				/// Display help message ///
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
								ipDest.X=screen_ptr->iWidth/2;//this assumes the same amount of wrapping is needed for starting at center, but is fixed after first run
								ipDest.Y=0;
								gfontDefault.MeasureTextByRef_UsingTypeFast(sizeText, screen_ptr, ipDest, sMsg1, GFont_GlyphTypePlain);
							}
							else DisplayHelpMessage1_bMeasuredActual=true; //was wrapped starting at final X location (below) instead of only test location (above)
							ipDest.X=screen_ptr->iWidth/2-(sizeText.Width/2);
							ipDest.Y=screen_ptr->iHeight/2;
							iYStart=ipDest.Y;
							DisplayHelpMessage1_bFirstRun=false;
						}
						static int iSlideMS=750;
						if (iTicksDisplayHelpMessage1<iSlideMS) {
							ipDest.Y=iYStart-(int) ( (double)(iSlideMS-iTicksDisplayHelpMessage1)/(double)iSlideMS * ((double)screen_ptr->iHeight/2.0) );
						}
						gfontDefault.TypeFast(screen_ptr, ipDest, RememberTextCursorPosition, sMsg1,GFont_GlyphTypePlain,DrawModeBlendAlpha,DisplayHelpMessage1_bFirstRun,sizeText);
					}
				}

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
				Console::Error.WriteLine("WARNING: GAMESTATE_SHUTDOWN (this should never happen, because GameMain should not be called after SDL_EXIT event fires).");
			}//end GAMESTATE_SHUTDOWN
			else if (iGameState==GAMESTATE_WIN_ENCOUNTER) {

				rsound.StopMusic();

				if (settings.GetForcedBool("music")) {
					rsound.PlayAsMusic("Victory Trumpets",false,1.0); //VirtualMusicPlay("sound/trumpet.wav", 0);
				}


				for (int iX=0; iX<MAXSHOTS; iX++) {
					if (arrpentShot[iX]!=nullptr)
					{
						delete arrpentShot[iX];
						arrpentShot[iX]=nullptr;
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
					if (animGameScreen.frame_ptr!=nullptr) animGameScreen.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, (int)( (float)screen_ptr->iWidth/2.0f-animGameScreen.Width()/2.0f ), (int)( (float)screen_ptr->iHeight/2.0f-animGameScreen.Height()/2.0f ), DrawModeBlendAlpha);
					else Console::Error.WriteLine("animGameScreen.frame_ptr nullptr for frame "+RString_ToString(iScreenNow));
					DrawScreen();
					SleepAllowingEvents(5000);
				}
			}//end GAMESTATE_WIN_ENCOUNTER
			else if (iGameState==GAMESTATE_WIN_GAME) {
				int iScreenNow=1;
				if (iDoubleCodeCounter>=CODE_THRESHOLD || (hero&&hero->u32Status&STATUS_DOUBLESPEED)) {
					iScreenNow=7;
				}
				if (iRapidFireCodeCounter>=CODE_THRESHOLD|| (hero&&hero->u32Status&STATUS_RAPIDFIRE)) iScreenNow=8;

				animGameScreen.GotoFrame(iScreenNow); //Congratulations (type of ending depends on above)
				if (animGameScreen.frame_ptr!=nullptr) animGameScreen.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, (int)( (float)screen_ptr->iWidth/2.0f-animGameScreen.Width()/2.0f ), (int)( (float)screen_ptr->iHeight/2.0f-animGameScreen.Height()/2.0f ), DrawModeBlendAlpha);
				else Console::Error.WriteLine("animGameScreen.frame_ptr nullptr for frame "+RString_ToString(iScreenNow));
				DrawScreen();

				rsound.StopMusic();

				if (settings.GetForcedBool("music"),true) {
					rsound.PlayAsMusic("Ending"); //VirtualMusicPlay(MusicFileName_Ending,-1);
				}

				//SleepAllowingEvents(10000);//SleepAllowingEvents(20000);
				int iMinWait=7000;
				int iEndingWait=7250;
				int iEndingWaited=0;
				bool bExitAfterGameWin=false;
				animCursor.GotoFrame(1);
				/// Ending "Congratulations" ///
				while(!bExitAfterGameWin) { //mini polling loop FOUR OF FOUR! (show ending screen)
					bMouseMove=false;
					if (iEndingWaited>=iMinWait) animCursor.GotoFrame(2);
					while (SDL_PollEvent(&event)) {
						if (event.type==SDL_KEYDOWN) {
							//if (event.key.keysym.sym==SDLK_ESCAPE
							//	|| event.key.keysym.sym==SDLK_a
							//	|| event.key.keysym.sym==SDLK_s
							//	|| event.key.keysym.sym==SDLK_d
							//	|| event.key.keysym.sym==SDLK_w
							//	|| event.key.keysym.sym==SDLK_RETURN
							//	|| event.key.keysym.sym==SDLK_SPACE
							//	|| event.key.keysym.sym==SDLK_DELETE
							//	|| event.key.keysym.sym==SDLK_UP
							//	|| event.key.keysym.sym==SDLK_DOWN
							//	|| event.key.keysym.sym==SDLK_LEFT
							//	|| event.key.keysym.sym==SDLK_RIGHT ) {
							if (iEndingWaited>=iEndingWait) bExitAfterGameWin=true; //if (iEndingWaited>=iMinWait) bExitAfterGameWin=true;
							//}
						}
						else if (event.type==SDL_MOUSEBUTTONDOWN) {
							if (iEndingWaited>=iEndingWait) bExitAfterGameWin=true; //if (iEndingWaited>=iMinWait) bExitAfterGameWin=true;
						}
						else if (event.type==SDL_QUIT) {
							bExitAfterGameWin=true;
						}
						else if (event.type==SDL_MOUSEMOTION) {
							xCursor=event.motion.x;
							yCursor=event.motion.y;
							xfCursor=xCursor;
							yfCursor=yCursor;
							bMouseMove=true;
						}
					}
					if (animBackdrop.frame_ptr!=nullptr) animBackdrop.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, 0, 0, DrawModeBlendAlpha);
					if (animGameScreen.frame_ptr!=nullptr) animGameScreen.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, (int)( (float)screen_ptr->iWidth/2.0f-animGameScreen.Width()/2.0f ), (int)( (float)screen_ptr->iHeight/2.0f-animGameScreen.Height()/2.0f ), DrawModeBlendAlpha);
					animCursor.DrawToLargerWithoutCropElseCancel(screen_ptr, xCursor-xCursorCenter, yCursor-yCursorCenter, DrawModeBlendAlpha);
					DrawScreen();
					SleepWrapper(33);
					iEndingWaited+=33;
				}
				iGameState=GAMESTATE_EXIT;
			}
			else if (iGameState==GAMESTATE_YOU_LOSE) {
				iGuys=iGuysStart;
				rsound.StopMusic();
				if (settings.GetForcedBool("music")) {
					rsound.PlayAsMusic("Intro"); //VirtualMusicPlay(MusicFileName_Intro, -1);
				}
				animGameScreen.GotoFrame(0);
				if (animGameScreen.frame_ptr!=nullptr) animGameScreen.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, (int)( (float)screen_ptr->iWidth/2.0f-animGameScreen.Width()/2.0f ), (int)( (float)screen_ptr->iHeight/2.0f-animGameScreen.Height()/2.0f ), DrawModeBlendAlpha);
				else Console::Error.WriteLine("animGameScreen.frame_ptr nullptr for frame 0");
				DrawScreen();
				SleepAllowingEvents(5000);
				iGameState=GAMESTATE_INIT;
			}//end GAMESTATE_YOU_LOSE
			else if (iGameState==GAMESTATE_EXIT) {
				//TODO: finish this why isn't quit working
				//Console::Error.WriteLine("WARNING: GAMESTATE_EXIT (this should never happen, because GameMain should not be called after SDL_EXIT event fires).");
				//exit(0);
				bDone=true;//stops main event loop
				//try {
				//	if (!bSDLQuitDone) {
				//		bSDLQuitDone=true;
				//		SDL_Quit(); //this should never be called except by SafeQuit which is called by exit(0) (so call exit(0) to exit the program instead)
				//	}
				//	Console::Error.WriteLine("SDL_Quit...done");
				//}
				//catch (exception& exn) {
				//	ShowExn(exn,"GameMain(){state:EXIT}","trying to run SDL_Quit");
				//}
				//catch (...) {
				//	ShowUnknownExn("GameMain(){state:EXIT}","trying to run SDL_Quit");
				//}
				//if (!bDone) {
				//	Console::Error.WriteLine("GameMain(){state:EXIT}...done");
				//}
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

		/*
		//this is done in a more automated way further down
		Console::Error.Write("Initializing sound...");
		Console::Error.Out.Flush();
		float cam_coords[3];
		cam_coords[0] = camera.m3dCam.X;
		cam_coords[1] = camera.m3dCam.Y;
		cam_coords[1] = camera.m3dCam.Z;
		float cam_velocity[3];
		cam_velocity[0] = 0;
		cam_velocity[1] = 0;
		cam_velocity[2] = 0;
		float cam_orientation[6]; //"at" and "up" -- see OpenAL documentation
		cam_orientation[0] = 0;
		cam_orientation[1] = 1;
		cam_orientation[2] = 0;
		cam_orientation[3] = 0;
		cam_orientation[4] = 0;
		cam_orientation[5] = 1;
		rsound.Start(cam_orientation,cam_coords,cam_velocity);
		Console::Error.WriteLine("OK");
		*/

		Console::Error.Write("Initializing buffer...");
		Console::Error.Out.Flush();
		screen_ptr=new GBuffer(640,480,4,true);
		//screen_ptr->InitNull();
		//screen_ptr->Init(640, 480,4,true);//debug assumes 640x480
		screen_ptr->sFile="(screen).tga";
		Console::Error.WriteLine("done creating screen.");
		//if (!
		//) {
		//	sMsg="Failed to create screen sprite buffer";
		//	Console::Error.WriteLine("In "+__func__+" -- "+sMsg);
		//	iErrors++;
		//	//MessageBox (0, sMsg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
		//	bGood=false;
		//	return bGood;
		//}
		//else {
		Console::Error.Write("Loading debug points...");
		Console::Error.Flush();
		try {
			m3dCursor.xSize=.5f;
			m3dCursor.ySize=.5f;
			m3dCursor.zSize=.5f;
			px3DCursorNear.Set(255,0,0,255);
			px3DCursorFar.Set(255,0,0,128);
			for (int iNow=0; iNow<9; iNow++) {
				m3darrDebug[iNow].X=0;
				m3darrDebug[iNow].Y=0;
				m3darrDebug[iNow].Z=0;
				m3darrDebug[iNow].xSize=1.0f;
				m3darrDebug[iNow].ySize=1.0f;
				m3darrDebug[iNow].zSize=1.0f;
			}
			static float fTestRad=4.0f;
			static float fTestDiam=fTestRad*2.0f;
			m3darrDebug[0].X-=fTestRad; //see image 1.box-indeces.png
			m3darrDebug[0].Y+=fTestRad; //see image 1.box-indeces.png
			m3darrDebug[0].Z+=fTestDiam;
			m3darrDebug[1].X+=fTestRad;
			m3darrDebug[1].Y+=fTestRad;
			m3darrDebug[1].Z+=fTestDiam;
			m3darrDebug[2].X+=fTestRad;
			m3darrDebug[2].Y-=fTestRad;
			m3darrDebug[2].Z+=fTestDiam;
			m3darrDebug[3].X-=fTestRad;
			m3darrDebug[3].Y-=fTestRad;
			m3darrDebug[3].Z+=fTestDiam;
			m3darrDebug[4].X-=fTestRad; //see image 1.box-indeces.png
			m3darrDebug[4].Y+=fTestRad; //see image 1.box-indeces.png
			m3darrDebug[5].X+=fTestRad;
			m3darrDebug[5].Y+=fTestRad;
			m3darrDebug[6].X+=fTestRad;
			m3darrDebug[6].Y-=fTestRad;
			m3darrDebug[7].X-=fTestRad; //see image 1.box-indeces.png
			m3darrDebug[7].Y-=fTestRad; //see image 1.box-indeces.png
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
			//settings.CreateOrIgnore("aim.flip_pitch","yes");
			//bFlipPitch=settings.GetForcedBool("aim.flip_pitch");
			//settings.CreateOrIgnore("aim.flip_yaw","yes");
			//bFlipYaw=settings.GetForcedBool("aim.flip_yaw");
			settings.CreateOrIgnore("debugmode","no");
			//bDebug=true;
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



			//Console::Error.Write("Setting up arrays...");
			//Console::Error.Flush();
			//arrpentAlien=(Entity**)malloc(sizeof(Entity*)*iMaxAliens);
			string sMsg;
			//Console::Error.WriteLine("Done (iMaxAliens:"+RString_ToString(iMaxAliens)+")");
							/// Initialize SDL ///
			Console::Error.Write("Initializing video...");
			Console::Error.Flush();
			if (SDL_Init (SDL_INIT_VIDEO) < 0) {//if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
				sMsg="Couldn't initialize SDL:\n";
				sMsg+=RString_ToString(SDL_GetError());
				Console::Error.WriteLine("In "+RString_ToString(__func__)+" -- "+sMsg);
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //re-implement this w/o windows api
				bGood=false;
				return bGood;
			}
			Console::Error.WriteLine("done.");
			Console::Error.Write("Registering QuitSafe...");
			int good_if_zero=atexit(QuitSafe);
			Console::Error.WriteLine((good_if_zero==0)?"OK":"FAIL");
			Console::Error.Write("Initializing audio...");
			Console::Error.Flush();

			float cameraOrientation[6];
			float cameraPosition[3];
			float cameraVelocity[3];
			camera.m3dCam.CopyTo(cameraOrientation,cameraPosition,cameraVelocity,camera.make_orientation_Z_UP,camera.make_orientation_X_FORWARD);//camera should be toward positive Y for DXMan classic
			rsound.Start(cameraOrientation,cameraPosition,cameraVelocity);

			Console::Error.WriteLine("done.");

			/* Set 640x480 32-bits video mode */
			Console::Error.Write("Setting video mode...");
			Console::Error.Flush();
			//screen=SDL_SetVideoMode (screen_ptr->iWidth, screen_ptr->iHeight, SCREEN_BPP, SDL_SWSURFACE | SDL_DOUBLEBUF); //deprecated in SDL2
			sdlWindow = SDL_CreateWindow(MYNAME_AND_VERSION,
						  SDL_WINDOWPOS_UNDEFINED,
						  SDL_WINDOWPOS_UNDEFINED,
						  screen_ptr->iWidth, screen_ptr->iHeight,
						  SDL_WINDOW_OPENGL);//SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
			if (sdlWindow == nullptr) {
				sMsg="Couldn't set "+RString_ToString(screen_ptr->iWidth)+"x"+RString_ToString(screen_ptr->iHeight)+" video mode: \n";
				sMsg+=RString_ToString(SDL_GetError());
				Console::Error.WriteLine("In "+RString_ToString(__func__)+" -- "+sMsg);
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
				bGood=false;
				return bGood;
			}
			//SDL_WM_SetCaption ("DXMan", nullptr); //deprecated in SDL2
			Console::Error.WriteLine("done.");
			Console::Error.Write("Loading SDL renderer...");
			Console::Error.Flush();
			sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);//window, index, flags
			Console::Error.WriteLine("done.");

			Console::Error.Write("Creating SDL screen surface (screen)...");
			Console::Error.Flush();
			// "if all this hex scares you, check out SDL_PixelFormatEnumToMasks()!" -- http://wiki.libsdl.org/MigrationGuide#Moving_from_SDL_1.2_to_2.0
			//The four last params below are			RED mask,	GREEN mask,	BLUE mask,	ALPHA mask:
			screen = SDL_CreateRGBSurface(0, screen_ptr->iWidth, screen_ptr->iHeight, 32,
																0x00FF0000,	0x0000FF00,	0x000000FF,	0xFF000000);
			if (screen!=nullptr) {
				Console::Error.WriteLine("OK (little endian)");
			}
			else {
				Console::Error.WriteLine("trying big endian...");
				screen = SDL_CreateRGBSurface(0, screen_ptr->iWidth, screen_ptr->iHeight, 32,
																0x0000FF00,	0x00FF0000,	0xFF000000,	0x000000FF);
				if (screen!=nullptr) {
					Console::Error.WriteLine("OK (big endian)");
				}
				else {
					Console::Error.WriteLine("FAIL");
				}
			}

			Console::Error.Write("Creating SDL screen texture (sdlTexture)...");
			Console::Error.Flush();
			sdlTexture = SDL_CreateTexture(sdlRenderer,
																	  SDL_PIXELFORMAT_ARGB8888,
																	  SDL_TEXTUREACCESS_STREAMING,
																	  screen_ptr->iWidth, screen_ptr->iHeight);
			Console::Error.WriteLine("done.");

			Console::Error.Write("Loading splash screen...");
			Console::Error.Flush();
			GBuffer gbSplash;
			if (!gbSplash.Load("images/intro-splash.tga")) {
				sMsg="Failed to load splash screen";
				Console::Error.WriteLine("In "+RString_ToString(__func__)+" -- "+sMsg);
				iErrors++;
				//MessageBox (0, sMsg, "Error", MB_ICONHAND); //TODO: re-implement w/o windows api
			}
			bool bSplashDrawn=false;
			Console::Error.WriteLine("done.");
			iSplashTick=Base_GetTicks_Absolute();//use non-sdl for safety since SDL isn't necessarily initialized
			Console::Error.Write("Creating screen buffer...");
			Console::Error.Flush();
			//screen_ptr->DrawExclusiveRect(0,0,screen_ptr->iWidth,screen_ptr->iHeight,0xFFFFFFFF,true);
			screen_ptr->DrawExclusiveRect(0,0,screen_ptr->iWidth,screen_ptr->iHeight,0x00000000,true);
			if ( gbSplash.DrawToLargerWithoutCropElseCancel(screen_ptr, (int)( (float)screen_ptr->iWidth/2.0f-(float)gbSplash.iWidth/2.0f), (int)( (float)screen_ptr->iHeight/2.0f-(float)gbSplash.iHeight/2.0f ), DrawModeBlendAlpha) ) {
				bSplashDrawn=true;
			}
			//}
			Console::Error.WriteLine("done.");
			Console::Error.Write("Drawing screen (after attempting to draw splash)...");
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

			//Load alpha lookup table from file
			//int file_handle;

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

			int iCount=0;
			int iTotal=256+200+500;
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
							by3dAlphaLookup[iSource][iDest][iAlpha]=(byte)((source-dest)*alpha/255.0f+dest+.5f); //+.5f for rounding
						}
					}
					screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true);
					iCount++;
					DrawScreen();
					DoEvents();
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
			//bTest=gfontDefault.FromImageAsPredefinedColorFont(sFontFile_RelName, iCharW, iCharH, 16, 16);//.FromImageValue("images/font.tga", 16, 24, 16, 16);
			bTest=gfontDefault.FromImageValue("images/ascii256-dos-fixed.tga", 7, 12, 16, 16);
			Console::Error.Write(bTest?"gfontDefault.FromImageAsPredefinedColorFont...OK":"gfontDefault.FromImageAsPredefinedColorFont...FAILED");
			Console::Error.Flush();

			iCount+=200;
			screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true);
			DrawScreen();
			DoEvents();

			Console::Error.Write("Loading image files...");
			Console::Error.Flush();

			for (int shotsNow=0; shotsNow<MAXSHOTS; shotsNow++)
				arrpentShot[shotsNow]=nullptr;
			for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++)
				arrpentAlien[iAliensNow]=nullptr;

			// this is called once after the initial window is created and
			// before the main event loop is entered, do all your initialization
			// here
			//lpspritearr=(LPSPRITE*)malloc(MAX_SPRITES*sizeof(LPSPRITE));
			//memset(lpspritearr,0,MAX_SPRITES*sizeof(LPSPRITE));

			//RECT explosionRect={0,0,200,200};
			//IRECT screenRect= {0,0,screen_ptr->iWidth,screen_ptr->iHeight};
			REAL_BPP=32;
			REAL_BYTEDEPTH=4;

			///#region 500 iCount (progress)
			LoadImage(gbIntro, "images/intro-title.tga");
			iCount+=10; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadImage(gbIntroTextBack, "images/intro-text-back.tga");
			iCount+=10; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadImage(gbIntroText, "images/intro-text.tga");
			iCount+=10; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadImage(shield_symbol_image_ptr, "images/symbol-shield.tga");
			iCount+=10; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadImage(boss_health_symbol_image_ptr, "images/symbol-bosshealth.tga");
			iCount+=10; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadImage(gbLives, "images/lives.tga");
			iCount+=10; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animBackdrop, "images/area", iBackdrops);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animHero, "images/hero", iFramesHero);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animHeroShadow, "images/hero-shad", iFramesHeroShadow);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animFlyer, "images/flyer", iFramesFlyer);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animFlyerShadow, "images/flyer-shad", iFramesFlyerShadow);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animBoss, "images/boss", iFramesBoss);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animBossShadow, "images/boss-shad", iFramesBossShadow);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animShot, "images/shot", iFramesShot);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animShotShadow, "images/shot-shad", iFramesShotShadow);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animBurn, "images/burn", iFramesBurn);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animExpArt, "images/expart", iFramesExpArt);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animGameScreen, "images/gamescreen", iFramesGameScreen);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animGameScreenArea, "images/gamescreenarea", iAreas);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animGameScreenEncounter, "images/gamescreenencounter", iEncounters);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			LoadSequence(animCursor, "images/cursor", iFramesCursor);
			iCount+=30; screen_ptr->DrawExclusiveRect(0,screen_ptr->iHeight-8,(int)(((double)iCount/(double)iTotal)*(double)screen_ptr->iWidth),screen_ptr->iHeight-4,0xFF8800FF,true); DrawScreen(); DoEvents();
			//#endregion 500 iCount (progress)
			settings.bSaveEveryChange=true;
			dat.bSaveEveryChange=true;
			Console::Error.WriteLine("done (end of "+RString_ToString(__func__)+").");
		}//end try
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,__func__);
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn(__func__);
		}
		return(bGood);
	} // end GameInit

	const int OUTPUT_NOCONSOLE=0;
	const int OUTPUT_WRITE=1;
	const int OUTPUT_WRITELINE=1;
	bool LastStatusWasWrite=false;
	const int OUTPUTMAXSTRINGS=300;
	StringQ linesQ(512);

	void ShowLineQueue() {
		int linesQ_LastIndex=linesQ.GetLastIndex();
		int yPixel=0;
		bool RememberStatusCursorPosition=false;
		for (int index=0; index<=linesQ_LastIndex; index++) {
				statuscursor_Loc.X=0;
				statuscursor_Loc.Y=0;
				gfontDefault.TypeFast(screen_ptr, statuscursor_Loc, RememberStatusCursorPosition, linesQ.Peek(index),0,DrawModeBlendAlpha, false, statuscursor_LastTextBlockSize);
				statuscursor_Loc.Y+=(int)statuscursor_LastTextBlockSize.Height;
				if (statuscursor_Loc.Y+statuscursor_LastTextBlockSize.Height>=screen_ptr->iHeight) {
					linesQ.DeqVoid();
				}
		}
	}

	void pushStatus(string msg, bool IsQueueToBeShown, int OUTPUT) {
		sVerb_Engine=msg;

		if (LastStatusWasWrite) {
			linesQ.PokeLast(linesQ.PeekLast()+msg+"...");
		}
		else {
			linesQ.Enq(sVerb_Engine+"...");
		}

		if (OUTPUT==OUTPUT_WRITELINE) {
			Console::Error.WriteLine(msg+"...");
			LastStatusWasWrite=false;
		}
		else if (OUTPUT==OUTPUT_WRITE) {
			Console::Error.Write(msg+"...");
			Console::Error.Flush();
			LastStatusWasWrite=true;
		}
		if (IsQueueToBeShown) ShowLineQueue();
	}
	/////////////////////////////////////////////////////////////
	bool GameShutdown() {
		bool bGood=false;

		if (!bShuttingDown) {
			bShuttingDown=true;
			bGood=true;
			Console::Error.WriteLine();
			Console::Error.WriteLine();
			Console::Error.WriteLine();
			Console::Error.WriteLine("GameShutdown...");
			//This is called after the game is exited and the main event
			// loop while is exited, do all you cleanup and shutdown here
			if (bMegaDebug) Console::Error.WriteLine("checking whether game was already shut down...");
			//Release palette if 8-bit was used
			//if (lpddpal){lpddpal->Release(); lpddpal=nullptr;} //
			if (!bDoneShutdown) {
				try {
					if (SDL_ShowCursor(SDL_QUERY)==SDL_DISABLE) {
						SDL_ShowCursor(SDL_ENABLE);
					}
					pushStatus("shutting down game",true,OUTPUT_WRITELINE);
					pushStatus("disposing sound system...",true,OUTPUT_WRITELINE);
					rsound.Dispose();
					pushStatus("freeing flyer array...",true,OUTPUT_WRITE);
					//if (bDebug)
					Console::Error.WriteLine();
					//if (arrpentAlien!=nullptr) {
						try {
							pushStatus("found "+RString_ToString(iMaxAliens)+" aliens in "+RString_ToString((int)arrpentAlien.capacity())+"-long array...",true,OUTPUT_WRITE);
							for (int iAliensNow=0; iAliensNow<iMaxAliensNow; iAliensNow++) {
								if (arrpentAlien[iAliensNow]!=nullptr) {
									if (iAliensNow==iBoss) iBoss=-1;
									//if (bDebug) {
									Console::Error.WriteLine("free flyer...DELETING arrpentAlien["+RString_ToString(iAliensNow)+"].m3dEnt={");
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
									//}
									delete arrpentAlien[iAliensNow];
									arrpentAlien[iAliensNow]=nullptr;
								}
								else {
									//if (bDebug) {
									Console::Error.WriteLine("free flyer...nothing to do for arrpentAlien["+RString_ToString(iAliensNow)+"]=nullptr");
									//}
								}
							}
							Console::Error.WriteLine("freeing flyer pointer array...");
							//free(arrpentAlien); //debug: was crashing when was ** (now is std::vector)
							//arrpentAlien = nullptr;
						}
						catch (exception& exn) {
							bGood=false;
							ShowExn(exn,"GameShutdown","deleting flyers");
						}
						catch (...) {
							bGood=false;
							ShowUnknownExn("GameShutdown","deleting flyers");
						}
						//SafeFree(arrpentAlien);
						Console::Error.WriteLine("OK (freed flyer array)");
					//}
					//else Console::Error.WriteLine("NOTHING TO DO for freeing alien array.");
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
		else {
			Console::Error.WriteLine("GameShutdown: Already called GameShutdown!");
		}
		return(bGood);
	} // end GameShutdown
	void SleepWrapper(int iTicks) {
		SDL_Delay(iTicks);
	}
	void LoadSequence(Anim &animToLoad, string sFileBaseNameOnly, int iFramesExpectedElseError) {
		int iFramesFound;
		bool bGood=animToLoad.LoadSeq(sFileBaseNameOnly+"*.tga");
		if (!bGood)	ShowError(sFileBaseNameOnly+"*.tga failed to load",__func__);
		iFramesFound=animToLoad.IFrames();
		if (bGood && iFramesFound!=iFramesExpectedElseError) {
			bGood=false;
			ShowError("not all "+sFileBaseNameOnly+RString_ToString("*.tga frames loaded."));
		}
	}
	void LoadImage(GBuffer* &gbToLoad, string sFile) {
		Console::Error.Write("LoadImage ** "+sFile+"...new GBuffer...");
		Console::Error.Out.Flush();
		if (gbToLoad==nullptr) gbToLoad=new GBuffer();
		Console::Error.Write("Load...");
		Console::Error.Out.Flush();
		bool bGood=gbToLoad->Load(sFile);
		if (!bGood)	ShowError(sFile+".tga failed to load","GameInit");
		else Console::Error.WriteLine("OK");
	}
	void LoadImage(GBuffer &gbToLoad, string sFile) {
		Console::Error.Write("LoadImage & "+sFile+"...new GBuffer...");
		Console::Error.Out.Flush();
		bool bGood=gbToLoad.Load(sFile);
		if (!bGood)	ShowError(sFile+".tga failed to load","GameInit");
		else Console::Error.WriteLine("OK");
	}
	void MoveSoundEmitter(string EmitterName, Mass3d& thisMass3d) {
		float orientationVector6[6];
		float positionVector3[3];
		float velocityVector3[3];
		thisMass3d.CopyTo(orientationVector6,positionVector3,velocityVector3,camera.make_orientation_Z_UP,camera.make_orientation_Z_UP);
		rsound.MoveEmitter(EmitterName,positionVector3,velocityVector3);
	}
	void DirKeyDown() {
		 if ((hero!=nullptr&&hero->m3dEnt.Z<=hero->m3dEnt.zMin) && (hero->u32Status & STATUS_ANTIGRAVITY)) {
			if (iDirKeysDown<=0) {
				if (bMegaDebug) {
					Console::Error.Write("DirKeyDown{iDirKeysDown<=0; action:thruster} Mix_PlayChannel...");
					Console::Error.Flush();
				}
				PlaySound("Thruster","hero",hero->m3dEnt,true); //iChanThruster=Mix_PlayChannel(-1, mcThruster, -1);//chan, sound, #loops
				//iReturn=Mix_FadeInChannelTimed(iChan, mcThruster,
				//			iLoops, iMillisecondsFadeIn, iTicksDuration)
				if (bMegaDebug) {
					Console::Error.WriteLine("done...");
					Console::Error.Flush();
				}
			}
			MoveSoundEmitter("hero",hero->m3dEnt);//UpdateThrustPan();
		 }
		 iDirKeysDown++;
	}//end DirKeyDown
	void DirKeyUp() {
		 iDirKeysDown--;
		 if (iDirKeysDown<=0) {
			if (bMegaDebug) {
				Console::Error.Write("DirKeyUp{iDirKeysDown<=0; action:thruster} Mix_PlayChannel...");
				Console::Error.Flush();
			}
			StopSoundInstance("Thruster","hero");
			if (bMegaDebug) {
				Console::Error.Write("done...");
				Console::Error.Flush();
			}
			iDirKeysDown=0;
		 }
	}
	///////////////////////////////////////////////////////////////
	bool addScreenItemTypeOrderEntityIndex_ErrorWasShown=false;
	bool AddScreenItem(int iType, int zOrder, int iEntityIndex) {
		bool bGood=false;
		if (iScreenItems<MAX_SCREENITEMS) {
			screenitemarr[iScreenItems].zOrder=zOrder;
			screenitemarr[iScreenItems].index=iEntityIndex;
			screenitemarr[iScreenItems].iType=iType;
			iScreenItems++;
			bGood=true;
		}
		else {
			if (!addScreenItemTypeOrderEntityIndex_ErrorWasShown) {//if (bDebug) {
				addScreenItemTypeOrderEntityIndex_ErrorWasShown=true;
				ShowErr("Beyond maximum screen items when adding:"+ScreenItemToString(iType));
			}
		}
		return bGood;
	}
	///////////////////////////////////////////////////////////////
	bool addScreenItemTypeOrderEntityIndexM3d_ErrorWasShown=false;
	bool AddScreenItem(int iType, int zOrder, int iEntityIndex, Mass3d& m3dNow) {
		bool bGood=false;
		if (iScreenItems<MAX_SCREENITEMS) {
			screenitemarr[iScreenItems].zOrder=zOrder;
			screenitemarr[iScreenItems].index=iEntityIndex;
			screenitemarr[iScreenItems].iType=iType;
			m3dNow.CopyTo(screenitemarr[iScreenItems].m3dCube);
			iScreenItems++;
			bGood=true;
		}
		else {
			if (!addScreenItemTypeOrderEntityIndexM3d_ErrorWasShown) { //if (bDebug) {
				ShowErr("Beyond maximum screen items when adding:"+ScreenItemToString(iType));
				addScreenItemTypeOrderEntityIndexM3d_ErrorWasShown=true;
			}
		}
		return bGood;
	}
	string ScreenItemToString(int iType) {
		string sReturn="(not yet analyzed)";
		try {
			if (iType>=0&&iType<=iScreenItemTypes) {
				sReturn=sarrScreenItem_DONTUSEMEDIRECTLY[iType];
			}
			else {
				sReturn="(out-of-range screenitem type "+Convert::ToString(iType)+")";
			}
		}
		catch (exception& exn) {
			ShowExn(exn,"ScreenItemToString");
		}
		return sReturn;
	}//end ScreenItemToString
	///////////////////////////////////////////////////////////////
	void QuitSafe() {
		Console::Error.Write("Running QuitSafe...");
		Console::Error.Flush();
		if (!bShuttingDown) GameShutdown();
		if (!bSDLQuitDone) {
			bSDLQuitDone=true;
			SDL_Quit();//this is required--cleans up SDL window.  In some situations, SDL_Quit can be the atexit parameter.
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
			int this_life_y=(int)(zPos);
			int left=xCenter-xOffset, right=xCenter+xOffset, top=yCenter-this_life_y, bottom=yCenter+this_life_y;
			screen_ptr->DrawExclusiveRect(left, top, right+1, bottom+1, u32Pixel, bFilled);
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
			screen_ptr->DrawExclusiveRect(iLeft, iTop, iRight+1, iBottom+1, u32Pixel, bFilled);
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
		static int xCenter=screen_ptr->iWidth-(int)(FRADAR_XRAD+6.0f);
		static int yCenter=(int)((float)FRADAR_YRAD+6.0f);
		static int xOffset=(int)(FRADAR_ZOOM*FRADAR_XRAD);
		static int this_life_y=(int)(FRADAR_ZOOM*FRADAR_YRAD);
		static int left=xCenter-xOffset, right=xCenter+xOffset, top=yCenter-this_life_y, bottom=yCenter+this_life_y;
		screen_ptr->DrawExclusiveRect(left, top, right, bottom, u32Pixel, false);
	}//end DrawRadarField

/*
	void ShowDebugVars() {//debug doesn't work, isn't used
		if (!bTTFStarted) {
			if (TTF_Init() == -1) { //Initialize SDL_ttf and if it fails, report the error.
				//printf("Unable to initialize SDL_ttf: ", TTF_GetError(), endl);
				bTTFError=true;
			}
	 		font=TTF_OpenFont("arial.ttf", 8);
			if (font==nullptr) {
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
			int sx=0,sy=0,sw=screen_ptr->iWidth,sh=screen_ptr->iHeight/6,dx=0,dy=0,alpha=255;
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
	string GetOrCreateSoundInstanceName(string SoundName, string EmitterName) {
		return EmitterName+"."+SoundName; //debug note: forces only one instance of a given sound to play from a source
	}
	void PlaySound(string SoundName, string EmitterName) {
		static Mass3d m3dTemp;
		if (m3dTemp.name!="PlaySound Temp") m3dTemp.name="PlaySound Temp";
		camera.m3dCam.CopyTo(m3dTemp);
		m3dTemp.HardRotation(m3dTemp.X,m3dTemp.Y,RMath::SafeAngle360(m3dTemp.Z+180.0));
		Travel3d(m3dTemp.X,m3dTemp.Y,m3dTemp.Z,camera.m3dCam.yRot,camera.m3dCam.zRot,1.0f,camera.make_orientation_Z_UP,camera.make_orientation_X_FORWARD); //play the sound 1.0 in front of the camera
		//now make it relative:
		m3dTemp.X-=camera.m3dCam.X;
		m3dTemp.Y-=camera.m3dCam.Y;
		m3dTemp.Z-=camera.m3dCam.Z;
		PlaySound(SoundName,EmitterName,m3dTemp);
	}
	void PlaySound(string SoundName, string EmitterName, Mass3d& thisMass3d) {
		PlaySound(SoundName,EmitterName,thisMass3d,false);
	}
	void PlaySound(string SoundName, string EmitterName, Mass3d& thisMass3d, bool IsLooped) {
		//static Mass3d m3dTemp;
		//m3dTemp.name="PlaySound Temp";
		//m3dLoc.CopyTo(m3dTemp);
		//Travel3d(&m3dTemp.X,&m3dTemp.Y,&m3dTemp.Z,m3dLoc.yRot,m3dLoc.zRot,1.0f,camera.make_orientation_Z_UP,camera.make_orientation_X_FORWARD); //move "velocity" point 1.0 in direction of rotation (so has a direction even if xVel,yVel,zVel are all 0)
		//now make it relative:
		//m3dTemp.X-=m3dLoc.X;
		//m3dTemp.Y-=m3dLoc.Y;
		//m3dTemp.Z-=m3dLoc.Z;
		float orientationVector6[6];
		float positionVector3[3];
		float velocityVector3[3];
		thisMass3d.CopyTo(orientationVector6,positionVector3,velocityVector3,camera.make_orientation_Z_UP,camera.make_orientation_Z_UP);
		rsound.PlayAsSound(SoundName,EmitterName,GetOrCreateSoundInstanceName(SoundName,EmitterName),positionVector3,velocityVector3,IsLooped);
	}
	//void PlaySound(string SoundName, string EmitterName, Mass3d& m3dLoc, Mass3d& m3dVel) {
	//	rsound.PlayAsSound(SoundName,EmitterName,m3dLoc.X,m3dLoc.Y, m3dLoc.Z, m3dVel.X, m3dVel.Y, m3dVel.Z);
	//}
	void StopSoundInstance(string SoundName, string EmitterName) {
		rsound.StopInstance(GetOrCreateSoundInstanceName(SoundName,EmitterName));
	}

	bool bFirstRunDrawPointWithZOffsetIndicator=true;
	////////////////////////////////////////////////////////////////////
	void DrawPointWithZOffsetIndicator(Mass3d m3dNow, string sDebugMessage) {
		bool bGood=false;
		try {
			Mass3d m3dTemp;
			m3dTemp.name="DrawPointWithZOffsetIndicator Temp";
			m3dNow.CopyTo(m3dTemp);
			FPoint fp2dOf3dNow_z0;
			FPoint fp2dOf3dNow;
			if (bFirstRunRefresh3dCursor) {
				Console::Error.Write("camera.Point2dFrom3d(fp2dOf3dNow,"+sDebugMessage+"=("+m3dTemp.ToString()+"));...");
				Console::Error.Flush();
			}
			camera.Point2dFrom3d(fp2dOf3dNow,m3dTemp);
			m3dTemp.Z=0.0f;
			if (bFirstRunRefresh3dCursor) {
				Console::Error.Write("camera.Point2dFrom3d(...,"+sDebugMessage+"=("+m3dTemp.ToString()+"))...");
				Console::Error.Flush();
			}
			camera.Point2dFrom3d(fp2dOf3dNow_z0,m3dTemp);
			screen_ptr->SetBrushColor(0,0,0,255);
			if (fp2dOf3dNow.Y<1) fp2dOf3dNow.Y=1;//1 pixel away from edge since using 3x3 block for dest
			else if (fp2dOf3dNow.Y>(screen_ptr->iHeight-2)) fp2dOf3dNow.Y=screen_ptr->iHeight-2;//1 pixel away from edge since using 3x3 block for dest
			if (fp2dOf3dNow.X<1) fp2dOf3dNow.X=1;//1 pixel away from edge since using 3x3 block for dest
			else if (fp2dOf3dNow.X>(screen_ptr->iWidth-2)) fp2dOf3dNow.X=screen_ptr->iWidth-2;//1 pixel away from edge since using 3x3 block for dest

			if (fp2dOf3dNow_z0.Y<1) fp2dOf3dNow_z0.Y=1;//1 pixel away from edge since using 3x3 block for dest
			else if (fp2dOf3dNow_z0.Y>(screen_ptr->iHeight-2)) fp2dOf3dNow_z0.Y=screen_ptr->iHeight-2;//1 pixel away from edge since using 3x3 block for dest
			if (fp2dOf3dNow_z0.X<1) fp2dOf3dNow_z0.X=1;//1 pixel away from edge since using 3x3 block for dest
			else if (fp2dOf3dNow_z0.X>(screen_ptr->iWidth-2)) fp2dOf3dNow_z0.X=screen_ptr->iWidth-2;//1 pixel away from edge since using 3x3 block for dest
			if (bFirstRunRefresh3dCursor) {
				Console::Error.Write("DrawHorzLine for z diff (3x3 block for dest) at fp2dOf3dNow:"+fp2dOf3dNow.ToString()+"...");
				Console::Error.Flush();
			}
			screen_ptr->DrawHorzLine(fp2dOf3dNow.X-1,fp2dOf3dNow.Y-1,3);
			screen_ptr->DrawHorzLine(fp2dOf3dNow.X-1,fp2dOf3dNow.Y,3);
			screen_ptr->DrawHorzLine(fp2dOf3dNow.X-1,fp2dOf3dNow.Y+1,3);
			if (bFirstRunRefresh3dCursor) {
				Console::Error.Write("DrawVertLine for z diff from fp2dOf3dNow_z0:"+fp2dOf3dNow_z0.ToString()+"...");
				Console::Error.Flush();
			}
			if (fp2dOf3dNow_z0.Y<fp2dOf3dNow.Y) screen_ptr->DrawVertLine(fp2dOf3dNow_z0.X,fp2dOf3dNow_z0.Y,fp2dOf3dNow.Y-fp2dOf3dNow_z0.Y);
			else screen_ptr->DrawVertLine(fp2dOf3dNow.X,fp2dOf3dNow.Y,fp2dOf3dNow_z0.Y-fp2dOf3dNow.Y);
			if (VisualDebugEnable) {
				debugtextcursor_Loc.X=0.0f;
				gfontDefault.TypeFast(screen_ptr, debugtextcursor_Loc, RememberTextCursorPosition, "["+sDebugMessage+"]fp2dOf3dNow_z0:"+fp2dOf3dNow_z0.ToString(),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
				debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
				gfontDefault.TypeFast(screen_ptr, debugtextcursor_Loc, RememberTextCursorPosition, "["+sDebugMessage+"]fp2dOf3dNow:"+fp2dOf3dNow.ToString(),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
				IPoint ipAt;
				ipAt.X=fp2dOf3dNow.X;
				ipAt.Y=fp2dOf3dNow.Y;
				debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
				gfontDefault.TypeFast(screen_ptr,ipAt,RememberTextCursorPosition, sDebugMessage+":"+m3dNow.ToString());
			}
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"DrawPointWithZOffsetIndicator");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("DrawPointWithZOffsetIndicator");
		}
		bFirstRunRefresh3dCursor=false;
	}//end DrawPointWithZOffsetIndicator
	void DrawPointWithGroundSquareIndicator(Mass3d m3dNow, Pixel color, string sDebugMessage) {
		bool bGood=false;
		try {
			Mass3d m3dTemp;
			m3dTemp.name="DrawPointWithGroundSquareIndicator Temp";
			m3dNow.CopyTo(m3dTemp);
			FPoint fp2dOf3dNow_z0;
			FPoint fp2dOf3dNow;
			if (bFirstRunRefresh3dCursor) {
				Console::Error.Write("camera.Point2dFrom3d(fp2dOf3dNow,"+sDebugMessage+"=("+m3dTemp.ToString()+"));...");
				Console::Error.Flush();
			}
			camera.Point2dFrom3d(fp2dOf3dNow,m3dTemp);
			m3dTemp.Z=0.0f;
			if (bFirstRunRefresh3dCursor) {
				Console::Error.Write("camera.Point2dFrom3d(...,"+sDebugMessage+"=("+m3dTemp.ToString()+"))...");
				Console::Error.Flush();
			}
			camera.Point2dFrom3d(fp2dOf3dNow_z0,m3dTemp);
			screen_ptr->SetBrushColor(color.r,color.g,color.b,255);
			/*
			if (fp2dOf3dNow.Y<1) fp2dOf3dNow.Y=1;//1 pixel away from edge since using 3x3 block for dest
			else if (fp2dOf3dNow.Y>(screen_ptr->iHeight-2)) fp2dOf3dNow.Y=screen_ptr->iHeight-2;//1 pixel away from edge since using 3x3 block for dest
			if (fp2dOf3dNow.X<1) fp2dOf3dNow.X=1;//1 pixel away from edge since using 3x3 block for dest
			else if (fp2dOf3dNow.X>(screen_ptr->iWidth-2)) fp2dOf3dNow.X=screen_ptr->iWidth-2;//1 pixel away from edge since using 3x3 block for dest

			if (fp2dOf3dNow_z0.Y<1) fp2dOf3dNow_z0.Y=1;//1 pixel away from edge since using 3x3 block for dest
			else if (fp2dOf3dNow_z0.Y>(screen_ptr->iHeight-2)) fp2dOf3dNow_z0.Y=screen_ptr->iHeight-2;//1 pixel away from edge since using 3x3 block for dest
			if (fp2dOf3dNow_z0.X<1) fp2dOf3dNow_z0.X=1;//1 pixel away from edge since using 3x3 block for dest
			else if (fp2dOf3dNow_z0.X>(screen_ptr->iWidth-2)) fp2dOf3dNow_z0.X=screen_ptr->iWidth-2;//1 pixel away from edge since using 3x3 block for dest

			if (bFirstRunRefresh3dCursor) {
				Console::Error.Write("DrawHorzLine for z diff (3x3 block for dest) at fp2dOf3dNow:"+fp2dOf3dNow.ToString()+"...");
				Console::Error.Flush();
			}
			screen_ptr->DrawHorzLine(fp2dOf3dNow.X-1,fp2dOf3dNow.Y-1,3);
			screen_ptr->DrawHorzLine(fp2dOf3dNow.X-1,fp2dOf3dNow.Y,3);
			screen_ptr->DrawHorzLine(fp2dOf3dNow.X-1,fp2dOf3dNow.Y+1,3);
			if (bFirstRunRefresh3dCursor) {
				Console::Error.Write("DrawVertLine for z diff from fp2dOf3dNow_z0:"+fp2dOf3dNow_z0.ToString()+"...");
				Console::Error.Flush();
			}
			*/
			//if (fp2dOf3dNow_z0.Y<fp2dOf3dNow.Y) screen_ptr->DrawVertLine(fp2dOf3dNow_z0.X,fp2dOf3dNow_z0.Y,fp2dOf3dNow.Y-fp2dOf3dNow_z0.Y);
			//else screen_ptr->DrawVertLine(fp2dOf3dNow.X,fp2dOf3dNow.Y,fp2dOf3dNow_z0.Y-fp2dOf3dNow.Y);
			///#region draw 3d cursor target polygon on ground
			static Mass3d m3dCenter;
			if (m3dCenter.name!="DrawPointWithGroundSquareIndicator Center") m3dCenter.name="DrawPointWithGroundSquareIndicator Center";
			m3dCenter.HardLocation(m3dNow.X,m3dNow.Y,m3dNow.Z);//m3dCenter.CopyTo(m3d0);
			m3dCenter.Z=0.0f;
			///camera.Draw3dRegularPolygon(gbDest, 	m3dCenter,	Axis,	StartAngle_Radians,	Radius,	Sides,	color)
			Pixel pixelPartialOpacity(color);
			pixelPartialOpacity.a=64;
			camera.Draw3dRegularPolygon(	screen_ptr,	m3dCenter,  'Z',	0.0f,						0.5f,		32,		color);
		 //screen_ptr->DrawSubpixelArc()
			///#endregion draw 3d cursor target polygon on ground
			if (VisualDebugEnable) {
				debugtextcursor_Loc.X=0.0f;
				gfontDefault.TypeFast(screen_ptr, debugtextcursor_Loc, RememberTextCursorPosition, "["+sDebugMessage+"]fp2dOf3dNow_z0:"+fp2dOf3dNow_z0.ToString(),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
				debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
				gfontDefault.TypeFast(screen_ptr, debugtextcursor_Loc, RememberTextCursorPosition, "["+sDebugMessage+"]fp2dOf3dNow:"+fp2dOf3dNow.ToString(),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
				IPoint ipAt;
				ipAt.X=fp2dOf3dNow.X;
				ipAt.Y=fp2dOf3dNow.Y;
				debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
				gfontDefault.TypeFast(screen_ptr,ipAt,RememberTextCursorPosition, sDebugMessage+":"+m3dNow.ToString());
			}
			bGood=true;
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"DrawPointWithZOffsetIndicator");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("DrawPointWithZOffsetIndicator");
		}
		bFirstRunRefresh3dCursor=false;
	}//end DrawPointWithGroundSquareIndicator

	////////////////////////////////////////////////////////////////////

	void Refresh3dCursor() {
		bool bGood=false;
		//if (bDebug) ExpertMultimediaBase::bMegaDebug=true;//debug only
		try {
			if (hero!=nullptr) {
				float fAltitude, fAzimuth,
					xCursorMin=(float)screen_ptr->iWidth*.1f, yCursorMin=(float)screen_ptr->iHeight*.1f,
					xCursorMax=(float)screen_ptr->iWidth-xCursorMin, yCursorMax=(float)screen_ptr->iHeight-yCursorMin,
					xCursorRange=xCursorMax-xCursorMin, yCursorRange=yCursorMax-yCursorMin,
					xCursorness, yCursorness,
					fAimerDistance=3;
				xCursorness=( ((xfCursor<xCursorMin)?xCursorMin:((xfCursor>xCursorMax)?xCursorMax:xfCursor)) - xCursorMin )  /  xCursorRange;
				yCursorness=( ((yfCursor<yCursorMin)?yCursorMin:((yfCursor>yCursorMax)?yCursorMax:yfCursor)) - yCursorMin )  /  yCursorRange;
				//if (bFlipYaw) xCursorness=1.0f-xCursorness;
				//if (bFlipPitch) yCursorness=1.0f-yCursorness;
				//fAzimuth=xCursorness*360.0f-90.0f;
				//fAltitude=yCursorness*180.0f-90.0f;
				//NEW AIMING--TOWARD CURSOR:
				fAltitude=0.0f;

				//m3dCursor.Z=HITDETECTION2D_CONSTANT_Z_DBL;
				//m3dCursor.X=xCursorness*(hero->m3dEnt.xMax-hero->m3dEnt.xMin) + hero->m3dEnt.yMin; //assumes lots of stuff
				//m3dCursor.Y=(1.0-yCursorness)*(hero->m3dEnt.yMax-hero->m3dEnt.yMin) + hero->m3dEnt.yMin; //assumes lots of stuff
				float maximumPitchDegrees=-5.0f;
				//camera.Point3dFrom2dAssumingHeight(m3dCursor,iLastLineRelationshipType,xfCursor,yfCursor,HITDETECTION2D_CONSTANT_Z_DBL,maximumPitchDegrees);
				if (VisualDebugEnable) {
					//params of Point3dFrom2dAssumingHeight:
					float x2D=xfCursor;
					float y2D=yfCursor;
					float z3D=HITDETECTION2D_CONSTANT_Z_DBL;
					float maximumPitch=maximumPitchDegrees;
					//local variables and statements of Point3dFrom2dAssumingHeight:
					static float cursorRightness;
					static float cursorBottomness;
					static float cursorYaw;
					static float cursorPitch;
					static float xCursorTarget;
					static float yCursorTarget;
					static float zCursorTarget;
					//members of camera:
					int LastSideLineRelationshipTypeNumber=0;
					int LastTopLineRelationshipTypeNumber=0;
					//local variables and statements of Point3dFrom2dAssumingHeight:
					bool bGood=true;
					if (maximumPitch>-5.0f) maximumPitch=-5.0f;
					cursorRightness=x2D/camera.get_fScreenW();
					cursorBottomness=y2D/camera.get_fScreenH();
					cursorYaw=camera.get_fAngleApertureLeft()-cursorRightness*camera.get_fApertureW();
					cursorPitch=camera.get_fAngleApertureTop()-cursorBottomness*camera.get_fApertureH();
					if (cursorPitch>maximumPitch) cursorPitch=maximumPitch;
					//ANSWER: where does pitch intersect a plane whose z==z3D
					double xInter=0.0d, yInter=0.0d,zInter=0.0d; //the intersection is on the yz plane, and is ALWAYS z==height of plane since plane is flat
					//iLastLineRelationshipType=RMath::IntersectionAndRelationship(yInter, zInter, Line1_y1, Line1_z1, Line1_y2, Line1_z2, Line2_y1, Line2_z1, Line2_y2, Line2_z2);
					xCursorTarget=camera.m3dCam.X;
					yCursorTarget=camera.m3dCam.Y;
					zCursorTarget=camera.m3dCam.Z;
					gfontDefault.TypeFast(screen_ptr,debugtextcursor_Loc,RememberTextCursorPosition, "[sim 3dFrom2d]cursorPitch:"+RString_ToString(cursorPitch));
					debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
					gfontDefault.TypeFast(screen_ptr,debugtextcursor_Loc,RememberTextCursorPosition, "[sim 3dFrom2d]cursorYaw:"+RString_ToString(cursorYaw));
					debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
					gfontDefault.TypeFast(screen_ptr,debugtextcursor_Loc,RememberTextCursorPosition, "[sim 3dFrom2d]rCameraDist:"+RString_ToString((double)camera.get_rCameraDist()));
					debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
					gfontDefault.TypeFast(screen_ptr,debugtextcursor_Loc,RememberTextCursorPosition, "[sim 3dFrom2d]	  camera:"+RString_ToString(xCursorTarget)+", "+RString_ToString(yCursorTarget)+", "+RString_ToString(zCursorTarget));
					debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
					Travel3d(xCursorTarget,yCursorTarget,zCursorTarget,(double)cursorPitch,(double)cursorYaw,(double)camera.get_rCameraDist(),camera.make_orientation_Z_UP,camera.make_orientation_X_FORWARD);
					gfontDefault.TypeFast(screen_ptr,debugtextcursor_Loc,RememberTextCursorPosition, "[sim 3dFrom2d]CursorTarget:"+RString_ToString(xCursorTarget)+", "+RString_ToString(yCursorTarget)+", "+RString_ToString(zCursorTarget));
					debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
				}
				camera.Point3dFrom2dAssumingHeight(m3dCursor,iLastLineRelationshipType,xfCursor,yfCursor,HITDETECTION2D_CONSTANT_Z_DBL,maximumPitchDegrees);
				m3dCursor.Z=hero->EyeZ();

				static IPoint debug3DCursor_textCursor_Loc;
				debug3DCursor_textCursor_Loc.X=(int)xfCursor;
				debug3DCursor_textCursor_Loc.Y=(int)yfCursor;
				float inversion_multiplier=-1.0f;
				fAzimuth=RConvert_THETAOFXY_DEG(m3dCursor.X-hero->m3dEnt.X, m3dCursor.Y-hero->m3dEnt.Y);

				if (VisualDebugEnable) {
					gfontDefault.TypeFast(screen_ptr, debug3DCursor_textCursor_Loc, RememberTextCursorPosition, "[Refresh3dCursor]LastSideLineRelationshipType:"+RMath::LineRelationshipToString(camera.LastSideLineRelationshipTypeNumber),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
					debug3DCursor_textCursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
					gfontDefault.TypeFast(screen_ptr, debug3DCursor_textCursor_Loc, RememberTextCursorPosition, "[Refresh3dCursor]LastTopLineRelationshipType:"+RMath::LineRelationshipToString(camera.LastTopLineRelationshipTypeNumber),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
					debug3DCursor_textCursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
					gfontDefault.TypeFast(screen_ptr, debug3DCursor_textCursor_Loc, RememberTextCursorPosition, "[Refresh3dCursor]xfCursor:"+RString_ToString(xfCursor),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
					debug3DCursor_textCursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
					gfontDefault.TypeFast(screen_ptr, debug3DCursor_textCursor_Loc, RememberTextCursorPosition, "[Refresh3dCursor]yfCursor:"+RString_ToString(yfCursor),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
					debug3DCursor_textCursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
					gfontDefault.TypeFast(screen_ptr, debug3DCursor_textCursor_Loc, RememberTextCursorPosition, "[Refresh3dCursor]xCursorness:"+RString_ToString(xCursorness),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
					debug3DCursor_textCursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
					gfontDefault.TypeFast(screen_ptr, debug3DCursor_textCursor_Loc, RememberTextCursorPosition, "[Refresh3dCursor]yCursorness:"+RString_ToString(yCursorness),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
					debug3DCursor_textCursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
					gfontDefault.TypeFast(screen_ptr, debug3DCursor_textCursor_Loc, RememberTextCursorPosition, "[Refresh3dCursor]fAzimuth from hero:"+RString_ToString(fAzimuth),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
					debug3DCursor_textCursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;

					gfontDefault.TypeFast(screen_ptr, debug3DCursor_textCursor_Loc, RememberTextCursorPosition, "[Refresh3dCursor]m3dCursor:"+m3dCursor.ToString(true),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
					debug3DCursor_textCursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;


					debugtextcursor_Loc.X=0.0f;
					gfontDefault.TypeFast(screen_ptr, debugtextcursor_Loc, RememberTextCursorPosition, "camera:"+camera.ToString(true),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
					debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height; //debugtextcursor_Loc
					debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height*10; //TODO: find out why m3dCameraTarget would otherwise overlap previous text

					Mass3d m3dCameraTarget;
					m3dCameraTarget.name="CameraTarget";
					if (bFirstRunRefresh3dCursor) {
						Console::Error.Write("bFirstRunRefresh3dCursor...");
						Console::Error.Flush();
					}
					if (bFirstRunRefresh3dCursor) {
						Console::Error.Write("GetTarget...");
						Console::Error.Flush();
					}
					camera.GetTarget(m3dCameraTarget);
					DrawPointWithZOffsetIndicator(m3dCameraTarget,"m3dCameraTarget");
					camera.DrawBox(screen_ptr, m3dCursor,pixelYellow,pixelMediumDarkYellow);
				}
				byte byBrightness=SafeByte( (ANGLEDIFFPOSITIVE(fAzimuth,90.0f)/180.0f) * 128.0f + 128.0f);
				if (byBrightness<128) byBrightness=255; //fixes byte integer looping
				if (bMouseDown) {
					hero->Shoot(fAltitude,fAzimuth); //p3dCursorAbs is an absolute position
				}
				if (bMouseMove) {
					hero->Turn(fAzimuth);
				}

				//DrawPointWithZOffsetIndicator(m3dCursor,"m3dCursor");
				DrawPointWithGroundSquareIndicator(m3dCursor, pixelDarkRed, "m3dCursor");

				m3dHeroEye.X=hero->EyeX();
				m3dHeroEye.Y=hero->EyeY();
				m3dHeroEye.Z=hero->EyeZ();
				m3dFiringLineNearPointForDisplayOnly.xSize=1.0f;
				m3dFiringLineNearPointForDisplayOnly.ySize=1.0f;
				m3dFiringLineNearPointForDisplayOnly.zSize=1.0f;
				m3dFiringLineNearPointForDisplayOnly.X=hero->m3dWeapon.X; //m3dHeroEye.X;//m3dCursor.X=m3dHeroEye.X;
				m3dFiringLineNearPointForDisplayOnly.Y=hero->m3dWeapon.Y; //m3dHeroEye.Y;//m3dCursor.Y=m3dHeroEye.Y;
				m3dFiringLineNearPointForDisplayOnly.Z=hero->m3dWeapon.Z; //m3dHeroEye.Z;//m3dCursor.Z=m3dHeroEye.Z;
				Travel3d(m3dFiringLineNearPointForDisplayOnly, fAltitude, fAzimuth, fAimerDistance, camera.make_orientation_Z_UP, camera.make_orientation_X_FORWARD); //Travel3d(m3dCursor, fAltitude, fAzimuth, fAimerDistance);
				//m3dFiringLineNearPointForDisplayOnly.CopyTo(m3dCursor);
				AddScreenItem(SCREENITEM_3DCURSOR, ZORDER_FROMY(m3dFiringLineNearPointForDisplayOnly.Y), byBrightness); //AddScreenItem(SCREENITEM_3DCURSOR,ZORDER_FROMY(m3dCursor.Y),byBrightness);
				//Red cube:
				//AddScreenItem(SCREENITEM_CUBE, ZORDER_FROMY(m3dFiringLineNearPointForDisplayOnly.Y), byBrightness, m3dFiringLineNearPointForDisplayOnly);//AddScreenItem(SCREENITEM_CUBE,ZORDER_FROMY(m3dCursor.Y),byBrightness,m3dCursor);
				/*
				Mass3d p3dHeroEye;
				p3dHeroEye.name="HeroEye";
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
				FPoint fptHeroeye;
				camera.Point2dMultipliersFrom3d(fptHeroeye.X,fptHeroeye.Y,p3dHeroEye.X,p3dHeroEye.Y,p3dHeroEye.Z);
				fptHeroeye.X=fptHeroeye.X*screen_ptr->iWidth;
				fptHeroeye.Y=fptHeroeye.Y*screen_ptr->iHeight;
				xRelNear=xfCursor-fptHeroeye.X; yRelNear=yfCursor-fptHeroeye.Y;

				fAngleFlat=SafeAngle360(FTHETAOFXY(xRelNear,-yRelNear));
				fAngleFlatNonCartesian=SafeAngle360(FTHETAOFXY(xRelNear,yRelNear));
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
				const float fCircleSize=.02*(float)screen_ptr->iHeight;
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
				//screen_ptr->DrawSubpixelLine(fptHeroeye.X+xRelNear, //debug only
				//							   fptHeroeye.Y+yRelNear,
				//							   fptHeroeye.X+xRelNear-zFake,
				//							   fptHeroeye.Y+yRelNear+zFake,
				//							   255,0,0,
				//							   33,.2);
				//float fFrontnessFlatFake=((zFake*-1.0f+fAimRadNear)/fAimRadFar);
				//byte byValue=(byte)( 63.0f+192*fFrontnessFlatFake);
				//screen_ptr->DrawSubpixelArc(xCursorNearFlatAbs,
				//							yCursorNearFlatAbs+,
				//							4.5,1,0,
				//							0,SafeAngle360(FTHETAOFXY(xRelNear,zFake)),
				//							byValue,0,0,128,
				//							5,0);
				fSizeModifier*=fCircleSize*.3f;
				float fCircleResult;

			   	//fSizeModifier*=fSizeModifier;
				//if (bBack) {
			 //   fSizeModifier*=-1;
				//}
				fCircleResult=fCircleSize+fSizeModifier;
				/Inspector Gadget Series Review /fCircleResult*=fCircleResult;

				//screen_ptr->DrawSubpixelLine(32,32,xfCursor,yfCursor,0,255,0,255,.2);
				//screen_ptr->DrawSubpixelLine(fptHeroeye.X, fptHeroeye.Y, xCursorNearFlatAbs, yCursorNearFlatAbs,
				//		0,(bBack?127:255),0,255,32*fDistFlatNear/fAimRadNear);
				//screen_ptr->DrawSubpixelArc(xCursorNearFlatAbs, yCursorNearFlatAbs, fCircleResult, 1.0f-(fDistFlatNear/fAimRadNear), fAngleFlatNonCartesian, 0, 360,
				//		0,(bBack?127:255),0,255,.5,0);
				//float fBackness=0, fFrontness=0;
				//if (zFake<0) {
				//	fFrontness=-1.0f*zFake/fAimRadNear;
				//}
				//else {
				//	fBackness=zFake/fAimRadNear;
				//}
				//screen_ptr->DrawSubpixelLine(32,32,xfCursor,yfCursor,
				//(byte)(255.0f*fBackness),0,(byte)(255.0f*fFrontness),
				//255,.2);
				bTurn=bMouseMove;
				Mass3d p3dCursorAbs, p3dCursorRel;
				p3dCursorAbs.name="CursorAbs";
				p3dCursorRel.name="CursorRel";
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
				screen_ptr->DrawSubpixelArc(fptCursor.X, fptCursor.Y, fCircleResult, 1.0f-(fDistFlatNear/fAimRadNear), fAngleFlatNonCartesian, 0, 360,
					pixelNow,.5,0);
				//screen_ptr->DrawSubpixelLine(fptHeroeye.X, fptHeroeye.Y, xCursorNearFlatAbs, yCursorNearFlatAbs,
				//		(bBack?64:128),0,0,(bBack?64:128),.3);
				float fAlpha=191+fForwardness*128;
				if (fAlpha>255.0f) fAlpha=255.0f;
				Pixel pixelEnd;
				pixelEnd.Set(0,(byte)(191.0f+fForwardness*64),0,(byte)fAlpha);
				pixelNow.Set(pixelEnd.r,pixelEnd.g,pixelEnd.b,0);
				screen_ptr->DrawSubpixelLine(fptHeroeye.X, fptHeroeye.Y, fptCursor.X, fptCursor.Y,
						pixelNow,&pixelEnd,.9);
				//static bool Refresh3dCursor_bFirstRun=true;
				//if (bTurn||Refresh3dCursor_bFirstRun) {//commented for debug only asdf
					//float zRealRot;
					//zRealRot=FTHETAOFXY(xRelNear,zFake);
					hero->Turn(p3dCursorAbs.zRot);
				//	Refresh3dCursor_bFirstRun=false;//commented for debug only asdf
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
				bFirstRunRefresh3dCursor=false;
				///see also Draw3dCursor
			}//end if hero
			else { //else show a 2d cursor since hero not present
				animCursor.GotoFrame(0);
				if (animCursor.frame_ptr!=nullptr) animCursor.frame_ptr->DrawToLargerWithoutCropElseCancel(screen_ptr, xCursor-xCursorCenter, yCursor-yCursorCenter, DrawModeBlendAlpha);
				else Console::WriteLine("nullptr animCursor.frame_ptr");
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
		FPoint fpStart,//this point is first used as the hero weapon
			fpEnd;//this 2d point is first used as the 3d cursor
		Pixel pixelEnd, pixelStart;

		hero->RefreshWeapon();
		//hero->m3dWeapon.Z=HITDETECTION2D_CONSTANT_Z_DBL;
		camera.Point2dFrom3d(fpStart,hero->m3dWeapon);//m3dHeroEye);
		camera.Point2dFrom3d(fpEnd,m3dCursor);
		///#region draw line to 3d cursor
		//pixelStart.Set(byBrightness,0,0,0);//Red
		//pixelEnd.Set(byBrightness,0,0,255);//Red
		//screen_ptr->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
		//		pixelNow,&pixelEnd,.9);
		///#endregion draw line to 3d cursor

		///#region draw screen-edge crosshairs
		if (VisualDebugEnable) {
			pixelStart.Set(255,0,0,255);//Red
			pixelEnd.Set(255,0,0,255);//Red
			float fLenPix=16;
			//top:
			fpStart.X=xfCursor;
			fpStart.Y=0;
			fpEnd.X=fpStart.X;
			fpEnd.Y=fpStart.Y+fLenPix;
			screen_ptr->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
					pixelNow,&pixelEnd,.9);
			//bottom:
			fpStart.X=xfCursor;
			fpStart.Y=(float)screen_ptr->iHeight;
			fpEnd.X=fpStart.X;
			fpEnd.Y=fpStart.Y-fLenPix;
			screen_ptr->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
					pixelNow,&pixelEnd,.9);
			//left:
			fpStart.X=0.0f;
			fpStart.Y=yfCursor;
			fpEnd.X=fpStart.X+fLenPix;
			fpEnd.Y=fpStart.Y;
			screen_ptr->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
					pixelNow,&pixelEnd,.9);
			//right:
			fpStart.X=(float)screen_ptr->iWidth;
			fpStart.Y=yfCursor;
			fpEnd.X=fpStart.X-fLenPix;
			fpEnd.Y=fpStart.Y;
			screen_ptr->DrawSubpixelLine(fpStart.X, fpStart.Y, fpEnd.X, fpEnd.Y,
					pixelNow,&pixelEnd,.9);
		}
		///#endregion draw screen-edge crosshairs
		static IPoint ipCursor;
		ipCursor.X=(int)fpEnd.X;
		ipCursor.Y=(int)fpEnd.Y;
		animCursor.GotoFrame(0);
		if (animCursor.frame_ptr!=nullptr) GBuffer_FX_Scaled(screen_ptr,animCursor.frame_ptr,ipCursor.X-(animCursor.Width()/2),ipCursor.Y-(animCursor.Height()/2),1.0f,0.0f ,false ,1.0f,"3D Cursor", VisualDebugEnable?&gfontDefault:nullptr);
		//animCursor.DrawFrameOverlay(screen_ptr,ipCursor);
		if (VisualDebugEnable) {
			gfontDefault.TypeFast(screen_ptr, debugtextcursor_Loc, RememberTextCursorPosition, "cursor:"+m3dCursor.ToString(true),0,DrawModeBlendAlpha, false, debugtextcursor_LastTextBlockSize);
			debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
			//gfontDefault.TypeFast(debugtextcursor_LastTextBlockSize, screen_ptr, debugtextcursor_Loc, "hero.eye:"+m3dHeroEye.ToString(true),0,DrawModeBlendAlpha, false);
			//debugtextcursor_Loc.Y+=(int)debugtextcursor_LastTextBlockSize.Height;
		}
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
	bool DrawScreen() {
		bool bGood=true;
		static bool bFirstRun=true;
		if (screen==nullptr) {
			if (bFirstRun) Console::Error.Write("Screen is nullptr (skipping DrawScreen)!...");
			if (bFirstRun) Console::Error.Flush();
			bFirstRun=false;
			return false;
		}
		if (bFirstRun) Console::Error.Write("checking for locking requirement...");
		if (bFirstRun) Console::Error.Flush();
		if (SDL_MUSTLOCK(screen)!=0) {
			if (bFirstRun) Console::Error.Write("locking...");
			if (bFirstRun) Console::Error.Flush();
			if (SDL_LockSurface(screen)<0) {
				bGood=false;
				ShowError("Can't lock screen surface","DrawScreen");
			}
		}
		else {
			if (bFirstRun) Console::Error.Write("locking not required...");
			if (bFirstRun) Console::Error.Flush();
		}
		int iDestBytesPP=0;
		int iDestStride=0;
		int iDestWidth=screen_ptr->iWidth;
		int iDestHeight=screen_ptr->iHeight;
		int iSrcStart;
		int iSrcStride=0;
		int iStrideMin=0;
		if (bGood) {
			if (bFirstRun) Console::Error.Write("getting data on backward-compatible 'screen' surface in system memory...");
			if (bFirstRun) Console::Error.Flush();
			iDestBytesPP=screen->format->BytesPerPixel;
			iDestStride=screen->pitch;
			iSrcStride=screen_ptr->iStride;
			iSrcStart=0;
			iStrideMin=(iSrcStride<iDestStride)?iSrcStride:iDestStride;
			if (screen_ptr->BytePtrStart()==nullptr) {
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
				if (bFirstRun) Console::Error.Write("getting screen BytePtrStart...");
				if (bFirstRun) Console::Error.Flush();
				lpSrcLine=screen_ptr->BytePtrStart();
				lpSrcLine+=iSrcStart;//byte offset for cropped screen_ptr's top left corner
				if (bFirstRun) Console::Error.Write("debug test...");

				//Uint8* lpbyScreen=(Uint8*)screen->pixels;//debug only
				//*((Uint32*)lpbyScreen)=0xFFFFFF; //debug only
				//*(Uint32*)(&lpbyScreen[32*iDestStride+32*iDestBytesPP])=0xFFFFFF; //debug only
				//screen_ptr->Fill(255);//debug only

				//if (bFirstRun) Console::Error.Write("copy...");
				if (bFirstRun) Console::Error.Write("getting screen->pixels...");
				if (bFirstRun) Console::Error.Flush();
				lpDestLine=(Uint8*)screen->pixels;//8bit in order to use stride
				switch (iDestBytesPP) {
					case 1:
						if (bFirstRun) Console::Error.Write("writing to 8-bit...");
						if (bFirstRun) Console::Error.Flush();
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
						if (bFirstRun) Console::Error.Write("writing to 16-bit...");
						if (bFirstRun) Console::Error.Flush();
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
							if (bFirstRun) Console::Error.Write("writing to little-endian 24-bit...");
							if (bFirstRun) Console::Error.Flush();
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
							if (bFirstRun) Console::Error.Write("writing to big-endian 24-bit...");
							if (bFirstRun) Console::Error.Flush();
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
						if (bFirstRun) Console::Error.Write("writing to 32-bit...");
						if (bFirstRun) Console::Error.Flush();
						Uint32* lpu32DestPix;
						if (bFirstRun) Console::Error.Write("32-bit from "+RString_ToString(screen_ptr->iWidth)+"X"+RString_ToString(screen_ptr->iHeight)+"X"+RString_ToString(screen_ptr->iBytesPP*8)+"...");
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
				//SDL_Flip(screen); //deprecated in SDL2
				SDL_UpdateTexture(sdlTexture, nullptr, screen->pixels, screen->pitch);
				SDL_RenderClear(sdlRenderer);
				SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
				SDL_RenderPresent(sdlRenderer);
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
	bool DoEvents() {
		bool IsContinuing=true;
		while (SDL_PollEvent (&event)) { //allow mouse movement, to avoid appearance of freezing (there is not a real freeze without this though)
			if (event.type==SDL_KEYDOWN) {
				break;
			}
			else if (event.type==SDL_MOUSEBUTTONDOWN) {
				break;
			}
			else if (event.type==SDL_QUIT) {
				IsContinuing=false;
			}
		}
		return IsContinuing;
	}
	void SleepAllowingEvents(int delay_ms) {
		int count_ms=0;
		if (delay_ms%100==0) {
			while (count_ms<delay_ms) {
				SleepWrapper(100);
				DoEvents();
				count_ms+=100;
			}
		}
		else if (delay_ms%10==0) {
			while (count_ms<delay_ms) {
				SleepWrapper(10);
				DoEvents();
				count_ms+=10;
			}
		}
		else {
			while (count_ms<delay_ms) {
				if (delay_ms-count_ms>10) {
					SleepWrapper(10);
					DoEvents();
					count_ms+=10;
				}
				else {
					SleepWrapper(1);
					DoEvents();
					count_ms+=1;
				}
			}
		}
	}
	///shot_index must be 0 or higher if bumper is a shot
	void DoCollision(Entity* bumpee, Entity* bumper, int shot_index) {
		//TODO: make bombs be able to get hit (btw ignore shield in that case, which they have just for blue outline effect)
		bool bumper_delete_enable=false;
		if (bumpee->HasAttrib(STATUS_SHIELD)) { //when boss is present, aliens are invincible with shield except boss
			if (bumpee->HasAttrib(STATUS_BOSS)||(bumpee->iType==ENTITY_TYPE_HERO)) bumpee->RemoveAttrib(STATUS_SHIELD); //only Boss loses shield, other shields are removed when boss dies
			bumpee->rFortitude=.1f; //prevents multiple hits
			if (shot_index>=0) {
				delete arrpentShot[shot_index];
				arrpentShot[shot_index]=nullptr;
			}
			else if (bumper->iType==ENTITY_TYPE_ALIEN) PlaySound("Blerrp",bumpee->NameUnique,bumpee->m3dEnt);
			if (!bumpee->bExploding) {
				PlaySound("ShieldZap",bumpee->NameUnique,bumpee->m3dEnt);
			}
		}
		else if (bumpee->HasAttrib(STATUS_VARIABLESHIELD)) { //not normally used if (iType==ENTITY_TYPE_ALIEN || iType==ENTITY_TYPE_BOSS)
			bumpee->RemoveAttrib(STATUS_SHIELD); //always remove incompatible attribute
			bumpee->rFortitude=.1f; //prevents multiple hits
			bumpee->fShield-=bumpee->fLaserSusceptibility*3.0f; //.196
			if (bumpee->fShield<=0.0f) bumpee->RemoveAttrib(STATUS_VARIABLESHIELD);
			if (shot_index>=0) {
				delete arrpentShot[shot_index];
				arrpentShot[shot_index]=nullptr;
			}
			else if (bumper->iType==ENTITY_TYPE_ALIEN) PlaySound("Blerrp",bumpee->NameUnique,bumpee->m3dEnt);
			PlaySound("ShieldZap",bumpee->NameUnique,bumpee->m3dEnt);
		}
		else {
			bumpee->rFortitude=.1f;
			bumpee->fHealth-=bumpee->fLaserSusceptibility*((bumper->iType==ENTITY_TYPE_ALIEN)?3.0f:1.0f); //hero loses ~.196 to alien, ~.059 to laser
			if (shot_index>=0) {
				delete arrpentShot[shot_index];
				arrpentShot[shot_index]=nullptr;
			}
			else if (bumper->iType==ENTITY_TYPE_ALIEN) PlaySound("Blerrp",bumpee->NameUnique,bumpee->m3dEnt);
			if (!bumpee->bExploding) {
				if (bumpee->iType==ENTITY_TYPE_HERO) PlaySound("OuchZap",bumpee->NameUnique,bumpee->m3dEnt);
				else if (bumpee->iType==ENTITY_TYPE_ALIEN) {
					PlaySound("OuchAlien",bumpee->NameUnique,bumpee->m3dEnt);
				}
			}
		}
	}
	//public string GameStateRString_ToString(int iGameStateX) {
	//
	//}
	///#endregion functions
}//end namespace

using namespace ExpertMultimediaBase;
////using namespace ProtoArmor;
//int SDL_main();
//int main(int argc, char *argv[]) {
//int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR
//	lpCmdLine, unsigned int nCmdShow ) {
int main(int iArgs, char *lpsArg[]) { //int main(int iArgs, char** lpsArg) {
	m3dTemp.name="dxman Temp";
	m3dHeroEye.name="HeroEye";
	m3dCursor.name="Cursor";
	m3dFiringLineNearPointForDisplayOnly.name="FiringLineNearPointForDisplayOnly";

	sVerb_Engine="running GameInit (in main)";
	Console::Error.WriteLine("GameInit:");//NOTE: this will be written to stderr.txt because of SDL's trickery!!!
	Console::Error.WriteLine("Date and time:"+System::DateTime_Now.ToString("yyyy-MM-dd HH:mm:ss"));
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
		if (!bDebug) SleepAllowingEvents(3000);
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
				if (event.key.keysym.sym==SDLK_F3) {
					if (VisualDebugEnable) VisualDebugEnable=false;
					else VisualDebugEnable=true;
				}
				else if (event.key.keysym.sym==SDLK_SPACE) {
					u32Pressing|=GAMEKEY_JUMP;
				}
				else if (event.key.keysym.sym==SDLK_a) {
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
				}
				else if (event.key.keysym.sym==SDLK_F2) {
					stringstream timeSS;
					time_t t = time(0);   // get time now
					struct tm * now = localtime( & t );
					timeSS << (now->tm_year + 1900) << '-'
						<<  ((now->tm_mon<10)?"0":"") << now->tm_mon << "-"
						<<  ((now->tm_mday<10)?"0":"") << now->tm_mday << " "
						<<  ((now->tm_hour<10)?"0":"") << now->tm_hour << "_"
						<<  ((now->tm_min<10)?"0":"") << now->tm_min << "_"
						<<  ((now->tm_sec<10)?"0":"") << now->tm_sec;
					string screenShotFileName=timeSS.str()+".tga";
					Console::Error.Write("Saving screenshot to "+screenShotFileName+"...");
					Console::Error.Flush();
					screen_ptr->FillAlpha(255);
					bool bSaved=screen_ptr->Save(screenShotFileName);
					Console::Error.WriteLine(bSaved?"OK":"FAIL");
				}
				/*
				else if (event.key.keysym.sym==SDLK_KP_PLUS) {
				//This case is only for testing--smart Bomb to skip level!!
					if (!bBombed) {
						bBombed=true;
						for (int index=0; index<iMaxAliensNow; index++) {
							if (arrpentAlien[index] != nullptr) {
								delete arrpentAlien[index];
								arrpentAlien[index]=nullptr;
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
				else if (event.key.keysym.sym==SDLK_SPACE) {
					u32Pressing&=GAMEKEY_JUMP^0xFFFFFFFF;
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
					bDone=true; //this will stop the main event loop, causing main to end, which will cause atexit to fire, which will pop QuitSafe off the stack, which will call GameShutdown, and SDL_Quit which cleans up the window.
					//Console::Error.Write("GameShutDown...");
					//Console::Error.Flush();
					//if (GameShutdown()) Console::Error.WriteLine("...Success (GameShutdown).");
					//else Console::Error.WriteLine("Failed!");
					//exit(0)
				}
				break;
			default:
				break;
			}//end switch event.type
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
			else SleepWrapper(1);//outer loop repeats this line until next frame is needed
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
			///TODO: force rsound.Iterate?? PlaySounds();
			if (bSDLQuitDone) bDone=true;
			if (bFirstRunOfMainEventLoop) Console::Error.WriteLine("FirstRun: Finished main event loop successfully.");
		}//end if not bDone do normal stuff
		else if (bFirstRunOfMainEventLoop) Console::Error.WriteLine("yes.");
		bFirstRunOfMainEventLoop=false;
		iMainEventLoopCount++;
		//bDebug=VisualDebugEnable;
	}//end while not bDone continue main event loop

	if (bMegaDebug) Console::Error.WriteLine(RString_ToString("Main event loop ran ")+RString_ToString(iMainEventLoopCount)+RString_ToString(" times."));
	Console::Error.Write("Exiting main after main event loop--Stopping Remaining processes...");
	Console::Error.Flush();
	///NOTE: QuitSafe runs atexit (after main event loop), and DOES run GameShutdown and SDL_Quit
	//if (!bSDLQuitDone) {
	//	bSDLQuitDone=true;
	//	SDL_Quit();
	//}
	//if (!bShuttingDown && !bDoneShutdown) {
	//	Console::Error.Write("GameShutdown...");
	//	if (GameShutdown()) Console::Error.WriteLine("...OK (GameShutdown)");
	//	else Console::Error.WriteLine("...FAILED (GameShutdown)");
	//}
	Console::Error.WriteLine("Returning from main.");
	return iMainReturn;
}//end main(...)
#endif
