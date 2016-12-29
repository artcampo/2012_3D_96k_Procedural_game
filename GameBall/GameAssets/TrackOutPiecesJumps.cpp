#include  "GameBall/GameAssets/TrackOutPiecesJumps.hpp"

namespace StoredPiecesJumps {

  DirectedGraph::PatternOut* patternOutRamp( int aId )
  { 
    
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );
    using namespace DirectedGraph;

    PatternOutNode* sn3 = new PatternOutNode( GeomNode( 1.0f, 0, 0), 
                                              tangent, normal );
    PatternOutNode* sn2 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              Ramp::landEnd + 2*General::paddingLength, 0, 0 ), 
                                              tangent, normal, sn3 );
    PatternOutNode* sn1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                                        Ramp::landEnd + General::paddingLength, 0, 0 ), 
                                              tangent, normal, sn2, false, TrackBp::eEdgeRamp );
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0, General::paddingLength, 0, 0), 
                                              tangent, normal, sn1, false );     

    PatternOutNode* sn0pad = new PatternOutNode( GeomNode( 0, 0, 0 ), 
                                              tangent, normal, sn0 );     

    return new PatternOut(  sn0pad, aId, 
                            Ramp::subsMinDistance, 5000 );    
  }  


  DirectedGraph::PatternOut* patternOutNineTurn( int aId )
  { 
    
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );
    D3DXVECTOR3 bitangent= D3DXVECTOR3( 0, 0, 1 );
    using namespace DirectedGraph;

    PatternOutNode* sn3 = new PatternOutNode( GeomNode( 1.0f, 0, 0), 
                                              tangent, normal );

    PatternOutNode* sn2 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              450, 0, 0 ), 
                                              tangent, normal, sn3 );
    PatternOutNode* sn1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              50,0, 0 ), 
                                              tangent, normal, sn2, false, TrackBp::eEdgeNineTurn );
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0), 
                                              tangent, normal, sn1, false );     

    //return new PatternOut( sn0, aId, NineTurn::subsMinDistance, 5000 );    
    return new PatternOut( sn0, aId, 450, 5000,80, 100 );    
  }  

  DirectedGraph::PatternOut* patternOutOne80Turn    ( int aId )
  { 
    
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );
    D3DXVECTOR3 bitangent= D3DXVECTOR3( 0, 0, 1 );
    using namespace DirectedGraph;

    PatternOutNode* sn2 = new PatternOutNode( GeomNode( 1.0f, 0, 0), 
                                              tangent, normal );
    PatternOutNode* sn1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              NineTurn::outEndT, NineTurn::outEndB, 0 ), 
                                              bitangent, normal, sn2, false, TrackBp::eEdgeNineTurn );
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0), 
                                              tangent, normal, sn1, false );     

    return new PatternOut( sn0, aId, Ramp::subsMinDistance, 5000 );    
  }  

  DirectedGraph::PatternOut* patternOutLoop         ( int aId )
  { 
    
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );
    D3DXVECTOR3 bitangent= D3DXVECTOR3( 0, 0, 1 );
    using namespace DirectedGraph;

    PatternOutNode* sn2 = new PatternOutNode( GeomNode( 1.0f, 0, 0), 
                                              tangent, normal );
    PatternOutNode* sn1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              Loop::outLength, Loop::outWidthC, 0 ), 
                                              //bitangent, normal, sn2, false, TrackBp::eEdgeLoop );
                                              tangent, normal, sn2, false, TrackBp::eEdgeLoop );
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0), 
                                              tangent, normal, sn1, false );     

    return new PatternOut( sn0, aId, Loop::subsMinDistance, 5000 );    
  }  

  DirectedGraph::PatternOut* patternOutLateral      ( int aId )
  { 
    
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );
    D3DXVECTOR3 bitangent= D3DXVECTOR3( 0, 0, 1 );
    using namespace DirectedGraph;

    PatternOutNode* sn2 = new PatternOutNode( GeomNode( 1.0f, 0, 0), 
                                              tangent, normal );
    PatternOutNode* sn1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              NineTurn::outEndT, NineTurn::outEndB, 0 ), 
                                              bitangent, normal, sn2, false, TrackBp::eEdgeNineTurn );
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0), 
                                              tangent, normal, sn1, false );     

    return new PatternOut( sn0, aId, Ramp::subsMinDistance, 5000, DxMath::degToRad(85), DxMath::degToRad(95) );    
  }  

  DirectedGraph::PatternOut* patternOutLateralCross ( int aId )
  { 
    
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );
    D3DXVECTOR3 bitangent= D3DXVECTOR3( 0, 0, 1 );
    using namespace DirectedGraph;

    PatternOutNode* sn2 = new PatternOutNode( GeomNode( 1.0f, 0, 0), 
                                              tangent, normal );
    PatternOutNode* sn1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              NineTurn::outEndT, NineTurn::outEndB, 0 ), 
                                              bitangent, normal, sn2, false, TrackBp::eEdgeNineTurn );
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0), 
                                              tangent, normal, sn1, false );     

    return new PatternOut( sn0, aId, Ramp::subsMinDistance, 5000 );    
  }  

  DirectedGraph::PatternOut* patternOutTwoWayJump   ( int aId )
  { 
    using namespace StoredPiecesJumps::TwoWayJump;
    using namespace StoredPiecesJumps::General;
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );
    D3DXVECTOR3 bitangent= D3DXVECTOR3( 0, 0, 1 );
    using namespace DirectedGraph;

    PatternOutNode* sn7 = new PatternOutNode( GeomNode( 1, 0, 0), 
                                              tangent, normal );

    PatternOutNode* sb6 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              -20, 0, 0, GeomNode::eAbsCoordEnd ), 
                                              tangent, normal, sn7 );

    //  Branch 2
    PatternOutNode* sa5 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              -(e1Length + 10), -10, 0, GeomNode::eAbsCoordEnd ), 
                                              DxMath::unitVector( D3DXVECTOR3(2,0,1), D3DXVECTOR3(0,0,0) ),
                                              normal, sb6 );

    PatternOutNode* sa4 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              -(e1Length + paddingLength + 10), -30, -25, GeomNode::eAbsCoordEnd ), 
                                              DxMath::unitVector( D3DXVECTOR3(2,1,0), D3DXVECTOR3(0,0,0) ), 
                                              normal, sa5 );

    PatternOutNode* sa3 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              -(e2Length + e1Length + paddingLength), -10, b2Height, GeomNode::eAbsCoordEnd ), 
                                              DxMath::unitVector( D3DXVECTOR3(2,0,-1), D3DXVECTOR3(0,0,0) ), normal, sa4 );

    PatternOutNode* sa2 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              240, 0, b2Height ), 
                                              tangent, normal, sa3 );

    PatternOutNode* sa1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                               paddingLength + peLength, 0, b2Height ), 
                                              tangent, normal, sa2, false, TrackBp::eEdgeTwoWayJump );

    //  Branch 1

    PatternOutNode* sb5 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              -(paddingLength + e1Length), 0, 0, GeomNode::eAbsCoordEnd ), 
                                              tangent, normal, sb6, false );

    PatternOutNode* sb4 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              -(e2Length + paddingLength + e1Length), 0, b1Height, GeomNode::eAbsCoordEnd ), 
                                              tangent, normal, sb5, false );

    PatternOutNode* sb3 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              -(e2Length + 2*paddingLength + e1Length), 0, b1Height, GeomNode::eAbsCoordEnd ), 
                                              tangent, normal, sb4, false );

    PatternOutNode* sb2 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              2*paddingLength + peLength, 0, b1Height ), 
                                              tangent, normal, sb3, false ); 

    PatternOutNode* sb1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              paddingLength + peLength, 0, b1Height ), 
                                              tangent, normal, sb2, false, TrackBp::eEdgeTwoWayJump ); 

    std::vector<PatternOutNode*> dn1;
    dn1.push_back( sb1 );
    dn1.push_back( sa1 );

    PatternOutNode* sc1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              paddingLength, 0, 0 ), 
                                              tangent, normal, dn1, false ); 

    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0 ), 
                                              tangent, normal, sc1 );

    return new PatternOut( sn0, aId, subsMinDistance, 5000, 80, 100 ); 
  }  

  DirectedGraph::PatternOut* patternOutThreeWayJump ( int aId )
  { 
    return NULL;   
  }  

  DirectedGraph::PatternOut* patternOutTwister ( int aId )
  { 
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );
    D3DXVECTOR3 bitangent= D3DXVECTOR3( 0, 0, 1 );
    using namespace DirectedGraph;

    PatternOutNode* sn2 = new PatternOutNode( GeomNode( 1.0f, 0, 0), 
                                              tangent, normal, false, TrackBp::eEdgeTwister );    
    PatternOutNode* sn1 = new PatternOutNode( GeomNode::geomNodeAbsolute( 15, 0, 0 ),                                               
                                              tangent, normal, sn2, false  );
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0), 
                                              tangent, normal, sn1 );     

    //sn1->mTangentAndNormal = true;
    //sn2->mTangentAndNormal = true;


    return new PatternOut( sn0, aId, Twister::subsMinDistance, Twister::subsMaxDistance );
  }  

