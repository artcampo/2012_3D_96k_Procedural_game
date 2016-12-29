#include  "GameBall/Game.hpp"
#include  "GameBall/Modes/GameChrono.hpp"
#include  "GameBall/Modes/GameLab.hpp"
#include  "GameBall/Modes/GameChase.hpp"
#include  "GameBall/Modes/GameSpartacus.hpp"
#include  "GameBall/Track/TrackGraphCollision.hpp"

#include  "Project_Dependant/IOinput.hpp"
#include  "GameBall/Entities/Arch.hpp"
#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Metropolis/MetropolisStructure.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"
#include  "GameBall/Recording/GameRecording.hpp"
#include  "GameBall/Recording/GameReplay.hpp"

Game::~Game()
{
  
  *mLog->mFileFps << "Fps mean: " << mFpsCounting.mean() << std::endl;
  *mLog->mFileFps << "Fps  max: " << mFpsCounting.maximum() << std::endl;
  *mLog->mFileFps << "Fps  min: " << mFpsCounting.minimum() << std::endl;

  if ( mIOinput != NULL )
    delete mIOinput;
  delete mPhysics;
  delete mCam;
  delete mLog;
#ifndef GBP_GFXDEBUG_TRACK
  delete mTrack;
  for ( int i = 0; i < mTrackModels.size() ; ++i)    
    delete mTrackModels[i];
#endif
  delete mBall;
  delete mEnd;

  for ( int i = 0; i < mArchs.size() ; ++i)    
    delete mArchs[i];
  
  for ( int i = 0; i < mArchsCol.size() ; ++i)
    delete mArchsCol[i];

  for ( int i = 0; i < mBumpers.size() ; ++i)
  {
    delete mBumpers[i];
    delete mBumpersCol[i];
  }
  

  
  delete mArchMesh;
  delete mBumperCubeMesh;
  delete mArchModel[0];       // WARNING: only one component
  delete mBumperCubeModel[0];
  delete mFog;

  mProfLoop->dumpToFile( "./log/profile_main_loop.txt" );  
  delete mProfLoop;
}

//  Private ctor
Game::Game()
{   
  mLog  = new Log();     
  mProfLoop = new ProfilerLoopDp( std::string( "Profile of physics" ) );

  mLoadState          = 0;
  mLoadStateTrack     = 0;
  mLoadStateTrackEnt  = 0;
  mLoadStateTrackMisc = 0;
  Global::mShowIterTrack = 0;   

  mLoadedInitPosition = D3DXVECTOR3( 10, 0.5, 0);

  mTrickManager = NULL;
  mTrack = NULL;
}


Game*  Game::gameChrono( const int aDifficulty,
          const int aFamily, const int aBall, GameBpApp* aGameApp   )
{
  Game* g = new GameChrono( aDifficulty );     
  g->mTrackDesc.family = aFamily;
  g->mTrackDesc.lapsed = true;
  g->mUser_idBall = aBall;
  g->mGameApp = aGameApp;
  return g;
}

Game*  Game::gameLab( const int aDifficulty,
          const int aFamily, const int aBall, GameBpApp* aGameApp )
{
  Game* g = new GameLab( aDifficulty );    
  g->mTrackDesc.family = aFamily;
  g->mTrackDesc.lapsed = false;
  g->mUser_idBall = aBall;
  g->mGameApp = aGameApp;
  return g;
}


Game*  Game::gameChase( const int aDifficulty,
          const int aFamily, const int aBall, GameBpApp* aGameApp )
{
  Game* g = new GameChase( aDifficulty );    
  g->mTrackDesc.family = aFamily;
  g->mTrackDesc.lapsed = false;
  g->mUser_idBall = aBall;
  g->mGameApp = aGameApp;
  return g;
}

Game*  Game::gameSpartacus( const int aDifficulty,
          const int aFamily, const int aBall, GameBpApp* aGameApp )
{
  Game* g = new GameSpartacus( aDifficulty );    
  g->mTrackDesc.family = aFamily;
  g->mTrackDesc.lapsed = false;
  g->mUser_idBall = aBall;
  g->mGameApp = aGameApp;
  return g;
}

void Game::initIOandCameras()
{
  mCam          = new Camera();
  mIOinput      = new IoInput( mCam, this, mGameApp );  	
  mCam3dPerson  = new Camera3rdPersonView( mCam );
  //mCam1stPerson = new Camera1stPersonView( mCam );
}

