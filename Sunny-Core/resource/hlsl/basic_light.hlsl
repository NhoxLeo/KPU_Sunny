struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
};

struct VSOutput
{
	float4 positionCS : SV_POSITION;
	float3 cameraPosition : CAMERA_POSITION;
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float3 color : COLOR;
	float4 shadowCoord : SHADOW_POSITION;
	float4 lightPosition : LIGHT_POSITION;
};

cbuffer VSSystemUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
	float4x4 SUNNY_ViewMatrix;
	float4x4 SUNNY_ModelMatrix;
	float3	 SUNNY_CameraPosition;
	float4x4 SUNNY_LightProjectionMatrix;
	float4x4 SUNNY_LightViewMatrix;
};

VSOutput VSMain(in VSInput input)
{
	float3x3 wsTransform = (float3x3)SUNNY_ModelMatrix;

	VSOutput output;
	output.position = mul(input.position, SUNNY_ModelMatrix);
	output.positionCS =  mul(output.position, mul(SUNNY_ViewMatrix, SUNNY_ProjectionMatrix));
	output.normal = mul(input.normal, wsTransform);
	output.binormal = mul(input.binormal, wsTransform);
	output.tangent = mul(input.tangent, wsTransform);
	output.uv = input.uv;
	output.color = float3(1.0f, 1.0f, 1.0f);
	output.shadowCoord = float4(0.0f, 0.0f, 0.0f, 0.0f); // output.shadowCoord = mul(output.position, depthBias);

	output.lightPosition = mul(output.positionCS, SUNNY_LightViewMatrix);//�߰�
	output.lightPosition = mul(output.lightPosition, SUNNY_LightProjectionMatrix);//�߰�
	output.cameraPosition = SUNNY_CameraPosition;

	return output;
}

struct Light
{
	float4 color;
	float3 position;
	float p0;
	float3 ambientDown;
	float p1;
	float3 ambientRange;
	float intensity;
};

struct Attributes
{
	float3 position;
	float2 uv;
	float3 normal;
	float3 binormal;
	float3 tangent;
};

Texture2D textures : register(t0);
Texture2D shadowMap: register(t1);
SamplerState samplers : register(s0);

cbuffer PSSystemUniforms : register(b0)
{
	Light SUNNY_Light;
	float4 SUNNY_Color;
	float  SUNNY_HasTexture;
};


float3 CalcAmbient(float3 normal, float3 diffuseColor)
{
	float up = normal.y * 0.5 + 0.5;//�븻�� 0~1������ ��ȯ y���� ������� ��ġ�Ѵٰ� ����

	float3 ambient = SUNNY_Light.ambientDown + up * SUNNY_Light.ambientRange;//�����Ʈ ���� ���

	return ambient * diffuseColor;//�����Ʈ ���� ���� ���Ѵ�
}

float3 CalcDirectional(float3 worldPosition, float3 worldNormal, float4 diffuseColor, float3 EyePosition)
{
	// Phong diffuse
	float NDotL = dot(SUNNY_Light.position, worldNormal);//��ְ� ���� ���� ���͸� ������ ���ݻ籤�� ���� ���Ѵ�.
	float3 finalColor = SUNNY_Light.color.rgb * saturate(NDotL);//������ ���� 0���� �۰ų� 1���� ū ���� �ʿ�����Ƿ� ������

																// Blinn specular
	float3 ToEye = EyePosition.xyz - worldPosition;//ī�޶� ��ġ�� ���� ���������� ������ ���� ī�޶� ���͸� ���Ѵ�.
	ToEye = normalize(ToEye);//����ȭ
	float3 HalfWay = normalize(ToEye + SUNNY_Light.position);//ī�޶��Ϳ� ���� ������ �߰� ���͸� ���ϰ� ����ȭ
	float NDotH = saturate(dot(HalfWay, worldNormal));//�߰����Ϳ� �������͸� �����ؼ� �ڻ��� ���� ���ϰ� 0~1������ ����
	finalColor += SUNNY_Light.color.rgb * pow(NDotH, 250.0f) * SUNNY_Light.intensity;//���ݻ籤 ����� ������ ������ ���ϰ� ���⸦ ���ؼ� ���� ���ݻ籤�� ���� ����

	return finalColor * diffuseColor.rgb;//���ݻ籤+���ݻ�� * �ؼ�
}


float4 PSMain(in VSOutput input) : SV_TARGET
{
	float3 normal = normalize(input.normal);

	float4 texColor = (float4)SUNNY_Color;

	if (SUNNY_HasTexture >= 1)
	{
		texColor = textures.Sample(samplers, input.uv);
	}

	texColor.rgb *= texColor.rgb; // �� ��������.

	float3 finalColor = CalcAmbient(normal, texColor.rgb);
	finalColor += CalcDirectional(input.position, normal, texColor, input.cameraPosition);

	float2 shadowMapUV = input.lightPosition.xy / input.lightPosition.w;
	shadowMapUV.y = -shadowMapUV.y;
	shadowMapUV = shadowMapUV * 0.5 + 0.5;

	float shadowDepth = shadowMap.Sample(samplers, shadowMapUV).r;
	float currentDepth = input.position.z / input.position.w;

	if (currentDepth > shadowDepth + 0.000125f)
	{
		finalColor.rgb *= 0.5f;
	}

	return float4(finalColor, texColor.a);
}