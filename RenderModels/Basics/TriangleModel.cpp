#include		"RenderModels/Basics/TriangleModel.hpp"


TriangleModel::TriangleModel( std::vector<D3DXVECTOR3>& aVertices )
{
  mVertices.resize( 3 );
  for( unsigned int i = 0; i < 3; ++i)
    mVertices[i] = aVertices[i];
  mNumVertices = 3;
}

TriangleModel::TriangleModel( D3DXVECTOR3& aV1, D3DXVECTOR3& aV2, D3DXVECTOR3& aV3 )
{
  mVertices.resize( 3 );
  mVertices[0] = aV1;
  mVertices[1] = aV2;
  mVertices[2] = aV3;
  mNumVertices = 3;
}

void  TriangleModel::storeMesh ( std::vector<VertexPosNor>& aMesh )
{
  int index = aMesh.size();
  aMesh.resize( index + 3 );

  aMesh[index + 0].pos = mVertices[0];
  aMesh[index + 1].pos = mVertices[1];
  aMesh[index + 2].pos = mVertices[2];

  D3DXVECTOR3 normal = NormalOfTriangle( &mVertices[0], &mVertices[1], &mVertices[2] );
  aMesh[index + 0].nor = normal;
  aMesh[index + 1].nor = normal;
  aMesh[index + 2].nor = normal;
}

