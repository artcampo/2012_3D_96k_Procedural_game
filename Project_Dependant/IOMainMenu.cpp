#include  "Project_Dependant/IOMainMenu.hpp"
#include  "App/GameBpMenu.hpp"
#include  "App/MainMenu/GameBpMainMenu.hpp"
#include "Camera/Camera.hpp"

IOMainMenu::IOMainMenu ( Camera* aCam ):
  inc(1.36f)
{
  mCam      = aCam;      
  mLastKey = K_NON_EXISTANT; 
}

void  IOMainMenu::menu  ( GameBpMainMenu* aMenu  )
{
  mMenu = aMenu;
}

void IOMainMenu::Move ( unsigned int key )
{
  if ( mLastKey == key )
  {
    //mLastKey = K_NON_EXISTANT;
    return;
  }
  /*
  if ( mWaitCount != 0 )
  {
    --mWaitCount;
    return;
  }*/
  
  mLastKey = key;

  switch( key )
    {   
      case KESC:
        mMenu->setAction( MainMenu::eFallBack );
        //mWaitCount = 30;
        return;

      case KSPACE:
      case KINTRO:
        mMenu->setAction( MainMenu::eAccepted );
        //mWaitCount = 30;
        return;

      case KLEFT: 
        mMenu->left(); 
        break;
      case KRIGHT: 
        mMenu->right(); 
        break;

      case KUP:       
        mMenu->decreaseIndex();        
        break;

      case KDOWN: 
        mMenu->increaseIndex();        
        break;

    };

    
}


void  IOMainMenu::MouseMove ( D3DXVECTOR3* inc_mouse )
{

  //move mouse with camera
  D3DXVECTOR3 v = 0.05f*(*inc_mouse);
  mCam->incrementTargetPos( &v ); 
  
}