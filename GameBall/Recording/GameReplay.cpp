#include  "GameBall/Recording/GameReplay.hpp"
#include  "GameBall/Recording/GameRecording.hpp"
#include  "GameBall/Game.hpp"
#include  "Camera/CameraThidPersonFly.hpp"
#include "Global.hpp"


GameReplay::GameReplay( Game* aGame )
  : mGame(aGame),
    mTime0(0),
    mTime1(10000),
    mCurrentTime(0)
{
  mCam = new Camera3rdPersonFly( mGame->mCam );
  mLog = new Log();
}

void    GameReplay::play( const float aLapsedTime )
{
  
  mGame->mRecording->play( mTime0 + mCurrentTime );
  mCam->updateTargetParameters( &mGame->mRecording->ballOrientation(), 
                                &mGame->mRecording->ballPosition());
  mCam->updateCamera(aLapsedTime);
  mCurrentTime += aLapsedTime;

  if ( mCurrentTime > (mTime1 - mTime0) )
  {
    //  now show whole session
    mTime0 = 3*100.0f;
    mTime1 = mGame->mLapsedTime - 1*10.0f;
    mCurrentTime = mTime0;
  }
  mCurrentTime = fmod( mCurrentTime, (mTime1 - mTime0) );

  *mLog->mFileEvents 
    << "t: " 
    << mCurrentTime
    << "Ball: " 
    << mGame->mRecording->ballOrientation().x
    << mGame->mRecording->ballOrientation().y
    << mGame->mRecording->ballOrientation().z 
    << " (" << mGame->mRecording->mPlayIndex 
    << "/" << mGame->mRecording->mTimePos.size() << ")" << std::endl;
}
