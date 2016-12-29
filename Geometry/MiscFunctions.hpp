#ifndef			_MISC_FUNCTIONS_HPP
#define			_MISC_FUNCTIONS_HPP

#include    "dx_misc.hpp"

namespace GeometryMiscFunctions
{
  float signedArea3d( D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c);

  D3DXVECTOR3	NormalOfTriangle  ( const D3DXVECTOR3  *v0, const D3DXVECTOR3  *v1, const D3DXVECTOR3  *v2 );
  D3DXVECTOR3 perpendicular3D(const D3DXVECTOR3& th);

  D3DXMATRIX basisMatrix ( const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3 );

  D3DXVECTOR3 MultiplyMatrix ( const D3DXMATRIX& aM, const D3DXVECTOR3& aV );

  float	Distance ( const D3DXVECTOR3 &v1, const D3DXVECTOR3 &v2 );
};  // end namespace GeometryMiscFunctions

#endif