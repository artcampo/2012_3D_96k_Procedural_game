#include  "Geometry/DirGraph/DirGraph.hpp"
#include  "GameBall/Track/TrackGraphCollision.hpp"

namespace DirectedGraph{


////////////////////////////////////////////////////////
//  modifying functions
///////////////////////////////////////////////////////

void  DirGraph::setRoot( const int aIdv )
{
  mRoot = aIdv;
}

int   DirGraph::newEdge( const int aIdv1, const int aIdv2, const TrackBp::EdgeData& aEdge )
{
  bool found = false;
  for( int i = 0; i < mVertices[aIdv1].desc.size() ; ++i )
      if ( mVertices[aIdv1].desc[i] == aIdv2 ) found = true;

  if ( !found )
  {
    
    mVertices[aIdv1].desc.push_back ( aIdv2 );
    mVertices[aIdv1].edgeData.push_back ( aEdge );
    mVertices[aIdv2].par.push_back  ( aIdv1 );  
  }

  return 0;
}

int   DirGraph::newNode( const TrackBp::Vertex& aData, const bool aIsMutable )
{
  int index = mVerticesData.size();
  mVerticesData.push_back( aData );

  mVertices.resize            ( mVerticesData.size() );
  mVerticesIsMutable.resize   ( mVerticesData.size() );

  mVerticesIsMutable[index] = aIsMutable;

  return index;
}


void  DirGraph::deleteDescendants ( const int aIdv )
{
  for ( int i = 0; i < mVertices[aIdv].desc.size() ; ++i )
    deleteParent( mVertices[aIdv].desc[i], aIdv );
  mVertices[aIdv].desc.clear();
  mVertices[aIdv].edgeData.clear();
}

void  DirGraph::deleteParents     ( const int aIdv )
{
  for ( int i = 0; i < mVertices[aIdv].par.size() ; ++i )
    deleteDescendant( mVertices[aIdv].par[i], aIdv );
  mVertices[aIdv].par.clear();
}

void  DirGraph::deleteDescendant ( const int aIdv1, const int aIdv2 )
{
  for ( int i = 0; i < mVertices[aIdv1].desc.size() ; ++i )
    if ( mVertices[aIdv1].desc[i] == aIdv2 )
    {
      mVertices[aIdv1].desc.erase( mVertices[aIdv1].desc.begin() + i);
      mVertices[aIdv1].edgeData.erase( mVertices[aIdv1].edgeData.begin() + i);
      return;
    }    
}

void  DirGraph::deleteParent ( const int aIdv1, const int aIdv2 )
{
  for ( int i = 0; i < mVertices[aIdv1].par.size() ; ++i )
    if ( mVertices[aIdv1].par[i] == aIdv2 )
    {
      mVertices[aIdv1].par.erase( mVertices[aIdv1].par.begin() + i);
      return;
    }    
}

////////////////////////////////////////////////////////
//  consulting functions
///////////////////////////////////////////////////////

std::vector<EdgeCon> DirGraph::edgesAndConnectivity() const
{
  std::vector<EdgeCon> edges;
  edgesRec( 0, 0, edges );
  return edges;
}

std::vector<Edge> DirGraph::edges() const
{
  std::vector<Edge> edges;
  edgesRec( 0, edges );
  return edges;
}

void DirGraph::edgesRec( const int aIdv, std::vector<Edge>& aEdges ) const
{
  for ( int i = 0; i < mVertices[aIdv].desc.size() ; ++i )
  {
    Edge e ( aIdv, mVertices[aIdv].desc[i], mVertices[aIdv].edgeData[i] );
    bool found = false;
    for( int i = 0; i < aEdges.size() ; ++i )
      if ( aEdges[i] == e ) found = true;
    if ( !found )
      aEdges.push_back( e );
  }

  for ( int i = 0; i < mVertices[aIdv].desc.size() ; ++i )
    edgesRec( mVertices[aIdv].desc[i], aEdges );
}

Edge DirGraph::edgesRec( const int aIdv, const int aDescIndex, std::vector<EdgeCon>& aEdges ) const
{
  Edge e ( aIdv, mVertices[aIdv].desc[aDescIndex], mVertices[aIdv].edgeData[aDescIndex] );
  bool found = false;
  for( int i = 0; i < aEdges.size() ; ++i )
    if ( aEdges[i].e == e ) 
      return aEdges[i].e;

  EdgeCon edgeCon;
  edgeCon.e = e;
  
  int childrenVertex = mVertices[aIdv].desc[aDescIndex];
  for ( int i = 0; i < mVertices[childrenVertex].desc.size() ; ++i )
  {
    Edge children = edgesRec( childrenVertex, i, aEdges );
    edgeCon.desc.push_back( children );
  }

  aEdges.push_back( edgeCon );
  return edgeCon.e;  
}

TrackBp::Vertex     DirGraph::vertex( const int aIdv ) const
{
  return mVerticesData[aIdv];
}



int               DirGraph::root() const
{
  return mRoot;
}

int               DirGraph::numVertices() const
{
  return mVertices.size();
}

int               DirGraph::numOutEdges( const int aIdv ) const
{
  return mVertices[aIdv].edgeData.size();
}

TrackBp::EdgeData          DirGraph::edge    ( const int aIdv1, const int aIdv2 ) const
{
  int index = 0;
  if ( !StlHelpers::findInVector( mVertices[aIdv1].desc, aIdv2, index ) )
    index = 0;  //TODO: say error
  
  return mVertices[aIdv1].edgeData[index];
}

void          DirGraph::setEdge    ( const int aIdv1, const int aIdv2, const TrackBp::EdgeData& aData  )
{
  int index = 0;
  if ( !StlHelpers::findInVector( mVertices[aIdv1].desc, aIdv2, index ) )
    return;  //TODO: say error
  
  mVertices[aIdv1].edgeData[index] = aData;
}

void  DirGraph::setVertex ( const int aIdv, TrackBp::Vertex& aData, const bool aIsMutable  )
{
  mVerticesData[aIdv] = aData;
  mVerticesIsMutable[aIdv] = aIsMutable;
}

std::vector<int>  DirGraph::descendants ( const int aIdv ) const
{
  return mVertices[aIdv].desc;
}

std::vector<int>  DirGraph::parents     ( const int aIdv ) const
{
  return mVertices[aIdv].par;
}

int               DirGraph::numDesc ( const int aIdv ) const
{
  return mVertices[aIdv].desc.size();
}

int               DirGraph::numPar  ( const int aIdv ) const
{
  return mVertices[aIdv].par.size();
}

bool              DirGraph::hasArityTwo( const int aIdv ) const
{
  return ( numDesc( aIdv ) <= 1 && numPar( aIdv ) <= 1 );
}


bool DirGraph::isVertexMutable  ( const int aIdv ) const
{
  return mVerticesIsMutable[aIdv];
}

std::vector<int> DirGraph::mutableVertices() const
{
  std::vector<int> ids;

  for ( int i = 0; i < mVertices.size() ; ++i )
  {
    if (mVerticesIsMutable[i] )
      ids.push_back( i );
  }

  return ids;
}

TrackGraphCollision::CollisionData  DirGraph::collisionData( const TrackGraphCollision::CollisionConfiguration& aCollisionConfiguration ) const
{
  TrackGraphCollision::CollisionData colData;

  colData.mEdges.resize( mVertices.size() );
  for ( int i = 0; i < mVertices.size() ; ++i )
  {
    int vb = i;
    colData.mEdges[i].resize( mVertices[i].desc.size() );
    for ( int j = 0; j < mVertices[i].desc.size() ; ++j )
    {
      int ve = mVertices[i].desc[j];
      colData.mEdges[i][j].mVertexEnd = mVertices[i].desc[j];
      colData.mEdges[i][j].mSpheres   = TrackGraphCollision::boundingSpheresOfEdge( 
            aCollisionConfiguration,
            vertex(vb).pos,
            vertex(ve).pos );

    }
  }  
  return colData;
}

/*
  CAREFUL: expensive!
*/
void    DirGraph::deleteVertex( const int aIdv )
{
  for ( int i = aIdv; i < mVertices.size() - 1 ; ++i )
  {
    mVertices[i]          = mVertices[i + 1];
    mVerticesData[i]      = mVerticesData[i + 1];
    mVerticesIsMutable[i] = mVerticesIsMutable[i + 1];
  }

  mVertices.resize          ( mVertices.size()          - 1 );
  mVerticesData.resize      ( mVerticesData.size()      - 1 );
  mVerticesIsMutable.resize ( mVerticesIsMutable.size() - 1 );

  for ( int i = 0; i < mVertices.size(); ++i )
  {
    for ( int j = 0; j < mVertices[i].desc.size() ; ++j )
      if( mVertices[i].desc[j] > aIdv ) --mVertices[i].desc[j];
    for ( int j = 0; j < mVertices[i].par.size() ; ++j )
      if( mVertices[i].par[j] > aIdv ) --mVertices[i].par[j];
  }
}

} //  end namespace DirectedGraph