void Game::preInitTrack()
{
  //initTrack();
  
}

int Game::initGameObjects()
{
  if ( mLoadState == 0 )
  {
    
    mPhysics      = new Physics(); 
    //initIOandCameras(); 
  
    initCamera();  
    loadPhysicFile();  
    initModels();
    initIntroPieces();
    _initStates();

    mLoadState = 1;
    return 0;
  }

  if ( mLoadState == 1 )
  { 
    mTrack->generateTrackFinish();
    generateTrack();    
    int currentLoad = 100;
    if ( currentLoad == 100)
      mLoadState = 2;
    currentLoad = int ( float(currentLoad)*40.0f / 100.0f );    
    return 5 + currentLoad;
  }

  if ( mLoadState == 2 )
  {
    int currentLoad = initTrackEntities();
    if ( currentLoad == 100)
      mLoadState = 3;
    currentLoad = int ( float(currentLoad)*40.0f / 100.0f );
    return 45 + currentLoad;
  }

  if ( mLoadState == 3 )
  {
    int lastStateCount;
    if ( mLoadStateTrackMisc == 0 )
    {
      logTrack();
      mLoadStateTrackMisc = 1;
      lastStateCount = 10;
    }
    
    if ( mLoadStateTrackMisc == 1 )
    {
      initBall();
      mLoadStateTrackMisc = 2;
      lastStateCount = 20;
    }
    if ( mLoadStateTrackMisc == 2 )
    {
      mBox.init();  
      mLoadStateTrackMisc = 3;
      lastStateCount = 30;
    }

    if ( mLoadStateTrackMisc == 3 )
    {
      setPhysicsParameters();
      mLoadStateTrackMisc = 4;
      lastStateCount = 40;
    }
    if ( mLoadStateTrackMisc == 4 )
    {
      mLog->mProf->start( std::string("Kdd generation") );  
      startPhysics();
      mLog->mProf->end();
      mLoadStateTrackMisc = 5;
      lastStateCount = 50;
    }
    
    if ( mLoadStateTrackMisc == 5 )
    {
      initGameState();
      mLoadStateTrackMisc = 6;
      lastStateCount = 60;
    }
    
    if ( mLoadStateTrackMisc == 6 )
    {
      dumpInitLogs();
      mLoadStateTrackMisc = 7;
      lastStateCount = 70;
    }
    if ( mLoadStateTrackMisc == 7 )
    {
#ifndef GBP_GFXDEBUG_TRACK
      mTrackLinesModel    = mTrack->convertToLines();     
#endif
#ifdef GBP_GFXDEBUG_TRACK
      mTrackLinesModel.resize( mLoaded_diff_Track_Num_Intersections[mDifficulty] );
      for ( int i = 0; i < mLoaded_diff_Track_Num_Intersections[mDifficulty]; ++i)
        mTrackLinesModel[i]    = mTrack[i]->convertToLines();     
#endif
      mLoadStateTrackMisc = 8;
      lastStateCount = 80;
    }
    if ( mLoadStateTrackMisc == 8 )
    {
#ifndef GBP_GFXDEBUG_TRACK
      TrackBp::TrackFogBounding fogData = mTrack->getFogBoundingData();
#endif
#ifdef GBP_GFXDEBUG_TRACK
      TrackBp::TrackFogBounding fogData = mTrack[0]->getFogBoundingData();
#endif 
      
      mFog                = new FogGeometry(  fogData.xzPlaneHeight, 
                                              fogData.boundingCircRadius, 
                                              fogData.boundingCircRadius, 
                                              mPermTextureSRV );
      mEntitiesLinesModel = new EntitiesOnLinesAndTNBs( mArchs, mBumpers );     
      mLoadStateTrackMisc = 9;
      //TODO:DEBUG
      Global::gCam  = mCam;
      mRecording    = new GameRecording( this );     
      mReplay       = new GameReplay( this );  
      //TODO:DEBUG
      lastStateCount = 100;
    }
         
    return 85 + int ( float(lastStateCount)*15.0f / 100.0f );
  }

  
}

void Game::dumpInitLogs()
{
  mLog->mProf->dumpToFile( "./log/init/log_prof_init_timing.txt" );
  mLog->mProfGeom->dumpToFile( "./log/init/log_prof_init_timing_track.txt" );
}

void  Game::setPhysicsParameters()
{
  mPhysics->setParameters( mLoaded_gravityAcc );
}

