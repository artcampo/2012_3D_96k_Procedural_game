#include "physics/Physics.hpp"
#define _DEBUG_SHOW_ONLY_DOT_INFO

Physics::Physics()
{
  mTime                       = 0.0f;
  mEpsilon                    = 0.1f;  
  mInterpenetrationThreshold  = 0.1;
  mEpsilonBarycentric         = 0.1f;
  mContactDotEpsilon          = 0.1f;

  mMinimumTimeOfCollision     = 0.000001f;
  mMinimumTimeOfCollisionStep = 0.0001f;
  mMinimumForceIncrement      = 0.000001f;

  mBroadPhaseDistanceEpsilon = 3.6;  
  mDebugFrame = 0;
}

void Physics::setParameters( float aGravityAcc )
{
  mGravityAcc         = aGravityAcc;
  
}

Physics::~Physics()
{
  //TODO: smart or release:
  //std::vector<StaticRigidBody*> mStaBodies

}



//===========================================================================
/*!
    creates a new static rigid body
       
*/
//===========================================================================
void Physics::createStaticRigidBody ( Mesh* aMesh )
{
  //  Allocate
  int index = numStaBodies();
  mStaBodies.push_back ( new StaticRigidBody( aMesh ) ); 
  mStaBodies[index]->mDebugId = index;
}

void Physics::createStaticRigidBody ( MeshCol* aMesh )
{
  //  Allocate
  int index = numStaBodies();
  mStaBodies.push_back ( new StaticRigidBody( aMesh ) );
  mStaBodies[index]->mDebugId = index;
}





//===========================================================================
/*!
    creates a new rigid body
    \return     pointer to new rigid body
    
*/
//===========================================================================
RigidBody* Physics::createRigidBody ( const D3DXVECTOR3& aPositionWS, 
                                      const float aMass,
                                      const float aBoundingRadius,
                                      const float aDragForce,
                                      const float aBounceCoefficient )
{
  //  Allocate
  int index = numDynBodies();

  mDynBodiesLastPos.resize( index + 1 );  
  mDynBodiesLastPos[index]  = aPositionWS;

  mDynBodiesExt.push_back( new RigidBodyExt( aMass, aBoundingRadius, aDragForce, aBounceCoefficient ) );
  mDynBodies.push_back( new RigidBody( aPositionWS, index, mDynBodiesExt[index] ) );  

  mPrecisionPerBody.resize( index + 1 );  
  mPrecisionPerBody[index].mEpsilonBarycentric = mEpsilonBarycentric;
  mPrecisionPerBody[index].mIterationsReset    = 0;

  return mDynBodies[index];
}

//===========================================================================
/*!
    Given a time-point of the simulation where two objects collide,
    new impulses are applied to take them away
    \param  aObjects  Pair of colliding objects
*/
//===========================================================================
void Physics::resolveCollision( RigidBody* aBodyA, RigidBody* aBodyB, const Contact& aContact )
{
  aBodyA->computeTemporalValues();
  aBodyB->computeTemporalValues();

  D3DXVECTOR3 paDot = pointVelocity( aBodyA, aContact.pointWS );
  D3DXVECTOR3 pbDot = pointVelocity( aBodyB, aContact.pointWS );
  D3DXVECTOR3 n = aContact.normalWS;
  D3DXVECTOR3 ra = aContact.pointWS - aBodyA->xWS;
  D3DXVECTOR3 rb = aContact.pointWS - aBodyB->xWS;

  D3DXVECTOR3 pabDot = paDot - pbDot;
  double vRel = D3DXVec3Dot( &n, &pabDot );
  float bounceCoefficient = ( aBodyA->mBodyExt->mBounceCoefficient + aBodyB->mBodyExt->mBounceCoefficient ) * 0.5f ;
  double numerator = (1.0f + bounceCoefficient) * ( -1.0f * vRel);

  double term1 = 1.0f / ( aBodyA->mBodyExt->m );
  double term2 = 1.0f / ( aBodyB->mBodyExt->m );

  D3DXVECTOR3 out;
  D3DXVec3Cross( &out, &ra, &n );
  D3DXVec3TransformCoord( &out, &out, &aBodyA->iInvWS );
  D3DXVec3Cross( &out, &out, &ra );
  double term3 = D3DXVec3Dot( &n, &out );

  D3DXVec3Cross( &out, &rb, &n );
  D3DXVec3TransformCoord( &out, &out, &aBodyB->iInvWS );
  D3DXVec3Cross( &out, &out, &rb );
  double term4 = D3DXVec3Dot( &n, &out );
 
  double j = numerator /( term1 + term2 + term3 + term4 );
  D3DXVECTOR3 force = float(j)*n;

  aBodyA->pWS += force;
  aBodyB->pWS -= force;

  D3DXVec3Cross ( &out, &ra, &force);
  aBodyA->lWS += out;
  D3DXVec3Cross ( &out, &rb, &force);
  aBodyB->lWS -= out;

  aBodyA->vWS = aBodyA->pWS / aBodyA->mBodyExt->m;
  aBodyB->vWS = aBodyB->pWS / aBodyB->mBodyExt->m;

  D3DXVec3TransformCoord( &out, &aBodyA->lWS, &aBodyA->iInvWS );
  aBodyA->wWS = out;
  
  D3DXVec3TransformCoord( &out, &aBodyB->lWS, &aBodyB->iInvWS );
  aBodyB->wWS = out;

}

