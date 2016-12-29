#include  "GameBall/Entities/Colliders/RectangleCollider.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"

const float RectangleCollider::msReloadTime = (1.0 * 100.0f);


RectangleCollider::
RectangleCollider(  const Intersections::CollisionRectangle& aRect,                   
                    const float aReloadTime )
  : mReloadTime( aReloadTime),
    mRect(aRect),
    mHasBeenUsed(false)
{
#ifndef GBP_DISABLE_RENDER_COLLIDERS_RECT
  generateMesh();
#endif
}

RectangleCollider::
RectangleCollider(  const Intersections::CollisionRectangle& aRect )
  : mReloadTime( msReloadTime ),
    mRect(aRect),
    mHasBeenUsed(false)
{
#ifndef GBP_DISABLE_RENDER_COLLIDERS_RECT
  generateMesh();
#endif
}


void  RectangleCollider::apply()
{
  mHasBeenUsed        = true;
  mLapsedTimeSinceUse = 0.0f;
}

bool  RectangleCollider::hasEffect() const
{
  if (!mHasBeenUsed)  
    //*mLog->mFile << "Crossed: "<< std::endl;
    return true;      
  else  
    return false;
    
}

bool  RectangleCollider::cross( const D3DXVECTOR3& aPosWS )
{
  bool intersected = 
    Intersections::intersectionPointTriangle( aPosWS, 0.5f, mRect.tri1 ) |
    Intersections::intersectionPointTriangle( aPosWS, 0.5f, mRect.tri2 );

  if ( intersected && hasEffect() )
  {    
    apply();    
    return true;
  }
  return false;
}

float  RectangleCollider::crossDebug( const D3DXVECTOR3& aPosWS )
{
  float ret = min(
    Intersections::intersectionPointTriangleDebug( aPosWS, 14.5f, mRect.tri1 ),
    Intersections::intersectionPointTriangleDebug( aPosWS, 14.5f, mRect.tri2 ) );
  return ret;
}

void  RectangleCollider::update( const float aLapsedTime )
{
  if (mHasBeenUsed)
  {
    mLapsedTimeSinceUse += aLapsedTime;
    if ( mLapsedTimeSinceUse > mReloadTime )
      mHasBeenUsed = false;
  }
}

#ifndef GBP_DISABLE_RENDER_COLLIDERS_RECT

void RectangleCollider::generateMesh()
{
  VertPNT v[6];
  v[0].pos = mRect.tri1.p1;
  v[1].pos = mRect.tri1.p2;
  v[2].pos = mRect.tri1.p3;
  v[3].pos = mRect.tri2.p1;
  v[4].pos = mRect.tri2.p2;
  v[5].pos = mRect.tri2.p3;
  for (int i = 0; i < 6; ++i)
  {
  v[i].tan = D3DXVECTOR3(1,0,0);
  v[i].nor = D3DXVECTOR3(0,1,0);
  }
  
  DxMeshModel<VertPNT>* m = new DxMeshModel<VertPNT>( sizeof(VertPNT), 6, v );
  mMesh = new DxMeshModelBatch<VertPNT> ( sizeof(VertPNT), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  mMesh->addMeshModel(m);
  mMesh->createVB();
}

#endif