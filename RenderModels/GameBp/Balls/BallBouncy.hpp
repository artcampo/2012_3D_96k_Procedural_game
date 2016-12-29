#ifndef			_BallBouncy_HPP
#define			_BallBouncy_HPP


#include  "dx_misc.hpp"
#include  "RenderModels/Basics/SphereModel_PN.hpp"
#include  "RenderModels/GameBp/BallModel.hpp"
#include	"Shader/ShaderManager.hpp"

class BallBouncy : public BallModel{

public:
  BallBouncy( ID3D10ShaderResourceView* aPermTextureSRV  );
  ~BallBouncy();  
  
private:
  virtual void _setShader ();

private:
  SphereModel_PN*    mSphere;  

  ShaderManager	mShaderManager;


};



#endif  //BallBouncy