void Physics::resolveCollision( RigidBody* aBodyA, StaticRigidBody* aBodyB, const Contact& aContact )
{
  
  aBodyA->computeTemporalValues();  

  D3DXVECTOR3 paDot = pointVelocity( aBodyA, aContact.pointWS );
  D3DXVECTOR3 pbDot = D3DXVECTOR3( 0, 0, 0 );
  D3DXVECTOR3 n = aContact.normalWS;
  D3DXVECTOR3 ra = aContact.pointWS - aBodyA->xWS;
  D3DXVECTOR3 rb = aContact.pointWS;

  D3DXVECTOR3 pabDot = paDot - pbDot;
  double vRel = D3DXVec3Dot( &n, &pabDot );
  double numerator = (1.0f + aBodyA->mBodyExt->mBounceCoefficient) * ( -1.0f * vRel);

  double term1 = 1.0f / ( aBodyA->mBodyExt->m );
  double term2 = 0;

  D3DXVECTOR3 out;
  D3DXVec3Cross( &out, &ra, &n );
  D3DXVec3TransformCoord( &out, &out, &aBodyA->iInvWS );
  D3DXVec3Cross( &out, &out, &ra );
  double term3 = D3DXVec3Dot( &n, &out );


  double term4 = 0;
 
  double j = numerator /( term1 + term2 + term3 + term4 );
  D3DXVECTOR3 force = float(j)*n;

  aBodyA->pWS += force;
  D3DXVec3Cross ( &out, &ra, &force);
  aBodyA->lWS += out;
  aBodyA->vWS = aBodyA->pWS / aBodyA->mBodyExt->m; 
  D3DXVec3TransformCoord( &out, &aBodyA->lWS, &aBodyA->iInvWS );
  aBodyA->wWS = out;
  
}

//===========================================================================
/*!
    Given a time-point of the simulation where two objects collide,
    new impulses are applied to take them away
    \param    aBodyA  Body
    \param    aBodyB  Body
    \returns  point of contact between them
*/
//===========================================================================
Contact Physics::computeContact( RigidBody* aBodyA, RigidBody* aBodyB )
{
  Contact contact;

  aBodyA->computeTemporalValues();
  aBodyB->computeTemporalValues();

  D3DXVECTOR3 normal = aBodyA->xWS - aBodyB->xWS;
  D3DXVec3Normalize( &normal, &normal );

  D3DXVECTOR3 pointOfContact = 
    aBodyA->xWS + ( (-aBodyA->mBodyExt->mBoundingSphereRadius)*normal );

  contact.pointWS   = pointOfContact;
  contact.normalWS  = normal;

  return contact;
}


int Physics::interpenetrationOfContact( RigidBody* aBodyA, RigidBody* aBodyB, const Contact& aContact )
{
  D3DXVECTOR3 paDot = pointVelocity( aBodyA, aContact.pointWS );
  D3DXVECTOR3 pbDot = pointVelocity( aBodyB, aContact.pointWS );
  double vRel = D3DXVec3Dot( &(aContact.normalWS), &(paDot - pbDot) );

  if (vRel > mInterpenetrationThreshold )
    return eContactResult::eContactMovingAway;
  if (vRel > (-mInterpenetrationThreshold) )
    return eContactResult::eContactRestingContact;
  else
    return eContactResult::eContactInterpenetration;
}

int Physics::interpenetrationOfContact( RigidBody* aBodyA, 
                                       StaticRigidBody* aBodyB, 
                                       Contact& aContact,
                                       float& aVrel)
{  
  aBodyA->computeTemporalValues();
  D3DXVECTOR3 paDot = pointVelocity( aBodyA, aContact.pointWS );
  DxMath::clean( paDot, 0.00001f );
  D3DXVECTOR3 pbDot = D3DXVECTOR3( 0, 0, 0 );
  double vRel = D3DXVec3Dot( &(aContact.normalWS), &(paDot - pbDot) );

  aVrel = vRel;
  aContact.DEBUG_dot = vRel;

  int ret;
  if (vRel > mInterpenetrationThreshold )
    ret =  eContactResult::eContactMovingAway;
  else
    if (vRel > (-mInterpenetrationThreshold) )
      ret =  eContactResult::eContactRestingContact;
    else
      ret =  eContactResult::eContactInterpenetration;

  return ret;
}

//===========================================================================
/*!
    Computes the velocity of a given point in a body
    \param    aBody   body to consider
    \param    aPoint  point to consider
    \returns  point velocity
    
*/
//===========================================================================
D3DXVECTOR3 Physics::pointVelocity( RigidBody* aBody, D3DXVECTOR3 aPoint )
{
  aBody->computeTemporalValues();
  D3DXVECTOR3 velocity;
  D3DXVec3Cross( &velocity, &aBody->wWS, &(aPoint - aBody->xWS) );
  velocity = velocity + aBody->vWS;
  return velocity;
}

