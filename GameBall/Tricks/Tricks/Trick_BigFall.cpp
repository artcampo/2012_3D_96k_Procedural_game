#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_BigFall.hpp"
#include  "GameBall/Tricks/TricksPunctuation.hpp"

using namespace TricksScore;
const float Trick_BigFall::mHeight       = 100.0f;
const float Trick_BigFall::mHeightExtra  = 250.0f;

enum states{
  ready,  
  jump
};

Trick_BigFall::Trick_BigFall( Tricks* aTrickManager  )
: mTrickManager( aTrickManager ),
  mNum(0),
  mState(ready)
{  
}

/*
Trick_BigFall::~Trick_BigFall()
{ }
*/
  
void  Trick_BigFall::update(  const double        aLapsedTime, 
                const bool          aHasStartedJump,
                const bool          aHasLanded,                                  
                const float          aJumpAbsHeight)
{
  switch ( mState )
  {
  case ready:
    if ( aHasStartedJump )
    {           
      mState            = jump;      
    }
    break;

  case jump:
    if ( aHasLanded )
    {
      if ( aJumpAbsHeight > mHeight)
        {
          if ( aJumpAbsHeight > mHeight && aJumpAbsHeight < mHeightExtra )
            mTrickManager->addPunctuation(  Score_BigFall::mPoints +
            Score_BigFall::mPoints/10*mNum,
                                            std::string("Big Fall") );                              
          if ( aJumpAbsHeight > mHeightExtra )
            mTrickManager->addPunctuation(  Score_BigFall::mPoints +
              Score_BigFall::mPoints/10*mNum,
                                            std::string("Gaze into the abyss") );  
          ++mNum;    
        }
      mState = ready;
    }
    break;

  default: break;
  }
}


void  Trick_BigFall::reset()
{
  mState = ready;
  mNum = 0;
}