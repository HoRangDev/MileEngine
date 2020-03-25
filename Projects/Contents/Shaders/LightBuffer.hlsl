// Textures
Texture2D NormalTexture : register(t0);
Texture2D PositionTexture : register(t1);

// Constants
cbuffer LightParamsBuffer
{
	float3 LightPos;
	float3 LightColor;
	float3 LightDirection;
	float2 SpotlightAngles;
	float3 LightRange;
	uint LightType;
};

cbuffer CameraParamsBuffer
{
	float3 CameraPos;
};

cbuffer CheckerBoardBuffer
{
	bool CheckerBoardEnabled;
};

float4 MileVS(in float4 Position : POSITION) : SV_Position
{
	return Position;
}

void GetGBufferAttributes(in float2 screenPos, out float3 normal,
                           out float3 position)
{
	int3 sampleIndices = int3(screenPos.xy, 0);
	float4 normalTex = NormalTexture.Load(sampleIndices);
	normal = normalTex.xyz;
	position = PositionTexture.Load(sampleIndices).xyz;
}

float4 CalcLighting(in float3 normal, in float3 position)
{
   // 0 : Directional Light
   // 1 : Point Light
   // 2 : Spot Light
   // Diffuse term
	float3 L = 0;
	float attenuation = 1.0f;

	if (LightType == 1 || LightType == 2)
	{
		L = normalize(LightPos - position);

		float dist = length(L);
		attenuation = max(0, 1.0f - (dist / LightRange.x));

		L /= dist;
	}
	else if (LightType == 0)
	{
		L = -LightDirection;
	}

	if (LightType == 2)
	{
      // Attenuation by angle with light direction
		float3 L2 = LightDirection;
		float rho = dot(-L, L2);
		attenuation *= saturate((rho - SpotlightAngles.y) / (SpotlightAngles.x - SpotlightAngles.y));
	}

	float nDotL = saturate(dot(normal, L));
	float3 diffuse = nDotL * LightColor * attenuation;

	return float4(diffuse, 0.0f);
}

float4 MilePS(in float4 screenPos : SV_Position) : SV_Target0
{
   /* Checkerboard Rendering (Discard needless pixels) */
	bool discardPixel = ((screenPos.x + screenPos.y) % 2) == 0;
	clip(CheckerBoardEnabled && discardPixel ? -1 : 1);

	float3 normal;
	float3 position;

	GetGBufferAttributes(screenPos.xy, normal, position);

	return CalcLighting(normal, position);
}