//===========================================================================
/*!

*/
//===========================================================================
int  Physics::numDynBodies() const
{
  return mDynBodies.size();
}

//===========================================================================
/*!

*/
//===========================================================================
int  Physics::numStaBodies() const
{
  return mStaBodies.size();
}

int Physics::getNumBodies() const
{
  return numDynBodies();
}

//===========================================================================
/*!
    Returns pointer to ith Body
    \returns    pointer to body
    
*/
//===========================================================================
RigidBody*  Physics::getIthBody ( int i )
{
  return mDynBodies[i];
}

void Physics::printPos( int i )
{

  mDynBodies[i]->computeTemporalValues();

#ifndef _DEBUG_SHOW_ONLY_DOT_INFO    
  *Global::mFileDebugBody[i]  
    <<std::endl
    << "Obj "<< i <<std::endl;

  *Global::mFileDebugBody[i]  
    << " X: " << mDynBodies[i]->xWS.x << ", " << mDynBodies[i]->xWS.y << ", " << mDynBodies[i]->xWS.z << "" 
    << " V: " << mDynBodies[i]->vWS.x << ", " << mDynBodies[i]->vWS.y << ", " << mDynBodies[i]->vWS.z << ""     
    << " [" << mDebugFrame << "]"
    << std::endl;
#endif
    
}

void Physics::initStaticStructures()
{
  mStaBodiesKdd = new Kdd2d::KddTree2d<StaticRigidBody>( mStaBodies );
  updateContactList();

  mDynBodiesLastPos.resize( numDynBodies() );
  for ( int i = 0; i < numDynBodies(); ++i )  
    mDynBodiesLastPos[i] = mDynBodies[i]->xWS + D3DXVECTOR3(0, 0.5, 0 );
  checkContacts( mDynBodies );
}

void Physics::limitVelocities( std::vector<RigidBody*>& aBodies )
{
  for ( int i = 0; i < aBodies.size(); ++i )  
    aBodies[i]->limitVelocity();      
}

void Physics::storeRestingContact ( RigidBody* aBody, const Contact& aContact )
{
  aBody->mIsInRestingContact = true;
  int indexContact = aBody->mContactNormal.size();
  D3DXVECTOR3 normal = aContact.normalWS;
  DxMath::clean( normal );
  aBody->mContactNormal.push_back( normal );
  /*
  if ( aContact.DEBUG_d < 0.0f )
  {
    D3DXVECTOR3 inc = normal * -( aContact.DEBUG_d*0.2f  );
    aBody->xWS +=  inc;
  }
  */
  /*
  int id = aBody->mId;
  *Global::mFileDebugBody[id]  
  << " X afterwards: " << aBody->xWS.x << ", " << aBody->xWS.y << ", " << aBody->xWS.z << "" << std::endl;
     */   
}



bool Physics::narrowCollision( RigidBody* aBodyA, 
                              StaticRigidBody* aBodyB, 
                              const float aEpsilon, 
                              Contact& aContact, const bool aExtendedDebug,
                              const float aEpsilonBarycentric )
{

  D3DXVECTOR3 rayD = aBodyA->xWS - mDynBodiesLastPos[aBodyA->mId];


  float d;
  bool interpenetration = aBodyB->rayIntesection( mDynBodies[ aBodyA->mId ]->xWS,
                                                  rayD,
                                                  aEpsilon,
                                                  mDynBodies[ aBodyA->mId ]->mBodyExt->mBoundingSphereRadius,
                                                  aContact.pointWS,
                                                  aContact.normalWS,
                                                  aContact.offender,
                                                  d,
                                                  aExtendedDebug,
                                                  aEpsilonBarycentric);
  aContact.DEBUG_d = d;
  aContact.DEBUG_aId = (int)aBodyA->mId;
  aContact.DEBUG_bId = (int)aBodyB->mDebugId;  

  //  Normal as returned by the function is of the triangle,
  //  and we want the opposite one

  
  D3DXVec3Normalize( &aContact.normalWS, &aContact.normalWS );  
  // TODO: why reverse???
  DxMath::reverse( aContact.normalWS ) ;    
  DxMath::clean( aContact.normalWS );

  return interpenetration;
}

