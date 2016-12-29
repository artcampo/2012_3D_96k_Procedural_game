#include  "GameBall/Track/TrackEdge.hpp"
#include  "GameBall/Entities/Bumper.hpp"
#include  "GameBall/Entities/Arch.hpp"
#include  "GameBall/IA/Ball_IA _common.hpp"

bool TrackEdge::curvesCanBeAdded() const
{
  return mCurvesCanBeAdded;
}

bool TrackEdge::areEntitiesPlacable() const
{
  return mAreEntitiesPlacable;
}

float TrackEdge::trackWidth() const
{
  return mTrackWidth;
}

TrackEdge::~TrackEdge()
{ 
  delete mSplinePath;
  delete mSplineSliceBeg;
  delete mSplineSliceEnd;
  if (mSurfaceCreated)
  {
    mVertices[0].clear();
    mVertices[1].clear();
    mVertices[2].clear();
  }
}

void TrackEdge::init()
{
  mPathTes  = int( mLength/float(mTesselationPerUnit));
  if ( mPathTes < 2 )
    mPathTes = 2;
  
  //TODO: DEBUG
  //mPathTes =20;
  //mPathTes = 10;
  //TODO: DEBUG

  mSliceTes   = 20;

}

TrackEdge::TrackEdge (  const std::vector<D3DXVECTOR3>& aBezierPointsPath, 
                        const std::vector<D3DXVECTOR3>& aBezierPointsSliceBeg,
                        const std::vector<D3DXVECTOR3>& aBezierPointsSliceEnd,
                        const D3DXVECTOR3& aInitialTangent, 
                        const D3DXVECTOR3& aLastTangent, 
                        const D3DXVECTOR3& aInitialNormal,
                        const D3DXVECTOR3& aLastNormal,
                        const float aTrackWidth,
                        const D3DXVECTOR3& aSliceMult,
                        const bool aCurvesCanBeAdded,
                        const bool aAreEntitiesPlacable)              
{
  mSliceMult            = aSliceMult ;
  mCurvesCanBeAdded     = aCurvesCanBeAdded;
  mAreEntitiesPlacable  = aAreEntitiesPlacable;
  mSplinePath       = new SplineBezierTMapped( &aBezierPointsPath[0], aBezierPointsPath.size() );
  mSplineSliceBeg   = new SplineBezierTMapped( &aBezierPointsSliceBeg[0], aBezierPointsSliceBeg.size() );
  mSplineSliceEnd   = new SplineBezierTMapped( &aBezierPointsSliceEnd[0], aBezierPointsSliceEnd.size() );

  D3DXVECTOR3 d = aBezierPointsPath[0] - aBezierPointsPath[aBezierPointsPath.size() - 1];
  mLength       = D3DXVec3Length( &d );
  mNumPointsOfSpline = mSplinePath->getNumPoints();

  mInitialTangent = aInitialTangent;
  mLastTangent    = aLastTangent;
  mInitialNormal  = aInitialNormal;
  mLastNormal     = aLastNormal;
  mTrackWidth     = aTrackWidth;
  mSurfaceCreated = false;
  init();
}

void  TrackEdge::generateLoftSurface ()
{  
  int lod[3] = {1,2,4};
  mVertices.resize(3);
  for ( int i = 0; i < 3; ++i )
  {    
    int sliceTes = (mSliceTes/lod[i] < 3) ? 3 : mSliceTes/lod[i];
    int pathTes =  (mPathTes/lod[i]  < 2) ? 2 : mPathTes/lod[i];
    if ( mPathTes < 2 ) mPathTes = 2;
    Loft::loftBezier(  mSplinePath, mSplineSliceBeg, mSplineSliceEnd,
                       pathTes, sliceTes,
                       mSliceMult,
                       mInitialTangent, 
                       mLastTangent,
                       mInitialNormal,
                       mLastNormal,
                       mVertices[i]);
  }
  mNumTrianglesMesh = mVertices[0].size() / 3;
  mSurfaceCreated = true;
}

