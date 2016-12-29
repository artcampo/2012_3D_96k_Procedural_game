#include  "GameBall/Track.hpp"
#include  "GameBall/Game.hpp"
#include <map>
#include "Project_Dependant/ConditionalCompilation.hpp"
#include  "GameBall/GameAssets/TrackOutPiecesBalancers.hpp"

const float Track::mArchPlacementWidthReduction = 0.5f;
const float Track::mEntityMaxAzimuth            = 135;
const float Track::mEntityMinAzimuth            = 80;

Track::~Track()
{
  for ( int i = 0; i < mTrackPreview.size() ; ++i )
    delete mTrackPreview[i];
  delete mLog;
  if ( mEdgePathsKdd != NULL )
    delete mEdgePathsKdd;
}

Track::Track (  TrackBp::TrackDescription& aDesc,
                const std::vector<D3DXVECTOR3>& mBezierSliceBorder,
                const std::vector<D3DXVECTOR3>& mBezierSliceNoBorder,                
                const unsigned long aTrackSeed,
                const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration )              
{
  mRand   = Rand::RandGen( aTrackSeed );
  mDesc   = aDesc;
  mCollisionConfiguration = aCollisionConfiguration;
  float inverse = 1.0 / ( 50.0f );
  mSliceMultiplier = D3DXVECTOR3( inverse, inverse, inverse )*aDesc.trackWidth;

#ifndef _DPBP_DISABLE_BORDER_SLICE
  mSliceBorder    = mBezierSliceBorder;
  mSliceNoBorder  = mBezierSliceNoBorder;
#endif
#ifdef _DPBP_DISABLE_BORDER_SLICE
  mSliceBorder    = mBezierSliceNoBorder;
  mSliceNoBorder  = mBezierSliceNoBorder;
#endif

  switch ( aDesc.layout){
    case TrackBp::eTrackLayoutPlane:
      initGraphStraightTrack( aDesc );
      break;
    case TrackBp::eTrackLayoutFall:
      initGraphFallingTrack( aDesc );
      break;
    case TrackBp::eTrackLayoutSpiral:
      initGraphSpiralTrack( aDesc );
      break;
  };


  
  //
  
  initPatternIns();

  mLog = new Log();
  mEdgePathsKdd = NULL;
  mTrackPreview.resize(1);
  mTrackPreview[0] = NULL;
  mPanickExit = false;
}

void Track::initGraphStraightTrack( const TrackBp::TrackDescription& aDesc )
{
  TrackBp::Vertex fake;

  float padStart = 50;

  D3DXVECTOR3 startVertex = D3DXVECTOR3(0,0,0);
  D3DXVECTOR3 padVertex   = D3DXVECTOR3(padStart,-0.25f,0);

  //D3DXVECTOR3 endVertex   = D3DXVECTOR3(mDesc.totalLength,-mDesc.totalLength*0.5,0); 
  D3DXVECTOR3 endVertex = D3DXVECTOR3(mDesc.totalLength,0,0);  
  D3DXVECTOR3 tan = DxMath::unitVector( endVertex, padVertex );
  D3DXVECTOR3 nor = D3DXVECTOR3(0,1,0);

  TrackBp::Vertex start ( startVertex, D3DXVECTOR3(1,0,0), nor );  
  TrackBp::Vertex startA( padVertex, tan, nor );  
  TrackBp::Vertex end   ( endVertex, tan, nor );

  int startN  = mGraph.newNode( start, false ); 
  int startNA = mGraph.newNode( startA, true ); 
  int endN    = mGraph.newNode( end, true );     
  mFakeNodeA  = mGraph.newNode( start, false ); 
  mFakeNodeB  = mGraph.newNode( end, false ); 
  mStartNodeId = startN;
  mEndNodeId   = endN;

  mGraph.setRoot( startN );

  mGraph.newEdge( mFakeNodeA, startN, TrackBp::EdgeData(0, TrackBp::eEdgeNormal, 0 ) );
  mGraph.newEdge( startN, startNA,    TrackBp::EdgeData(padStart, TrackBp::eEdgeNormal, 0 ) );
  mGraph.newEdge( startNA, endN,      TrackBp::EdgeData(mDesc.totalLength - padStart, TrackBp::eEdgeNormal, 0  ) );
  mGraph.newEdge( endN, mFakeNodeB,   TrackBp::EdgeData(0, TrackBp::eEdgeNormal, 0 ) );
}


void Track::initGraphFallingTrack( const TrackBp::TrackDescription& aDesc )
{
  TrackBp::Vertex fake;

  float padStart = 50;

  D3DXVECTOR3 startVertex = D3DXVECTOR3(0,0,0);
  D3DXVECTOR3 padVertex   = D3DXVECTOR3(padStart,-0.25f,0);

  D3DXVECTOR3 endVertex   = D3DXVECTOR3(mDesc.totalLength,-mDesc.totalLength*0.5,0);   
  D3DXVECTOR3 tan = DxMath::unitVector( endVertex, padVertex );
  D3DXVECTOR3 nor = D3DXVECTOR3(0,1,0);

  TrackBp::Vertex start ( startVertex, D3DXVECTOR3(1,0,0), nor );  
  TrackBp::Vertex startA( padVertex, tan, nor );  
  TrackBp::Vertex end   ( endVertex, tan, nor );

  int startN  = mGraph.newNode( start, false ); 
  int startNA = mGraph.newNode( startA, true ); 
  int endN    = mGraph.newNode( end, true );     
  mFakeNodeA  = mGraph.newNode( start, false ); 
  mFakeNodeB  = mGraph.newNode( end, false ); 
  mStartNodeId = startN;
  mEndNodeId   = endN;

  mGraph.setRoot( startN );

  mGraph.newEdge( mFakeNodeA, startN, TrackBp::EdgeData(0, TrackBp::eEdgeNormal, 0 ) );
  mGraph.newEdge( startN, startNA,    TrackBp::EdgeData(padStart, TrackBp::eEdgeNormal, 0 ) );
  mGraph.newEdge( startNA, endN,      TrackBp::EdgeData(mDesc.totalLength - padStart, TrackBp::eEdgeNormal, 0  ) );
  mGraph.newEdge( endN, mFakeNodeB,   TrackBp::EdgeData(0, TrackBp::eEdgeNormal, 0 ) );  
}

