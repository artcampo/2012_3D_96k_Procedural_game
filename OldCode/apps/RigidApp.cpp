#include "RigidApp.hpp"


RigidApp::RigidApp(HINSTANCE& handler, int width, int height, bool isFullScreen, 
               std::wstring wTitle, bool showFPS): 
               BaseDX(hInstance, width, height, isFullScreen, wTitle, showFPS)
{

  initColours();  
}

RigidApp::~RigidApp()
{

}

Mesh*  RigidApp::loadMesh ( const char* aFileName )
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

bool RigidApp::init()
{
	if (!BaseDX::init()) return false;


  initShip();
  mBox.init();



      /*
  GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( 1000.0f, 0.0f, 0.0f), 15.0f );
  GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( 350.0f, 0.0f, 0.0f), 15.0f );
  GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( 450.0f, 0.0f, 0.0f), 15.0f );
  GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( 250.0f, 0.0f, 40.0f), 15.0f );
  GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( 250.0f, 0.0f, 80.0f), 15.0f );
  GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( 250.0f, 0.0f, 120.0f), 15.0f );
  GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( 250.0f, 0.0f, -40.0f), 15.0f );
*/

mMeshTest.resize(1);
  mMeshTest[0] =  loadMesh( "./pool2.nbo" );

  mMeshModel = mMeshTest[0]->convert( Global::md3dDevice, 
                        D3DXVECTOR3( 10,0,0),
                        &mColours );

  GlobalGame::gPhysic->createStaticRigidBody( mMeshTest[0] );
  /*
  mMeshTest.resize(1);
  mMeshTest[0] =  loadMesh( "./plano.nbo" );

  mMeshModel = mMeshTest[0]->convert( Global::md3dDevice, 
                        D3DXVECTOR3( 10,0,0),
                        &mColours );

  GlobalGame::gPhysic->createStaticRigidBody( mMeshTest[0] );
*/
   /*
  mMeshTest.resize(2);
  mMeshTest[0] =  loadMesh( "./piscina.nbo" );
  mMeshTest[1] =  loadMesh( "./lapiz.nbo" );
  //mMeshModel[0] 


  mMeshModel.resize(2);
  mMeshModel[0] = mMeshTest[0]->convert( Global::md3dDevice, 
                        D3DXVECTOR3( 10,0,0),
                        &mColours )[0];
  mMeshModel[1] = mMeshTest[1]->convert( Global::md3dDevice, 
                        D3DXVECTOR3( 10,0,0),
                        &mColours )[0];

  GlobalGame::gPhysic->createStaticRigidBody( mMeshTest[0] );
  GlobalGame::gPhysic->createStaticRigidBody( mMeshTest[1] );
*/
  return true;

}

void  RigidApp::initShip()
{
//////////////////////////////////////////////////////////
  //    Init Ship and its forces
  //GlobalGame::gShip = GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( -150.0f, 250.0f, -150.0f), 15.0f );
  GlobalGame::gShip = GlobalGame::gPhysic->createRigidBody( D3DXVECTOR3( 0, 10.0f, 2), 1.0f );
  

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

void RigidApp::onResize()
{
	BaseDX::onResize();
}

void RigidApp::drawScene()
{
	BaseDX::drawScene();
  
  //GlobalGame::gCam3dPerson->updateCamera();

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

  for ( int i = 0 ; i< mMeshModel.size() ; ++i )
    mMeshModel[i]->displayTriangles();
  
  int steps = 2;
  for ( int i = 0; i < steps; ++i)
    GlobalGame::gPhysic->advanceSimulation( 0.01f/float(steps) );

  //GlobalGame::gCam3dPerson->updateTargetParameters(  GlobalGame::gPhysic->getIthBody(

  //GlobalGame::gCam1stPerson->updateTargetParameters( &GlobalGame::gShip->qWS, &GlobalGame::gShip->xWS );  
  //GlobalGame::gCam1stPerson->updateCamera();

  

}


void RigidApp::initColours()
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