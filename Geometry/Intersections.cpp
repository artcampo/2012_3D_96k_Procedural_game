#include "Intersections.hpp"
#include "Global.hpp"

//#define _DEBUG_EXTENDED_LOG        

namespace Intersections{

float distance ( D3DXVECTOR3& a, D3DXVECTOR3& b )
{
  D3DXVECTOR3 v = a - b;
  return fabs( D3DXVec3Length( &v ) );
}

bool areCollinear ( D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c )
{
  float ab = distance(a,b);
  float ac = distance(a,c);
  float bc = distance(b,c);
  float epsilon = ac + bc - ab;
  epsilon = fabs(epsilon);

  if ( epsilon < 0.1f )
    return true;
  else
    return false;
}

float distancePointToLine ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXVECTOR3 aPoint )
{
  D3DXVECTOR3 director = aRayE - aRayB;
  D3DXVECTOR3 w = aPoint - aRayB;

  float c1 = D3DXVec3Dot( &director, &w );
  float c2 = D3DXVec3Dot( &director, &director );
  float b = c1/c2;

  D3DXVECTOR3 p = aRayB + b*director;
  D3DXVECTOR3 d =  p - aPoint;
  return D3DXVec3Length( &d );
}

float distancePointToSegment ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXVECTOR3 aPoint )
{
  D3DXVECTOR3 director = aRayE - aRayB;
  D3DXVECTOR3 w = aPoint - aRayB;

  float c1 = D3DXVec3Dot( &director, &w );
  float c2 = D3DXVec3Dot( &director, &director );
  float b = c1/c2;

  D3DXVECTOR3 p = aRayB + b*director;
  if ( b < 0 ) p = aRayB;
  if ( b > 1 ) p = aRayE;

  D3DXVECTOR3 d =  p - aPoint;
  return D3DXVec3Length( &d );
}

D3DXVECTOR3 distancePointToLineVector ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXVECTOR3 aPoint )
{
  D3DXVECTOR3 director = aRayE - aRayB;
  D3DXVECTOR3 w = aPoint - aRayB;

  float c1 = D3DXVec3Dot( &director, &w );
  float c2 = D3DXVec3Dot( &director, &director );
  float b = c1/c2;

  D3DXVECTOR3 p = aRayB + b*director;

  return p;
}

// l1 = seg
// l2 = lin
float distanceSegmentToLine ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE,
                              D3DXVECTOR3 aSegB, D3DXVECTOR3 aSegE,
                              D3DXVECTOR3& aPointInSegment)
{
  D3DXVECTOR3 u = aSegE - aSegB;
  D3DXVECTOR3 v = aRayE - aRayB;
  D3DXVECTOR3 w = aSegB - aRayB;
  float a = D3DXVec3Dot( &u, &u );
  float b = D3DXVec3Dot( &u, &v );
  float c = D3DXVec3Dot( &v, &v );
  float d = D3DXVec3Dot( &u, &w );
  float e = D3DXVec3Dot( &v, &w );
  float D = a*c - b*b;
  float sc, tc;

  if ( D < 0.0001f )
  {
    sc = 0.0f;
    tc = (b>c ? d/b : e/c);
  }
  else
  {
    sc = (b*e - c*d) / D;
    tc = (a*e - b*d) / D;
  }

  float distance;
  D3DXVECTOR3 dP;

  if ( sc > 0.0f && sc < 1.0f )
  {
    dP = w + (sc * u) - (tc * v);
    distance = D3DXVec3Length( &dP );
    aPointInSegment = (sc * u) + aSegB;
  }
  else
  {

    float d1, d2;
    dP = w - (tc * v);
    d1 = D3DXVec3Length( &dP );
    dP = w + u - (tc * v);
    d2 = D3DXVec3Length( &dP );
    distance = (d1<d2 ? d1 : d2 );
  }

  return distance;
}

//  line is not contained into the plane
D3DXVECTOR3 intersectionLinePlane ( D3DXVECTOR3 aRayB,
                                    D3DXVECTOR3 aRayE,
                                    Plane& aPlane,
                                    bool& aUniqueSolution )
{
  D3DXVECTOR3 lineVectorDirector = aRayE - aRayB;
  float num = D3DXVec3Dot( &aPlane.normal(), &(aPlane.point() - aRayB) );
  float den = D3DXVec3Dot( &aPlane.normal(), &lineVectorDirector );

  D3DXVECTOR3 intersectionPoint;
  if ( den != 0.0f )
  {
    float t = num / den;
    aUniqueSolution = true;
    intersectionPoint = aRayB + t*lineVectorDirector;
  }
  else
  {
    aUniqueSolution = false;
  }
  return intersectionPoint;
}

Plane::Plane(D3DXVECTOR3 u, D3DXVECTOR3 v, D3DXVECTOR3 p)
{
  D3DXVec3Cross( &mNormal, &u, &v );
  D3DXVec3Normalize( &mNormal, &mNormal );
  mPoint = p;
}

