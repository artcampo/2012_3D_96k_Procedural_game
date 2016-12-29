#include  "GameBall/Track/TrackGraphCollision.hpp"
#include  "Global.hpp"

namespace TrackGraphCollision{

bool pieceAgainstGraph( const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration,
                        const TrackGraphCollision::CollisionData& aCollisionData,
                        const D3DXVECTOR3& aPosBeg,
                        const D3DXVECTOR3& aPosEnd,
                        const int aStartVertex,
                        const int aEndVertex )
{
  bool valid = true;

  std::vector<Intersections::BoundingSphere> boundingOfEdge = boundingSpheresOfEdge( 
        aCollisionConfiguration,
        aPosBeg,
        aPosEnd );

  for ( int i = 0; i < aCollisionData.mEdges.size() ; ++i )
  {
    const int vb = i;    
    for ( int j = 0; j < aCollisionData.mEdges[i].size() ; ++j )
    {
      const int ve = aCollisionData.mEdges[i][j].mVertexEnd;

      //  do not check edge that it's being substituted
      if ( !( vb == aStartVertex && ve == aEndVertex ) )
      {
        // check against whole edge
        for ( int b1 = 0; b1 < aCollisionData.mEdges[i][j].mSpheres.size() ; ++b1 )
          for ( int b2 = 0; b2 < boundingOfEdge.size() ; ++b2 )
          {
            valid &= Intersections::intersectionSphereSphere(
              aCollisionData.mEdges[i][j].mSpheres[b1],
              boundingOfEdge[b2]
              );

              *Global::mFileDebugTrack << DxMath::distance( 
                aCollisionData.mEdges[i][j].mSpheres[b1].center,
                boundingOfEdge[b2].center )
              << std::endl;
            if ( !valid) 
              return valid;
          }        
      }
    }
  }
  return valid;
}


std::vector<Intersections::BoundingSphere> boundingSpheresOfEdge(
                        const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration,
                        const D3DXVECTOR3& aPosBeg,
                        const D3DXVECTOR3& aPosEnd )
{
  std::vector<Intersections::BoundingSphere> bounding;

  bool  finished          = false;
  float lengthOfEdge      = DxMath::distance( aPosBeg, aPosEnd );
  D3DXVECTOR3 d           = DxMath::unitVector( aPosEnd, aPosBeg );

  if (  lengthOfEdge > 2 &&
        lengthOfEdge < aCollisionConfiguration.mHaloLength )
  {
    Intersections::BoundingSphere b;
    b.center =  d*lengthOfEdge*0.5f + aPosBeg;
    b.radius = aCollisionConfiguration.mSphereRadius;
    bounding.push_back( b );
    return bounding;
  }

  float unprocessedLength = lengthOfEdge - aCollisionConfiguration.mHaloLength;
  float processedLength   = aCollisionConfiguration.mHaloLength;
  

  

  while ( unprocessedLength >= (aCollisionConfiguration.mHaloLength + 2*aCollisionConfiguration.mSphereRadius ) ) 
  {
    Intersections::BoundingSphere b;
    b.center =  d*(processedLength + aCollisionConfiguration.mSphereRadius )
              + aPosBeg;
    b.radius = aCollisionConfiguration.mSphereRadius;
    bounding.push_back( b );

    unprocessedLength -= 2*aCollisionConfiguration.mSphereRadius;
    processedLength   += 2*aCollisionConfiguration.mSphereRadius;
  }

  return bounding;
}

};