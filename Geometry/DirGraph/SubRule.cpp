#include  "Geometry/DirGraph/SubRule.hpp"
#include  "Noise/RandomNumberGenerator.hpp"
#include  "Global.hpp"
#include  "GameBall/Track/TrackGraphCollision.hpp"

namespace DirectedGraph{

  float distance(  const DirectedGraph::DirGraph& aGraph,
                            const int aVid1, 
                            const int aVid2 )
  {

    TrackBp::Vertex v1 = aGraph.vertex( aVid1 );
    TrackBp::Vertex v2 = aGraph.vertex( aVid2 );

    D3DXVECTOR3 d = v1.pos - v2.pos;
    return D3DXVec3Length( &d );
  }

  DirectedGraph::RuleMatch
          matchPatternREC(  const DirectedGraph::DirGraph& aGraph,
                            DirectedGraph::PatternIn& aPatternIn,
                            DirectedGraph::PatternInNode const* aCurrentNode,
                            const int aStartingVertex,
                            const int aCurrentVertex,
                            DirectedGraph::PatternOutNode* aOutCurrentNode,
                            DirectedGraph::PatternOut* aOutcomingPattern )
  {

    DirectedGraph::RuleMatch m;
    m.matched = false;                

    if (  !(  aGraph.numPar ( aCurrentVertex ) == aCurrentNode->degIn &&
              aGraph.numDesc( aCurrentVertex ) == aCurrentNode->degOut) )
    {
      // no match for whole pattern      
    }    
    else
    {
      //local match
      if ( aCurrentNode->mDesc.size() == 0 )
      {
        //  last node reached      
        float distanceBegEnd = distance( aGraph, aStartingVertex, aCurrentVertex );
        TrackBp::Vertex vBeg = aGraph.vertex( aStartingVertex );
        TrackBp::Vertex vEnd = aGraph.vertex( aCurrentVertex );

        if ( distanceBegEnd > aPatternIn.mMinLength
            && aOutcomingPattern->passesRequirements( distanceBegEnd, vBeg, vEnd ))
        {
          m.matched           = true;                                
          m.lastVertex        = aCurrentVertex;
                    
          aOutcomingPattern->mEndNode->mVertexInGraphCreated = true;
          aOutcomingPattern->mEndNode->mVertexInGraphId      = m.lastVertex;
        }
        //  terminal node reached
        else
        {                
        }
      }
      else
      {
        //  test this node collisions

        //  test branches's nodes        
        bool allSubranchesMatched = true;
        DirectedGraph::RuleMatch matchBranch;                       
        std::vector<int> v = aGraph.descendants( aCurrentVertex );
        for ( int i = 0; allSubranchesMatched && i < aCurrentNode->mDesc.size() ; ++i )
        {
          
          matchBranch = matchPatternREC(  aGraph,
                                aPatternIn,
                                &aCurrentNode->mDesc[i],
                                aStartingVertex,
                                v[i],
                                aOutCurrentNode->mDesc[i],
                                aOutcomingPattern); 
          allSubranchesMatched = (matchBranch.matched == true);
        }
        if ( allSubranchesMatched )
        {
          m = matchBranch;
        }
        
      }
    }
  
    return m;
  }

  DirectedGraph::RuleMatch
          matchPattern( const DirectedGraph::DirGraph& aGraph, 
                        DirectedGraph::PatternIn& aPatternIn,
                        DirectedGraph::PatternOut* aOutPatternNode,
                        const int aStartingVertex )
  {    
    DirectedGraph::PatternInNode* currentNode = &aPatternIn.mMatch;
    int currentVertex                         = aStartingVertex;

    aOutPatternNode->mRoot->mVertexInGraphCreated = true;
    aOutPatternNode->mRoot->mVertexInGraphId      = aStartingVertex;

    DirectedGraph::RuleMatch m;
    m  = matchPatternREC(   aGraph, 
                            aPatternIn,
                            currentNode, 
                            aStartingVertex, 
                            currentVertex,
                            aOutPatternNode->mRoot,
                            aOutPatternNode);
    m.outcomingPattern = aOutPatternNode->mOutPatternId;
    return m;
  }

