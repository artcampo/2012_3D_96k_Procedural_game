#include  "GameBall/GameAssets/TrackOutPiecesBalancers.hpp"

namespace StoredPiecesBalancers {

  DirectedGraph::PatternOut* patternOutZigZagDown( int aId )
  { 
    using namespace General;
    using namespace DirectedGraph;

    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );

    PatternOutNode* sn3 = new PatternOutNode( GeomNode( 1, 0, 0 ), tangent, normal );    
    PatternOutNode* sn2 = new PatternOutNode( GeomNode( 0, 0.6, 0.2, 0,0,0, false, true, true ), sn3 );     
    PatternOutNode* sn1 = new PatternOutNode( GeomNode( 1, 0.3, -0.2, 0,0,0, false, true, true ), sn2 );         
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0 ), tangent, normal, sn1 );     

    sn3->mTanNorUnmodified = false;
    sn0->mTanNorUnmodified = false;

    return new PatternOut(  sn0, aId, 
                            subsMinDistance, 5000, 
                            subsMaxAzimuth, 180, 
                            0, subsMaxPolar );    
  }  


  DirectedGraph::PatternOut* patternOutZigZagUp( int aId )
  { 
    using namespace General;
    using namespace DirectedGraph;
    
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );

    PatternOutNode* sn3 = new PatternOutNode( GeomNode( 1, 0, 0 ), tangent, normal );    
    PatternOutNode* sn2 = new PatternOutNode( GeomNode( 0, 0.6, 0.2, 0,0,0, false, true, true ), sn3 );     
    PatternOutNode* sn1 = new PatternOutNode( GeomNode( 1, 0.3, -0.2, 0,0,0, false, true, true ), sn2 );         
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0 ), tangent, normal, sn1 );     

    sn3->mTanNorUnmodified = false;
    sn0->mTanNorUnmodified = false;

    return new PatternOut(  sn0, aId, 
                            subsMinDistance, 5000, 
                            0, subsMinAzimuth, 
                            0, subsMaxPolar );    
  }  

  DirectedGraph::PatternOut* patternOutTanAbsorberBeg  ( int aId )
  {
    using namespace General;
    using namespace DirectedGraph;
    
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );

    PatternOutNode* sn3 = new PatternOutNode( GeomNode( 1, 0, 0 ), tangent, normal);
    PatternOutNode* sn1 = new PatternOutNode( GeomNode::geomNodeAbsorberBeg(), tangent, normal, sn3 );         
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0 ), tangent, normal, sn1);     

    return new PatternOut(  sn0, aId, 
                            General::tanAbsoberDistance*3.0f, 5000, 
                            subsMinAzimuth, subsMaxAzimuth, 
                            subsMaxPolar - 1, 360,
                            true, false );    
  }

DirectedGraph::PatternOut* patternOutTanAbsorberEnd  ( int aId )
  {
    using namespace General;
    using namespace DirectedGraph;
    
    D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
    D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );

    PatternOutNode* sn3 = new PatternOutNode( GeomNode( 1, 0, 0 ), tangent, normal);
    PatternOutNode* sn1 = new PatternOutNode( GeomNode::geomNodeAbsorberEnd(), tangent, normal, sn3 );         
    PatternOutNode* sn0 = new PatternOutNode( GeomNode( 0, 0, 0 ), tangent, normal, sn1);     

    return new PatternOut(  sn0, aId, 
                            General::tanAbsoberDistance*3.0f, 5000, 
                            subsMinAzimuth, subsMaxAzimuth, 
                            subsMaxPolar - 1, 360,
                            false, true );    
  }


};  // end namespace StoredPiecesBalancers