//===========================================================================
/*!
    Advances the simulation by a time step
    \param    aDeltaT   time step
    \return   vector of bools, for each dyn object if it had a contact
    
*/
//===========================================================================
std::vector<bool>  Physics::advanceSimulation ( float aDeltaT )
{

  float targetDeltaT = aDeltaT;
  float appliedDeltaT = 0.0f;

  std::vector<bool> dynBodyHadContact ( numDynBodies() );

  for ( int i = 0; i < numDynBodies() ; ++i )
    dynBodyHadContact[i] = false;

  for ( int i = 0; i < numDynBodies() ; ++i )
  {
    if ( mPrecisionPerBody[i].mIterationsReset != 0 )
    {
      --mPrecisionPerBody[i].mIterationsReset;
      if ( mPrecisionPerBody[i].mIterationsReset == 0 )
        mPrecisionPerBody[i].mEpsilonBarycentric = mEpsilonBarycentric;
    }
    printPos(i);  
  }

  updateContactList();  



  D3DXVECTOR3 pos = mDynBodies[0]->xWS;
  int numIterationsTotal = 1;
  //while ( appliedDeltaT < aDeltaT )
  while ( !DxMath::equality(  aDeltaT - appliedDeltaT, 0.0f, mMinimumTimeOfCollisionStep ) )
  {       
    ++mFcNumIterations;
    std::vector<RigidBody*> bodies( numDynBodies() );
    for ( int i = 0; i < numDynBodies(); ++i )
      bodies[i] = new RigidBody( *mDynBodies[i] );
        
    tryToAdvanceSimulation( targetDeltaT, bodies );
    

    PairBodies collidingObjects;
    Contact dummyContact;
    int dummyNum; bool dummyBool;

    *Global::mFileDebugCol << "Collision finding"<< " ["<< mDebugFrame<< "] targetDeltaT: "<< targetDeltaT<<std::endl;
    
    if ( findContacts( bodies, collidingObjects, dummyContact, dummyNum, dummyBool ) )
    {
      Contact contact;

      *Global::mFileDebugCol << "  Search first collision, targetDeltaT: "<< targetDeltaT<<std::endl;
      //  Advance until first collision
      bool collisionFound;
      int numIterations;
      for ( int i = 0; i < numDynBodies(); ++i )
        *bodies[i] = *mDynBodies[i];
      targetDeltaT = findFirstTimeOfContact(  targetDeltaT, 
                                              bodies, 
                                              collidingObjects, 
                                              contact,
                                              collisionFound,
                                              numIterations);
      //tryToAdvanceSimulation( targetDeltaT, bodies );
      applySimulationStep( bodies );
      numIterationsTotal += numIterations;

      *Global::mFileDebugPhysGen << "  Collision found, iterations:"<< numIterations <<" targetDeltaT: "<< targetDeltaT<<std::endl;
      *Global::mFileDebugCol << "  Collision found, iterations:"<< numIterations <<" targetDeltaT: "<< targetDeltaT<<std::endl;
      
      

      appliedDeltaT += targetDeltaT;
      mTime += targetDeltaT;
      targetDeltaT = aDeltaT - appliedDeltaT;      

      *Global::mFileDebugTime << " -tC: "<< appliedDeltaT<<std::endl;



      if ( collisionFound )
      {
        //  Resolve collision
        //resolveCollision( bodies + collidingObjects.a, bodies + collidingObjects.b );

        

        if ( collidingObjects.bIsDynamic )
        {
          *Global::mFileDebugColOffenders << "Solving dynamic col." << std::endl;
          dynBodyHadContact[collidingObjects.a] = true;
          dynBodyHadContact[collidingObjects.b] = true;
          resolveCollision( mDynBodies[collidingObjects.a], 
                            mDynBodies[collidingObjects.b], 
                            contact );
        }
        else
        {
          *Global::mFileDebugColOffenders << "Solving for  obj-"<< contact.DEBUG_aId<< " with distance: " << contact.DEBUG_d << " against: [" << contact.DEBUG_bId << "]"<< " [" <<mDebugFrame <<"]" << std::endl;

          int id = contact.DEBUG_aId;

        bodies[id]->computeTemporalValues();
        Global::mFileDebugColOffenders->precision( 4 );
        *Global::mFileDebugColOffenders  << " bef ";

        *Global::mFileDebugColOffenders  
            << " X: " << mDynBodies[id]->xWS.x << ", " << mDynBodies[id]->xWS.y << ", " << mDynBodies[id]->xWS.z << "" 
            << " Dist: " << contact.DEBUG_d
            << " Dot: " << contact.DEBUG_dot
            //<< " Pc: " << aContact.pointWS.x << ", " << aContact.pointWS.y << ", " << aContact.pointWS.z << "" 
            << " V: " << mDynBodies[id]->vWS.x << ", " << mDynBodies[id]->vWS.y << ", " << mDynBodies[id]->vWS.z << "" 
            << " [" << mDebugFrame << "]"
            << std::endl;

        dynBodyHadContact[collidingObjects.a] = true;
          resolveCollision( mDynBodies[collidingObjects.a],
                            collidingObjects.bStatic, 
                            contact );

mDynBodies[id]->computeTemporalValues();
        Global::mFileDebugColOffenders->precision( 4 );
      *Global::mFileDebugColOffenders  << " aft ";
        *Global::mFileDebugColOffenders  
            << " X: " << mDynBodies[id]->xWS.x << ", " << mDynBodies[id]->xWS.y << ", " << mDynBodies[id]->xWS.z << "" 
            << " Dist: " << contact.DEBUG_d
            << " Dot: " << contact.DEBUG_dot
            //<< " Pc: " << aContact.pointWS.x << ", " << aContact.pointWS.y << ", " << aContact.pointWS.z << "" 
            << " V: " << mDynBodies[id]->vWS.x << ", " << mDynBodies[id]->vWS.y << ", " << mDynBodies[id]->vWS.z << "" 
            << " [" << mDebugFrame << "]"
            << std::endl;

        }
      }
      else
      {
        *Global::mFileDebugColIt << "Collision missed. " <<numIterations<< " [" << mDebugFrame << "]"<<std::endl;
      }

      
    }
    else
    {
      *Global::mFileDebugCol << "  No collision found."<<std::endl;
      //  No collision      
      applySimulationStep( bodies );
      appliedDeltaT += targetDeltaT;
      mTime += targetDeltaT;
      targetDeltaT = aDeltaT - appliedDeltaT;
    }

    // release memory
    for ( int i = 0; i < numDynBodies(); ++i )
      delete bodies[i];
  }
  D3DXVECTOR3 d = mDynBodies[0]->xWS - pos;
  *Global::mFileDebugColIt  << "Num Iterations: " <<numIterationsTotal
                            << " with time:" << targetDeltaT 
                            << " and inc: " << D3DXVec3Length( &d )
                            <<" [" << mDebugFrame << "]"<<std::endl;
  pos = mDynBodies[0]->xWS;

  *Global::mFileDebugNumStatic << "num static objects: " << mStaContactList.size()<< " [" << mDebugFrame << "]" << std::endl;  
  
  ++mDebugFrame; 
  D3DXVECTOR3 dif = mDynBodies[0]->xWS - mLastPos;
  *Global::mFileDebugPhysStep << "Step: " << D3DXVec3Length( &dif ) << " ["<< mDebugFrame <<"] "<< std::endl;
  mLastPos = mDynBodies[0]->xWS;

  return dynBodyHadContact;
}

