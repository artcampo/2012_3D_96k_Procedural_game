#include "shader_defines.fx"

Texture2D textColBase;    //  Base colour

float4x4 ModelView : VIEW;
float4x4 ModelViewProj : WORLDVIEWPROJECTION;


float   time;

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

float4 PS_MICROSCOPE( VS_Output Input ) : SV_TARGET
{    
  float2 vSamplePosition  = Input.Tex;

  float4 finalColour, baseColour;

  //  Now distort simulating lens
  float2 textCoord = vSamplePosition - float2( 0.5, 0.5 );
  textCoord *= 2;  
  float   polarRadius  = sqrt( textCoord.x*textCoord.x  + textCoord.y*textCoord.y );
  float   polarAngle   = atan2( textCoord.y, textCoord.x );

  float t = time / 10;

  float deformation;

  float distance = polarRadius *t;
  float x = 2*3.1214 * distance / 10;

  deformation = sin(x-t)/x;
  deformation = deformation*0.5 + 0.5;

  float2 sum = float2 ( cos(polarAngle), sin(polarAngle) )*  0.1*deformation;

  textCoord += sum;
  
  textCoord *= 0.5;
  textCoord += float2( 0.5, 0.5 );  
  finalColour = textColBase.Sample( samLinear, textCoord);

float inte = deformation;
inte *= 0.1;
finalColour +=  float4(inte,inte,inte,1);

  return finalColour;
}


//--------------------------------------------------------------------------------------
technique10 PMIC
{

    pass C
    {
        SetDepthStencilState( StencilState_NoDepth, 0 );          
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );          
        
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_MICROSCOPE() ) );
    }

}