void  TrackEdge::generateLoftSurfaceInt ( const LoftBoolean::LoftIntersectionInfo& aIntStart, 
                                          const LoftBoolean::LoftIntersectionInfo& aIntEnd )
{  
  int lod[3] = {1,2,4};
  mVertices.resize(3);
  for ( int i = 0; i < 3; ++i )
  {    
    int sliceTes = (mSliceTes/lod[i] == 0) ? 1 : mSliceTes/lod[i];
    int pathTes =   (mPathTes/lod[i]  < 2) ? 2 : mPathTes/lod[i];
    Loft::loftBezier(  mSplinePath, mSplineSliceBeg, mSplineSliceEnd,
                       pathTes, sliceTes,
                       mSliceMult,
                       mInitialTangent, 
                       mLastTangent,
                       mInitialNormal,
                       mLastNormal,
                       aIntStart,
                       aIntEnd,
                       true,
                       true,
                       mVertices[i]);
  }
  mNumTrianglesMesh = mVertices[0].size() / 3;
  mSurfaceCreated = true;
}

void  TrackEdge::generateLoftSurfaceIntStart ( const LoftBoolean::LoftIntersectionInfo& aIntStart )
{  
  int lod[3] = {1,2,4};
  mVertices.resize(3);
  for ( int i = 0; i < 3; ++i )
  {    
    int sliceTes = (mSliceTes/lod[i] == 0) ? 1 : mSliceTes/lod[i];
    int pathTes =   (mPathTes/lod[i]  < 2) ? 2 : mPathTes/lod[i];
    Loft::loftBezier(  mSplinePath, mSplineSliceBeg, mSplineSliceEnd,
                       pathTes, sliceTes,
                       mSliceMult,
                       mInitialTangent, 
                       mLastTangent,
                       mInitialNormal,
                       mLastNormal,
                       aIntStart,
                       aIntStart,
                       true,
                       false,
                       mVertices[i]);
  }
  mNumTrianglesMesh = mVertices[0].size() / 3;
  mSurfaceCreated = true;
}

void  TrackEdge::generateLoftSurfaceIntEnd ( const LoftBoolean::LoftIntersectionInfo& aIntEnd )
{  
  int lod[3] = {1,2,4};
  mVertices.resize(3);
  for ( int i = 0; i < 3; ++i )
  {    
    int sliceTes =  (mSliceTes/lod[i] < 3) ? 3 : mSliceTes/lod[i];
    int pathTes =   (mPathTes/lod[i]  < 2) ? 2 : mPathTes/lod[i];
    Loft::loftBezier(  mSplinePath, mSplineSliceBeg, mSplineSliceEnd,
                       pathTes, sliceTes,
                       mSliceMult,
                       mInitialTangent, 
                       mLastTangent,
                       mInitialNormal,
                       mLastNormal,
                       aIntEnd,
                       aIntEnd,
                       false,
                       true,
                       mVertices[i]);
  }
  mNumTrianglesMesh = mVertices[0].size() / 3;
  mSurfaceCreated = true;
}

LoftBoolean::LoftInfo  TrackEdge::loftInfo() const
{
  LoftBoolean::LoftInfo info;
  info.aSplinePath = mSplinePath;
  info.aSplineSliceBeg = mSplineSliceBeg;
  info.aSplineSliceEnd = mSplineSliceEnd;
  info.aPathTes = mPathTes;  
  info.aSliceTes = mSliceTes;
  info.aSliceMult = mSliceMult;
  info.aInitialTangent = mInitialTangent;
  info.aLastTangent = mLastTangent;
  info.aInitialNormal = mInitialNormal;
  info.aLastNormal = mLastNormal;
  return info;
}

DxLodModel<VertPNTUV>* TrackEdge::model() const
{
  DxLodModel<VertPNTUV>*   mesh;
  std::vector<D3DXVECTOR3> testPoints;
  
  const float length = integrateLength();
  const float pathTesellation = length / mFustrumResolution;
  const int numIterations = length/mFustrumResolution + (fmod(length, mFustrumResolution) != 0.0f);
  for ( int i = 0; i <= numIterations ; ++i )
  {
    float t = i/float(numIterations);
    t = min(t,1.0f);
    testPoints.push_back( mSplinePath->getPoint( t ) );
  }
  
  mesh = new DxLodModel<VertPNTUV>( mVertices, testPoints );
    

  return mesh;
}

MeshCol* TrackEdge::meshCol() const
{  
  std::vector<VertexPosNor> v ( mVertices[0].size() );
  for ( unsigned int i = 0 ; i < mVertices[0].size(); ++i )
  {
    v[i].pos = mVertices[0][i].pos;
    v[i].nor = mVertices[0][i].nor;
  }
  MeshCol* m = new MeshCol( v ) ;
  return m;
}


