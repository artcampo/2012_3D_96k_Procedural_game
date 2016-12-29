#ifndef _GameBP_Spartacus_HPP_
#define _GameBP_Spartacus_HPP_

#include  "GameBall/Game.hpp"
#include  "GameBall/IA/Ball_IA.hpp"
#include  "RenderModels/Basics/linelistmodel.h"

class GameSpartacusContext;

class GameSpartacus : public Game {
public:
  GameSpartacus( const int aDifficulty );
  ~GameSpartacus();
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
  virtual void _drawBalls();
  virtual void _IAstep( const float aLapsedTime );
  virtual void _checkEndGame( const float aLapsedTime );  
  virtual std::string  _getMainMessage() const;

  virtual void _triggerEnd(Ball &aBall);
  virtual void _triggerAddTime ( const float aTime ){};
  
private:
  int                     mNumBalls;
  int                     mNumBallsStillOnRace;
  std::vector<Ball*>      mChaseBall;  
  std::vector<Ball_IA*>   mChaseBall_IA;
  std::vector< LineListModel<VertexPos>* >    mTargetLineModel;

  std::vector<float>      mSpartacusBallLapsedTimeSinceInContact;
  std::vector<bool>       mSpartacusBallInRace;

  GameSpartacusContext*   mGameContext;

private:
  void      actionSpartacusBallOutOfRace( const int aIndex );

public:
  int       numBallsInRace() const;
};


#endif  //_GameBP_Spartacus_HPP_
