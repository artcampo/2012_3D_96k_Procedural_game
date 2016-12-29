#include "GameBpApp.hpp"
#include "Project_Dependant/IOinput.hpp"
#include "Project_Dependant/ConditionalCompilation.hpp"
#include "App/MainMenu/GameBpMainMenu.hpp"
#include "App/MainMenu/GameBpMainMenuWidget.hpp"
#include "Project_Dependant/IOTextInput.hpp"
#include "App/MainMenu/GameBpMainMenu.hpp"

GameBpApp::GameBpApp(HINSTANCE& handler, int width, int height, bool isFullScreen, 
               std::wstring wTitle, bool showFPS): 
               BaseDX(hInstance, width, height, isFullScreen, wTitle, showFPS)
{
  mLog    = new Log(); 
  mDxInitialized = false;  
  mGameInitialized = false;
  

  mUserDifficulty = 0;
  mUserBall = 0;
  mUserTrack = 0;
  mUserMode = 0;

  {
    std::vector<std::string> s;
    s.push_back( std::string("Continue") );
    s.push_back( std::string("Reset") );
    s.push_back( std::string("Main menu") );
    s.push_back( std::string("Exit") );
    mInGameMenu = new GameBpMenu(s);
  }
  {
    std::vector<std::string> s;    
    s.push_back( std::string("Reset") );
    s.push_back( std::string("Main menu") );
    s.push_back( std::string("Exit") );
    mGameOverMenu = new GameBpMenu(s);
  }    
  resetMenu();  
}

void GameBpApp::resetMenu()
 {

 }

GameBpApp::~GameBpApp()
{
  debugEnd();
  if ( mGameInitialized )
  {
    delete mGame;
    mGameInitialized = false;
  }
  delete mLog;
  delete mInGameMenu;
  delete mGameOverMenu;
  delete mIOLoadScreen;
}

bool GameBpApp::init()
{
	if (!BaseDX::init()) return false;

  debugStart();      

  return true; 
}


void GameBpApp::onResize()
{
	BaseDX::onResize();
}

void GameBpApp::drawScene()
{
  if (!mTimeInit)
  {
    mTimeInit = true;
    mLastTime = getCentiseconds();
    
  }
  double time   = getCentiseconds();
  double lapsed = time - mLastTime;
  mLastTime = time;
  
  *Global::mFileDebugFps << "Fps: " << nFrame << " [ " <<mGame->mPhysics->mDebugFrame << "]" << std::endl;    

	BaseDX::drawScene();
  
#ifdef _DP_PHYSICS_DEBUG_FIXED_TIME_STEP
  //lapsed = 0.3;   //slow motion
  lapsed = 1;
#endif
  if ( mGameInCourse && !mGame->isGameFinished() )
    mGame->update( lapsed );
  else
  {
    mGame->playRecording( lapsed );
  }

  DxFustrum fustrum;
  mGame->drawScene( fustrum, !mGame->isGameFinished() );

#ifdef GBP_ENABLE_DEBUG_GFX_COLLISION_SPHERES
  {
    TrackGraphCollision::CollisionData c = mGame->mTrack[ mGame->mTrack.size() - 1 ]->collisionData();
    for ( int i = 0; i < c.mEdges.size() ; ++i )
    {        
      for ( int j = 0; j < c.mEdges[i].size() ; ++j )
      {
        // check against whole edge
        for ( int b1 = 0; b1 < c.mEdges[i][j].mSpheres.size() ; ++b1 )        
        {          
              
          D3DXMatrixTranslation(  &Global::mWorld, 
                                  c.mEdges[i][j].mSpheres[b1].center.x,
                                  c.mEdges[i][j].mSpheres[b1].center.y,
                                  c.mEdges[i][j].mSpheres[b1].center.z);

          D3DXMATRIX m;    
          float scale = c.mEdges[i][j].mSpheres[b1].radius;
          D3DXMatrixScaling(&m, scale, scale, scale);
          m = m*Global::mWorld;    

          ID3D10EffectPass* pass;
          mShaderManager.mShaderTable[indexShadDefGeomV].SetMatrix( &(std::string("World")), &m );
          mShaderManager.mShaderTable[indexShadDefGeomV].SetMatrix( &(std::string("View")), &Global::mView );
          mShaderManager.mShaderTable[indexShadDefGeomV].SetMatrix( &(std::string("Projection")), &Global::mProjection );
          mShaderManager.mShaderTable[indexShadDefGeomV].SetResource( &(std::string("PermTexture")), mPermTextureSRV );
          mShaderManager.mShaderTable[indexShadDefGeomV].SetResource( &(std::string("textWorley")), mShaderManager.mWorleyTextureSRV);
    
          Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV].mVertexLayout );  
          Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );  
          pass = mShaderManager.mShaderTable[indexShadDefGeomV].mTech->GetPassByIndex(0);
          pass->Apply(0);

          mSphere->display();
          //box.display();

        }
      }
      
    }
    
  }
