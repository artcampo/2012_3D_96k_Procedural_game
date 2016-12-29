#ifndef _GameTrack_HPP_
#define _GameTrack_HPP_



#pragma ONCE
#include "dx_misc.hpp"
#include <vector>

#include  "Geometry\Spline\SplineBezierTMapped.hpp"
#include  "RenderModels/DxMeshModel.hpp"
#include  "RenderModels/DxLodModel.hpp"
#include  "RenderModels/GameBp/TrackLinesAndTNBs.hpp"
#include  "Physics/MeshCol.hpp"
#include  "Noise/RandomNumberGenerator.hpp"

#include  "Geometry/Loft.hpp"
#include  "Geometry/LoftBoolean.hpp"
#include  "Geometry/DirGraph/DirGraph.hpp"
#include  "Geometry/DirGraph/Pattern.hpp"
#include  "Geometry/DirGraph/SubRule.hpp"
#include  "Geometry/Spline/SplineIntegral.hpp"

#include  "GameBall/TrackBp.hpp"
#include  "GameBall/Track/TrackPath.hpp"
#include  "GameBall/Track/TrackPathIA.hpp"
#include  "GameBall/Track/TrackHistogram.hpp"
#include  "GameBall/Track/TrackGraphCollision.hpp"
#include  "GameBall/Entities/PlacedEntity.hpp"
#include  "GameBall/Entities/Bumper.hpp"
#include  "GameBall/Entities/Arch.hpp"
#include  "GameBall/Entities/EndTrigger.hpp"
#include  "GameBall/GameAssets/TrackOutPieces.hpp"
#include  "GameBall/GameAssets/TrackOutPiecesJumps.hpp"
#include  "GameBall/GameAssets/TrackOutPiecesBalancers.hpp"
#include  "GameBall/GameAssets/TrackOutPiecesIO.hpp"


#include  "Misc/StlHelpers.hpp"
#include  "Misc/Log.hpp"


#include  "GameBall/Debug/TrackReport.hpp"

class Game;
class EdgePath;


class Track {
public:  
  Track(  TrackBp::TrackDescription& aDesc,
          const std::vector<D3DXVECTOR3>& mBezierSliceBorder,
          const std::vector<D3DXVECTOR3>& mBezierSliceNoBorder,
          const unsigned long aTrackSeed,
          const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration );
  ~Track();

  //  Render functions
  std::vector<DxLodModel<VertPNTUV>*>  model()       const;  
  std::vector<MeshCol*>               meshColTri()  const;
  TrackLinesAndTNBs*  convertToLines() const;
  TrackBp::TrackFogBounding   getFogBoundingData () const;

  //  Ia functions
  Kdd2d::KddTree2d<EdgePath>* edgePathsKdd() const;

  //  construction functions
  void generateTrackFinish();
  void generateLoftSurface();
  std::vector<Arch*>    placeArchs  ( const TrackBp::TrackEntityDescription& aTrEntDesc, Game* aGame );
  std::vector<Bumper*>  placeBumpers( const TrackBp::TrackEntityDescription& aTrEntDesc, Game* aGame );  
  float length() const;

  EndTrigger* generateEndTrigger( Game* aGame );

  //TODO: these should go private
  //int     newNode( const D3DXVECTOR3& aX, const D3DXVECTOR3& aT, const D3DXVECTOR3& aN );
  int     newEdge( const int aIdN1, const int aIdN2, const TrackBp::EdgeData& aEdge );

  //  Debug functions: Logs
  void    writeBranchesLength();
  void    writeVertices();
  void    writeEdgesLength();
  void    writeEdgesAndTheirPathsLength();
  void    writePieces();
  
private:
  //  configuration data
  TrackBp::TrackDescription       mDesc;
  std::vector<D3DXVECTOR3>        mSliceBorder;
  std::vector<D3DXVECTOR3>        mSliceNoBorder;
  D3DXVECTOR3                     mSliceMultiplier;
  Rand::RandGen  mRand;
  const static float              mArchPlacementWidthReduction;
  TrackGraphCollision::CollisionConfiguration mCollisionConfiguration;
  const static float              mEntityMaxAzimuth;
  const static float              mEntityMinAzimuth;

