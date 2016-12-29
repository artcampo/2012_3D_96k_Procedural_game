#ifndef _GameBP_Ball_IA_HPP_
#define _GameBP_Ball_IA_HPP_

//#pragma ONCE
#include "dx_misc.hpp"
#include <vector>

class Track;
class EdgePath;
class Ball;
class Physics;

#include "Misc/Log.hpp"
#include "Noise/RandomNumberGenerator.hpp"
#include "GameBall/IA/Ball_IA _common.hpp"

class Ball_IA {
public:  
  Ball_IA( Ball& aBall, Physics* aPhysics, Track& aTrack );
  ~Ball_IA();

  
  void  update( const float aLapsedTime );

  //render
  std::vector<VertexPos> getRenderTargetPoints();
  std::vector<VertexPos> getRenderTargetExtraPoints();
  
private:  
  EdgePath*   findNearestEdgePath( const float aDistanceThreshold );
  float       findNearestPointInPath( const EdgePath* aEdge );
  BallIA::Target  findNearestTarget( const EdgePath* aEdge, 
                                      const float aPathT,
                                      const float aUnitsAhead);
  void        moveToTarget( const D3DXVECTOR3& aTargetPoint );

private:
  Ball&       mBall;
  Track&      mTrack;
  Log*        mLog;
  Rand::RandGen  mRand;

  bool        mCheckForNearestEdge;
  EdgePath*   mCurrentEdgePath;

  
  int         mAccelerationMode;  // 0 min, 3 max

  float       mUnitsAhead;  

  int         mNumLookAheadPoints;
  float       mPathNearestTBias;  // look a little ahead not to go forward  
  BallIA::Target  mTarget;
  D3DXVECTOR3     mTargetPoint;
  D3DXVECTOR3     mTargetTangent;
  std::vector<D3DXVECTOR3>  mIaTarget;
  float       mPathTargetT;   // position of current target
  float       mPathTargetTEpsilon;
  float       mPathPreviousT;
  float       mPathPointSurpassedEpsilon;

  D3DXVECTOR3 mDebugPathPoint;
  D3DXVECTOR3 mDebugPathObstacle;

private:
  //  Ia look-ahead
  void createTestPoints(  const int aIndex,
                          const std::vector<BallIA::Target>& aTargets, 
                          std::vector< std::vector<float> >& aCombinations,
                          std::vector<float>& aPositions,
                          const float aTrackWidth);

  float computePathLength( const std::vector<BallIA::Target>& aTargets, 
                          const std::vector<float>& aPositions );

  float computePathChangeAngle( const std::vector<BallIA::Target>& aTargets, 
                          const std::vector<float>& aPositions );


  friend class GameChase;
  friend class GameSpartacus;
};


#endif  //_GameBP_Ball_IA_HPP_
