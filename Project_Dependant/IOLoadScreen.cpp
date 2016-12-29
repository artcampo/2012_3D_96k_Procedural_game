#include  "Project_Dependant/IOLoadScreen.hpp"
#include  "App/GameBpMenu.hpp"
#include "Camera/Camera.hpp"

IOLoadScreen::IOLoadScreen ( Camera* aCam ):
  inc(1.36f)
{
  mCam      = aCam;      

}



void IOLoadScreen::Move ( unsigned int key )
{
  
  switch( key )
    {   


      ////////////////////////////////////////////////////
      //  Camera movement
      ////////////////////////////////////////////////////
        
  case KI:
        mCam->incrementEyePos( &D3DXVECTOR3( 0.0f, 0.0f, inc*10 ) );        
        break;

      case KO:
        mCam->incrementEyePos( &D3DXVECTOR3( 0.0f, 0.0f,-inc*10 ) );        
        break;

      case KU:
        mCam->incrementEyePos( &D3DXVECTOR3( 0.0f, 0.0f, inc ) );        
        break;

      case KJ:
        mCam->incrementEyePos( &D3DXVECTOR3( 0.0f, 0.0f,-inc ) );        
        break;

      case KH:
        mCam->incrementEyePos( &D3DXVECTOR3( inc, 0.0f, 0.0f ) ); 
        break;

      case KK:
        mCam->incrementEyePos( &D3DXVECTOR3( -inc, 0.0f, 0.0f ) );
        break;

      case KW:
        mCam->incrementEyePos( &D3DXVECTOR3( 0.0f,inc, 0.0f ) );
        break;

      case KS:
        mCam->incrementEyePos( &D3DXVECTOR3( 0.0f,-inc, 0.0f ) );
        break;


    };

    
}

void  IOLoadScreen::MouseMove ( D3DXVECTOR3* inc_mouse )
{

  //move mouse with camera
  D3DXVECTOR3 v = 0.05f*(*inc_mouse);
  mCam->incrementTargetPos( &v ); 
  
}


//===========================================================================
/*!
    To be called prior to reading IO devices.
    \param      aDeltaT    current time position delta, simulation space
*/
//===========================================================================
void  IOLoadScreen::initIoReading( float aDeltaT )
{
 
    
}

//===========================================================================
/*!
    Decreases forces whose button is not pressed.
    Applis mouse movement.
    \param    aMouseIncrement   Mouse increment of this frame's deltaT
*/
//===========================================================================

void  IOLoadScreen::finishIoReading( D3DXVECTOR3* aMouseIncrement )
{

}