void  Game::loadPhysicFile()
{
  using namespace std;
  ifstream fileStd ( "physics_vars.txt", fstream::in );  

  if ( fileStd.good() )
  { 
    fileStd >> mLoaded_maxStepTime;
    fileStd >> mLoaded_gravityAcc;
    fileStd >> mLoaded_dragForce;
    fileStd >> mLoaded_bounceCoefficient;
    fileStd >> mLoaded_FwdModule;
    fileStd >> mLoaded_BckModule;
    fileStd >> mLoaded_SidModule;    
    fileStd >> mLoaded_maxVelocity;  

    fileStd >> mLoaded_NumStartingNitros;
    fileStd >> mLoaded_NitroCoefImpulse;
    fileStd >> mLoaded_NitroCoefMaxVel;
    fileStd >> mLoaded_NitroTimeRecharge;
    fileStd >> mLoaded_NitroTimeVelChange;
    
    mNitroDesc.NumStartingNitros  = mLoaded_NumStartingNitros ;
    mNitroDesc.NitroCoefImpulse   = mLoaded_NitroCoefImpulse;
    mNitroDesc.NitroCoefMaxVel    = mLoaded_NitroCoefMaxVel ;
    mNitroDesc.NitroTimeRecharge  = mLoaded_NitroTimeRecharge * 100.0f;    
    mNitroDesc.NitroTimeVelChange = mLoaded_NitroTimeVelChange* 100.0f;
  }
}



void  Game::loadDifficultyFile( const char* aName )
{
  using namespace std;
  ifstream fileStd( aName, fstream::in );  

  if ( fileStd.good() )
  { 
    fileStd >> mLoaded_diff_chrono_mult[0];
    fileStd >> mLoaded_diff_chrono_mult[1];
    fileStd >> mLoaded_diff_chrono_mult[2];
    fileStd >> mLoaded_diff_lab_mult[0];
    fileStd >> mLoaded_diff_lab_mult[1];
    fileStd >> mLoaded_diff_lab_mult[2];
    fileStd >> mLoaded_diff_IA_Max_Speed;
    fileStd >> mLoaded_diff_Spartacus_Num_Opponents[0];
    fileStd >> mLoaded_diff_Spartacus_Num_Opponents[1];
    fileStd >> mLoaded_diff_Spartacus_Num_Opponents[2];
    fileStd >> mLoaded_diff_Track_Length_Multiplier[0];
    fileStd >> mLoaded_diff_Track_Length_Multiplier[1];
    fileStd >> mLoaded_diff_Track_Length_Multiplier[2];
    fileStd >> mLoaded_diff_Track_Width[0];
    fileStd >> mLoaded_diff_Track_Width[1];
    fileStd >> mLoaded_diff_Track_Width[2];
    fileStd >> mLoaded_diff_Track_Width_Entities[0];
    fileStd >> mLoaded_diff_Track_Width_Entities[1];
    fileStd >> mLoaded_diff_Track_Width_Entities[2];
    fileStd >> mLoaded_diff_Track_Num_Intersections[0];
    fileStd >> mLoaded_diff_Track_Num_Intersections[1];
    fileStd >> mLoaded_diff_Track_Num_Intersections[2];
  }
}

void  Game::loadTrackFile( const char* aName )
{
  using namespace std;
  ifstream fileStd( aName, fstream::in );  

  if ( fileStd.good() )
  { 
    fileStd >> mLoaded_trackLength;
    mLoaded_trackLength *= mLoaded_diff_Track_Length_Multiplier[mDifficulty];
    fileStd >> mLoaded_trackSeed;
  }
}


void  Game::loadEntityFile( const char* aName )
{
  using namespace std;
  ifstream fileStd( aName, fstream::in );  

  if ( fileStd.good() )
  {     
    fileStd >> mLoaded_archDensity;
    fileStd >> mLoaded_archDispersion;
    fileStd >> mLoaded_bumperDensity;
    fileStd >> mLoaded_bumperDispersion;

    mTrackEntDesc.archDensity       = mLoaded_archDensity;
    mTrackEntDesc.archDispersion    = mLoaded_archDispersion;
    mTrackEntDesc.bumperDensity     = mLoaded_bumperDensity;
    mTrackEntDesc.bumperDispersion  = mLoaded_bumperDispersion;

    mTrackEntDesc.archHeight    = 4.0f;
    mTrackEntDesc.bumperHeight  = 2.0f;
  }
  _initEntityDesc();
}

