float4x4 World;
float4x4 ViewProjection;
float4x4 LightViewProjection;

texture Heightmap;
texture Normalmap;
texture Lightmap;

texture Texture0;
texture Texture1;
texture Texture2;
texture TextureCliff;

texture Shadowmap;

bool ShadowmapEnabled;

float2 TextureResolution;

float4 TextureRange;
float4 CliffRange;

int TerrainWidth;
int TerrainLength;

float TerrainOffset;

float Bumpiness;

struct TLight
{
	float3 Direction;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};

struct TMaterial
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emissive;
	float Power;
};

struct TCamera
{
	float3 Position;
};

TMaterial Material;
TLight Light;
TCamera Camera;

int LevelOfDetail;
int MinLevelOfDetail;

float4 ClipPlane;

sampler2D HeightmapSampler = sampler_state { 
	texture = <Heightmap>; 
	MagFilter = point;
	MinFilter = point;
	MipFilter = point;
	AddressU = clamp; 
	AddressV = clamp;
};


sampler2D NormalmapSampler = sampler_state 
{
	texture = <Normalmap>; 
	MagFilter = linear;
	MinFilter = linear;
	MipFilter = linear;
	AddressU = clamp; 
	AddressV = clamp;
};


sampler2D LightmapSampler = sampler_state 
{
	texture = <Lightmap>; 
	MagFilter = linear;
	MinFilter = linear;
	MipFilter = linear;
	AddressU = clamp; 
	AddressV = clamp;
};


sampler2D ShadowmapSampler = sampler_state 
{
	texture = <Shadowmap>; 
	MagFilter = point;
	MinFilter = point;
	MipFilter = point;
	AddressU = clamp;
	AddressV = clamp;
};


sampler2D TextureSampler0 = sampler_state 
{
	texture = <Texture0>; 
	MagFilter = linear;
	MinFilter = linear;
	MipFilter = linear;
	AddressU = wrap; 
	AddressV = wrap;
};


sampler2D TextureSampler1 = sampler_state 
{
	texture = <Texture1>; 
	MagFilter = linear;
	MinFilter = linear;
	MipFilter = linear;
	AddressU = wrap; 
	AddressV = wrap;
};


sampler2D TextureSampler2 = sampler_state {
	texture = <Texture2>; 
	MagFilter = linear;
	MinFilter = linear;
	MipFilter = linear;
	AddressU = wrap; 
	AddressV = wrap;
};


sampler2D TextureSamplerCliff = sampler_state {
	texture = <TextureCliff>; 
	MagFilter = linear;
	MinFilter = linear;
	MipFilter = linear;
	AddressU = wrap; 
	AddressV = wrap;
};


struct VSInstance
{
	float2 PatchOffset	: TEXCOORD0;
	float Blending			: TEXCOORD1;
};


struct VertexShaderInput
{
	float4 Position : POSITION0;
};


struct VertexShaderOutput
{
	float4 Position		: POSITION0;
	float2 TexCoord		: TEXCOORD0;
	float Clip				: TEXCOORD1;
	float3 View				: TEXCOORD2;
	float Depth				: TEXCOORD4;
	float4 WorldPos		: TEXCOORD5;
	float4 ScreenPos		: TEXCOORD6;
};


float2 getTexCoord(float2 position)
{
	return float2(position.x / TerrainWidth, 1.0f - position.y / TerrainLength);
}


float getHeight(float2 position)
{
	return tex2Dlod(HeightmapSampler, float4(getTexCoord(position), 0, 0)).r;
}


float getMorphedHeight(float2 position, float blending, int levelOfDetail)
{
	float height = getHeight(position);

	if (levelOfDetail == MinLevelOfDetail)
		return height;

	int delta = pow(2, levelOfDetail);

	int a = ((position.x / delta) % 2) * delta;
	int b = ((position.y / delta) % 2) * delta;

	float h0 = getHeight(position + float2(a, b));
	float h1 = getHeight(position - float2(a, b));

	return height + blending * ((h0 + h1) / 2.0f - height);
}


