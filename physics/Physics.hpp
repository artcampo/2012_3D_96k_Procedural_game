#ifndef PHYSICS_HPP
#define PHYSICS_HPP



#include    "dx_misc.hpp"
#include    "Camera/Camera.hpp"
#include    "Global.hpp"
#include	  "Physics/RigidBody.hpp"
#include	  "Physics/RigidBodyExt.hpp"
#include	  "Physics/RigidBodyOde.hpp"
#include	  "Physics/StaticRigidBody.hpp"
#include	  "Physics/MeshCol.hpp"
#include    "ObjEditor/Mesh/Mesh.hpp"
#include    "Geometry/2dDataStructures/KddTree2d.hpp"
#include    "Geometry/DxMathHelpers.hpp"

//  profile
#include "Misc/Timing_windows_x64.hpp"

struct PairBodies{
  int               a;  
  bool              bIsDynamic;
  int               b;
  StaticRigidBody*  bStatic;
};

class Precision{
public:
  float   mEpsilonBarycentric;
  int     mIterationsReset;
};

struct Contact{
  D3DXVECTOR3 pointWS;      //  Point in first object
  D3DXVECTOR3 normalWS;     //  Going outwards from second object
  DxMath::Triangle    offender;     //  Triangle that has caused collision/contact
  float DEBUG_d;
  int   DEBUG_aId;
  int   DEBUG_bId;
  float DEBUG_dot;
};



class Physics {
 
public:	
	Physics();
	~Physics();

  //  init functions
  RigidBody* createRigidBody (  const D3DXVECTOR3& aPositionWS,
                                const float aMass,
                                const float aBoundingRadius,
                                const float aDragForce,
                                const float aBounceCoefficient);
  void  createStaticRigidBody ( Mesh* aMesh );
  void  createStaticRigidBody ( MeshCol* aMesh );

  void  initStaticStructures();
  void  setParameters( float aGravityAcc );

  //  simulation functions
  std::vector<bool>  advanceSimulation ( float aDeltaT );
  


  //  TODO: put an iterator here
  
  int         getNumBodies() const;
  RigidBody*  getIthBody ( int i );

  //  Rewritten
private:
  int         numDynBodies() const;
  int         numStaBodies() const;

  //  Constants of simulation
private:
  float   mGravityAcc;  /*Gravity*/
  float   mEpsilon;   /*!< precision of interpenetration*/
  

  float   mInterpenetrationThreshold; /*!< Threshold of distance*/
  float   mMinimumTimeOfCollision;    /*!< Fine-grain threshold for collisions,
                                           also minimum step of update*/
  float   mMinimumTimeOfCollisionStep;  /*!< */
  float   mMinimumForceIncrement;  /*!< */
  float   mContactDotEpsilon;
  float   mEpsilonBarycentric;
  std::vector<Precision> mPrecisionPerBody;

  float   mBroadPhaseDistanceEpsilon;

  //  Simulation data
private:
  float       mTime;        /*!< Current time */  
  std::vector<RigidBody*>       mDynBodies;     /*!< Vector of Dynamic bodies*/  
  std::vector<RigidBodyExt*>    mDynBodiesExt;  /*!< Vector of Dynamic bodies*/  
  std::vector<StaticRigidBody*> mStaBodies;     /*!< Vector of static bodies: will be sorted by distance to the only moving object*/  
  
  std::vector<D3DXVECTOR3>      mDynBodiesLastPos;
  D3DXVECTOR3   mLastPos;

  Kdd2d::KddTree2d<StaticRigidBody>* mStaBodiesKdd;

  //  Per-step temporal information
private:
  std::vector<PairBodies>       mDynContactList; /*!< List of possible colisions*/
  std::vector<PairBodies>       mStaContactList; /*!< List of possible colisions*/


private:
  void    updateContactList       ();
  void    checkContacts( std::vector<RigidBody*>& aBodies );
  bool    findContacts            ( std::vector<RigidBody*>& aBodies,
                                    PairBodies& aCollidingObjects,
                                    Contact& aContact,
                                    int& aNumberOfIntepenetrations,
                                    bool& aAllCollisionsAreFromSameObject);
  float   findFirstTimeOfContact  ( float aDeltaT, 
                                    std::vector<RigidBody*>& aBodies, 
                                    PairBodies& aCollidingObjects, 
                                    Contact& aContact,
                                    bool& aTimePointFound,
                                    int& aNumIterations );
  
  void    tryToAdvanceSimulation  ( float aDeltaT, std::vector<RigidBody*>& aBodies );
  void    applySimulationStep     ( std::vector<RigidBody*>& aBodies );

  //  Against dynamic bodies
  bool    narrowCollision           ( RigidBody* aBodyA, RigidBody* aBodyB, const float aEpsilon, Contact& aContact );
  Contact computeContact            ( RigidBody* aBodyA, RigidBody* aBodyB );
  int     interpenetrationOfContact ( RigidBody* aBodyA, RigidBody* aBodyB, const Contact& aContact );
  void    resolveCollision          ( RigidBody* aBodyA, RigidBody* aBodyB, const Contact& aContact );

  //  Against static bodies
  bool    narrowCollision           ( RigidBody* aBodyA, 
                                      StaticRigidBody* aBodyB, 
                                      const float aEpsilon, 
                                      Contact& aContact, const bool aExtendedDebug,
                                      const float aEpsilonBarycentric );  

  bool    narrowCollision           ( RigidBody* aBodyA, 
                                      StaticRigidBody* aBodyB, 
                                      const D3DXVECTOR3& aRayB,
                                      const D3DXVECTOR3& aRayD,
                                      const float aEpsilon, 
                                      Contact& aContact, const bool aExtendedDebug,
                                      const float aEpsilonBarycentric ); 
                                
  int     interpenetrationOfContact ( RigidBody* aBodyA, 
                                      StaticRigidBody* aBodyB, 
                                      Contact& aContact,
                                      float& aVrel );
  void    resolveCollision          ( RigidBody* aBodyA, StaticRigidBody* aBodyB, const Contact& aContact );

  void storeRestingContact ( RigidBody* aBody, const Contact& aContact );

  //  helpers
private:
  void limitVelocities( std::vector<RigidBody*>& aBodies );
  D3DXVECTOR3 pointVelocity( RigidBody* aBody, D3DXVECTOR3 aPoint );

  //  DEBUG
public:  
  
  int   mDebugFrame;
  void printPos( int i);
  void perFrameCountersReset();
  void perFrameCountersPrint();

  int   mFcNumIterations;
  int   mFcNumObjects;

private:
  enum  eContactResult{
    eContactMovingAway,
    eContactRestingContact,
    eContactInterpenetration
  };


};


#endif