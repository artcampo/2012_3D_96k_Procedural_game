#include  "GameBall/Entities/EndTrigger.hpp"
#include  "GameBall/Game.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"
#include  "GameBall/Entities/Colliders/RectangleCollider.hpp"

EndTrigger::~EndTrigger()
{
  delete mLog;
}

EndTrigger::EndTrigger ( const D3DXVECTOR3& aCenter,
              const D3DXMATRIX& aBasis,              
              const float aWidth,
              const float aHeigth,
              Game* aGame)              
{  
  mCenter = aCenter;
  mBasis  = aBasis;  
  mWidth  = aWidth;
  mHeigth  = aHeigth;

  mCollider = new RectangleCollider( initCollisionRect() );

  mLog = new Log();
  mGame = aGame;

}



Intersections::CollisionRectangle EndTrigger::initCollisionRect()
{
  D3DXVECTOR3 i = D3DXVECTOR3(0,0,1);
  D3DXVECTOR3 j = D3DXVECTOR3(0,1,0);
  
  D3DXVec3TransformCoord( &i, &i, &mBasis );
  D3DXVec3TransformCoord( &j, &j, &mBasis );
  
  D3DXVECTOR3 a = -i*mWidth + j*mHeigth + mCenter;
  D3DXVECTOR3 b = +i*mWidth + j*mHeigth + mCenter;
  D3DXVECTOR3 c = -i*mWidth + mCenter;
  D3DXVECTOR3 d = +i*mWidth + mCenter;

  Intersections::CollisionRectangle rect;
  rect.tri1.p1 = a; rect.tri1.p2 = c; rect.tri1.p3 = b;
  rect.tri2.p1 = c; rect.tri2.p2 = d; rect.tri2.p3 = b;
  
  return rect;
}

void  EndTrigger::applyEffect( Ball& aBall )
{
  *mLog->mFile << "EndTrigger: ";
  mGame->triggerEnd( aBall );
}

bool  EndTrigger::hasEffect() const
{
  return mCollider->hasEffect();
}

void  EndTrigger::cross( const D3DXVECTOR3 aBallPos, Ball& aBall )
{  
  if ( mCollider->cross( aBallPos ) )
  {    
    applyEffect( aBall );   
    *mLog->mFileEvents << "Finished" <<std::endl;
  }
    
}

void  EndTrigger::update( const float aLapsedTime )
{
  mCollider->update( aLapsedTime );
}
