#include  "GameBall/Entities/Bumper.hpp"
#include  "GameBall/Entities/Colliders/RectangleCollider.hpp"
#include  "GameBall/Tricks/Tricks.hpp"

Bumper::~Bumper()
{
  
}

Bumper::Bumper ( const int aIdBumper,
                const int aIdShape,
                const int aIdRotation,
                const D3DXVECTOR3& aCenter,
                const D3DXMATRIX& aBasis,
                const float aHeight ) 
  : mId(aIdBumper)
{
  mIdShape    = aIdShape;
  mIdRotation = aIdRotation;
  mCenter     = aCenter;
  mBasis      = aBasis;
  mHeight     = aHeight;

  mCurrentRotation = mBasis;

  mColliderOnTop    = new RectangleCollider( initCollisionRect( 1.5f, 3,    0,      1.25f) );
  mColliderAtLeft   = new RectangleCollider( initCollisionRect( 3,    1.5f, +3.5f,  0) );
  mColliderAtRight  = new RectangleCollider( initCollisionRect( 3,    1.5f, -3.0f,  0) );

}


void  Bumper::update( const float aLapsedTime )
{
  mColliderOnTop->update( aLapsedTime );
  mColliderAtLeft->update( aLapsedTime );
  mColliderAtRight->update( aLapsedTime );
}

D3DXMATRIX Bumper::basisRender()
{
  D3DXMATRIX m;
  D3DXMatrixTranslation(  &m, 
                          mCenter.x,
                          mCenter.y,
                          mCenter.z);

  m = mBasis*m;
  D3DXMATRIX s;
  D3DXMatrixScaling(&s, mHeight, mHeight, mHeight);
  m = s*m;

  return m;
}

Intersections::CollisionRectangle Bumper::initCollisionRect(  const float aWidth, const float aHeigth,
                                                        const float aX, const float aY )
{
  D3DXVECTOR3 i = D3DXVECTOR3(0,0,1);
  D3DXVECTOR3 j = D3DXVECTOR3(0,1,0);
  
  D3DXVec3TransformCoord( &i, &i, &mBasis );
  D3DXVec3TransformCoord( &j, &j, &mBasis );
  
  D3DXVECTOR3 center = mCenter + i*aX + j*aY;
  D3DXVECTOR3 a = -i*aWidth + j*aHeigth + center;
  D3DXVECTOR3 b = +i*aWidth + j*aHeigth + center;
  D3DXVECTOR3 c = -i*aWidth + center;
  D3DXVECTOR3 d = +i*aWidth + center;

  Intersections::CollisionRectangle rect;
  rect.tri1.p1 = a; rect.tri1.p2 = c; rect.tri1.p3 = b;
  rect.tri2.p1 = c; rect.tri2.p2 = d; rect.tri2.p3 = b;
  
  return rect;
}

void  Bumper::cross( Ball& aBall, Tricks* aTrickManager )
{
  if ( mColliderOnTop->cross( aBall.mBallBody->getxWS() ) )      
    aTrickManager->onColBumperOver( mId );

  if ( mColliderAtLeft->cross( aBall.mBallBody->getxWS() ) )      
    aTrickManager->onColBumperLeft( mId );

  if ( mColliderAtRight->cross( aBall.mBallBody->getxWS() ) )      
    aTrickManager->onColBumperRight( mId );
}