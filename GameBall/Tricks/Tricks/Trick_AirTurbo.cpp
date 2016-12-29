#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_AirTurbo.hpp"
#include  "GameBall/Tricks/TricksPunctuation.hpp"

using namespace TricksScore;

const float Trick_AirTurbo::mVelInc         = 1.5f;
const float Trick_AirTurbo::mMinHeightJump  = 6;

enum states{
  ready,  
  jump
};

Trick_AirTurbo::Trick_AirTurbo( Tricks* aTrickManager  )
: mTrickManager( aTrickManager ),
  mNum(0),
  mState(ready)
{  
}

/*
Trick_AirTurbo::~Trick_AirTurbo()
{ }
*/
  
void  Trick_AirTurbo::update( const double  aLapsedTime, 
                const bool          aHasStartedJump,    
                const bool          aHasLandedJump,
                const bool          aUsedNitro,
                const float         aJumpMaxHeight,
                const D3DXVECTOR3&  aBallVel)
{
  switch ( mState )
  {

  case ready:
    if( aHasStartedJump )
    {
      mBallVelPrev  = aBallVel;
      mState        = jump;
    }
    break;

  case jump:
    if ( aHasLandedJump ) 
      mState = ready;
    if ( !aHasLandedJump && aJumpMaxHeight > mMinHeightJump )
    {
      float vel1 = D3DXVec3Length( &mBallVelPrev );
      float vel2 = D3DXVec3Length( &aBallVel );
      if ( vel2 > vel1*mVelInc )
        {
          mTrickManager->addPunctuation(  Score_AirTurbo::mPoints +
                                          Score_AirTurbo::mPoints/10*mNum,
                                          std::string("Air Turbo") );                                    
          mBallVelPrev  = aBallVel;
          mState = ready;
          ++mNum;
        }
    }
    break;
  }
  
}

void  Trick_AirTurbo::reset()
{
  mNum = 0;
  mState = ready;
}