void Track::initGraphSpiralTrack( TrackBp::TrackDescription& aDesc )
{
  TrackBp::Vertex fake;

  float padStart  = 50;
  int   numPoints = 10;
  float radius = 15*mDesc.totalLength / ( 2.0f*DxMath::msPI*6);

  D3DXVECTOR3 center      = D3DXVECTOR3(mDesc.totalLength*0.5f,0,0);
  D3DXVECTOR3 startVertex = D3DXVECTOR3(radius,0,-padStart) + center;
  D3DXVECTOR3 padVertex   = D3DXVECTOR3(radius,0,0) + center;
  aDesc.layoutSpiralInitialBallPosition = padVertex + D3DXVECTOR3(0,1,-1);

  TrackBp::Vertex start   ( startVertex,  D3DXVECTOR3(0,0,1), D3DXVECTOR3(0,1,0) );  
  TrackBp::Vertex startPad( padVertex,    D3DXVECTOR3(0,0,1), D3DXVECTOR3(0,1,0) );  

  int startN    = mGraph.newNode( start, false ); 
  int startPadN = mGraph.newNode( startPad, true );   
  mFakeNodeA    = mGraph.newNode( start, false ); 
  
  mStartNodeId = startN;  

  mGraph.setRoot( startN );
  mGraph.newEdge( mFakeNodeA, startN,   TrackBp::EdgeData(0, TrackBp::eEdgeNormal, 0 ) );
  mGraph.newEdge( startN, startPadN,    TrackBp::EdgeData(padStart, TrackBp::eEdgeNormal, 0 ) );

  int previousNode = startPadN;
  
  for ( int i = 0; i < numPoints ; ++i )
  {
    const float angle   = (i + 1)*DxMath::degToRad(100);
    const float height  = i*40;
    D3DXVECTOR3 v = D3DXVECTOR3(radius*cos( angle ), -height, radius*sin( angle )) + center;
    const float angleTangent = angle + DxMath::degToRad(90);
    D3DXVECTOR3 t = D3DXVECTOR3(cos( angleTangent ), 0, sin( angleTangent ));
    TrackBp::Vertex currentVertex( v, t, D3DXVECTOR3(0,1,0) );   
    int currentNode = mGraph.newNode( currentVertex, i != (numPoints - 1) );   
    mGraph.newEdge( previousNode, currentNode, TrackBp::EdgeData(mDesc.totalLength/float(numPoints), TrackBp::eEdgeNormal, 0  ) );
    if ( i == (numPoints - 1) ) 
    {
      mFakeNodeB = mGraph.newNode( currentVertex, false ); 
      mGraph.newEdge( currentNode, mFakeNodeB,   TrackBp::EdgeData(0, TrackBp::eEdgeNormal, 0 ) );  
    }
    previousNode  =   currentNode;
    radius        -=  15;
  }

  mEndNodeId    = previousNode;
  
}


void   Track::generateLoftSurface()
{  
  //  Compute intersections between edges starting or ending
  //  at same vertex

  for (int i = 0; i < mPath.size(); ++i)
  {
    ++mTrackHisto.mNumEdges;
    mTrackHisto.inVertexDegree( mPath[i]->numParents() );
    mTrackHisto.outVertexDegree( mPath[i]->numDescendants() );

#ifndef _DEBUG_DISABLE_INTERSECTION
    if ( mPath[i]->numParents() == 2 )
    {
      TrackPath*  par1 = mPath[i]->parent(0);
      TrackPath*  par2 = mPath[i]->parent(1);
      LoftBoolean::LoftIntersection intersection = 
          LoftBoolean::loftIntersectEnd(  par1->loftInfoLastEdge(),
                                          par2->loftInfoLastEdge() );
      par1->setLoftIntersectLastEdge( intersection.surfaceA );
      par2->setLoftIntersectLastEdge( intersection.surfaceB );
    }

    if ( mPath[i]->numDescendants() == 2 )
    {
      TrackPath*  des1 = mPath[i]->descendant(0);
      TrackPath*  des2 = mPath[i]->descendant(1);
      LoftBoolean::LoftIntersection intersection = 
          LoftBoolean::loftIntersectStart(  des1->loftInfoFirstEdge(),
                                            des2->loftInfoFirstEdge() );
      des1->setLoftIntersectFirstEdge( intersection.surfaceA );
      des2->setLoftIntersectFirstEdge( intersection.surfaceB );
    }
#endif
    
  }

  //  Generate surface for each path
  for (int i = 0; i < mPath.size(); ++i)
    mPath[i]->generateLoftSurface();
}

std::vector<DxLodModel<VertPNTUV>*>   Track::model()       const
{
  std::vector<DxLodModel<VertPNTUV>*> m;
  for (int i = 0; i < mPath.size(); ++i)
  {
    if ( i == 55)
      int a = 2;
    std::vector<DxLodModel<VertPNTUV>*> p = mPath[i]->model();
    m.insert(m.end(), p.begin(), p.end());
  }
  return m;
}

std::vector<MeshCol*>           Track::meshColTri()  const
{
  std::vector<MeshCol*> ret;
  for (int i = 0; i < mPath.size(); ++i)
  {
    std::vector<MeshCol*> m = mPath[i]->meshColTri();
    ret.insert(ret.end(), m.begin(), m.end());
  }
  
  return ret;
}

void Track::initPatternIns()
{
  using namespace DirectedGraph;

  float shortSubsMinLength  = 150.0f;
  float shortSubsMaxLength  = 300.0f;
  
  float medSubsMinLength    = 300.0f;
  float medSubsMaxLength    = 600.0f;

  float longSubsMinLength   = 600.0f;
  float longSubsMaxLength   = 20000.0f;

  D3DXVECTOR3 tangent  = D3DXVECTOR3( 1, 0, 0 );
  D3DXVECTOR3 normal   = D3DXVECTOR3( 0, 1, 0 );

  //////////////////////////////
  //  In0
  PatternInNode nb( 1, 1 );
  PatternInNode na( 1, 1, nb );
  PatternIn rule1( na, 20.0f, 3000.0f ,0 );

  mPatternIns.push_back( rule1 ) ;
  mPatternInsOut.resize(1);


  //  Out pieces for in0
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutTanAbsorberBeg(0) );  
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutTanAbsorberEnd(1) );  
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutZigZagDown(2)  );
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutZigZagUp(3)    );  
  initPatternInsFall();
  initPatternInsPlane();
  writePieces();
}

