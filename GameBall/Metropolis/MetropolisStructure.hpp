#ifndef _GameBP_MetropolisStructure_HPP_
#define _GameBP_MetropolisStructure_HPP_

#include  "GameBall/Track.hpp"
#include "dx_misc.hpp"
#include <vector>

class Rand::RandGen;
class MetArmModel;
class TrackEdge;

namespace MetData{

  //  Segment of an edge, to place arms around
  class EdgeSeg{
  public:
    EdgeSeg(){};
    EdgeSeg( TrackEdge* aEdge, const float l )
    {
      edge   = aEdge;
      length  = l;
    }
    TrackEdge*  edge;
    float       length;
  };

  //  Edge for collision
  class EdgeCol{
  public:
    EdgeCol(){};
    EdgeCol( std::vector<D3DXVECTOR3> aSeg )
    {
      mSeg  = aSeg;
    }
    std::vector<D3DXVECTOR3> mSeg;

  };
};


class MetropolisStructure {

public:  
  MetropolisStructure( Track& aTrack, Rand::RandGen& aRandGen );

  void    display( DxFustrum& aFustrum );
public:

private:
  Track&   mTrack;
  Rand::RandGen& mRand;  //  copy of rand seed

  std::vector<MetData::EdgeCol>   mEdgesCollision;
  std::vector<MetData::EdgeSeg>   mEdgesChosenForPlacement;

  //  configuration
  float     mVertexDistance;
  float     mProbArmAtEdge;
  float     mDistanceToEdge;
  float     mDistanceBetweenTries;
  float     mEdgeMinimumLengthForPlacing;
  float     mEdgeTesellationToSegment;

  //  Render models
  std::vector<MetArmModel*>     mArms;

private:
  void    chooseEdges();
  void    placeStructuresAtEdges();
  void    placeStructureAtEdge( MetData::EdgeSeg& aEdge );

  bool    armPositionIsValid( const D3DXVECTOR3& aPos, const float aDist );

  bool    placeStructureAtEdge1(  const D3DXVECTOR3& pos, 
                                  const D3DXVECTOR3& directorVector, 
                                  const D3DXVECTOR3& normalVector  );
  bool    placeStructureAtEdge2(  const D3DXVECTOR3& pos, 
                                  const D3DXVECTOR3& directorVector, 
                                  const D3DXVECTOR3& normalVector  );

  void    placeArmEdge1( const D3DXVECTOR3& aPos );
  void    placeArmEdge2( const D3DXVECTOR3& aPos1, const D3DXVECTOR3& aPos2 );


  void    addEdgeToCollisionStructure( TrackEdge* aEdge );
  void    chooseEdgeForPlacing( TrackEdge* aEdge );
  
public:

};


#endif  //_GameBP_MetropolisStructure_HPP_
