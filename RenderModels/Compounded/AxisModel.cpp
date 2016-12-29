#include "AxisModel.hpp"

AxisModel::AxisModel( std::vector<D3DXVECTOR3>& mVertices )
{
  {
    DxMeshModel<VertexPosNorCol>* mesh;
    VertexPosNorCol v[2];
    v[0].pos = mVertices[ 0 + 0];
    v[1].pos = mVertices[ 0 + 1];
    v[0].nor = D3DXVECTOR3(0,1,0);
    v[1].nor = D3DXVECTOR3(0,1,0);
    v[0].col = D3DXCOLOR(1,0,0,1);
    v[1].col = D3DXCOLOR(1,0,0,1);
    mesh = new DxMeshModel<VertexPosNorCol>( sizeof(VertexPosNorCol), 2, v );
    mMeshX = new DxMeshModelBatch<VertexPosNorCol>( sizeof(VertexPosNorCol), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
    mMeshX->addMeshModel( mesh );
    mMeshX->createVB( );
  }

  {
    DxMeshModel<VertexPosNorCol>* mesh;
    VertexPosNorCol v[2];
    v[0].pos = mVertices[ 2 + 0];
    v[1].pos = mVertices[ 2 + 1];
    v[0].nor = D3DXVECTOR3(0,1,0);
    v[1].nor = D3DXVECTOR3(0,1,0);
    v[0].col = D3DXCOLOR(0,1,0,1);
    v[1].col = D3DXCOLOR(0,1,0,1);
    mesh = new DxMeshModel<VertexPosNorCol>( sizeof(VertexPosNorCol), 2, v );
    mMeshY = new DxMeshModelBatch<VertexPosNorCol>( sizeof(VertexPosNorCol), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
    mMeshY->addMeshModel( mesh );
    mMeshY->createVB( );
  }

  {
    DxMeshModel<VertexPosNorCol>* mesh;
    VertexPosNorCol v[2];
    v[0].pos = mVertices[ 4 + 0];
    v[1].pos = mVertices[ 4 + 1];
    v[0].nor = D3DXVECTOR3(0,1,0);
    v[1].nor = D3DXVECTOR3(0,1,0);
    v[0].col = D3DXCOLOR(0,0,1,1);
    v[1].col = D3DXCOLOR(0,0,1,1);
    mesh = new DxMeshModel<VertexPosNorCol>( sizeof(VertexPosNorCol), 2, v );
    mMeshZ = new DxMeshModelBatch<VertexPosNorCol>( sizeof(VertexPosNorCol), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
    mMeshZ->addMeshModel( mesh );
    mMeshZ->createVB( );
  }
}

AxisModel::~AxisModel()
{
  delete mMeshX;
  delete mMeshY;
  delete mMeshZ;
}

void AxisModel::display()
{
  displayX();
  displayY();
  displayZ();
}

void AxisModel::displayX()
{
  mMeshX->DisplaySansShader();
}

void AxisModel::displayY()
{
  mMeshY->DisplaySansShader();
}

void AxisModel::displayZ()
{
  mMeshZ->DisplaySansShader();
}

D3DXCOLOR AxisModel::getColourX()
{
  return D3DXCOLOR(1,0,0,1);
}

D3DXCOLOR AxisModel::getColourY()
{
  return D3DXCOLOR(0,1,0,1);
}

D3DXCOLOR AxisModel::getColourZ()
{
  return D3DXCOLOR(0,0,1,1);
}


AxisModelPC::AxisModelPC( std::vector<D3DXVECTOR3>& mVertices )
{
  {
    DxMeshModel<VertexPC>* mesh;
    VertexPC v[2];
    v[0].pos = mVertices[ 0 + 0];
    v[1].pos = mVertices[ 0 + 1];
    v[0].col = D3DXCOLOR(1,0,0,1);
    v[1].col = D3DXCOLOR(1,0,0,1);
    mesh = new DxMeshModel<VertexPC>( sizeof(VertexPC), 2, v );
    mMeshX = new DxMeshModelBatch<VertexPC>( sizeof(VertexPC), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
    mMeshX->addMeshModel( mesh );
    mMeshX->createVB( );
  }

  {
    DxMeshModel<VertexPC>* mesh;
    VertexPC v[2];
    v[0].pos = mVertices[ 2 + 0];
    v[1].pos = mVertices[ 2 + 1];
    v[0].col = D3DXCOLOR(0,1,0,1);
    v[1].col = D3DXCOLOR(0,1,0,1);
    mesh = new DxMeshModel<VertexPC>( sizeof(VertexPC), 2, v );
    mMeshY = new DxMeshModelBatch<VertexPC>( sizeof(VertexPC), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
    mMeshY->addMeshModel( mesh );
    mMeshY->createVB( );
  }

  {
    DxMeshModel<VertexPC>* mesh;
    VertexPC v[2];
    v[0].pos = mVertices[ 4 + 0];
    v[1].pos = mVertices[ 4 + 1];
    v[0].col = D3DXCOLOR(0,0,1,1);
    v[1].col = D3DXCOLOR(0,0,1,1);
    mesh = new DxMeshModel<VertexPC>( sizeof(VertexPC), 2, v );
    mMeshZ = new DxMeshModelBatch<VertexPC>( sizeof(VertexPC), D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
    mMeshZ->addMeshModel( mesh );
    mMeshZ->createVB( );
  }
}

AxisModelPC::~AxisModelPC()
{
  delete mMeshX;
  delete mMeshY;
  delete mMeshZ;
}

void AxisModelPC::display()
{
  displayX();
  displayY();
  displayZ();
}

void AxisModelPC::displayX()
{
  mMeshX->DisplaySansShader();
}

void AxisModelPC::displayY()
{
  mMeshY->DisplaySansShader();
}

void AxisModelPC::displayZ()
{
  mMeshZ->DisplaySansShader();
}
