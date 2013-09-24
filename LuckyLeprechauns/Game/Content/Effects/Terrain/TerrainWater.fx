float4x4 WorldViewProjection;

texture Refractionmap;
texture Reflectionmap;
texture Wavemap;

float3 CameraPosition;

float3 WaterOffset;
int WaterWidth;
int WaterLength;

int PatchSize;

float2 WaveHeight;
float2 WaveLength;
float2 WaveSpeed;

float Time;

sampler2D ReflectionmapSampler = sampler_state {
	texture = <Reflectionmap>; 
	MagFilter = linear;
	MinFilter = linear;
	AddressU = mirror;
	AddressV = mirror;
};


sampler2D RefractionmapSampler = sampler_state {
	texture = <Refractionmap>; 
	MagFilter = linear;
	MinFilter = linear;
	AddressU = mirror;
	AddressV = mirror;
};


sampler2D WavemapSampler = sampler_state {
	texture = <Wavemap>; 
	MagFilter = linear;
	MinFilter = linear;
	MipFilter = linear;
	AddressU = mirror;
	AddressV = mirror;
};


struct VertexShaderInput
{
	float4 Position : POSITION0;
};


struct VertexShaderOutput
{
	float4 Position		: POSITION0;
	float4 ScreenPos		: TEXCOORD0;
	float2 WaveCoord		: TEXCOORD1;
	float3 View				: TEXCOORD2;
};


VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output;

	float4 position = input.Position;
	float2 texCoord = float2(position.x / PatchSize, 1.0f - position.z / PatchSize);

	position.x *= (float)WaterWidth / PatchSize;
	position.z *= (float)WaterLength / PatchSize;

	position += float4(WaterOffset, 0);

	output.Position = mul(position, WorldViewProjection);
	output.ScreenPos = output.Position;
	output.WaveCoord = (texCoord + Time * WaveSpeed) / WaveLength;
	output.View = CameraPosition - position.xyz;

	return output;
}


float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{  
   float2 texCoord = (input.ScreenPos.xy / input.ScreenPos.w + 1.0f) / 2.0f;
	float2 offset = tex2D(WavemapSampler, input.WaveCoord).xy * 2.0f - 1.0f;

	float fresnel = dot(normalize(input.View), float3(0, 1, 0));

	texCoord.y = 1.0f - texCoord.y;
	texCoord.y += (1.0f - fresnel) * 0.003f;
	texCoord += offset * WaveHeight;

	float4 reflectionColor = tex2D(ReflectionmapSampler, texCoord);
	float4 refractionColor = tex2D(RefractionmapSampler, texCoord);

	return lerp(reflectionColor, refractionColor, fresnel);
}


technique Technique1
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}
