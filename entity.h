#ifndef ENTITY_H
#define ENTITY_H

#include "base.h"
//#include "targastruct.h"
#include <RAnim_bgra32.h>

using namespace std;

namespace ExpertMultimediaBase {
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
		int iIndex;//my index in entity array

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
		void DeformTerrain();
		void SetVars(int x2, int y2, int z2, int xVel2, int yVel2, int zVel2, bool IsAlien, bool IsBomb);
		float PixelsToMetersAtMyLocation(float fPixels);
		void Stop();
		bool HasAttrib(UInt32 bit);
		void RemoveAttrib(UInt32 bit);
		void AddVariableShield(float fSetShieldTo);
	private:
		//shot vars:
		REAL rSecThisEntityCreated;
		REAL rSecondsLife;
		bool bFirstRunOfThisEntity;
		int iDir;
		REAL rSecLastRefresh;
		REAL rSecLastMetricMove; //only used for metric movement as of 2007
		//int iFrameBurn;

		void DrawMeters();
		int FrameFromRot();
		void ShowDebugInfo();
		//float BottomOffsetHere();
	};
}//end namespace

#endif
