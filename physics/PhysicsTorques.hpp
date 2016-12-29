#ifndef _PHYSICS_TORQUES_HPP_
#define _PHYSICS_TORQUES_HPP_

#include <d3dx10.h>
#include <dxerr.h>
#include <string>
#include <stdio.h>
#include <vector>



//===========================================================================
/*

     Individual particle
*/
//===========================================================================
struct TimeTorqueState{
  //  Constants
  float     t;        /*!< Current time in force space*/
  bool      advancing;/*!< true <=> the force is advancing in force space */
  
};

 

//===========================================================================
/*!
      Time Force
      \brief      A force which varies over time, which is described by
                  a two dimensional function.
*/
//===========================================================================
class TimeTorque
{
public:

  //int                       mNumBodies; /*!< Number of particles */
  //std::vector<RigidBody>    mBodies;   /*!< Vector of particles*/

  TimeTorque( D3DXVECTOR3* aApplicationVectorLS, D3DXVECTOR3* aForceVectorLS );

  void          updateState ( TimeTorqueState aState );
  void          initialState ();
  D3DXVECTOR3   getTorque ( float aDeltaT, const D3DXMATRIX* aWorld );

private:

  float           mT;         /*!< Current time in force space*/
  float           mDeltaT;    /*!< Current time delta multiplier */
  float           mTimeMax;   /*!< Maximum time in force space*/

  D3DXVECTOR3     mApplicationPositionLS; /*!< in which point does the force act*/
  D3DXVECTOR3     mForceVectorLS;     /*!< in which direction does the force act*/

  friend class Ball;
};



#endif