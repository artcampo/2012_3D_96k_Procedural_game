#ifndef _DX_CAMERA_1STPERSON
#define _DX_CAMERA_1STPERSON

#include <d3dx10.h>
#include <dxerr.h>

#include		"Camera.hpp"

class Camera1stPersonView
{
  public:
    Camera1stPersonView( Camera* aCam );

    //  to control camera with a path
    void  updateCamera  ();
    void  updateTargetParameters  ( const D3DXQUATERNION* aOrientation, D3DXVECTOR3* aCamTarget );

  private:
    Camera*     mCam;
    bool        mInitialized;     /*!< Is mCamPosWS initialized?*/
        
    D3DXVECTOR3 mCamPosWS;        /*!< Where is the camera*/
    D3DXVECTOR3 mCamTarget;       /*!< Where it points to */
    D3DXVECTOR3 mOrientationWS;

    D3DXMATRIX  mMatrix;  /*!< Matrix with target object orientation*/

    float       mDistanceOfView;
    
};


#endif