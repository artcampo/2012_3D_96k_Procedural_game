#include  "GameBall/Game.hpp"

#include  "Project_Dependant/IOinput.hpp"
#include  "GameBall/Entities/Arch.hpp"
#include  "GameBall/Tricks/Tricks.hpp"

#define _BP_EXTRA_LOG

void  Game::actionNitro( Ball& aBall )
{
  if ( aBall.allowsNitro() )
  {
    aBall.applyNitro();  
    mNitroUsed = true;
  }
  else
    mNitroUsed = false;
}

void  Game::actionAddNitro( Ball& aBall )
{
  aBall.addNitro();
#ifdef _BP_EXTRA_LOG
  *mLog->mFile << "Add Nitro"<< " [" << mPhysics->mDebugFrame << "]" << std::endl;
#endif 
   addMessage( 3, std::string("Add nitro"));
}

void  Game::actionBrake( Ball& aBall )
{}

void  Game::actionSetCheckPoint( const Ball& aBall, const D3DXVECTOR3& aDisplacement )
{
  mLastCheckPoint.pos       = aBall.mBallBody->xWS + aDisplacement;
  mLastCheckPoint.camera3rd = *mCam3dPerson;
  addMessage( 3, std::string("Checkpoint"));
}

void  Game::actionRestoreCheckPoint( Ball& aBall )
{
  mLapsedTimeSinceInContact = 0.0f;
  *mCam3dPerson         = mLastCheckPoint.camera3rd;
  aBall.mBallBody->xWS  = mLastCheckPoint.pos;
  aBall.mBallBody->standstill();
  aBall.inGameReset();  
  mTrickManager->reset();
}

void  Game::actionMaxVelChange( Ball& aBall, const float aInc, const float aTime  )
{
  aBall.maxVelChange( aInc, aTime );
  if ( aInc > 0.0f )  addMessage( 3, std::string("Vel up"));
  else                addMessage( 3, std::string("Vel down"));
}


void  Game::actionInvertCommands( const float aTime )
{
  mInvertCommandsTimed = TimeLimitedEvent( aTime, true );
  addMessage( 3, std::string("Commands inverted!"));
}

