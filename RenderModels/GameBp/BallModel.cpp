#include		"RenderModels/GameBp/BallModel.hpp"


BallModel::BallModel( const int aShaderId, const int aModelTesellation, ID3D10ShaderResourceView* aPermTextureSRV ) :
  mShaderId (aShaderId)  
{
  mSphere  = new SphereModel_PN( aModelTesellation );
  mPermTextureSRV = aPermTextureSRV;
}

BallModel::~BallModel()
{
  delete mSphere;
}

void BallModel::display()
{
  //_setShader();
  commonDisplay();
  mSphere->display();
}
    
void BallModel::commonDisplay()
{
  mShaderManager.mShaderTable[mShaderId].SetMatrix( &(std::string("World")),      &Global::mWorld );
  mShaderManager.mShaderTable[mShaderId].SetMatrix( &(std::string("View")),       &Global::mView );
  mShaderManager.mShaderTable[mShaderId].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  mShaderManager.mShaderTable[mShaderId].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[mShaderId].mVertexLayout );  
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  ID3D10EffectPass* pass = mShaderManager.mShaderTable[mShaderId].mTech->GetPassByIndex(0);
  pass->Apply(0);

}