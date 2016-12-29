#ifndef _Trick_Ponder_HPP_
#define _Trick_Ponder_HPP_

#include  "GameBall/Entities/Ball.hpp"

class Tricks;
class D3DXVECTOR3;

class Trick_Ponder{
public:
  //~Trick_Ponder();
  
  Trick_Ponder( Tricks* aTrickManager );
  
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

  const static float mVelChange;
  const static float mVelChange2;
  


};

#endif  //_Trick_Ponder_HPP_
