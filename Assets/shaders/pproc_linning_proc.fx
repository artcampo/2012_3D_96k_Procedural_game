
Texture2D textCol;
Texture2D textSeed;

float4x4 ModelView : VIEW;
float4x4 ModelViewProj : WORLDVIEWPROJECTION;

float   Height;
float   Width;

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

  float2 screen = float2(1.0f / Width, 1.0f / Height);

  float3 vColor;
  float3 vColor00, vColor01, vColor02;
  float3 vColor10, vColor11, vColor12;
  float3 vColor20, vColor21, vColor22;

  vColor00 = textCol.Sample( samLinear, vSamplePosition + float2( -1.0f, -1.0f )*screen ).xyz;    
  vColor01 = textCol.Sample( samLinear, vSamplePosition + float2( 0.0f, -1.0f )*screen ).xyz;    
  vColor02 = textCol.Sample( samLinear, vSamplePosition + float2( 1.0f, -1.0f )*screen ).xyz;    

  vColor10 = textCol.Sample( samLinear, vSamplePosition + float2( -1.0f, 0.0f )*screen ).xyz;    
  vColor11 = textCol.Sample( samLinear, vSamplePosition + float2( 0.0f, 0.0f )*screen ).xyz;    
  vColor12 = textCol.Sample( samLinear, vSamplePosition + float2( 1.0f, 0.0f )*screen ).xyz;    

  vColor20 = textCol.Sample( samLinear, vSamplePosition + float2( -1.0f, 1.0f )*screen ).xyz;    
  vColor21 = textCol.Sample( samLinear, vSamplePosition + float2( 0.0f, 1.0f )*screen ).xyz;    
  vColor22 = textCol.Sample( samLinear, vSamplePosition + float2( 1.0f, 1.0f )*screen ).xyz;    


  //SOBEL:
  float sobelX = -vColor00 -2.0f*vColor10 - vColor20 + vColor02 + 2.0f*vColor12 + vColor22;
  float sobelY = -vColor00 -2.0f*vColor01 - vColor02 + vColor20 + 2.0f*vColor21 + vColor22;

/*
  //Basic differentiation
  float sobelX = vColor12 - vColor10;
  float sobelY = vColor21 -vColor01;
*/


  float colorGrayIntensity = dot(vColor11, float3(0.3, 0.59, 0.11));
/*
  vColor.x = sobelX;
  vColor.y = sobelY;
  vColor.z = 0.0f;

  vColor.x = 0.0f;
  vColor.y = 0.0f;
  vColor.z = colorGrayIntensity;
*/
  vColor.x = (sobelX +1.0)*0.5;
  vColor.y = (sobelY +1.0)*0.5;
  vColor.z = colorGrayIntensity;


//  vColor.xyz = textCol.Sample( samLinear, vSamplePosition);

	return float4( vColor, 1.0f );
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
