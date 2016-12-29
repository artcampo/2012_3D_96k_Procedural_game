#include    "RenderModels/GameBp/Balls/BallSpartacus.hpp"

BallSpartacus::BallSpartacus( ID3D10ShaderResourceView* aPermTextureSRV  )
  : BallModel(indexShadDefGeomV_PNT_BallSpartacus, 4, aPermTextureSRV)
{  
}

BallSpartacus::~BallSpartacus()
{
  
}

void BallSpartacus::_setShader()
{
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallSpartacus].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallSpartacus].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallSpartacus].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallSpartacus].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallSpartacus].mVertexLayout );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  
  ID3D10EffectPass* pass;  
  pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallSpartacus].mTech->GetPassByIndex(0);
  pass->Apply(0);
}
