#ifndef			_BallGlass_HPP
#define			_BallGlass_HPP


#include  "dx_misc.hpp"
#include  "RenderModels/Basics/SphereModel_PN.hpp"
#include  "RenderModels/GameBp/BallModel.hpp"
#include	"Shader/ShaderManager.hpp"

class BallGlass : public BallModel{

public:
  BallGlass( ID3D10ShaderResourceView* aPermTextureSRV  );
  ~BallGlass();  
  
private:
  virtual void _setShader ();

private:
  SphereModel_PN*    mSphere;  

  ShaderManager	mShaderManager;


};



#endif  //BallGlass
