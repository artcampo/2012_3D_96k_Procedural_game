#ifndef			_BallSpartacus_HPP
#define			_BallSpartacus_HPP


#include  "dx_misc.hpp"
#include  "RenderModels/Basics/SphereModel_PN.hpp"
#include  "RenderModels/GameBp/BallModel.hpp"
#include	"Shader/ShaderManager.hpp"

class BallSpartacus : public BallModel{

public:
  BallSpartacus( ID3D10ShaderResourceView* aPermTextureSRV  );
  ~BallSpartacus();  
  
private:
  virtual void _setShader ();

private:
  SphereModel_PN*    mSphere;  

  ShaderManager	mShaderManager;


};



#endif  //BallSpartacus
