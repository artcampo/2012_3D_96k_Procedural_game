#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_MoodSwing.hpp"
#include "Geometry/DxMathHelpers.hpp"
#include  "GameBall/Tricks/TricksPunctuation.hpp"

using namespace TricksScore;

const float Trick_MoodSwing::mAngle        = 110.0f;

enum states{
  inLand,  
  inAir
};

Trick_MoodSwing::Trick_MoodSwing( Tricks* aTrickManager  )
: mTrickManager( aTrickManager ),
  mNum(0),
  mState(inLand)
{  
}

/*
Trick_MoodSwing::~Trick_MoodSwing()
{ }
*/
  
void  Trick_MoodSwing::update(  const double        aLapsedTime, 
                                const bool          aHasStartedJump,
                                const bool          aHasLanded,    
                                const D3DXVECTOR3&  aBallVel)
{
  switch ( mState )
  {
  case inLand:
    if ( aHasStartedJump )
    {      
      mBallVelPrev      = aBallVel;
      mState            = inAir;      
    }
    break;

  case inAir:
    if ( aHasLanded )
    {
      float angle = DxMath::polarAngleDistance( aBallVel, mBallVelPrev );
      //*mTrickManager->mLog->mFileEvents << "Swing: " << DxMath::radToDeg(angle) << std::endl;
      if ( angle > DxMath::degToRad( mAngle) )
      {
        mTrickManager->addPunctuation(  Score_MoodSwing::mPoints +
          Score_MoodSwing::mPoints/10*mNum,
                                        std::string("Mood swing") );     
        ++mNum;
      }
      mState = inLand;
    }
    break;

  default: break;
  }
}


void  Trick_MoodSwing::reset()
{
  mState = inLand;
  mNum = 0;
}