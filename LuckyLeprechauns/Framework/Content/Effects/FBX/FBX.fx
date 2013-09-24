float4x4 World;
float4x4 View;
float4x4 Projection;

float4 ClipPlane;

static const int MAX_BONEMATRICES = 59;

float4x4 BoneMatrices[MAX_BONEMATRICES];

texture Colormap;

float AlphaCutoff;

struct TCamera
{
	float3 Position;
};

struct TMaterial
{
	float4 Ambient;
	float4 Diffuse;	
	float4 Specular;
	float4 Emissive;
	float Power;
};

struct TLight
{
	float3 Direction;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};

TLight Light;
TMaterial Material;
TCamera Camera;

sampler2D ColormapSampler = sampler_state 
{ 
	Texture = <Colormap>; 
	MagFilter = linear;
	MinFilter = linear;
	MipFilter = linear;
	AddressU = wrap; 
	AddressV = wrap;
};


struct VertexShaderInput
{
	float4 Position			: POSITION0;
	float3 Normal				: NORMAL0;
	float2 TexCoord			: TEXCOORD0;
	float4 BlendWeights		: BLENDWEIGHT;
	float4 BlendIndices		: BLENDINDICES;
};


struct VertexShaderOutput
{
	float4 Position			: POSITION0;
	float3 Normal				: TEXCOORD0;
	float2 TexCoord			: TEXCOORD1;
	float Clip					: TEXCOORD2;
	float3 View					: TEXCOORD3;
	float4 ScreenPosition	: TEXCOORD4;
};


struct VSInstance
{
	float4 Row1	: TEXCOORD1;
	float4 Row2	: TEXCOORD2;
	float4 Row3	: TEXCOORD3;
	float4 Row4	: TEXCOORD4;
};


VertexShaderOutput VSDefault(VertexShaderInput input, VSInstance instance)
{
	VertexShaderOutput output;

	float4x4 instanceTransform = float4x4(instance.Row1, instance.Row2, instance.Row3, instance.Row4);

	float4x4 world = mul(instanceTransform, World);

	float4 worldPosition = mul(input.Position, world);

	input.TexCoord.y = 1 - input.TexCoord.y;

	output.Position = mul(mul(worldPosition, View), Projection);
	output.Normal = mul(input.Normal, world);
	output.View = Camera.Position - worldPosition;
	output.TexCoord = input.TexCoord;
	output.Clip = dot(worldPosition, ClipPlane);
	output.ScreenPosition = output.Position;

	return output;
}


VertexShaderOutput VSSkinned(VertexShaderInput input, VSInstance instance)
{
	float3 position = 0;
	float3 normal = 0;

	float weights[4] = (float[4])input.BlendWeights;
	int indices[4] = (int[4])input.BlendIndices;

	for (int bone = 0; bone < 4; ++bone)
	{
		position += mul(input.Position, BoneMatrices[indices[bone]]) * weights[bone];
		normal += mul(input.Normal, BoneMatrices[indices[bone]]) * weights[bone];
	}

	input.Position = float4(position, 1.0f);
	input.Normal = normal;

	return VSDefault(input, instance);
}


float4 PSDefault(VertexShaderOutput input) : COLOR0
{
	clip(input.Clip);

	float4 texColor = tex2D(ColormapSampler, input.TexCoord);

	clip(texColor.a - AlphaCutoff);

	float3 normal = normalize(input.Normal);
	float3 light = normalize(-Light.Direction);
	float3 view = normalize(input.View);
	float3 reflection = reflect(-light, normal);

	float diffuse = saturate(dot(normal, light));
	float specular = pow(saturate(dot(view, reflection)), Material.Power);

	float4 ambientColor = Material.Ambient * Light.Ambient;
	float4 diffuseColor = Material.Diffuse * Light.Diffuse * diffuse;
	float4 specularColor = Material.Specular * Light.Specular * specular;
	float4 emissiveColor = Material.Emissive;

	return saturate(diffuseColor + ambientColor + emissiveColor) * texColor + specularColor;
}


float4 PSShadowMap(VertexShaderOutput input) : COLOR0
{
	float4 texColor = tex2D(ColormapSampler, input.TexCoord);

	clip(texColor.a - AlphaCutoff);

	return 1.0f - input.ScreenPosition.z / input.ScreenPosition.w;
}


technique Default
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 VSDefault();
		PixelShader = compile ps_3_0 PSDefault();
	}
}


technique Skinned
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 VSSkinned();
		PixelShader = compile ps_3_0 PSDefault();
	}
}


technique ShadowMapDefault
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 VSDefault();
		PixelShader = compile ps_3_0 PSShadowMap();
	}
}


technique ShadowMapSkinned
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 VSSkinned();
		PixelShader = compile ps_3_0 PSShadowMap();
	}
}