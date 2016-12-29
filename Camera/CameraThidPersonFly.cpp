#include		"Camera/CameraThidPersonFly.hpp"
#include    "Global.hpp"
#include		"Camera.hpp"
#include "Geometry/DxMathHelpers.hpp"

Camera3rdPersonFly::Camera3rdPersonFly( Camera* aCam )
{
  //mDistanceToTargetLS = D3DXVECTOR3( -50.0f, 20.0f, 0.0f);
  mDistanceToTargetLS = D3DXVECTOR3( -4.0f, 8.0f, 0.0f);
  mInitialized        = false;
  mIncMultiplier      = 0.45f;
  mCam                = aCam;
  mUsedOrientation    = D3DXVECTOR3( 1, 0.0f, 0.0f);
}


void Camera3rdPersonFly::updateTargetParameters( const D3DXVECTOR3* aOrientation, D3DXVECTOR3* aCamTarget )
{
 
  mCamTarget = *aCamTarget;
  mOrientationWS = DxMath::normalizeVector(*aOrientation);
  if ( fabs( mOrientationWS.y )> 0.6f  )
    mOrientationWS.y *= 0.5f;
  if ( fabs( mOrientationWS.y) > 0.4f  )
    mOrientationWS.y *= 0.5f;
  if ( fabs( mOrientationWS.y )> 0.2f  )
    mOrientationWS.y *= 0.5f;
  mOrientationWS = DxMath::normalizeVector(mOrientationWS);
  
}

void Camera3rdPersonFly::updateCamera ( const float aLapsedTime )
{
  mUsedOrientation += ( mOrientationWS - mUsedOrientation )*aLapsedTime*0.01f;
  mUsedOrientation = DxMath::normalizeVector( mUsedOrientation );
  //Global::mView = mMatrix;
/*
  float angle = atan2( mOrientationWS.z, mOrientationWS.x );
  angle += 3.1412f/2.0f;
  float radius = 100.0f;
  float height = 50.0f;
  D3DXVECTOR3 distance = 
    D3DXVECTOR3 (
      sin(angle)*radius,
      -height,
      cos(angle)*radius
      );
      /*
    D3DXVECTOR3 (
      100.0f,
      -height,
      100.0f
      );*/
  D3DXVECTOR3 upWS = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);


  mCam->setUp (
    &upWS
    );
  mCam->setPositionAndAt(
    &(mCamTarget - mUsedOrientation*6.5f +  5.0f*upWS),
    &(mCamTarget) );
  
}

