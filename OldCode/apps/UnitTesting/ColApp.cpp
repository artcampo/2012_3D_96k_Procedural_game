#include "ColApp.hpp"


ColApp::ColApp(HINSTANCE& handler, int width, int height, bool isFullScreen, 
               std::wstring wTitle, bool showFPS): 
               BaseDX(hInstance, width, height, isFullScreen, wTitle, showFPS)
{

  initColours(); 
}

ColApp::~ColApp()
{
  debugEnd();
}

Mesh*  ColApp::loadMesh ( const char* aFileName )
{
  Mesh* mesh = new Mesh();
  //ifstream fileStd ("./piscina.nbo", fstream::in| fstream::binary );
  ifstream fileStd ( aFileName, fstream::in );  

  if ( fileStd.good() )
  {  
    mesh->load( fileStd );
    fileStd.close();
    return mesh;
  }
  else 
    return NULL;
}

bool ColApp::init()
{
	if (!BaseDX::init()) return false;

  initShip();
  mBox.init();
  debugStart();


  mMeshTest.resize(1);
  mMeshTest[0] =  loadMesh( "./pool2.nbo" );

  mMeshModel = mMeshTest[0]->convert( Global::md3dDevice, 
                        D3DXVECTOR3( 10,0,0),
                        &mColours );

  //GlobalGame::gPhysic->createStaticRigidBody( mMeshTest[0] );
 
  mTestTrack = new TrackPath( &Spline0PATH[0], 16,
                              &Spline0SLICE[0], 28,
                              D3DXVECTOR3(1,0,0), D3DXVECTOR3(1,0,0), D3DXVECTOR3(0,1,0), D3DXVECTOR3(0,1,0) );
  




  VertPNT v;
  v.nor = D3DXVECTOR3(0,1,0);
  v.tan = D3DXVECTOR3(1,0,0);

  std::vector<VertPNT> vert;
  std::vector<D3DXVECTOR3> vertOnlyV;
  std::vector<D3DXVECTOR3> vertOnlyV1;
  float a = 2.0f;
  v.pos = D3DXVECTOR3(-a,a,0); vert.push_back(v); vertOnlyV.push_back (v.pos);
  v.pos = D3DXVECTOR3(+a,-a,-a); vert.push_back(v); vertOnlyV.push_back (v.pos);
  v.pos = D3DXVECTOR3(+a,-a,+a); vert.push_back(v); vertOnlyV.push_back (v.pos);

  v.pos = D3DXVECTOR3(+a,-a,+a); vert.push_back(v); vertOnlyV1.push_back (v.pos);
  v.pos = D3DXVECTOR3(+a,-a,-a); vert.push_back(v); vertOnlyV1.push_back (v.pos);
  v.pos = D3DXVECTOR3(3.0f*a,a,0); vert.push_back(v); vertOnlyV1.push_back (v.pos);

  DxMeshModel<VertPNT>* t;
  t = new DxMeshModel<VertPNT>( sizeof(VertPNT), vert.size(), &vert[0] );
  mTestTrackModel = new DxMeshModelBatch<VertPNT>( sizeof(VertPNT), 
                                                           D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  mTestTrackModel->addMeshModel( t );
  mTestTrackModel->createVB();

  mTestCol = new MeshCol( vertOnlyV );
  mTestCol1 = new MeshCol( vertOnlyV1 );
  
  GlobalGame::gPhysic->createStaticRigidBody( mTestCol );
  GlobalGame::gPhysic->createStaticRigidBody( mTestCol1 );

  GlobalGame::gCam->setPositionAndAt(
    //&D3DXVECTOR3(1,1,-3), 
    &D3DXVECTOR3(0,1,-3),
    &D3DXVECTOR3(0,0,0) );  

  return true;

}

void  ColApp::initShip()
{
//////////////////////////////////////////////////////////
  //    Init Ship and its forces
  //GlobalGame::gShip = GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( -150.0f, 250.0f, -150.0f), 15.0f );
  GlobalGame::gShip = GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( 0, 0.1, 0), 1.0f );
  

  GlobalGame::gShipTForceForw =
    GlobalGame::gShip->attachTimeForce( &D3DXVECTOR3( 1.0f, 0.0f, 0.0f), 
                                        &gShipTForceFwdSpline, GlobalGame::mFwdModule );

  GlobalGame::gShipTForceBack =
    GlobalGame::gShip->attachTimeForce( &D3DXVECTOR3( -1.0f, 0.0f, 0.0f), 
                                        &gShipTForceFwdSpline, GlobalGame::mBckModule );
  
  GlobalGame::gShipTForceLeft = 
    GlobalGame::gShip->attachTimeForce( &D3DXVECTOR3( 0.0f, 0.0f, 1.0f), 
                                        &gShipTForceFwdSpline, GlobalGame::mSidModule );

  GlobalGame::gShipTForceRight =
    GlobalGame::gShip->attachTimeForce( &D3DXVECTOR3( 0.0f, 0.0f, -1.0f), 
                                        &gShipTForceFwdSpline, GlobalGame::mSidModule  );

  //////////////////////////////////////////////////////////
  //    Init Ship Torques
  GlobalGame::gShipTTorqueUp  = 
    GlobalGame::gShip->attachTimeTorque( &D3DXVECTOR3( 1.0f, 0.0f, 0.0f), &D3DXVECTOR3( 0.0f, 1.0f, 0.0f) );

  GlobalGame::gShipTTorqueDown  = 
    GlobalGame::gShip->attachTimeTorque( &D3DXVECTOR3( -1.0f, 0.0f, 0.0f), &D3DXVECTOR3( 0.0f, 1.0f, 0.0f) );

  GlobalGame::gShipTTorqueLeft  =
    GlobalGame::gShip->attachTimeTorque( &D3DXVECTOR3( 0.0f, 0.0f, -1.0f), &D3DXVECTOR3( 0.0f, 1.0f, 0.0f) );    

  GlobalGame::gShipTTorqueRight  = 
    GlobalGame::gShip->attachTimeTorque( &D3DXVECTOR3( 0.0f, 0.0f, 1.0f), &D3DXVECTOR3( 0.0f, 1.0f, 0.0f) );


  GlobalGame::gShip->computeTemporalValues();
  GlobalGame::gCam3dPerson->updateTargetParameters( GlobalGame::gShip->getxWS(), GlobalGame::gShip->getvWS() );  
  GlobalGame::gCam1stPerson->updateTargetParameters( &GlobalGame::gShip->qWS, &GlobalGame::gShip->xWS );  
}

