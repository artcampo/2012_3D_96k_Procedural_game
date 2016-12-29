Texture2D textPos;
Texture2D textNor;
Texture2D textTan;
Texture2D textCol;
Texture2D textLig;

Texture1D <float3> textSeed;

float4x4 ModelView : VIEW;
float4x4 ModelViewProj : WORLDVIEWPROJECTION;

RasterizerState rs_noCull
{
CullMode=None;
};

BlendState NoBlending
{
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0F;
};

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

float rand(float2 co){
    return frac(sin(dot(co.xy ,float2(12.9898,78.233))) * 43758.5453);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------



float4 PS_FINAL_COMPOSITION( VS_Output Input ) : SV_TARGET
{    
  const float sampleRadius  = 1.5;
  const float depthBias     = 0.2;
  const float singularity   = 1.5;
  const float strength      = 2;
  const int   patternSize   = 64;
  static const float PI = 3.14159265f;



  float4 vSample = 0.0f;
  float4 vColor = 0.0f;
  float2 vSamplePosition;
  
  
  vSamplePosition = Input.Tex;

  float4 pos      = textPos.Sample( samLinear, vSamplePosition);
  float3 normal1  = textNor.Sample( samLinear, vSamplePosition).xyz;

  float3 N      = normalize( normal1 );
  float3 Tan    = textTan.Sample( samLinear, vSamplePosition).xyz;
  Tan           = normalize(Tan);
  float3 BiTan  = normalize(cross(N, Tan));

  matrix  matLocal = matrix ( Tan.x, N.x, BiTan.x, 0,
                                  Tan.y, N.y, BiTan.y, 0,
                                  Tan.z, N.z, BiTan.z,0,
                                  0,0,0,1 );
                     
               
  float     ambientOcclusion    = 0.0;
  float3    directRadianceSum   = float3 ( 0.0, 0.0, 0.0 );
  float3    occRadianceSum      = float3 ( 0.0, 0.0, 0.0 );
 
  int numIterations = 8;
  /*
  int patternIndex = (int(vSamplePosition.x*1024) % (patternSize - numIterations)) + 
                     (int(vSamplePosition.y*748) % patternSize)*patternSize;
*/
  float2 randPos = rand(vSamplePosition);
  int patternIndex = (int(randPos.x*1024) % (patternSize - numIterations)) + 
                     (int(randPos.y*768) % patternSize)*patternSize;
 
                             
  float times = 0;
  float depthDifference = 0;

  for ( int i = 0; i < numIterations ; ++i )
    {
    float3 sample = textSeed.Load( int2( patternIndex+i, 0) ).xyz;
    sample= normalize(sample);
    

    float3 normSample = normalize(sample);
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
    if ( abs( depth - depthSam ) < singularity )
    {
      distanceTerm = 1.0;
        if (depthSam > depth)
          times += 1.0; 
      }
    else
    {
      distanceTerm = 0.0;
      }


    //depthDifference += abs( depth - depthSam );
    depthDifference += depth - depthSam ;

    float visibility = 1.0 - strength * ( depthSam > depth ? 1.0 : 0.0 ) * distanceTerm;
    
	  
    float recGeometricTerm = max( 0.0, dot( normSample, normal1 ));
    float3 senderRadiance = float3(1,1,1);
    float3 radiance = senderRadiance * visibility * recGeometricTerm;

    directRadianceSum += radiance;
    }

  directRadianceSum = max( directRadianceSum, float3 ( 0.0, 0.0, 0.0 ) ); 
  float3 radianceSum = directRadianceSum;
  radianceSum = radianceSum * (2.0*PI/float(numIterations));


  float4 vertexPos = textPos.Sample( samLinear, vSamplePosition);    
  float4 vertexLig = textLig.Sample( samLinear, vSamplePosition);    
  float4 vertexNor = textNor.Sample( samLinear, vSamplePosition);    
  vColor.xyz = textCol.Sample( samLinear, vSamplePosition).xyz;  

//  vColor.xyz += 0.5*radianceSum;
//vColor.xyz += float3(1,1,1)*(times/numIterations)*0.7;

//  vColor.xyz *= 1.5*textCol.Sample( samLinear, vSamplePosition).xyz;  
//  vColor.xyz = (vColor.xyz*vertexLig.xyz)+0.4*vColor.xyz;

  vColor.xyz = vertexLig.xyz + 0.3*vColor.xyz;
// vColor.xyz +=float3(1,1,1)*(times/numIterations)*0.7;
//  vColor.xyz = vertexLig.xyz;
//vColor.xyz = vertexNor;
//	vColor.a = 1.0;
	return vColor;



}

//--------------------------------------------------------------------------------------
technique10 SSDO
{

    pass P0
    {
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetRasterizerState(rs_noCull);
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_FINAL_COMPOSITION() ) );
        

    }
}
