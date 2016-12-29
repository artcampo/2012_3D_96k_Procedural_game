#include	"Physics/PhysicsTorques.hpp"

//===========================================================================
/*!
    Constructor.
    \param  aApplicationVector  in which direction does hte force act
*/
//===========================================================================

TimeTorque::TimeTorque( D3DXVECTOR3* aApplicationPositionLS, D3DXVECTOR3* aForceVectorLS  )
{
  mT        = 0.0f;
  mDeltaT   = 0.0f;
  mTimeMax  = 10.0f;

  mApplicationPositionLS  = *aApplicationPositionLS;
  mForceVectorLS          = *aForceVectorLS;
}


//===========================================================================
/*!
    Updates the state of the force: in which time position of its
    own space it is, and if it's going forward on time
    \param      aState    New state: time ellapsed since last update
*/
//===========================================================================

void    TimeTorque::updateState ( TimeTorqueState aState )
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

void TimeTorque::initialState ()
{
  mT = 0;
}

//===========================================================================
/*!
    Gets the force at aDeltaT
    \param      aDeltaT     time position, simulation space
    \param      aWorld      matrix that transforms LS into WS
*/
//===========================================================================

D3DXVECTOR3 TimeTorque::getTorque ( float aDeltaT, const D3DXMATRIX *aWorld )
{
  D3DXVECTOR3 ret;

  D3DXVECTOR3 fLS = mForceVectorLS;
  D3DXVECTOR3 posWS, fWS;

  D3DXVec3TransformCoord ( &posWS, &mApplicationPositionLS, aWorld );
  D3DXVec3TransformCoord ( &fWS, &fLS, aWorld );

  D3DXVec3Cross ( &ret, &posWS, &fWS ); 

  if ( ret.x == -0.0 ) ret.x = 0.0;
  if ( ret.y == -0.0 ) ret.y = 0.0;
  if ( ret.z == -0.0 ) ret.z = 0.0;

  return ret;
}