#endif

#ifdef GBP_DISABLE_LIGHTS
  nLights = 0;
#endif
#ifndef GBP_DISABLE_LIGHTS  
  
  pLights[0].center = Global::mBallPosition;
  pLights[0].colour = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
  pLights[0].radius = 10;

  nLights = 1;
  
  int i = 1;
  float d;
  for ( int j = 0; j < mGame->mArchs.size() && i < maxDefLights; ++j ) 
  {
    if ( fustrum.isInside( mGame->mArchs[j]->mCenter, d ) )
    {
      pLights[i].center = mGame->mArchs[j]->mCenter + D3DXVECTOR3(0,3,0);
      pLights[i].colour = mGame->mArchs[j]->color();      
      pLights[i].radius = 15;
      ++i; 
      ++nLights;
    }
  }

  for ( int j = 0; j < mGame->mBumpers.size() && i < maxDefLights ; ++j )    
  {
    if ( fustrum.isInside( mGame->mBumpers[j]->mCenter, d ) )
    {
      pLights[i].center = mGame->mBumpers[j]->mCenter + D3DXVECTOR3(0,3,0);
      pLights[i].colour = D3DXVECTOR4(0.8f,0.8f,0.8f,1);      
      pLights[i].radius = 10;
      ++i;
      ++nLights;
    }
  }
#endif
  

}

