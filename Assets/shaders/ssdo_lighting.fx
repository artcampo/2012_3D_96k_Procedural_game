
cbuffer cbChangesEveryResize
  {
  matrix    mP;
  matrix    mV;
  float3    camPos;
  };

cbuffer cbChangesForEveryLight
  {
  matrix    mW;
  float3    lPos;       //WC
  float4    lCol;     
  float     lRad;       //WC
  float2    scCenter;
  float2    scRadius;
  };

Texture2D textPos2;
Texture2D textNor2;
Texture2D textTan2;
Texture2D textCol2;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

DepthStencilState EnableDepthLess
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
	  DepthFunc = LESS;
};

DepthStencilState StencilState_NoDepth
{
    DepthEnable = false;
    DepthWriteMask = ZERO;
    DepthFunc = Less;
};

BlendState NoBlending
{
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0F;
};

RasterizerState rs_noCull
{
CullMode=None;
};

BlendState SrcAlphaBlendingAdd
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ONE;
    DestBlendAlpha = ONE;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;   
    float2 Tex    : TEXCOORD0;
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{

    VS_OUTPUT output;
/*
   float2 offset;
    offset.x =  scRadius.x * input.Tex.x ;
    offset.y =  -scRadius.y * input.Tex.y ;
    output.Pos = float4 ( scCenter + offset , 0.5, 1.0);
    output.Tex = output.Pos.xy;
*/

    output.Pos = input.Pos;
    output.Tex = input.Tex;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 lighting(float3 pos, float4 base, float3 normal ){

	// Lighting vectors
	float3 lVec = (lPos - pos);
	float3 lightVec = normalize(lVec);
	float3 viewVec = normalize(pos - camPos);

	// Attenution that falls off to zero at light radius
	const float a = 0.15;
  float distance = length(lVec);
  float d = max(distance - lRad, 0);  // Force radius of light
//  float d = distance;
	//float atten = 2 * saturate((1.0 + a) / (1.0 + (1.0 / a) * dot(lVec, lVec)) - a);
//float atten = 1.0 / (d * d);

  float dist = length(lVec);
  float att_t = clamp ( ( lRad - dist ), 0.0, lRad );
  att_t = att_t / lRad;
  float atten = 1.0f - exp ( -(att_t*10.0f));

/*
  float dist = length(lVec);
  //float d = max( (dist - lRad) , 0.0f );
  float d = clamp( (dist - lRad) , 0.0f, lRad );
  float invRadius = 1.0f / lRad ;
  float atten = clamp(1.0 - invRadius * d, 0.0, 1.0);
  //atten = 1.0 / (1.0f + 0.0000005f * d * d);


	// Lighting
	float diffuse = saturate(dot(lightVec, normal));
	float specular = 0.2 * pow(saturate(dot(reflect(-viewVec, normal), lightVec)), 10.0);

//return (diffuse * base + specular);
	return float4( atten * (diffuse * base.xyz + specular* base.xyz) , base.a);
//return diffuse * (base );
}

float4 PS( VS_OUTPUT input ) : SV_Target
{
//float4 color = lCol;   

  float4 vertexPos = textPos2.Sample( samLinear, input.Tex );    
  float4 vertexNor = textNor2.Sample( samLinear, input.Tex );   

  float4 color = float4 (lighting( vertexPos.xyz , lCol, vertexNor.xyz ) );

  float intensity = dot ( float3( 1.0f, 1.0f, 1.0f ), color.xyz );
  if ( intensity == 0.0f )
    discard;
//color = lCol;   
//color = lCol ;

  return color;
}

//--------------------------------------------------------------------------------------
technique10 SSDO_LIGHTING_PASS
{

    pass P0
    {
        SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( StencilState_NoDepth, 0 );

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        SetRasterizerState(rs_noCull);
    }
}
