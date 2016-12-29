#include		"RenderModels\Basics/CylinderModel.hpp"


#define PI float(3.14121592f)

const int CYL_LENGTH_TESELLATION = 2; //min: 2
const int CYL_RADIUS_TESELLATION = 8; //min: 2



///////////////////////////////////////////////////////////////////////////////
//		CylinderModel
///////////////////////////////////////////////////////////////////////////////


bool CylinderModel::init( D3DXVECTOR3 p1, D3DXVECTOR3 p2, float r1, float r2 )
{
	mLengthTesellation	=	CYL_LENGTH_TESELLATION;
	mRadiusTesellation	=	CYL_RADIUS_TESELLATION;
	BuildModel ( p1, p2, r1, r2, false, false, D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f));
  return true;
}

bool CylinderModel::initDirNext( D3DXVECTOR3 p1, D3DXVECTOR3 p2, float r1, float r2, D3DXVECTOR3 nd )
{
	mLengthTesellation	=	CYL_LENGTH_TESELLATION;
	mRadiusTesellation	=	CYL_RADIUS_TESELLATION;
	BuildModel ( p1, p2, r1, r2, false, true, D3DXVECTOR3(0.0f,0.0f,0.0f), nd );
  //BuildModel ( p1, p2, r1, r2, false, false, D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f) );
  return true;
}

bool CylinderModel::initDirPrev( D3DXVECTOR3 p1, D3DXVECTOR3 p2, float r1, float r2, D3DXVECTOR3 pd )
{
	mLengthTesellation	=	CYL_LENGTH_TESELLATION;
	mRadiusTesellation	=	CYL_RADIUS_TESELLATION;
	BuildModel ( p1, p2, r1, r2, true, false, pd, D3DXVECTOR3(0.0f,0.0f,0.0f) );
  //BuildModel ( p1, p2, r1, r2, false, false, D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f) );
  return true;
}


bool CylinderModel::init( D3DXVECTOR3 p1, D3DXVECTOR3 p2, float r1, float r2, D3DXVECTOR3 pd, D3DXVECTOR3 nd )
{
	mLengthTesellation	=	CYL_LENGTH_TESELLATION;
	mRadiusTesellation	=	CYL_RADIUS_TESELLATION;
	BuildModel ( p1, p2, r1, r2, true, true, pd, nd );
  //BuildModel ( p1, p2, r1, r2, false, false, D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f) );
  return true;

}