//===========================================================================
/*!
    Uses mDynBodies to simulate an step and leave the result in the array,
    starting at time position mTime
    \param    aDeltaT   time step
    \param    aBodies   where to store result from ODE
*/
//===========================================================================
void Physics::tryToAdvanceSimulation( float aDeltaT, std::vector<RigidBody*>& aBodies )
{
  limitVelocities( aBodies );  

  RigidBodyOde ode( aBodies, numDynBodies(), mTime, mGravityAcc, mContactDotEpsilon, mMinimumForceIncrement );

  ode.Step( aDeltaT );  

  limitVelocities( aBodies );
}

//===========================================================================
/*!
    Updates resting contacts lists for each body. To be called prior
    to ODE.
    \param    aDeltaT   time step
    \param    aBodies   bodies
*/
//===========================================================================
void Physics::checkContacts( std::vector<RigidBody*>& aBodies )
{

  //  clean resting contacts
  for ( int i = 0; i < numDynBodies() ; ++i )
  {
    mDynBodies[i]->mIsInRestingContact = false;
    mDynBodies[i]->mContactNormal.clear();
  }  

  //  check each nearby object
  PairBodies collidingObjects;  
  Contact dummyContact;
  int dummyNum;
  bool dummyBool;
  findContacts( aBodies, collidingObjects, dummyContact, dummyNum, dummyBool );

}

//===========================================================================
/*!
    Copies the result of ODE back to mDynBodies
    \param    aBodies   result from ODE
*/
//===========================================================================
void Physics::applySimulationStep ( std::vector<RigidBody*>& aBodies )
{
  for ( int i = 0; i < numDynBodies() ; ++i )
  {
    *mDynBodies[i] = *aBodies[i];
    if ( !DxMath::equality( mDynBodiesLastPos[i], mDynBodies[i]->xWS, 0.001f ) ) 
      mDynBodiesLastPos[i] = mDynBodies[i]->xWS;
    mDynBodies[i]->clearRotation();
  }
}


//===========================================================================
/*!
    Updates the contact list - broad phase - marking pairs
    of objects that are near enough to collide.
    They are kept into "mDynContactList".
    "mHasCollided" is cleared before use.
*/
//===========================================================================
void Physics::updateContactList()
{
  mDynContactList.resize( 0 );
  
  for ( int i = 0; i < numDynBodies() ; ++i )
    for ( int j = i + 1; j < numDynBodies() ; ++j )
    {
      PairBodies pair;
      pair.a          = i;
      pair.b          = j;
      pair.bIsDynamic = true;
      mDynContactList.push_back ( pair );
    }
  

  mStaContactList.resize( 0 );
  for ( int i = 0; i < numDynBodies() ; ++i )
  {
    std::vector<StaticRigidBody*> staticBodies;
    staticBodies = mStaBodiesKdd->neighbours( mDynBodies[i]->xWS.x, 
                                              mDynBodies[i]->xWS.z, 
                                              mBroadPhaseDistanceEpsilon );

    for ( int j = 0; j < staticBodies.size() ; ++j )
    {
      PairBodies pair;
      pair.a          = i;   
      pair.bStatic    = staticBodies[j];
      pair.bIsDynamic = false;
      mStaContactList.push_back ( pair );
    }
    mFcNumObjects += staticBodies.size();
  }   

}

