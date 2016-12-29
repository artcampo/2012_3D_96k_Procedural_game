#ifndef _IO_INPUT_BLOCKED_HPP
#define _IO_INPUT_BLOCKED_HPP


#include "dx_misc.hpp"
#include "Camera/Camera.hpp"
#include "Camera/CameraThidPerson.hpp"
#include "Camera/CameraFirstPerson.hpp"
#include "physics/Physics.hpp" 
#include "GameBall/Game.hpp"
#include "Project_Dependant/IOAbstract.hpp"

class GameBpApp;

class IoInputBlocked : public IoAbstract { 
public:	
	IoInputBlocked( Camera* aCam, Game *aGame, GameBpApp* aGameApp );
	~IoInputBlocked();

	void	Move	( unsigned int key );
  void  MouseMove ( D3DXVECTOR3* inc_mouse );
  void  initIoReading( float aDeltaT );
  void  finishIoReading( D3DXVECTOR3* aMouseIncrement );

private:  
  Camera*     mCam;               /*!< Camera*/  
  

  Game*       mGame;    //for callbacks
  GameBpApp* mGameApp;  //for callbacks


};


#endif