void        Track::initPatternInsPlane()
{
/*
  mPatternInsOut[0].push_back (
    TrackPiecesIO::generatePatternOut( StoredPieces::twoLevel, 500, 5000, 20, 80, 100 ) );
*/
 
  mPatternInsOut[0].push_back (
    TrackPiecesIO::generatePatternOut( StoredPieces::twoLevel2, 500, 5000, 20 ) );

/*
  mPatternInsOut[0].push_back (
    TrackPiecesIO::generatePatternOut( StoredPieces::p1, 200, 5000, 10 ) );
  
  mPatternInsOut[0].push_back (
    TrackPiecesIO::generatePatternOut( StoredPieces::p2, 200, 5000, 11 ) );
  
  mPatternInsOut[0].push_back (
    TrackPiecesIO::generatePatternOut( StoredPieces::p3, 200, 5000, 12 ) );
  
  mPatternInsOut[0].push_back (
    TrackPiecesIO::generatePatternOut( StoredPieces::p4, 200, 5000, 13 ) );
  */
}

void        Track::initPatternInsFall()
{
/*
  mPatternInsOut[0].push_back (
    TrackPiecesIO::generatePatternOut( StoredPieces::m0, 500, 45000, 5 ) );
  /*
  mPatternInsOut[0].push_back (
    TrackPiecesIO::generatePatternOut( StoredPieces::m1, 500, 5000, 6 ) );
*/
}

void        Track::initPatternInsAbility()
{

  mPatternInsOut[0].push_back ( StoredPiecesJumps::patternOutRamp(4) );    
  mPatternInsOut[0].push_back ( StoredPiecesJumps::patternOutTwoWayJump(5) );
   
  //mPatternInsOut[0].push_back ( StoredPiecesJumps::patternOutLoop(7) );
  
  mPatternInsOut[0].push_back ( StoredPiecesJumps::patternOutTwister(8) );
  mPatternInsOut[0].push_back ( StoredPiecesJumps::patternOutLevelCross(9) );
  
  
}
/*
void Track::generateTrack ( const int aMaxGraphIterations )              
{
  // The track for tests is all done at init
  //  TODO: INIT TIME
  //mLog->mProf->start( std::string("Graph") );
  generateGraph( aMaxGraphIterations );
  mLog->mProf->end();
}*/

void Track::generateTrackFinish ()              
{

  std::vector<D3DXVECTOR3> path;

  mGraph.deleteDescendants( mFakeNodeA );
  mGraph.deleteParents    ( mFakeNodeB );

  writeBranchesLength();
  //writeEdgesLength();
  writeVertices();

  std::vector<DirectedGraph::EdgeCon> e = mGraph.edgesAndConnectivity();
  
  
  mLog->mProf->start( std::string("Generate geometry") );

  //////////////////////////////////////////////
  //  generate geometry

  std::map< std::pair<int,int>, TrackPath* > pathOfEdge;
  for ( int i = 0; i < e.size() ; ++i )
  {              
    TrackPath* p = generatePath( i, mDesc.trackWidth, e[i].e );  

    std::pair<int,int> key( e[i].e.vb, e[i].e.ve );
    pathOfEdge[key] = p;

    if( e[i].e.ve == mEndNodeId )
      mEndEdge = p;
    if( e[i].e.vb == mStartNodeId )
      mStartEdge = p;
 
    if ( p != NULL )
      mPath.push_back( p );    
  }
  // Once all paths created, link each one to its descendants and parents
  for ( int i = 0; i < e.size() ; ++i )
  {  
    std::pair<int,int> key( e[i].e.vb, e[i].e.ve );
    TrackPath* parent = pathOfEdge[key];
    for ( int j = 0; j < e[i].desc.size() ; ++j )
    {
      std::pair<int,int> key( e[i].desc[j].vb, e[i].desc[j].ve );
      TrackPath* children = pathOfEdge[key];
      parent->addDescendant( children );
      children->addParent( parent );
    }
  }
  // Now 

  mLog->mProf->end();

  //////////////////////////////////////////////
  //  generate IA info
  //if( mDesc.usesIA )
  {
    mLog->mProf->start( std::string("Generate IA structures") );
    generateIaStructures();
    mLog->mProf->end();
  }

  mTrackLength = computeLength();
  writeEdgesAndTheirPathsLength();
}

namespace TrackSubstitutionSort{
  class VertexAndEdgeLength{
    public:
      int   v;
      float l;

    };

    bool  compareVertexAndEdgeLength( const VertexAndEdgeLength& a, const VertexAndEdgeLength& b )
    {
      return !(a.l < b.l);
    };
};

