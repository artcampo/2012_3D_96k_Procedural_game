#ifndef _GameBP_Arch_HPP_
#define _GameBP_Arch_HPP_

#include "dx_misc.hpp"
#include <vector>

#include "physics/Physics.hpp" 
#include "RenderModels/Basics/SingleBox.hpp"
#include "Global.hpp"

#include "Project_Dependant/StoredShaders.hpp"
#include "Shader/Shader.hpp"

#include "physics/RigidBody.hpp"
#include "Misc/Log.hpp"
#include "GameBall/Entities/Ball.hpp"


class RectangleCollider;
class Game;

class Arch {
public:  
  Arch( const int aId,
        const D3DXVECTOR3& aCenter,
        const D3DXMATRIX& aBasis,
        const float aHeight,
        Game* aGame);
  ~Arch();

  void  update( const float aLapsedTime );
  
  void  cross( Ball& aBall );

  D3DXMATRIX*   basis();
  D3DXMATRIX    basisRender();
  D3DXVECTOR4   color() const;

  //TODO: private
  bool  hasEffect() const;

public:
  const static int   msNumArchs;
  enum eArchId{
    eArchCheck,
    eArchNitro,
    eArchVelUp,
    eArchVelDown,
    eArchInvert
  };

public:
  D3DXVECTOR3   mCenter;
  RectangleCollider*  mCollider;

private:
  Game* mGame;
  int   mId;
   
  D3DXMATRIX    mBasis;
  float         mHeight;    // around center following basis

  SingleBox                     mBox;
  Log* mLog;

private:
  
  const static float msReloadTime[];
  const static float msInvertTime;
  const static float msMaxVelUpTime;
  const static float msMaxVelDownTime;
  const static float msMaxVelUpCoef;
  const static float msMaxVelDownCoef;
  const static float msTimeUp;
  const static D3DXVECTOR4  msColors[];

  Intersections::CollisionRectangle initCollisionRect();
  
  void  applyEffect( Ball& aBall );
  D3DXVECTOR3 positionInFront() const;

public:

};


#endif  //_GameBP_Arch_HPP_
