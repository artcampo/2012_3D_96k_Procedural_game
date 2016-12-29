#include		"RenderModels/Compounded\ConcavePolygonModel.hpp"



ConcavePolygonModel::ConcavePolygonModel   ( std::vector<D3DXVECTOR3>& aVertices )
{
  mVertices.resize( aVertices.size() );
  for( unsigned int i = 0; i < aVertices.size(); ++i)
    mVertices[i] = aVertices[i];
  mNumVertices = mVertices.size();
}

void ConcavePolygonModel::scale( float aScale )
{
  D3DXVECTOR3 centerPol = center();
  for( unsigned int i = 0; i < mNumVertices ; ++i)
  {
    mVertices[i] -= centerPol;
    mVertices[i] *= aScale;
    mVertices[i] += centerPol;
  }
}

D3DXVECTOR3 ConcavePolygonModel::getIthPoint ( int i )
{
  return mVertices[i];
}

D3DXVECTOR3 ConcavePolygonModel::center()
{
  D3DXVECTOR3 center = D3DXVECTOR3( 0, 0, 0 );
  for( unsigned int i = 0; i < mNumVertices ; ++i)
    center += mVertices[i];
  center *= ( 1.0f / float( mNumVertices ));
  return center;
}


D3DXVECTOR3 ConcavePolygonModel::getNormal()
{
  return GeometryMiscFunctions::NormalOfTriangle( &mVertices[0], &mVertices[1], &mVertices[2] );
}

int ConcavePolygonModel::getNumVertices()
{
  return mNumVertices;
}

bool ConcavePolygonModel::isClockwise()
{
  bool isClockWise = true;
  for( unsigned int i = 0; i < mNumVertices ; ++i)
  {
    bool isTriangle;
    isTriangle = ( 
        signedArea3d(
          mVertices[ (i + 0) % mNumVertices],
          mVertices[ (i + 1) % mNumVertices],
          mVertices[ (i + 2) % mNumVertices]
        ) < 0);
    isClockWise &= isTriangle;
  }
  return isClockWise;
}

void ConcavePolygonModel::forceClockwise()
{
  if( !isClockwise())
  {/*
    for( unsigned int i = 0; i < (mNumVertices/2) ; ++i)
    {
      D3DXVECTOR3 temp = mVertices[i];
      mVertices[i] = mVertices[ (mNumVertices - 1 - i)];
      mVertices[ (mNumVertices - 1 - i)] = temp;
    }*/
    std::vector<D3DXVECTOR3> temp( mVertices.size());
    for( unsigned int i = 0; i < (mNumVertices) ; ++i)    
      temp[i] = mVertices[i];
    for( unsigned int i = 0; i < (mNumVertices) ; ++i)    
      mVertices[i] = temp[mNumVertices-1-i];
        
  }
  bool a = isClockwise();
}

void  ConcavePolygonModel::storeMesh ( std::vector<VertexPosNor>& aMesh )
{
  int numberTriangles = mNumVertices;
  /*int index = aMesh.size();
  aMesh.resize( index + 3*numberTriangles );*/

  D3DXVECTOR3 centerPol = center();
  for( unsigned int i = 0; i < mNumVertices ; ++i)
  {
    TriangleModel t( mVertices[i], mVertices[ ((i + 1) % mNumVertices)], centerPol );
    t.storeMesh( aMesh );
  }
}


