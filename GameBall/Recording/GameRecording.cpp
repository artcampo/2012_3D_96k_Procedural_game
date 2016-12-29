#include  "GameBall/Recording/GameRecording.hpp"
#include  "GameBall/Game.hpp"

GameRecording::GameRecording( Game* aGame )
  : mGame(aGame),
    mPlayIndex(0)
{
}


void    GameRecording::store( const float aLapsedTime )
{
  mTimePos.push_back( aLapsedTime );
  mBallPos.push_back( mGame->mBall->mBallBody->getxWS() );
  mBallOrientation.push_back( mGame->mBall->mBallBody->getvWS() );
}


void    GameRecording::play( const float aLapsedTime )
{
  bool  timePosFound = false;
  while (! timePosFound)
  {
    if ( mPlayIndex >= mTimePos.size() -1 )
      continue;

    if (    DxMath::greaterThan ( aLapsedTime, mTimePos[mPlayIndex]     , 0.001f )
        &&  DxMath::lesserThan  ( aLapsedTime, mTimePos[mPlayIndex + 1] , 0.001f ) )
      timePosFound = true;
    else
    {
      //modify index
      if ( aLapsedTime < mTimePos[mPlayIndex]  )
      {
        if ( mPlayIndex == 0 )
          timePosFound = true;
        else
          mPlayIndex = (mPlayIndex - 1)% mTimePos.size();
      }
      else
      {
        if ( mPlayIndex >= mTimePos.size() - 1 )
          timePosFound = true;
        else
          mPlayIndex = (mPlayIndex + 1)% mTimePos.size();
      }
      
    }
  }

  //  Now set ball's position
  mGame->mBall->mBallBody->xWS = mBallPos[mPlayIndex];

}

D3DXVECTOR3  GameRecording::ballOrientation() const
{
  return mBallOrientation[mPlayIndex];
}

D3DXVECTOR3     GameRecording::ballPosition() const
{
  return mBallPos[mPlayIndex];
}