#ifndef _GameBP_Bumper_HPP_
#define _GameBP_Bumper_HPP_

#include "dx_misc.hpp"
#include <vector>

#include "Physics/MeshCol.hpp"
#include "RenderModels\DxMeshModel.hpp"
#include "Geometry/Loft.hpp"
#include "Geometry/Spline/SplineIntegral.hpp"

#include "Project_Dependant/StoredShaders.hpp"
#include "Shader/Shader.hpp"
#include "GameBall/Entities/Ball.hpp"

class RectangleCollider;
class Tricks;

class Bumper {
public:  
  Bumper( const int aIdBumper,
          const int aIdShape,
          const int aIdRotation,
          const D3DXVECTOR3& aCenter,
          const D3DXMATRIX& aBasis,
          const float aHeight  );
  ~Bumper();

  
  void  update( const float aLapsedTime );
  void  cross( Ball& aBall, Tricks* aTrickManager );

  D3DXMATRIX Bumper::basisRender();
  D3DXMATRIX Bumper::basis(){ return mBasis;};


public:
  RectangleCollider*  mColliderOnTop;
  RectangleCollider*  mColliderAtLeft;
  RectangleCollider*  mColliderAtRight;
  

private:
  int   mId;
  int   mIdShape;
  int   mIdRotation;

  D3DXVECTOR3   mCenter;
  D3DXMATRIX    mBasis;
  float         mHeight;  

  D3DXMATRIX  mCurrentRotation;

  friend class GameBpApp;
  friend class EntitiesOnLinesAndTNBs;

  Intersections::CollisionRectangle initCollisionRect(  const float aWidth, const float aHeigth,
                                                        const float aX, const float aY );
};


#endif  //_GameBP_Bumper_HPP_