  //  Modifiable
  DirectedGraph::DirGraph     mGraph;  
  int mFakeNodeA, mFakeNodeB;
  int mStartNodeId, mEndNodeId;
  bool mPanickExit;
  
  std::vector<DirectedGraph::PatternIn>                     mPatternIns;
  std::vector< std::vector<DirectedGraph::PatternOut*> >    mPatternInsOut;  
  
  //  Resulting track
  std::vector<TrackPath*>       mPath;
  float                         mTrackLength;
  TrackHistogram                mTrackHisto;

  //  Preview data
  std::vector<TrackLinesAndTNBs*>  mTrackPreview;

  //  Cached data
  TrackPath*   mEndEdge;  //no propierty
  TrackPath*   mStartEdge;  //no propierty

public:
  Log*                          mLog;
  void        generateGraph( const int aMaxGraphIterations, 
                            const bool  aComputeAllIterations = true, 
                            const int   aCurrentIteration = 0,
                            const bool  aOnlySubstituteAzimutIllegalEdges = false );
  void        generateGraphChangeToAbilitySet();
  void        generateGraphChangeToAzimuthSet();
  bool        generateGraphNeedsCorrectionOfAzimuth();

private:
  //  construction functions
  TrackPath*  generatePath( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );  


  bool        substitutePattern( const int aPatternInId, const int aPatternOutId );
  void        balanceBranchesLengths ( const int aPatternInId, const int aPatternOutId );

  void        initPatternIns();
  void        initPatternInsPlane();
  void        initPatternInsFall();
  void        initPatternInsAbility();

  int        extractBranchesREC( const int aVertex, 
                                 const int aNumBranch,
                                 std::vector<DirectedGraph::Edge>& aEdges,
                                 std::vector<int>& aVisitedVertices,
                                 std::vector< std::vector<DirectedGraph::Edge> >& aBranches   ) const;

  //  specific for edges
  TrackPath*  generatePathSansCurves  ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );
  TrackPath*  generatePathCurvy       ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );

  TrackPath*  generatePathRamp          ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );
  TrackPath*  generatePathNineTurn      ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );
  TrackPath*  generatePathOne80Turn     ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );  
  TrackPath*  generatePathLoop          ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );
  TrackPath*  generatePathLateral       ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );
  TrackPath*  generatePathLateralCross  ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );
  TrackPath*  generatePathTwoWayJump    ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );
  TrackPath*  generatePathThreeWayJump  ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );
  TrackPath*  generatePathTwister       ( const int aTrackPathDebugId, const float aTrackWidth,  const DirectedGraph::Edge& aEdge );

    
  float integralOfPath ( std::vector<D3DXVECTOR3>& aPath, const int aNumSteps ) ;

  float computeLength() const;

private:
  // diverse inits for graphs
  void initGraphFallingTrack  ( const TrackBp::TrackDescription& aDesc );
  void initGraphStraightTrack ( const TrackBp::TrackDescription& aDesc );
  void initGraphSpiralTrack   ( TrackBp::TrackDescription& aDesc );
 

  //  Twords construction
private:
  static const float mEpsilonDesiredLengthEquality;

  void        generatePathPoint       ( const D3DXVECTOR3& aPos,
                                        const D3DXVECTOR3& aPointTangent,
                                        const float aBaseLength,
                                        const D3DXVECTOR3& aTan,
                                        const D3DXVECTOR3& aNor,
                                        const D3DXVECTOR3& aBin,
                                        const float aCompT,
                                        const float aCompN,
                                        const float aCompB,
                                        const float aTLength,
                                        const float aThita1,
                                        const float aThita2,
                                        D3DXVECTOR3* aPointAndTangentData );

  

  friend class Game;
  friend class MetropolisStructure;

  public:
    TrackGraphCollision::CollisionData  collisionData() const;
  //  Twords IA
private:
  Kdd2d::KddTree2d<EdgePath>* mEdgePathsKdd;

  void generateIaStructures();

  bool  canPlaceEntity( const D3DXVECTOR3& aPathTangent );
};


#endif
