#ifndef _GameBP_HPP_
#define _GameBP_HPP_

#include "dx_misc.hpp"
#include <vector>


#include  "Project_Dependant/ConditionalCompilation.hpp"

#include  "Misc/Log.hpp"

#include  "Camera/Camera.hpp"
#include  "Camera/CameraThidPerson.hpp"
#include  "Camera/CameraFirstPerson.hpp"

#include  "GameBall/GameBp.hpp"
#include  "GameBall/Track.hpp"
#include  "GameBall/TrackBp.hpp"
#include  "GameBall/Track/TrackPath.hpp"
#include  "GameBall/Entities/Bumper.hpp"
#include  "GameBall/Entities/Ball.hpp"

#include  "GameBall/Logic/TrackTimer.hpp"
#include  "GameBall/Logic/GameLogic.hpp"
#include  "GameBall/Logic/TimeLimitedEvent.hpp"

#include  "RenderModels/SingleBox.hpp"
#include  "RenderModels/Compounded/DeferredMeshModel.h"
#include  "RenderModels/GameBp/TrackLinesAndTNBs.hpp"
#include  "RenderModels/GameBp/EntitiesOnLinesAndTNBs.hpp"
#include  "RenderModels/GameBp/FogGeometry.hpp"

#include  "Physics/MeshCol.hpp"
#include  "Physics/Physics.hpp"
#include  "Physics/RigidBody.hpp"

#include  "Project_Dependant/StoredShaders.hpp"
#include  "Project_Dependant/StoredSplines.hpp"
#include  "Shader/Shader.hpp"
#include	"Shader/ShaderManager.hpp"

#include  "Misc/CountMeanMax.hpp"
#include  "GameBall/Logic/TimedMessage.hpp"

class IoAbstract;
class IoInput;
class Arch;
class DxFustrum;
class MetropolisStructure;
class GameBpApp;
class TimedMessage;
class Tricks;
class GameRecording;
class GameReplay;


namespace GameOptions{
  enum eGameModes{
    eModeChrono,
    eModeLab,
    eModeChase
  };

  enum eTrackFamilies{
    ePlane,
    eFall,
    eAbility    
  };
};

class Game {
public:  
  static Game*  gameChrono    ( const int aDifficulty, const int aFamily, const int aBall, GameBpApp* aGameApp );
  static Game*  gameLab       ( const int aDifficulty, const int aFamily, const int aBall, GameBpApp* aGameApp ); 
  static Game*  gameChase     ( const int aDifficulty, const int aFamily, const int aBall, GameBpApp* aGameApp ); 
  static Game*  gameSpartacus ( const int aDifficulty, const int aFamily, const int aBall, GameBpApp* aGameApp ); 
  
  ~Game();

  void      loadDifficultyFile( const char* aName );
  void      loadTrackFile( const char* aName );
  void      loadEntityFile( const char* aName );
  int       initGameObjects();
  void      drawScene( DxFustrum& aFustrum, const bool aSetCamera = true );
  void      update( const double aLapsedTime );  

  std::string   getMainMessage() const;
  std::string   getComboMessage() const;

  TrackBp::TrackDescription getTrackDesc() const;
public:
  void      triggerEnd( Ball& aBall );

  //  Modes virtual functions
protected:  
  virtual IoAbstract*  handlerIOinput() = 0;
  virtual void _setCamera() = 0;  
  virtual void _initTrackDesc() = 0;
  virtual void _initGameState() = 0; 
  virtual void _initEntityDesc() = 0;
  virtual void _initBalls() = 0;
  virtual void _initStates() = 0;

  virtual void _drawScene() = 0;
  virtual void _drawBalls() = 0;
  virtual void _update( const float aLapsedTime ) = 0;
  virtual void _updateCamera( const float aLapsedTime ) = 0;
  virtual void _IAstep( const float aLapsedTime ) = 0;
  virtual void _checkEndGame( const float aLapsedTime ) = 0;  
  virtual std::string  _getMainMessage() const = 0;

  virtual void _triggerEnd(Ball &aBall) = 0;  
  virtual void _triggerAddTime ( const float aTime ) = 0;
  

  //  Shared functions between different modes
  void setCamera3rdPerson();
  void updateCamera3rdPerson( const float aLapsedTime );

  Game();
  //  Game Logic  
  Camera*               mCam;
  Physics*              mPhysics;
  IoInput*              mIOinput;
  Camera3rdPersonView*  mCam3dPerson;
  //Camera1stPersonView*  mCam1stPerson;  
  bool                  mGameFinished;
  bool                  mGameWon;
  float                 mLapsedTime;


  //  Game options
  TrackBp::TrackDescription         mTrackDesc;
public:
  TrackBp::TrackEntityDescription   mTrackEntDesc;
protected:
  int                   mMode;
  int                   mDifficulty;

  //  Debug / Log
  MeanMax<int>          mFpsCounting;
  Log*                  mLog;  

  //  Profile
  ProfilerLoopDp*   mProfLoop;

public:
  //  Game objects
  Ball*                   mBall;    //  Player1
  std::vector<Ball*>      mBalls;   // non-propiety
  std::vector<Arch*>      mArchs;
  std::vector<Bumper*>    mBumpers;  

  EndTrigger*             mEnd;

#ifndef GBP_GFXDEBUG_TRACK
  Track*                  mTrack;
#endif
#ifdef GBP_GFXDEBUG_TRACK
  std::vector<Track*>     mTrack;
#endif

  MetropolisStructure*    mMetropolis;

  //  Intro stuff
  //TrackPath*              mIntroPath;
  //std::vector<DxMeshModel<VertPNT>*>  mIntroPathModel;

