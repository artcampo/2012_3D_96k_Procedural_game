#include  "GameBall/Debug/TrackReport.hpp"
#include  "GameBall/Track.hpp"

namespace TrackReport{
/*
void  writeBranchesLength( const Log& aLog, const Track& aTrack )
{

}
*/

};



int    Track::extractBranchesREC(  const int aVertex, 
                                   const int aNumBranch,
                                   std::vector<DirectedGraph::Edge>& aEdges,
                                   std::vector<int>& aVisitedVertices,
                                   std::vector< std::vector<DirectedGraph::Edge> >& aBranches ) const
{
  if ( mGraph.numDesc( aVertex ) == 0 )
  {
    aBranches.resize( aNumBranch + 1 );
    aBranches[aNumBranch] = aEdges;
    return aNumBranch;
  }
  /*
  if ( StlHelpers::isInVector( aVisitedVertices, aVertex ) )
    return aNumBranch;
*/
  aVisitedVertices.push_back( aVertex );
  std::vector<int> desc = mGraph.descendants( aVertex );  

  //if ( mGraph.numDesc( aVertex ) > 1 )

  int currentBranch = aNumBranch;
  if ( mGraph.numDesc( aVertex ) <= 1 )  
  {
    TrackBp::EdgeData data = mGraph.edge( aVertex, desc[0] );    
    aEdges.push_back( DirectedGraph::Edge( aVertex, desc[0], data ));
    currentBranch = extractBranchesREC( desc[0], currentBranch, aEdges, aVisitedVertices, aBranches );
    return currentBranch;
  }

  std::vector< std::vector<DirectedGraph::Edge> > edges ( mGraph.numDesc( aVertex ) - 1 );
    
  for ( int i = 0; i < mGraph.numDesc( aVertex ) - 1 ; ++i )
    edges[i] = aEdges;
  

  for ( int i = 0; i < mGraph.numDesc( aVertex ) ; ++i )
  {    
    TrackBp::EdgeData data = mGraph.edge( aVertex, desc[i] );

    if ( i == 0 )
    {
      aEdges.push_back( DirectedGraph::Edge( aVertex, desc[i], data ));
      currentBranch = extractBranchesREC( desc[i], currentBranch, aEdges, aVisitedVertices, aBranches );
    }
    else
    {
      edges[i - 1].push_back( DirectedGraph::Edge( aVertex, desc[i], data ));
      currentBranch = extractBranchesREC( desc[i], currentBranch + 1, edges[i - 1], aVisitedVertices, aBranches );
    }
    
  }
  return currentBranch;
}