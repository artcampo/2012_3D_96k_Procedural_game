#include  "GameBall/Game.hpp"
#include  "GameBall/Modes/GameChrono.hpp"
#include  "GameBall/Modes/GameLab.hpp"
#include  "GameBall/Modes/GameChase.hpp"
#include  "GameBall/Modes/GameSpartacus.hpp"

#include  "Project_Dependant/IOinput.hpp"
#include  "GameBall/Entities/Arch.hpp"

#include  "Misc/CountMeanMax.hpp"
#include  "Geometry/DxFustrum.hpp"


#include  "RenderModels/Metropolis/dx_metarm_model.hpp"
#include  "GameBall/Metropolis/MetropolisStructure.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"
#include  "GameBall/Tricks/Tricks.hpp"
#include  "GameBall/Entities/Colliders/RectangleCollider.hpp"
#include  "GameBall/Recording/GameRecording.hpp"
#include  "GameBall/Recording/GameReplay.hpp"

void Game::drawScene( DxFustrum& aFustrum, const bool aSetCamera )
{
  mProfLoop->start( std::string("Render") );
#ifndef GBP_RENDER_TRACK_ON_LINES
  //Set camera
  _setCamera();
/*
#ifdef DISABLE_CAM
      mCam->setPositionAndAt(      
  &D3DXVECTOR3(0.0f, 3.0f, 35.0f),    
  &D3DXVECTOR3(0, 0, 40) ); 
#endif*/

  ID3D10EffectPass* pass;
#ifndef GBP_DISABLE_FOG_DISPLAY
  mFog->display();
#endif
 
  
  Global::mBallPosition = mBall->mBallBody->getxWS();
  Global::mBallVel      = mBall->mBallBody->getvWS();

  // Draw end of track (debug)
  mShaderManager.mShaderTable[indexShadDefGeomV].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  mShaderManager.mShaderTable[indexShadDefGeomV].SetResource( &(std::string("PermTexture")), mPermTextureSRV );
  mShaderManager.mShaderTable[indexShadDefGeomV].SetResource( &(std::string("textWorley")), 
    mShaderManager.mWorleyTextureSRV);
  
  {
    Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV].mVertexLayout );  
    Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );  
    pass = mShaderManager.mShaderTable[indexShadDefGeomV].mTech->GetPassByIndex(0);
    pass->Apply(0);

    Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

#ifndef GBP_DISABLE_RENDER_END_OF_TRACK_RECTANGLE
    mEnd->mCollider->mMesh->DisplaySansShader();
#endif
  }

#ifndef GBP_DISABLE_RENDER_COLLIDERS_RECT
  /*
  for ( int i = 0; i < mArchs.size() ; ++i)  
    mArchs[i]->mCollider->mMesh->DisplaySansShader();
*/
  for ( int i = 0; i < mBumpers.size() ; ++i)  
  {
    mBumpers[i]->mColliderOnTop->mMesh->DisplaySansShader();
    //mBumpers[i]->mColliderAtLeft->mMesh->DisplaySansShader();
    //mBumpers[i]->mColliderAtRight->mMesh->DisplaySansShader();
  }
#endif
  
#ifndef _GBP_DISABLE_METROPOLIS_RENDER
  {
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_MetCube].SetMatrix( &(std::string("World")), &Global::mWorld );
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_MetCube].SetMatrix( &(std::string("View")), &Global::mView );
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_MetCube].SetMatrix( &(std::string("Projection")), &Global::mProjection );
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_MetCube].SetResource( &(std::string("PermTexture")), mPermTextureSRV ); 
    Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNT_MetCube].mVertexLayout );  
    Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );  
    pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNT_MetCube].mTech->GetPassByIndex(0);
    pass->Apply(0);
    mMetropolis->display( aFustrum );
  }