  int    applyRuleCreateNodeRec(  DirectedGraph::DirGraph&        aGraph,
                                  DirectedGraph::PatternOut*      aOutPattern,
                                  DirectedGraph::PatternOutNode*  aOutPatternNode,
                                  Rand::RandGen&                  aRandGen,
                                  const TrackBp::Vertex&          aVerBeg,
                                  const int aStartVertex,
                                  const int aEndVertex,
                                  const DxMath::TnbBasis& aBasisAB,                                  
                                  const float aLength )
  {    
       
    TrackBp::Vertex ve  = aGraph.vertex( aEndVertex );    
    D3DXVECTOR3 abVector = ve.pos - aVerBeg.pos;
   
    TrackBp::Vertex n = aOutPatternNode->pos( aBasisAB, abVector, aVerBeg.pos, ve.pos, aVerBeg.tan, ve.tan );

    DxMath::limitAzimut( n.nor, DxMath::degToRad(0) );

    bool  isMutable = aOutPatternNode->mIsMutable; 
    bool recurseChildren = false;
    int currentVertex;
    if ( aOutPatternNode->mVertexInGraphCreated )
    {
      currentVertex = aOutPatternNode->mVertexInGraphId;
      TrackBp::Vertex v = aGraph.vertex( currentVertex );
      if ( !DxMath::equality( v.pos, n.pos, 0.1f) )
      {
        *Global::mFileDebugTrack << "PANIK: new vertex::position mistmatch" << std::endl;
        *Global::mFileDebugTrack << "int:"<< v.pos.x << "," <<v.pos.y<<","<<v.pos.z<<"  ";
        *Global::mFileDebugTrack << "new:"<< n.pos.x << "," <<n.pos.y<<","<<n.pos.z<<std::endl;  
        int b = 3;
        n.pos = v.pos;

      }
      
      if ( !aOutPatternNode->mTanNorUnmodified )
        aGraph.setVertex( currentVertex, n, isMutable );

      if ( currentVertex == aStartVertex )
        recurseChildren = true;
    }
    else
    {
      currentVertex = aGraph.newNode( n, isMutable );
      aOutPatternNode->mVertexInGraphCreated  = true;
      aOutPatternNode->mVertexInGraphId       = currentVertex;      
      recurseChildren = true;
    }

  
    if ( aOutPatternNode->mDesc.size() > 0  )
    {
      if ( recurseChildren )
      {
        //  Node has children, create them
        aOutPatternNode->mEdgesCreated = true;
        std::vector<int> descendants ( aOutPatternNode->mDesc.size() );
        for ( int i = 0; i < aOutPatternNode->mDesc.size() ; ++i )
        {      
          DirectedGraph::PatternOutNode* currentSubs = aOutPatternNode->mDesc[i];
          descendants[i]= applyRuleCreateNodeRec( aGraph,
                                aOutPattern,
                                currentSubs,
                                aRandGen,
                                aVerBeg,
                                aStartVertex,
                                aEndVertex,
                                aBasisAB,
                                aLength);          
        }    

        for ( int i = 0; i < aOutPatternNode->mDesc.size() ; ++i )
        {          
          TrackBp::EdgeData   edgeData  ( 0, aOutPatternNode->mDesc[i]->mEdgeJumpId, aRandGen.randLong()  );
          DirectedGraph::Edge edge      ( currentVertex, descendants[i], edgeData );
          int id = aGraph.newEdge( currentVertex, descendants[i], edgeData );          
          aOutPatternNode->mEdges[i] = edge;

          {
            *Global::mFileDebugTracks << std::endl;
            *Global::mFileDebugTracks << "New edge " << id;

            *Global::mFileDebugTracks << ", with ";
            //aOutPatternNode->mGeomRules.logWrite( Global::mFileDebugTracks );
            *Global::mFileDebugTracks << " [ vb: " << currentVertex;
            TrackBp::Vertex v = aGraph.vertex( currentVertex );
            *Global::mFileDebugTracks << " ("<< v.pos.x<<","<<v.pos.y<<","<<v.pos.z<<") ";

            *Global::mFileDebugTracks << " to ve: " << descendants[i];
            TrackBp::Vertex ve = aGraph.vertex( descendants[i] );
            *Global::mFileDebugTracks << " ("<< ve.pos.x<<","<<ve.pos.y<<","<<ve.pos.z<<")  ]";

            //*Global::mFileDebugTracks << std::endl;
          }
        }
      }
    }
    else
    {
      //  Vertex is terminal, create last vertex
      int a  = 2;
    }

    return currentVertex;
  }

  void    applyRule(  DirectedGraph::DirGraph& aGraph,
                      DirectedGraph::PatternOut* aOutPattern,
                      Rand::RandGen&                  aRandGen,
                      const int aStartingVertex,
                      const int aEndVertex )
  {
    TrackBp::EdgeData e = aGraph.edge( aStartingVertex, aEndVertex );
    aOutPattern->mOriginalDesiredLength = e.mDesiredLength;
    aGraph.deleteDescendants( aStartingVertex );
    aGraph.deleteParents    ( aEndVertex );
    
    bool finished = false;

    TrackBp::Vertex vb = aGraph.vertex( aStartingVertex );
    TrackBp::Vertex ve = aGraph.vertex( aEndVertex );

    float l   = DxMath::distance          ( ve.pos, vb.pos );

    DxMath::TnbBasis basis;
    basis.tan = DxMath::unitVector        ( ve.pos, vb.pos );
    basis.nor = DxMath::alignToTangents   ( basis.tan, D3DXVECTOR3(1,0,0), D3DXVECTOR3(0,1,0) );
    basis.bin = DxMath::thirdVectorBasis  ( basis.tan, basis.nor );
    DxMath::clean( basis.bin, 0.0001f );
    
    DirectedGraph::PatternOutNode* currentSubs = aOutPattern->mRoot;

    int n0 = applyRuleCreateNodeRec( aGraph,
                            aOutPattern,
                            currentSubs,
                            aRandGen,
                            vb,
                            aStartingVertex,
                            aEndVertex,
                            basis,
                            l);

    //aGraph.newEdge( aStartingVertex, n0, 0 );    
    //aOutPattern->mVertexAEdge = Edge( aStartingVertex, n0, TrackBp::EdgeData( 0 ) );   
  }

