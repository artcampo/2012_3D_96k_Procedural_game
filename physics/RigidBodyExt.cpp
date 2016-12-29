#include	"Physics/RigidBodyExt.hpp"



RigidBodyExt::RigidBodyExt( const float aMass,                 
                            const float aBoundingRadius,
                            const float aDragForce,
                            const float aBounceCoefficient )
{

  mHasMesh  = false;  
  
  mDragForce            = aDragForce;
  mBounceCoefficient    = aBounceCoefficient;  
  mBoundingSphereRadius = aBoundingRadius;  
  m                     = aMass;

  iInvLS = D3DXMATRIX (
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    );

  iLS = D3DXMATRIX (
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    );

}


TimeForce*  RigidBodyExt::attachTimeForce    ( D3DXVECTOR3* aApplicationVector, StoredSplineDesc* aFunction, float aModule )
{  
  int index = mTimeForces.size();
  mTimeForces.resize ( index + 1 );
  mTimeForces[ index ] = new TimeForce( aApplicationVector, aFunction, aModule) ;
  return mTimeForces[ index ];
}

//===========================================================================
/*!
    Includes a time-Torque to body.   
*/
//===========================================================================
TimeTorque* RigidBodyExt::attachTimeTorque   ( D3DXVECTOR3* aApplicationVectorLS, D3DXVECTOR3* aForceVectorLS )

{
  int index = mTimeTorques.size();
  mTimeTorques.resize ( index + 1 );
  mTimeTorques[ index ] = new TimeTorque( aApplicationVectorLS, aForceVectorLS );
  return mTimeTorques[ index ];
}


void  RigidBodyExt::setMaxVelocity( const float aMaxVelocity )
{
  mMaxVelocity = aMaxVelocity;
}