/*
  aMaxGraphIterations needed as to generate intermediate levels
*/
void Track::generateGraph(  const int   aMaxGraphIterations, 
                            const bool  aComputeAllIterations, 
                            const int   aCurrentIteration,
                            const bool  aOnlySubstituteAzimutIllegalEdges )
{

  int currentSubstitution = 0;
  int maxSubstitution = aMaxGraphIterations;

  if ( !aComputeAllIterations )
  {
    currentSubstitution = aCurrentIteration;
    maxSubstitution     = aCurrentIteration + 1;
  }
   
  for ( int substitution = currentSubstitution; substitution < maxSubstitution ; ++substitution ) 
  { 
    int patternInId = 0;

    *Global::mFileDebugTracks  
      << "Sub #" << substitution << ": ";
    *Global::mFileDebugTrack<< std::endl 
      << "----------------------------------------------------------------------------"
      << std::endl<< " Iteration "<<substitution<< std::endl;

    // Generate random permutation outPattern ids
    int numPatternOuts = mPatternInsOut[ patternInId ].size();
    std::vector<int> patternOutIds  = mRand.genPermutations( numPatternOuts );

    //  get modifiable vertices
    std::vector<int> idVertices     = mGraph.mutableVertices();
    mRand.genPermutations( idVertices, idVertices.size() );

    // Get only nodes whose azimuth is way too high
    if ( aOnlySubstituteAzimutIllegalEdges || generateGraphNeedsCorrectionOfAzimuth() )
    {
      std::vector<int> verticesWithIllegalAzimuth;
      for ( int i = 0; i < idVertices.size() ; ++i)
      {        
        float a = DxMath::azimut( 
          DxMath::unitVector( 
            mGraph.vertex( idVertices[i] ).pos,
            mGraph.vertex(  mGraph.descendants( idVertices[i] )[0] ).pos
            )  ); 
        float l = DxMath::distance (
          mGraph.vertex( idVertices[i] ).pos,
          mGraph.vertex(  mGraph.descendants( idVertices[i] )[0] ).pos
          );
        if ( l > StoredPiecesBalancers::General::subsMinDistance )
          if ( a < DxMath::degToRad(75) || a > DxMath::degToRad(115) )
            verticesWithIllegalAzimuth.push_back( idVertices[i] );
      }
      idVertices = verticesWithIllegalAzimuth;
    }

    //  Sort input vertices selon edge length
    if ( mDesc.substituteLongerEdgesFirst )
    {
      std::vector<TrackSubstitutionSort::VertexAndEdgeLength> v(idVertices.size() );

      for ( int i = 0; i < idVertices.size() ; ++i)
      {
        v[i].v = idVertices[i];
        v[i].l = mGraph.edge( idVertices[i], 
          mGraph.descendants( idVertices[i] )[0] ).mDesiredLength;
      }

      std::sort( v.begin(), v.end(), TrackSubstitutionSort::compareVertexAndEdgeLength );

      for ( int i = 0; i < idVertices.size() ; ++i)
      {
        idVertices[i] = v[i].v;        
      }
    }


    //  get collision of current graph
    TrackGraphCollision::CollisionData colData = collisionData();    

    //  Check all vertices, and for each every patterOut
    bool pieceSubstituted = false;
    for ( int i = 0; i < idVertices.size() && !pieceSubstituted ; ++i )
    {
      for ( int subsOut = 0; !pieceSubstituted && subsOut < numPatternOuts ; ++subsOut )
        {                   
          int idVertex      = idVertices[i];
          int patternOutId  = patternOutIds[subsOut];
 
          if (substitution == 4 && patternOutId == 1 && idVertex == 10 )
            int a = 1;

          DirectedGraph::RuleMatch match;
          DirectedGraph::initPatternOut( mPatternInsOut[patternInId][patternOutId]->mRoot );
          
          *Global::mFileDebugTrack << std::endl << "Trying at [vb,piece] = [" << idVertex <<","<<patternOutId<<"]: ";
          bool valid = mPatternInsOut[patternInId][patternOutId]->validateInitialEdge( mGraph, idVertex, aOnlySubstituteAzimutIllegalEdges );
          if ( valid ) 
          {

            if ( !mPatternInsOut[patternInId][patternOutId]->isTangentAbsorber() )
              valid &= DirectedGraph::testPieceInGraph( mGraph,                                                     
                                                        mCollisionConfiguration,
                                                        colData,                                                    
                                                        mPatternInsOut[patternInId][patternOutId],
                                                        idVertex,
                                                        mGraph.descendants(idVertex)[0]);    
            
            if (!valid)
            {
              *Global::mFileDebugTrack << " collided.";              
            }

            if ( valid ) 
            {
              match = DirectedGraph::matchPattern( mGraph, 
                                                   mPatternIns[patternInId], 
                                                   mPatternInsOut[patternInId][patternOutId],
                                                   idVertex );            
              if ( match.matched )
              {   
                DirectedGraph::applyRule( mGraph,                                    
                                          mPatternInsOut[patternInId][patternOutId],
                                          mRand,
                                          idVertex, 
                                          match.lastVertex );              
                balanceBranchesLengths ( patternInId, patternOutId );
                          
                *Global::mFileDebugTrack << " matched ";
                *Global::mFileDebugTracks
                  << std::endl
                  << " Vertex " << idVertex
                  << " changed for piece " << patternOutId << std::endl;        

                D3DXVECTOR3 p = mGraph.vertex( idVertex ).pos;
                *Global::mFileDebugTracks << " In: " << p.x << "," <<p.y<<","<<p.z;
                p = mGraph.vertex( match.lastVertex ).pos;
                *Global::mFileDebugTracks << " -> " << p.x << "," <<p.y<<","<<p.z;
                *Global::mFileDebugTracks<< std::endl;
              
                pieceSubstituted = true;
              } // end if match
            } // end if valid
          } // end if valid

        }// end for subsout    
    }// end for vertex  

    
    if ( !pieceSubstituted )
    {
      if ( mTrackPreview[0] != NULL ) delete mTrackPreview[0];
      mTrackPreview[0] = convertToLines();

      mPanickExit = true;
      *mLog->mFileTrack << std::endl;
      *mLog->mFileTrack << "------------------------------------------------------" << std::endl;
      *mLog->mFileTrack << "Start panick mode: graph not finished." << std::endl;
      *mLog->mFileTrack << "------------------------------------------------------" << std::endl;

       std::vector<DirectedGraph::Edge> e = mGraph.edges();
       
       

       for ( int i = 0; i < e.size() ; ++i )
       {

         if ( e[i].edgeData.isModifiable() &&
           mGraph.numDesc( e[i].vb ) == 1 &&
           mGraph.numDesc( e[i].ve ) == 1 &&
           e[i].ve != 3 &&  //last vertex
           DxMath::distance( mGraph.vertex( e[i].vb ).pos, mGraph.vertex( e[i].ve ).pos ) > 200
           )
         {
            *mLog->mFileTrack << "-Y-";                   
           *mLog->mFileTrack << "edge " << i; 
           *mLog->mFileTrack << " (from v: " << e[i].vb << ")"; 
           *mLog->mFileTrack << " Distance: " << 
             DxMath::distance( mGraph.vertex( e[i].vb ).pos, mGraph.vertex( e[i].ve ).pos );
           *mLog->mFileTrack << " Azimut: " << 
             DxMath::radToDeg( 
             DxMath::azimut( 
             DxMath::unitVector( mGraph.vertex( e[i].ve ).pos, mGraph.vertex( e[i].vb ).pos )));
           *mLog->mFileTrack << " PolarB: " << 
             DxMath::radToDeg( 
              DxMath::polarAngleDistance(
                DxMath::unitVector( 
                  mGraph.vertex( e[i].ve ).pos, mGraph.vertex( e[i].vb ).pos 
                  )
                , 
                mGraph.vertex( e[i].vb ).tan ));
*mLog->mFileTrack << " PolarE: " << 
             DxMath::radToDeg( 
             DxMath::polarAngleDistance( 
              DxMath::unitVector( mGraph.vertex( e[i].ve ).pos, mGraph.vertex( e[i].vb ).pos ), mGraph.vertex( e[i].ve ).tan ));
          *mLog->mFileTrack << " Desc: " << mGraph.numDesc( e[i].vb );
          *mLog->mFileTrack << " Par: " << mGraph.numPar( e[i].vb );
              if( mGraph.isVertexMutable(e[i].vb ) )
                *mLog->mFileTrack << " Mutable";
              else
                *mLog->mFileTrack << " Inmutable";
                  
            *mLog->mFileTrack << std::endl;
         }
          
       }

      for ( int i = 0; i < e.size() ; ++i )
       {
         if ( !e[i].edgeData.isModifiable() ||
           mGraph.numDesc( e[i].vb ) > 1 ||
           mGraph.numDesc( e[i].ve ) > 1 ||
           e[i].ve == 3 ||  //last vertex
           DxMath::distance( mGraph.vertex( e[i].vb ).pos, mGraph.vertex( e[i].ve ).pos ) < 200
           )
         {
          *mLog->mFileTrack << "-N-";            
           *mLog->mFileTrack << "edge " << i; 
           *mLog->mFileTrack << " Distance: " << 
             DxMath::distance( mGraph.vertex( e[i].vb ).pos, mGraph.vertex( e[i].ve ).pos );
           *mLog->mFileTrack << " Azimut: " << 
             DxMath::radToDeg( 
             DxMath::azimut( 
             DxMath::unitVector( mGraph.vertex( e[i].ve ).pos, mGraph.vertex( e[i].vb ).pos )));
*mLog->mFileTrack << " Desc: " << mGraph.numDesc( e[i].vb );
            *mLog->mFileTrack << std::endl;
         }
          
       }

      return;
    }// end of !subs
    else
    {
      if ( mTrackPreview[0] != NULL ) delete mTrackPreview[0];
      mTrackPreview[0] = convertToLines();
    }

  }// end for substitution
}



