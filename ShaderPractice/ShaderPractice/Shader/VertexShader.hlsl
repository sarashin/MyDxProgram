//座標行列
cbuffer global_0:register(b0)
{
	matrix g_W;			//ワールド行列
	matrix g_View;		//ビュー行列
	matrix g_Proj;		//プロジェクション行列
	float4 g_Light=float4(1.0,1.0,1.0,1.0);	//ライト方向
};

//マテリアル情報
cbuffer global_1:register(b1)
{
	float4 g_Ambient = float4(0, 0, 0, 1);//アンビエント光
	float4 g_Diffuse = float4(0, 0, 0, 1);//拡散反射(色）
	float  g_Alpha = 1.0;				  //透過
};

//ボーン情報
cbuffer global_2:register(b2)
{
	matrix g_BoneMat[2] : packoffset(c0); //ボーン姿勢行列
	matrix g_BoneMatEnd : packoffset(c765);
};
                
//入力構造体
struct VS_IN {                     
    float4	Position: POSITION;      
	float3	Normal	: NORMAL;
	float2	Tex		: TEXCOORD0;
	float4	BoneId	: BONEID;
	float4  Weight	: WEIGHT;
};                                 
                                   
//バーテックスバッファー出力構造体
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
	//								0.0f, 0.0f, 0.0f, 1.0f);	//ボーン姿勢によるポリゴンへの影響

	////ボーンによる座標変換
	//SkinTransform += g_BoneMat[In.BoneId.x] * In.Weight.x;
	//SkinTransform += g_BoneMat[In.BoneId.y] * In.Weight.y;
	//SkinTransform += g_BoneMat[In.BoneId.z] * In.Weight.z;
	//SkinTransform += g_BoneMat[In.BoneId.w] * In.Weight.w;

	//頂点をワールド空間へ座標変換
	//float4 Pos = mul(SkinTransform,In.Position);
	float4 Pos = In.Position;
	Pos = mul(Pos, g_W);
    Pos = mul( Pos, g_View );   
    Out.Position = mul( Pos, g_Proj );   

	//法線をワールド空間へ座標変換
	Out.Normal = mul(In.Normal, g_W);
	Out.View = normalize(float3(g_View._13, g_View._23, g_View._33));
	Out.LightVec = g_Light.xyz;

	//他オブジェクトへ影を映さない簡易シャドウ
	float3 Norm = normalize(Out.Normal);
	float Shadow = saturate(dot(Norm, g_Light*-1));
	Shadow = Shadow * 0.5f + 0.5f;
	Out.Color = float4(Shadow, Shadow, Shadow, 1.0);

	//テクスチャー座標
	Out.Tex = In.Tex;

    return Out;                       
}                                     