//===========================================================================
/*!
    Finds all the pairs of objects that collide.
    Store result in "mHasCollided"
    \param  aObjects  Pair of colliding objects
    \return   true<=> one or more objects are interpetrating
*/
//===========================================================================
bool Physics::findContacts( std::vector<RigidBody*>& aBodies,
                            PairBodies& aCollidingObjects,
                            Contact& aContact,
                            int& aNumberOfIntepenetrations,
                            bool& aAllCollisionsAreFromSameObject)
{
  
  for ( int i = 0; i < numDynBodies() ; ++i )
  {
    aBodies[i]->mIsInRestingContact = false;
    aBodies[i]->mContactNormal.clear();
  }  
  
  
  bool collisionFound = false;
  aNumberOfIntepenetrations = 0;

  Contact currentContact;

  for ( unsigned int i = 0; i < mDynContactList.size() ; ++i )
  {
    
    bool pairCollides = narrowCollision(  aBodies[ mDynContactList[i].a ], 
                                          aBodies[ mDynContactList[i].b ], 
                                          mEpsilon,
                                          currentContact );

    if ( pairCollides )   
    {
      int resultContact = interpenetrationOfContact(  aBodies[ mDynContactList[i].a ], 
                                                      aBodies[ mDynContactList[i].b ], 
                                                      currentContact );
      pairCollides &= ( resultContact == eContactResult::eContactInterpenetration);

      if ( resultContact == eContactResult::eContactInterpenetration)
      {
        ++aNumberOfIntepenetrations;        
        aCollidingObjects = mDynContactList[i];                          
        aContact = currentContact;
      }
    }

    collisionFound      |= pairCollides;    
  }

  Contact prevContact;
  int     prevContactRigidBodyId;

  for ( unsigned int i = 0; i < mStaContactList.size() ; ++i )
  {
    int id = mStaContactList[i].a;
    bool pairCollides = narrowCollision(  aBodies[ id ], 
                                          mStaContactList[i].bStatic, 
                                          mEpsilon,
                                          currentContact,
                                          (aBodies[id]->mBodyExt->mPreviousContactSurface == mStaContactList[i].bStatic),
                                          mPrecisionPerBody[id].mEpsilonBarycentric);

    if( aBodies[id]->mBodyExt->mPreviousContactSurface == mStaContactList[i].bStatic )
    {
      *Global::mFileDebugColOffenders
        << " [" << mDebugFrame << "]"
        << std::endl;
    }

    bool currentCollision = false;
    if ( pairCollides )
    {
      float vRel;
      int resultContact = interpenetrationOfContact(  aBodies[ id ], 
                                                      mStaContactList[i].bStatic, 
                                                      currentContact,
                                                      vRel );

      currentCollision = ( resultContact == eContactResult::eContactInterpenetration );

#ifdef _DEBUG_SHOW_ONLY_DOT_INFO
      if (  resultContact == eContactResult::eContactInterpenetration  ||
            resultContact == eContactResult::eContactRestingContact)
      {
        aBodies[id]->mBodyExt->mPreviousContactSurface = mStaContactList[i].bStatic;
        aBodies[id]->computeTemporalValues();
        Global::mFileDebugBody[id]->precision( 4 );
        if ( resultContact == eContactResult::eContactRestingContact )
          *Global::mFileDebugBody[id]  << " RSTC ";
        if ( resultContact == eContactResult::eContactInterpenetration )
          *Global::mFileDebugBody[id]  << " NONE ";
        *Global::mFileDebugBody[id]  
            //<< " X: " << aBodies[id]->xWS.x << ", " << aBodies[id]->xWS.y << ", " << aBodies[id]->xWS.z << "" 
            //<< " Dist: " << currentContact.DEBUG_d
            //<< " Dot: " << currentContact.DEBUG_dot
            //<< " Pc: " << currentContact.pointWS.x << ", " << currentContact.pointWS.y << ", " << currentContact.pointWS.z << "" 
            //<< " V: " << aBodies[id]->vWS.x << ", " << aBodies[id]->vWS.y << ", " << aBodies[id]->vWS.z << "" 
            << " W: " << aBodies[id]->qWS.x << ", " 
                      << aBodies[id]->qWS.y << ", " 
                      << aBodies[id]->qWS.z << ", " 
                      << aBodies[id]->qWS.w 
            << " [" << mDebugFrame << "]"
            << std::endl;
      }
#endif



      //  discard current collision if its plane is coplanar to a previous collision
      //  of the same object
      if (  currentCollision && 
            aNumberOfIntepenetrations == 1 && 
            id == prevContactRigidBodyId )
      {            
        if ( DxMath::areTrianglesCoplanar( prevContact.offender, currentContact.offender ) )
        {
          currentCollision = false;
          *Global::mFileDebugColIt << "Coplanarity found" << std::endl;
        }
        else
          *Global::mFileDebugColIt << "Coplanarity not found" << std::endl;

      }

      //  count if all the collisions are of the same object
      if (  currentCollision && aNumberOfIntepenetrations >= 1 )
          aAllCollisionsAreFromSameObject = (id == prevContactRigidBodyId);

      pairCollides &= currentCollision;

      if ( resultContact == eContactResult::eContactRestingContact )              
      {
        storeRestingContact( aBodies[ id ], currentContact );
        *Global::mFileDebugContacts << "B "<< id << ": In contact. ["<<mDebugFrame<<"]"<<std::endl;
      }
      

      if ( currentCollision )         
      {
        ++aNumberOfIntepenetrations;        
        aCollidingObjects = mStaContactList[i];        
        prevContact = currentContact;
        aContact = currentContact;
        prevContactRigidBodyId = id;
        collisionFound      = true;
        *Global::mFileDebugContacts << "B "<< id << ": In interpt. ["<<mDebugFrame<<"]"<<std::endl;

        
        *Global::mFileDebugColOffenders << "Interp of obj-"<< currentContact.DEBUG_aId<< " with distance: " << currentContact.DEBUG_d << " against: [" << currentContact.DEBUG_bId << "]"<< " [" <<mDebugFrame <<"]" << std::endl;
        *Global::mFileDebugColOffenders << " --> "<< currentContact.offender.p1.x << ", "<< currentContact.offender.p1.y << ", "<< currentContact.offender.p1.z << " dist: " <<DxMath::distance( currentContact.offender.p1, aBodies[ id ]->xWS )  << std::endl;
        *Global::mFileDebugColOffenders << " --> "<< currentContact.offender.p2.x << ", "<< currentContact.offender.p2.y << ", "<< currentContact.offender.p2.z << " dist: " <<DxMath::distance( currentContact.offender.p2, aBodies[ id ]->xWS )  <<std::endl;
        *Global::mFileDebugColOffenders << " --> "<< currentContact.offender.p3.x << ", "<< currentContact.offender.p3.y << ", "<< currentContact.offender.p3.z << " dist: " <<DxMath::distance( currentContact.offender.p3, aBodies[ id ]->xWS )  <<std::endl;
        *Global::mFileDebugColOffenders << " ----> " << DxMath::distance( currentContact.offender.p1, aBodies[ id ]->xWS ) +
                                                        DxMath::distance( currentContact.offender.p2, aBodies[ id ]->xWS ) +
                                                        DxMath::distance( currentContact.offender.p3, aBodies[ id ]->xWS ) <<std::endl;
      }

    }
        
  }

  return collisionFound;
}