  void    balanceBranchesLengths( DirectedGraph::DirGraph& aGraph,
                                  DirectedGraph::PatternOut* aOutPattern )
  {
    // TODO: WHY NO CODE?!?!?!
  }

  /*
    Resets the created Ids in graph.
  */
  void  initPatternOut ( DirectedGraph::PatternOutNode* aPattern )
  {
    aPattern->mVertexInGraphCreated = false;

    for ( int i = 0; i < aPattern->mDesc.size() ; ++i )
      initPatternOut( aPattern->mDesc[i] );

  }  

  bool testPieceInGraph(  const DirectedGraph::DirGraph& aGraph,
                          const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration,
                          const TrackGraphCollision::CollisionData& aCollisionData,                          
                          DirectedGraph::PatternOut* aOutPattern,                                                    
                          const int aStartingVertex,
                          const int aEndVertex )
  {
    TrackBp::Vertex vb = aGraph.vertex( aStartingVertex );
    TrackBp::Vertex ve = aGraph.vertex( aEndVertex );
    DxMath::TnbBasis basis;
    basis.tan = DxMath::unitVector        ( ve.pos, vb.pos );
    basis.nor = DxMath::alignToTangents   ( basis.tan, D3DXVECTOR3(1,0,0), D3DXVECTOR3(0,1,0) );
    basis.bin = DxMath::thirdVectorBasis  ( basis.tan, basis.nor );    
    DxMath::cleanZeroes( basis.bin );
    DxMath::cleanZeroes( basis.nor );
      
    return testPieceInGraphRec( 
                            aGraph,
                            aCollisionConfiguration,
                            aCollisionData,                            
                            aOutPattern->mRoot,                            
                            aStartingVertex,
                            aEndVertex,
                            basis);
  }


  bool   testPieceInGraphRec( const DirectedGraph::DirGraph&        aGraph,
                              const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration,
                              const TrackGraphCollision::CollisionData& aCollisionData,                              
                              DirectedGraph::PatternOutNode*  aOutPatternNode,
                              const int aStartVertex,
                              const int aEndVertex,
                              const DxMath::TnbBasis& aBasisAB )
  {    
    
    TrackBp::Vertex vb  = aGraph.vertex( aStartVertex );        
    TrackBp::Vertex ve  = aGraph.vertex( aEndVertex );        
    D3DXVECTOR3 abVector = ve.pos - vb.pos;
   
    TrackBp::Vertex currentVertexOut = aOutPatternNode->pos( aBasisAB, abVector, vb.pos, ve.pos, vb.tan, ve.tan );
    bool  valid = true; 

    if ( aOutPatternNode->mDesc.size() > 0  )
    {   
      //  check edges from currentVertexOut      
      for ( int i = 0; i < aOutPatternNode->mDesc.size() ; ++i )
      {      
        DirectedGraph::PatternOutNode* currentSubs = aOutPatternNode->mDesc[i];
        TrackBp::Vertex currentVertexOut2 = currentSubs->pos( aBasisAB, abVector, vb.pos, ve.pos, vb.tan, ve.tan );       
        
        valid &= TrackGraphCollision::pieceAgainstGraph(  aCollisionConfiguration,
                                                          aCollisionData,
                                                          currentVertexOut.pos, 
                                                          currentVertexOut2.pos,
                                                          aStartVertex,
                                                          aEndVertex );
        if (!valid)
        {
        }
        if (!valid) return valid;
        
      }    

      for ( int i = 0; i < aOutPatternNode->mDesc.size() ; ++i )
      {          
        DirectedGraph::PatternOutNode* currentSubs = aOutPatternNode->mDesc[i];
        valid &= testPieceInGraphRec( 
                              aGraph,
                              aCollisionConfiguration,
                              aCollisionData,
                              currentSubs,
                              aStartVertex,
                              aEndVertex,
                              aBasisAB);   
        if (!valid) return valid;
      }
      
    }

    return valid;
  }


}