/*
  this class is to provide continous cylinders,
  as such the current one can be modified in function of the
  previous and next one. Taking their direction vectors.
  D3DXVECTOR3 pd, D3DXVECTOR3 nd
*/
void		CylinderModel::BuildModel ( D3DXVECTOR3 p1, D3DXVECTOR3 p2, float r1, float r2, bool pdUsed, bool ndUsed, D3DXVECTOR3 pd, D3DXVECTOR3 nd )
{
  int number_quads = mRadiusTesellation * mLengthTesellation ;
  mNumVertices =  number_quads * (6);
  mVertices.resize ( mNumVertices );

  ///////////////////////////////////
  //  Hacer matriz regular
	D3DXVECTOR3 p2_p1 = p2 - p1;
  p1 -= p2_p1*0.03f;
  p2 += p2_p1*0.03f;
  p2_p1 = p2 - p1;
  D3DXVECTOR3 copy_p2_p1 = p2_p1;
  D3DXMATRIX FrameBasisMatrix = TbnMatrix ( p2_p1 );

  ///////////////////////////////////
  //  Hacer matriz primer corte
	if (pdUsed)
  {
    //p2_p1.normalize();
    D3DXVECTOR3 u = pd;
    //u.normalize();
    p2_p1 += u;
    p2_p1 = p2_p1 / 2.0f;
  }
	D3DXMATRIX FrameBasisMatrix_initial = TbnMatrix ( p2_p1 );

  ///////////////////////////////////
  //  Hacer matriz corte final
  if (ndUsed)
  {
   // p2_p1.normalize();
    D3DXVECTOR3 u = nd;
    //u.normalize();
    p2_p1 += u;
    p2_p1 = p2_p1 / 2.0f;
  }
	D3DXMATRIX FrameBasisMatrix_final = TbnMatrix ( p2_p1 );

  //restaurar p2p1
  p2_p1 = copy_p2_p1;

	int		next_quad = 0;
	for (  int leveli = 0;  leveli < (mLengthTesellation ) ; ++leveli)
		for (  int radi = 0;  radi < mRadiusTesellation ; ++radi)
		{
			D3DXVECTOR3 v = p1 + p2_p1* (leveli / float(mLengthTesellation) );
			D3DXVECTOR3 v2 = p1 + p2_p1* ((leveli + 1) / float(mLengthTesellation) );

			float	ri	= (r1*leveli)/float(mLengthTesellation) + (r2*(mLengthTesellation-leveli))/float(mLengthTesellation) ;
			float	rii	= (r1*(leveli + 1))/float(mLengthTesellation) + (r2*(mLengthTesellation-(leveli + 1)))/float(mLengthTesellation) ;
			float	ai	= radi*2*PI/float(mRadiusTesellation);
			float	aii	= (radi + 1)*2*PI/float(mRadiusTesellation);

      /////////////////////////////////
      if (leveli == 0)
			mVertices[ next_quad + 0 ].pos = 
				v + MultiplyMatrix( FrameBasisMatrix_initial, D3DXVECTOR3( 
								cos(ai) * ri,
								0.0f, 
								sin(ai) * ri 
								));
      else
			mVertices[ next_quad + 0 ].pos = 
				v + MultiplyMatrix( FrameBasisMatrix, D3DXVECTOR3( 
								cos(ai) * ri,
								0.0f, 
								sin(ai) * ri 
								));

      /////////////////////////////////
      if (leveli == 0)
			mVertices[ next_quad + 1 ].pos = 
				v + MultiplyMatrix( FrameBasisMatrix_initial, D3DXVECTOR3( 
								cos(aii) * ri,
								0.0f, 
								sin(aii) * ri 
								));
      else
			mVertices[ next_quad + 1 ].pos = 
				v + MultiplyMatrix( FrameBasisMatrix, D3DXVECTOR3( 
								cos(aii) * ri,
								0.0f, 
								sin(aii) * ri 
								));

      /////////////////////////////////
      if (leveli == (mLengthTesellation - 1))
			mVertices[ next_quad + 2 ].pos = 
				v2 + MultiplyMatrix( FrameBasisMatrix_final, D3DXVECTOR3( 
								cos(aii) * rii,
								0.0f, 
								sin(aii) * rii 
								));
      else
			mVertices[ next_quad + 2 ].pos = 
				v2 + MultiplyMatrix( FrameBasisMatrix, D3DXVECTOR3( 
								cos(aii) * rii,
								0.0f, 
								sin(aii) * rii 
								));

      //triangulo 2

      /////////////////////////////////
      if (leveli == (mLengthTesellation - 1))
			mVertices[ next_quad + 3].pos = 
				v2 + MultiplyMatrix( FrameBasisMatrix_final, D3DXVECTOR3( 
								cos(aii) * rii,
								0.0f, 
								sin(aii) * rii 
								));
      else
			mVertices[ next_quad + 3].pos = 
				v2 + MultiplyMatrix( FrameBasisMatrix, D3DXVECTOR3( 
								cos(aii) * rii,
								0.0f, 
								sin(aii) * rii 
								));

      /////////////////////////////////
      if (leveli == (mLengthTesellation - 1))
			mVertices[ next_quad + 4 ].pos = 
				v2 + MultiplyMatrix( FrameBasisMatrix_final, D3DXVECTOR3( 
								cos(ai) * rii,
								0.0f, 
								sin(ai) * rii
								));
      else
			mVertices[ next_quad + 4 ].pos = 
				v2 + MultiplyMatrix( FrameBasisMatrix, D3DXVECTOR3( 
								cos(ai) * rii,
								0.0f, 
								sin(ai) * rii
								));

      /////////////////////////////////
      if (leveli == 0)
			mVertices[ next_quad + 5 ].pos = 
				v + MultiplyMatrix( FrameBasisMatrix_initial, D3DXVECTOR3( 
								cos(ai) * ri,
								0.0f, 
								sin(ai) * ri 
								));
      else
			mVertices[ next_quad + 5 ].pos = 
				v + MultiplyMatrix( FrameBasisMatrix, D3DXVECTOR3( 
								cos(ai) * ri,
								0.0f, 
								sin(ai) * ri 
								));

			//D3DXVECTOR3 n = NormalOfQuad( &quads[ next_quad + 0 ] );
      D3DXVECTOR3 n = NormalOfTriangle( &mVertices[ next_quad + 4 ].pos, &mVertices[ next_quad + 2 ].pos, &mVertices[ next_quad + 0 ].pos );
      mVertices[ next_quad + 0 ].nor = n;
      mVertices[ next_quad + 1 ].nor = n;
      mVertices[ next_quad + 2 ].nor = n;
      mVertices[ next_quad + 3 ].nor = n;
      mVertices[ next_quad + 4 ].nor = n;
      mVertices[ next_quad + 5 ].nor = n;

			next_quad += 6;
		}	
}



void  CylinderModel::storeMesh ( DxMeshModel<VertexPosNor>** aMesh )
{
  *aMesh = new DxMeshModel<VertexPosNor>( sizeof(VertexPosNor), mNumVertices, &mVertices[0] );
}


D3DXMATRIX  CylinderModel::TbnMatrix ( const D3DXVECTOR3& aVect )
{
	D3DXVECTOR3 TBNvec[3];

	if ( aVect.x == 0.0f && aVect.y == 0.0f && aVect.z != 0.0f )
	{		 
    D3DXVec3Normalize ( &TBNvec[0], &aVect );
		TBNvec[1] = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		D3DXVec3Cross( &TBNvec[2], &TBNvec[0], &TBNvec[1] );
	}
	else
	{
		D3DXVec3Normalize ( &TBNvec[0], &aVect );
		TBNvec[1] = perpendicular3D(TBNvec[0]);
		D3DXVec3Cross( &TBNvec[2], &TBNvec[0], &TBNvec[1] );
	}
	return basisMatrix( TBNvec[1], TBNvec[0], TBNvec[2] );
}