#include		"RenderModels\Basics\RectangleModel.hpp"



RectangleModel::RectangleModel ( const D3DXVECTOR3& aCenter, const float aLengthU, const float aLengthV )
{
  mNumVertices = 6;
  mVertices.resize( mNumVertices );
  
  D3DXVECTOR3 u = D3DXVECTOR3(1,0,0);
  D3DXVECTOR3 v = D3DXVECTOR3(0,0,1);

  mVertices[0] = aCenter + 0.5f*aLengthU*u + 0.5f*aLengthV*v;
  mVertices[1] = aCenter - 0.5f*aLengthU*u + 0.5f*aLengthV*v;
  mVertices[2] = aCenter - 0.5f*aLengthU*u - 0.5f*aLengthV*v;
  mVertices[3] = aCenter - 0.5f*aLengthU*u - 0.5f*aLengthV*v;
  mVertices[4] = aCenter + 0.5f*aLengthU*u - 0.5f*aLengthV*v;
  mVertices[5] = aCenter + 0.5f*aLengthU*u + 0.5f*aLengthV*v;
  
  
  mMesh = new DxMeshModelBatch<VertexPos> ( sizeof(VertexPos), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  mMesh->addMeshModel( new DxMeshModel<VertexPos>( sizeof(VertexPos), mNumVertices, (VertexPos*)&mVertices[0] ) );
  mMesh->createVB();
    
}