DirectedGraph::PatternOut* patternOutLevelCross ( int aId )
  { 
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );
    D3DXVECTOR3 bitangent= D3DXVECTOR3( 0, 0, 1 );
    D3DXVECTOR3 minusBitangent= D3DXVECTOR3( 0, 0, -1 );

    D3DXVECTOR3 normal_n1 = DxMath::normalizeVector( D3DXVECTOR3(0,1,1));
    D3DXVECTOR3 normal_n2 = DxMath::normalizeVector( D3DXVECTOR3(0,1,-1));
    using namespace DirectedGraph;
    
    PatternOutNode* sc2 = new PatternOutNode( GeomNode( 1, 0, 0), 
                                              tangent, normal );

    PatternOutNode* sc1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              -15, 0, 0, GeomNode::eAbsCoordEnd ), 
                                              tangent, normal, sc2 );  

    PatternOutNode* sb4 = new PatternOutNode( GeomNode( 0.5, 0.4, 0,
                                                        0, 0, LevelCross::lowLevelHeight), 
                                              tangent, normal, sc1 ); 

    PatternOutNode* sb3 = new PatternOutNode( GeomNode::geomNodeCenterAbs( 0, 20, LevelCross::lowLevelHeight ), 
                                              bitangent, normal, sb4 );  

    PatternOutNode* sb2 = new PatternOutNode( GeomNode::geomNodeCenterAbs( 0, -20, LevelCross::lowLevelHeight ), 
                                              bitangent, normal, sb3 );  

    PatternOutNode* sb1 = new PatternOutNode( GeomNode( 0.5, -0.4, 0,
                                                        0, 0, LevelCross::lowLevelHeight ), 
                                              bitangent, normal, sb2 );  

    PatternOutNode* sa4 = new PatternOutNode( GeomNode( 0.6, 0, 0), 
                                              tangent, normal, sc1 ); 

    PatternOutNode* sa3 = new PatternOutNode( GeomNode::geomNodeCenterAbs( 20, 0, 0 ), 
                                              tangent, normal, sa4 );  

    PatternOutNode* sa2 = new PatternOutNode( GeomNode::geomNodeCenterAbs( -20, 0, 0 ), 
                                              tangent, normal, sa3 );  


    PatternOutNode* sa1 = new PatternOutNode( GeomNode( 0.4, 0.4, 0 ), 
                                              minusBitangent, normal, sa2 );  

    std::vector<PatternOutNode*> dn1;
    dn1.push_back( sb1 );
    dn1.push_back( sa1 );

    PatternOutNode* sn1 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              15, 0, 0 ), 
                                              tangent, normal, dn1, false );  

    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0, 
                                              0, 0, 0 ), 
                                              tangent, normal, sn1 );  
    
    return new PatternOut( sn0, aId, LevelCross::subsMinDistance, LevelCross::subsMaxDistance );
  }  

  

};  // end namespace StoredPiecesJumps