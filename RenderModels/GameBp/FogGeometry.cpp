#include		"RenderModels/GameBp/FogGeometry.hpp"


FogGeometry::FogGeometry( const float aPlaneHeight, const float aPlaneLengthU,
                const float aPlaneLengthV,
                ID3D10ShaderResourceView* aPermTextureSRV ) :  
  mColor( 0, 0, 0, 1) 
{  
  mPermTextureSRV = aPermTextureSRV;
  mFogBottom = new RectangleModel( D3DXVECTOR3( 0, aPlaneHeight - 10, 0 ), aPlaneLengthU, aPlaneLengthV );
}

FogGeometry::~FogGeometry()
{
  delete mFogBottom;
}

void FogGeometry::display()
{
  ID3D10EffectPass* pass;
  
  mShaderManager.mShaderTable[indexShadDefGeomV_P].SetMatrix  ( &(std::string("World")), &Global::mWorld );
  mShaderManager.mShaderTable[indexShadDefGeomV_P].SetMatrix  ( &(std::string("View")), &Global::mView );
  mShaderManager.mShaderTable[indexShadDefGeomV_P].SetMatrix  ( &(std::string("Projection")), &Global::mProjection );  
  mShaderManager.mShaderTable[indexShadDefGeomV_P].SetRawValue( &(std::string("uniColor")), (float *)(&mColor), 3 );

  pass = mShaderManager.mShaderTable[indexShadDefGeomV_P].mTech->GetPassByIndex(0);   
  pass->Apply(0);

  Global::md3dDevice->IASetInputLayout( mShaderManager.mShaderTable[indexShadDefGeomV_P].mVertexLayout );
  Global::md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    
  mFogBottom->display();
}
