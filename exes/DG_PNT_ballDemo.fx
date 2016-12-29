#include "simplex.fx"
#include "shader_defines.fx"

matrix World;
matrix View;
matrix Projection;
  

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 nor : NORMAL;
    float3 tan : TANGENT;
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
	  output.Nor	= mul ( float4(input.nor,0.0f) , World);
	  output.Tan	= mul ( float4(input.tan,0.0f) , World);

    output.col = float4(1,1,1,1);

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

PS_OUTPUT PS( VS_OUTPUT input ) : SV_Target
{
	PS_OUTPUT output = (PS_OUTPUT)0;
  float4 finalPixel = float4(0,0,0,1);

  float3  noiseParams;
  float   noiseT;
  float   value;  float3  noiseIn;
  float3  vectorModulus;
  float3  vectorValue;
  float   tFade;
  float3  inPerlin;
  float   divPerlin;
  float3  spherical;
  float3  ballScale = float3(2500.000f, 2500.000f, 2500.000f);

  spherical.z = length(input.PosO);
  spherical.x = acos(input.PosO.z / spherical.z);
  spherical.y = atan(input.PosO.y / input.PosO.x);


  noiseIn = input.PosO.xyz*ballScale;
  noiseT = 0;
  vectorValue = noiseIn * float3(2.42718f, 2.42718f, 2.42718f);
  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + 3.42233f + Snoise3D( 1.00000f *noiseIn )));
  if ( value < 0.30000f)
    noiseT += 1 - ( (value/0.30000f)*0.5 + 0.5 );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal00 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.20392f, 0.20392f, 0.20392f, 1.00000f), noiseT );

  noiseIn = input.PosO.xyz*ballScale;
  noiseT = Snoise3D ( noiseIn);
  float4 Signal22 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.20784f, 0.20784f, 0.23922f, 1.00000f), noiseT );

  noiseIn = input.PosO.xyz*ballScale;
  value = Snoise3D ( noiseIn);
  noiseT =  sqrt(abs(cos( 5*noiseIn.x + 2*value ) ));
  noiseT +=  sqrt(abs(cos( 5*noiseIn.y + 2*value ) ));
  noiseT *= 0.5f;
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal11 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.27843f, 0.27843f, 0.27843f, 1.00000f), noiseT );


//RGBA CHANNEL
  float4 Layer00 = Signal00;
  float4 Layer33 = Signal22;
  float4 Layer22 = 0.5*Signal00 + 0.5*Signal22;

//BUMP CHANNEL
  float4 Layer11 = Signal11;

//FINAL PIXEL


  float4 diffuseColour = Layer22;
  float3 vec[4] = {
      float3(0,0,0),  
      float3(0.01,0,0),
      float3(0,0.01,0),
      float3(0,0,0.01)    
      };
  float res[4];      

  for ( int i = 0; i < 4 ; ++i)
    {
    float3 inputParameters = input.Pos + vec[i];

  noiseIn = inputParameters.xyz*ballScale;
  noiseT = 0;
  vectorValue = noiseIn * float3(2.42718f, 2.42718f, 2.42718f);
  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + 3.42233f + Snoise3D( 1.00000f *noiseIn )));
  if ( value < 0.30000f)
    noiseT += 1 - ( (value/0.30000f)*0.5 + 0.5 );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal00 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.20392f, 0.20392f, 0.20392f, 1.00000f), noiseT );

  noiseIn = inputParameters.xyz*ballScale;
  noiseT = Snoise3D ( noiseIn);
  float4 Signal22 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.20784f, 0.20784f, 0.23922f, 1.00000f), noiseT );

  noiseIn = inputParameters.xyz*ballScale;
  value = Snoise3D ( noiseIn);
  noiseT =  sqrt(abs(cos( 5*noiseIn.x + 2*value ) ));
  noiseT +=  sqrt(abs(cos( 5*noiseIn.y + 2*value ) ));
  noiseT *= 0.5f;
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal11 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.27843f, 0.27843f, 0.27843f, 1.00000f), noiseT );

  float4 Layer11 = Signal11;

 res[i] = Layer11;
  }
  float3 dF = float3( res[1] - res[0], res[2] - res[0], res[3] - res[0] );
  dF /= 0.01;
  float3 disturbedNormal = input.Nor.xyz + dF;
  disturbedNormal = normalize(disturbedNormal);

	

	///////////////////////////////////////////////////
	//  WRITE OUT
	output.nor = float4(input.Nor.xyz,0);
//	output.nor = float4(disturbedNormal,0);
	output.tan = input.Tan;
  output.pos = input.PosO;
	output.col = float4( diffuseColour.xyz, 1);

	

    return output;
}

//--------------------------------------------------------------------------------------
technique10 DeferredRender
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
