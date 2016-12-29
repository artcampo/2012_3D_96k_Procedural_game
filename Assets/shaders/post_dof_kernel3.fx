
Texture2D textZPos;       //  r-> z position of sample
Texture2D textColBase;    //  Base colour
Texture2D textColBlurred; //  Blurred version

float4x4 ModelView : VIEW;
float4x4 ModelViewProj : WORLDVIEWPROJECTION;

float   Width;
float   Height;
float   ZFocus;
float   ZRadius;

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

float4 PS_DOFH( VS_Output Input ) : SV_TARGET
{    
  float off[5] = {0,1,2,3,4};
  float kernel3[3] = { 0.2270270270, 0.3162162162, 0.0702702703 };
  float kernel5[5] ={ 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 };
  float2 vSamplePosition  = Input.Tex;

  float   offset = 1.0f / Width;

  float4 vColor = textColBase.Sample( samLinear, vSamplePosition) * kernel3[0];    
  for ( int i = 1; i < 3; ++i)
  {
    vColor.xyz += (textColBase.Sample( samLinear, vSamplePosition + float2( offset*off[i], 0 ))*kernel3[i]).xyz;    
    vColor.xyz += (textColBase.Sample( samLinear, vSamplePosition - float2( offset*off[i], 0 ))*kernel3[i]).xyz;    
  }

  vColor.a = 1.0f;
  return vColor;

}

float4 PS_DOFV( VS_Output Input ) : SV_TARGET
{    
  float off[5] = {0,1,2,3,4};
  float kernel3[3] = { 0.2270270270, 0.3162162162, 0.0702702703 };
float kernel5[5] ={ 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 };
  float2 vSamplePosition  = Input.Tex;

  float   offset = 1.0f / Height;


  float4 vColorBlur = textColBlurred.Sample( samLinear, vSamplePosition) * kernel3[0];    
  for ( int i = 1; i < 3; ++i)
  {
    vColorBlur.xyz += (textColBlurred.Sample( samLinear, vSamplePosition + float2( 0,offset*off[i] ))*kernel3[i]).xyz;    
    vColorBlur.xyz += (textColBlurred.Sample( samLinear, vSamplePosition - float2( 0,offset*off[i] ))*kernel3[i]).xyz;    
  }


  vColorBlur.a = 1.0f;
  float4 vColorBase = textColBase.Sample( samLinear, vSamplePosition);
  vColorBase.a = 1.0f;

  float4 finalColour;
  float t = (textZPos.Sample( samLinear, vSamplePosition) - ZFocus)/ZRadius;
  t = abs(t);
  t = clamp ( t, 0, 1);
  t = 1.0f - t;
  finalColour = lerp ( vColorBlur, vColorBase, t );  

  return finalColour;

}

//--------------------------------------------------------------------------------------
technique10 DOF
{

    pass H
    {
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_DOFH() ) );
    }

    pass V
    {
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_DOFV() ) );
    }
}