void  GameBpApp::debugStart()
{

  std::vector<const char*> names( 10 );
  names[0] = "./log/phys/bodies/body_0.txt";
  names[1] = "./log/phys/bodies/body_1.txt";
  names[2] = "./log/phys/bodies/body_2.txt";
  names[3] = "./log/phys/bodies/body_3.txt";
  names[4] = "./log/phys/bodies/body_4.txt";
  names[5] = "./log/phys/bodies/body_5.txt";
  names[6] = "./log/phys/bodies/body_6.txt";
  names[7] = "./log/phys/bodies/body_7.txt";
  names[8] = "./log/phys/bodies/body_8.txt";
  names[9] = "./log/phys/bodies/body_9.txt";

  Global::mFileDebugBody.resize(10);
  for ( int i = 0; i < Global::mFileDebugBody.size() ; ++i )
  {
    Global::mFileDebugBody[i] = new std::ofstream ( names[i], std::fstream::out );  
    Global::mFileDebugBody[i]->setf(std::ios::fixed,std::ios::floatfield); 
    Global::mFileDebugBody[i]->precision(2);
  }

  Global::mFileDebugContacts = new std::ofstream ( "./log/phys/log_physics_contacts.txt", std::fstream::out );  
  Global::mFileDebugContacts->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugContacts->precision(4);

  Global::mFileDebugCol = new std::ofstream ( "./log/phys/log_physics_collision.txt", std::fstream::out );  
  Global::mFileDebugCol->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugCol->precision(1);

  Global::mFileDebugColOffenders = new std::ofstream ( "./log/phys/log_physics_collision_offenders.txt", std::fstream::out );  
  Global::mFileDebugColOffenders->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugColOffenders->precision(4);

  Global::mFileDebugPhysStep = new std::ofstream ( "./log/phys/log_physics_pos_step.txt", std::fstream::out );  
  Global::mFileDebugPhysStep->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugPhysStep->precision(6);



  Global::mFileDebugColIt = new std::ofstream ( "./log/phys/log_physics_col_iter.txt", std::fstream::out );  
  Global::mFileDebugColIt->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugColIt->precision(10);


  Global::mFileDebugPhysGen = new std::ofstream ( "./log/phys/log_physics_general.txt", std::fstream::out );  
  Global::mFileDebugPhysGen->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugPhysGen->precision(12);

  Global::mFileDebugTime = new std::ofstream ( "./log/phys/log_physics_time.txt", std::fstream::out );  
  Global::mFileDebugTime->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugTime->precision(12);

  Global::mFileDebugTimeProf = new std::ofstream ( "./log/phys/log_physics_time_profile.txt", std::fstream::out );  
  Global::mFileDebugTimeProf->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugTimeProf->precision(0);

  Global::mFileDebugCamera = new std::ofstream ( "./log/game/log_camera.txt", std::fstream::out );  
  Global::mFileDebugCamera->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugCamera->precision(2);


  Global::mFileDebugKeys = new std::ofstream ( "./log/game/log_keys.txt", std::fstream::out );  
  Global::mFileDebugKeys->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugKeys->precision(2);
  
  Global::mFileDebugNumStatic = new std::ofstream ( "./log/phys/log_phys_num_static.txt", std::fstream::out );  
  Global::mFileDebugNumStatic->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugNumStatic->precision(2);
  
  Global::mFileDebugFps = new std::ofstream ( "./log/game/log_fps.txt", std::fstream::out );  
  Global::mFileDebugFps->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugFps->precision(0);
  
  Global::mFileDebugTracks = new std::ofstream ( "./log/init/log_tracks.txt", std::fstream::out );  
  Global::mFileDebugTracks->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugTracks->precision(1);
  
  Global::mFileDebugTrack = new std::ofstream ( "./log/init/log_track.txt", std::fstream::out );  
  Global::mFileDebugTrack->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebugTrack->precision(1);
   

}

void  GameBpApp::debugEnd()
{
  for ( int i = 0; i < Global::mFileDebugBody.size() ; ++i )
  {
    Global::mFileDebugBody[i]->flush();
    Global::mFileDebugBody[i]->close();
  }

  Global::mFileDebugTrack->flush();
  Global::mFileDebugTrack->close();
  Global::mFileDebugTracks->flush();
  Global::mFileDebugTracks->close();
  
  
  Global::mFileDebugContacts->flush();
  Global::mFileDebugContacts->close();
  Global::mFileDebugCol->flush();
  Global::mFileDebugCol->close();
  Global::mFileDebugColOffenders->flush();
  Global::mFileDebugColOffenders->close();
  Global::mFileDebugPhysStep->flush();
  Global::mFileDebugPhysStep->close();
  
  
  Global::mFileDebugPhysGen->flush();
  Global::mFileDebugPhysGen->close();
  Global::mFileDebugTimeProf->flush();
  Global::mFileDebugTimeProf->close();
  Global::mFileDebugTime->flush();
  Global::mFileDebugTime->close();
  Global::mFileDebugColIt->flush();
  Global::mFileDebugColIt->close();
  Global::mFileDebugCamera->flush();
  Global::mFileDebugCamera->close();
  Global::mFileDebugKeys->flush();
  Global::mFileDebugKeys->close();
  Global::mFileDebugNumStatic->flush();
  Global::mFileDebugNumStatic->close();
  Global::mFileDebugFps->flush();
  Global::mFileDebugFps->close();
   
  
}

