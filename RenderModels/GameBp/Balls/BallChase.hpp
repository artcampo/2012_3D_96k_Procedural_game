#ifndef			_BallChase_HPP
#define			_BallChase_HPP


#include  "dx_misc.hpp"
#include  "RenderModels/Basics/SphereModel_PN.hpp"
#include  "RenderModels/GameBp/BallModel.hpp"
#include	"Shader/ShaderManager.hpp"

class BallChase : public BallModel{

public:
  BallChase( ID3D10ShaderResourceView* aPermTextureSRV  );
  ~BallChase();  
  
private:
  virtual void _setShader ();

private:
  SphereModel_PN*    mSphere;  

  ShaderManager	mShaderManager;


};



#endif  //BallChase
