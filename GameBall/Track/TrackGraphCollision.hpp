#ifndef _GameTrack_GraphCollision_HPP_
#define _GameTrack_GraphCollision_HPP_

#include "dx_misc.hpp"
#include <vector>

#include "Geometry\Intersections.hpp""

namespace TrackGraphCollision{

class CollisionConfiguration{
  public:
    float mHaloLength;        //space not used for collision
    float mSphereRadius;      
  };

class CollisionEdge{
public:
  int mVertexEnd;
  std::vector<Intersections::BoundingSphere>  mSpheres;
};

class CollisionData{
  public:
    std::vector< std::vector<CollisionEdge> > mEdges;
  };

bool pieceAgainstGraph( const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration,
                        const TrackGraphCollision::CollisionData& aCollisionData,
                        const D3DXVECTOR3& aPosBeg,
                        const D3DXVECTOR3& aPosEnd,
                        const int aStartVertex,
                        const int aEndVertex );

std::vector<Intersections::BoundingSphere> boundingSpheresOfEdge(
                        const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration,
                        const D3DXVECTOR3& aPosBeg,
                        const D3DXVECTOR3& aPosEnd );

};


#endif  //_GameTrack_GraphCollision_HPP_