//===========================================================================
/*!
    Computes a fine-grain collision between a pair of objects.
    
    \param    aBodyA  first object
    \param    aBodyB  second object    
    \return   true<=> objects interpenetrate
*/
//===========================================================================
bool Physics::narrowCollision( RigidBody* aBodyA, RigidBody* aBodyB, const float aEpsilon, Contact& aContact )
{
  float distanceCenters = D3DXVec3Length( &(aBodyA->xWS - aBodyB->xWS) );
  bool interpenetration = false;

  float d = distanceCenters - (aBodyA->mBodyExt->mBoundingSphereRadius + aBodyB->mBodyExt->mBoundingSphereRadius);
  if ( d < aEpsilon )
  {
    interpenetration = true;    
  }

  if ( interpenetration )
    aContact = computeContact( aBodyA, aBodyB );

  return interpenetration;
}

bool Physics::narrowCollision( RigidBody* aBodyA, 
                              StaticRigidBody* aBodyB, 
                              const D3DXVECTOR3& aRayB,
                              const D3DXVECTOR3& aRayD,
                              const float aEpsilon, 
                              Contact& aContact, const bool aExtendedDebug,
                              const float aEpsilonBarycentric   )
{

  float d;
  bool interpenetration = aBodyB->rayIntesection( aRayB,
                                                  aRayD, 
                                                  aEpsilon,
                                                  mDynBodies[ aBodyA->mId ]->mBodyExt->mBoundingSphereRadius,
                                                  aContact.pointWS,
                                                  aContact.normalWS,
                                                  aContact.offender,
                                                  d,
                                                  aExtendedDebug,
                                                  aEpsilonBarycentric);

  //  Normal as returned by the function is of the triangle,
  //  and we want the opposite one

  if ( d != -1.0f )
    *Global::mFileDebugColOffenders << "I: " << d << " [" << aBodyB->mDebugId << "]" << " [" <<mDebugFrame <<"]"<< std::endl;
  

  D3DXVec3Normalize( &aContact.normalWS, &aContact.normalWS );
  if ( aContact.normalWS.x != 0.0f )
    aContact.normalWS.x *= -1.0f;
  if ( aContact.normalWS.y != 0.0f )
    aContact.normalWS.y *= -1.0f;
  if ( aContact.normalWS.z != 0.0f )
    aContact.normalWS.z *= -1.0f;
  DxMath::clean( aContact.normalWS );

  return interpenetration;
}

