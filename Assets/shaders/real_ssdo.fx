
Texture2D textPos;
Texture2D textNor;
Texture2D textTan;
Texture2D textCol;
Texture2D textLig;

Texture2D textSeed;

float4x4 ModelView : VIEW;
float4x4 ModelViewProj : WORLDVIEWPROJECTION;

int toggle;
float radius;

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

float4  decodifyVector ( float4 v )
{
  float4 ret;
	ret = (v - float4 ( 0.5, 0.5, 0.5, 0.5 ) ) * 2.0f;
  return ret;
}

float4  codifyVector ( float4 v )
{
  float4 ret;
	ret = (v * 0.5f) + float4 ( 0.5, 0.5, 0.5, 0.5 );
  return ret;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_Output VS_FINAL_COMPOSITION( VS_Input Input )
{
    VS_Output Output;

      Output.Pos = Input.Pos;
//Output.Pos = float4 ( Input.Tex.x*2.0f - 1.0f, 1.0f - Input.Tex.y*2.0f, 0.5f, 1.0f);


    Output.Tex = Input.Tex;

    return Output;
}



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------



float4 PS_FINAL_COMPOSITION( VS_Output Input ) : SV_TARGET
{    
  const int   patternSize   = 16;
  static const float PI = 3.14159265f;

  const float sampleRadius  = radius;
  const float depthBias     = 0.01;
  const float singularity   = radius;
  const float strength      = 0.05;
	const float bounceSingularity = radius;
	const float bounceStrength = 0.025f;


    float4 vSample = 0.0f;
    float4 vColor = 0.0f;
    float2 vSamplePosition;
    
    vSamplePosition = Input.Tex;

    float4 pos      = textPos.Sample( samLinear, vSamplePosition);
    float3 normal1  = decodifyVector( textNor.Sample( samLinear, vSamplePosition) ).xyz;


    float3 N      = normalize( normal1 );
    float3 Tan = decodifyVector( textTan.Sample( samLinear, vSamplePosition) ).xyz;
    Tan    = normalize(Tan);
    float3 BiTan  = normalize(cross(N, Tan));

    float3x3  matLocal = float3x3 ( Tan.x, N.x, BiTan.x, 
                                    Tan.y, N.y, BiTan.y, 
                                    Tan.z, N.z, BiTan.z );		

    float     ambientOcclusion    = 0.0;
    float3    directRadianceSum   = float3 ( 0.0, 0.0, 0.0 );
    float3    occRadianceSum      = float3 ( 0.0, 0.0, 0.0 );
    
    float numsamples = 0;

    int patternIndex = int(vSamplePosition.x*1024) % patternSize +
                      (int(vSamplePosition.y*748) % patternSize) * patternSize;
    float patternSizeSquareInverse = 1.0f/float(patternSize*patternSize);

    for ( int i = 0; i < 8 ; ++i )
      {

	    float3 sample = textSeed.Sample( samLinear, float2( i, patternIndex )*patternSizeSquareInverse ).xyz;
      float3 normSample = normalize(sample);
      sample = sample;
      sample = mul ( sample, matLocal );
      
      float4 worldSamOccPos = pos + float4( sampleRadius*sample.xyz, 0.0 );
      float4 occSamPos = mul ( worldSamOccPos , ModelViewProj );

      float2 occTexCord = float2( occSamPos.x / occSamPos.w, -occSamPos.y / occSamPos.w );
      occTexCord = (occTexCord + float2( 1.0, 1.0 ) ) * 0.5;
	  
	  float4 occPos = textPos.Sample( samLinear, occTexCord);
	  float3 occNor = textNor.Sample( samLinear, occTexCord).xyz;

      //
      float depth     = (mul ( worldSamOccPos , ModelView )).z;
      float depthSam  = (mul ( occPos , ModelView )).z + depthBias;

      float distanceTerm;
      if ( abs( depth - depthSam ) < singularity )
        distanceTerm = 1.0;
      else
        distanceTerm = 0.0;


      float visibility = 1.0 - strength * ( depthSam > depth ? 1.0 : 0.0 ) * distanceTerm;


	    if (visibility != 0.0f) numsamples+=1;
	  
      float recGeometricTerm = max( 0.0, dot( normSample, normal1 ));

//      float3 radiance = float3(1.0, 1.0, 1.0 ) * visibility * recGeometricTerm;
	  float3 senderRadiance = float3(0.5, 0.5, 0.5 );
      float3 radiance = senderRadiance * visibility * recGeometricTerm;
	  ambientOcclusion += visibility ;

      directRadianceSum += radiance;

      //Compute occluder radiance
//      float3 directRadiance = tex2D(ColrSampler,occTexCord).xyz;
//      float3 directRadiance = textPos.Sample( samLinear, occTexCord).xyz;    
float3 directRadiance = float3(0.5, 0.5, 0.5 );

      float3 delta = pos.xyz - occPos.xyz;
      float3 normDelta = normalize ( delta );

      float unclampedBounceGeometricTerm = 
            max ( 0.0, dot ( normDelta , - normal1 ))*
            max ( 0.0, dot ( normDelta , occNor )) /
            dot ( delta, delta );

	  float bounceGeometricTerm = min(unclampedBounceGeometricTerm, bounceSingularity);
			
      float3 occRadiance = bounceStrength * directRadiance * bounceGeometricTerm;
      occRadianceSum += occRadiance;




      }

  directRadianceSum = max( directRadianceSum, float3 ( 0.0, 0.0, 0.0 ) );
  occRadianceSum    = max( occRadianceSum, float3 ( 0.0, 0.0, 0.0 ) );
  float3 radianceSum = directRadianceSum + occRadianceSum;
//float3 radianceSum = directRadianceSum;
//float3 radianceSum = occRadianceSum;
  radianceSum = radianceSum * (2.0*PI/8);
//float3 radianceSum = float3(1.0, 1.0, 1.0 ) * (numsamples/8.0);


  float4 vertexPos = textPos.Sample( samLinear, vSamplePosition);    
  float4 vertexLig = textLig.Sample( samLinear, vSamplePosition);    
  float4 vertexNor = decodifyVector( textNor.Sample( samLinear, vSamplePosition) );    
  float4 vertexTan = decodifyVector( textTan.Sample( samLinear, vSamplePosition) );    
  float4 vertexCol = textCol.Sample( samLinear, vSamplePosition);    


  float3 ao = directRadianceSum + occRadianceSum;
//  ao = occRadianceSum;
//  ao = (ambientOcclusion/8.0f) ;

  if (toggle == 0)
    //vColor.xyz =  ((ambientOcclusion/8.0f) * float3(1.0, 1.0, 1.0 )) ;
  vColor.xyz= ao;

  if (toggle == 1)
    vColor.xyz = vertexLig.xyz;

  if (toggle == 2)
    vColor.xyz = vertexCol.xyz;

  if (toggle == 3)
    {
    vColor.xyz = codifyVector( vertexNor ).xyz;
    }

  if (toggle == 4)
    vColor.xyz = codifyVector( vertexTan ).xyz;

  if (toggle == 5)
    vColor.xyz = ao*vertexCol.xyz;

	vColor.a = 1.0;

	return vColor;



}

//--------------------------------------------------------------------------------------
technique10 SSDO
{

    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_FINAL_COMPOSITION() ) );
    }
}