#endif

  //  _drawScene(); can change render state
  _drawScene();
  //GlobalGame::gDelaunayMesh->DisplaySansShader();
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );


  //display balls
  _drawBalls();

  

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV].mVertexLayout );  
  pass = mShaderManager.mShaderTable[indexShadDefGeomV].mTech->GetPassByIndex(0);


  {
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Arch].SetMatrix( &(std::string("World")), &Global::mWorld );
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Arch].SetMatrix( &(std::string("View")), &Global::mView );
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Arch].SetMatrix( &(std::string("Projection")), &Global::mProjection );
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Arch].SetResource( &(std::string("PermTexture")), mPermTextureSRV ); 
    Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Arch].mVertexLayout );  
    Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );  
    pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Arch].mTech->GetPassByIndex(0);
    pass->Apply(0);    
  }
  
  for ( int i = 0; i < mArchs.size() ; ++i)  
  {
    D3DXMATRIX basisRender = mArchs[i]->basisRender();
    D3DXMATRIX m; D3DXMatrixRotationY( &m, D3DX_PI*0.5f );
    basisRender = m*basisRender;
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Arch].SetMatrix( &(std::string("World")), &basisRender );
    pass->Apply(0);    
    for ( int j = 0; j < mArchModel.size() ; ++j)  
      mArchModel[j]->displayTriangles();    
  }
  
  {
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Bumper].SetMatrix( &(std::string("World")), &Global::mWorld );
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Bumper].SetMatrix( &(std::string("View")), &Global::mView );
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Bumper].SetMatrix( &(std::string("Projection")), &Global::mProjection );
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Bumper].SetResource( &(std::string("PermTexture")), mPermTextureSRV ); 
    Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Bumper].mVertexLayout );  
    Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );  
    pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Bumper].mTech->GetPassByIndex(0);
    pass->Apply(0);    
  }
  for ( int i = 0; i < mBumpers.size() ; ++i) 
  {
    mShaderManager.mShaderTable[indexShadDefGeomV_PNT_Bumper].SetMatrix( &(std::string("World")), &mBumpers[i]->basisRender() );
    pass->Apply(0);
    
    mBumperCubeModel[0]->displayTriangles();    
  }
    
    

  /////////////////////////////
  //  draw static
  D3DXMatrixIdentity  ( &Global::mWorld );  
  

  //DEBUG
    //  Display lines
#ifdef GBP_RENDER_WIREFRAME
    Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP );
#endif
  //-DEBUG
#ifndef GBP_RENDER_WIREFRAME
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
#endif
  

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNTUV_track].mVertexLayout );

  mShaderManager.mShaderTable[indexShadDefGeomV_PNTUV_track].SetRawValue ( &(std::string("camPos")), (float*)&Global::mEyePos, 3 );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNTUV_track].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNTUV_track].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNTUV_track].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNTUV_track].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  D3DXMATRIX matrixWVP = Global::mWorld * Global::mView * Global::mProjection;

  pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNTUV_track].mTech->GetPassByIndex(0);
  pass->Apply(0);


  D3DXMATRIX viewFustrum; 
  mCam->updateCameraForFustrum( &viewFustrum, 30 );

  D3DXMATRIX matrixWVPfustrum = viewFustrum * Global::mProjection;
  aFustrum = DxFustrum ( matrixWVPfustrum ) ;

#ifndef GBP_GFXDEBUG_TRACK
  for ( int i = 0; i < mTrackModels.size() ; ++i)    
    mTrackModels[i]->display( aFustrum );
#endif
#ifdef GBP_GFXDEBUG_TRACK
  if (Global::mShowIterTrack < 0 ) Global::mShowIterTrack = 0;
  if (Global::mShowIterTrack > (mTrackDesc.iterations - 1) ) Global::mShowIterTrack = (mTrackDesc.iterations - 1);

	for ( int i = 0; i < mTrackModels[Global::mShowIterTrack].size() ; ++i)    
    mTrackModels[Global::mShowIterTrack][i]->display( aFustrum );
#endif


  // Draw Terrain - lines
  
  mShaderManager.mShaderTable[indexShadDefGeomV_PC].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_PC].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_PC].SetMatrix( &(std::string("Projection")), &Global::mProjection );

  
  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PC].mVertexLayout );  
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );
  
  
  pass = mShaderManager.mShaderTable[indexShadDefGeomV_PC].mTech->GetPassByIndex(0);
  pass->Apply(0);
#ifndef GBP_GFXDEBUG_TRACK
  mTrackLinesModel->display();
#endif
#ifdef GBP_GFXDEBUG_TRACK
  mTrackLinesModel[Global::mShowIterTrack]->display();
