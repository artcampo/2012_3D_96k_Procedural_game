#ifndef _DIRGRAPH_HPP
#define _DIRGRAPH_HPP

//#include <algorithm>
#include <vector>

#include "GameBall/TrackBp.hpp"
#include "Geometry/DirGraph/DirGraphDataTypes.hpp"
#include "Misc/StlHelpers.hpp"
#include "Geometry/DirGraph/DirGraphIO.hpp"

class Track;

namespace TrackGraphCollision{
  class CollisionConfiguration;
  class CollisionData;
};

namespace DirectedGraph {

class DirGraph{

public:
  //DirGraph(){};

  void  setRoot( const int aIdv );
  int   newNode( const TrackBp::Vertex& aData, const bool aIsMutable );  
  int   newEdge( const int aIdv1, const int aIdv2, const TrackBp::EdgeData& aEdge );  

  void  setEdge   ( const int aIdv1, const int aIdv2, const TrackBp::EdgeData& aData );
  void  setVertex ( const int aIdv, TrackBp::Vertex& aData, const bool aIsMutable  );

  bool              isVertexMutable  ( const int aIdv ) const;
  std::vector<int>  mutableVertices() const;


  void  deleteDescendants ( const int aIdv );
  void  deleteParents     ( const int aIdv );
  void  deleteDescendant  ( const int aIdv1, const int aIdv2 );
  void  deleteParent      ( const int aIdv1, const int aIdv2 );


  std::vector<Edge>     edges() const;
  std::vector<EdgeCon>  edgesAndConnectivity() const;

  int               root() const;
  int               numVertices() const;
  int               numOutEdges( const int aIdv ) const;
  
  TrackBp::Vertex   vertex  ( const int aIdv ) const;
  TrackBp::EdgeData edge    ( const int aIdv1, const int aIdv2 ) const;
  int               numDesc ( const int aIdv ) const;
  int               numPar  ( const int aIdv ) const;
  bool              hasArityTwo ( const int aIdv ) const;

  std::vector<int>  descendants ( const int aIdv ) const;
  std::vector<int>  parents     ( const int aIdv ) const;

  TrackGraphCollision::CollisionData  collisionData( const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration ) const;

  void    deleteVertex( const int aIdv );
private:  
  int                           mRoot;
  std::vector<Vertex>           mVertices;
  std::vector<TrackBp::Vertex>  mVerticesData;
  std::vector<bool>             mVerticesIsMutable;

private:
  void edgesRec( const int aIdv, std::vector<Edge>& aEdges ) const;
  Edge edgesRec( const int aIdv, const int aDescIndex, std::vector<EdgeCon>& aEdges ) const;

  //DEBUG
  friend class Track;
  //-DEBUG

};

};



#endif // _DIRGRAPH_HPP
