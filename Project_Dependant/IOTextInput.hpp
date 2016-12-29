#ifndef _IO_TEXT_UIN_HPP
#define _IO_MAIN_MENU_HPP

#include "dx_misc.hpp"
#include "Project_Dependant/IOAbstract.hpp"
class GameBpMainMenu;
class Camera;
#include <string>

class IOTextInput : public IoAbstract { 
public:	
  IOTextInput();
  void  menu  ( GameBpMainMenu* aMenu );
	void	Move	( unsigned int key );
  void  MouseMove ( D3DXVECTOR3* inc_mouse ){};
  void  initIoReading( float aDeltaT ){};
  void  finishIoReading( D3DXVECTOR3* aMouseIncrement ){};
  void  reset();
private:
  GameBpMainMenu* mMenu;
  //int   mWaitCount;
  int   mLastKey;
  std::string mText;
};

#endif