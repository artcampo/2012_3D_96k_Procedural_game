#ifndef _GameTrackEdge_HPP_
#define _GameTrackEdge_HPP_

#include  "dx_misc.hpp"
#include  <vector>

#include  "Geometry\Spline\SplineBezierTMapped.hpp"
#include  "RenderModels\DxMeshModel.hpp"
#include  "RenderModels\DxLodModel.hpp"
#include  "Physics/MeshCol.hpp"

#include  "Geometry/Loft.hpp"
#include  "Geometry/LoftIntersect.hpp"
#include  "Geometry/LoftBoolean.hpp"

#include  "GameBall/TrackBp.hpp"
#include  "GameBall/Entities/PlacedEntity.hpp"


class Bumper;
class Arch;
class Target;


class TrackEdge {

  //  Info
public:
  float   integrateLength()     const;
  bool    curvesCanBeAdded()    const;
  bool    areEntitiesPlacable() const;
  float   trackWidth()          const;
 
  LoftBoolean::LoftInfo  loftInfo() const;

  //  Creation
public:  
  TrackEdge ( const std::vector<D3DXVECTOR3>& aBezierPointsPath, 
              const std::vector<D3DXVECTOR3>& aBezierPointsSliceBeg,
              const std::vector<D3DXVECTOR3>& aBezierPointsSliceEnd,
              const D3DXVECTOR3& aInitialTangent,
              const D3DXVECTOR3& aLastTangent, 
              const D3DXVECTOR3& aInitialNormal,
              const D3DXVECTOR3& aLastNormal,
              const float aTrackWidth,
              const D3DXVECTOR3& aSliceMult,
              const bool aCurvesCanBeAdded = true,
              const bool aAreEntitiesPlacable = true);

  ~TrackEdge();

  //  Render/physics
public:  
  void  generateLoftSurface ();
  DxLodModel<VertPNTUV>*   model() const;  
  std::vector<MeshCol*>   meshColTri() const;
  //  for convex meshes
  MeshCol*                        meshCol() const;

  //  Surface with itnersections
  void generateLoftSurfaceInt ( const LoftBoolean::LoftIntersectionInfo& aIntStart, 
                                const LoftBoolean::LoftIntersectionInfo& aIntEnd );
  void generateLoftSurfaceIntStart  ( const LoftBoolean::LoftIntersectionInfo& aIntStart );
  void generateLoftSurfaceIntEnd    ( const LoftBoolean::LoftIntersectionInfo& aIntEnd );

  //  Entities over this Edge
  void addEntity( const PlacedEntity& aEntity, Bumper* aBumper );
  void addEntity( const PlacedEntity& aEntity, Arch* aArch );

  PlacedEntity nextEntitySplineT( const float aT );

  bool    advanceAlongPathNunits( const float aUnits, 
                                  const float aT, 
                                  float& aNewT,                                   
                                  float& aAdvancedUnits,
                                  bool& aMaxAngleSurpassed,
                                  const float aMaxAngle,                                                                                   
                                  const D3DXVECTOR3& aBallPos,
                                  const D3DXVECTOR3& aBallVel,
                                  BallIA::Target& aOutTarget);

  float    advanceAlongPath( const float aUnits, const float aT  );

  //  Propierties
private:
  bool                        mSurfaceCreated;
  bool                        mCurvesCanBeAdded;  
  bool                        mAreEntitiesPlacable;
private:
  static const int    mTesselationPerUnit = 3;
  static const int    mFustrumResolution  = 12;

private:
  
  //  Big Data
  std::vector< std::vector<VertPNTUV> >    mVertices;
  //  Spline data
private:
  D3DXVECTOR3           mSliceMult;
  int                   mPathTes;
  int                   mSliceTes;
  SplineBezierTMapped*  mSplinePath;
  SplineBezierTMapped*  mSplineSliceBeg;
  SplineBezierTMapped*  mSplineSliceEnd;
  float                 mLength;
  int                   mNumPointsOfSpline;
  int                   mNumTrianglesMesh;  

  TrackBp::Vertex         mNodeB;
  TrackBp::Vertex         mNodeE;

  D3DXVECTOR3           mInitialTangent;
  D3DXVECTOR3           mLastTangent;
  D3DXVECTOR3           mInitialNormal;
  D3DXVECTOR3           mLastNormal;
  float                 mTrackWidth;

  std::vector<PlacedEntity> mPlacedEntity;

  friend class Track;
  friend class Game;
  friend class Ball_IA;
  friend class MetropolisStructure;

private:
  void  init();
  friend class Track;
  friend class Ball;

  void addEntity( const PlacedEntity& aEntity );

public:
  void  writeLog( std::ofstream* aStream );


};


#endif  //_GameTrackEdge_HPP_
