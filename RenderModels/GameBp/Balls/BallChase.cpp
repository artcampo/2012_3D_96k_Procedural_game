#include    "RenderModels/GameBp/Balls/BallChase.hpp"

BallChase::BallChase( ID3D10ShaderResourceView* aPermTextureSRV  )
  : BallModel(indexShadDefGeomV_PNT_BallChase, 4, aPermTextureSRV)
{  
}

BallChase::~BallChase()
{
  
}

void BallChase::_setShader()
{
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallChase].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallChase].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallChase].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallChase].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallChase].mVertexLayout );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  
  ID3D10EffectPass* pass;  
  pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallChase].mTech->GetPassByIndex(0);
  pass->Apply(0);
}
