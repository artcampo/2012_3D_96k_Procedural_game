#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Tricks/Tricks/Trick_Ponder.hpp"
#include  "Geometry/DxMathHelpers.hpp"
#include  "GameBall/Tricks/TricksPunctuation.hpp"

using namespace TricksScore;

const float Trick_Ponder::mVelChange    = 0.8f;
const float Trick_Ponder::mVelChange2   = 0.4f;

enum states{
  inLand,  
  inAir
};

Trick_Ponder::Trick_Ponder( Tricks* aTrickManager  )
: mTrickManager( aTrickManager ),
  mNum(0),
  mState(inLand)
{  
}

/*
Trick_Ponder::~Trick_Ponder()
{ }
*/
  
void  Trick_Ponder::update( const double        aLapsedTime, 
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
    if ( !aHasLanded )
    {
      float vel1 = D3DXVec3Length( &mBallVelPrev );
      float vel2 = D3DXVec3Length( &aBallVel );
      if ( vel1 > 60.0f )              
      {
        if (  vel2 < (vel1*mVelChange) && vel2 > (vel1*mVelChange2) )
          mTrickManager->addPunctuation(  Score_Ponder::mPoints +
          Score_Ponder::mPoints/10*mNum,
                                          std::string("Ponder") );    
        if ( vel2 < (vel2*mVelChange) )
          mTrickManager->addPunctuation(  Score_Ponder::mPointsExtra +
          Score_Ponder::mPointsExtra/10*mNum,
                                          std::string("Deep breath") ); 
        mBallVelPrev = aBallVel;
        ++mNum;
      }
    }
    if ( aHasLanded )
    { 
      mState = inLand;
    }
    break;

  default: break;
  }
}


void  Trick_Ponder::reset()
{
  mState = inLand;
  mNum = 0;
}