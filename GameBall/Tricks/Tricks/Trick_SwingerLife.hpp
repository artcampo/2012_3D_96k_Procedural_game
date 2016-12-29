#ifndef _Trick_SwingerLife_HPP_
#define _Trick_SwingerLife_HPP_

#include  "GameBall/Entities/Ball.hpp"

class Tricks;

class Trick_SwingerLife{
public:
  //~Trick_SwingerLife();
  
  Trick_SwingerLife( Tricks* aTrickManager );
  
  void  reset();
  void  update( const std::vector<int>& aColBumperLeft, 
                const std::vector<int>& aColBumperRight);

public:
  int     mNum;
private:  
  Tricks*       mTrickManager;
  int           mState;
  int           mPrevId;
  bool          mPassedAtLeft;
  

  const static int   mPoints;  

  void apply();
};

#endif  //_Trick_SwingerLife_HPP_
