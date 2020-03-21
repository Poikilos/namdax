#ifndef ENTITY_CPP
#define ENTITY_CPP

#include "entity.h"
#include <base.h>
#include "dxman.h"
#include <RImage_bgra32.h> //DrawSubPixelArc etc
#include <RMath.h>

using namespace std;

namespace ExpertMultimediaBase {
	void Entity::InitNull() {
		lpanimMain=NULL;
		lpanimShadow=NULL;
		fShield=0.0f;
		bHitSoundWasPlayed=false;
		bCycleFrames=false;
		bGravity=false;
		iBoundaryAction=BOUNDARY_BOUNCE;
		iTickLastHitGroundSound=Base_GetTicks_Relative();
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
		iTickLastRefresh=Base_GetTicks_Relative();
		iTickLastMetricMove=Base_GetTicks_Relative();
		bFirstRunOfThisEntity=true;
		//iFramesShadow=1;
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
		//TODO: m3dEnt.SetRotMaxSpeed(180,180,180);
		m3dEnt.zMin=0.0f;
		rFortitude=1.0f;
		fSpeedMultiplier=1.0f;
		iTicksLife=-1;
		iTickThisEntityCreated=Base_GetTicks_Relative();

		iIndex=-1;
		iDir=-1;
		zRotSpeed=0.0f;
		fShootMetersPerSec=1.0f;
		fHealth=1.0f;
		fPower=1.0f;
	}//end InitNull()
	Entity::Entity(int iTypeX) {
		InitNull();
		iType=iTypeX;
		//bool bGood=true;
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
					dat.GetOrCreate(fHealthMax,"hero.maxhp");
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
					m3dEnt.X=0;//(SCREEN_WIDTH/2 - m2dEnt.rectOriginal.right/2)-21;
					m3dEnt.Y=m3dEnt.yMin;//(SCREEN_HEIGHT*.75 -height/2)-90;//m3dEnt.Y=SCREEN_HEIGHT*.75 - height/2;
					m3dEnt.Z=4; //starts falling from air
					dat.GetOrCreate(m3dEnt.X, "hero.start.X");
					dat.GetOrCreate(m3dEnt.Y, "hero.start.Y");
					dat.GetOrCreate(m3dEnt.Z, "hero.start.Z");
					m3dEnt.xSize=.4f;
					m3dEnt.ySize=.4f;
					m3dEnt.zSize=1.8f;//1.6f;
					dat.GetOrCreate(m3dEnt.xSize, "hero.size.X");
					dat.GetOrCreate(m3dEnt.ySize, "hero.size.Y");
					dat.GetOrCreate(m3dEnt.zSize, "hero.size.Z");
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
					ShowExn(exn,"Entity constructor");
				}
				catch (...) {
					ShowUnknownExn("Entity constructor");
				}
			}//end if ENTITY_TYPE_HERO
			else if (iType==ENTITY_TYPE_SHOT) {
				Console::Error.WriteLine("Created a useless shot using no constructor parameters.");
			}//end if ENTITY_TYPE_SHOT
			else if (iType==ENTITY_TYPE_ALIEN) {
				Console::Error.WriteLine("Created a useless alien using no constructor parameters.");
			}//end if ENTITY_TYPE_ALIEN
		}
		catch (exception& exn) {
			ShowExn(exn,"Entity constructor by type");
		}
		catch (...) {
			ShowUnknownExn("Entity constructor by type");
		}
		//TODO: this constructor isn't used except for hero.  move to an overload of init and make that the init overload that contains the init code
		m3dEnt.zRotDest=m3dEnt.zRot;
	}//END CONSTRUCTOR BY iTypeX
	Entity::Entity(int iTypeX, Mass3d m3dDirectionAndLocation, float fMetersPerSec, bool IsAlien, bool IsBomb) {
		InitNull();
		//bool bGood=true;
		iType=iTypeX;
		bUsePitchYaw=true;
		fMetersPerSecond=fMetersPerSec;
		fMetersPerSecondMax=fMetersPerSecond;
		m3dEnt.HardLocation(m3dDirectionAndLocation.X,m3dDirectionAndLocation.Y,m3dDirectionAndLocation.Z);
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
				iTicksLife=7000;
				m3dEnt.xSize=.25;
				m3dEnt.zSize=.25;
				m3dEnt.ySize=.25;

				lpanimMain=&animShot;
	 			m2dEnt.Init(lpanimMain->Width(),lpanimMain->Height(),400.0f);
				settings.GetOrCreate(m2dEnt.fPixelsPerMeter,"shot.size.pixelspermeter");
				m2dEnt.SetHitRect(19,11,58,252);//TODO: comment this again??
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
			catch (exception& exn) {
				ShowExn(exn,"Entity constructor using Shot angle and Shot Variables");
			}
			catch (...) {
				ShowUnknownExn("Entity constructor using Shot angle and Shot Variables");
			}
		}//end if ENTITY_TYPE_SHOT
	  m3dEnt.zRotDest=m3dEnt.zRot;
	}//end shot constructor by Mass3d
	Entity::Entity(int iTypeX, float x2, float y2, float z2, float xVel2, float yVel2, float zVel2, bool IsAlien, bool IsBomb) {
		InitNull();
		iType=iTypeX;
		//bool bGood=true;
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
				m3dEnt.X=x2;
				m3dEnt.Y=y2;
				m3dEnt.Z=z2;
				m3dEnt.xVel=xVel2;
				m3dEnt.yVel=yVel2;
				m3dEnt.zVel=zVel2;
				bAlien=IsAlien;
				bBomb=IsBomb;
				dat.GetOrCreate(fShadowOpacity,"shot.shadow.multiplier");
				dat.GetOrCreate(iTicksLife,"shot.life.msec");
			}
			catch (exception& exn) {
				ShowExn(exn,"Entity constructor using Shot angle and Shot Variables");
			}
			catch (...) {
				ShowUnknownExn("Entity constructor using Shot angle and Shot Variables");
			}
		}//end if ENTITY_TYPE_SHOT
	  m3dEnt.zRotDest=m3dEnt.zRot;
	}//END CONSTRUCTOR BY SHOT ANGLE AND SHOT VARIABLES
	Entity::Entity(int iTypeX, float x2, float y2, float z2) {
		InitNull();
		iType=iTypeX;
		//bool bGood=true;
		if (iType==ENTITY_TYPE_ALIEN) {
			try {
				fHealthMax=.118;
				dat.GetOrCreate(fHealthMax, "flyer.L1Health");
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
				m3dEnt.X=x2;
				m3dEnt.Y=y2;
				m3dEnt.Z=z2;
				m3dEnt.LocationToLimits();
				m3dEnt.xSize=2.0f;
				m3dEnt.ySize=1.0f;
				m3dEnt.zSize=.25f;
				dat.GetOrCreate(m3dEnt.xSize, "flyer.size.X");
				dat.GetOrCreate(m3dEnt.ySize, "flyer.size.Y");
				dat.GetOrCreate(m3dEnt.zSize, "flyer.size.Z");
				//m3dEnt.xVel=(float)(iEncounter+1)*(float)(IRand()%3+1)/30.0f;
				//m3dEnt.yVel=-(float)(IRand()%4)*.01;
				//m3dEnt.zVel=-(float)(IRand()%4)*.01;
				for (int iX=0; iX<22; iX++) {
					IRandPositive();//iterate a couple times for extra "randomness"
				}
				fMetersPerSecondMax=(float)iEncounter * ((float)IRand(1,4)*.25f+2.0f);
				fMetersPerSecond=fMetersPerSecondMax;
				if (fMetersPerSecondMax<2.0f) fMetersPerSecondMax=2.0f;
				m3dEnt.yRotDest=0.0;//unused for flyer after 2011-12 (float)IRand(80,400);
				m3dEnt.yRot=m3dEnt.yRotDest;
				m3dEnt.xRot=0.0;
				m3dEnt.zRot=(float)IRand(0,360);
				//m3dEnt.zRotDest is set to m3dEnt.zRot at end of this method
				bUsePitchYaw=true;
				iFramesShootDelay=20;
				lpanimMain=&animFlyer;
	 			m2dEnt.Init(lpanimMain->Width(),lpanimMain->Height(),150.0f);
				settings.GetOrCreate(m2dEnt.fPixelsPerMeter,"flyer.size.pixelspermeter");
				m2dEnt.SetHitRect(19,11,58,252);//this was the new setting in the crossplatform version: m2dEnt.SetHitRect(107,91,159,254); //TODO: finish this: set hit rect automatically using image, EVERY TIME SetHitRect is called since frame changes it
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
				ShowExn(exn,"Entity constructor using location");
			}
			catch (...) {
				ShowUnknownExn("Entity constructor using location");
			}
		}//end if ENTITY_TYPE_ALIEN
	  m3dEnt.zRotDest=m3dEnt.zRot;
	}//end constructor by location
	Entity::~Entity() {
	}
	////////////////////		   end constructors		  /////////////////////////////
	void Entity::SetAsBoss() {
		bool bGood=false;
		if (iType==ENTITY_TYPE_BOSS) {
			static bool bShown=false;
			if (!bShown) {
				if (ShowError()) Console::Error.WriteLine("Error in SetAsBoss--type is already boss.");
				bShown=true;
			}
			return;
		}
		iType=ENTITY_TYPE_BOSS;
		bCycleFrames=true;
		m3dEnt.xSize=4.7f;
		m3dEnt.ySize=4.7f;
		m3dEnt.zSize=2.0f;//1.2f;
		dat.GetOrCreate(m3dEnt.xSize, "boss.size.X");
		dat.GetOrCreate(m3dEnt.zSize, "boss.size.Z");
		dat.GetOrCreate(m3dEnt.ySize, "boss.size.Y");
		try {
			try {
				try {
					fHealthMax=4.0f;
					dat.GetOrCreate(fHealthMax,"boss.maxhealth");
					m3dEnt.yRotDest=(float)IRand(80,400);
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
					m3dEnt.X=0;m3dEnt.Y=0;m3dEnt.Z=m3dEnt.zMax;
					dat.GetOrCreate(m3dEnt.X, "boss.start.X");
					dat.GetOrCreate(m3dEnt.Y, "boss.start.Y");
					dat.GetOrCreate(m3dEnt.Z, "boss.start.Z");
					m3dEnt.xVel=.1f;
					m3dEnt.yVel=.04f;
					m3dEnt.zVel=-.0333f;
					fMetersPerSecond=4.0;
					bUsePitchYaw=false;//TODO: change vars and this
					dat.GetOrCreate(m3dEnt.xVel, "boss.start.velocity.X");
					dat.GetOrCreate(m3dEnt.yVel, "boss.start.velocity.Y");
					dat.GetOrCreate(m3dEnt.zVel, "boss.start.velocity.Z");
					fShootMetersPerSec=6.6f;
					dat.GetOrCreate(fShootMetersPerSec,"boss.shot.meterspersec");
					fHealth=fHealthMax;
					//sound names: bomb, laser-alien, laser, explosion, ouchalien, ouchzap, shieldzap
					//	blerrp, hitdirt, jump, scrapeground, angryalien, trumpet, thruster, laser
					iPlayAngryAlien=SOUNDPAN_FROM3DX(m3dEnt.X);
					iFramesShootDelay=4;
					fLaserSusceptibility=.05;//.0265;
					dat.GetOrCreate(fLaserSusceptibility,"boss.fLaserSusceptibility");
					fShadowOpacity=.35;
					dat.GetOrCreate(fShadowOpacity,"boss.shadow.multiplier");
					settings.Save();
					bGood=true;
				}
				catch (exception& exn) {
					bGood=false;
					ShowExn(exn,"initializing boss");
				}
				catch (...) {
					bGood=false;
					ShowUnknownExn("initializing boss");
				}
			}
			catch (exception& exn) {
				bGood=false;
				ShowExn(exn,"loading boss");
			}
			catch (...) {
				bGood=false;
				ShowUnknownExn("loading boss");
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"enabling boss");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("enabling boss");
		}
	}//end SetAsBoss
	void Entity::CheckFrames() {
		if (!bRotationTakesPrecedence && !bUsePitchYaw) {
			m3dEnt.zRot=FTHETAOFXY_DEG(m3dEnt.xVel,m3dEnt.yVel);
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
		//if (!bUsePitchYaw && iType!=ENTITY_TYPE_HERO) m3dEnt.zRot=FTHETAOFXY(m3dEnt.xVel,m3dEnt.yVel);
		float fDeg=RMath::SafeAngle360(m3dEnt.zRot);
		if (fDeg<=90.0f) iFrameTest=(int)APPROACH(0,15,fDeg/90.0f);
		else if (fDeg<=180.0f) iFrameTest=(int)APPROACH(15,30,(fDeg-90.0f)/90.0f);
		else if (fDeg<=270.0f) iFrameTest=(int)APPROACH(30,45,(fDeg-180.0f)/90.0f);
		else iFrameTest=(int)APPROACH(45,60,(fDeg-270.0f)/90.0f);
		if (iFrameTest<0) iFrameTest=0;
		else if (lpanimMain==null || iFrameTest>=lpanimMain->IFrames()) {
			if (ShowErr()) Console::Error.WriteLine("Warning: FrameFromRot(fDeg:"+RString_ToString(fDeg)+") (was zRot:"+RString_ToString(m3dEnt.zRot)+") is frame "+RString_ToString(iFrameTest));
			iFrameTest=0;
		}
		//static REAL rSecondsSinceLastShow=Base_GetSeconds_Relative();
		//if (Base_SecondsSince(rSecondsSinceLastShow)>=1) {
		//	rSecondsSinceLastShow=Base_GetSeconds_Relative();
		//	Console::Error.WriteLine("FrameFromRot() "+m3dEnt.zRot+" is "+iFrameTest);
		//}
		//static int iTest=0;
		//if (iTest<1000 && iType==ENTITY_TYPE_ALIEN) {
		//	Console::Error.WriteLine("FrameFromRot "+m3dEnt.zRot+" (cropped to "+fDeg+") is "+iFrameTest+".");
		//	iTest++;
		//}
		return iFrameTest;
	}
	void Entity::TurnIncrement(bool bPositiveAngle) {
		if (bPositiveAngle) m3dEnt.zRotDest=m3dEnt.zRot+DegreesToMoveThisManyMS(m3dEnt.zRotVelDegreesPerSec,Base_TicksSince_Relative(iTickLastRefresh));
		else m3dEnt.zRotDest=m3dEnt.zRot-DegreesToMoveThisManyMS(m3dEnt.zRotVelDegreesPerSec,Base_GetTicks_Relative()-iTickLastRefresh);
	}
	void Entity::Turn(float fDeg) {
		m3dEnt.zRotDest=RMath::SafeAngle360(fDeg);
	}
	void Entity::TurnPitchIncrement(bool bUp) {
		if (bUp) m3dEnt.yRotDest=m3dEnt.yRot+DegreesToMoveThisManyMS(m3dEnt.yRotVelDegreesPerSec,Base_GetTicks_Relative()-iTickLastRefresh);
		else m3dEnt.yRotDest=m3dEnt.yRot-DegreesToMoveThisManyMS(m3dEnt.yRotVelDegreesPerSec,Base_GetTicks_Relative()-iTickLastRefresh);
	}
	void Entity::TurnPitch(float fDeg) {
		m3dEnt.yRotDest=RMath::SafeAngle360(fDeg);
		if (m3dEnt.yRotDest<m3dEnt.yRotMin)m3dEnt.yRotDest=m3dEnt.yRotMin;
		else if (m3dEnt.yRotDest>m3dEnt.yRotMax)m3dEnt.yRotDest=m3dEnt.yRotMax;
	}
	void Entity::Jump() {
		if (iFramesMoveDelay<=0) {
			if ((m3dEnt.Z<=m3dEnt.zMin) || (u32Status & STATUS_ANTIGRAVITY)) {//don't unless not jumping or if ANTIGRAVITY status
				m3dEnt.zVel=(u32Status & STATUS_ANTIGRAVITY) ? .07f :.42f;//m3dEnt.zVel=40.0f;//debug this
				m3dEnt.Z+=m3dEnt.zVel;//push us out of the way of ground detection
				if (u32Status & STATUS_DOUBLESPEED) m3dEnt.zVel *= 2.0f;
				if (!(u32Status & STATUS_ANTIGRAVITY)) {//if no antigravity, delay movement in air
					iFramesMoveDelay= (u32Status & STATUS_DOUBLESPEED) ? 15 : 30;
					iPlayJump=SOUNDPAN_FROM3DX(m3dEnt.X);
				}
			}
		}
	}
	void Entity::Move(int xDir, int yDir) {
		static float fDist=.2f;
		if (iFramesMoveDelay<=0) {
			if (u32Status&STATUS_DOUBLESPEED) fDist=.4f;
			if (yDir>0) {//Move up (back, +m3dEnt.Y)
				if (m3dEnt.Y<m3dEnt.yMax) {
					m3dEnt.Y+=fDist;
					m3dEnt.yVel=fDist*yDir;
				}
			}
			else if (yDir<0) {//Move down (forward, -m3dEnt.Y)
				if (m3dEnt.Y>m3dEnt.yMin) {
					m3dEnt.Y-=fDist;
					m3dEnt.yVel=fDist*yDir;
				}
			}
			if (xDir<0) {//Move left, -m3dEnt.X
				if (m3dEnt.X>m3dEnt.xMin) {
					m3dEnt.X-=fDist;
					m3dEnt.xVel=fDist*xDir;
				}
			}
			else if (xDir>0) {//Move right +m3dEnt.X
				if (m3dEnt.X<m3dEnt.xMax) {
					m3dEnt.X+=fDist;
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
			if ((iFramesShootDelay<=0) && fPower>=fPowerPerShot) {
				if (u32Status&STATUS_RAPIDFIRE) {
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
				m3dWeapon.X=EyeX();
				m3dWeapon.Y=EyeY();
				m3dWeapon.Z=EyeZ();
				m3dWeapon.zRot=m3dEnt.zRot;
				m3dWeapon.yRot=m3dEnt.yRot;
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (arrpentShot[iShotNow] == NULL) {//create a shot at the first available shot pointer
						arrpentShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, false, false);
						(arrpentShot[iShotNow])->iIndex=iShotNow;
						iPlayLaser=SOUNDPAN_FROM3DX(m3dWeapon.X);
						break;
						//iChanLaser=Mix_PlayChannel(2, mcLaser, 0);//chan, sound, #loops
					}
				}
			}//end if able to shoot
		}//end if ENTITY_TYPE_HERO
		else if (iType==ENTITY_TYPE_ALIEN) {
			if (iFramesShootDelay<=0) {
				iFramesShootDelay=rand()%30+10;
				static Mass3d m3dWeapon;
				m3dWeapon.X=m3dEnt.X-m3dEnt.xSize/2.0f;
				m3dWeapon.Y=m3dEnt.Y;
				m3dWeapon.Z=m3dEnt.Z;
				m3dWeapon.zRot=(iDir==-1)?180.0f:0;
				m3dWeapon.yRot=0;
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (arrpentShot[iShotNow] == NULL) {
						iPlayLaserAlien=SOUNDPAN_FROM3DX(m3dEnt.X);
						arrpentShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, bAlien, false);
						(arrpentShot[iShotNow])->iIndex=iShotNow;
						break;
					}
				}
			}
		}//end if ENTITY_TYPE_ALIEN

	}
	void Entity::Shoot(float fPitch, float fYaw) {
		if (iType==ENTITY_TYPE_HERO) {
			if ((iFramesShootDelay<=0) && fPower>=fPowerPerShot) {
				if (u32Status&STATUS_RAPIDFIRE) {
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
				//pointAsVelocity.X=m3dEnt.X;
				//pointAsVelocity.Y=m3dEnt.Y;
				//pointAsVelocity.Z=m3dEnt.Z;
				//Travel3d(pointAsVelocity,m3dDest,fShootMetersPerSec/fDistTarget);
				//pointAsVelocity.X-=m3dEnt.X;
				//pointAsVelocity.Y-=m3dEnt.Y;
				//pointAsVelocity.Z-=m3dEnt.Z;
				//xNess=fShotSpeed*(ANGLEDIFFPOSITIVE(180,zRotDir)/180.0f*2.0f - 1.0f);
				//yNess=fShotSpeed*(ANGLEDIFFPOSITIVE(90,zRotDir)/1800.0f*2.0f - 1.0f);
				//if (zRotDir>=90.0f&&zRotDir<=270.0f) xNess*=-1.0f;
				static Mass3d m3dWeapon;
				m3dWeapon.X=EyeX();
				m3dWeapon.Y=EyeY();
				m3dWeapon.Z=EyeZ();
				m3dWeapon.yRot=fPitch;
				m3dWeapon.zRot=fYaw;
				for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
					if (arrpentShot[iShotNow] == NULL) {//create a shot at the first available shot pointer
						arrpentShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, false, false);
						(arrpentShot[iShotNow])->iIndex=iShotNow;
						iPlayLaser=SOUNDPAN_FROM3DX(m3dWeapon.X);
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
		if (narrGameStateCount[iGameState]==0) {//if (GAMESTATE_RUN_bFirstRun) {
			Console::Error.Write("Base_GetTicks_Relative...");
			Console::Error.Out.Flush();
		}
		int iTickRelNow=Base_GetTicks_Relative();//REAL rSecRelNow=Base_GetSeconds_Relative();
		int iTicksSinceLastRefresh=iTickRelNow-iTickLastRefresh;//REAL rSecondsSinceLastRefresh=rNow-rSecLastRefresh;

		if (true) {//!bFirstRunOfThisEntity) {
			// PHYSICS //
			if (narrGameStateCount[GAMESTATE_RUN]==0) {//GAMESTATE_RUN_bFirstRun
				Console::Error.Write("Physics...");
				Console::Error.Out.Flush();
			}
			if (bGravity) m3dEnt.zVel-=GRAVITY_METERS_PERSEC_PERFRAME; //only .8m/s/s gravitational acceleration (.8/30perframe), for high jumping //earth gravitational acceleration is 1.8 m/s/s
			if (narrGameStateCount[GAMESTATE_RUN]==0) {//GAMESTATE_RUN_bFirstRun
				Console::Error.Write("RotateTowardDest...");
				Console::Error.Out.Flush();
			}
			if ((iTicksSinceLastRefresh)>15)
				m3dEnt.RotateTowardDest(iTicksSinceLastRefresh);
			//m3dEnt.zRot=m3dEnt.zRotDest;
			//if (m3dEnt.yRot!=m3dEnt.yRotDest) m3dEnt.yRot=m3dEnt.yRotDest;
			//if (bRotationTakesPrecedence && m3dEnt.zRot!=m3dEnt.zRotDest) {
			//	 m3dEnt.zRot=m3dEnt.zRotDest;
			//}

			float fSpeedMultNow=rFortitude*fSpeedMultiplier;
			if (HasAttrib(STATUS_BOSS)) fSpeedMultNow=1.0f;


			if (bUsePitchYaw) {
				if (narrGameStateCount[iGameState]==0) {//(GAMESTATE_RUN_bFirstRun) {
					Console::Error.Write("{bUsePitchYaw:true}...");
					Console::Error.Out.Flush();
				}
				if ( (bMustMove)||((iTickRelNow-iTickLastMetricMove)>30) ) {//helps division accuracy on modern (fast) processors
					//int iTest=0;
					float fDist;
					fDist=MetersToMoveThisManyMS(fMetersPerSecond,iTickRelNow-iTickLastMetricMove);
					Travel3d(m3dEnt, m3dEnt.yRot, m3dEnt.zRot, fDist*fSpeedMultNow);
					iTickLastMetricMove=iTickRelNow;
					bMustMove=false;
				}//end if move (if enough time passed since last time to allow division accuracy)
			}
			else {
				if (narrGameStateCount[iGameState]==0) { //if (GAMESTATE_RUN_bFirstRun) {
					Console::Error.Write("{bUsePitchYaw:false}...");
					Console::Error.Out.Flush();
				}
				m3dEnt.X+=m3dEnt.xVel*fSpeedMultNow;
				m3dEnt.Y+=m3dEnt.yVel*fSpeedMultNow;
				m3dEnt.Z+=m3dEnt.zVel*fSpeedMultNow;
			}


			// HIT EDGES //

			bool bHitEdge=false,bHitMinX=false,bHitMinY=false,bHitMinZ=false,bHitMaxX=false,bHitMaxY=false,bHitMaxZ=false;
			if (narrGameStateCount[iGameState]==0) { //if (GAMESTATE_RUN_bFirstRun) {
				Console::Error.Write("{bHitEdge:"+RString_ToString(bHitEdge)+"}...");
				Console::Error.Out.Flush();
			}

			if	  (m3dEnt.X<m3dEnt.xMin) { bHitMinX=true; bHitEdge=true; }
			else if (m3dEnt.X>m3dEnt.xMax) { bHitMaxX=true; bHitEdge=true; }
			if	  (m3dEnt.Y<m3dEnt.yMin) { bHitMinY=true; bHitEdge=true; }
			else if (m3dEnt.Y>m3dEnt.yMax) { bHitMaxY=true; bHitEdge=true; }
			if	  (m3dEnt.Z<m3dEnt.zMin) { bHitMinZ=true; bHitEdge=true; }
			else if (m3dEnt.Z>m3dEnt.zMax) { bHitMaxZ=true; bHitEdge=true; }
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
								if ((iTickRelNow-iTickLastHitGroundSound)>300) {
									iPlayHitDirt=SOUNDPAN_FROM3DX(m3dEnt.X);
									bHitSoundWasPlayed=true;
									iTickLastHitGroundSound=iTickRelNow;
								}
								m3dEnt.Z+=(fabs(m3dEnt.Z-m3dEnt.zMin)/1.5f);
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
							float fDistFlatToHero=FPDIST(p3dHero.X,p3dHero.Y,m3dEnt.X,m3dEnt.Y);
							float fHeightDiff=p3dHero.Z-m3dEnt.Z;
							float fPitchToHero=FTHETAOFXY_DEG(fDistFlatToHero,fHeightDiff);
							if (fPitchToHero<-90) {
								Console::Error.Write("Programmer Error: fPitchToHero<-90 even though fDistFlatToHero is always positive...");
								fPitchToHero=180+fPitchToHero;//+ is ok since negative
								Console::Error.WriteLine("Adusted fPitchToHero to "+RString_ToString(fPitchToHero));
							}
							//float fYawToHero=FTHETAOFXY(m3dEnt.X-p3dHero.X,m3dEnt.Y-p3dHero.Y);
							if (bHitMinZ) {
								//m3dEnt.yRot*=-1;
								m3dEnt.yRotDest=IRand(40,55);
								m3dEnt.yRot=m3dEnt.yRotDest;//bounce off ground immediately
								static int hitbottomerror_iLimitedOutput=0;
								if (m3dEnt.yRotDest<40 || m3dEnt.yRotDest>55) {
									if (hitbottomerror_iLimitedOutput<100) Console::Error.WriteLine("Hit bottom but new yRot is "+RString_ToString(m3dEnt.yRot)); //debug only
									hitbottomerror_iLimitedOutput++;
								}
								//m3dEnt.Z=m3dEnt.zMin+.02;//debug only --shouldn't be needed//MetersToMoveThisManyMS(fMetersPerSecond,iTickRelNow-iTickLastRefresh);
								//if (m3dEnt.yRot<0.0f)m3dEnt.yRot+=360.0f;
								//if (m3dEnt.yRot>180.0f) m3dEnt.yRot=360.0f-m3dEnt.yRot;
							}
							else if (bHitMaxZ) {
								m3dEnt.yRotDest=fPitchToHero;
							}
							if (bHitMinX||bHitMaxX||bHitMinY||bHitMaxY) {
								m3dEnt.yRotDest=fPitchToHero;
								m3dEnt.zRotDest=AngleToward(p3dHero.X, p3dHero.Y, m3dEnt.X, m3dEnt.Y);
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
								iPlayScrapeGround=SOUNDPAN_FROM3DX(m3dEnt.X);
								if (iType!=ENTITY_TYPE_BOSS) fSpeedMultiplier=0.3;
							}
						}
						else {
							if (bHitMinZ) {
								iPlayScrapeGround=SOUNDPAN_FROM3DX(m3dEnt.X);
								m3dEnt.zVel=-m3dEnt.zVel;
								if (iType!=ENTITY_TYPE_BOSS) fSpeedMultiplier=0.3;
							}
							else if (bHitMaxZ) m3dEnt.zVel=-m3dEnt.zVel;
							if (bHitMinX) m3dEnt.xVel=-m3dEnt.xVel; //TODO: deform terrain here
							else if (bHitMaxX) m3dEnt.xVel=-m3dEnt.xVel;
							if (bHitMinY) m3dEnt.yVel=-m3dEnt.yVel;
							else if (bHitMaxY) m3dEnt.yVel=-m3dEnt.yVel;
						}//end else NOT bUsePitchYaw
						/*
						if (bUsePitchYaw) {
							m3dEnt.zRot=FTHETAOFXY(m3dEnt.xVel,m3dEnt.yVel);
							m3dEnt.zRotDest=m3dEnt.zRot;
							m3dEnt.yRotDest=FTHETAOFXY(m3dEnt.xVel,m3dEnt.zVel);
							m3dEnt.xRotDest=FTHETAOFXY(m3dEnt.yVel,m3dEnt.zVel);
						}
						*/
						break;
					case BOUNDARY_DIE:
						if (bHitEdge) {
							Stop();
							if (bHitMinZ && HasAttrib(STATUS_ALIVE)) {
								DeformTerrain();
								iPlayHitDirt=SOUNDPAN_FROM3DX(m3dEnt.X);
							}
							RemoveAttrib(STATUS_ALIVE);
						}
						break;
					default:
						break;
				}//end switch
			}//end if bHitEdge
			//bCycleFrames=true;//debug only
			if (bCycleFrames) {
				if (lpanimMain!=null) {
					if (lpanimMain->LastFrame()) lpanimMain->GotoFrame(0);
					else lpanimMain->GotoNextFrame();
				}
			}
			else CheckFrames(); //DOES set iFrame using FrameFromRot else cycles frames
		}//end (true) [used to run only if bFirstRunOfThisEntity]
		else bFirstRunOfThisEntity=false;
		if (fMetersPerSecond<fMetersPerSecondMax) fMetersPerSecond=APPROACH(fMetersPerSecond,fMetersPerSecondMax,.5);

		//entity-type-specific statements

		if (narrGameStateCount[iGameState]==0) { //if (GAMESTATE_RUN_bFirstRun) {
			Console::Error.Write("Entity-specific actions...");
			Console::Error.Out.Flush();
		}
		if (iType==ENTITY_TYPE_HERO) {
			// UPDATE //
			DrawMeters();
			if (iFramesMoveDelay) iFramesMoveDelay--;
			if (iFramesMoveDelay<0) iFramesMoveDelay=0;
			if (iFramesAimDelay) iFramesAimDelay--;
			if (iFramesAimDelay<0) iFramesAimDelay=0;
			if (iFramesShootDelay) iFramesShootDelay--;
			if (iFramesShootDelay<0) iFramesShootDelay=0;
			if (rFortitude<1.0f) rFortitude+=.1f;
			if (rFortitude>1.0f) rFortitude=1.0f;
			if (fSpeedMultiplier<1.0f) fSpeedMultiplier+=.1f;
			if (fSpeedMultiplier>1.0f) fSpeedMultiplier=1.0f;

			//re-charge gun
			float fPowerMaxNow=fPowerMax;
			float fChargeAddNow=.033f;
			if (u32Status & STATUS_DOUBLESPEED) {
				fPowerMaxNow*=2.0f;
				fChargeAddNow*=2.0f;
			}
			fPower+=(fPower<fPowerMaxNow*.66f)?fChargeAddNow:fChargeAddNow*.66f;
			if (fPower>fPowerMaxNow) fPower=fPowerMaxNow;

			// HITDETECT //
			if (rFortitude>=1.0) DamageDetect();
			// CHECK LIFE //
			if (iType==ENTITY_TYPE_HERO && fHealth<=0) u32Status=0; //died, stop other special abilities too
			if (!(u32Status & STATUS_ALIVE)) {
				if (!bExploding) {
					iPlayExplosion=SOUNDPAN_FROM3DX(m3dEnt.X);
					bExploding=true;
				}
				else fExplodedness+=.1f;
				if (fExplodedness>1.0f) fExplodedness=1.0f;
			}
		}//end if ENTITY_TYPE_HERO
		else if (iType==ENTITY_TYPE_ALIEN||iType==ENTITY_TYPE_BOSS) {
			// HITDETECT //
			if (rFortitude<1.0f) rFortitude+=.03f;
			if (rFortitude>1.0f) rFortitude=1.0f;
			if (fSpeedMultiplier<1.0f) fSpeedMultiplier+=.03f;
			if (fSpeedMultiplier>1.0f) fSpeedMultiplier=1.0f;
			if (iFramesShootDelay) iFramesShootDelay--;
			if (iFramesShootDelay<0) iFramesShootDelay=0;
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
					iPlayExplosion=SOUNDPAN_FROM3DX(m3dEnt.X);
					if (iType==ENTITY_TYPE_ALIEN||iType==ENTITY_TYPE_BOSS) {
						iPlayOuchAlien=SOUNDPAN_FROM3DX(m3dEnt.X);
					}
					bExploding=true;
				}
				else fExplodedness+=.1f;
				if (fExplodedness>1.0f) fExplodedness=1.0f;
			}
		}//end if ENTITY_TYPE_ALIEN || ENTITY_TYPE_BOSS
		//else if (iType==ENTITY_TYPE_SHOT) {//TODO: ok to remove iType check since rSecondsLife compared to zero below
		if (narrGameStateCount[iGameState]==0) {//if (GAMESTATE_RUN_bFirstRun) {
			Console::Error.Write("check life...");
			Console::Error.Out.Flush();
		}
		if ((iTicksLife>0)&&(iTickRelNow-iTickThisEntityCreated>iTicksLife)) {
			RemoveAttrib(STATUS_ALIVE);
		}
		//} //end if ENTITY_TYPE_SHOT
		if (narrGameStateCount[iGameState]==0) {//if (GAMESTATE_RUN_bFirstRun) {
			Console::Error.Write("Translate3D...");
			Console::Error.Out.Flush();
		}
		Translate3D();
		iTickLastRefresh=iTickRelNow;
		IPoint ipDest;
		ipDest.X=m2dEnt.rectRender.left+SCREEN_OFFSET_X;
		ipDest.Y=m2dEnt.rectRender.top+SCREEN_OFFSET_Y;
		if (iType==ENTITY_TYPE_HERO) {
			if (narrGameStateCount[iGameState]==0) {//if (GAMESTATE_RUN_bFirstRun) {
				Console::Error.Write("ENTITY_TYPE_HERO post-translate-actions...");
				Console::Error.Out.Flush();
			}
			p3dHero.X=m3dEnt.X;
			p3dHero.Y=m3dEnt.Y;
			p3dHero.Z=m3dEnt.Z;
			if (narrGameStateCount[iGameState]==0) {//if (GAMESTATE_RUN_bFirstRun) {
				Console::Error.Write("TypeFast...");
				Console::Error.Out.Flush();
				Console::Error.Write("gbScreen:"+gbScreen.Description()+"...");
				Console::Error.Out.Flush();
				Console::Error.Write("ipDest:"+ipDest.ToString()+"...");
				Console::Error.Out.Flush();
				Console::Error.Write("m3dEnt.Z:"+RString_ToString(m3dEnt.Z)+"...");
				Console::Error.Out.Flush();
				Console::Error.Write("m3dEnt.ToString(false):"+m3dEnt.ToString(false)+"...");
				Console::Error.Out.Flush();
			}
		}
		if (bDebug) {
			if (iType==ENTITY_TYPE_HERO) {
				gfontDefault.TypeFast(gbScreen, ipDest, "hero:"+m3dEnt.ToString(true),0,DrawModeBlendAlpha);
			}
			else if (iType==ENTITY_TYPE_ALIEN) {
				gfontDefault.TypeFast(gbScreen, ipDest, "alien["+RString_ToString(iIndex)+"]:"+m3dEnt.ToString(true),0,DrawModeBlendAlpha);
			}
			else if (iType==ENTITY_TYPE_BOSS) {
				gfontDefault.TypeFast(gbScreen, ipDest, "boss:"+m3dEnt.ToString(true),0,DrawModeBlendAlpha);
			}
			else if (iType==ENTITY_TYPE_SHOT) {
				gfontDefault.TypeFast(gbScreen, ipDest, "shot["+RString_ToString(iIndex)+"]:"+m3dEnt.ToString(),0,DrawModeBlendAlpha);
			}
			else {
				gfontDefault.TypeFast(gbScreen, ipDest, "unknown-entity-type:"+m3dEnt.ToString(),0,DrawModeBlendAlpha);
			}
		}//end if bDebug show locations of objects
		if (narrGameStateCount[iGameState]==0) {//if (GAMESTATE_RUN_bFirstRun) {
			Console::Error.Write("done Entity::Refresh...");
			Console::Error.Out.Flush();
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
						"self"); //TODO: change color a different way (not explodedness) otherwise make a new shot animation for aliens
				}
				else RemoveAttrib(STATUS_ALIVE);
			}
			else if (iType==ENTITY_TYPE_ALIEN||iType==ENTITY_TYPE_BOSS) {
				float fShadowOpacityNow;
				fShadowOpacityNow=(float)fShadowOpacity*(1.2f-(m3dEnt.Z/m3dEnt.zMax));
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
				fShadowOpacityNow=(float)fShadowOpacity*(1.2f-(m3dEnt.Z/m3dEnt.zMax));
				if (fShadowOpacityNow>fShadowOpacity) fShadowOpacityNow=fShadowOpacity;
				else if (fShadowOpacityNow<0.0f) fShadowOpacityNow=0.0f;
				if (bShadow) {
					//debug only (real version is below):
					SayWhatIDrewIfFalse(GBuffer_FX_Scaled(gbScreen, lpanimShadow->gbFrame, SCREEN_OFFSET_X+m2dShadow.rectRender.left, SCREEN_OFFSET_Y+m2dShadow.rectRender.top, fShadowOpacityNow, 0.0f, 0, m2dShadow.fScale),
							iType,
							"shadow");
					//commented for debug only (crashes WITHOUT warning nor output):
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
			}//end else if (iType==ENTITY_TYPE_HERO)

			if (bDebug) {
				ShowDebugInfo();
			}
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"Entity::Draw");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("Entity::Draw");
		}
	}//end Draw
	float Entity::EyeX() {
		//float fFrontEccentricityPixels=11;
		SafeAngle360ByRef(m3dEnt.zRot);//commented for debug only ////m3dEnt.zRot-=FFLOOR(m3dEnt.zRot/360.0f)*360.0f;
		float fApparentDegFromOrthagonalY=ANGLEDIFFPOSITIVE(90.0f,m3dEnt.zRot);
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
					0, ANGLEDIFFPOSITIVE(m3dEnt.zRot,90.0f),
					pixelNow,
					3,0);
			pixelNow.g=0;
			pixelNow.b=128;
			gbScreen.DrawSubpixelArc(FSCREEN_WIDTH/2+64+SCREEN_OFFSET_X, FSCREEN_HEIGHT/2-64+FSCREEN_OFFSET_Y,
					10, 1, 0,
					0, fApparentDegFromOrthagonalY,
					pixelNow,
					3,0);
		}
		fOutness/=90.0f;
		//if (fOutness>1.0f) fOutness=1.0f;//this should never happen
		float xQuads1And2Modifier=0.0f;
		if ( (m3dEnt.zRot<180&&m3dEnt.zRot>0.0f) || (m3dEnt.zRot<0.0f&&m3dEnt.zRot>-180.0f) ) {
			xQuads1And2Modifier=PixelsToMetersAtMyLocation(20)*(1.0f-fOutness);
			//TODO: finish this--fix location when pointing "away" from screen
		}
		float fHeadRange=PixelsToMetersAtMyLocation(72.0f);//=m2dEnt.fScale*m3dEnt.xSize*1.0;
		float fEye=m3dEnt.X+(fHeadRange/2.0f)*fOutness+xQuads1And2Modifier;//=m3dEnt.X+fHeadRange*fOutness-.1;
		//fEye=ANGLEDIFFPOSITIVE(90.0f,m3dEnt.zRot)/90.0f;//debug only
		//static int iTest=0; //debug only
		//if (iTest<100) {
		//	Console::Error.WriteLine("{"
		//	+"fApparentDegFromOrthagonalY:"+fApparentDegFromOrthagonalY+"; "
		//	+"fOutness:"+fOutness+"; "
		//	+"fHeadRange:"+fHeadRange+"; "
		//	+"EyeX():"+fEye+"; "
		//	+"}");
		//   iTest++;
		//}
		//fEye=m3dEnt.X+xHeadRange*fOutness;
		//fEye=m3dEnt.X;
		return fEye;
	}//end EyeX
	float Entity::EyeY() {
		float fEye;
		fEye=m3dEnt.Y;
		return fEye;
	}//end EyeY
	float Entity::EyeZ() {
		return (m3dEnt.Z+m3dEnt.zSize/2.0f-PixelsToMetersAtMyLocation(10.0f))+PixelsToMetersAtMyLocation(54.0f);
	}//end EyeZ
	void Entity::Translate3D() {
		camera.Mass2dFrom3d(m2dEnt, m2dShadow, m3dEnt);
	}
	void Entity::DrawMeters() {
		bool bGood=false;
		//register UInt32* lpu32Dest=(UInt32*)gbScreen.arrbyData;
		//TODO: improve this, don't use low-level writing
		//register byte *byteBuffer=gbScreen.arrbyData;
		//byteBuffer+=SCREEN_OFFSET_Y*lptargaScreen->width*4+SCREEN_OFFSET_X*4;
		//int iScreenStride=gbScreen.iStride;
		register int x2D=32+SCREEN_OFFSET_X, y2D=SCREEN_HEIGHT-3+SCREEN_OFFSET_Y;
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
				//lpu32Dest[x2D + ((y2D)*iScreenStride >> 2)]=pixel;
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
					gbScreen.DrawSubpixelDot(x2D+xNow,y2D-iNotch,pixelNow);
					//lpu32Dest[(x2D + xNow) + (y2D-iNotch)*BUFFER_WIDTH]=arru32EnergyGrad[iVal];
				//}
			}
		}

		x2D+=32;
		fMax=fHealthMax*100.0f;
		fMaxNow=(fHealth/fHealthMax)*100.0f;
		iMaxNow=IROUNDF(fMaxNow);
		for (iNotch=0; iNotch<=fMaxNow; iNotch++) {
			//pixel=_RGB32BIT(255,55+iX*2,55+iX*2,155+iX);
			for (register int xNow=0; xNow<=13; xNow++) {
				//lpu32Dest[x2D + ((y2D)*iScreenStride >> 2)]=pixel;
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
					gbScreen.DrawSubpixelDot(x2D+xNow,y2D-iNotch,pixelNow);
					//lpu32Dest[(x2D + xNow) + (y2D-iNotch)*BUFFER_WIDTH]=arru32HealthGrad[iVal];
				//}
			}
		}
		int yStart=y2D-gbSymbolShield.iHeight;
		x2D+=32;
		if (HasAttrib(STATUS_VARIABLESHIELD)) {
			//TODO: make a non-scaled function for next line
			//GBuffer_FX(gbScreen,gbSymbolShield,x2D, yStart, ( (fShield<1.0f) ? ((fShield>0.0f)?fShield:0.0f) : 1.0f ), DrawModeBlendAlpha );
			GBuffer_FX_Scaled(gbScreen,gbSymbolShield,x2D, yStart, ( (fShield<1.0f) ? ((fShield>0.0f)?fShield:0.0f) : 1.0f ), 0.0f, 0, 1.0f);
			x2D+=gbSymbolShield.iWidth+(int)( (float)gbSymbolShield.iWidth/2.0f );
		}
		try {
			if (iBoss>=0) {
				yStart=y2D-gbSymbolBossHealth.iHeight;
				//float fPixMin=x2D+32;
				//float fPixMax=FSCREEN_WIDTH-fPixMin;
				//float yMax=FSCREEN_HEIGHT-10;
				//float yMin=FSCREEN_HEIGHT-20;
				fRatio=fBoss/fBossMax;
				//TODO: make a non-scaled function for next line
				//GBuffer_FX(gbScreen, gbSymbolBossHealth, x2D, yStart, ( (fShield<1.0f) ? ((fRatio>0.0f)?fRatio:0.0f) : 1.0f ), 0.0f, 0, 1.0f );
				GBuffer_FX_Scaled(gbScreen, gbSymbolBossHealth, x2D, yStart, ( (fShield<1.0f) ? ((fRatio>0.0f)?fRatio:0.0f) : 1.0f ), 0.0f, 0, 1.0f );
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
				gbScreen.DrawSubpixelArc(128+SCREEN_OFFSET_X, FSCREEN_HEIGHT-64+FSCREEN_OFFSET_Y,
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
			ShowExn(exn,"entity.DrawMeters");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("entity.DrawMeters");
		}
	}//end DrawMeters
	void Entity::DrawTarget(int alienNum) {
		//for (int alienNow=0; alienNow < iMaxAliens; alienNow++)
		//{
			//if (arrpentAlien[alienNum] != NULL)
			//TODO: remove or fix calling function for new aiming
			int iX=alienNum;
			if (arrpentAlien[iX]==NULL) return;
			Entity* lpAlienNow=arrpentAlien[iX];
			float zOurRange=m3dEnt.zSize/2+lpAlienNow->m3dEnt.zSize/2;
			//I switched the right and left edges on purpose to be more inclusive
			//since screen offset prevents bad writes
			if (lpAlienNow->m3dEnt.Z-m3dEnt.Z<zOurRange && lpAlienNow->m2dEnt.rectRender.right>0 && lpAlienNow->m2dEnt.rectRender.left<SCREEN_WIDTH ) {
				//register UInt32* lpu32Dest =
				register byte *byteBuffer=gbScreen.arrbyData;//TODO: avoid low-level operations
				static int iScreenStride=gbScreen.iStride;
				//register UInt32 pixel;//=_RGB32BIT(255,255,255,255); //remember this MACRO is ARGB unlike actual screen
				register int x2D=lpAlienNow->m2dEnt.rectRender.left, y2D=lpAlienNow->m2dEnt.rectRender.top;
				x2D+=SCREEN_OFFSET_X;
				y2D+=SCREEN_OFFSET_Y;
				for (int iX=0; iX<7; iX++) {
					for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++) {
					//draw crosshairs
						byteBuffer[(x2D+iX)*REAL_BYTEDEPTH + (y2D+3)*iScreenStride + byteNow]=(byteNow==1)? 255 : 0; //across
						byteBuffer[(x2D+3)*REAL_BYTEDEPTH + (y2D+iX)*iScreenStride + byteNow]=(byteNow==1)? 255 : 0; //down
					}
				}

	/*			x2D=lpAlienNow->m3dEnt.X;
				y2D=lpAlienNow->m3dEnt.Y-2;
				for (iX=0; iX<5; iX++)
				{

					for (register int byteNow=0; byteNow<REAL_BYTEDEPTH; byteNow++)
						byteBuffer[(x2D)*REAL_BYTEDEPTH + (y2D+iX)*iScreenStride + byteNow]=(byteNow==1)? 255 : 0;
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
				delete arrpentShot[iShotIndex];
				arrpentShot[iShotIndex]=NULL;
				if (!bExploding) {
					iPlayShieldZap=SOUNDPAN_FROM3DX(m3dEnt.X);
				}
			}
			else {
				rFortitude=.1;
				fHealth-=fLaserSusceptibility;
				delete arrpentShot[iShotIndex];
				arrpentShot[iShotIndex]=NULL;
				if (!bExploding) {
					iPlayOuchAlien=SOUNDPAN_FROM3DX(m3dEnt.X);
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
						lpAlienNow=arrpentAlien[iAlien];
						float xOurRange=lpAlienNow->m3dEnt.xSize/2+xMyRange;
						float yOurRange=lpAlienNow->m3dEnt.ySize/2+yMyRange;
						float zOurRange=lpAlienNow->m3dEnt.zSize/2+zMyRange;
					//CENTERPOINT DISTANCE-BASED
						if ( abs(lpAlienNow->m3dEnt.X-m3dEnt.X) < xOurRange) {
							if ( abs(lpAlienNow->m3dEnt.Y-m3dEnt.Y) < yOurRange) {
								if (abs(lpAlienNow->m3dEnt.Z-m3dEnt.Z) < zOurRange) {
									if (HasAttrib(STATUS_SHIELD)) {
										RemoveAttrib(STATUS_SHIELD);//fHealth+=fLaserSusceptibility*3.0f;//.196;
										//make alien blurrp as well as us zap
										rFortitude=.1f;//prevents multiple hits
										iPlayBlerrp=SOUNDPAN_FROM3DX(m3dEnt.X);
										iPlayShieldZap=SOUNDPAN_FROM3DX(m3dEnt.X);
									}
									else if (HasAttrib(STATUS_VARIABLESHIELD)) {
										RemoveAttrib(STATUS_SHIELD);
										//make alien blurrp as well as us zap
										iPlayBlerrp=SOUNDPAN_FROM3DX(m3dEnt.X);
										iPlayShieldZap=SOUNDPAN_FROM3DX(m3dEnt.X);
										rFortitude=.1f;//prevents multiple hits
										fShield-=fLaserSusceptibility*3.0f;//.196
										if (fShield<=0.0f) RemoveAttrib(STATUS_VARIABLESHIELD);
									}
									else {
										iPlayBlerrp=SOUNDPAN_FROM3DX(m3dEnt.X);
										iPlayOuchZap=SOUNDPAN_FROM3DX(m3dEnt.X);
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
					if (arrpentShot[iShotNow] != NULL) {//DISTANCE FORMULA: uses centerpoint (m3dEnt.X,m3dEnt.Y,m3dEnt.Z)
						lpShotNow=arrpentShot[iShotNow];
						float xOurRange=lpShotNow->m3dEnt.xSize/2.0f+xMyRange;
						float yOurRange=lpShotNow->m3dEnt.ySize/2.0f+yMyRange;
						float zOurRange=lpShotNow->m3dEnt.zSize/2.0f+zMyRange;
						if (abs(lpShotNow->m3dEnt.X-m3dEnt.X) < xOurRange) {
							if (abs(lpShotNow->m3dEnt.Y-m3dEnt.Y) < yOurRange) {
								if (abs(lpShotNow->m3dEnt.Z-m3dEnt.Z) < zOurRange && (lpShotNow->bAlien)) {
									if (HasAttrib(STATUS_SHIELD)) {
										RemoveAttrib(STATUS_SHIELD);
										rFortitude=.1f; //prevents multiple hits
										//fHealth+=fLaserSusceptibility;//.059;
										delete arrpentShot[iShotNow];
										arrpentShot[iShotNow]=NULL;
										iPlayShieldZap=SOUNDPAN_FROM3DX(m3dEnt.X);
									}
									else if (HasAttrib(STATUS_VARIABLESHIELD)) {
										rFortitude=.1f; //prevents multiple hits
										fShield-=fLaserSusceptibility*3.0f;//.196//fHealth+=fLaserSusceptibility;//.059;
										if (fShield<=0.0f) RemoveAttrib(STATUS_VARIABLESHIELD);
										delete arrpentShot[iShotNow];
										arrpentShot[iShotNow]=NULL;
										iPlayShieldZap=SOUNDPAN_FROM3DX(m3dEnt.X);
									}
									else {
										rFortitude=.1f;
										fHealth-=fLaserSusceptibility;//.059;
										delete arrpentShot[iShotNow];
										arrpentShot[iShotNow]=NULL;
										iPlayOuchZap=SOUNDPAN_FROM3DX(m3dEnt.X);
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
					if (arrpentShot[iShotNow]!=NULL) {//DISTANCE FORMULA: uses centerpoint (m3dEnt.X,m3dEnt.Y,m3dEnt.Z)
						lpShotNow=arrpentShot[iShotNow];
						float xOurRange=lpShotNow->m3dEnt.xSize/2.0f+xMyRange;
						float yOurRange=lpShotNow->m3dEnt.ySize/2.0f+yMyRange;
						float zOurRange=lpShotNow->m3dEnt.zSize/2.0f+zMyRange;
						if (abs(lpShotNow->m3dEnt.Y-m3dEnt.Y) < yOurRange) {
							if (abs(lpShotNow->m3dEnt.X-m3dEnt.X) < xOurRange) {
								if ( (abs(lpShotNow->m3dEnt.Z-m3dEnt.Z) < zOurRange) && (!lpShotNow->bAlien)) {
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
			ShowExn(exn,"damage detect");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("damage detect");
		}
	}//END DAMAGE DETECT
	void Entity::SetDoubleSpeed() {
		if (0==(u32Status & STATUS_DOUBLESPEED)) {
			u32Status |= STATUS_DOUBLESPEED;
			bPlayTrumpet=true;
			m3dEnt.zRotVelDegreesPerSec*=2.0f;
			m3dEnt.yRotVelDegreesPerSec*=2.0f;
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
	void Entity::Bomb(float x2D, float y2D, float zDest) {
		if (iFramesShootDelay<=0) {
			if (u32Status&STATUS_BOSS) iFramesShootDelay=IRand(5,15);
			else iFramesShootDelay=IRand(10,20);
			float xEye=m3dEnt.X-m3dEnt.xSize/2.0f;
			if (fShootMetersPerSec<6.0f) fShootMetersPerSec=6.0f;
			float xSpeed,ySpeed,zSpeed;
			float fArrivalFrames=60.0f;
			if (u32Status&STATUS_BOSS) fArrivalFrames=(float)(IRand(30,60));
			xSpeed=(x2D-m3dEnt.X)/fArrivalFrames;
			ySpeed=(y2D-m3dEnt.Y)/fArrivalFrames;
			zSpeed=(zDest-m3dEnt.Z)/fArrivalFrames;
			for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
				if (arrpentShot[iShotNow] == NULL) {
				//create a shot at the first available shot pointer
					iPlayBomb=SOUNDPAN_FROM3DX(m3dEnt.X);
					arrpentShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, xEye, m3dEnt.Y, m3dEnt.Z, xSpeed, ySpeed, zSpeed, bAlien, false);
					(arrpentShot[iShotNow])->iIndex=iShotNow;
					break;
				}
			}
		}
	}
	void Entity::Bomb(){
		if (iFramesShootDelay<=0) {
			if (u32Status&STATUS_BOSS) iFramesShootDelay=IRand(5,15);
			else iFramesShootDelay=IRand(10,20);
			//float xEye=m3dEnt.X-m3dEnt.xSize/2.0f;
			for (int iShotNow=0; iShotNow<MAXSHOTS; iShotNow++) {
				if (arrpentShot[iShotNow] == NULL) {
				//create a shot at the first available shot pointer
					iPlayBomb=SOUNDPAN_FROM3DX(m3dEnt.X);
					//Assume Shoot left
					//Prototype: Shot(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, int isRed2)
					fShootMetersPerSec=2.0f*(float)(IRand(1,4));
					Mass3d m3dWeapon;
					m3dWeapon.X=m3dEnt.X;
					m3dWeapon.Y=m3dEnt.Y;
					m3dWeapon.Z=m3dEnt.Z;
					m3dWeapon.zRotDest=(float)(IRand(0,360));
					m3dWeapon.yRotDest=-(float)(IRand(0,45));
					arrpentShot[iShotNow]=new Entity(ENTITY_TYPE_SHOT, m3dWeapon, fShootMetersPerSec, bAlien, false);
					(arrpentShot[iShotNow])->iIndex=iShotNow;
					break;
				}
			}
		}
	}//end Bomb()
	void Entity::DeformTerrain() {
		bool bGood=true;
		try {
			if (lpanimShadow->gbFrame.arrbyData!=NULL) {
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
				if (bDraw) {//formerly used animBurn
					if (iType==ENTITY_TYPE_SHOT) {
						//iFrameBurnNow++;
						if (lpanimShadow->LastFrame()) lpanimShadow->GotoFrame(0);//else if (iFrameBurnNow<0) iFrameBurnNow=0;
						else lpanimShadow->GotoNextFrame();//if (iFrameBurnNow>=lpanimShadow->IFrames()) iFrameBurnNow=0;
						float fPixelHeight=(float)(m2dEnt.rectRender.bottom-m2dEnt.rectRender.top);
						if ( (lpanimShadow->Height()>0) && (fPixelHeight>5.5f) ) {
							//done above: lpanimShadow->GotoFrame(iFrameBurnNow);//done below: TargaToTarga32(lpanimShadow->gbFrame, lptargaBackdropNow, m2dEnt.rectRender.left, m2dEnt.rectRender.top);
							//float fPixelHeightSource=(float)lpanimShadow->Height();
							SayWhatIDrewIfFalse( GBuffer_FX_Scaled(   animBackdrop.gbFrame, lpanimShadow->gbFrame,
								m2dEnt.rectRender.left, m2dEnt.rectRender.top,
								0.9f/*opacity*/, 0/*explodedness*/, 0/*attributes*/, m2dEnt.fScale),//2.0f*(fPixelHeight/fPixelHeightSource)),
								iType,
								"turf burn");
						}
					}//end if draw shot, draw burn mark instead of shadow
					else {
						SayWhatIDrewIfFalse(GBuffer_FX_Scaled(animBackdrop.gbFrame, lpanimShadow->gbFrame, m2dEnt.rectRender.left, m2dEnt.rectRender.top, .078, 0, 0, m2dShadow.fScale),
								iType,
								"turf damage");
					}//end else draw damage as shadow since not shot
				}//end if drawable here
			}
			//else lpanimShadow is null
		}
		catch (exception& exn) {
			bGood=false;
			ShowExn(exn,"DeformTerrain");
		}
		catch (...) {
			bGood=false;
			ShowUnknownExn("DeformTerrain");
		}
	}//end DeformTerrain
	void Entity::SetVars(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, bool IsAlien, bool IsBomb){
		m3dEnt.X=x2;
		m3dEnt.Y=y2;
		m3dEnt.Z=z2;
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
		m3dDest.X=m3dEnt.X;
		m3dDest.Y=m3dEnt.Y;
		m3dDest.Z=m3dEnt.Z;

		if (bUsePitchYaw) {
			Travel3d(m3dDest, m3dEnt.yRot, m3dEnt.zRot, fMetersPerSecond);
			float fOpacityOfFar=ANGLEDIFFPOSITIVE(m3dEnt.yRot,0);//always near RELATIVE to m3d, not necessarily to camera (see below for swap when not)
			if (fOpacityOfFar>180) {
				Console::Error.WriteLine("Math usage error: ANGLEDIFFPOSITIVE returned greater than 180 {ANGLEDIFFPOSITIVE("+RString_ToString(m3dEnt.yRot)+",0):"+RString_ToString(fOpacityOfFar)+"}");
				//fOpacityOfFar=fOpacityOfFar-180;
			}
			if (fOpacityOfFar>90) {
				fOpacityOfFar=180-fOpacityOfFar;
			}
			//if (fOpacityOfFar<0) fOpacityOfFar=180+fOpacityOfFar;
			fOpacityOfFar=0.0f;//(fOpacityOfFar)/90.0f;//the closer it is to zero (angled toward horizon) the less opaque far pixel will be (unless flipped when point far from object is closer to camera)
			byte byFirstAlpha=255;//=0;
			byte byLastAlpha=SafeByRoundF((fOpacityOfFar)*255.0f);
			if (DIST3D(camera.m3dCam,m3dDest)<DIST3D(camera.m3dCam,m3dEnt)) {
				byte bySwap=byFirstAlpha;
				byFirstAlpha=byLastAlpha;
				byLastAlpha=bySwap;
			}
			pixelNear.Set(255,255,255,byFirstAlpha);
			pixelFar.Set(255,255,255,byLastAlpha);
		}
		else {
			pixelNear.Set(255,255,0,0);
			pixelFar.Set(255,255,0,255);
			m3dDest.X+=m3dEnt.xVel;
			m3dDest.Y+=m3dEnt.yVel;
			m3dDest.Z+=m3dEnt.zVel;
		}
		FPoint pointSrc, pointDest;
		camera.Point2dFrom3dWithScreenOffset(pointSrc,m3dEnt,FSCREEN_OFFSET_X,FSCREEN_OFFSET_Y);
		camera.Point2dFrom3dWithScreenOffset(pointDest,m3dDest,FSCREEN_OFFSET_X,FSCREEN_OFFSET_Y);
		gbScreen.DrawSubpixelLine( pointSrc, pointDest, pixelNear, &pixelFar, 1);
		//gbScreen.DrawAlphaPix(pointSrc.X,pointSrc.Y,pixelNear.r,pixelNear.g,pixelNear.b,255);//start cap
		//gbScreen.DrawAlphaPix(pointDest.X,pointDest.Y,pixelFar.r,pixelFar.g,pixelFar.b,255);//end cap
	}//end ShowDebugInfo
	float Entity::PixelsToMetersAtMyLocation(float fPixels) {//formerly MetersFromPixels (MetersFromPixelsAtThisLocation? MetersFromPixelsHere?)
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
}//end namespace
#endif
