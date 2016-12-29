#ifndef _Trick_OverBumper_HPP_
#define _Trick_OverBumper_HPP_

#include  "GameBall/Entities/Ball.hpp"

class Tricks;

class Trick_OverBumper{
public:
  //~Trick_OverBumper();
  
  Trick_OverBumper( Tricks* aTrickManager );
  
  void  reset();
  void  update( const bool    aHasPassedOver );

public:
  int     mNum;
private:  
  Tricks*       mTrickManager;
 
  const static int   mPoints;


};

#endif  //_Trick_OverBumper_HPP_
