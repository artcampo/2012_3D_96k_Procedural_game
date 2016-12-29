#include "Project_Dependant/IOinputBlocked.hpp"
#include "App/GameBpApp.hpp"


IoInputBlocked::IoInputBlocked ( Camera* aCam, Game* aGame, GameBpApp* aGameApp )
{
  mCam      = aCam;      
  mGame     = aGame;  
  mGameApp  = aGameApp;
}



IoInputBlocked::~IoInputBlocked()
{

}

void IoInputBlocked::Move ( unsigned int key )
{
  TimeForceState state;
  
  switch( key )
    {   
      case KESC:
        mGameApp->inGameMenuShow();
        return;

      ////////////////////////////////////////////////////
      //  debug
      ////////////////////////////////////////////////////
      case KM:
        Global::mExcludeEdge += 1;
        break;

      case KN:
        Global::mExcludeEdge -= 1;
        break;

    };

    
}

void  IoInputBlocked::MouseMove ( D3DXVECTOR3* inc_mouse )
{  
}


//===========================================================================
/*!
    To be called prior to reading IO devices.
    \param      aDeltaT    current time position delta, simulation space
*/
//===========================================================================
void  IoInputBlocked::initIoReading( float aDeltaT )
{
    
}

//===========================================================================
/*!
    Decreases forces whose button is not pressed.
    Applis mouse movement.
    \param    aMouseIncrement   Mouse increment of this frame's deltaT
*/
//===========================================================================

void  IoInputBlocked::finishIoReading( D3DXVECTOR3* aMouseIncrement )
{  
}
