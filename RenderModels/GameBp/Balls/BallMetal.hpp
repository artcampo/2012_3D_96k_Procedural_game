#ifndef			_BallMetal_HPP
#define			_BallMetal_HPP


#include  "dx_misc.hpp"
#include  "RenderModels/Basics/SphereModel_PN.hpp"
#include  "RenderModels/GameBp/BallModel.hpp"
#include	"Shader/ShaderManager.hpp"

class BallMetal : public BallModel{

public:
  BallMetal( ID3D10ShaderResourceView* aPermTextureSRV  );
  ~BallMetal();  
  
private:
  virtual void _setShader ();

private:
  SphereModel_PN*    mSphere;  

  ShaderManager	mShaderManager;


};



#endif  //BallMetal
