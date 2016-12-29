#include	"Physics/PhysicsForces.hpp"

//===========================================================================
/*!
    Constructor.
    \param  aApplicationVector  in which direction does hte force act
*/
//===========================================================================

TimeForce::TimeForce( D3DXVECTOR3* aApplicationVector, StoredSplineDesc* aFunction, float aModule )
{
  mModule   = aModule;
  mT        = 0.0f;
  mDeltaT   = 0.0f;
  mTimeMax  = 20.0f;

  mApplicationVector = *aApplicationVector;

  mFunction = new SplineLengthMapped( aFunction );
}


//===========================================================================
/*!
    Updates the state of the force: in which time position of its
    own space it is, and if it's going forward on time
    \param      aState    New state: time ellapsed since last update
*/
//===========================================================================

void    TimeForce::updateState ( TimeForceState aState )
{
  if ( aState.advancing )
  {
    mT += aState.t;
  }
  else
  {
    mT -= aState.t;
  }

  //  Check bounds
  if ( mT < 0.0f ) mT = 0.0f;
  if ( mT > mTimeMax ) mT = mTimeMax;
}
 

//===========================================================================
/*!
    Sets the force to its repose state (initial).
*/
//===========================================================================

void    TimeForce::initialState ()
{  
  mT = 0.0f;
}

//===========================================================================
/*!
    Gets the force at aDeltaT
    \param      aDeltaT    time position, simulation space
    \param      aWorld      matrix that transforms LS into WS
*/
//===========================================================================

D3DXVECTOR3 TimeForce::getForce ( float aDeltaT, const D3DXMATRIX *aWorld )
{
  D3DXVECTOR3 ret;

  float interpolation = mFunction->getPoint( mT/mTimeMax );
  //float interpolation = mT/mTimeMax;
  float module = mModule*interpolation;

  D3DXVec3TransformCoord ( &ret, &(module * mApplicationVector), aWorld );

  if ( ret.x == -0.0 ) ret.x = 0.0;
  if ( ret.y == -0.0 ) ret.y = 0.0;
  if ( ret.z == -0.0 ) ret.z = 0.0;

  return ret;
}

void    TimeForce::setApplicationVector( const D3DXVECTOR3& aApplicationVector )
{
  mApplicationVector = aApplicationVector;
}