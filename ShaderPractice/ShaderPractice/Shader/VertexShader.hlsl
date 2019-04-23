//���W�s��
cbuffer global_0:register(b0)
{
	matrix g_W;			//���[���h�s��
	matrix g_View;		//�r���[�s��
	matrix g_Proj;		//�v���W�F�N�V�����s��
	float4 g_Light=float4(1.0,1.0,1.0,1.0);	//���C�g����
};

//�}�e���A�����
cbuffer global_1:register(b1)
{
	float4 g_Ambient = float4(0, 0, 0, 1);//�A���r�G���g��
	float4 g_Diffuse = float4(0, 0, 0, 1);//�g�U����(�F�j
	float  g_Alpha = 1.0;				  //����
};

//�{�[�����
cbuffer global_2:register(b2)
{
	matrix g_BoneMat[2] : packoffset(c0); //�{�[���p���s��
	matrix g_BoneMatEnd : packoffset(c765);
};
                
//���͍\����
struct VS_IN {                     
    float4	Position: POSITION;      
	float3	Normal	: NORMAL;
	float2	Tex		: TEXCOORD0;
	float4	BoneId	: BONEID;
	float4  Weight	: WEIGHT;
};                                 
                                   
//�o�[�e�b�N�X�o�b�t�@�[�o�͍\����
struct VS_OUTPUT
{
	float4 Position	: SV_POSITION;
	float3 Normal	: Normal;
	float2 Tex		: TEXCOORD0;
	float4 Color	: TEXCOORD1;
	float3 View		: VIEW;
	float3 LightVec : LIGHT;
};
                                   
VS_OUTPUT main( VS_IN In )
{          
    VS_OUTPUT Out = (VS_OUTPUT)0; 
	//matrix SkinTransform = matrix(1.0f, 0.0f, 0.0f, 0.0f,
	//	                            0.0f, 1.0f, 0.0f, 0.0f,
	//								0.0f, 0.0f, 1.0f, 0.0f,
	//								0.0f, 0.0f, 0.0f, 1.0f);	//�{�[���p���ɂ��|���S���ւ̉e��

	////�{�[���ɂ����W�ϊ�
	//SkinTransform += g_BoneMat[In.BoneId.x] * In.Weight.x;
	//SkinTransform += g_BoneMat[In.BoneId.y] * In.Weight.y;
	//SkinTransform += g_BoneMat[In.BoneId.z] * In.Weight.z;
	//SkinTransform += g_BoneMat[In.BoneId.w] * In.Weight.w;

	//���_�����[���h��Ԃ֍��W�ϊ�
	//float4 Pos = mul(SkinTransform,In.Position);
	float4 Pos = In.Position;
	Pos = mul(Pos, g_W);
    Pos = mul( Pos, g_View );   
    Out.Position = mul( Pos, g_Proj );   

	//�@�������[���h��Ԃ֍��W�ϊ�
	Out.Normal = mul(In.Normal, g_W);
	Out.View = normalize(float3(g_View._13, g_View._23, g_View._33));
	Out.LightVec = g_Light.xyz;

	//���I�u�W�F�N�g�։e���f���Ȃ��ȈՃV���h�E
	float3 Norm = normalize(Out.Normal);
	float Shadow = saturate(dot(Norm, g_Light*-1));
	Shadow = Shadow * 0.5f + 0.5f;
	Out.Color = float4(Shadow, Shadow, Shadow, 1.0);

	//�e�N�X�`���[���W
	Out.Tex = In.Tex;

    return Out;                       
}                                     