void Track::balanceBranchesLengths ( const int aPatternInId, const int aPatternOutId )
{
  int numBranches   = mPatternInsOut[aPatternInId][aPatternOutId]->mBranches.size();
  int numTotalEdges = mPatternInsOut[aPatternInId][aPatternOutId]->mEdges.size();

  std::vector<float>  branchLength( numBranches );
  std::vector<float>  edgeLength  ( numTotalEdges );

  //  Compute length of each new edge
  for ( int i = 0; i < numTotalEdges ; ++i )
  {
    /*
    int vb = mPatternInsOut[aPatternInId][aPatternOutId]->mEdges[i]->vb;
    int ve = mPatternInsOut[aPatternInId][aPatternOutId]->mEdges[i]->ve;
    */
    TrackPath* path = generatePathSansCurves( 0, mDesc.trackWidth, *mPatternInsOut[aPatternInId][aPatternOutId]->mEdges[i] );
    float l = path->integrateLength();
    edgeLength[i] = l;
    delete path;
  }


  //  Compute length of each branch  
  for ( int i = 0; i < numBranches ; ++i )
  {
    float length = 0.0f;
    for ( int j = 0; j < mPatternInsOut[aPatternInId][aPatternOutId]->mBranches[i].mEdgeUsed.size() ; ++j )
    {
      int edgeId = mPatternInsOut[aPatternInId][aPatternOutId]->mBranches[i].mEdgeUsed[j];
      length += edgeLength[ edgeId ];
    }
    branchLength[i] = length;
  }

  //  Compute maximum length 
  float lengthMax = mPatternInsOut[aPatternInId][aPatternOutId]->mOriginalDesiredLength;
  for ( int i = 0; i < numBranches ; ++i )
    if ( branchLength[i] > lengthMax )
      lengthMax = branchLength[i];

  //  Copy vectors of used branches
  std::vector< std::vector<int> > branchEdges         (numBranches);
  std::vector< std::vector<int> > branchOffenderEdges (numBranches);
  
  for ( int i = 0; i < numBranches ; ++i )
  {
    branchEdges[i]          = mPatternInsOut[aPatternInId][aPatternOutId]->mBranches[i].mEdgeUsed;
    branchOffenderEdges[i]  = mPatternInsOut[aPatternInId][aPatternOutId]->mBranches[i].mEdgesToUpdate;
  }

  if ( branchOffenderEdges[0].size() == 0 )
    return;

  //  Now balance out all the branches
  bool  finished  = false;
  float epsilon   = 0.1f;
  while ( !finished )
  {
    for ( int i = 0; i < numBranches ; ++i )
    {
      if ( branchOffenderEdges[i].size() > 0 )
      {
        int numEdges = branchOffenderEdges[i].size();
        //float delta = fabs( lengthMax - branchLength[i]);
        float delta = ( lengthMax - branchLength[i]);
        delta *= epsilon;
        delta /= float( numEdges );

        for ( int j = 0; j < numEdges ; ++j )
        {
          int edgeId = branchOffenderEdges[i][j];
          edgeLength[edgeId] += delta;
        }

        //  Compute new length
        branchLength[i] = 0.0f;
        for ( int j = 0; j < branchEdges[i].size() ; ++j )
        {
          int edgeId = branchEdges[i][j];
          branchLength[i] += edgeLength[ edgeId ];
        }

        //  Remove branch if length surpassed
        if ( DxMath::equality( branchLength[i], lengthMax, lengthMax*0.01f ) )
        {
          std::vector<int> edgesToRemove;
          edgesToRemove = branchOffenderEdges[i];
          bool allRemoved = true;
          for ( int k = 0; k < numBranches ; ++k )
          {
            for ( int j = 0; j < edgesToRemove.size() ; ++j )          
              StlHelpers::removeFromVector( branchOffenderEdges[k], edgesToRemove[j] );
               
            allRemoved &= ( branchOffenderEdges[k].size() == 0 );
          }

          //  Check if finished
          if ( allRemoved )
            finished = true;

        }
      }// end of if
    }// for each branch
  }// while !finished

  //  Update edges
  for ( int i = 0; i < numTotalEdges ; ++i )
  {    
    int vb = mPatternInsOut[aPatternInId][aPatternOutId]->mEdges[i]->vb;
    int ve = mPatternInsOut[aPatternInId][aPatternOutId]->mEdges[i]->ve;
    
    TrackBp::EdgeData e = mGraph.edge( vb, ve );
    e.mDesiredLength = edgeLength[i];
    mGraph.setEdge( vb, ve, e );
  }
}
/*
int     Track::newNode( const D3DXVECTOR3& aX, const D3DXVECTOR3& aT, const D3DXVECTOR3& aN )
{
  return mGraph.newNode( TrackBp::Vertex( aX, aT, aN ) );
}
*/
int     Track::newEdge( const int aIdN1, const int aIdN2, 
                        const TrackBp::EdgeData& aEdge )
{
  return mGraph.newEdge( aIdN1, aIdN2, aEdge );
}

