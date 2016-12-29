#include		"dx_misc.hpp"
#include		"dx_model_basic.hpp"
#include    "Global.hpp"
#include  "Geometry/DxMathHelpers.hpp"

#define PI float(3.14121592f)
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//		DxCubeModel
///////////////////////////////////////////////////////////////////////////////


void		DxCubeModel::ConvertToDx	( CubeModel c )
{
  _nTriangles   = 6*6;
  _nVertices    = _nTriangles;

  D3DXVECTOR3 center( c.center.x, c.center.y, c.center.z ) ;

  _vertices	=	vector<D3DXVECTOR3> ( 36*2) ;
  /*
	id_linearorder	= c.id_linearorder;
	id_side			    = c.id_side;
  */

	//Transform matrix

  D3DXMATRIX  t;
  t = c.frameBasis;
  D3DXMATRIX ti;
  D3DXMatrixInverse ( &ti, NULL, &t );
	

	//Planes where Z is fixed
	
  D3DXVECTOR4 temp;

  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, +c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*0]	= center + D3DXVECTOR3(temp);
  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, -c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*1]	= center + D3DXVECTOR3(temp);
  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, -c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*2]	= center +  D3DXVECTOR3(temp);

  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, -c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*3]	= center +  D3DXVECTOR3(temp);
  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, +c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*4]	= center +  D3DXVECTOR3(temp);
  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, +c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*5]	= center + D3DXVECTOR3(temp);

  _vertices[2*5 + 1 ] = _vertices[2*4 + 1] = 
	_vertices[2*0 + 1] = _vertices[2*1 + 1]	= _vertices[2*2 + 1 ] = _vertices[2*3 + 1] = 
      DxMath::normalOfTriangle ( _vertices[2*0], _vertices[2*1], _vertices[2*2] );



  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, +c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*6]	= center +  D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, -c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*7]	= center +  D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, -c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*8]	= center +  D3DXVECTOR3(temp);

    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, -c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*9]	= center +  D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, +c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*10]	= center +  D3DXVECTOR3(temp);
  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, +c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*11]	= center +  D3DXVECTOR3(temp);


  _vertices[2*6 + 1] = _vertices[2*7 + 1]	=
	_vertices[2*8 + 1] = _vertices[2*9 + 1]	= _vertices[2*10 + 1 ] = _vertices[2*11 + 1] = 
      DxMath::normalOfTriangle ( _vertices[2*6], _vertices[2*7], _vertices[2*8] );


	//Planes where X is fixed
  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, +c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*12]	= center +  D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, -c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*13]	= center +  D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, -c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*14]	= center +  D3DXVECTOR3(temp);

D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, -c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*15]	= center +  D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, +c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*16]	= center +  D3DXVECTOR3(temp);
  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, +c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*17]	= center +  D3DXVECTOR3(temp);

  _vertices[2*16 + 1 ] = _vertices[2*17 + 1] = 
	_vertices[2*12 + 1] = _vertices[2*13 + 1]	= _vertices[2*14 + 1 ] = _vertices[2*15 + 1] = 
      DxMath::normalOfTriangle ( _vertices[2*12], _vertices[2*13], _vertices[2*14] );


  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, +c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*18]	= center +  D3DXVECTOR3(temp);
     D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, -c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*19]	= center + D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, -c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*20]	= center +  D3DXVECTOR3(temp);

D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, -c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*21]	= center +  D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, +c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*22]	= center +  D3DXVECTOR3(temp);
  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, +c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*23]	= center +  D3DXVECTOR3(temp);

  _vertices[2*18 + 1] = _vertices[2*19 + 1]	=
	_vertices[2*20 + 1] = _vertices[2*21 + 1]	= _vertices[2*22 + 1 ] = _vertices[2*23 + 1] = 
      DxMath::normalOfTriangle ( _vertices[2*18], _vertices[2*19], _vertices[2*20] );


	//Planes where Y is fixed
   D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, -c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*24]	= center + D3DXVECTOR3(temp);
     D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, -c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*25]	= center + D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, -c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*26]	= center +  D3DXVECTOR3(temp);

D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, -c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*27]	= center +  D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, -c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*28]	= center +  D3DXVECTOR3(temp);
D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, -c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*29]	= center + D3DXVECTOR3(temp);

  _vertices[2*24 + 1] = _vertices[2*25 + 1]	= 
	_vertices[2*26 + 1] = _vertices[2*27 + 1]	= _vertices[2*28 + 1 ] = _vertices[2*29 + 1] = 
      DxMath::normalOfTriangle ( _vertices[2*24], _vertices[2*25], _vertices[2*26] );


  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, +c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*30]	= center +  D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, +c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*31]	= center +  D3DXVECTOR3(temp);
   D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, +c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*32]	= center +  D3DXVECTOR3(temp);

