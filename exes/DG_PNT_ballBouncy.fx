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
  float3 objectScale = float3(1,1,1);

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
  noiseT = Snoise3D ( noiseIn);
  float4 Signal00 = lerp( float4(0.29804f, 0.07451f, 0.07451f, 1.00000f), float4(0.69020f, 0.32941f, 0.03529f, 1.00000f), noiseT );

  noiseIn = objectScale*input.posLS.xyz*float3(0.80000f, 0.80000f, 0.80000f) + float3(0.81000f, 0.81000f, 0.81000f);
;
  vectorModulus = float3(1.17647f, 1.17647f, 1.17647f);
  noiseT = 0;
;
  vectorValue = fmod( abs(noiseIn), vectorModulus );
  vectorValue -= 0.5*float3(1.17647f, 1.17647f, 1.17647f);
  vectorValue = abs(vectorValue);
  vectorValue = max( vectorValue, float3(0.00588f, 0.00588f, 0.00588f));
  vectorValue = ( float3(0.00588f, 0.00588f, 0.00588f) / vectorValue);
  noiseT += vectorValue.x + vectorValue.y + vectorValue.z;

  noiseT = saturate( noiseT );
  float4 Signal22 = lerp( float4(0.08627f, 0.02353f, 0.02353f, 1.00000f), float4(0.17647f, 0.08235f, 0.12157f, 1.00000f), noiseT );

  noiseIn = objectScale*input.posLS.xyz*float3(0.60000f, 1.20000f, 0.20000f);
  value = Snoise3D ( noiseIn);
  noiseT =  sqrt(abs(cos( 5*noiseIn.x + 2*value ) ));
  noiseT +=  sqrt(abs(cos( 5*noiseIn.y + 2*value ) ));
  noiseT *= 0.5f;
  noiseT = clamp( noiseT, 0, 1 );
  float4 Signal33 = lerp( float4(0.00000f, 0.00000f, 0.00000f, 1.00000f), float4(1.00000f, 1.00000f, 1.00000f, 1.00000f), noiseT );


//RGBA CHANNEL
  float4 Layer00 = Signal00;
  float4 Layer33 = Signal22;
  float4 Layer55 = Signal33;
  float4 Layer44 = 0.5*Signal22 + 0.5*Signal33;
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