void ColApp::onResize()
{
	BaseDX::onResize();
}

void ColApp::drawScene()
{
	BaseDX::drawScene();
  
  GlobalGame::gShipPosition = GlobalGame::gShip->getxWS();
/*
  GlobalGame::gCam->setPositionAndAt( 
    &D3DXVECTOR3(       10.0f,
                        10.0f,
                        10.0f
                        ) ,
    &D3DXVECTOR3(       00.0f,
                        00.0f,
                        0.0f
                        ) );    
  */
  // Draw Terrain
  Global::gShaderTable[indexShadDefGeomV].SetMatrix( &(std::string("World")), &Global::mWorld );
  Global::gShaderTable[indexShadDefGeomV].SetMatrix( &(std::string("View")), &Global::mView );
  Global::gShaderTable[indexShadDefGeomV].SetMatrix( &(std::string("Projection")), &Global::mProjection );

  
  Global::md3dDevice->IASetInputLayout( Global::gShaderTable[indexShadDefGeomV].mVertexLayout );
  //Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  


  ID3D10EffectPass* pass = Global::gShaderTable[indexShadDefGeomV].mTech->GetPassByIndex(0);
  pass->Apply(0);

  //GlobalGame::gDelaunayMesh->DisplaySansShader();


  //display particles
  for ( int i = 0; i < GlobalGame::gPhysic->getNumBodies() ; ++i)
  {
    RigidBody* body = GlobalGame::gPhysic->getIthBody( i );
    D3DXMatrixTranslation(  &Global::mWorld, 
                            body->xWS.x,
                            body->xWS.y,
                            body->xWS.z);

    D3DXMATRIX matrixRotation;

    D3DXMatrixRotationQuaternion( &matrixRotation, &body->qWS );

    Global::mWorld = matrixRotation*Global::mWorld;
    D3DXMATRIX m;
    D3DXMatrixScaling(&m, 0.01, 0.01, 0.01);
    Global::mWorld = m*Global::mWorld;
    //Global::mWorld = Global::mWorld*matrixRotation;

    Global::gShaderTable[indexShadDefGeomV].SetMatrix( &(std::string("World")), &Global::mWorld );
    pass->Apply(0);

    mBox.display();
  }



  /////////////////////////////
  //  draw static
  D3DXMatrixIdentity  ( &Global::mWorld );  
  
  Global::gShaderTable[indexShadDefGeomV_PN].SetMatrix( &(std::string("World")), &Global::mWorld );
  Global::gShaderTable[indexShadDefGeomV_PN].SetMatrix( &(std::string("View")), &Global::mView );
  Global::gShaderTable[indexShadDefGeomV_PN].SetMatrix( &(std::string("Projection")), &Global::mProjection );

  
  Global::md3dDevice->IASetInputLayout( Global::gShaderTable[indexShadDefGeomV_PN].mVertexLayout );
  //Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  



  pass = Global::gShaderTable[indexShadDefGeomV_PN].mTech->GetPassByIndex(0);
  pass->Apply(0);
  mTestTrackModel->DisplaySansShader();

  
  int steps = 2;
  for ( int i = 0; i < steps; ++i)
    GlobalGame::gPhysic->advanceSimulation( 0.01f/float(steps) );


 

/*
  GlobalGame::gCam1stPerson->updateTargetParameters( &GlobalGame::gShip->qWS, &GlobalGame::gShip->xWS );  
  GlobalGame::gCam1stPerson->updateCamera();
  */
  
  drawText();

}


