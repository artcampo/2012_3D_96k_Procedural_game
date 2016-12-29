#include "simplex.fx"
#include "lighting.fx"

matrix World;
matrix View;
matrix Projection;

  float3    camPos;

DepthStencilState EnableDepthLess
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
	  DepthFunc = LESS_EQUAL;
};

DepthStencilState DisableDepthLess
{
    DepthEnable = FALSE;
    DepthWriteMask = ALL;
	  DepthFunc = LESS;
};

BlendState NoBlending
{
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0F;
};

RasterizerState NoCull
{
    CULLMODE = 1;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 nor : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos		: SV_POSITION;
    float3 nor    : NORMAL;
    float  Depth  : TEXCOORD1;
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

	  float4x4 gWVP;
	  gWVP = mul(World,View);
	  gWVP = mul(gWVP,Projection);
	  float4x4 gWV;
	  gWV = mul(World,View);

    output.Pos 	  = mul ( float4(input.pos,1.0f) , gWVP);
    //output.nor    = mul ( float4(input.nor,0.0f) , gWV ).xyz;
output.nor    =input.nor;
    output.Depth  = (mul ( float4(input.pos,1.0f) , gWV)).z;
    return output;
}



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

PS_OUTPUT PS( VS_OUTPUT input ) : SV_Target
{
	PS_OUTPUT output = (PS_OUTPUT)0;



	  float4x4 gWVP;
	  gWVP = mul(World,View);
	  gWVP = mul(gWVP,Projection);
	  float4x4 gWV;
	  gWV = mul(World,View);

  float3 lPos = float3 ( 100, 200, 300);
  lPos =mul ( float4(lPos,1.0f) , gWVP);
  output.col  = float4(   lightingP( input.Pos, float3 ( 1.0f, 1.0f, 1.0f ), input.nor, lPos, camPos )   , 1.0f) ;

lPos = float3 ( 100, 500, 300);
  lPos =mul ( float4(lPos,1.0f) , gWVP);
  output.col  += float4(   lightingP( input.Pos, float3 ( 0.0f, 0.0f, 1.0f ), input.nor, lPos, camPos )   , 1.0f) ;

  output.col  += float4( 0.1f, 0.1f, 0.1f, 1.0f);
  output.col.a = 1.0f;
//  output.col = float4 ( 1, 0, 0,1);
  output.ZPos = input.Depth;
  return output;
}

//--------------------------------------------------------------------------------------
technique10 test
{

    pass P0
    {
        SetDepthStencilState(EnableDepthLess, 0);
        //SetBlendState(NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
        //SetRasterizerState(NoCull);

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
  //      SetDepthStencilState(EnableDepthLess, 0);
    }
}
