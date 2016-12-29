
BlendState SrcAlphaBlendingAdd
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};



DepthStencilState StencilState_NoDepth
{
    DepthEnable = false;
    DepthWriteMask = ZERO;
    DepthFunc = Less;


};

cbuffer cbPerFrame
{
	float4x4  gViewProj; 
};
 

struct VS_IN
{
	float3 centerW : POSITION;

};

struct VS_OUT
{
	float4 center :  SV_POSITION;

};

 
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	

	vOut.center  = mul(float4(vIn.centerW, 1.0f ), gViewProj);


	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{

    return float4 ( 1.0, 0.0, 0.0, 1.0 );
}
 
technique10 ParticleTest
{
    pass P0
    {

        SetDepthStencilState( StencilState_NoDepth, 0 );
        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(    ps_4_0, PS() ) );

    }
}