void Track::writeBranchesLength()
{

  std::vector<int> visitedVertices;
  std::vector<DirectedGraph::Edge> edges;
  std::vector< std::vector<DirectedGraph::Edge> > branches;
                                   
  extractBranchesREC( 0, 0, edges, visitedVertices, branches  );

  mLog->mFile->precision(0);
  *mLog->mFile << std::endl;
  *mLog->mFile << "-----------------------------------------------------------------------------------" << std::endl;
  *mLog->mFile << "          BRANCHES" << std::endl;
  *mLog->mFile << "-----------------------------------------------------------------------------------" << std::endl;
  float totalL = 0.0;
  for ( int i = 0; i < branches.size() ; ++i )
  {
    *mLog->mFile << "Branch: ";
    float l = 0.0f;
    for ( int j = 0; j < branches[i].size() ; ++j )
      l += branches[i][j].edgeData.mDesiredLength;
    *mLog->mFile << l << " (" << branches[i].size() << " nodes)" <<std::endl;
    totalL += l;
  }

}

void Track::writeEdgesLength()
{
  std::vector<DirectedGraph::Edge> edges = mGraph.edges();

  mLog->mFile->precision(0);
  *mLog->mFile << std::endl;
  *mLog->mFile << "-----------------------------------------------------------------------------------" << std::endl;
  *mLog->mFile << "          EDGES" << std::endl;
  *mLog->mFile << "-----------------------------------------------------------------------------------" << std::endl;
  float totalL = 0.0;
  for ( int i = 0; i < edges.size() ; ++i )
  {
    TrackBp::Vertex vb = mGraph.vertex( edges[i].vb );
    TrackBp::Vertex ve = mGraph.vertex( edges[i].ve );

    float linearLength = DxMath::distance( vb.pos, ve.pos );

    *mLog->mFile << "edge: " << edges[i].edgeData.mDesiredLength;
    
    *mLog->mFile << " linear: "<< linearLength <<std::endl;
    
  }

}

void Track::writeEdgesAndTheirPathsLength()
{
  /*
  std::vector<DirectedGraph::Edge> edges = mGraph.edges();

  mLog->mFile->precision(0);
  *mLog->mFile << std::endl;
  *mLog->mFile << "-----------------------------------------------------------------------------------" << std::endl;
  *mLog->mFile << "          EDGES and their path" << std::endl;
  *mLog->mFile << "-----------------------------------------------------------------------------------" << std::endl;
  float totalL = 0.0;
  for ( int i = 0; i < edges.size() ; ++i )
  {
    TrackBp::Vertex vb = mGraph.vertex( edges[i].vb );
    TrackBp::Vertex ve = mGraph.vertex( edges[i].ve );

    float linearLength = DxMath::distance( vb.pos, ve.pos );

    *mLog->mFile << "edge: " << edges[i].edgeData.mDesiredLength;
    
    *mLog->mFile << " linear: "<< linearLength;
    *mLog->mFile << " path: "<< mPath[i]->integrateLength() <<std::endl;
  
  }

  */
}

void Track::writeVertices()
{
  mLog->mFile->precision(3);
  *mLog->mFileTrack << std::endl;
  *mLog->mFileTrack << "-----------------------------------------------------------------------------------" << std::endl;
  *mLog->mFileTrack << "          VERTICES" << std::endl;
  *mLog->mFileTrack << "-----------------------------------------------------------------------------------" << std::endl;
  for ( int i = 4; i < mGraph.numVertices(); ++i )
  {
    TrackBp::Vertex v = mGraph.vertex(i);
        
    *mLog->mFileTrack << "P: "<<v.pos.x<< ", "<<v.pos.y<< ", "<<v.pos.z << " ";
    *mLog->mFileTrack << "T: "<<v.tan.x<< ", "<<v.tan.y<< ", "<<v.tan.z << " ";
    *mLog->mFileTrack << "N: "<<v.nor.x<< ", "<<v.nor.y<< ", "<<v.nor.z << " ";
    *mLog->mFileTrack << std::endl;
  }
}

