#include  "GameBall/Track/TrackPath.hpp"

TrackPath::TrackPath( const int aId )
{
  //mIsEdge = aIsEdge;
  mSurfaceCreated = false;
  mId = aId;
}

void TrackPath::debugAddVbId( const int id )
{
  mDebugVbId = id;
  
}

TrackPath::~TrackPath()
{
  for ( int i = 0; i < mEdges.size() ; ++i )
    delete mEdges[i];
  for ( int i = 0; i < mEdgeEnds.size() ; ++i )
    delete mEdgeEnds[i];
}


void TrackPath::init()
{
}

void TrackPath::addEdge(const std::vector<D3DXVECTOR3>& aBezierPointsPath, 
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

  int index = mEdges.size();
  mEdges.resize ( index + 1 );
  mEdges[index] = new TrackEdge(  aBezierPointsPath, 
                                  aBezierPointsSliceBeg, 
                                  aBezierPointsSliceEnd,
                                  aInitialTangent,
                                  aLastTangent,
                                  aInitialNormal,
                                  aLastNormal,
                                  aTrackWidth,
                                  aSliceMult,
                                  aCurvesCanBeAdded,
                                  aAreEntitiesPlacable);
}

void  TrackPath::generateLoftSurface ()
{  

  //*Global::mFileDebugTrack << "VB: "<<mDebugVbId<< std::endl;
  for ( int i = 0; i < mEdges.size() ; ++i )
  {
      mEdges[i]->generateLoftSurface();      
  }
  for ( int i = 0; i < mEdgeEnds.size() ; ++i )
    mEdgeEnds[i]->generateLoftSurface();

  mSurfaceCreated = true;
}

std::vector<DxLodModel<VertPNTUV>*> TrackPath::model()
{
  if ( !mSurfaceCreated )
    generateLoftSurface();

  std::vector<DxLodModel<VertPNTUV>*>   meshes ( mEdges.size() );

  for ( int i = 0; i < mEdges.size() ; ++i )
    meshes[i] = mEdges[i]->model();

  return meshes;
}

std::vector<MeshCol*> TrackPath::meshColTri() 
{  
  if ( !mSurfaceCreated )
    generateLoftSurface();

  std::vector<MeshCol*>     m ;

  for ( unsigned int i = 0 ; i < mEdges.size(); ++i )
  {
    std::vector<MeshCol*> p = mEdges[i]->meshColTri();
    m.insert(m.end(), p.begin(), p.end());
  }  
  return m;
}

//  TODO: TRACKPATH
float TrackPath::integrateLength() const
{
  if ( mEdges[0]->curvesCanBeAdded() )
    return mEdges[0]->integrateLength();
  else
    return 0;
}

void  TrackPath::writeLog( std::ofstream* aStream )
{

  for ( int i = 0; i < mEdges.size() ; ++i )
    mEdges[i]->writeLog( aStream );    
}


//  Adds an spline based edge with only two points
void TrackPath::addEdge ( const TrackBp::Vertex& aVertA, 
                          const TrackBp::Vertex& aVertB, 
                          const float aLengthTan, 
                          const std::vector<D3DXVECTOR3>& aBezierPointsSliceBeg,
                          const std::vector<D3DXVECTOR3>& aBezierPointsSliceEnd,
                          const float aTrackWidth,
                          const D3DXVECTOR3& aSliceMult,
                          const bool aCurvesCanBeAdded,
                          const bool aAreEntitiesPlacable)
{

    std::vector<D3DXVECTOR3> path;
    path = Loft::splinePath(  aVertA.pos, aVertA.tan,
                              aVertB.pos, aVertB.tan, 
                              aLengthTan );

    addEdge( path, aBezierPointsSliceBeg, aBezierPointsSliceEnd,
            aVertA.tan,
            aVertB.tan,
            aVertA.nor,
            aVertB.nor,
            aTrackWidth,
            aSliceMult,
            aCurvesCanBeAdded,
            aAreEntitiesPlacable );
}


void    TrackPath::addDescendant( TrackPath* aDesc )
{
  mDescendants.push_back( aDesc );
}

void    TrackPath::addParent( TrackPath* aDesc )
{
  mParents.push_back( aDesc );
}

int     TrackPath::numDescendants() const
{
  return mDescendants.size();
}

int     TrackPath::numParents() const
{
  return mParents.size();
}

TrackPath*  TrackPath::descendant( const int aIndex ) const
{
  return mDescendants[aIndex];
}


TrackPath*  TrackPath::parent( const int aIndex ) const
{
  return mParents[aIndex];
}

LoftBoolean::LoftInfo   TrackPath::loftInfoFirstEdge() const
{
  return mEdges[0]->loftInfo();
}

LoftBoolean::LoftInfo   TrackPath::loftInfoLastEdge() const
{
  return mEdges[ (mEdges.size() - 1) ]->loftInfo();
}

void  TrackPath::setLoftIntersectLastEdge( LoftBoolean::LoftIntersectionInfo& aInfo )
{  
  mEndIntersectionInfo = aInfo;
}

void  TrackPath::setLoftIntersectFirstEdge( LoftBoolean::LoftIntersectionInfo& aInfo )
{
  mStartIntersectionInfo = aInfo; 
}