#ifndef _GameBP_Chase_HPP_
#define _GameBP_Chase_HPP_

#include  "GameBall/Game.hpp"
#include  "GameBall/IA/Ball_IA.hpp"
#include  "RenderModels/Basics/linelistmodel.h"
#include  "GameBall\Modes\Chase\GameChaseContext.hpp"

class GameChase : public Game {
public:
  GameChase( const int aDifficulty );  
  ~GameChase();
  virtual IoAbstract* handlerIOinput();
  virtual void _setCamera();
  virtual void _updateCamera( const float aLapsedTime );
  virtual void _update( const float aLapsedTime );
  virtual void _initTrackDesc();
  virtual void _initGameState(); 
  virtual void _initEntityDesc(); 
  virtual void _initBalls();
  virtual void _initStates();

  virtual void _drawScene();
  virtual void _drawBalls(){ drawBalls(); };
  virtual void _IAstep( const float aLapsedTime );
  virtual void _checkEndGame( const float aLapsedTime );  
  virtual std::string  _getMainMessage() const;

  virtual void _triggerEnd(Ball &aBall);
  virtual void _triggerAddTime ( const float aTime ){};
  
private:
  Ball*     mChaseBall;  
  Ball_IA*  mChaseBall_IA;
  LineListModel<VertexPos>*    mTargetLineModel;
  LineListModel<VertexPos>*    mIaExtraLineModel;

  GameChaseContext*             mGameContext;  
};


#endif  //_GameBP_Chase_HPP_