void Track::writePieces()
{   
  *mLog->mFileTrackPieces << "-----------------------------------------------------------------------------------" << std::endl;
  *mLog->mFileTrackPieces << "          Pieces" << std::endl;
  *mLog->mFileTrackPieces << "-----------------------------------------------------------------------------------" << std::endl;
  for ( int i = 0; i < mPatternInsOut[0].size(); ++i )
  {      
    *mLog->mFileTrackPieces << "Piece " << mPatternInsOut[0][i]->mOutPatternId<< std::endl;
    *mLog->mFileTrackPieces << " length: [" << mPatternInsOut[0][i]->mMinLength 
                            << ", " << mPatternInsOut[0][i]->mMaxLength << "]" << std::endl;
    *mLog->mFileTrackPieces << " azimut: [" << DxMath::radToDeg(mPatternInsOut[0][i]->mMinAzimut) 
      << ", " << DxMath::radToDeg(mPatternInsOut[0][i]->mMaxAzimut) << "]" << std::endl;
    *mLog->mFileTrackPieces << std::endl;
  }
}


std::vector<Bumper*> Track::placeBumpers( const TrackBp::TrackEntityDescription& aTrEntDesc, Game* aGame )
{
  std::vector<Bumper*> bumpers;

  int idBumper = 0;
  for ( int i = 0; i < mPath.size(); ++i )
  {
    for ( int k = 0; k < mPath[i]->mEdges.size() ; ++k )
    {
      float placeProbability = mRand.randFloat_0_1();
      if ( mPath[i]->mEdges[k]->areEntitiesPlacable() && placeProbability < aTrEntDesc.probEdgeWithBumper )
      {
        float l = mPath[i]->mEdges[k]->integrateLength();


        //  BUMPERS
        {
          
          int n = int(l/aTrEntDesc.bumperDensity);
          float tdensity    = 1.0f / float(n + 1);
          float tdispersion = aTrEntDesc.bumperDispersion/l;

          //  Create n archs for this edge
          for ( int j = 1; j < (n + 1); ++j )
          {
            float t = float(j)*tdensity;
            t += tdispersion * (mRand.randFloat_0_1() - 0.5f);

            if ( t >= 0.0f && t <= 1.0f )
            {
              D3DXVECTOR3 pos = mPath[i]->mEdges[k]->mSplinePath->getPoint( t );
                                      
              DxMath::TnbBasis basis = Loft::basisBezier( mPath[i]->mEdges[k]->mSplinePath, t, 
                                  mPath[i]->mEdges[k]->mInitialTangent, mPath[i]->mEdges[k]->mLastTangent,
                                  mPath[i]->mEdges[k]->mInitialNormal, mPath[i]->mEdges[k]->mLastNormal );              

              if ( canPlaceEntity( basis.tan ) )
              {
                pos += 0.25f*basis.nor;
                float trackWidth = mDesc.trackWidthEntities;;
                float tangentialComponent = ((mRand.randFloat_0_1()*2 - 1.0f)*trackWidth);
                pos +=  tangentialComponent*basis.bin;

                Bumper* bumper = new Bumper(idBumper, 0,0,
                                     pos,                                
                                     DxMath::basisMatrixInv( basis.tan, basis.nor, basis.bin),
                                     1.0f );
                bumpers.push_back ( bumper );  
                ++idBumper;

                PlacedEntity p ( t , tangentialComponent );
                mPath[i]->mEdges[k]->addEntity( p, bumper );
              }
            }        
          }
        }
      }
    }
  }
  return bumpers;
}

std::vector<Arch*> Track::placeArchs( const TrackBp::TrackEntityDescription& aTrEntDesc, Game* aGame )
{

  std::vector<Arch*> archs;

  for ( int i = 0; i < mPath.size(); ++i )
  {
    for ( int k = 0; k < mPath[i]->mEdges.size() ; ++k )
    {
      float placeProbability = mRand.randFloat_0_1();      
      if ( mPath[i]->mEdges[k]->areEntitiesPlacable() && placeProbability < aTrEntDesc.probEdgeWithArch )
      {
        float l = mPath[i]->mEdges[k]->integrateLength();

        //  ARCHS
        {
          
          int n = int(l/aTrEntDesc.archDensity);
          float tdensity    = 1.0f / float(n + 1);
          float tdispersion = aTrEntDesc.archDispersion/l;

          //  Create n archs for this edge
          for ( int j = 1; j < (n + 1); ++j )
          {
            float t = float(j)*tdensity;
            t += tdispersion * (mRand.randFloat_0_1() - 0.5f);            

            if ( t >= 0.0f && t <= 1.0f )
            {
              D3DXVECTOR3 pos = mPath[i]->mEdges[k]->mSplinePath->getPoint( t );

              DxMath::TnbBasis basis = Loft::basisBezier( mPath[i]->mEdges[k]->mSplinePath, t, 
                                  mPath[i]->mEdges[k]->mInitialTangent, mPath[i]->mEdges[k]->mLastTangent,
                                  mPath[i]->mEdges[k]->mInitialNormal, mPath[i]->mEdges[k]->mLastNormal );
           

              if ( canPlaceEntity( basis.tan ) )
              {
                pos += basis.nor*(-0.2f);
                float trackWidth = mDesc.trackWidthEntities - mArchPlacementWidthReduction;
                float tangentialComponent = ((mRand.randFloat_0_1()*2 - 1.0f)*trackWidth);
                pos += tangentialComponent*basis.bin;

                int id = mRand.randInt( Arch::msNumArchs - 1 );
                
                Arch* arch = new Arch( id, 
                                     pos, 
                                     DxMath::basisMatrixInv( basis.tan, basis.nor, basis.bin),                                                             
                                     aTrEntDesc.archHeight,
                                     aGame);
                archs.push_back ( arch );  

                PlacedEntity p ( t , tangentialComponent );
                mPath[i]->mEdges[k]->addEntity( p, arch );
              }
            }
          }
        }
      }
    }
  }

  return archs;
}

