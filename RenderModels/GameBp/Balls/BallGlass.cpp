#include    "RenderModels/GameBp/Balls/BallGlass.hpp"

BallGlass::BallGlass( ID3D10ShaderResourceView* aPermTextureSRV  )
  : BallModel(indexShadDefGeomV_PNT_BallMeteor, 4, aPermTextureSRV)
{  
}

BallGlass::~BallGlass()
{
  
}

void BallGlass::_setShader()
{
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallGlass].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallGlass].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallGlass].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallGlass].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallGlass].mVertexLayout );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  
  ID3D10EffectPass* pass;  
  pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallGlass].mTech->GetPassByIndex(0);
  pass->Apply(0);
}
