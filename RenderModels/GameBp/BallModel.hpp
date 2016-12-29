#ifndef			_BallModel_HPP
#define			_BallModel_HPP


#include  "dx_misc.hpp"
#include  "RenderModels/Basics/SphereModel_PN.hpp"
#include	"Shader/ShaderManager.hpp"

class BallModel {

public:
  BallModel( const int aShaderId, const int aModelTesellation, ID3D10ShaderResourceView* aPermTextureSRV );
  ~BallModel();  
  void display();

private:
  virtual void _setShader () = 0;

private:
  SphereModel_PN*    mSphere;  
protected:
  ID3D10ShaderResourceView*           mPermTextureSRV;
private:
  int     mShaderId;
  ShaderManager	mShaderManager;
protected:
  void    commonDisplay();


};



#endif  //BallModel