void ColApp::initColours()
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

  /*
  mColours.fac      = D3DXCOLOR( 0.2f, 0.2f, 0.8f, 1.0f );
  mColours.facSel   = D3DXCOLOR( 0.8f, 0.8f, 0.0f, 1.0f );
  mColours.facHigh  = D3DXCOLOR( 0.6f, 0.6f, 0.6f, 1.0f );
  */

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


void  ColApp::debugStart()
{

  Global::mFileDebug = new std::ofstream ( "physics_log.txt", std::fstream::out );  
  Global::mFileDebug->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebug->precision(8);

  Global::mFileDebug2 = new std::ofstream ( "physics_log2.txt", std::fstream::out );  
  Global::mFileDebug2->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebug2->precision(8);

  Global::mFileDebug3 = new std::ofstream ( "physics_log3.txt", std::fstream::out );  
  Global::mFileDebug3->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebug3->precision(8);

  Global::mFileDebug4 = new std::ofstream ( "physics_log4.txt", std::fstream::out );  
  Global::mFileDebug4->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebug4->precision(8);


  Global::mFileDebug5 = new std::ofstream ( "physics_time.txt", std::fstream::out );  
  Global::mFileDebug5->setf(std::ios::fixed,std::ios::floatfield); 
  Global::mFileDebug5->precision(8);
}

void  ColApp::debugEnd()
{
  Global::mFileDebug->flush();
  Global::mFileDebug->close();
  Global::mFileDebug2->flush();
  Global::mFileDebug2->close();
  Global::mFileDebug3->flush();
  Global::mFileDebug3->close();
  Global::mFileDebug4->flush();
  Global::mFileDebug4->close();
  Global::mFileDebug5->flush();
  Global::mFileDebug5->close();
}