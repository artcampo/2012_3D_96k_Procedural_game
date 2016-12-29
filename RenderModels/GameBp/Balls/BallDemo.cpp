#include    "RenderModels/GameBp/Balls/BallDemo.hpp"

BallDemo::BallDemo( ID3D10ShaderResourceView* aPermTextureSRV )
  : BallModel(indexShadDefGeomV_PNT_BallDemo, 1, aPermTextureSRV)
{  
}

BallDemo::~BallDemo()
{
  
}

void BallDemo::_setShader()
{
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallDemo].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallDemo].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallDemo].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallDemo].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallDemo].mVertexLayout );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  
  ID3D10EffectPass* pass;  
  pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallDemo].mTech->GetPassByIndex(0);
  pass->Apply(0);
}
