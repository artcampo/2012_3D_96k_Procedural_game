#ifndef _GameBP_RectangleCollider_HPP_
#define _GameBP_RectangleCollider_HPP_

#include "dx_misc.hpp"
#include <vector>

#include  "Geometry/intersections.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"
#include  "RenderModels/Compounded/DeferredMeshModel.h"

class RectangleCollider {

public:  

  RectangleCollider(  const Intersections::CollisionRectangle& aRect,
                      const float aReloadTime );

  RectangleCollider(  const Intersections::CollisionRectangle& aRect );

 

  //void  display() const;
  void  update( const float aLapsedTime );  
  bool  cross( const D3DXVECTOR3& aPosWS );
  float crossDebug( const D3DXVECTOR3& aPosWS );

  //TODO: private
  bool  hasEffect() const;

  void  apply();

private:
  bool  mHasBeenUsed;
  float mLapsedTimeSinceUse; 
  float mReloadTime;
  Intersections::CollisionRectangle mRect;

  const static float msReloadTime;

public:

#ifndef GBP_DISABLE_RENDER_COLLIDERS_RECT
  DxMeshModelBatch<VertPNT>* mMesh;
  void generateMesh();
#endif

};


#endif  //_GameBP_RectangleCollider_HPP_
