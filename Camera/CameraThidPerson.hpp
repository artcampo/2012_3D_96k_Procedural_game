#ifndef _DX_CAMERA_3RDPERSON
#define _DX_CAMERA_3RDPERSON

#include <d3dx10.h>
#include <dxerr.h>

#include		"Camera.hpp"
#include  "Geometry/DxMathHelpers.hpp"

class Camera3rdPersonView
{
  public:
    Camera3rdPersonView(){};
    Camera3rdPersonView( Camera* aCam );

    //  to control camera with a path
    void  updateCamera  () const;
    void  updateTargetParameters  ( const D3DXVECTOR3& aCamTargetPos, 
                                    const D3DXVECTOR3& aCamTargetVelocityDirection,
                                    const D3DXVECTOR3& aCamTargetNormalDirection,
                                    const D3DXVECTOR3& aCamTargetVelocity,
                                    const float aLapsedTime  );

  private:
    Camera*     mCam;

    float   mElevationWS;
    float   mDistanceToObjectWS;
    float   mDistanceFocusWS;
    float   mFocusInc;
    
    //D3DXVECTOR3 mCamPosWS;        /*!< Where is the camera*/
    D3DXVECTOR3 mCamTargetsWS;    /*!< Where it points to */
    D3DXVECTOR3 mCamTargetVelocityDirectionWS;    /*!< Where it points to */

    D3DXVECTOR3 mCamTargetVel;
    D3DXVECTOR3 mUpWS;



    
    friend class Game;
};


#endif