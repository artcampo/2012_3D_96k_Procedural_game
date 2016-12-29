#include "dx_misc.hpp"
#include "App/GameBpApp.hpp"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{  
/*
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
*/
  bool fullscreen = false;
  bool showFPS = true;
  std::wstring wTitle;
  wTitle =  L"Ball Plane 96k";  
  GameBpApp my(hInstance, SCREEN_W, SCREEN_H, fullscreen, wTitle, showFPS);  
  while(1)
  {
    if ( my.menuMain() == 1 )
    {
      //  all options chosen, start game
      //my.initGameWithUserOptions();
      if ( my.mainLoadScreen() == 1)
        {
          if ( my.Main() == 0 )
           return 0;
        }
      else
        return 0;
    }
    else
    {
      // exit chosen
      return 0;
    }
  }
}