#endif
  
  mEntitiesLinesModel->display();
  
#endif
  
  mProfLoop->end();

}

void Game::drawPreview( const float aCompletition )
{

  
#ifndef GBP_GFXDEBUG_TRACK
  if ( mTrack->mTrackPreview.size() == 0 )
    return;
  int index = int( aCompletition*(mTrack->mTrackPreview.size() - 1) );  
#endif
#ifdef GBP_GFXDEBUG_TRACK
if ( mTrack[0]->mTrackPreview.size() == 0 )
    return;
  int index = int( aCompletition*(mTrack[0]->mTrackPreview.size() - 1) ); 
#endif 

 //Set camera
  D3DXMatrixIdentity  ( &Global::mWorld );
  D3DXVECTOR3 at  = D3DXVECTOR3(mTrackDesc.totalLength*0.5,0,0);  
  float l = mTrackDesc.totalLength*0.7f;
  D3DXVECTOR3 pos = at + D3DXVECTOR3(l,0.25f*l,0);
  //mCam->setPositionAndAt( &pos, &at );
  mCam->updateCamera   ( &Global::mView );
  


  // Draw Terrain
  ID3D10EffectPass* pass;

  /////////////////////////////
  //  draw static
  D3DXMatrixIdentity  ( &Global::mWorld );  
 
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP );
  // Draw Terrain - lines
  mShaderManager.mShaderTable[indexShadDefGeomV_PC].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_PC].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_PC].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  
  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PC].mVertexLayout );  
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );
    
  pass = mShaderManager.mShaderTable[indexShadDefGeomV_PC].mTech->GetPassByIndex(0);
  pass->Apply(0);

#ifndef GBP_GFXDEBUG_TRACK
  mTrack->mTrackPreview[index]->display();      
#endif
#ifdef GBP_GFXDEBUG_TRACK
  mTrack[0]->mTrackPreview[index]->display();
#endif 
  

}