void  Game::initBall()
{
  GameBp::BallDescription ballDesc;
  ballDesc.idBall       = mUser_idBall;
  ballDesc.maxVelocity  = mLoaded_maxVelocity;
  ballDesc.fwdModule    = mLoaded_FwdModule;
  ballDesc.bckModule    = mLoaded_BckModule; 
  ballDesc.sidModule    = mLoaded_SidModule;
  ballDesc.maxVelTimeCoef = 0.3f;

  mLoadedInitPosition.x += 0.3f;
  mLoadedInitPosition.z += 0.3f;
  mLoadedInitPosition.y += 2;
  if ( mTrackDesc.layout == TrackBp::eTrackLayoutSpiral)
  {
    mLoadedInitPosition = mTrackDesc.layoutSpiralInitialBallPosition;
  }
  mBall = new Ball( mPhysics, mNitroDesc, ballDesc, mLoadedInitPosition, ballDesc.idBall, mPermTextureSRV );
  mBalls.push_back( mBall );

  _initBalls();
}


void Game::startPhysics()
{
  //  init data structures
#ifndef GBP_GFXDEBUG_TRACK
  mPhysics->initStaticStructures();
#endif
  //mPhysics->advanceSimulation( 0.01f );
}

int Game::initTrackEntities()
{
  
  //  TODO: INIT TIME
  if (mLoadStateTrackEnt == 0 )
  {
    mLog->mProf->start( std::string("Generate Entities") );
    mLoadStateTrackEnt = 1;
    return 0;
  }

  if (mLoadStateTrackEnt == 1 )
  {
#ifndef GBP_DISABLE_ARCH_CREATION
    mArchs = mTrack->placeArchs     ( mTrackEntDesc, this );
#endif
    mLoadStateTrackEnt = 2;
    return 30;
  }
  
  if (mLoadStateTrackEnt == 2 )
  {    
#ifndef GBP_DISABLE_BUMP_CREATION
    mBumpers = mTrack->placeBumpers ( mTrackEntDesc, this );
#endif
    mLog->mProf->end();
    mLoadStateTrackEnt = 3;
    return 60;
  }


 
  //  TODO: INIT TIME
  
  
  if (mLoadStateTrackEnt == 3 )
  {
    mLog->mProf->start( std::string("Generate Collision bodies: archs") );
    for (int i = 0; i < mArchs.size(); ++i)  
    {
      D3DXMATRIX basisRender = mArchs[i]->basisRender();
      D3DXMATRIX m; D3DXMatrixRotationY( &m, D3DX_PI*0.5f );
      basisRender = m*basisRender;
      MeshCol::meshColNonConvex( mArchMesh, &basisRender, mArchsCol );   
    }
  
    for (int i = 0; i < mArchsCol.size(); ++i)
      mPhysics->createStaticRigidBody( mArchsCol[i] );    

    mLog->mProf->end();
    mLoadStateTrackEnt = 4;
    return 80;
  }
   
  if (mLoadStateTrackEnt == 4 )
  {
    mLog->mProf->start( std::string("Generate Collision bodies: bumpers") );
    #ifndef GBP_DISABLE_BUMP_CREATION
      mBumpersCol.resize( mBumpers.size() );
      for (int i = 0; i < mBumpers.size(); ++i)
      {    
        mBumpersCol[i] = MeshCol::meshColConvex( mBumperCubeMesh, &mBumpers[i]->basisRender() );        
        mPhysics->createStaticRigidBody( mBumpersCol[i] );
      }
    #endif
      mLog->mProf->end();

    mLoadStateTrackEnt = 5;
    return 100;
  }

}



void Game::initTrack()
{  
}

void Game::allocateTrack()
{

}

