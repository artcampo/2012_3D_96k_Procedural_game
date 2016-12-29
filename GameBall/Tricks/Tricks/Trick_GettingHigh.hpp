#ifndef _Trick_GettingHigh_HPP_
#define _Trick_GettingHigh_HPP_

#include  "GameBall/Entities/Ball.hpp"

class Tricks;
class D3DXVECTOR3;

class Trick_GettingHigh{
public:
  //~Trick_GettingHigh();
  
  Trick_GettingHigh( Tricks* aTrickManager );
  
  void  reset();
  void  update( const double        aLapsedTime, 
                const bool          aHasStartedJump,
                const bool          aHasLanded,                                  
                const bool          aIsStartingJumpFall,
                const float         aJumpMaxHeight);

public:
  int       mNum;
private:  
  Tricks*     mTrickManager;
  int         mState;

  const static int   mPoints;
  const static int   mPointsExtra;
  const static float mHeight;
  const static float mHeightExtra;
  
  void checkJump( const D3DXVECTOR3&  aBallPos );


};

#endif  //_Trick_GettingHigh_HPP_
