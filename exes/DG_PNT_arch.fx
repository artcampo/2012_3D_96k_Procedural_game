//  Procedural Texture Shader v0.001
//  Deferred render
//  Not Gordian

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
    float4 pos   : SV_POSITION;
    float4 nor   : NORMAL;
    float4 tan   : TANGENT;
    float4 posLS : POSITION1;
    float4 posWS : POSITION2;
};


struct PS_OUTPUT
{
    float4 pos : SV_Target0;
    float4 col : SV_Target1;
    float4 nor : SV_Target2;
    float4 tan : SV_Target3;
    float4 lig : SV_Target4;
};

VS_OUTPUT VS( VS_INPUT input )
{
  VS_OUTPUT output;
	float4x4 gWVP;
  gWVP            = mul(World,View);
  gWVP            = mul(gWVP,Projection);
  output.pos      = mul( float4(input.pos,1.0f) , gWVP);
  output.nor      = mul( float4(input.nor,0.0f), World);
  output.nor.xyz  = normalize(output.nor.xyz);
  output.tan      = mul( float4(input.tan,0.0f), World);
  output.posLS    = float4(input.pos,1.0f);
  output.posWS    = mul( float4(input.pos,1.0f), World);
  return output;
}

// Assuming one point of the line is (0,0,0)
float distLine3d0( float3 p, float3 l)
{
  float n  = length(cross(l,-p));
  float d  = length(l);
  return n/d;
}

float procLine( float3 val, float3 m, float aBaseDist )
{
  float d = distLine3d0( val, m );
  d /= aBaseDist;
  d *= 30;
  d = saturate(d);
  return d;
}

PS_OUTPUT PS( VS_OUTPUT input ) : SV_Target
{
  PS_OUTPUT output = (PS_OUTPUT)0;
  float4 finalPixel = float4(0,0,0,1);
  float3 objectScale = float3(5,5,5);

  float3  noiseParams;
  float   noiseT;
  float   value;
  float   value2;
  float3  noiseIn;
  float3  vectorModulus;
  float3  vectorValue;
  float   tFade;
  float3  inPerlin;
  float   divPerlin;
  float3  spherical;

  spherical.z = length(input.posLS);
  spherical.x = acos(input.posLS.z / spherical.z);
  spherical.y = atan(input.posLS.y / input.posLS.x);


  noiseIn = objectScale*input.posLS.xyz*float3(2.60000f, 2.60000f, 2.60000f);
  noiseT = Snoise3D ( noiseIn);
  float4 Signal11 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = objectScale*input.posLS.xyz*float3(0.40000f, 0.40000f, 0.40000f);
  noiseT = 0;
    inPerlin = noiseIn;
  divPerlin = 1;
  for( int i = 0; i < 4; ++i)
  {
    float r = Snoise3D( inPerlin );
    r /= divPerlin;
    value += r;
    inPerlin *= 2;
    divPerlin *= 2;
  }
  noiseT = abs( noiseIn.x + value );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal00 = lerp( float4(0.34510f, 0.20000f, 0.07059f, 1.00000f), float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), noiseT );

  noiseIn = objectScale*input.posLS.xyz*float3(2.20000f, 2.20000f, 2.20000f);
  value = Snoise3D ( noiseIn);
noiseT =  sqrt(abs(cos( 3*noiseIn.x + 2*value ) ) );   noiseT = clamp( noiseT, 0, 1 );
  float4 Signal33 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.21176f, 0.11765f, 0.11765f, 1.00000f), noiseT );

  noiseIn = objectScale*input.posLS.xyz*float3(0.80000f, 0.80000f, 0.80000f);
  noiseT = Snoise3D ( noiseIn);
  float4 Signal44 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(0.67451f, 0.65098f, 0.58431f, 1.00000f), noiseT );

  noiseIn = objectScale*input.posLS.xyz*float3(1.20000f, 1.20000f, 1.20000f);
  noiseT = 0;
  vectorValue = noiseIn * float3(0.48000f, 0.48000f, 0.48000f);
  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + 1.08000f + Snoise3D( 1.00000f *noiseIn )));
  if ( value < 0.29000f)
    noiseT += 1 - ( (value/0.29000f)*0.5 + 0.5 );
  value = abs( cos( vectorValue.x + vectorValue.y + vectorValue.z + 1.08000f + Snoise3D( 4.00000f *noiseIn )));
  if ( value < 0.29000f)
    noiseT += 1 - ( (value/0.29000f)*0.5 + 0.5 );
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal55 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );

  noiseIn = objectScale*input.posLS.xyz*float3(1.20000f, 1.20000f, 1.20000f) + float3(0.61000f, 0.61000f, 0.61000f);
;
  vectorModulus = float3(0.74074f, 0.74074f, 0.74074f);
  noiseT = 0;
;
  vectorValue = fmod( abs(noiseIn), vectorModulus );
  vectorValue -= 0.5*float3(0.74074f, 0.74074f, 0.74074f);
  vectorValue = abs(vectorValue);
  vectorValue = max( vectorValue, float3(0.00370f, 0.00370f, 0.00370f));
  vectorValue = ( float3(0.00370f, 0.00370f, 0.00370f) / vectorValue);
  noiseT += vectorValue.x + vectorValue.y + vectorValue.z;

  noiseT = saturate( noiseT );
  float4 Signal22 = lerp( float4(0.76471f, 0.76471f, 0.76471f, 1.00000f), float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), noiseT );


//RGBA CHANNEL
  float4 Layer00 = Signal00;
  float4 Layer33 = Signal22;
  float4 Layer55 = Signal33;
  float4 Layer77 = Signal44;
  float4 Layer99 = Signal55;
  float4 Layer88;
  if ( Signal44.x < 0.5 )
  {
    Layer88.x = 2*Signal44.x*Signal55.x;
  }
  else
  {
    Layer88.x = 1- 2*(1 - Signal44.x)*(1 - Signal55.x);
  }
;
  if ( Signal44.y < 0.5 )
  {
    Layer88.y = 2*Signal44.y*Signal55.y;
  }
  else
  {
    Layer88.y = 1- 2*(1 - Signal44.y)*(1 - Signal55.y);
  }
;
  if ( Signal44.z < 0.5 )
  {
    Layer88.z = 2*Signal44.z*Signal55.z;
  }
  else
  {
    Layer88.z = 1- 2*(1 - Signal44.z)*(1 - Signal55.z);
  }
;
  float4 Layer66 = 0.5*Signal33 + 0.5*Layer88;
  float4 Layer44;
  if ( Signal22.x < 0.5 )
  {
    float tFade = Signal22 / 0.5;
    tFade = (tFade - 0.8) / (1 - 0.8);
    Layer44 = lerp( float4(0,0,0,1), Layer66, tFade);
  }
  else
    Layer44 = Layer66;
  float4 Layer22 = 0.5*Signal00 + 0.5*Layer44;

//BUMP CHANNEL
  float4 Layer11 = Signal11;

//FINAL PIXEL


  float4 diffuseColour = Layer22;
  output.nor = input.nor;

  output.tan = input.tan;
  output.pos = input.posWS;
  output.col = float4( diffuseColour.xyz, 1);
  output.lig = float4(1.00000f, 1.00000f, 1.00000f, 1.00000f);
  return output;
}

technique10 DeferredRender
{
    pass P0
    {
        SetDepthStencilState(EnableDepthLess, 0);
        SetBlendState(NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
        SetVertexShader( CompileShader(   vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(    ps_4_0, PS() ) );
    }
}

