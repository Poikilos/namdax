#ifndef ENTITY_H
#define ENTITY_H

#include <base.h>
//#include "targastruct.h"
#include <RAnim_bgra32.h>

using namespace std;


namespace ExpertMultimediaBase {

	extern int unnamed_index;
	extern double HITDETECTION2D_CONSTANT_Z_DBL;
	class Entity {
	public:
		string NameUnique;
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
		Mass3d m3dWeapon;
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
		Anim* lpanimMain;
		Anim* lpanimShadow;
		int iFramesMoveDelay; //delays motion by frames
		int iFramesAimDelay;
		int iFramesShootDelay;
		UInt32 u32Status;
		float fHealth;
		float fExplodedness;
		float fPower; //gun power in percent, max=1
		bool bExploding;
		bool bGravity;
		bool bMustMove;
		int iIndex;//my index in entity array
		Pixel shield_color;
		Pixel variable_shield_color;

		void InitNull();
		Entity(int iTypeX, string set_UniqueName);
		Entity(int iTypeX, string set_UniqueName, Mass3d m3dDirectionAndLocation, float fMetersPerSec, bool IsAlien, bool IsBomb);
		Entity(int iTypeX, string set_UniqueName, float x2, float y2, float z2, float xVel2, float yVel2, float zVel2, bool IsAlien, bool IsBomb);
		Entity(int iTypeX, string set_UniqueName, float x2, float y2, float z2);
		~Entity();
		void Bomb(float x2D, float y2D, float zDest);
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
		void RefreshWeapon();
		void Shoot(int iDir);
		void Shoot(float fPitch, float fYaw);
		void SetDoubleSpeed();
		void SetRapidFire();
		void DrawTarget(int alienNum);
		//void GetHit(int iShotIndex); see global function DoCollision instead
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
		int iTickThisEntityCreated;//formerly iTickStart
		int iTicksLife;
		bool bFirstRunOfThisEntity;
		int iDir;
		int iTickLastRefresh;
		int iTickLastMetricMove; //only used for metric movement as of 2007
		//int iFrameBurn;
		void DrawMeters();
		int FrameFromRot();
		void ShowDebugInfo();
		//float BottomOffsetHere();
	};
}//end namespace

#endif
