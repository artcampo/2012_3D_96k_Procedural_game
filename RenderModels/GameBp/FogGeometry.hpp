#ifndef			_FogGeometry_HPP
#define			_FogGeometry_HPP


#include  "dx_misc.hpp"
#include  "RenderModels/Basics/SphereModel_PN.hpp"
#include	"Shader/ShaderManager.hpp"
#include  "RenderModels\Basics\RectangleModel.hpp"

class FogGeometry {

public:
  FogGeometry(  const float aPlaneHeight,
                const float aPlaneLengthU,
                const float aPlaneLengthV,
                ID3D10ShaderResourceView* aPermTextureSRV );
  ~FogGeometry();  
  void display();

protected:
  void    commonDisplay();

protected:
  ID3D10ShaderResourceView*           mPermTextureSRV;

private:
  
  ShaderManager	      mShaderManager;
  
  D3DXCOLOR           mColor;
  RectangleModel*     mFogBottom;



};



#endif  //FogGeometry
