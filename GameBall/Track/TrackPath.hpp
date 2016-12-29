#ifndef _GameTrackPath_HPP_
#define _GameTrackPath_HPP_

#include  "dx_misc.hpp"
#include  <vector>

#include  "Geometry\Spline\SplineBezierTMapped.hpp"
#include  "RenderModels\DxMeshModel.hpp"
#include  "Physics/MeshCol.hpp"
#include  "Geometry/Loft.hpp"

#include  "GameBall/TrackBp.hpp"
#include  "GameBall/Track/TrackEdge.hpp"
#include  "GameBall/Track/TrackEdgeEnd.hpp"

// TODO: remove?
namespace eTrackPathId{
  enum Ids{
    eTrackEdge = false,
    eTrackComplex = true
  };
};


class TrackPath {
public:  
  //  construction
  TrackPath( const int aId );
  ~TrackPath();
 
  void addEdge ( const std::vector<D3DXVECTOR3>& mBezierPointsPath, 
              const std::vector<D3DXVECTOR3>& mBezierPointsSliceBeg,
              const std::vector<D3DXVECTOR3>& mBezierPointsSliceEnd,
              const D3DXVECTOR3& aInitialTangent,
              const D3DXVECTOR3& aLastTangent, 
              const D3DXVECTOR3& aInitialNormal,
              const D3DXVECTOR3& aLastNormal,
              const float aTrackWidth,
              const D3DXVECTOR3& aSliceMult,
              const bool aCurvesCanBeAdded = true,
              const bool aAreEntitiesPlacable = true);
  void  generateLoftSurface ();

  //  render
  std::vector<DxLodModel<VertPNTUV>*> model() ;  
  std::vector<MeshCol*>              meshColTri() ;

  //  usage
  float       integrateLength() const;

  void        addDescendant ( TrackPath* aDesc );
  void        addParent     ( TrackPath* aDesc );
  int         numDescendants() const;
  int         numParents    () const;
  TrackPath*  descendant    ( const int aIndex ) const;
  TrackPath*  parent        ( const int aIndex ) const;
  
  //  Loft intersection functions
  LoftBoolean::LoftInfo   loftInfoFirstEdge() const;
  LoftBoolean::LoftInfo   loftInfoLastEdge() const;
  void  setLoftIntersectLastEdge( LoftBoolean::LoftIntersectionInfo& aInfo );
  void  setLoftIntersectFirstEdge( LoftBoolean::LoftIntersectionInfo& aInfo );

  //  Run-time data
private:
  bool                        mSurfaceCreated;
  std::vector<TrackEdge*>     mEdges;
  std::vector<TrackEdgeEnd*>  mEdgeEnds;

  std::vector<TrackPath*>  mDescendants;
  std::vector<TrackPath*>  mParents;

  LoftBoolean::LoftIntersectionInfo mStartIntersectionInfo;
  LoftBoolean::LoftIntersectionInfo mEndIntersectionInfo;
  
  //debug data
public:
  int   mId;

private:
  void addEdge (  const TrackBp::Vertex& aVertA, 
                  const TrackBp::Vertex& aVertB, 
                  const float aLengthTan, 
                  const std::vector<D3DXVECTOR3>& aBezierPointsSliceBeg,
                  const std::vector<D3DXVECTOR3>& aBezierPointsSliceEnd,
                  const float aTrackWidth,
                  const D3DXVECTOR3& aSliceMult,                                               
                  const bool aCurvesCanBeAdded = true,
                  const bool aAreEntitiesPlacable = true );

  friend class Track;
  friend class Game;
  friend class Ball_IA;
  friend class MetropolisStructure;

private:
  void  init();
  friend class Track;

public:
  void  writeLog( std::ofstream* aStream );
  void  debugAddVbId( const int id );
  int   mDebugVbId;

};


#endif  //_GameTrackPath_HPP_
