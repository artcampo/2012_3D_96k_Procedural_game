#include		"Camera/CameraFirstPerson.hpp"
#include    "Global.hpp"

Camera1stPersonView::Camera1stPersonView( Camera* aCam )
{  
  mInitialized        = false;  
  mDistanceOfView     = 50.0f;
  mCam                = aCam;
}


void Camera1stPersonView::updateTargetParameters( const D3DXQUATERNION* aOrientation, D3DXVECTOR3* aCamTarget )
{
 
  mCamPosWS = *aCamTarget;

  D3DXMatrixRotationQuaternion( &mMatrix, aOrientation );    
  D3DXVec3TransformCoord ( &mOrientationWS, &(D3DXVECTOR3(1.0f,0.0f,0.0f)), &mMatrix );
}

void Camera1stPersonView::updateCamera ()
{
  //Global::mView = mMatrix;

  
  mCamTarget = mDistanceOfView*mOrientationWS;
  D3DXVECTOR3 upWS = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
  D3DXVec3TransformCoord ( &upWS, &(D3DXVECTOR3(0.0f,1.0f,0.0f)), &mMatrix );

  mCam->setUp (
    &upWS
    );
  mCam->setPositionAndAt(
    &(mCamPosWS),
    &(mCamPosWS+mCamTarget) );
  
}