void Game::update( const double aLapsedTime )
{  
#ifdef GBP_GFXDEBUG_TRACK
  return;
#endif  
  mLapsedTime += aLapsedTime;
  Global::mGameLapsedTime = mLapsedTime;
  updateMessages(aLapsedTime);

  mRecording->store( mLapsedTime );

  /////////////////////////////////////////////////////////
  //    Move IA
  mProfLoop->start( std::string("IA") );
  _IAstep(aLapsedTime);
  mProfLoop->end();

  /////////////////////////////////////////////////////////
  //    Advance Physics Engine

  double targetTime   = aLapsedTime*0.003;  
  double stillToAdvancedTime = targetTime;
   
  
  *Global::mFileDebugTime << "t:    "<< " ["<< mPhysics->mDebugFrame <<"]: "<< targetTime<<std::endl;


  *Global::mFileDebugPhysGen << "------------------------------------------------------------------------"<<std::endl;
  *Global::mFileDebugPhysGen << "Starting frame"<< " ["<< mPhysics->mDebugFrame <<"] targetTime: "<< targetTime<<std::endl;

  mProfLoop->start( std::string("Simulate Step") );
  mPhysics->perFrameCountersReset();
  double clocksA = mLog->mProf->ReadTSC();

  std::vector<bool> dynBodyHadContact ( mPhysics->getNumBodies() );
  for ( int i = 0; i < dynBodyHadContact.size(); ++i)
    dynBodyHadContact[i] = false;

  while ( !DxMath::equality( stillToAdvancedTime, 0.0f, 0.000001f ) )
  {
    std::vector<bool> dynBodyHadContactCurrent;
    if ( stillToAdvancedTime < mLoaded_maxStepTime )
    {
      dynBodyHadContactCurrent = mPhysics->advanceSimulation( stillToAdvancedTime );
      stillToAdvancedTime = 0.0f;
    }
    else
    {
      dynBodyHadContactCurrent = mPhysics->advanceSimulation( mLoaded_maxStepTime );
      stillToAdvancedTime -= mLoaded_maxStepTime;
    }
    for ( int i = 0; i < dynBodyHadContact.size(); ++i)
      dynBodyHadContact[i] = dynBodyHadContact[i] | dynBodyHadContactCurrent[i];
  }
  double clocksB = mLog->mProf->ReadTSC();
  mProfLoop->end();
  mPhysics->perFrameCountersPrint();
  *Global::mFileDebugTimeProf << "["<< mPhysics->mDebugFrame <<"]: "
    << (clocksB-clocksA)*0.0000001<<" Mega-Clocks "<<std::endl;
  

  mProfLoop->start( std::string("game logic") );
  //  Update camera
  _updateCamera(aLapsedTime);    
  _update(aLapsedTime);  

  
  /////////////////////////////////////////////////////////
  //    Update entities
  mTrickManager->resetFrameInfo();

  for ( int i = 0; i < mArchs.size() ; ++i)  
    mArchs[i]->update( float(aLapsedTime) ); 
      
  for ( int i = 0; i < mBumpers.size() ; ++i)  
    mBumpers[i]->update( float(aLapsedTime) ); 
  
  
  for ( int balli = 0; balli < mBalls.size() ; ++balli)  
  {
    mBalls[balli]->update( float(aLapsedTime), mTrack );

    for ( int i = 0; i < mArchs.size() ; ++i)  
      mArchs[i]->cross( *mBalls[balli] );

    for ( int i = 0; i < mBumpers.size() ; ++i)  
      mBumpers[i]->cross( *mBalls[balli], mTrickManager );
  }
  
  const TrackEdge* edgePath = mBall->nearestEdgePath( mTrack );
  mTrickManager->update(  aLapsedTime, 
                          mLapsedTime,
                          mBall->mBallBody->isInRestingContact(), 
                          dynBodyHadContact[0],
                          mNitroUsed,
                          mBall->nearestTrackPath( mTrack ),
                          edgePath,
                          mBall->distanceToEdgePath(edgePath),
                          mTrack->mDesc.trackWidth,
                          *mBall);
  Global::mPunctuation = mTrickManager->punctuation();
  
  mInvertCommandsTimed.update( float(aLapsedTime) );

  ////////////////////////////////////
  //  Check out of track
  if ( mBall->mBallBody->isInRestingContact() )
    mLapsedTimeSinceInContact = 0.0f;
  else
  {
    mLapsedTimeSinceInContact += float(aLapsedTime);
    
    if ( mLapsedTimeSinceInContact > (26.0f*100.0f) )
    {
      *mLog->mFileEvents << "Restore from check point"<< " [" << mPhysics->mDebugFrame << "]" << std::endl;
      actionRestoreCheckPoint( *mBall );
    }
  }
 
  ////////////////////////////////////
  //  end of game
  mEnd->cross( mBall->mBallBody->getxWS(), *mBall );
  mEnd->update( aLapsedTime );
 
  _checkEndGame( aLapsedTime );
  mProfLoop->end();

  mProfLoop->addTag( mPhysics->mDebugFrame );
}




std::string   Game::getComboMessage() const
{
  return mTrickManager->displayText(); 
}

std::string   Game::getMainMessage() const
{
  std::string s = _getMainMessage();

  for ( int i = 0; i < mToUserMessages.size() ; ++i) 
  {
    s += mToUserMessages[i]->mMessage;
    s += std::string("\n");
  }

  if ( isGoingBackwards() )
    s += std::string("\n Backwards!");

  return s;  
}

void Game::drawBalls() const
{
  for ( int i = 0; i < mBalls.size() ; ++i) 
    mBalls[i]->display();    
}



void Game::updateMessages( const double aLapsedTime )
{
  for ( int i = mToUserMessages.size() - 1; i >= 0  ; --i) 
  {
    mToUserMessages[i]->mTimeLimit->update( aLapsedTime );

    if ( !mToUserMessages[i]->mTimeLimit->acting() )    
      mToUserMessages.erase (mToUserMessages.begin() + i );          
  }
}

void Game::addMessage( const float aTimeSeconds, const std::string& aMessage )
{
  mToUserMessages.push_back( new TimedMessage( aTimeSeconds, aMessage ) );
}

TrackBp::TrackDescription Game::getTrackDesc() const
{
  return mTrackDesc;
}

void Game::playRecording( const double aLapsedTime )
{
  mReplay->play(  aLapsedTime );
}