TrackLinesAndTNBs* Track::convertToLines() const
{
  TrackLinesAndTNBs* model;

  model = new TrackLinesAndTNBs();

  std::vector< std::vector<D3DXVECTOR3> > basis;
  std::vector<VertexPC> edgeVertices;
  std::vector<DirectedGraph::Edge> edges = mGraph.edges();

  basis.resize( edges.size() );
  for ( int i = 0; i < edges.size() ; ++i )
  {
    TrackBp::Vertex vb = mGraph.vertex( edges[i].vb );
    TrackBp::Vertex ve = mGraph.vertex( edges[i].ve );

    VertexPC current; current.pos = vb.pos;
    VertexPC next;    next.pos    = ve.pos;

    current.col = D3DXCOLOR(0.8,0.8,0.8,0.5f);
    next.col = D3DXCOLOR(0.8,0.8,0.8,0.5f);

    edgeVertices.push_back( current );
    edgeVertices.push_back( next );

    float size = 20;
    current.pos += vb.nor;
    basis[i].push_back( current.pos );
    basis[i].push_back( current.pos + size*vb.tan );
    basis[i].push_back( current.pos );
    basis[i].push_back( current.pos + size*vb.nor );
    basis[i].push_back( current.pos );
    basis[i].push_back( current.pos + size*DxMath::thirdVectorBasis( vb.tan, vb.nor ) );   
  }
/*
  // TODO: show per jump TNB
  for ( int i = 0; i < mPath.size() ; ++i )
  {
    if ( ! mPath[i]->isEdge() )
    {
      for ( int j = 0; j < mPath[i]->mEdges.size() ; ++j )
      {
        for ( int k = 0; k < mPath[i]->mEdges[j]-> ; ++j )
        {
        }


      }
    }
  }
  */

  model->setEdges( edgeVertices );
  model->setBasis( basis );
  
  return model;
}

float Track::computeLength() const
{

  std::vector<int> visitedVertices;
  std::vector<DirectedGraph::Edge> edges;
  std::vector< std::vector<DirectedGraph::Edge> > branches;
                                   
  extractBranchesREC( 0, 0, edges, visitedVertices, branches  );

  float maxL = 0.0;
  for ( int i = 0; i < branches.size() ; ++i )
  {    
    float l = 0.0f;
    for ( int j = 0; j < branches[i].size() ; ++j )
      l += branches[i][j].edgeData.mDesiredLength;
    if ( l > maxL )
      maxL = l;
  }

  return maxL;
}

float Track::length() const
{
  return mTrackLength;
}

EndTrigger* Track::generateEndTrigger( Game* aGame )
{

  D3DXVECTOR3 pos = mEndEdge->mEdges[0]->mSplinePath->getPoint( 0.55 );

  DxMath::TnbBasis basis = Loft::basisBezier( mEndEdge->mEdges[0]->mSplinePath, 0.55, 
                      mEndEdge->mEdges[0]->mInitialTangent, mEndEdge->mEdges[0]->mLastTangent,
                      mEndEdge->mEdges[0]->mInitialNormal, mEndEdge->mEdges[0]->mLastNormal );
  

  return new EndTrigger(  pos, 
                          DxMath::basisMatrixInv( basis.tan, basis.nor, basis.bin),                                                             
                          15,
                          22,
                          aGame);  
}

TrackBp::TrackFogBounding   Track::getFogBoundingData () const
{
  std::vector<D3DXVECTOR3> p ( mGraph.numVertices() );
  for ( int i = 0; i < mGraph.numVertices() ; ++i )
  {
    TrackBp::Vertex v = mGraph.vertex(i);
    p[i] = v.pos;
  }
  
  //  compute minimum height
  float minY = 100000;
  D3DXVECTOR3 centerMass = D3DXVECTOR3(0,0,0);
  for ( int i = 0; i < p.size() ; ++i )  
  {
    minY = min( minY, p[i].y );   
    centerMass += p[i];
  }
  centerMass *= 1.0f/float( p.size() );

  float radius = 0;
  for ( int i = 0; i < p.size() ; ++i )  
  {
    float distance = DxMath::distance( centerMass, p[i] );
    radius = max( radius, distance );
  }


  TrackBp::TrackFogBounding data;
  data.xzPlaneHeight      = minY;
  data.boundingCircCenter = centerMass;
  data.boundingCircRadius = radius;
    
  return data;
}

/*
  Collision data has to be made from graph without fake edges
*/
TrackGraphCollision::CollisionData  Track::collisionData() const
{
  /*
  DirectedGraph::DirGraph graph = mGraph;
  graph.deleteDescendants( mFakeNodeA );
  graph.deleteParents    ( mFakeNodeB );

  graph.deleteVertex     ( mFakeNodeB );
  graph.deleteVertex     ( mFakeNodeA );*/
  
  return mGraph.collisionData( mCollisionConfiguration );
}

void Track::generateGraphChangeToAbilitySet()
{
  mPatternInsOut[0].clear();
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutTanAbsorberBeg(0) );  
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutTanAbsorberEnd(1) );  
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutZigZagDown(2)  );
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutZigZagUp(3)    );  
  initPatternInsAbility();
}

void Track::generateGraphChangeToAzimuthSet()
{
  mPatternInsOut[0].clear();
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutTanAbsorberBeg(0) );  
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutTanAbsorberEnd(1) );  
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutZigZagDown(2)  );
  mPatternInsOut[0].push_back ( StoredPiecesBalancers::patternOutZigZagUp(3)    );  
}

bool Track::generateGraphNeedsCorrectionOfAzimuth()
{
  if ( mPanickExit ) return false;

  std::vector<int> idVertices     = mGraph.mutableVertices();
  
  bool found = false;
  for ( int i = 0; i < idVertices.size() && !found ; ++i)
  {        
    float a = DxMath::azimut( 
      DxMath::unitVector( 
        mGraph.vertex( idVertices[i] ).pos,
        mGraph.vertex(  mGraph.descendants( idVertices[i] )[0] ).pos
        )  );  
    float l = DxMath::distance (
        mGraph.vertex( idVertices[i] ).pos,
        mGraph.vertex(  mGraph.descendants( idVertices[i] )[0] ).pos
        );
    
    if ( l > StoredPiecesBalancers::General::subsMinDistance )
      if ( a < DxMath::degToRad(75) || a > DxMath::degToRad(115) )
        found = true;
  }

  return found;
}

bool  Track::canPlaceEntity( const D3DXVECTOR3& aPathTangent )
{
  float azTan = DxMath::azimut( aPathTangent );
  return ( azTan > DxMath::degToRad(mEntityMinAzimuth) && azTan < DxMath::degToRad(mEntityMaxAzimuth) );
}