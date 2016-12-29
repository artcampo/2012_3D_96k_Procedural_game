#ifndef _IO_MENU_HPP
#define _IO_MENU_HPP

#include "dx_misc.hpp"
#include "Project_Dependant/IOAbstract.hpp"
class GameBpMenu;

class IOMenu : public IoAbstract { 
public:	
  IOMenu() { /*mWaitCount = 0;*/ mLastKey = K_NON_EXISTANT; };
  void  menu  ( GameBpMenu* aMenu );
	void	Move	( unsigned int key );
  void  MouseMove ( D3DXVECTOR3* inc_mouse ){};
  void  initIoReading( float aDeltaT ){};
  void  finishIoReading( D3DXVECTOR3* aMouseIncrement ){};
private:
  GameBpMenu* mMenu;
  //int   mWaitCount;
  int   mLastKey;
};

#endif