/*!
 * \file StoredSplines.cpp
 * \brief Static data with splines arrays
 * \author Arturo
 */


#include    "Project_Dependant/StoredSplines.hpp"

float gSplineFwdForce[21*4] = {
  0.00000f,  0.00000f, 0.00000f, 1.00000f ,
  00.00000f, 0.10000f, 0.00000f, 1.00000f ,
  10.00000f, 0.20000f, 0.00000f, 1.00000f ,
  30.00000f, 0.30000f, 0.00000f, 1.00000f ,
  70.00000f, 0.40000f, 0.00000f, 1.00000f ,

  75.00000f, 0.50000f, 0.00000f, 1.00000f ,
  80.00000f, 0.60000f, 0.00000f, 1.00000f ,
  85.00000f, 0.70000f, 0.00000f, 1.00000f ,
  90.00000f, 0.80000f, 0.00000f, 1.00000f ,
  90.00000f, 0.90000f, 0.00000f, 1.00000f ,

  90.00000f, 1.00000f, 0.00000f, 1.00000f ,
  90.00000f, 1.00000f, 0.00000f, 1.00000f ,
  90.00000f, 1.00000f, 0.00000f, 1.00000f ,
  90.00000f, 1.00000f, 0.00000f, 1.00000f ,
  90.00000f, 1.00000f, 0.00000f, 1.00000f ,

  90.00000f, 1.00000f, 0.00000f, 1.00000f ,
  90.00000f, 1.00000f, -0.00000f, 1.00000f ,
  90.00000f, 1.00000f, -0.00000f, 1.00000f ,
  90.00000f, 1.00000f, -0.00000f, 1.00000f ,
  90.00000f, 1.00000f, -0.00000f, 1.00000f 
};

StoredSplineDesc gShipTForceFwdSpline = { gSplineFwdForce, 21 };


D3DXVECTOR3 Spline_SliceNoBorder[22] = {
  D3DXVECTOR3( 0 , -20 , 40 ), 
  D3DXVECTOR3( 0 , -10 , 60 ), 
  D3DXVECTOR3( 0 , -0.429043 , 46.6893 ), 
  D3DXVECTOR3( 0 , 0 , 40 ), 
  D3DXVECTOR3( 0 , -0.231024 , 33.6503 ), 
  D3DXVECTOR3( 0 , 0.330031 , 38.0985 ), 
  D3DXVECTOR3( 0 , 0 , 30 ), 
  D3DXVECTOR3( 0 , 0.330031 , 21.562 ), 
  D3DXVECTOR3( 0 , 0.330031 , -32.292 ), 
  D3DXVECTOR3( 0 , 0 , -40 ), 
  D3DXVECTOR3( 0 , 0.990099 , -47.708 ), 
  D3DXVECTOR3( 0 , 0.330031 , -40.747 ), 
  D3DXVECTOR3( 0 , 0 , -50 ), 
  D3DXVECTOR3( 0 , 0.330031 , -59.253 ), 
  D3DXVECTOR3( 0 , -20 , -60 ), 
  D3DXVECTOR3( 0 , -20 , -50 ), 
  D3DXVECTOR3( 0 , -20 , -40 ), 
  D3DXVECTOR3( 0 , -20 , -10 ), 
  D3DXVECTOR3( 0 , -20 , 0 ), 
  D3DXVECTOR3( 0 , -20 , 10 ), 
  D3DXVECTOR3( 0 , -20 , 30 ), 
  D3DXVECTOR3( 0 , -20 , 40 )
 };


D3DXVECTOR3 Spline_SliceBorder [22] = {
  D3DXVECTOR3( 0 , -20 , 40 ), 
  D3DXVECTOR3( 0 , -10 , 60 ), 
  D3DXVECTOR3( 0 , 19.571 , 46.6893 ), 
  D3DXVECTOR3( 0 , 20 , 40 ), 
  D3DXVECTOR3( 0 , 19.769 , 33.6503 ), 
  D3DXVECTOR3( 0 , 0.330031 , 38.0985 ), 
  D3DXVECTOR3( 0 , 0 , 30 ), 
  D3DXVECTOR3( 0 , 0.330031 , 21.562 ), 
  D3DXVECTOR3( 0 , 0.330031 , -32.292 ), 
  D3DXVECTOR3( 0 , 0 , -40 ), 
  D3DXVECTOR3( 0 , 0.990099 , -47.708 ), 
  D3DXVECTOR3( 0 , 19.1089 , -42.4448 ), 
  D3DXVECTOR3( 0 , 20 , -50 ), 
  D3DXVECTOR3( 0 , 20.8911 , -57.2156 ), 
  D3DXVECTOR3( 0 , -20 , -60 ), 
  D3DXVECTOR3( 0 , -20 , -50 ), 
  D3DXVECTOR3( 0 , -20 , -40 ), 
  D3DXVECTOR3( 0 , -20 , -10 ), 
  D3DXVECTOR3( 0 , -20 , 0 ), 
  D3DXVECTOR3( 0 , -20 , 10 ), 
  D3DXVECTOR3( 0 , -20 , 30 ), 
  D3DXVECTOR3( 0 , -20 , 40 )
 };