D3DXVECTOR3 Plane::normal()
{
  return mNormal;
}

float Plane::implicitD()
{
  return -D3DXVec3Dot( &mNormal, &mPoint );
}

D3DXVECTOR3 Plane::point()
{
  return mPoint;
}



Plane Plane::planeBy3Points(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c)
{
  return Intersections::Plane( a-c, b-c, c );
}


bool  raySphere( const BoundingSphere& aSphere, D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE )
{
  D3DXVECTOR3 raydir = aRayE - aRayB;
float a = D3DXVec3Dot(&raydir,&raydir);
float b = D3DXVec3Dot(&raydir, &(2.0f * ( aRayB - aSphere.center)));
float c = D3DXVec3Dot( &aSphere.center, &aSphere.center) + D3DXVec3Dot(&aRayB,&aRayB) -2.0f*D3DXVec3Dot(&aRayB,&aSphere.center) - aSphere.radius*aSphere.radius;
float D = b*b + (-4.0f)*a*c;

// If ray can not intersect then stop
if (D < 0)
        return false;
D=sqrtf(D);

// Ray can intersect the sphere, solve the closer hitpoint
float t = (-0.5f)*(b+D)/a;
if (t > 0.0f)
        {
return true;
        }
else
        return false;

}

//  \param  (a,b,c) ordered vertices of a triangle
//  \return true <=> they are front facing
bool  frontFacing ( D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c, const D3DXVECTOR3 aCamPos )
{
  D3DXVECTOR3 u = b - a;
  D3DXVECTOR3 v = c - a;
  D3DXVECTOR3 n;
  D3DXVec3Cross( &n, &u, &v );
  D3DXVec3Normalize( &n, &n );

  D3DXVECTOR3 camToFace = a - aCamPos;

  float ret = D3DXVec3Dot( &n, &camToFace );
  return (ret > 0);
}

float signedArea( D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c)
{
  float a11 = b.x - a.x;
  float a12 = c.x - a.x;
  float a21 = b.y - a.y;
  float a22 = c.y - a.y;

  return 0.5f * (a11*a22 - a21*a12);
}

// true   <=> segments intersect, and the intersection is not in any vertex
// false  <=> segments do not intersect, or touch in either's vertex
bool segmentIntersectionExclusive ( D3DXVECTOR2 p, D3DXVECTOR2 q, D3DXVECTOR2 r, D3DXVECTOR2 s)
{
  bool	intersect 	= false;
  bool	done		= false;

  float d_pqr = signedArea( p , q , r );
  float d_pqs = signedArea( p , q , s );
  float d_rsp = signedArea( r , s , p );
  float d_rsq = signedArea( r , s , q );

  //Case a: all points are colinear
  if ( d_pqr == 0.0f && d_pqs == 0.0f && d_rsp == 0.0f && d_rsq == 0.0f)
  {

          if ( third_is_in_between( p , q , r) || third_is_in_between( p , q , s) ||
               third_is_in_between( r , s , p) || third_is_in_between( r , s , q) )
                  {
                  intersect = true;
                  }
          done = true;
  }

  //Case b: one vertex shared
  if ((! done) && ( (d_pqr*d_pqs == 0.0f) && (d_rsp*d_rsq == 0.0f)))
  {
          intersect = false;
          done = true;
  }

  //Case c: one vertex in the other segment
  if ((! done) && (  (d_pqr*d_pqs == 0.0f) || (d_rsp*d_rsq == 0.0f)))
  {
          intersect = false;
          done = true;
  }

  //Case d: intersection
  if ( (!done) && ( ((d_pqr*d_pqs) < 0) && ((d_rsp*d_rsq) < 0) ) )
  {
          intersect = true;
          done = true;
  }

  return intersect;
}


bool Intersections::third_is_in_between (D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c)
{
  if ( a.x == b.x )
          {
          return(
                  ( (a.y <= c.y) && (b.y >= c.y))
                  ||
                  ( (a.y >= c.y) && (b.y <= c.y))
                  );
          }
  else
          {
          return(
                  ( (a.x <= c.x) && (b.x >= c.x))
                  ||
                  ( (a.x >= c.x) && (b.x <= c.x))
                  );
          }
}

D3DXVECTOR2 pointOfIntersectingSegments ( D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3, D3DXVECTOR2 p4)
{
  float num = (p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y)*(p1.x - p3.x);
  float den = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x)*(p2.y - p1.y);
  float t = num/den;
  D3DXVECTOR2 point = p1 + t*(p2 - p1);
  return point;
}

float  distancePointSphere( const BoundingSphere& aSphere, const D3DXVECTOR3& p )
{
  D3DXVECTOR3 dv = aSphere.center - p;
  float d = D3DXVec3Length( &dv );
  d -= aSphere.radius;
  return d;
}

