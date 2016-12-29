#include  "GameBall/Game.hpp"

void Game::setCamera3rdPerson()
{
  D3DXMatrixIdentity  ( &Global::mWorld );
  mCam->updateCamera   ( &Global::mView );
  D3DXVECTOR3 mEyePos;
  mCam->getEyePos ( &Global::mEyePos );  
}

void Game::updateCamera3rdPerson( const float aLapsedTime )
{  
  mCam3dPerson->updateTargetParameters( mBall->mBallBody->xWS, 
                                        mBall->pointingUnit(),
                                        mBall->mLastNormalOfContact, 
                                        mBall->mBallBody->getvWS(), 
                                        aLapsedTime );  



  mCam3dPerson->updateCamera();
}