  ShaderManager           mShaderManager;

  //  collision
  std::vector<MeshCol*>   mTrackModCol;
  std::vector<MeshCol*>   mBumpersCol;
  std::vector<MeshCol*>   mArchsCol;

private:
  //  Models
  SingleBox                       mBox;
#ifndef GBP_GFXDEBUG_TRACK
  std::vector<DxLodModel<VertPNTUV>*>      mTrackModels;
#endif
#ifdef GBP_GFXDEBUG_TRACK    
  std::vector< std::vector<DxLodModel<VertPNTUV>*> >      mTrackModels;
#endif
  
  Mesh*                           mArchMesh;
  Mesh*                           mBumperCubeMesh;//,  mBumperHexaMesh;
  std::vector<DeferredMeshModel*> mArchModel;
  std::vector<DeferredMeshModel*> mBumperCubeModel;//, mBumperHexaModel;  
  
  FogGeometry*                    mFog;

#ifndef GBP_GFXDEBUG_TRACK
  TrackLinesAndTNBs*        mTrackLinesModel;
#endif
#ifdef GBP_GFXDEBUG_TRACK
  std::vector<TrackLinesAndTNBs*>        mTrackLinesModel;
#endif
  
  EntitiesOnLinesAndTNBs*   mEntitiesLinesModel;
  
  //  Game State
  TimeLimitedEvent      mInvertCommandsTimed;

  GameLogic::CheckPoint mLastCheckPoint;
  float                 mLapsedTimeSinceInContact;

  std::vector<TimedMessage*>   mToUserMessages;  

  //  State related to tricks
  Tricks*   mTrickManager;
  bool      mNitroUsed;

  GameRecording*  mRecording;

protected:
  GameBp::NitroDescription        mNitroDesc;
  
  //  TODO: per ball (?)
  float mLoaded_FwdModule;
  float mLoaded_BckModule;
  float mLoaded_SidModule;
  float mLoaded_maxVelocity;
  
  D3DXVECTOR3 mLoadedInitPosition;
  int     mUser_idBall;
  
  //  loaded from disk: track configuration
  float         mLoaded_trackLength;
  int           mLoaded_trackSeed;

  //  loaded from disk: entity configuration
  float         mLoaded_archDensity;
  float         mLoaded_archDispersion;
  float         mLoaded_bumperDensity;
  float         mLoaded_bumperDispersion;  

  //  loaded from disk: difficulty configuration
  float   mLoaded_diff_chrono_mult[3];
  float   mLoaded_diff_lab_mult[3];
  float   mLoaded_diff_IA_Max_Speed;
  float   mLoaded_diff_Spartacus_Num_Opponents[3];
  float   mLoaded_diff_Track_Length_Multiplier[3];
  float   mLoaded_diff_Track_Width[3];
  float   mLoaded_diff_Track_Width_Entities[3];
  float   mLoaded_diff_Track_Num_Intersections[3];


  //  loaded from disk (TODO: could be ball specific?)
  int   mLoaded_NumStartingNitros;
  float mLoaded_NitroCoefImpulse;
  float mLoaded_NitroCoefMaxVel;
  float mLoaded_NitroTimeRecharge;    
  float mLoaded_NitroTimeVelChange;

  //  loaded from disk
  float mLoaded_maxStepTime,
        mLoaded_gravityAcc, 
        mLoaded_dragForce, 
        mLoaded_bounceCoefficient;

private:
  //  loading progress
  int                   mLoadState;
  int                   mLoadStateTrack;
  int                   mLoadStateTrackEnt;
  int                   mLoadStateTrackMisc;

  //  Copy of base resources
protected:
  ID3D10ShaderResourceView*           mPermTextureSRV;
public:
  void initDxBaseResources( ID3D10ShaderResourceView* aPermTextureSRV );

private:
  void  initIOandCameras();

  void  initBall();
  void  loadPhysicFile();
  void  initTrack();


  int   initTrackEntities();
  void  setPhysicsParameters();
  void  startPhysics();
  void  initCamera();
  void  initGameState();
  void  initModels();
  void  initCollisionObjects();
  void  dumpInitLogs();
  void  initIntroPieces();

  void  initColours();
  Mesh* loadMesh ( const char* aFileName );
  ColourMapping   mColours;

  //  special loading
  void  preInitTrack();
  void  allocateTrack();
  int   generateTrackGraph( const TrackBp::TrackDescription& aTrackDesc );
  int   generateTrack();

  //  logging
  void  logTrack();

private:
  void updateMessages( const double aLapsedTime );
  void addMessage( const float aTimeSeconds, const std::string& aMessage );


private:
  //preview
  void drawPreview( const float aCompletition );
protected:
  void drawBalls() const;

public:
  void  actionNitro( Ball& aBall );
  void  actionBrake( Ball& aBall );
  void  actionAddNitro( Ball& aBall );

  void  actionSetCheckPoint( const Ball& aBall, const D3DXVECTOR3& aDisplacement );
  void  actionRestoreCheckPoint( Ball& aBall );

  void  actionMaxVelChange( Ball& aBall, const float aInc, const float aTime  );
  void  actionInvertCommands( const float aTime );


  public:
  // copy of pointer to parent
  GameBpApp*            mGameApp;

  friend class GameBpApp;
  friend class GameRecording;
  friend class GameReplay;
  
  //    Game recording
private:
  GameReplay*   mReplay;
  void playRecording( const double aLapsedTime );

  //  Game logic
public:
  bool      areControlsInverted() const;
  bool      isGameFinished() const;
  bool      isGameWon() const;
  bool      isGoingBackwards() const;

};


#endif  //_GameBP_HPP_