int GameBpApp::mainLoadScreen()
{
  MSG msg = {0};   
  bool    loadFinished = false;  
  mIOLoadScreen = new IOLoadScreen( mGame->mCam );

  while(msg.message != WM_QUIT && !loadFinished )
	{		
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE )) {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
		}		
    else {    
      calculateTime();      

      mIOLoadScreen->initIoReading( getDeltaTime() );
      getKeyboardMenu( mIOLoadScreen );   
            
      mTimeInit = false;    
      mLoadCompleteBar = mGame->initGameObjects();
      loadFinished = (mLoadCompleteBar == 100);
      setDeferred();     
      //mGame->drawPreview( float(mLoadCompleteBar)/100.0f );
      applyLightsDeferred();      
      compositeDeferred();
      drawTextLoad(); 
      
      mSwapChain->Present( 0, 0 ); 
    }
  }

  if ( loadFinished )
    return 1;
  else
    return 0;
  //return (int)msg.wParam;
}

void GameBpApp::drawTextLoad()
{  
  std::wostringstream textStream;   
	textStream.precision(0);

  textStream 
    << L"Loading: "<< mLoadCompleteBar << L"\n";

  text = textStream.str();
  
  RECT Rect = {5, 5, 0, 0};
  //SACA UN WARNING A SABER PORQUE
  mFont->DrawText(0, text.c_str(), -1, &Rect, DT_NOCLIP, WHITE);	
}

void GameBpApp::initGamePrevious()
{   
  mGame->loadTrackFile      ( "track_cfg.txt" );
  mGame->loadEntityFile     ( "entity_cfg.txt" );  
  mGame->initDxBaseResources( mPermTextureSRV );
          
  mGame->initIOandCameras(); 
  mTimeInit = false;
}



int GameBpApp::Main()
{
  startTimer();
  MSG msg = {0}; 
  mGameInitialized  = true;
  mGameInCourse     = true;
  mIoMenu.Move(K_NON_EXISTANT);
  mInGameMenu->setAction(Menu::eNoAction);

  while(msg.message != WM_QUIT)
	{		
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE )) {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
		}		
    else {      

      if ( mGameInCourse && !mGame->isGameFinished() )
      {
        calculateTime();      
        mGame->handlerIOinput()->initIoReading( getDeltaTime() );
        getKeyboard( mGame->handlerIOinput() );   
        mGame->handlerIOinput()->finishIoReading( &mMouseIncrement );
      }
      mMouseIncrement = D3DXVECTOR3 ( 0.0f, 0.0f, 0.0f );
      
      setDeferred();    

      drawScene();            
#ifndef _GBP_FAST_RENDER
      applyLightsDeferred();      
      compositeDeferred();
#endif
      BaseDX::drawText();

      drawText( mGame->getMainMessage(), 3, D3DXCOLOR(1,0,0,1) );
      drawText( mGame->getComboMessage(), 12, D3DXCOLOR(1,0,0,1) );
      
      
      //////////////////////////////////////////////////////////
      //  Draw menu
      if ( !mGameInCourse && !mGame->isGameFinished() )
      {  
        //////////////////////////////////////////////////////////
        //  Draw menu - in game
        mIoMenu.menu( mInGameMenu );
        getKeyboardMenu( &mIoMenu );  
        displayInGameMenu();
        int action = mInGameMenu->getAction();

        if ( action == Menu::eFallBack )
          inGameMenuRestore();

        if ( action == Menu::eAccepted )
        {
          if ( mInGameMenu->index() == 0 )
            inGameMenuRestore();
          if ( mInGameMenu->index() == 1 )
            inGameMenuReset();
          if ( mInGameMenu->index() == 2 )
          {
            resetMenu();
            return 1;
          }
          if ( mInGameMenu->index() == 3 )
            return 0;          
        }
      }
                    
      if ( mGame->isGameFinished() )
      {
        //////////////////////////////////////////////////////////
        //  Draw menu - game over
        mIoMenu.menu( mGameOverMenu );
        getKeyboard( &mIoMenu );  
        displayGameOverMenu();
        int action = mGameOverMenu->getAction();

        if ( action == Menu::eFallBack )
        {
          resetMenu();
          return 1;
        }

        if ( action == Menu::eAccepted )
        {
          int index = mInGameMenu->index();
          resetMenu();
          if ( index == 0 )
            inGameMenuReset();
          if ( index == 1 )          
            return 1;          
          if ( index == 2 )
            return 0;          
        }
      }      

      mSwapChain->Present( 0, 0 ); 
    }
  }

  return (int)msg.wParam;
}

