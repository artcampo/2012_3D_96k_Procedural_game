#include "simplex.fx"
#include "shader_defines.fx"

matrix World;
matrix View;
matrix Projection;
  

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 col : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos		: SV_POSITION;
    float4 Nor		: NORMAL;
    float4 Tan		: TANGENT;
    float4 PosO		: POSITION;
    float depth   : FOG;
    float4 col : COLOR;
};


struct PS_OUTPUT
{
	float4 pos : SV_Target0;
	float4 col : SV_Target1;
	float4 nor : SV_Target2;
	float4 tan : SV_Target3;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT output;

	  float4x4 gWVP;
	  gWVP = mul(World,View);
	  gWVP = mul(gWVP,Projection);
	  float4x4 gWV;
	  gWV = mul(World,View);

    output.Pos 	= mul ( float4(input.pos,1.0f) , gWVP);

	  output.PosO	= mul ( float4(input.pos,1.0f) , World);
	  output.Nor	= mul ( float4(0,1,0,0) , World);
	  output.Tan	= mul ( float4(1,0,0,0) , World);

    output.col = input.col;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

PS_OUTPUT PS( VS_OUTPUT input ) : SV_Target
{
	PS_OUTPUT output = (PS_OUTPUT)0;


	///////////////////////////////////////////////////
	//  WRITE OUT
	output.nor = float4(input.Nor.xyz,0);
	output.tan = input.Tan;
  output.pos = input.PosO;
	output.col = input.col;	

    return output;
}

//--------------------------------------------------------------------------------------
technique10 test
{

    pass P0
    {
//        SetRasterizerState(rs_Front);
      //  SetDepthStencilState(EnableDepthLess, 0);
      //  SetBlendState(NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
       // SetRasterizerState(rs_Back);
    }
}
