#ifndef			_BallDemo_HPP
#define			_BallDemo_HPP


#include  "dx_misc.hpp"
#include  "RenderModels/Basics/SphereModel_PN.hpp"
#include  "RenderModels/GameBp/BallModel.hpp"
#include	"Shader/ShaderManager.hpp"

class BallDemo : public BallModel{

public:
  BallDemo( ID3D10ShaderResourceView* aPermTextureSRV  );
  ~BallDemo();  
  
private:
  virtual void _setShader ();

private:
  SphereModel_PN*    mSphere;  

  ShaderManager	mShaderManager;


};



#endif  //BallDemo
