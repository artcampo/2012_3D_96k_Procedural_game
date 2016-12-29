#include "Geometry\Spline\SplineBezierTMapped.hpp"
#include "Geometry\DxMathHelpers.hpp""

//===========================================================================
/*!
    Init.

    \param      *aData    Pointer to D3DXVECTOR3 array
    \param      aNumNodes Number of nodes

*/
//===========================================================================

SplineBezierTMapped::SplineBezierTMapped( const D3DXVECTOR3* aData, int aNumNodes, int aPointsPerArch )
{
  mMinT = 0.01f;

  mPointsPerArch = aPointsPerArch;
  mNumNodes = aNumNodes;
  mNodes.resize( mNumNodes );
  
  mNumPoints  = ((aNumNodes - 1) / 3) + 1;
  mNumArcs    = mNumPoints - 1;

  mAccLength.resize( mNumArcs + 1 );

  for ( int i = 0; i < mNumNodes ; ++i)
  {
    mNodes[i] = *(aData + i);
  }

  computeLengths();

  std::vector<D3DXVECTOR3> p;
  std::vector<D3DXVECTOR3> ta; 

  int max = 100;
  for ( int i = 0; i < max ; ++i)
  {
    if ( i== 33 )
      int a = 0;
    float t = float(i)/float(max);
    p.push_back( getPoint( t ));
    ta.push_back( getPointTangent( t ));

  }



}

int SplineBezierTMapped::indexOfPoint( int aIndex ) const
{
  return (3*aIndex);
}
 
SplineBezierTMapped::~SplineBezierTMapped()
{  	

}

int   SplineBezierTMapped::getNumPoints ()
{
  return mNumPoints;
}

int   SplineBezierTMapped::getNumArcs ()
{
  return mNumArcs;
}


void SplineBezierTMapped::computeLengths()
{

 mAccLength[1] = 0.0f;

 for ( int i = 0; i < getNumArcs() ; ++i)
  {
    
    float distance = float(1) / float (getNumPoints());
    mAccLength[i + 1] = mAccLength[i] + distance;
  }

 mTotalLength = mAccLength[getNumArcs()];
}

//===========================================================================
/*!
    Get point in spline

    \param      t       0<=t<=1
    \returns    point at length t

*/
//===========================================================================

D3DXVECTOR3 SplineBezierTMapped::getPoint ( float aT )
{   
  float t = aT;
  if ( aT < 0.0f ) t = 0.0f;
  if ( aT > 1.0f ) t = 1.0f;
    
  float range = 1.0f/float(getNumArcs());
  int index   = int( t/range );
  float archT = t - (float(index)*range );
  archT = archT / range;

  if (t == 0.0f)
    return mNodes[0];
  if (t == 1.0f)
    return mNodes[mNodes.size() - 1];
  else
  {
    D3DXVECTOR3 result;
    int i = indexOfPoint( (index) );
    D3DXVECTOR3 p0 = mNodes[i];
    D3DXVECTOR3 p1 = mNodes[i + 1];
    D3DXVECTOR3 p2 = mNodes[i + 2];
    D3DXVECTOR3 p3 = mNodes[i + 3];
    float t = archT;
    result = 
        t*t*t*(p3 + 3.0f*(p1 - p2) - p0) +
        3*t*t*(p0 - 2.0f*p1 + p2) +
        3*t*  (p1 - p0) +
        p0;
    return result;
  }


}

D3DXVECTOR3 SplineBezierTMapped::getPointTangent ( float aT )
{
  float t = aT;
  if ( aT < 0.0f ) t = 0.0f;
  if ( aT > 1.0f ) t = 1.0f;
  
  float range = 1.0f/float(getNumArcs());
  int index   = int( t/range );
  float archT = t - (float(index)*range );
  archT = archT / range;

    
  D3DXVECTOR3 result;
  if (t == 0.0f)
  {
    D3DXVECTOR3 p0 = mNodes[0];
    D3DXVECTOR3 p1 = mNodes[1];
    D3DXVECTOR3 p2 = mNodes[2];
    D3DXVECTOR3 p3 = mNodes[3];
    float t = mMinT;
    result = 
        3*t*t*(p3 + 3.0f*(p1 - p2) - p0) +
        6*t*(p0 - 2.0f*p1 + p2) +
        3*  (p1 - p0);

    return result;
  }
  if (t == 1.0f)
  {
    D3DXVECTOR3 p0 = mNodes[ mNodes.size() - 4 ];
    D3DXVECTOR3 p1 = mNodes[ mNodes.size() - 3 ];
    D3DXVECTOR3 p2 = mNodes[ mNodes.size() - 2 ];
    D3DXVECTOR3 p3 = mNodes[ mNodes.size() - 1 ];
    float t = 1.0f - mMinT;
    result = 
        3*t*t*(p3 + 3.0f*(p1 - p2) - p0) +
        6*t*(p0 - 2.0f*p1 + p2) +
        3*  (p1 - p0);

    return result;
  }
  else
  {
    D3DXVECTOR3 result;


    int i = indexOfPoint( (index) );
    D3DXVECTOR3 p0 = mNodes[i];
    D3DXVECTOR3 p1 = mNodes[i + 1];
    D3DXVECTOR3 p2 = mNodes[i + 2];
    D3DXVECTOR3 p3 = mNodes[i + 3];
    float t = archT;
    result = 
        3*t*t*(p3 + 3.0f*(p1 - p2) - p0) +
        6*t*(p0 - 2.0f*p1 + p2) +
        3*  (p1 - p0);

    return result;
  }

}


