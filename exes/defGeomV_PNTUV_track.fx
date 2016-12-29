//  Procedural Texture Shader v0.001
//  Deferred render
//  Not Gordian

#include "simplex.fx"
#include "shader_defines.fx"

matrix World;
matrix View;
matrix Projection;
float3 camPos;

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 nor : NORMAL;
    float3 tan : TANGENT;
    float2 uv  : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos   : SV_POSITION;
    float4 nor   : NORMAL;
    float4 tan   : TANGENT;
    float4 posLS : POSITION1;
    float4 posWS : POSITION2;
    float2 uv    : TEXCOORD;
    float  depth : SV_ClipDistance0;
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
  float4x4 gWV;

  gWV             = mul(World,View);
  gWVP            = mul(gWV,Projection);
  gWVP            = mul(World,View);
  gWVP            = mul(gWVP,Projection);
  output.pos      = mul( float4(input.pos,1.0f) , gWVP);
  output.nor      = mul( float4(input.nor,0.0f), World);
  output.nor.xyz  = normalize(output.nor.xyz);
  output.tan      = mul( float4(input.tan,0.0f), World);
  output.posLS    = float4(input.pos,1.0f);
  output.posWS    = mul( float4(input.pos,1.0f), World);
  output.depth    = mul( float4(input.pos,1.0f), gWV ).z;
  output.uv       = input.uv;
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
  noiseT = (Snoise3D( noiseIn) + 1)*0.5;
  float Signal11 = noiseT;

  noiseIn = objectScale*input.posLS.xyz*float3(2.60000f, 2.60000f, 2.60000f);
  noiseT = (Snoise3D( noiseIn) + 1)*0.5;
  float Signal22 = noiseT;

  noiseIn = objectScale*input.posLS.xyz*float3(2.50000f, 2.50000f, 2.50000f);
  noiseT = (Snoise3D( noiseIn) + 1)*0.5;
  float Signal00 = noiseT;


  //RGBA CHANNEL
  float4 Layer00;
  float3 res;
  float3 Layer00_col[4]={
    float3(0.00000f, 0.00000, 0.00000),
    float3(0.00000f, 0.33333, 0.49804),
    float3(0.00000f, 0.33333, 0.49804),
    float3(0.00000f, 0.00000, 0.00000)};
  {
    float x = Signal00;
    int nSpans  = 2;
    float t     = clamp(x, 0, 1)*nSpans;
    int span    = int(t);
    t           -= float(span);
    float c3   = -0.5*Layer00_col[span].x + 1.5*Layer00_col[span + 1].x - 1.5*Layer00_col[span + 2].x + 0.5*Layer00_col[span + 3].x;
    float c2   = Layer00_col[span].x - 2.5*Layer00_col[span + 1].x + 2*Layer00_col[span + 2].x - 0.5*Layer00_col[span + 3].x;
    float c1   = -0.5*Layer00_col[span].x +  0.5*Layer00_col[span + 2].x;
    float c0   = 1*Layer00_col[span + 1].x;
    res.x = ((c3*x + c2)*x + c1)*x + c0;
  }
  {
    float x = Signal00;
    int nSpans  = 2;
    float t     = clamp(x, 0, 1)*nSpans;
    int span    = int(t);
    t           -= float(span);
    float c3   = -0.5*Layer00_col[span].y + 1.5*Layer00_col[span + 1].y - 1.5*Layer00_col[span + 2].y + 0.5*Layer00_col[span + 3].y;
    float c2   = Layer00_col[span].y - 2.5*Layer00_col[span + 1].y + 2*Layer00_col[span + 2].y - 0.5*Layer00_col[span + 3].y;
    float c1   = -0.5*Layer00_col[span].y +  0.5*Layer00_col[span + 2].y;
    float c0   = 1*Layer00_col[span + 1].y;
    res.y = ((c3*x + c2)*x + c1)*x + c0;
  }  
  {
    float x = Signal00;
    int nSpans  = 2;
    float t     = clamp(x, 0, 1)*nSpans;
    int span    = int(t);
    t           -= float(span);
    float c3   = -0.5*Layer00_col[span].z + 1.5*Layer00_col[span + 1].z - 1.5*Layer00_col[span + 2].z + 0.5*Layer00_col[span + 3].z;
    float c2   = Layer00_col[span].z - 2.5*Layer00_col[span + 1].z + 2*Layer00_col[span + 2].z - 0.5*Layer00_col[span + 3].z;
    float c1   = -0.5*Layer00_col[span].z +  0.5*Layer00_col[span + 2].z;
    float c0   = 1*Layer00_col[span + 1].z;
    res.z = ((c3*x + c2)*x + c1)*x + c0;
  }    
  
  Layer00 = float4(res,1);

  float4 Layer11 = lerp( float4(0.00000, 0.00000, 0.00000, 1.00000), float4(1.00000, 1.00000, 1.00000, 1.00000), Signal11 );


  //BUMP CHANNEL
  float4 Layer22 = lerp( float4(0.00000, 0.00000, 0.00000, 1.00000), float4(1.00000, 1.00000, 1.00000, 1.00000), Signal22 );


  //FINAL PIXEL
  float  depthT = clamp( input.depth,20.00000f, 100.00000f)/90.00000f;


  float4 diffuseColour = lerp( Layer00, Layer11, depthT );
;

  finalPixel.rgb = diffuseColour.rgb;
  output.nor = input.nor;

  output.tan = input.tan;
  output.pos = input.posWS;
  output.col = float4( diffuseColour.xyz, 1);
  output.lig = float4(1.00000, 1.00000, 1.00000, 1.00000);
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

