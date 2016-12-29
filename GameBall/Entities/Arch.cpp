#include  "GameBall/Entities/Arch.hpp"
#include  "GameBall/Game.hpp"
#include  "GameBall/Entities/Ball.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"
#include  "GameBall/Entities/Colliders/RectangleCollider.hpp"

const int   Arch::msNumArchs = 5;
const float Arch::msReloadTime[ Arch::msNumArchs ] = 
    { 10.0f *100.0f, 
      10.0f *100.0f, 
      10.0f *100.0f, 
      10.0f *100.0f, 
      10.0f *100.0f};

const float Arch::msInvertTime      = 10.0f*100.0f;
const float Arch::msMaxVelUpTime    = 6.0f *100.0f;
const float Arch::msMaxVelDownTime  = 6.0f *100.0f;
const float Arch::msMaxVelUpCoef    = 0.25f;
const float Arch::msMaxVelDownCoef  = -0.75f;
const float Arch::msTimeUp          = 10*100.0f;

const D3DXVECTOR4 Arch::msColors[ Arch::msNumArchs ] =
    { D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 1.0f ), 
      D3DXVECTOR4( 0.0f, 0.6f, 0.2f, 1.0f ), 
      D3DXVECTOR4( 0.1f, 0.1f, 1.0f, 1.0f ), 
      D3DXVECTOR4( 0.8f, 0.2f, 0.2f, 1.0f ), 
      D3DXVECTOR4( 1.0f, 0.3f, 0.0f, 1.0f )
    };


Arch::~Arch()
{
  delete mLog;
}

Arch::Arch (  const int aId, 
              const D3DXVECTOR3& aCenter,
              const D3DXMATRIX& aBasis,
              const float aHeight,
              Game* aGame)              
{
  mId     = aId;
  mCenter = aCenter;
  mBasis  = aBasis;
  mHeight = aHeight;
  mGame   = aGame;

  
  mCollider = new RectangleCollider( initCollisionRect(), msReloadTime[aId] );
  mBox.init();

  mLog = new Log();
}

D3DXMATRIX* Arch::basis()
{
  return &mBasis;
}

D3DXMATRIX Arch::basisRender()
{
  D3DXMATRIX m;
  D3DXMatrixTranslation(  &m, 
                          mCenter.x,
                          mCenter.y,
                          mCenter.z);

  m = mBasis*m;
  D3DXMATRIX s;
  D3DXMatrixScaling(&s, mHeight*1.5, mHeight, mHeight*1.5);
  m = s*m;

  return m;
}

Intersections::CollisionRectangle Arch::initCollisionRect()
{
  D3DXVECTOR3 i = D3DXVECTOR3(0,0,1);
  D3DXVECTOR3 j = D3DXVECTOR3(0,1,0);
  
  D3DXVec3TransformCoord( &i, &i, &mBasis );
  D3DXVec3TransformCoord( &j, &j, &mBasis );
  
  D3DXVECTOR3 a = -i*mHeight + mCenter + 0.5f*i*mHeight;
  D3DXVECTOR3 b = -i*mHeight + j*mHeight + mCenter + 0.5f*i*mHeight;
  D3DXVECTOR3 c = mCenter + 0.5f*i*mHeight ;
  D3DXVECTOR3 d = j*mHeight + mCenter + 0.5f*i*mHeight;

  Intersections::CollisionTriangle tri1;
  Intersections::CollisionTriangle tri2;
  tri1.p1 = a; tri1.p2 = c; tri1.p3 = b;
  tri2.p1 = c; tri2.p2 = d; tri2.p3 = b;

  Intersections::CollisionRectangle rect;
  rect.tri1 = tri1;
  rect.tri2 = tri2;
  
  return rect;
}

void  Arch::applyEffect( Ball& aBall )
{ 
  #ifdef _BP_EXTRA_LOG
    *mLog->mFile << "Arch: ";
  #endif

  switch ( mId )
  {    
  case eArchCheck:
    #ifdef _BP_EXTRA_LOG
      *mLog->mFile << "Checkpoint"<<std::endl;
    #endif    
      mGame->actionSetCheckPoint( aBall, positionInFront() );    
    break;

  case eArchNitro:
    #ifdef _BP_EXTRA_LOG
      *mLog->mFile << "Add nitro"<<std::endl;
    #endif
      mGame->actionAddNitro( aBall );
    break;

  case eArchVelUp:
    #ifdef _BP_EXTRA_LOG
      *mLog->mFile << "Vel up"<<std::endl;
    #endif
      mGame->actionMaxVelChange( aBall, msMaxVelUpCoef, msMaxVelUpTime );
    break;

  case eArchVelDown:
    #ifdef _BP_EXTRA_LOG
      *mLog->mFile << "Vel Down"<<std::endl;
    #endif
      mGame->actionMaxVelChange( aBall, msMaxVelDownCoef*0.5f, msMaxVelDownTime );
    break;

  case eArchInvert:
    #ifdef _BP_EXTRA_LOG
      *mLog->mFile << "Invert"<<std::endl;
    #endif
    mGame->actionInvertCommands( msInvertTime );
    break;
 
  };

}

bool  Arch::hasEffect() const
{
  return mCollider->hasEffect();  
}

void  Arch::cross( Ball& aBall )
{
  if ( mCollider->cross( aBall.mBallBody->getxWS() ) )      
    applyEffect( aBall );          
}

void  Arch::update( const float aLapsedTime )
{
  mCollider->update( aLapsedTime );
}


D3DXVECTOR4   Arch::color() const
{
 return msColors[mId];
}

D3DXVECTOR3 Arch::positionInFront() const
{
  D3DXVECTOR3 v(3,1,0);
  D3DXVec3TransformCoord( &v, &v, &mBasis);
  return v;
}