D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, +c.ysize/2.0f, -c.zsize/2.0f ), &ti );
	_vertices[2*33]	= center +  D3DXVECTOR3(temp);
    D3DXVec3Transform( &temp, &D3DXVECTOR3 ( -c.xsize/2.0f, +c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*34]	= center + D3DXVECTOR3(temp);
  D3DXVec3Transform( &temp, &D3DXVECTOR3 ( +c.xsize/2.0f, +c.ysize/2.0f, +c.zsize/2.0f ), &ti );
	_vertices[2*35]	= center +  D3DXVECTOR3(temp);

  _vertices[2*35 + 1] = _vertices[2*34 + 1]	= 
	_vertices[2*30 + 1] = _vertices[2*31 + 1]	= _vertices[2*32 + 1 ] = _vertices[2*33 + 1] = 
      DxMath::normalOfTriangle ( _vertices[2*30], _vertices[2*31], _vertices[2*32] );
}

///////////////////////////////////////////////////////////////////////////////
//		
///////////////////////////////////////////////////////////////////////////////

DxCubeModelPool::DxCubeModelPool   ()
{
  _nTriangles        = 0;
  _nVertices         = 0;
}

void DxCubeModelPool::addCubeModel  ( DxCubeModel* cyl )
{
  _nTriangles  += cyl->_nTriangles;
  _nVertices   += cyl->_nVertices;

  unsigned int size = _vertices.size();
  _vertices.resize ( size + cyl->_vertices.size() );

  for ( unsigned int i = 0; i < cyl->_vertices.size() ; ++i )
    _vertices[ size + i] = cyl->_vertices[i];

}

bool DxCubeModelPool::createVB   	( )
{
  
  //Vertex buffer

  D3D10_BUFFER_DESC bd;
  D3D10_SUBRESOURCE_DATA InitData;

  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = sizeof( VertexPosNor ) * _nVertices;

  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;


  HRESULT hr; //Used to check everything went ok (Windows only)
  
  InitData.pSysMem = &_vertices[0];
  hr = Global::md3dDevice->CreateBuffer( &bd, &InitData, &mVertexBuffer );
  if( FAILED( hr ) ) 
      return false;  

  return true;
}

void DxCubeModelPool::Display		  ( )
{   
  Global::md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  UINT stride = sizeof( VertexPosNor );
  UINT offset = 0;
  Global::md3dDevice->IASetVertexBuffers( 0, 1, &mVertexBuffer, &stride, &offset );  
  Global::md3dDevice->Draw(_nVertices, 0);
}



///////////////////////////////////////////////////////////////////////////////
//		DxArchCubeModel
///////////////////////////////////////////////////////////////////////////////

//Arch

DxArchCubeModel::DxArchCubeModel(
      const D3DXVECTOR3* pa,
      const D3DXVECTOR3* pb,
      const D3DXMATRIX*  ma,
      const D3DXMATRIX*  mb,
      float              yLength,
      float              zLength,
      int                side
                                 )
{
  // Each letter corresponds to a point of the ArchCube,
  // note that I skipped the g point, man I can't find it!
  D3DXVECTOR3 a, b, c, d, e, f, h, i;
  D3DXVECTOR3 nullvec;

 
  if ( side == 0 || side == 1 )
  {
    a = *pa + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, 1.0f, -zLength*0.5f ), ma );
    b = *pb + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, 1.0f, -zLength*0.5f ), mb );

    c = *pb + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, 1.0f + yLength, -zLength*0.5f ), ma );
    d = *pa + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, 1.0f + yLength, -zLength*0.5f ), mb );

    i = *pa + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, 1.0f, zLength*0.5f ), ma );
    h = *pb + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, 1.0f, zLength*0.5f ), mb );

    e = *pb + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, 1.0f + yLength, zLength*0.5f ), ma );
    f = *pa + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, 1.0f + yLength, zLength*0.5f ), mb );
  }
  else
  {
    a = *pa + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f,  (zLength*0.5f) , 1.0f ), ma );
    b = *pa + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, -(zLength*0.5f) , 1.0f ), ma );

    c = *pb + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, -(zLength*0.5f) , 1.0f ), mb );
    d = *pb + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f,  (zLength*0.5f) , 1.0f ), mb );

    i = *pb + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f,  (zLength*0.5f) , (1.0f + yLength) ), mb );
    h = *pb + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, -(zLength*0.5f) , (1.0f + yLength) ), mb );

    e = *pa + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f,  (zLength*0.5f) , (1.0f + yLength) ), ma );
    f = *pa + *D3DXVec3TransformCoord ( &nullvec, &D3DXVECTOR3( 0.0f, -(zLength*0.5f) , (1.0f + yLength) ), ma );
  }

  //  Create vertices
  _nTriangles = 6*2;
  _nVertices  = _nTriangles*3;
  _vertices   = vector<VertexPosNor> ( _nVertices );
  _pos = 0;

  //Matrix: (T,N,B)
  if ( side == 0 )
  {
    makeQuad( &a, &b, &c, &d );
    makeQuad( &d, &c, &e, &f );
    makeQuad( &a, &d, &f, &i );
    makeQuad( &b, &a, &i, &h );
    makeQuad( &e, &c, &b, &h );
    makeQuad( &f, &e, &h, &i );
  }

  //Matrix: (T,-N,B)
  if ( side == 1 )
  {
    makeQuad( &a, &d, &c, &b );//
    makeQuad( &f, &e, &c, &d );
    makeQuad( &i, &f, &d, &a );
    makeQuad( &h, &i, &a, &b );
    makeQuad( &h, &b, &c, &e );
    makeQuad( &i, &h, &e, &f );//
  }

  //Matrix: (T,N,B)
  if ( side == 2 )
  {
    makeQuad( &e, &f, &b, &a );
    makeQuad( &f, &h, &c, &b );
    makeQuad( &h, &i, &d, &c );
    makeQuad( &a, &d, &i, &e );
    makeQuad( &e, &i, &h, &f );
    makeQuad( &b, &c, &d, &a );
  }

  //Matrix: (T,-N,B)
  if ( side == 3 )
  {
    makeQuad( &a, &b, &f, &e );
    makeQuad( &b, &c, &h, &f );
    makeQuad( &c, &d, &i, &h );
    makeQuad( &d, &a, &e, &i );
    makeQuad( &f, &h, &i, &e );
    makeQuad( &a, &d, &c, &b );
  }

}

