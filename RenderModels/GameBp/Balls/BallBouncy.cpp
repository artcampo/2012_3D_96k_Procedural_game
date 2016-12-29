#include    "RenderModels/GameBp/Balls/BallBouncy.hpp"

BallBouncy::BallBouncy( ID3D10ShaderResourceView* aPermTextureSRV  )
  : BallModel(indexShadDefGeomV_PNT_BallBouncy, 4, aPermTextureSRV)
{  
}

BallBouncy::~BallBouncy()
{
  
}

void BallBouncy::_setShader()
{
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallBouncy].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallBouncy].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallBouncy].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallBouncy].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallBouncy].mVertexLayout );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  
  ID3D10EffectPass* pass;  
  pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallBouncy].mTech->GetPassByIndex(0);
  pass->Apply(0);
}
