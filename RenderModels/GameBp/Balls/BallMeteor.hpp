#ifndef			_BallMeteor_HPP
#define			_BallMeteor_HPP


#include  "dx_misc.hpp"
#include  "RenderModels/Basics/SphereModel_PN.hpp"
#include  "RenderModels/GameBp/BallModel.hpp"
#include	"Shader/ShaderManager.hpp"

class BallMeteor : public BallModel{

public:
  BallMeteor( ID3D10ShaderResourceView* aPermTextureSRV  );
  ~BallMeteor();  
  
private:
  virtual void _setShader ();

private:
  SphereModel_PN*    mSphere;  

  ShaderManager	mShaderManager;


};



#endif  //BallMeteor