void DxArchCubeModel::makeQuad ( const D3DXVECTOR3* p0, const D3DXVECTOR3* p1,
                            const D3DXVECTOR3* p2, const D3DXVECTOR3* p3)
{
  D3DXVECTOR3 n = DxMath::normalOfTriangle( *p0, *p1, *p2 );
  _vertices[_pos + 0].pos  = *p0;
  _vertices[_pos + 1].pos  = *p1;
  _vertices[_pos + 2].pos  = *p2;
  _vertices[_pos + 0].nor  = n;
  _vertices[_pos + 1].nor  = n;
  _vertices[_pos + 2].nor  = n; 

  n = DxMath::normalOfTriangle( *p2, *p3, *p0 );  
  _vertices[_pos + 3].pos  = *p2;
  _vertices[_pos + 4].pos  = *p3;
  _vertices[_pos + 5].pos  = *p0;
  _vertices[_pos + 3].nor  = n;
  _vertices[_pos + 4].nor  = n;
  _vertices[_pos + 5].nor  = n; 

  _pos += 6;
}

///////////////////////////////////////////////////////////////////////////////
//		
///////////////////////////////////////////////////////////////////////////////

DxArchCubeModelPool::DxArchCubeModelPool   ( )
{
  _nTriangles        = 0;
  _nVertices         = 0;
}

void DxArchCubeModelPool::addArchCubeModel  ( DxArchCubeModel* cyl )
{
  _nTriangles  += cyl->_nTriangles;
  _nVertices   += cyl->_nVertices;

  unsigned int size = _vertices.size();
  _vertices.resize ( size + cyl->_vertices.size() );

  for ( unsigned int i = 0; i < cyl->_vertices.size() ; ++i )
    _vertices[ size + i] = cyl->_vertices[i];

}

bool DxArchCubeModelPool::createVB   	( )
{
  
  //Vertex buffer

  D3D10_BUFFER_DESC bd;
  D3D10_SUBRESOURCE_DATA InitData;

  bd.Usage = D3D10_USAGE_DEFAULT;
  bd.ByteWidth = sizeof( VertexPosNor ) * _nVertices;

  //bd.ByteWidth = 100;
  bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;


  HRESULT hr; //Used to check everything went ok (Windows only)
  
  InitData.pSysMem = &_vertices[0];
  hr = Global::md3dDevice->CreateBuffer( &bd, &InitData, &mVertexBuffer );
  if( FAILED( hr ) ) 
      return false;  

  return true;
}

void DxArchCubeModelPool::Display		  ( )
{ 
  Global::md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  UINT stride = sizeof( VertexPosNor );
  UINT offset = 0;
  Global::md3dDevice->IASetVertexBuffers( 0, 1, &mVertexBuffer, &stride, &offset );  
  Global::md3dDevice->Draw(_nVertices, 0); 
}