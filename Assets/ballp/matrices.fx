Texture2D textPos;
Texture2D textNor;
Texture2D textTan;
Texture2D textCol;
Texture2D textLig;

Texture2D textSeed;

float4x4 ModelView : VIEW;
float4x4 ModelViewProj : WORLDVIEWPROJECTION;

RasterizerState rs_noCull
{
CullMode=None;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    /*
    AddressU = Clamp;
    AddressV = Clamp;*/
    AddressU = Wrap;
    AddressV = Wrap;    
    
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
  const float sampleRadius  = 5;
  const float depthBias     = 1.3;
  const float singularity   = 0.5;
  const float strength      = 0;
  const int   patternSize   = 16;
  static const float PI = 3.14159265f;
	const float bounceSingularity = 30;
	const float bounceStrength = 0.85f;


    float4 vSample = 0.0f;
    float4 vColor = 0.0f;
    float2 vSamplePosition;
    
    
    vSamplePosition = Input.Tex;

    float4 pos      = textPos.Sample( samLinear, vSamplePosition);
    float3 normal1  = textNor.Sample( samLinear, vSamplePosition).xyz;


    float3 N      = normalize( normal1 );
    float3 Tan    = textTan.Sample( samLinear, vSamplePosition).xyz;
    Tan           = normalize(Tan);
    float3 BiTan  = normalize(cross(Tan, N));

    matrix  matLocal = matrix ( Tan.x, N.x, BiTan.x, 0,
                                    Tan.y, N.y, BiTan.y, 0,
                                    Tan.z, N.z, BiTan.z,0,
                                    0,0,0,1 );
                /*                    
    matrix  matLocal = matrix ( 
                                    Tan.x, Tan.y, Tan.z, 0,
                                    N.x, N.y, N.z, 0,
                                    BiTan.x, BiTan.y, BiTan.z, 0,
                                    0,0,0,1 );                                    
                  */  
    float     ambientOcclusion    = 0.0;
    float3    directRadianceSum   = float3 ( 0.0, 0.0, 0.0 );
    float3    occRadianceSum      = float3 ( 0.0, 0.0, 0.0 );
   

    int patternIndex  = ((vSamplePosition.x*1024) % patternSize) + 
                        ((vSamplePosition.y*748) % patternSize)*patternSize;

    float indexY = float(patternIndex/float(patternSize*patternSize));
             
    int numIterations = 8;
    float times = 0;
    for ( int i = 0; i < numIterations ; ++i )
      {
      float indexX = float(sin(i*465))/float(patternSize);
	    float3 sample = textSeed.Sample( samLinear, float2( indexX, indexY ) ).xyz;


      sample = mul ( sample, matLocal );
      
      float4 worldSamOccPos = pos + sampleRadius*float4( sample.xyz, 0.0 );
      float4 occSamPos = mul ( worldSamOccPos , ModelViewProj );

      float2 occTexCord = float2( occSamPos.x / occSamPos.w, -occSamPos.y / occSamPos.w );
      occTexCord = (occTexCord + float2( 1.0, 1.0 ) ) * 0.5;
	  
	  float4 occPos = textPos.Sample( samLinear, occTexCord);
	  float3 occNor = textNor.Sample( samLinear, occTexCord).xyz;

      float depth     = (mul ( worldSamOccPos , ModelView )).z;
      float depthSam  = (mul ( occPos , ModelView )).z + depthBias;

      float distanceTerm;
      if (  (depth - depthSam)  < 0 )
      {
        distanceTerm = 1.0;
        times += 1.0;
        }
      else
      {
        distanceTerm = 0.0;
        }

      }
   
  vColor.xyz = float3(1,1,1)*(times/float(numIterations));
	vColor.a = 1.0;
	return vColor;
}

//--------------------------------------------------------------------------------------
technique10 SSDO
{

    pass P0
    {
        SetRasterizerState(rs_noCull);
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_FINAL_COMPOSITION() ) );
    }
}