D3DXVECTOR3 SplineBezierTMapped::getSecondDerivative ( float aT )
{
  float t = aT;
  if ( aT < 0.0f ) t = 0.0f;
  if ( aT > 1.0f ) t = 1.0f;

  float range = 1.0f/float(getNumArcs());
  int index   = int( t/range );
  float archT = t - (float(index)*range );
  archT = archT / range;
    
  D3DXVECTOR3 result;
  if (t == 0.0f)
  {
    D3DXVECTOR3 p0 = mNodes[0];
    D3DXVECTOR3 p1 = mNodes[1];
    D3DXVECTOR3 p2 = mNodes[2];
    D3DXVECTOR3 p3 = mNodes[3];
    float t = mMinT;
    result = 
        6*t*(p3 + 3.0f*(p1 - p2) - p0) +
        6*(p0 - 2.0f*p1 + p2);

    return result;
  }
  if (t == 1.0f)
  {
    D3DXVECTOR3 p0 = mNodes[ mNodes.size() - 4 ];
    D3DXVECTOR3 p1 = mNodes[ mNodes.size() - 3 ];
    D3DXVECTOR3 p2 = mNodes[ mNodes.size() - 2 ];
    D3DXVECTOR3 p3 = mNodes[ mNodes.size() - 1 ];
    float t = 1.0f - mMinT;
    result = 
        6*t*(p3 + 3.0f*(p1 - p2) - p0) +
        6*(p0 - 2.0f*p1 + p2);

    return result;
  }
  else
  {
    D3DXVECTOR3 result;

    int i = indexOfPoint( (index) );
    D3DXVECTOR3 p0 = mNodes[i];
    D3DXVECTOR3 p1 = mNodes[i + 1];
    D3DXVECTOR3 p2 = mNodes[i + 2];
    D3DXVECTOR3 p3 = mNodes[i + 3];
    float t = archT;
    result = 
        6*t*(p3 + 3.0f*(p1 - p2) - p0) +
        6*(p0 - 2.0f*p1 + p2);

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



//===========================================================================
/*!
    Get maximum and minimum points
    \param     aMax   max point 
    \param     aMin   min point

*/
//===========================================================================

void SplineBezierTMapped::getMaxMinMean ( D3DXVECTOR3& aMax, D3DXVECTOR3& aMin, D3DXVECTOR3& aMean )
{
  /*
  aMax = getPoint( 0.0f );
  aMin = getPoint( 0.0f );
  aMean = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
  */
  int n = 0;
  for ( int i = 0 ; i < (getNumArcs()*mPointsPerArch) ; ++i)
  {
    float t = float(i) / float ( (getNumArcs()*mPointsPerArch) );
    D3DXVECTOR3 p = getPoint ( t );
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

std::vector<D3DXVECTOR3> SplineBezierTMapped::getAllCurvePoints (  int aPointsPerArch )
{
  std::vector<D3DXVECTOR3> points ( getNumArcs()*aPointsPerArch );
  for ( int i = 0 ; i < (getNumArcs()*aPointsPerArch) ; ++i)
  {
    float t = float(i) / float ( (getNumArcs()*aPointsPerArch) );
    points[i] = getPoint(t);
  }
  return points;
}

std::vector<D3DXVECTOR3> SplineBezierTMapped::getAllCurvePoints ( )
{
  return getAllCurvePoints(mPointsPerArch);
}

D3DXVECTOR3 SplineBezierTMapped::getSplineNode ( int aIndex )
{
  return mNodes[ indexOfPoint(aIndex) ];
}

//===========================================================================
/*!
    Returns the t such that spline(t) is the nearest to aPoint

    \param      aPoint      Point
    \param      aPrecision  Number of iterations (the more the better)

*/
//===========================================================================

float SplineBezierTMapped::nearestPathPoint( const D3DXVECTOR3& aPoint, const int aPrecision )
{
  float mind = DxMath::distance( aPoint, getPoint(0) );
  float mint = 0;

  for ( int i = 1; i < aPrecision; ++i )
  {
    float t = float(i)/ float(aPrecision);
    float d = DxMath::distance( aPoint, getPoint(t) );
    if ( d < mind )
    {
      mind = d;
      mint = t;
    }
  }

  return mint;
}

//===========================================================================
/*!
    Advances t as to make an increment of aUnits    

    \param      aUnits      min distance( spline(t), spline(newt)
    \param      aT          current t
    \param      aNewT       new t after advancing aUnits
    \return     true<=> the point is inside this path

*/
//===========================================================================

bool SplineBezierTMapped::advanceAlongPathNunits( const float aUnits, 
                                                 const float aT, 
                                                 const float aMaxT,
                                                 float& aNewT, 
                                                 float& aAdvancedUnits,
                                                 bool& aMaxAngleSurpassed,
                                                 const float aMaxAngle,                                                 
                                                 const D3DXVECTOR3& aBallPos,
                                                 const D3DXVECTOR3& aBallVel,
                                                 BallIA::Target& aOutTarget)
{
  float       tPrecision = 0.0001f;
  float       t = aT;
  D3DXVECTOR3 initialPoint   = getPoint( aT );
  D3DXVECTOR3 initialTangent = getPointTangent( aT );
  initialTangent.y = 0;
  bool        finished = false;
  bool        insidePath;
  aMaxAngleSurpassed = false;  

  while (!finished )
  {
    t += tPrecision;

    if ( t >= 1.0f )
    {
      finished    = true;
      insidePath  = false;
      aAdvancedUnits = DxMath::distance( initialPoint, getPoint( 1 ) );
    }
    else
    {
      D3DXVECTOR3 point = getPoint( t );
      float distance = DxMath::distance( initialPoint, point );

      //  Have we reached desired distance
      if ( distance >= aUnits  )
      {
        finished    = true;
        insidePath  = true;
        aNewT       = t;
        aOutTarget.splineT    = t;
        aOutTarget.point      = point;
        aOutTarget.hasEntity  = false;
      }

      //  Is there an entity?
      if ( t >= aMaxT )
      {
        finished    = true;
        insidePath  = true;
        aNewT       = t;
        aOutTarget.splineT    = t; 
        aOutTarget.point      = point;
        aOutTarget.hasEntity  = true;        
      }

      D3DXVECTOR3 currentTangent = getPointTangent(t);
      currentTangent.y = 0;
      float angle = DxMath::angleBetweenVectors( initialTangent, currentTangent );           

      if ( angle > aMaxAngle )
      {        
        aMaxAngleSurpassed = true;
        aOutTarget.splineT    = t;
        aOutTarget.point      = point;
        aOutTarget.hasEntity  = false;
        return true;
      }
    }
  }

  return insidePath;
}

//===========================================================================
/*!
    Advances t as to make an increment of aUnits    

    \param      aUnits      min distance( spline(t), spline(newt)
    \param      aT          current t
    \param      aNewT       new t after advancing aUnits
    \return     true<=> the point is inside this path

*/
//===========================================================================

bool SplineBezierTMapped::advanceAlongPathNunits( const float aUnits, 
                                                 const float aT, 
                                                 float& aNewT )
{
  float       sign        = aUnits/fabs(aUnits);
  float       tPrecision  = 0.0001f*sign; //this has sign!!
  float       units       = fabs( aUnits );
  float       t = aT;
  D3DXVECTOR3 initialPoint   = getPoint( aT );
  bool        finished = false;
  bool        insidePath;  

  while (!finished )
  {
    t += tPrecision;

    if ( t >= 1.0f || t <= 0.0f )
    {
      finished    = true;
      insidePath  = false;      
    }
    else
    {
      D3DXVECTOR3 point = getPoint( t );
      float distance = DxMath::distance( initialPoint, point );
      if ( distance >= units )
      {
        finished    = true;
        insidePath  = true;
        aNewT       = t;        
      }
    }
  }

  return insidePath;
}