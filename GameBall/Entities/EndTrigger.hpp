#ifndef _EndTrigger_HPP_
#define _EndTrigger_HPP_

#include "dx_misc.hpp"
#include <vector>

#include "physics/Physics.hpp" 
#include "RenderModels/Basics/SingleBox.hpp"
#include "Global.hpp"

#include "Project_Dependant/StoredShaders.hpp"
#include "Shader/Shader.hpp"

#include "physics/RigidBody.hpp"
#include "Misc/Log.hpp"

class RectangleCollider;
class Game;

class EndTrigger {
public:  
  EndTrigger( const D3DXVECTOR3& aCenter,
              const D3DXMATRIX& aBasis,              
              const float aWidth,
              const float aHeigth,
              Game* aGame);
  ~EndTrigger();
  
  void  update( const float aLapsedTime );
  
  void  cross( const D3DXVECTOR3 aBallPos, Ball& aBall );
  RectangleCollider*  mCollider;

private:
  bool  hasEffect() const;

private:
  Game* mGame;
  D3DXVECTOR3   mCenter;

  D3DXMATRIX    mBasis;
  float         mWidth;    // around center following basis
  float         mHeigth;

  
  Log* mLog;

private:

  Intersections::CollisionRectangle initCollisionRect();
  
  void  applyEffect( Ball& aBall );

public:

};


#endif  //_EndTrigger_HPP_