bool intersectionPointTriangle( const D3DXVECTOR3 aPoint, 
                        const float aEpsilon,
                        const CollisionTriangle& aTriangle)
{
  bool collision = false;

  D3DXVECTOR3 p1 = aTriangle.p1;
  D3DXVECTOR3 p2 = aTriangle.p2;
  D3DXVECTOR3 p3 = aTriangle.p3;

  D3DXVECTOR3 rayB = aPoint;
  D3DXVECTOR3 rayD = DxMath::normalOfTriangle( p1, p2, p3 );

  //  Avoid collinear points in a face's edge
  float distance, u, v;            
  bool hit = false;
  if ( ! Intersections::areCollinear(p1,p2,p3) )        
  {
      hit = D3DXIntersectTri( &p1, &p2, &p3, &rayB, &rayD, &u, &v, &distance );
      if (! hit )
      {
        rayD *= -1.0f;
        hit = D3DXIntersectTri( &p3, &p2, &p1, &rayB, &rayD, &u, &v, &distance );
      }
  }

  //D3DXVECTOR3 distVector = distance*aRayD;


  if ( hit )
  {
    D3DXVECTOR3 dist = distance*rayD;              
    float d = D3DXVec3Length( &dist );      
    *Global::mFileDebugColOffenders << " d: " <<d << std::endl;

    if (  d < aEpsilon )
    {
      collision = true;
    }
  }


  return collision;
}

float intersectionPointTriangleDebug( const D3DXVECTOR3 aPoint, 
                        const float aEpsilon,
                        const CollisionTriangle& aTriangle)
{
  bool collision = false;

  D3DXVECTOR3 p1 = aTriangle.p1;
  D3DXVECTOR3 p2 = aTriangle.p2;
  D3DXVECTOR3 p3 = aTriangle.p3;

  D3DXVECTOR3 rayB = aPoint;
  D3DXVECTOR3 rayD = DxMath::normalOfTriangle( p1, p2, p3 );

  //  Avoid collinear points in a face's edge
  float distance, u, v;            
  bool hit = false;
  if ( ! Intersections::areCollinear(p1,p2,p3) )        
  {
      hit = D3DXIntersectTri( &p1, &p2, &p3, &rayB, &rayD, &u, &v, &distance );
      if (! hit )
      {
        rayD *= -1.0f;
        hit = D3DXIntersectTri( &p3, &p2, &p1, &rayB, &rayD, &u, &v, &distance );
      }
  }

  //D3DXVECTOR3 distVector = distance*aRayD;

  
  if ( hit )
  {
    D3DXVECTOR3 dist = distance*rayD;              
    float d = D3DXVec3Length( &dist );      
    *Global::mFileDebugColOffenders << " d: " <<d << std::endl;

    if (  d < aEpsilon )
    {
      collision = true;
    }
    return d;
  }


  return 1000.0f;
}

bool pointInTriangle( const D3DXVECTOR3& aPoint, 
                      const D3DXVECTOR3& a, const D3DXVECTOR3& b, const D3DXVECTOR3& c,
                      const float aEpsilonBarycentric )
{
  bool collision = false;

  D3DXVECTOR3 v0 = c - a;
  D3DXVECTOR3 v1 = b - a;
  D3DXVECTOR3 v2 = aPoint - a;

  float dot00 = D3DXVec3Dot( &v0, &v0 );
  float dot01 = D3DXVec3Dot( &v0, &v1 );
  float dot11 = D3DXVec3Dot( &v1, &v1 );
  float dot10 = D3DXVec3Dot( &v1, &v0 );
  float dot20 = D3DXVec3Dot( &v2, &v0 );
  float dot21 = D3DXVec3Dot( &v2, &v1 );

  float den = dot00*dot11 - dot01*dot10;
  float u   = dot11*dot20 - dot10*dot21; 
  float v   = dot00*dot21 - dot01*dot20; 
  u         /= den;
  v         /= den;
  
#ifdef _DEBUG_EXTENDED_LOG          
  *Global::mFileDebugColOffenders << "||||||||  u " << u  << std::endl;
  *Global::mFileDebugColOffenders << "||||||||  v " << v  << std::endl;  
#endif
  
  bool inside = DxMath::greaterThan ( u,      0.0, aEpsilonBarycentric ) &&
                DxMath::greaterThan ( v,      0.0, aEpsilonBarycentric ) &&
                DxMath::lesserThan  ( u + v,  1.0, aEpsilonBarycentric ) ;
  return inside;
}


bool  intersectionSphereSphere( const BoundingSphere& aSphere1, const BoundingSphere& aSphere2 )
{
  float d = DxMath::distance( aSphere1.center, aSphere2.center );
  return d > (aSphere1.radius + aSphere2.radius);
}

};