std::vector<MeshCol*> TrackEdge::meshColTri() const
{  
  std::vector<MeshCol*>     m  ( (mVertices[0].size())/3 );
  std::vector<VertexPosNor>  v  ( 3 );
  for ( unsigned int i = 0 ; i < mVertices[0].size(); ++i )
  {
    v[i%3].pos = mVertices[0][i].pos;
    v[i%3].nor = mVertices[0][i].nor;
    m[i/3] = new MeshCol( v ) ;    
  }  
  return m;
}


float TrackEdge::integrateLength() const
{
  int numSteps = 1000;
  float length = 0.0f;

  for ( int i = 0; i < numSteps ; ++i )
  {
    float t0 = float(i + 0) / float(numSteps);
    float t1 = float(i + 1) / float(numSteps);

    D3DXVECTOR3 p0 = mSplinePath->getPoint( t0 );
    D3DXVECTOR3 p1 = mSplinePath->getPoint( t1 );

    length += DxMath::distance( p0, p1 );
  }

  return length;
}

void  TrackEdge::writeLog( std::ofstream* aStream )
{
  *aStream << "Num Points:    " << mNumPointsOfSpline << std::endl;
  *aStream << "Num Triangles: " << mNumTrianglesMesh << std::endl;
  //*aStream << "Distance: " << mNumTrianglesMesh;
  *aStream << "Length:        " << mLength << std::endl;
  *aStream << "Ratio tri/leng:" << float(mNumTrianglesMesh)/mLength << std::endl;
  *aStream << "Tesellation:   " << mPathTes << std::endl;
  
}

void TrackEdge::addEntity( const PlacedEntity& aEntity, Bumper* aBumper )
{
  addEntity(aEntity);
}

void TrackEdge::addEntity( const PlacedEntity& aEntity, Arch* aArch )
{
  addEntity(aEntity);
}


void TrackEdge::addEntity( const PlacedEntity& aEntity )
{
  mPlacedEntity.push_back( aEntity );
  std::sort(  mPlacedEntity.begin(), 
              mPlacedEntity.end(), 
              PlacedEntity::comparePlacedEntities );
  
}
/*
  aUnits    number of units to advance
  aT        current t in spline
  aNewT     next position of entity OR end of spline

*/
bool    TrackEdge::advanceAlongPathNunits( 
                      const float aUnits, 
                      const float aT, 
                      float& aNewT,                       
                      float& aAdvancedUnits,
                      bool& aMaxAngleSurpassed,
                      const float aMaxAngle,                                                                                   
                      const D3DXVECTOR3& aBallPos,
                      const D3DXVECTOR3& aBallVel,
                      BallIA::Target& aOutTarget)
{
  PlacedEntity nextEntity    = nextEntitySplineT( aT );
  float tangentialComponent  = nextEntity.mBitangentialComponent;

  bool ret = mSplinePath->advanceAlongPathNunits(  aUnits, 
                                        aT, 
                                        nextEntity.mSplineT,
                                        aNewT, 
                                        aAdvancedUnits,
                                        aMaxAngleSurpassed,
                                        aMaxAngle,                                                            
                                        aBallPos,
                                        aBallVel,
                                        aOutTarget);

  if ( aOutTarget.hasEntity )
  {
    aOutTarget.entityBitangentComponent = tangentialComponent;
    aOutTarget.splineBitangent = Loft::bitangentBezier( 
                        mSplinePath, nextEntity.mSplineT, 
                        mInitialTangent, mLastTangent,
                        mInitialNormal, mLastNormal );
  }

  return ret;
}

float    TrackEdge::advanceAlongPath( const float aUnits, const float aT  )
{
  float newT; float dummy; bool dummybool;
  D3DXVECTOR3 dummyV3;
  BallIA::Target dummTarget;

  bool inside = mSplinePath->advanceAlongPathNunits(  aUnits, 
                                        aT, 
                                        10000,
                                        newT, 
                                        dummy,
                                        dummybool,
                                        1000,                                                            
                                        dummyV3,
                                        dummyV3,
                                        dummTarget);
  if ( inside )
    return newT;
  else
    return 1;
}

//  returns the t-position in the spline going forward
//  from aT, of the next entity, if none, 1 is returned instead
PlacedEntity TrackEdge::nextEntitySplineT( const float aT )
{
  int i = 0;
  PlacedEntity nullEntity( 1, 0 );

  for( int i = 0; i < mPlacedEntity.size() ; ++i )
  {
    if ( mPlacedEntity[i].mSplineT > aT )    
        return mPlacedEntity[i];
    
  }

  return nullEntity;
}