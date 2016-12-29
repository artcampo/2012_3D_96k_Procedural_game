#include    "RenderModels/GameBp/Balls/BallMeteor.hpp"

BallMeteor::BallMeteor( ID3D10ShaderResourceView* aPermTextureSRV  )
  : BallModel(indexShadDefGeomV_PNT_BallMeteor, 4, aPermTextureSRV)
{  
}

BallMeteor::~BallMeteor()
{
  
}

void BallMeteor::_setShader()
{
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMeteor].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMeteor].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMeteor].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMeteor].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMeteor].mVertexLayout );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  
  ID3D10EffectPass* pass;  
  pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMeteor].mTech->GetPassByIndex(0);
  pass->Apply(0);
}
