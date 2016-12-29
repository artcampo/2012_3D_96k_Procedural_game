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
    float2 uv  : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Pos		: SV_POSITION;
    float4 Nor		: NORMAL;
    float4 Tan		: TANGENT;
    float2 uv     : TEXCOORD;    
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
    output.uv  = input.uv;

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

  float3 inputLS = float3(input.uv,0);
  spherical.z = length(inputLS);
  spherical.x = acos(inputLS.z / spherical.z);
  spherical.y = atan(inputLS.y / inputLS.x);

  float3 trackScale = float3(1000,10,100);

  noiseIn = inputLS*trackScale + float3(0.00000f, 0.00000f, 0.00000f);
;
  vectorModulus = float3(2.00000f, 2.00000f, 2.00000f);
  noiseT = 0;
;
  vectorValue = fmod( abs(noiseIn), vectorModulus );
  vectorValue -= 0.5*float3(2.00000f, 2.00000f, 2.00000f);
  vectorValue = abs(vectorValue);
  vectorValue = max( vectorValue, float3(0.01000f, 0.01000f, 0.01000f));
  vectorValue = ( float3(0.01000f, 0.01000f, 0.01000f) / vectorValue);
  noiseT += vectorValue.x + vectorValue.y + vectorValue.z;

  noiseT = saturate( noiseT );
  float4 Signal22 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = inputLS*trackScale+ float3(0.44000f, 0.44000f, 0.44000f);
;
  vectorModulus = float3(0.81301f, 0.81301f, 0.81301f);
  noiseT = 0;
;
  vectorValue = fmod( abs(noiseIn), vectorModulus );
  vectorValue -= 0.5*float3(0.81301f, 0.81301f, 0.81301f);
  vectorValue = abs(vectorValue);
  vectorValue = max( vectorValue, float3(0.00407f, 0.00407f, 0.00407f));
  vectorValue = ( float3(0.00407f, 0.00407f, 0.00407f) / vectorValue);
  noiseT += vectorValue.x + vectorValue.y + vectorValue.z;

  noiseT = saturate( noiseT );
  float4 Signal11 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = inputLS*trackScale;
  noiseT = 0;
  vectorValue = noiseIn * float3(1.20000f, 1.20000f, 1.20000f);
  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + 0.00000f + Snoise3D( 1.00000f *noiseIn )));
  if ( value < 0.30000f)
    noiseT += 1 - ( (value/0.30000f)*0.5 + 0.5 );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal33 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), noiseT );

  noiseIn = inputLS*trackScale;
  noiseT = 0;
  vectorValue = noiseIn * float3(4.80000f, 4.80000f, 4.80000f);
  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + 2.44800f + Snoise3D( 1.00000f *noiseIn )));
  if ( value < 0.20000f)
    noiseT += 1 - ( (value/0.20000f)*0.5 + 0.5 );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal00 = lerp( float4(0.12941f, 0.13725f, 0.14902f, 1.00000f), float4(0.35294f, 0.41176f, 0.60392f, 1.00000f), noiseT );

  noiseIn = inputLS*trackScale;
  noiseT = 0;
  value = Snoise3D ( noiseIn);
  noiseT = abs( noiseIn.x + value );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal44 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = inputLS*trackScale;
  value = Snoise3D ( noiseIn);
noiseT =  sqrt(abs(cos( 3*noiseIn.x + 2*value ) ) );   noiseT = clamp( noiseT, 0, 1 );
  float4 Signal55 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.81569f, 0.60392f, 0.06275f, 1.00000f), noiseT );


//RGBA CHANNEL
  float4 Layer00 = Signal00;
  float4 Layer77 = Signal44;
  float4 Layer99 = Signal55;
  float4 Layer88 = 0.5*Signal44 + 0.5*Signal55;
  float4 Layer66 = 0.5*Signal00 + 0.5*Layer88;

//BUMP CHANNEL
  float4 Layer11 = Signal11;