int GameBpApp::menuMain()
{
  if ( !mDxInitialized )
  {
    //Initialization
    if( !init() ) {
      //std::cerr << "Initialization failed" << std::endl;
      return 0;
    }
    mDxInitialized = true;
  }

  startTimer();
  MSG msg = {0};   

  
  
  initGameWithUserOptions();
  initGamePrevious();
  mIoMainMenu = new IOMainMenu( mGame->mCam );
  Global::gCam = mGame->mCam;
  IOTextInput* ioSaveMenu = new IOTextInput();
  
  mGame->_initTrackDesc();
  TrackBp::TrackDescription d = mGame->getTrackDesc();
  
  mMainMenu = new GameBpMainMenu( &d, &mGame->mTrackEntDesc );

  mMainMenu->mItemWidget[MainMenu::eLayout]->setIndex(1);
  mMainMenu->mItemWidget[MainMenu::eNumIterations]->setIndex(15);   //15
  mMainMenu->mItemWidget[MainMenu::eLongFirst]->setIndex(1);        //0
  mMainMenu->mItemWidget[MainMenu::eSeed]->setIndex(20);  
  mMainMenu->mItemWidget[MainMenu::eLength]->setIndex(0);           //8
  mMainMenu->mItemWidget[MainMenu::eAbility]->setIndex(40);       
  mMainMenu->mItemWidget[MainMenu::eTrackWidth]->setIndex(10);
  mMainMenu->mItemWidget[MainMenu::eArchDensity]->setIndex(5);
  mMainMenu->mItemWidget[MainMenu::eBumperDensity]->setIndex(10);
  mMainMenu->mItemWidget[MainMenu::eArchProb]->setIndex(18);
  mMainMenu->mItemWidget[MainMenu::eBumperProb]->setIndex(0);
  
    
  //TrackLinesAndTNBs* lines = mGame->mTrack->convertToLines();

  bool  onSave = false;
  int   computedIterations  = 0;
  int   computedIterationsForCorrection  = 0;
  bool  askedToFinish       = false;

  Track* trackA = NULL;
  Track* trackB = NULL;

  while(msg.message != WM_QUIT)
	{		
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE )) {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
		}		
    else {      
      onSave = mMainMenu->onSave();

      if ( !onSave )
      {
        ioSaveMenu->reset();
        if ( !askedToFinish )
        {
        mMainMenu->setAction( MainMenu::eNoAction );
        mIoMainMenu->menu( mMainMenu );
        getKeyboardMenu( mIoMainMenu );   

        if ( mMainMenu->selectedPlay() )                  
          askedToFinish = true;
        if ( mMainMenu->selectedExit() )                  
          return 0;
        }

        bool redoA = mMainMenu->redoTrackA();
        bool redoB = mMainMenu->redoTrackB();
        if ( redoA || redoB )
        {          
            d.layout                      = mMainMenu->mItemWidget[MainMenu::eLayout]->value();
            d.iterations                  = mMainMenu->mItemWidget[MainMenu::eNumIterations]->value();
            d.iterationsAbility           = mMainMenu->mItemWidget[MainMenu::eAbility]->value();
            d.substituteLongerEdgesFirst  = mMainMenu->mItemWidget[MainMenu::eLongFirst]->value(); 
            d.seed                        = mMainMenu->mItemWidget[MainMenu::eSeed]->value();
            d.totalLength                 = mMainMenu->mItemWidget[MainMenu::eLength]->value();
            
            //  these do not affect recreation
            d.trackWidth  = mMainMenu->mItemWidget[MainMenu::eTrackWidth]->value();          
            mGame->mTrackEntDesc.archDensity        = mMainMenu->mItemWidget[MainMenu::eArchDensity]->value();
            mGame->mTrackEntDesc.bumperDensity      = mMainMenu->mItemWidget[MainMenu::eBumperDensity]->value();
            mGame->mTrackEntDesc.probEdgeWithArch   = mMainMenu->mItemWidget[MainMenu::eArchProb]->value();
            mGame->mTrackEntDesc.probEdgeWithBumper = mMainMenu->mItemWidget[MainMenu::eBumperProb]->value();

            if ( redoA  )
            {
              mGame->generateTrackGraph(d);
              mGame->mTrack->generateGraph( d.iterations, false, 0 );
              computedIterations = 1;
              computedIterationsForCorrection = 0;
            }
            else
            {
              mGame->mTrack = trackA;
              computedIterations = d.iterations;
              computedIterationsForCorrection = 0;
            }
        }
        else
        {
          //  Normal substitutions
          if ( computedIterations < d.iterations )
          {
            mGame->mTrack->generateGraph( d.iterations, false, computedIterations );
            ++computedIterations;

            if ( computedIterations == d.iterations )            
              trackA = mGame->mTrack;
          }
          else
          {
            //  Change piece set
            if ( computedIterations == d.iterations )
            {   
              if ( d.iterationsAbility != 0 )
                mGame->mTrack->generateGraphChangeToAbilitySet();
              else
                mGame->mTrack->generateGraphChangeToAzimuthSet();
            }

            //  Ability substitutions
            if ( (computedIterations - d.iterations) < d.iterationsAbility )
            {
              mGame->mTrack->generateGraph( d.iterations, false, computedIterations );
              ++computedIterations;
              if ( (computedIterations - d.iterations) == d.iterationsAbility)
              {
                mGame->mTrack->generateGraphChangeToAzimuthSet();
                computedIterationsForCorrection = 0;
              }
            }
            else
            {
              if ( computedIterations == d.iterations + d.iterationsAbility )              
                mGame->mTrack->generateGraphChangeToAzimuthSet();
              
              //  Azimuth substitutions
              if ( mGame->mTrack->generateGraphNeedsCorrectionOfAzimuth() )
              {
                mGame->mTrack->generateGraph( d.iterations, false, computedIterationsForCorrection, true );
                ++computedIterationsForCorrection;
              }
            }
          }
        }

        if ( askedToFinish 
          && computedIterations >= (d.iterations + d.iterationsAbility)
          && !mGame->mTrack->generateGraphNeedsCorrectionOfAzimuth())
          return 1;
      }// end onSave conditional

      if ( onSave )
      {
        ioSaveMenu->menu( mMainMenu );
        getKeyboardTextIn( ioSaveMenu );   
      }

      setDeferred();  

      mGame->drawPreview( float(mLoadCompleteBar)/100.0f );
      applyLightsDeferred();      

      compositeDeferred();
      
      for ( int i = 0; i < mMainMenu->mItems.size(); ++i)
      {
        drawMenuItem( mMainMenu->mItems[i], mMainMenu->index() == i, i );

      }

      for ( int i = 0; i < mMainMenu->mItems.size(); ++i)
      {        
        if ( i == MainMenu::eSavePreset )        
        {
          if ( onSave )
            drawMenuItem( mMainMenu->mSaveFileName, false, i, false, true  );
        }
        else
          drawMenuItem( mMainMenu->subWidget(i), false, i, false, true );
      }

      if ( computedIterations < d.iterations )
        drawMenuItem( std::string("Computing"), false, 0, false, true );
      else
      {
        if ( computedIterations < (d.iterations + d.iterationsAbility) )
          drawMenuItem( std::string("Computing Ability"), false, 0, false, true );
        else
        {
          if ( mGame->mTrack->generateGraphNeedsCorrectionOfAzimuth() )
            drawMenuItem( std::string("Correcting elevation"), false, 0, false, true );
        }
      }
            
      mSwapChain->Present( 0, 0 ); 
    }
  }

  return (int)msg.wParam;
}




