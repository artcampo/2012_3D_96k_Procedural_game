#include  "GameBall/GameAssets/TrackOutPiecesIO.hpp"

namespace TrackPiecesIO{
  DirectedGraph::PatternOut* generatePatternOut ( const DirectedGraph::StoredDirGraph& aStoredDirGraph,
                                   const float aSubsMinLength, 
                                   const float aSubsMaxLength,
                                   const int aOutPatternId,
                                   const float aMinAzimut,
                                   const float aMaxAzimut)
{
  using namespace DirectedGraph;
  PatternOut* pattern;

  int n = aStoredDirGraph.n;

  std::vector<int>  indexNumEdges   ( n ); 
  std::vector<bool> finished        ( n ); 
  std::vector<PatternOutNode*> node ( n ); 

  int indexE = 0;
  for ( int i = 0; i < n ; ++i )
  {
    finished[i]       = false;
    indexNumEdges[i]  = indexE;
    indexE+= aStoredDirGraph.StoredNumE[i];
  }

  int numFinished = 0;
  while ( numFinished != n )
  {
    bool  currentCanBeCreated = false;
    int i = n - 1;

    while (!currentCanBeCreated && i >= 0 )
    {
      if ( finished[i] )
        --i;    // skip already made ones
      else
      {
        bool  allDescendantsCreated = true;
        int index = indexNumEdges[i];
        for ( int j = 0; j < aStoredDirGraph.StoredNumE[i] ; ++j )
        {
          int descendant = aStoredDirGraph.StoredE[index + j];
          if ( !finished[descendant] )
            allDescendantsCreated = false;
        }

        if ( allDescendantsCreated )
          currentCanBeCreated = true;
        else
          --i;
      }


    }// while (!currentCanBeCreated

    //  i holds the one to be created
    finished[i] = true;
    ++numFinished;

    if ( aStoredDirGraph.StoredNumE[i] == 0 )
    {
      node[i] = new PatternOutNode( GeomNode( aStoredDirGraph.StoredV[i].x, 
                                              aStoredDirGraph.StoredV[i].z,
                                              aStoredDirGraph.StoredV[i].y ), 
                                              D3DXVECTOR3( 1, 0, 0 ),
                                              D3DXVECTOR3( 0, 1, 0 ) );
    }
    else
    {
      //  Create general one, but treat appart the root (for tangents)
      std::vector<PatternOutNode*> desc;
      int index = indexNumEdges[i];
      for ( int j = 0; j < aStoredDirGraph.StoredNumE[i] ; ++j )
      {
        int descendant = aStoredDirGraph.StoredE[index + j];
        desc.push_back( node[descendant] );
      }

      if ( i != 0 )
      {
        node[i] = new PatternOutNode( GeomNode( aStoredDirGraph.StoredV[i].x, 
                                                aStoredDirGraph.StoredV[i].z,
                                                aStoredDirGraph.StoredV[i].y ), 
                                                desc );
      }
      else
      {
        node[i] = new PatternOutNode( GeomNode( aStoredDirGraph.StoredV[i].x, 
                                                aStoredDirGraph.StoredV[i].z,
                                                aStoredDirGraph.StoredV[i].y ),                                                  
                                                D3DXVECTOR3( 1, 0, 0 ),
                                                D3DXVECTOR3( 0, 1, 0 ),
                                                desc);
      }

    }
    //  the node[i] has been created

  } //while ( numFinished != n )

  pattern = new PatternOut( node[0], aOutPatternId, aSubsMinLength, aSubsMaxLength, aMinAzimut, aMaxAzimut  );
  return pattern;
}

};  //end namespace TrackPiecesIO{