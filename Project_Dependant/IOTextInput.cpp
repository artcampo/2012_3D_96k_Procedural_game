#include  "Project_Dependant/IOTextInput.hpp"
#include  "App/GameBpMenu.hpp"
#include  "App/MainMenu/GameBpMainMenu.hpp"
#include "Camera/Camera.hpp"

IOTextInput::IOTextInput ()
{
  mLastKey = VK_RETURN; 
}

void  IOTextInput::menu  ( GameBpMainMenu* aMenu  )
{
  mMenu = aMenu;
}

void IOTextInput::Move ( unsigned int key )
{
  if ( mLastKey == key )
  {    
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
      case VK_ESCAPE:
        mMenu->cancelSave( );        
        return;

      case VK_RETURN:
        mMenu->endSave();
        //mWaitCount = 30;
        return;

      default: 
        if (   (key >= 'a' && key <= 'z')
            || (key >= 'A' && key <= 'Z')
            || (key >= '0' && key <= '0') )
        {
          mText.push_back( key );
          mMenu->saveFileName( mText );
        }
        break;
   
    };

    
}


void  IOTextInput::reset()
{
  mText = std::string("");
}