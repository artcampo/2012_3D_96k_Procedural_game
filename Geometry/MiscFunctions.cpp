#include		"Geometry/MiscFunctions.hpp"

namespace GeometryMiscFunctions
{

D3DXVECTOR3	NormalOfTriangle ( const D3DXVECTOR3  *v0, const D3DXVECTOR3  *v1, const D3DXVECTOR3  *v2 )
{
  D3DXVECTOR3  res2,res;
  D3DXVECTOR3 u = *(v1) - *(v0);
  D3DXVECTOR3 v = *(v2) - *(v0);
	D3DXVec3Cross ( &res2,  &u, &v );
	D3DXVec3Normalize ( &res, &res2);
	return res;
}

D3DXVECTOR3 perpendicular3D(const D3DXVECTOR3& th)
{
    D3DXVECTOR3 res;
    res = D3DXVECTOR3 (
      -th.y,
      th.x,
      th.z );
    if (res.x == -0.0f) res.x = 0.0f;
    if (res.y == -0.0f) res.y = 0.0f;
    if (res.z == -0.0f) res.z = 0.0f;
    return res;
}


D3DXMATRIX basisMatrix ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3 )
{
  
  D3DXMATRIX ret = D3DXMATRIX(
    v1.x, v2.x, v3.x, 0.0f, 
    v1.y, v2.y, v3.y, 0.0f, 
    v1.z, v2.z, v3.z, 0.0f, 
    0.0f, 0.0f, 0.0f, 1.0f 
    );
  /*
  D3DXMATRIX ret = D3DXMATRIX(
    v1.x, v1.y, v1.z, 0.0f, 
    v2.x, v2.y, v2.z, 0.0f, 
    v3.x, v3.y, v3.z, 0.0f, 
    0.0f, 0.0f, 0.0f, 1.0f 
    );*/
  return ret;
}

D3DXVECTOR3 MultiplyMatrix ( const D3DXMATRIX& aM, const D3DXVECTOR3& aV )
{
  D3DXVECTOR3 ret;
  D3DXVec3TransformCoord( &ret, &aV, &aM );
  return ret;
}

float	Distance ( const D3DXVECTOR3 &v1, const D3DXVECTOR3 &v2 )
{
	D3DXVECTOR3 v = v1-v2;
	return D3DXVec3Length( &v );  
}

float signedArea3d( D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c)
{
  float min0 =  a.x * ( b.y*c.z - c.y*b.z);
  float min1 = -a.y * ( b.x*c.z - c.x*b.z);
  float min2 =  a.z * ( b.x*c.y - c.x*b.y);
	return min0 + min1 + min2;
}

};  // End namespace GeometryMiscFunctions