//FINAL PIXEL



  float4 diffuseColour = Layer66;
  float3 vec[4] = {
      float3(0,0,0),  
      float3(0.01,0,0),
      float3(0,0.01,0),
      float3(0,0,0.01)    
      };
  float res[4];      

  for ( int i = 0; i < 4 ; ++i)
    {
    float3 inputParameters = inputLS + vec[i];

  noiseIn = inputParameters.xyz*trackScale + float3(0.00000f, 0.00000f, 0.00000f);
;
  vectorModulus = float3(2.00000f, 2.00000f, 2.00000f);
  noiseT = 0;
;
  vectorValue = fmod( abs(noiseIn), vectorModulus );
  vectorValue -= 0.5*float3(2.00000f, 2.00000f, 2.00000f);
  vectorValue = abs(vectorValue);
  vectorValue = max( vectorValue, float3(0.01000f, 0.01000f, 0.01000f));
  vectorValue = ( float3(0.01000f, 0.01000f, 0.01000f) / vectorValue);
  noiseT += vectorValue.x + vectorValue.y + vectorValue.z;

  noiseT = saturate( noiseT );
  float4 Signal22 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = inputParameters.xyz*float3(2.00000f, 2.00000f, 2.00000f) + float3(0.44000f, 0.44000f, 0.44000f);
;
  vectorModulus = float3(0.81301f, 0.81301f, 0.81301f);
  noiseT = 0;
;
  vectorValue = fmod( abs(noiseIn), vectorModulus );
  vectorValue -= 0.5*float3(0.81301f, 0.81301f, 0.81301f);
  vectorValue = abs(vectorValue);
  vectorValue = max( vectorValue, float3(0.00407f, 0.00407f, 0.00407f));
  vectorValue = ( float3(0.00407f, 0.00407f, 0.00407f) / vectorValue);
  noiseT += vectorValue.x + vectorValue.y + vectorValue.z;

  noiseT = saturate( noiseT );
  float4 Signal11 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = inputParameters.xyz*trackScale;
  noiseT = 0;
  vectorValue = noiseIn * float3(1.20000f, 1.20000f, 1.20000f);
  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + 0.00000f + Snoise3D( 1.00000f *noiseIn )));
  if ( value < 0.30000f)
    noiseT += 1 - ( (value/0.30000f)*0.5 + 0.5 );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal33 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), noiseT );

  noiseIn = inputParameters.xyz*trackScale;
  noiseT = 0;
  vectorValue = noiseIn * float3(4.80000f, 4.80000f, 4.80000f);
  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + 2.44800f + Snoise3D( 1.00000f *noiseIn )));
  if ( value < 0.20000f)
    noiseT += 1 - ( (value/0.20000f)*0.5 + 0.5 );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal00 = lerp( float4(0.12941f, 0.13725f, 0.14902f, 1.00000f), float4(0.35294f, 0.41176f, 0.60392f, 1.00000f), noiseT );

  noiseIn = inputParameters.xyz*trackScale;
  noiseT = 0;
  value = Snoise3D ( noiseIn);
  noiseT = abs( noiseIn.x + value );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal44 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = inputParameters.xyz*trackScale;
  value = Snoise3D ( noiseIn);
noiseT =  sqrt(abs(cos( 3*noiseIn.x + 2*value ) ) );   noiseT = clamp( noiseT, 0, 1 );
  float4 Signal55 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.81569f, 0.60392f, 0.06275f, 1.00000f), noiseT );

  float4 Layer11 = Signal11;

 res[i] = Layer11;
  }
  float3 dF = float3( res[1] - res[0], res[2] - res[0], res[3] - res[0] );
  dF /= 0.01;
  float3 disturbedNormal = input.Nor.xyz + dF;

	
	///////////////////////////////////////////////////
	//  WRITE OUT
	output.nor = float4(input.Nor.xyz,0);
	output.nor = float4(disturbedNormal,0);
	output.tan = input.Tan;
  output.pos = input.PosO;
	output.col = float4( diffuseColour.xyz, 1);
//	output.col = float4( input.uv,0,1);
	

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
