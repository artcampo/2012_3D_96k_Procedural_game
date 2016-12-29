#ifndef _IO_MAIN_MENU_HPP
#define _IO_MAIN_MENU_HPP

#include "dx_misc.hpp"
#include "Project_Dependant/IOAbstract.hpp"
class GameBpMainMenu;
class Camera;

class IOMainMenu : public IoAbstract { 
public:	
  IOMainMenu( Camera* aCam );
  void  menu  ( GameBpMainMenu* aMenu );
	void	Move	( unsigned int key );
  void  MouseMove ( D3DXVECTOR3* inc_mouse );
  void  initIoReading( float aDeltaT ){};
  void  finishIoReading( D3DXVECTOR3* aMouseIncrement ){};
private:
  GameBpMainMenu* mMenu;
  //int   mWaitCount;
  int   mLastKey;
  Camera* mCam;
  float inc;
};

#endif