#ifndef _Trick_AirTurbo_HPP_
#define _Trick_AirTurbo_HPP_

#include  "GameBall/Entities/Ball.hpp"

class Tricks;

class Trick_AirTurbo{
public:
  //~Trick_AirTurbo();
  
  Trick_AirTurbo( Tricks* aTrickManager );
  
  void  reset();
  void  update( const double        aLapsedTime, 
                const bool          aHasStartedJump,    
                const bool          aHasLandedJump,    
                const bool          aUsedNitro,
                const float         aJumpMaxHeight,
                const D3DXVECTOR3&  aBallVel);

public:
  int     mNum;
private:  
  Tricks*       mTrickManager;
  int           mState;
  D3DXVECTOR3   mBallVelPrev;
  

  const static int   mPoints;
  const static float mVelInc;
  const static float mMinHeightJump;

};

#endif  //_Trick_AirTurbo_HPP_
