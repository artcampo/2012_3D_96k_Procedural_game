#ifndef _Trick_BigFall_HPP_
#define _Trick_BigFall_HPP_

#include  "GameBall/Entities/Ball.hpp"

class Tricks;
class D3DXVECTOR3;

class Trick_BigFall{
public:
  //~Trick_BigFall();
  
  Trick_BigFall( Tricks* aTrickManager );
  
  void  reset();
  void  update( const double        aLapsedTime, 
                const bool          aHasStartedJump,
                const bool          aHasLanded,                                  
                const float         aJumpAbsHeight);

public:
  int       mNum;
private:  
  Tricks*     mTrickManager;
  int         mState;  


  const static int   mPoints;
  const static int   mPointsExtra;
  const static float mHeight;
  const static float mHeightExtra;
  



};

#endif  //_Trick_BigFall_HPP_
