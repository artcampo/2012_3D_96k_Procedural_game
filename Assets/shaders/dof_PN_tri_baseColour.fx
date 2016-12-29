#include "shader_defines.fx"

matrix World;
matrix View;
matrix Projection;


struct VS_INPUT
{
    float3 pos : POSITION;
    float3 nor : NORMAL;
};

struct VS_OUTPUT
{
    float4 pos	  : SV_POSITION;
    float3 nor    : NORMAL;
    float depth   : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 col  : SV_Target0;
	float4 ZPos : SV_Target1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT output;
    //Object to world

	  float4x4 gWVP = mul(World,View);
	  gWVP = mul(gWVP,Projection);
	  float4x4 gWV = mul(World,View);

    output.pos 	  = mul ( float4(input.pos,1.0f) , gWVP);
	  output.nor    = input.nor;
	  output.depth  = (mul ( float4(input.pos,1.0f) , gWV)).z;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

PS_OUTPUT PS( VS_OUTPUT input ) : SV_Target
{
	PS_OUTPUT output = (PS_OUTPUT)0;


	output.col = float4 ( 1, 0, 0, 1 );
  output.ZPos = input.depth;

  return output;
}

//--------------------------------------------------------------------------------------
technique10 test
{

    pass P0
    {
        SetDepthStencilState(EnableDepthLess, 0);
        SetBlendState(NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