void GameBpApp::drawText ( std::string& aString ) const
{
  if( !aString.empty() )
    drawText( aString, true, D3DXCOLOR(1,0,0,1) );
}

void GameBpApp::drawText ( std::string& aString, bool aCentered, D3DXCOLOR aCol ) const
{
  std::wstring s = WinIO::s2ws( aString );    
  //RECT Rect = {mCurrentWidth/2, mCurrentHeight/2, 0, 0};
  RECT Rect = {0, mCurrentHeight/2, mCurrentWidth, mCurrentHeight};
  mFontBig->DrawText(0, s.c_str(), -1, &Rect, DT_NOCLIP | DT_CENTER , aCol);	  
}

void GameBpApp::drawText ( std::string& aString, const int aHeightDivider, const D3DXCOLOR aCol ) const
{
  std::wstring s = WinIO::s2ws( aString );    
  //RECT Rect = {mCurrentWidth/2, mCurrentHeight/2, 0, 0};
  RECT Rect = {0, mCurrentHeight/aHeightDivider, mCurrentWidth, mCurrentHeight};
  mFontBig->DrawText(0, s.c_str(), -1, &Rect, DT_NOCLIP | DT_CENTER , aCol);	  
}

void GameBpApp::drawMenuItem ( std::string& aString, 
                              bool aHighlighted, 
                              int aIndex, 
                              bool aCentered,
                              const bool aMenuWidget ) const
{
  std::wstring s = WinIO::s2ws( aString );
    
  int xpos;
  int ypos;
  if ( aCentered )
  {    
    xpos = 200;
    ypos = 200;
  }
  else
  {    
    xpos = 5;
    ypos = 0;
  }

  if ( aMenuWidget )
    xpos += 250;
    
  RECT Rect = {xpos, 25*aIndex + ypos, 0, 0};
  D3DXCOLOR col;
  if ( aHighlighted )
    col = WHITE;
  else
    col = GREY;

  mFont->DrawText(0, s.c_str(), -1, &Rect, DT_NOCLIP, col);	  
}