int   Game::generateTrackGraph( const TrackBp::TrackDescription& aTrackDesc )
{
  mTrackDesc = aTrackDesc;
  //  prepare slice
  std::vector<D3DXVECTOR3> slice    ( StoredSplines::gSplineSliceNumElems );
  std::vector<D3DXVECTOR3> sliceNo  ( StoredSplines::gSplineSliceNumElems );
  for (int i = 0; i < StoredSplines::gSplineSliceNumElems; ++i )
  {
    slice[i ]     = Spline_SliceBorder  [i];
    sliceNo[i]    = Spline_SliceNoBorder[i];
  }

  

//  TODO: INIT TIME
  
  TrackGraphCollision::CollisionConfiguration colConfiguration;
  colConfiguration.mHaloLength    = 2;
  colConfiguration.mSphereRadius  = 10;
  
#ifndef GBP_GFXDEBUG_TRACK
  if ( mTrack != NULL )
    delete mTrack;
  mTrack = new Track( mTrackDesc, slice, sliceNo, aTrackDesc.seed, colConfiguration );  
#endif
#ifdef GBP_GFXDEBUG_TRACK
  mTrack.resize( mLoaded_diff_Track_Num_Intersections[mDifficulty] );
  for ( int i = 0; i < mLoaded_diff_Track_Num_Intersections[mDifficulty]; ++i)
  {
    mTrack[i] = new Track( mTrackDesc, slice, sliceNo, aTrackDesc.seed, colConfiguration );    
  }
#endif

  

  Global::mShowIterTrack = mLoaded_diff_Track_Num_Intersections[mDifficulty] - 1;

  //if ( mTrackDesc.family == TrackBp::eTrackLinear ) ;
  if ( mTrackDesc.family == TrackBp::eTrackFall)      mLoadedInitPosition += D3DXVECTOR3(5,0.5,0);
  if ( mTrackDesc.family == TrackBp::eTrackAbility )  mLoadedInitPosition += D3DXVECTOR3(-6,1,0);
  

  return 0;
}

int Game::generateTrack()
{

  //  TODO: INIT TIME
  mLog->mProf->start( std::string("Track generation") );

#ifndef GBP_GFXDEBUG_TRACK
   mEnd = mTrack->generateEndTrigger( this );
#endif
#ifdef GBP_GFXDEBUG_TRACK
   mEnd = mTrack[0]->generateEndTrigger( this );
#endif 
  
  mLog->mProf->end();

  //  TODO: INIT TIME
  mLog->mProf->start( std::string("Loft-Surface generation") );  
  
#ifndef GBP_GFXDEBUG_TRACK
  mTrack->generateLoftSurface();
#endif
#ifdef GBP_GFXDEBUG_TRACK  
  for ( int i = 0; i < mLoaded_diff_Track_Num_Intersections[mDifficulty]; ++i)
  {
    mTrack[i]->generateLoftSurface();    
  }
#endif

  mLog->mProf->end();

  //  Generate collision and render model
  //  TODO: INIT TIME
  mLog->mProf->start( std::string("Generate Dx models") );
#ifndef GBP_GFXDEBUG_TRACK
  mTrackModels = mTrack->model();
#endif
#ifdef GBP_GFXDEBUG_TRACK
  mTrackModels.resize( mLoaded_diff_Track_Num_Intersections[mDifficulty] );
  for ( int j = 0; j < mLoaded_diff_Track_Num_Intersections[mDifficulty]; ++j)
  {    
    mTrackModels[j] = mTrack[j]->model();
  }
#endif
  mLog->mProf->end();




  //  TODO: INIT TIME
  mLog->mProf->start( std::string("Generate Collision bodies: track") );
#ifndef GBP_GFXDEBUG_TRACK
  mTrackModCol = mTrack->meshColTri();
#endif  
  mLog->mProf->end();

  //  TODO: INIT TIME
  mLog->mProf->start( std::string("Create KDD tree") );
  for (int i = 0; i < mTrackModCol.size(); ++i)
    mPhysics->createStaticRigidBody( mTrackModCol[i] );
  mLog->mProf->end();

  mLog->mProf->start( std::string("Create Metropolis Structure") );

#ifndef GBP_GFXDEBUG_TRACK
  mMetropolis = new MetropolisStructure( *mTrack, mTrack->mRand );      
#endif
#ifdef GBP_GFXDEBUG_TRACK
  mMetropolis = new MetropolisStructure( *mTrack[0], mTrack[0]->mRand );
#endif   
  
  mLog->mProf->end();

  return 100;
}

void Game::initCamera()
{  
  #ifdef GBP_GFXDEBUG_TRACK

    mCam->setPositionAndAt(      
  &D3DXVECTOR3(0.0f, 50.0f, 0.0f),    
  &D3DXVECTOR3(50, 0, 0) ); 
    #ifdef CAMERA_FOR_SSDO_TEST
    mCam->setPositionAndAt(
      //&D3DXVECTOR3(1,1,-3), 
      &D3DXVECTOR3(34, 7,8),
      &D3DXVECTOR3(34,-13,-11) ); 
    #endif  
  #endif
  #ifndef GBP_GFXDEBUG_TRACK
    mCam->setPositionAndAt(
      //&D3DXVECTOR3(1,1,-3), 
      &D3DXVECTOR3(-50, 50,0),
      &D3DXVECTOR3(0,0,0) ); 
  #endif
}


