#include "Geometry/Spline/SplineTMapped.hpp"


SplineTMapped::SplineTMapped( D3DXVECTOR3* aData, 
                              int aNumNodes,
                              float aTime0,
                              float aTimeF,
                              int aRepeatMode)
{
  mTime0 = aTime0;
  mTimeF = aTimeF;
  mRepeatMode = aRepeatMode;

  mNumNodes = aNumNodes;
  mNodes.resize( mNumNodes );
  mAccLength.resize( mNumNodes );

  for ( int i = 0; i < mNumNodes ; ++i)
  {
    mNodes[i] = *(aData + i);
  }

  computeLengths();
}

D3DXVECTOR3 SplineTMapped::getPointViaTime ( float aTime )
{
  float time;
  if ( aTime < mTime0 || aTime > mTimeF )
  {
    if ( mRepeatMode == eRepeatModes::Clamp)
    {
      if ( aTime < mTime0)
        time = mTime0;
      if ( aTime > mTimeF)
        time = mTimeF;
    }
    if ( mRepeatMode == eRepeatModes::Mod)
    {
      time = fmod( aTime, (mTimeF - mTime0) );
    }
  }
  else
    time = aTime;

  float t = (time - mTime0) / (mTimeF - mTime0) ;
  return getPoint ( t );
}

//===========================================================================
/*!
    Init.

    \param      *aData    Pointer to D3DXVECTOR3 array
    \param      aNumNodes Number of nodes

*/
//===========================================================================

SplineTMapped::SplineTMapped( D3DXVECTOR3* aData, int aNumNodes )
{
  mTime0 = 0;
  mTimeF = 1;
  mRepeatMode = eRepeatModes::Clamp;

  mNumNodes = aNumNodes;
  mNodes.resize( mNumNodes );
  mAccLength.resize( mNumNodes );

  for ( int i = 0; i < mNumNodes ; ++i)
  {
    mNodes[i] = *(aData + i);
  }

  computeLengths();
}

//===========================================================================
/*!
    Init.

    \param      *aData    Pointer to consecutive floats (3 each)
    \param      aNumNodes Number of nodes

*/
//===========================================================================

SplineTMapped::SplineTMapped( float* aData, int aNumNodes )
{
  mNumNodes = aNumNodes;
  mNodes.resize( mNumNodes );
  mAccLength.resize( mNumNodes );

  for ( int i = 0; i < mNumNodes ; ++i)
  {
    mNodes[i].x = *(aData + 3*i + 0);
    mNodes[i].y = *(aData + 3*i + 1);
    mNodes[i].z = *(aData + 3*i + 2);
  }

  computeLengths();
}
 
SplineTMapped::~SplineTMapped()
{  	

}

int   SplineTMapped::getNumNodes ()
{
  return mNumNodes-2;
}

void SplineTMapped::computeLengths()
{

 mAccLength[1] = 0.0f;

 for ( int i = 2; i < (mNumNodes - 1) ; ++i)
  {
    
    float distance = float(1) / float (mNumNodes - 2);

    mAccLength[i] = mAccLength[i - 1] + distance;
  }

 mTotalLength = mAccLength[mNumNodes - 2];
}

//===========================================================================
/*!
    Get point in spline

    \param      t       0<=t<=1
    \returns    point at length t

*/
//===========================================================================

D3DXVECTOR3 SplineTMapped::getPoint ( float t )
{
  bool  nodeFound = false;
  int   index     = 1;
  while (!nodeFound)
  {
    float currentParam = mAccLength[index] / mTotalLength;
    if ( t <= currentParam )
      nodeFound = true;
    else
      ++index;

  }

  --index;
  float currentParam = mAccLength[index] / mTotalLength;
  float targetT = mTotalLength * t;
  float archT;
  if ( t == 0.0f || ( targetT - mAccLength[index] == 0.0f))
    archT = 0.0f;
  else
  {
    float range = mAccLength[index + 1] - mAccLength[index];
    float interRangePos = targetT - mAccLength[index];
    archT = interRangePos / range;
  }
    
  if (t == 0.0f)
    return mNodes[1];
  if (t == 1.0f)
    return mNodes[mNumNodes - 2];
  else
  {
    D3DXVECTOR3 result;
    D3DXVec3CatmullRom (  &result, &mNodes[index - 1], &mNodes[index - 0],
                          &mNodes[index + 1], &mNodes[index + 2], archT);

    return result;
  }


}

//===========================================================================
/*!
    Get normal of point in spline

    \param      t       0<=t<=1
    \returns    normal at length t

*/
//===========================================================================

D3DXVECTOR3 SplineTMapped::getPointNormal ( float t )
{
  bool  nodeFound = false;
  int   index     = 1;
  while (!nodeFound)
  {
    float currentParam = mAccLength[index] / mTotalLength;
    if ( t <= currentParam )
      nodeFound = true;
    else
      ++index;

  }

  --index;
  float currentParam = mAccLength[index] / mTotalLength;
  float targetT = mTotalLength * t;
  float archT;
  if ( t == 0.0f || ( targetT - mAccLength[index] == 0.0f))
    archT = 0.0f;
  else
  {
    float range = mAccLength[index + 1] - mAccLength[index];
    float interRangePos = targetT - mAccLength[index];
    archT = interRangePos / range;
  }
    
  if (t == 0.0f)
    return mNodes[1];
  if (t == 1.0f)
    return mNodes[mNumNodes - 2];
  else
  {
    D3DXVECTOR3 result      
      = 0.5f *
       (
        1.0f  * ( -mNodes[index - 1] + mNodes[index + 1]) +
        archT * ( 2.0f*mNodes[index - 1] - 5.0f*mNodes[index - 0] + 4.0f*mNodes[index + 1] - mNodes[index + 2] ) +
        archT*archT * ( -1.0f*mNodes[index - 1] + 3.0f*mNodes[index - 0] - 3.0f*mNodes[index + 1] + mNodes[index + 2] ) 
       );
    D3DXVec3Normalize ( &result, &result);
      
    return result;
  }


}


//===========================================================================
/*!
    Get maximum and minimum points
    \param     aMax   max point 
    \param     aMin   min point

*/
//===========================================================================

void SplineTMapped::getMaxMinMean ( D3DXVECTOR3& aMax, D3DXVECTOR3& aMin, D3DXVECTOR3& aMean )
{
  aMax = getPoint( 0.0f );
  aMin = getPoint( 0.0f );
  aMean = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
  int n = 0;
  for ( int i = 1 ; i < (mNumNodes*60) ; ++i)
  {
    D3DXVECTOR3 p = getPoint ( float(i)/float(mNumNodes*60) );
    aMean += p;
    ++n;
    if ( p.x > aMax.x ) aMax.x = p.x;
    if ( p.x < aMin.x ) aMin.x = p.x;
    if ( p.y > aMax.y ) aMax.y = p.y;
    if ( p.y < aMin.y ) aMin.y = p.y;
    if ( p.z > aMax.z ) aMax.z = p.z;
    if ( p.z < aMin.z ) aMin.z = p.z;
  }

  aMean /= float(n);
}