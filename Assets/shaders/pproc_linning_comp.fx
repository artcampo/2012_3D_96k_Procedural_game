
Texture2D textCol;
Texture2D textSeed;

float4x4 ModelView : VIEW;
float4x4 ModelViewProj : WORLDVIEWPROJECTION;


SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_Input
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct VS_Output
{
    float4 Pos : SV_POSITION;              
    float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_Output VS_FINAL_COMPOSITION( VS_Input Input )
{
    VS_Output Output;
    Output.Pos = Input.Pos;
    Output.Tex = Input.Tex;

    return Output;
}



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------



float4 PS_FINAL_COMPOSITION( VS_Output Input ) : SV_TARGET
{    

    float2 vSamplePosition;
    
    vSamplePosition = Input.Tex;



  float4 vColor;
    vColor.xyz = textCol.Sample( samLinear, vSamplePosition).xyz;    



	vColor.a = 1.0;
	return vColor;



}

//--------------------------------------------------------------------------------------
technique10 PPROC_LINING
{

    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_FINAL_COMPOSITION() ) );
    }
}