// überprüft, ob die angegebene Position im Schatten liegt
bool IsInShadow(float4 position, sampler2D shadowMap)
{
	// XY Koordinaten auf der ShadowMap berechnen
	float2 uv = ((position.xy / position.w) + float2(1.0f, 1.0f)) / 2.0f;

	uv.y = 1.0f - uv.y;

	// Tiefenwert der Position und der Shadowmap berechnen
	float z = position.z;
	float pixelDepth = position.z / position.w;
	float shadowMapDepth = 1.0f - tex2D(shadowMap, uv).r;

	// wenn Position außerhalb der Shadowmap liegt
	if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
		return false;

	float error = 0.001f;

	// Position liegt im Schatten, wenn Tiefenwert der Position größer als Tiefenwert der ShadowMap ist
	return pixelDepth >= shadowMapDepth + error;
}


VertexShaderOutput VertexShaderFunction(VertexShaderInput input, VSInstance instance)
{
	VertexShaderOutput output;

	float4 position = input.Position;

	position.xz += instance.PatchOffset;

	float2 texCoord = getTexCoord(position.xz);

	float height = (getMorphedHeight(position.xz, instance.Blending, LevelOfDetail) + TerrainOffset) * Bumpiness;

	position.y = (LevelOfDetail + 1) * position.y + height;

	float4 worldPos = mul(position, World);

	output.Position = mul(worldPos, ViewProjection);
	output.TexCoord = texCoord;
	output.Clip = dot(position, ClipPlane);
	output.View = Camera.Position - worldPos;
	output.Depth = output.Position.z;
	output.WorldPos = worldPos;
	output.ScreenPos = output.Position;

	return output;
}


float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{    	
	clip(input.Clip);

	float3 normal = normalize(2.0f * tex2D(NormalmapSampler, input.TexCoord).xzy - 1.0f);
	float3 light = normalize(-Light.Direction);
	float3 view = normalize(input.View);

	float3 reflection = reflect(-light, normal);

	float diffuse = saturate(dot(normal, light));
	float specular = pow(saturate(dot(view, reflection)), Material.Power);
	
	float height = input.WorldPos.y / Bumpiness;

	float2 texCoord = input.TexCoord * TextureResolution;

	float4 tex0 = tex2D(TextureSampler0, texCoord);
	float4 tex1 = tex2D(TextureSampler1, texCoord);
	float4 tex2 = tex2D(TextureSampler2, texCoord);
	float4 texCliff = tex2D(TextureSamplerCliff, texCoord);

	float weight0 = saturate(1.0f - (height - TextureRange.y) / (TextureRange.x - TextureRange.y));
	float weight2 = saturate((height - TextureRange.w) / (TextureRange.z - TextureRange.w));
	float weight1 = 1.0f - weight0 - weight2;

	float weightCliff = saturate(1.0f - (normal.y - CliffRange.x) / (CliffRange.y - CliffRange.x));

	float4 colorTexture = lerp(tex0 * weight0 + tex1 * weight1 + tex2 * weight2, texCliff, weightCliff);
	float4 colorLightmap = tex2D(LightmapSampler, input.TexCoord).r;

	bool isInShadow = ShadowmapEnabled && IsInShadow(mul(input.WorldPos, LightViewProjection), ShadowmapSampler);

	float4 colorAmbient = Material.Ambient * Light.Ambient;
	float4 colorDiffuse = Material.Diffuse * Light.Diffuse * diffuse * !isInShadow * colorLightmap;
	float4 colorSpecular = Material.Specular * Light.Specular * specular * weightCliff * !isInShadow * colorLightmap;

	return colorTexture * saturate(colorAmbient + colorDiffuse) + colorSpecular;
}


float4 PSShadowMap(VertexShaderOutput input) : COLOR0
{
	return 1.0f - input.ScreenPos.z / input.ScreenPos.w;
}


float4 PSLightMap(VertexShaderOutput input) : COLOR0
{
	return !IsInShadow(mul(input.WorldPos, LightViewProjection), ShadowmapSampler);
}


technique Default
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}


technique ShadowMap
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PSShadowMap();
	}
}


technique LightMap
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PSLightMap();
	}
}