void Game::initGameState()
{  
  _initGameState();
 
  //  Set Ball
  mBall->mBallBody->xWS = mLoadedInitPosition;
  mBall->mBallBody->standstill();
  mBall->inGameReset();

  mBall->mBallBody->computeTemporalValues();
  mCam3dPerson->updateTargetParameters( mBall->mBallBody->getxWS(), D3DXVECTOR3(1,0,0), D3DXVECTOR3(0,1,0), D3DXVECTOR3(1,0,0), 0.1 );  

  //  Set other  
  mGameFinished             = false;
  mInvertCommandsTimed      = TimeLimitedEvent( 0.0f, false );  
  mLapsedTime               = 0.0f;
  mLapsedTimeSinceInContact = 0.0f;
  actionSetCheckPoint( *mBall, D3DXVECTOR3(0,0,0) );

  if ( mTrickManager != NULL )  
    delete mTrickManager;
  mTrickManager = new Tricks();
  Global::mPunctuation = 0;
  
}

Mesh*  Game::loadMesh ( const char* aFileName )
{
  using namespace std;
  Mesh* mesh = new Mesh();  
  fstream fileStd ( aFileName, fstream::in );  

  if ( fileStd.good() )
  {  
    mesh->load( fileStd );
    fileStd.close();
    return mesh;
  }
  else 
    return NULL;
}



void Game::initColours()
{
  mColours.obj      = D3DXCOLOR( 0.8f, 0.0f, 0.0f, 1.0f );
  mColours.objSel   = D3DXCOLOR( 0.8f, 0.0f, 0.0f, 1.0f );
  mColours.objHigh  = D3DXCOLOR( 0.8f, 0.0f, 0.0f, 1.0f );

  mColours.ver      = D3DXCOLOR( 0.8f, 0.0f, 0.0f, 1.0f );
  mColours.verSel   = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
  mColours.verHigh  = D3DXCOLOR( 0.0f, 0.0f, 0.8f, 1.0f );

  mColours.edg      = D3DXCOLOR( 0.8f, 0.0f, 0.0f, 1.0f );
  mColours.edgSel   = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
  mColours.edgHigh  = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );


  mColours.fac      = D3DXCOLOR( 0.2f, 0.2f, 0.8f, 0.2f );
  mColours.facSel   = D3DXCOLOR( 0.8f, 0.8f, 0.0f, 0.2f );
  mColours.facHigh  = D3DXCOLOR( 0.6f, 0.6f, 0.6f, 0.2f );

  mColours.gizmo[0] = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
  mColours.gizmo[1] = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );
  mColours.gizmo[2] = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );

  mColours.gizmoHigh[0] = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
  mColours.gizmoHigh[1] = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
  mColours.gizmoHigh[2] = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

  mColours.gizmoSel[0] = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
  mColours.gizmoSel[1] = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
  mColours.gizmoSel[2] = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
}

void  Game::initModels()
{
 
  mArchMesh =  loadMesh( "./arch.nbo" );

  mArchModel = mArchMesh->convertToDeferred( Global::md3dDevice, 
                        D3DXVECTOR3( 10,0,0),
                        &mColours);

  mBumperCubeMesh =  loadMesh( "./bumpercube.nbo" );

  mBumperCubeModel = mBumperCubeMesh->convertToDeferred( Global::md3dDevice, 
                        D3DXVECTOR3( 10,0,0),
                        &mColours);
  
}

void Game::initDxBaseResources( ID3D10ShaderResourceView* aPermTextureSRV )
{
  mPermTextureSRV = aPermTextureSRV;
}

void  Game::initIntroPieces()
{
  /*
  mIntroPath = new TrackPath( true );
  TrackBp::Vertex a ( D3DXVECTOR3( 0,0,0),
                      D3DXVECTOR3( 1,0,0),
                      D3DXVECTOR3( 0,1,0) );
  TrackBp::Vertex b ( D3DXVECTOR3( 10,0,0),
                      D3DXVECTOR3( 0,0,1),
                      D3DXVECTOR3( 0,1,0) );
  mIntroPath->addEdge( a, b, 3, mTrack[0]->mSliceBorder );
  mIntroPath->generateLoftSurface();
  mIntroPathModel = mIntroPath->model();
*/
}