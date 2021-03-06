#ifndef _PHYSICS_FORCES_HPP_
#define _PHYSICS_FORCES_HPP_

#include <d3dx10.h>
#include <dxerr.h>
#include <string>
#include <stdio.h>
#include <vector>

#include "Geometry/Spline/SplineLengthMapped.hpp"
#include "Project_Dependant/StoredSplines.hpp"

//===========================================================================
/*

     Individual particle
*/
//===========================================================================
struct TimeForceState{
  //  Constants
  float     t;        /*!< Current time in force space*/
  bool      advancing;/*!< true <=> the force is advancing in force space */
  
  //D3DXMATRIX    iLS;    /*!< Inertia tensor*/
  //D3DXQUATERNION  qWS;   /*!< Rotation quaternion*/
  //D3DXVECTOR3   tWS;  /*!< Torque*/
  

};

 

//===========================================================================
/*!
      Time Force
      \brief      A force which varies over time, which is described by
                  a two dimensional function.
*/
//===========================================================================
class TimeForce
{
public:

  //int                       mNumBodies; /*!< Number of particles */
  //std::vector<RigidBody>    mBodies;   /*!< Vector of particles*/

  TimeForce( D3DXVECTOR3* aApplicationVector, StoredSplineDesc* aFunction, float aModule );
  void    updateState ( TimeForceState aState );
  void    initialState ();

  D3DXVECTOR3   getForce ( float aDeltaT, const D3DXMATRIX *aWorld );
  void    setApplicationVector( const D3DXVECTOR3& aApplicationVector );

private:

  float           mModule;
  float           mT;         /*!< Current time in force space*/
  //TODO: se utiliza???
  float           mDeltaT;    /*!< Current time delta multiplier */
  float           mTimeMax;/*!< Maximum time in force space*/

  D3DXVECTOR3     mApplicationVector; /*!< in which direction does hte force act*/

  SplineLengthMapped*    mFunction; /*!< Spline with f(t)*/

};



#endif