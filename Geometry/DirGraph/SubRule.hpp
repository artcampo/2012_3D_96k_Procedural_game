#ifndef _SubRule_HPP
#define _SubRule_HPP

//#include <algorithm>
#include <vector>

#include  "GameBall/TrackBp.hpp"
#include  "Geometry/DirGraph/DirGraph.hpp"
#include  "Geometry/DirGraph/Pattern.hpp"
#include  "Geometry/DirGraph/DirGraphDataTypes.hpp"
#include  "Noise/RandomNumberGenerator.hpp"

namespace TrackGraphCollision{
  class CollisionConfiguration;
  class CollisionData;
};

namespace DirectedGraph{
  
   
  class RuleMatch{
  public:
    bool  matched;
    int   outcomingPattern;
    int   lastVertex;       //last vertex of graph that matches
  };

  float distance(  const DirectedGraph::DirGraph& aGraph,
                   const int aVid1, 
                   const int aVid2 );

  void  initPatternOut      ( DirectedGraph::PatternOutNode* aPattern );

  DirectedGraph::RuleMatch
          matchPatternREC(  const DirectedGraph::DirGraph& aGraph, 
                            DirectedGraph::PatternIn& aPatternIn,
                            DirectedGraph::PatternInNode const* aCurrentNode,
                            const int aStartingVertex,
                            const int aCurrentVertex,
                            DirectedGraph::PatternOutNode* aOutCurrentNode,
                            DirectedGraph::PatternOut* aOutcomingPattern);

  DirectedGraph::RuleMatch
          matchPattern( const DirectedGraph::DirGraph& aGraph, 
                        PatternIn& aPatternIn,                         
                        DirectedGraph::PatternOut* aOutcomingPattern,
                        const int aStartingVertex );

  void    applyRule(  DirectedGraph::DirGraph& aGraph,
                      DirectedGraph::PatternOut* aOutPattern,
                      Rand::RandGen&                  aRandGen,
                      const int aStartingVertex,
                      const int aEndVertex );

  int    applyRuleCreateNodeRec(  DirectedGraph::DirGraph& aGraph,                                   
                                  DirectedGraph::PatternOut* aOutPattern,
                                  DirectedGraph::PatternOutNode* aOutcomingPattern,                                                                    
                                  Rand::RandGen&                  aRandGen,
                                  const TrackBp::Vertex& aVerBeg,
                                  const int aStartVertex,
                                  const int aEndVertex,
                                  const DxMath::TnbBasis& aBasisAB,
                                  const float aLength );

  void    balanceBranchesLengths( DirectedGraph::DirGraph& aGraph,
                                  DirectedGraph::PatternOut* aOutPattern );
  

  bool testPieceInGraph(  const DirectedGraph::DirGraph& aGraph,
                          const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration,
                          const TrackGraphCollision::CollisionData& aCollisionData,                          
                          DirectedGraph::PatternOut* aOutPattern,                          
                          const int aStartingVertex,
                          const int aEndVertex );

  bool   testPieceInGraphRec( const DirectedGraph::DirGraph&        aGraph,
                              const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration,
                              const TrackGraphCollision::CollisionData& aCollisionData,                              
                              DirectedGraph::PatternOutNode*  aOutPatternNode,                                                            
                              const int aStartVertex,
                              const int aEndVertex,
                              const DxMath::TnbBasis& aBasisAB);

};



#endif // _SubRule_HPP
