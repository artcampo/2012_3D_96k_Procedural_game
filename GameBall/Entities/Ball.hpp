#ifndef _GameBP_Ball_HPP_
#define _GameBP_Ball_HPP_

#include "dx_misc.hpp"
#include <vector>

#include  "physics/Physics.hpp" 
#include  "GameBall/GameBp.hpp"
#include  "physics/RigidBody.hpp"
#include  "Misc/Log.hpp"
#include  "RenderModels/GameBp/BallModel.hpp"
#include  "RenderModels/GameBp/Balls/BallMetal.hpp"
#include  "RenderModels/GameBp/Balls/BallGlass.hpp"
#include  "RenderModels/GameBp/Balls/BallMeteor.hpp"
#include  "RenderModels/GameBp/Balls/BallBouncy.hpp"
#include  "RenderModels/GameBp/Balls/BallDemo.hpp"
#include  "RenderModels/GameBp/Balls/BallChase.hpp"
#include  "RenderModels/GameBp/Balls/BallSpartacus.hpp"
#include	"Shader/ShaderManager.hpp"
#include  "Project_Dependant/StoredShaders.hpp"


class EdgePath;
class TrackPath;
class TrackEdge;
class Track;

class Ball {
public:  
  Ball( Physics* aPhysics,
        const GameBp::NitroDescription& aNitroDesc, 
        const GameBp::BallDescription&  aBallDesc,
        const D3DXVECTOR3& aInitPosition,
        const int aBall, ID3D10ShaderResourceView* aPermTextureSRV );
  ~Ball();

  void  display() const;
  void  update( const float aLapsedTime, const Track* aTrack );
  void  inGameReset();

  //  move functions to be used by both player and IA
  void  moveFwd( const bool aMove );
  void  moveBck( const bool aMove );
  void  moveLft( const bool aMove );
  void  moveRgt( const bool aMove );

  int   id() const;
  
  //  Render
public:
  RigidBody*      mBallBody;
private:
  ShaderManager   mShaderManager;

  //  Physics vars
private:
  TimeForce*      mBallTForceForw;
  TimeForce*      mBallTForceBack;
  TimeForce*      mBallTForceLeft;
  TimeForce*      mBallTForceRight;
public:
  TimeTorque*     mBallTTorqueUp;
  TimeTorque*     mBallTTorqueDown;
  TimeTorque*     mBallTTorqueLeft;
  TimeTorque*     mBallTTorqueRight;  

public:
  BallModel*      mBallModel;

  //  Static description
private:
  GameBp::BallDescription   mBallDesc;
  GameBp::NitroDescription  mNitroDesc;
  
  //  Dynamic data
  float                     mCurrentMaxVelocity;  //passed to physics
  float                     mTargetMaxVelocity;   //used to time-filter increments
  bool      mNitroUsed;
  float     mLapsedTimeSinceNitro;
  int       mNumNitro;
  std::vector<GameBp::VelModifier>  mVelModifiers;

  //  update camera via ball's position
public:
  D3DXVECTOR3   mLastNormalOfContact;
private:
  D3DXVECTOR3   mLastVelocityNonZero;
  D3DXVECTOR3   mPointingDirection;
  D3DXVECTOR3   mLastCrossProd;

  Log*      mLog;

  D3DXVECTOR3   mLastRotationV;
  D3DXVECTOR3   mLastRotationN;

public:
  bool      allowsNitro();
  void      applyNitro();
  void      addNitro();
  void      maxVelChange( const float aInc, const float aTime  );

  D3DXVECTOR3 pointingUnit()    const;
  D3DXVECTOR3 movementVector()  const;

private:
  void  clearForces();
  void  resetMovementVector();
  void  updateCameraParameters( const float aLapsedTime );
  void  rotate( const D3DXVECTOR3 aV, const D3DXVECTOR3 aN, const float aLapsedTime );

  friend class Ball_IA;

  static const int  mRotationInfinitesimalSteps = 100;


  
public:
  TrackPath*  nearestTrackPath    ( const Track* aTrack );
  TrackEdge*  nearestEdgePath     ( const Track* aTrack );
  float       distanceToEdgePath  ( const TrackEdge* aEdge );
private:
  EdgePath* findNearestEdgePath( const float aDistanceThreshold, const Track* aTrack );
  float     findNearestPointInPath( const EdgePath* aEdge );
};


#endif  //_GameBP_Ball_HPP_
