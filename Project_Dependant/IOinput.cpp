#include "Project_Dependant/IOinput.hpp"
#include "App/GameBpApp.hpp"


IoInput::IoInput ( Camera* aCam, Game* aGame, GameBpApp* aGameApp ):
  inc(1.36f)
{
  mCam      = aCam;      
  mGame     = aGame;  
  mGameApp  = aGameApp;
}


IoInput::~IoInput()
{

}

void IoInput::Move ( unsigned int key )
{
  TimeForceState state;
  
  switch( key )
    {   
      case KESC:
        mGameApp->inGameMenuShow();
        return;
      case KBACK:
        mGame->actionRestoreCheckPoint( *mGame->mBall );
        return;
      ////////////////////////////////////////////////////
      //  Movement
      ////////////////////////////////////////////////////
      case KUP:  
        *Global::mFileDebugKeys << "Key u" << std::endl;
        mGame->mBall->moveFwd(true);
        mTimeForceOn[PHYS_SHIP_ACC_FORWARDS_ID] = true;
        break;

      case KDOWN:    
        *Global::mFileDebugKeys << "Key d" << std::endl;          
        mGame->mBall->moveBck(true);
        mTimeForceOn[PHYS_SHIP_ACC_BACKWARDS_ID] = true;
        break;

      case KLEFT:       
        *Global::mFileDebugKeys << "Key l" << std::endl;     
        if ( !mGame->areControlsInverted() )
        {
          mGame->mBall->moveLft(true);
          mTimeForceOn[PHYS_SHIP_ACC_LEFT_ID] = true;
        }
        else
        {
          mGame->mBall->moveRgt(true);
          mTimeForceOn[PHYS_SHIP_ACC_RIGHT_ID] = true;
        }
        break;

      case KRIGHT:        
        *Global::mFileDebugKeys << "Key r" << std::endl;
        if ( !mGame->areControlsInverted() )
        {
          mGame->mBall->moveRgt(true);
          mTimeForceOn[PHYS_SHIP_ACC_RIGHT_ID] = true;
        }
        else
        {
          mGame->mBall->moveLft(true);
          mTimeForceOn[PHYS_SHIP_ACC_LEFT_ID] = true;
        }
        break;

      case KSHIFT:        
        *Global::mFileDebugKeys << "Key sh" << std::endl;
        mGame->actionNitro( *mGame->mBall );
        break;

      
      case KSPACE:        
        *Global::mFileDebugKeys << "Key sp" << std::endl;
        mGame->actionBrake( *mGame->mBall );
        break;

      ////////////////////////////////////////////////////
      //  Camera movement
      ////////////////////////////////////////////////////
        /*
      case KT:
        GlobalDebugSSDO::debug_depthBias += 0.001f;
        break;

      case KG:
        GlobalDebugSSDO::debug_depthBias -= 0.001f;
        break;

      case KY:
        GlobalDebugSSDO::debug_sample_radius += 0.01f;
        break;

      case KH:
        GlobalDebugSSDO::debug_sample_radius -= 0.01f;
        break;

      case KU:
        GlobalDebugSSDO::debug_singularity += 0.01f;
        break;

      case KJ:
        GlobalDebugSSDO::debug_singularity -= 0.01f;
        break;

      case KI:
        GlobalDebugSSDO::debug_strength += 0.001f;
        break;

      case KK:
        GlobalDebugSSDO::debug_strength -= 0.001f;
        break;
        */
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
/*
void  IoInput::MouseMove ( D3DXVECTOR3* inc_mouse )
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
void  IoInput::initIoReading( float aDeltaT )
{
  mDeltaT     = aDeltaT;

  mTimeForceOn[PHYS_SHIP_ACC_FORWARDS_ID]   = false;
  mTimeForceOn[PHYS_SHIP_ACC_BACKWARDS_ID]  = false;
  mTimeForceOn[PHYS_SHIP_ACC_LEFT_ID]       = false;
  mTimeForceOn[PHYS_SHIP_ACC_RIGHT_ID]      = false;
    
}

//===========================================================================
/*!
    Decreases forces whose button is not pressed.
    Applis mouse movement.
    \param    aMouseIncrement   Mouse increment of this frame's deltaT
*/
//===========================================================================

void  IoInput::finishIoReading( D3DXVECTOR3* aMouseIncrement )
{
  for ( int i = 0; i < 4; ++i)
  {
    if ( !mTimeForceOn[i] )
    {
      switch( i ){        
        case PHYS_SHIP_ACC_FORWARDS_ID:
          mGame->mBall->moveFwd(false);
          break;
        case PHYS_SHIP_ACC_BACKWARDS_ID:
          mGame->mBall->moveBck(false);
          break;
        case PHYS_SHIP_ACC_RIGHT_ID:
          mGame->mBall->moveRgt(false);
          break;
        case PHYS_SHIP_ACC_LEFT_ID:
          mGame->mBall->moveLft(false);
          break;
        
      };
      
    }
  }

  //////////////////////////////////////////////////////////////////
  //    Not turning into any direction
  if ( aMouseIncrement->x == 0)
  {
      TimeTorqueState state;
      state.t         = 1.0f;
      state.advancing = false;        
      mGame->mBall->mBallTTorqueLeft->updateState( state );
      mGame->mBall->mBallTTorqueRight->updateState( state );
  }

  if ( aMouseIncrement->y == 0)
  {
      TimeTorqueState state;
      state.t         = 1.0f;
      state.advancing = false;        
      mGame->mBall->mBallTTorqueUp->updateState( state );
      mGame->mBall->mBallTTorqueDown->updateState( state );
  }

  //////////////////////////////////////////////////////////////////
  //    Turning into any direction
  if ( aMouseIncrement->x != 0)
  {
      TimeTorqueState state;
      state.t         = 1.0f;
      state.advancing = true;
      if (aMouseIncrement->x > 0)
        mGame->mBall->mBallTTorqueRight->updateState( state );
      else
        mGame->mBall->mBallTTorqueLeft->updateState( state );
  }

  if ( aMouseIncrement->y != 0)
  {
      TimeTorqueState state;
      state.t         = 1.0f;
      state.advancing = true;    
      if (aMouseIncrement->y > 0)
        mGame->mBall->mBallTTorqueUp->updateState( state );
      else
        mGame->mBall->mBallTTorqueDown->updateState( state );
  }
}
