#include  "Project_Dependant/IOMenu.hpp"
#include  "App/GameBpMenu.hpp"

void  IOMenu::menu  ( GameBpMenu* aMenu )
{
  mMenu = aMenu;  
  
}

void IOMenu::Move ( unsigned int key )
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
        mMenu->setAction( Menu::eFallBack );
        //mWaitCount = 30;
        return;

      case KSPACE:
      case KINTRO:
        mMenu->setAction( Menu::eAccepted );
        //mWaitCount = 30;
        return;

      case KUP: 
      case KLEFT: 
        mMenu->decreaseIndex();
        //mWaitCount = 20;
        break;

      case KDOWN:
      case KRIGHT: 
        mMenu->increaseIndex();
        //mWaitCount = 20;
        break;

    };

    
}