void GameBpApp::initGameWithUserOptions()
{
  //int difficulty = mMenu[eMenuDifficulty].index();
  int difficulty = 0;
  switch (mUserMode)
  {
  case 0:
    mGame   = Game::gameChrono( difficulty, mUserTrack, mUserBall, this );
    break;
  case 1:
    mGame   = Game::gameLab( difficulty, mUserTrack, mUserBall, this );
    break;
  case 2:
    mGame   = Game::gameChase( difficulty, mUserTrack, mUserBall, this );
    break;
  case 3:
    mGame   = Game::gameSpartacus( difficulty, mUserTrack, mUserBall, this );
    break;
  };
  
  mGameInitialized = false;
}

void GameBpApp::displayInGameMenu() const
{
  for ( int i = 0; i < mInGameMenu->mItems.size(); ++i)
    drawMenuItem( mInGameMenu->mItems[i], mInGameMenu->index() == i, i, true );
  
}

void GameBpApp::displayGameOverMenu() const
{
  for ( int i = 0; i < mGameOverMenu->mItems.size(); ++i)
    drawMenuItem( mGameOverMenu->mItems[i], mGameOverMenu->index() == i, i, true );
  
}

void GameBpApp::inGameMenuRestore()
{
  mIoMenu.Move(K_NON_EXISTANT);
  mInGameMenu->setAction(Menu::eNoAction);
  //startTimer();
  mGameInCourse = true;
}

void GameBpApp::inGameMenuReset()
{  
  mIoMenu.Move(K_NON_EXISTANT);
  mInGameMenu->setAction(Menu::eNoAction);
  //startTimer();
  mGame->initGameState();  
  mGameInCourse = true;
}

void GameBpApp::inGameMenuShow()
{
  mGameInCourse = false;
  keybCleanAll();  
}

void GameBpApp::_storeFps( const int aFps )
{
  mGame->mFpsCounting.count( aFps );
}