#include  "GameBall/Metropolis/MetropolisStructure.hpp"
#include  "Noise/RandomNumberGenerator.hpp"
#include  "Geometry/Intersections.hpp"
#include  "RenderModels/Metropolis/dx_metarm_model.hpp"

using namespace MetData;

MetropolisStructure::MetropolisStructure( Track& aTrack, 
                                          Rand::RandGen& aRandGen)              
  : mTrack( aTrack ),
    mVertexDistance( 50 ),
    mDistanceToEdge( 40 ),
    mProbArmAtEdge( 1 ),
    mRand( aRandGen ),
    mDistanceBetweenTries( 40 ),
    mEdgeMinimumLengthForPlacing( 120 ),
    mEdgeTesellationToSegment( 50 )
{
  chooseEdges();
  placeStructuresAtEdges();
}


void    MetropolisStructure::display( DxFustrum& aFustrum )
{
  for ( int i = 0 ; i < mArms.size(); ++i )
    mArms[i]->display();

}

void    MetropolisStructure::addEdgeToCollisionStructure( TrackEdge* aEdge )
{
  bool  finished  = false;
  float t         = 0;
  std::vector<D3DXVECTOR3> p;
  p.push_back( aEdge->mSplinePath->getPoint(t) );
  while (!finished )
  {
    t = aEdge->advanceAlongPath( mEdgeTesellationToSegment, t );

    if( t >= 1)
    {
      finished  = true;
      t         = 1;
    }
    p.push_back( aEdge->mSplinePath->getPoint(t) );
  }

  mEdgesCollision.push_back( EdgeCol( p ) );
}

void    MetropolisStructure::chooseEdgeForPlacing( TrackEdge* aEdge )
{
  float length = aEdge->integrateLength();
  if ( length > mEdgeMinimumLengthForPlacing )
  {
    mEdgesChosenForPlacement.push_back( EdgeSeg( aEdge, length ) ) ;
  }
}

void    MetropolisStructure::chooseEdges()
{
  for ( int i = 0 ; i < mTrack.mPath.size(); ++i )
  {
    for ( int j = 0 ; j < mTrack.mPath[i]->mEdges.size(); ++j )
    {
      TrackEdge* edge = mTrack.mPath[i]->mEdges[j];

      addEdgeToCollisionStructure( edge );
      chooseEdgeForPlacing( edge );

    }
  }  
}

void    MetropolisStructure::placeStructuresAtEdges()
{
  for ( int i = 0 ; i < mEdgesChosenForPlacement.size(); ++i )
    placeStructureAtEdge( mEdgesChosenForPlacement[i] );
}

void    MetropolisStructure::placeStructureAtEdge( MetData::EdgeSeg& aEdge )
{
  float   mDistanceBetweenTries = 40;

  float   placableLength = aEdge.length - 2*mVertexDistance;

  //  if there's not enough length, don't put anything
  if( placableLength < mDistanceBetweenTries )
    return;

  //  determine in how many positions it can be placed
  int     numPositions        = int( floor( placableLength/mDistanceBetweenTries) );
  std::vector<int> positions  = mRand.genPermutations( numPositions );

  bool        finished        = false;

  for( int i = 0; !finished && i < numPositions ; ++i )
  {
    std::vector<int> structures  = mRand.genPermutations(2);

    //  Choose one point in between, get its tangent and normal
    float       moduleLength    = mVertexDistance + mDistanceBetweenTries*positions[i];     
    float t                     = aEdge.edge->advanceAlongPath( moduleLength, 0 );
    D3DXVECTOR3 directorVector  = aEdge.edge->mSplinePath->getPointTangent( t );
    directorVector.y = 0;
    D3DXVec3Normalize( &directorVector, &directorVector ); 
    D3DXVECTOR3 normalVector    = D3DXVECTOR3( -directorVector.z, 0, directorVector.x );
    D3DXVECTOR3 pos             = aEdge.edge->mSplinePath->getPoint( t );

    for( int j = 0; !finished && j < structures.size() ; ++j )
    {
      bool placed;
      if ( structures[j] == 0) placed = placeStructureAtEdge1( pos, directorVector, normalVector );
      if ( structures[j] == 1) placed = placeStructureAtEdge2( pos, directorVector, normalVector );

      if (placed) finished = true;
    }
        
  }
}

bool MetropolisStructure::placeStructureAtEdge1( const D3DXVECTOR3& pos, 
                                  const D3DXVECTOR3& directorVector, 
                                  const D3DXVECTOR3& normalVector   )
{
  bool        placed = false;
  
  float       sideSign      = (mRand.randFloat_0_1() < 0.5) ? 1 : -1;
  D3DXVECTOR3 positionArm = pos                          
                          + sideSign*mDistanceToEdge*normalVector;

  if ( armPositionIsValid( positionArm, mDistanceToEdge*0.9f) )
  {    
    placeArmEdge1( positionArm );
    placed = true;
  }
  return placed;
}

bool MetropolisStructure::placeStructureAtEdge2( const D3DXVECTOR3& pos, 
                                  const D3DXVECTOR3& directorVector, 
                                  const D3DXVECTOR3& normalVector   )
{
  bool        placed = false;  
  
  D3DXVECTOR3 positionArm1 = pos                          
                            + mDistanceToEdge*normalVector;
  D3DXVECTOR3 positionArm2 = pos                          
                            - mDistanceToEdge*normalVector;

  if ( armPositionIsValid( positionArm1, mDistanceToEdge*0.9f ) 
    && armPositionIsValid( positionArm2, mDistanceToEdge*0.9f ) )
  {    
    placeArmEdge1( positionArm1 );
    placeArmEdge1( positionArm2 );
    placed = true;
  }
  return placed;
}


//  returns true if the position is farther than aDist to every edge
bool MetropolisStructure::armPositionIsValid( const D3DXVECTOR3& aPos, const float aDist )
{
  bool    isFarEnough = true;

  for( int i = 0; isFarEnough && i < mEdgesCollision.size() ; ++i )
  {
    for( int j = 0; isFarEnough && j < mEdgesCollision[i].mSeg.size() - 1 ; ++j )
    {
      float distance = Intersections::distancePointToSegment(
            mEdgesCollision[i].mSeg[j],
            mEdgesCollision[i].mSeg[j + 1],
            aPos );
      isFarEnough = distance > aDist;
    }
  }

  return  isFarEnough;
}

void MetropolisStructure::placeArmEdge1( const D3DXVECTOR3& aPos )
{
  D3DXVECTOR3 top     = aPos + D3DXVECTOR3(0,  50, 0 );
  D3DXVECTOR3 bottom  = aPos + D3DXVECTOR3(0, -50, 0 );
  mArms.push_back( new MetArmModel( top, bottom ) );
}

void MetropolisStructure::placeArmEdge2( const D3DXVECTOR3& aPos1, const D3DXVECTOR3& aPos2 )
{
}