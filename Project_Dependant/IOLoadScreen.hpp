#ifndef _IO_LOAD_SCREEN_HPP
#define _IO_LOAD_SCREEN_HPP

#include "dx_misc.hpp"
#include "Project_Dependant/IOAbstract.hpp"
class GameBpMenu;
class Camera;

class IOLoadScreen : public IoAbstract { 
public:	
  IOLoadScreen(Camera* aCam);  
	void	Move	( unsigned int key );
  void  MouseMove ( D3DXVECTOR3* inc_mouse );
  void  initIoReading( float aDeltaT );
  void  finishIoReading( D3DXVECTOR3* aMouseIncrement);
private:
  Camera* mCam;
  float inc;

};

#endif  //_IO_LOAD_SCREEN_HPP