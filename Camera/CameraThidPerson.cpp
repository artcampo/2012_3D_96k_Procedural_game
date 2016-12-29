#include		"Camera/CameraThidPerson.hpp"
#include    "Global.hpp"

Camera3rdPersonView::Camera3rdPersonView( Camera* aCam )
{
  mElevationWS        = 2.25f;
  mDistanceToObjectWS = 3.0f;
  mDistanceFocusWS    = 4.0f;
  mCamTargetVel       = D3DXVECTOR3( 1,0,0 );
  mUpWS               = D3DXVECTOR3( 0,1,0 );
  mFocusInc           = 0.01f;
  mCam                = aCam;
}


void Camera3rdPersonView::updateTargetParameters( const D3DXVECTOR3& aCamTargetPos, 
                                                  const D3DXVECTOR3& aCamTargetVelocityDirection,
                                                  const D3DXVECTOR3& aCamTargetNormalDirection,
                                                  const D3DXVECTOR3& aCamTargetVelocity,
                                                  const float aLapsedTime )
{ 
  mCamTargetsWS         = aCamTargetPos;
  mCamTargetVelocityDirectionWS  = aCamTargetVelocityDirection;
  D3DXVec3Normalize( &mCamTargetVelocityDirectionWS, &mCamTargetVelocityDirectionWS );
  if ( DxMath::equalsZero( mCamTargetVelocityDirectionWS ) )
    mCamTargetVelocityDirectionWS = D3DXVECTOR3( 1,0,0 );

  D3DXVECTOR3 update = mCamTargetVelocityDirectionWS - mCamTargetVel;
  //D3DXVec3Normalize( &update, &update );

  const float ymax = 0.2f;
  if ( fabs(update.y) > ymax )
  {
    //update.y = ymax + (update.y - ymax)*0.5f;
    D3DXVec3Normalize( &update, &update );
  }

  

  //if ( D3DXVec3Length( &aCamTargetVelocity ) > 2 )
  mCamTargetVel += update*mFocusInc*aLapsedTime;

  D3DXVECTOR3 targetNormal;
  float a = DxMath::azimut( mCamTargetVel );
  if ( a < DxMath::degToRad(80) || a > DxMath::degToRad(100) )
    targetNormal = DxMath::alignToTangents( D3DXVECTOR3(1,0,0), mCamTargetVel,D3DXVECTOR3(0,1,0) );
  else
    targetNormal = aCamTargetNormalDirection;

  D3DXVECTOR3 updateNormal = targetNormal - mUpWS;
  D3DXVec3Normalize( &updateNormal, &updateNormal );
  mUpWS += updateNormal*aLapsedTime*mFocusInc*0.1f;
  D3DXVec3Normalize( &mUpWS, &mUpWS );
  
  *Global::mFileDebugCamera << "n: " << aCamTargetNormalDirection.x << "," <<aCamTargetNormalDirection.y<<","<<aCamTargetNormalDirection.z<<std::endl;
  *Global::mFileDebugCamera << "UP: " << mUpWS.x << "," <<mUpWS.y<<","<<mUpWS.z<<std::endl;


}

void Camera3rdPersonView::updateCamera () const
{
  //Global::mView = mMatrix;

  D3DXVECTOR3 upWS = mUpWS;

  D3DXVECTOR3 camPos  = mElevationWS*upWS + mCamTargetsWS - mCamTargetVel*mDistanceToObjectWS;
  D3DXVECTOR3 camAt   = mCamTargetsWS + mCamTargetVel*mDistanceFocusWS;

  mCam->setUp (
    &upWS
    );
  mCam->setPositionAndAt(
    &camPos, 
    &camAt );
  
}

