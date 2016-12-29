#ifndef _Trick_MoodSwing_HPP_
#define _Trick_MoodSwing_HPP_

#include  "GameBall/Entities/Ball.hpp"

class Tricks;
class D3DXVECTOR3;

class Trick_MoodSwing{
public:
  //~Trick_MoodSwing();
  
  Trick_MoodSwing( Tricks* aTrickManager );
  
  void  reset();
  void  update( const double        aLapsedTime, 
                const bool          aHasStartedJump,
                const bool          aHasLanded,    
                const D3DXVECTOR3&  aBallVel );

public:
  int       mNum;
private:  
  Tricks*     mTrickManager;
  int         mState;  
  D3DXVECTOR3 mBallVelPrev;

  const static int   mPoints;
  const static float mAngle;
  


};

#endif  //_Trick_MoodSwing_HPP_
