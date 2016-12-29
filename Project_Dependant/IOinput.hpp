#ifndef _IO_INPUT_HPP
#define _IO_INPUT_HPP


#include "dx_misc.hpp"
#include "Camera/Camera.hpp"
#include "Camera/CameraThidPerson.hpp"
#include "Camera/CameraFirstPerson.hpp"
#include "physics/Physics.hpp" 
#include "GameBall/Game.hpp"
#include "Project_Dependant/IOAbstract.hpp"

class GameBpApp;

class IoInput : public IoAbstract { 
public:	
	IoInput( Camera* aCam, Game *aGame, GameBpApp* aGameApp );
	~IoInput();

	void	Move	( unsigned int key );
  void  MouseMove ( D3DXVECTOR3* inc_mouse ){};
  void  initIoReading( float aDeltaT );
  void  finishIoReading( D3DXVECTOR3* aMouseIncrement );

private:
  float       inc;
  Camera*     mCam;               /*!< Camera*/  
  float       mDeltaT;            /*!< Current time step */
  bool        mTimeForceOn[4];   /*!< Indexed via physTimeForces*/

  Game*       mGame;    //for callbacks
  GameBpApp*  mGameApp;  //for callbacks


};


#endif