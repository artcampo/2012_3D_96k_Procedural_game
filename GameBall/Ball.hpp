#ifndef _GameBP_Ball_HPP_
#define _GameBP_Ball_HPP_

#include "dx_misc.hpp"
#include <vector>

#include	  "physics/Physics.hpp" 

/*
#include "Geometry\Spline\SplineBezierTMapped.hpp"
#include "RenderModels\DxMeshModel.hpp"
#include "Physics/MeshCol.hpp"
#include "Noise/RandomNumberGenerator.hpp"

#include "Geometry/Loft.hpp"
#include "Geometry/DirGraph/DirGraph.hpp"
#include "Geometry/DirGraph/Pattern.hpp"
#include "Geometry/DirGraph/SubRule.hpp"
#include "Geometry/Spline/SplineIntegral.hpp"

#include "GameBall/Track.hpp"
#include "Misc/StlHelpers.hpp"
#include "Misc/Log.hpp"
*/

#include    "physics/RigidBody.hpp"

class Ball {
public:  
  Ball();
  ~Ball();

public:
  RigidBody*      mBallBody;

  TimeForce*      mBallTForceForw;
  TimeForce*      mBallTForceBack;
  TimeForce*      mBallTForceLeft;
  TimeForce*      mBallTForceRight;

  TimeTorque*     mBallTTorqueUp;
  TimeTorque*     mBallTTorqueDown;
  TimeTorque*     mBallTTorqueLeft;
  TimeTorque*     mBallTTorqueRight;  

private:

public:

};


#endif  //_GameBP_Ball_HPP_
