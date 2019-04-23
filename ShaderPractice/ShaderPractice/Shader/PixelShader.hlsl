Texture2D g_Tex: register(t0);
SamplerState g_SampleLinear : register(s0);
Texture2D g_DepthTex:register(t1);

cbuffer global_1:register(b0)
{
	float4 g_Ambient = float4(0, 0, 0, 0);//�A���r�G���g��
	float3 g_Diffuse = float3(0, 0, 0);//�g�U����(�F�j
	float g_Alpha = 0.5;
};

//�o�͍\����
struct PS_OUTPUT {
	float4 Default;
};

//���͍\����
struct PS_INPUT
{
	float4 Pos		: SV_POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD0;
	float4 Color	: TEXCOORD1;
	float3 View		: VIEW;
	float3 LightVec : LIGHT;
};

float4 main(PS_INPUT In) : SV_TARGET
{
	//���̔��˂��v�Z
	float3 ViewDir = In.View + In.LightVec;
	float specular = max(0, mul(normalize(ViewDir),normalize(In.Normal)));
    specular = pow(specular, 20);

	float4 color = g_Tex.Sample(g_SampleLinear, In.Tex);
	color.rgb += float3(1.0,1.0,1.0) * specular;
	color.w = g_Alpha;
	
	return color;
}