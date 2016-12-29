#ifndef _DX_CAMERA_3RDPERSON_FLY_
#define _DX_CAMERA_3RDPERSON_FLY_

#include <d3dx10.h>
#include <dxerr.h>

class Camera;

class Camera3rdPersonFly
{
  public:
    Camera3rdPersonFly( Camera* aCam );

    //  to control camera with a path
    void  updateCamera  ( const float aLapsedTime );
    void  updateTargetParameters  ( const D3DXVECTOR3* aOrientation, D3DXVECTOR3* aCamTarget );

  private:
    Camera*     mCam;             // copy
    bool        mInitialized;     /*!< Is mCamPosWS initialized?*/
    float       mIncMultiplier;   /*!< At which rate does the camera update its position*/

    D3DXVECTOR3 mDesiredCamPosWS; /*!< Where should be the camera*/
    D3DXVECTOR3 mCamPosWS;        /*!< Where is the camera*/
    D3DXVECTOR3 mCamTarget;       /*!< Where it points to */
    D3DXVECTOR3 mOrientationWS;

    D3DXVECTOR3 mDistanceToTargetLS;    /*!< How far behind from the target should we be?*/
    D3DXVECTOR3 mDistanceToTargetWS;    /*!< How far behind from the target should we be?*/

    D3DXVECTOR3 mUsedOrientation;
    D3DXMATRIX  mMatrix;  /*!< Matrix with target object orientation*/    
};


#endif  //_DX_CAMERA_3RDPERSON_FLY_