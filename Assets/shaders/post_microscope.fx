#include "shader_defines.fx"

Texture2D textColBase;    //  Base colour

float4x4 ModelView : VIEW;
float4x4 ModelViewProj : WORLDVIEWPROJECTION;

float   Width;
float   Height;
float   ZFocus;
float   ZRadius;
float   Overture;

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
  float polarRadiusOld = polarRadius;

  float ovClamp = clamp( Overture, 0, 1);
  polarRadius /= ovClamp;
//  polarRadius = clamp( polarRadius, 0, 1 );
  polarRadius = asin( polarRadius );

  textCoord = float2 ( cos(polarAngle), sin(polarAngle) );
  textCoord *= polarRadius;
  textCoord *= 0.5;
  textCoord += float2( 0.5, 0.5 );  
  finalColour = textColBase.Sample( samLinear, textCoord);


  float distanceHalo = 0.1;  
  if ( polarRadiusOld < (Overture - distanceHalo) )
  {    
  baseColour = textColBase.Sample( samLinear, vSamplePosition);
  
  float tMaxRadius = (polarRadiusOld - 0.5) / (1 - 0.5);
  finalColour = lerp( finalColour, baseColour,  tMaxRadius);
  }
  else
  {
    float t = polarRadiusOld - (Overture - distanceHalo);

    t = t/distanceHalo;
    t = clamp (t,0,1);
    
    finalColour = lerp( finalColour, float4 (0.1,0.1,0.1,1),  t);
  }

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