//===========================================================================
/*!
    Finds the time-position of the first interpenetration. As such if more
    than one is present, more resolution is needed in time step.
    Takes into account "mEpsilon" to decide if they collide.
    \param    aDeltaT   time resolution of the pretended step
    \param    aBodies   bodies (input/output)
    \param    aCollidingObjects   pair of objects that collide
    \param    aContact  their point of collision
    \param
    \return   time-position of the first interpenetration
*/
//===========================================================================
float Physics::findFirstTimeOfContact( float aDeltaT, 
                                      std::vector<RigidBody*>& aBodies, 
                                      PairBodies& aCollidingObjects,
                                      Contact& aContact,
                                      bool& aTimePointFound,
                                      int& aNumIterations)
{
  float timeOfInterPenetration;

  float timeStart             = 0.0f;
  float timeEnd               = aDeltaT;
  bool  timeOfUniqueCollision = false;
  aNumIterations = 0;
  Contact prevContact;
  int     prevContactRigidBodyId;
  

  while ( !timeOfUniqueCollision)
  {    
    ++aNumIterations;
    float testTimePosition = (timeEnd - timeStart) * 0.5f;

    std::vector<RigidBody*> bodies( numDynBodies() );
    for ( int i = 0; i < numDynBodies(); ++i )
      bodies[i] = new RigidBody( *aBodies[i] );
    
    tryToAdvanceSimulation ( timeStart + testTimePosition, bodies );

    int   numberOfIntepenetrations;
    bool  suitableInterpenetration = false; 
    bool  allCollisionsComeFromSameObject = false;
    
    suitableInterpenetration = findContacts(  bodies, 
                                              aCollidingObjects, 
                                              aContact, 
                                              numberOfIntepenetrations,
                                              allCollisionsComeFromSameObject);
          
  //  Check underflow
    if ((timeEnd - timeStart)< (mMinimumTimeOfCollision))
    {          
      *Global::mFileDebugTime
        << "Finding time failed. "<< " [" <<mDebugFrame <<"]"
        << std::endl;
            
      *Global::mFileDebugPhysGen << "---- Finding time failed ("<<aNumIterations<<" iterations)"<<std::endl;    
      *Global::mFileDebugCol << "---- Finding time failed ("<<aNumIterations<<" iterations)"<<std::endl;    
      
      // More than one collision was found, but given we can
      // no longer increase resolution, return just one of them
      timeOfUniqueCollision   = true;
      timeOfInterPenetration  = timeStart + testTimePosition;

      // copy back
      aTimePointFound = true;
      for ( int i = 0; i < numDynBodies(); ++i )
        *aBodies[i] = *bodies[i];
      return timeOfInterPenetration;
    }

    //  First check if we have already found an interpenetration we can solve
    if ( suitableInterpenetration && numberOfIntepenetrations == 1)
    {
      
      timeOfUniqueCollision   = true;
      timeOfInterPenetration  = timeStart + testTimePosition;

      *Global::mFileDebugCol << "---- Time found: "<< timeOfInterPenetration <<std::endl;    
      

      // copy back
      aTimePointFound = true;
      for ( int i = 0; i < numDynBodies(); ++i )
        *aBodies[i] = *bodies[i];
    }
    else
    {
      //  Otherwise halve time interval
      ( numberOfIntepenetrations == 0 ) ?
            timeStart = timeStart + testTimePosition
          : timeEnd = timeStart + testTimePosition;

      if ( allCollisionsComeFromSameObject )
      {
        int id = aContact.DEBUG_aId;
        mPrecisionPerBody[id].mEpsilonBarycentric *= 0.5f;
        mPrecisionPerBody[id].mIterationsReset    = 30;
        *Global::mFileDebugColOffenders << std::endl<< "barycentric precision halved" << std::endl;
      }


      *Global::mFileDebugColOffenders << std::endl;
      
#ifndef _DEBUG_SHOW_ONLY_DOT_INFO    
      for ( int i = 0; i < bodies.size() ; ++i )
      {
        Global::mFileDebugBody[i]->precision(8);
        *Global::mFileDebugBody[i]           
          << " X: " << bodies[i]->xWS.x << ", " << bodies[i]->xWS.y << ", " << bodies[i]->xWS.z << "" 
          << std::endl;
        Global::mFileDebugBody[i]->precision(2);
      }
#endif

      *Global::mFileDebugTime
        << " Reduced: " << timeEnd - timeStart << " [" <<mDebugFrame <<"]"        
        << std::endl;
    } //end of test for unique intersection

  

    for ( int i = 0; i < numDynBodies(); ++i )
      delete bodies[i];
  }
  
  return timeOfInterPenetration;
}

void Physics::perFrameCountersReset()
{
  mFcNumIterations  = 0;
  mFcNumObjects     = 0;
}

void Physics::perFrameCountersPrint()
{
  if ( mFcNumIterations != 0)
  *Global::mFileDebugPhysGen 
      << "Iterations: "
      << mFcNumIterations
      << " Mean of objects collided against: " 
      << mFcNumObjects/mFcNumIterations
      << std::endl;
}