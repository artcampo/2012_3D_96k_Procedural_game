#include    "RenderModels/GameBp/Balls/BallMetal.hpp"

BallMetal::BallMetal( ID3D10ShaderResourceView* aPermTextureSRV  )
  : BallModel(indexShadDefGeomV_PNT_BallMetal, 4, aPermTextureSRV)
{  
}

BallMetal::~BallMetal()
{
  
}

void BallMetal::_setShader()
{
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMetal].SetMatrix( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMetal].SetMatrix( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMetal].SetMatrix( &(std::string("Projection")), &Global::mProjection );
  mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMetal].SetResource( &(std::string("PermTexture")), mPermTextureSRV );

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMetal].mVertexLayout );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  
  ID3D10EffectPass* pass;  
  pass = mShaderManager.mShaderTable[indexShadDefGeomV_PNT_BallMetal].mTech->GetPassByIndex(0);
  pass->Apply(0);
}
