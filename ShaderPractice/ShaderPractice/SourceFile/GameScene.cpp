#include "stdafx.h"
#include "GameScene.h"
#include "CharactorObject.h"
#include"PhysicsWorld.h"
#include"Input.h"

//�R���X�g���N�^
GameScene::GameScene()
{
}

//�f�X�g���N�^
GameScene::~GameScene()
{
}

//�V�[���̃��[�h
//pRenderer:�����_���[
void GameScene::Load(shared_ptr<D3DRenderer> pRenderer, StageData Data)
{
	m_pRenderer = pRenderer;
	int ModelNum = 30;
	m_pCharactor.resize(ModelNum);
	m_pSafeArea.resize(1);
	m_StageObject.resize(ModelNum);
	
	//�v���C���[
	m_Player = make_shared<PlayerObject>("../ShaderPractice/Resource/BlackBall.fbx", m_pRenderer->GetDevice(), Transform(XMFLOAT3(0.0, 0.0, 0.0)), 1, XMLoadFloat(new FLOAT(1.0)),XMLoadFloat(new FLOAT(1.2f)),10.0);
	//�����I�u�W�F�N�g
	m_pCharactor[0] = make_unique<CharactorObject>("../ShaderPractice/Resource/BlackBall.fbx", m_pRenderer->GetDevice(), Transform(XMFLOAT3(20.0, 10.0, 0.0)), 1, XMLoadFloat(new FLOAT(1.0)),1.0);
	//�Q�[���I�[�o�[�G���A
	m_pSafeArea[0] = make_unique<SafeArea>(0, XMLoadFloat3(&XMFLOAT3(1000, 90, 1000)), Transform(XMFLOAT3(0, -100, 0)),m_Player);
	float x, y;

	//�R�s�[���Đ���
	for (int i = 1; i < ModelNum; i++)
	{
		m_pCharactor[i] = make_unique<CharactorObject>();
		srand(int(&m_pCharactor[i]));
		m_pCharactor[0]->CopyObject(m_pCharactor[i], Transform(XMFLOAT3(rand()%30, 30, rand() % 30)));
	}

	//�����Ȃ��I�u�W�F�N�g
	m_StageObject[0] = make_unique<StageObject>("../ShaderPractice/Resource/Field.fbx", m_pRenderer->GetDevice(), Transform(XMFLOAT3(0.0, 0.0, 0.0)), 0, XMLoadFloat3(&XMFLOAT3(50.0, 5.0, 50.0)));
	//�J�����̑O�x�N�g��
	m_Player->Flont = &m_Camera;
}

//�V�[���̏�����
void GameScene::Init()
{

	PhysicsWorld::GetInstance().StepPhysics();

	m_pSafeArea[0]->Init();
	m_Light.resize(1);
	m_Light[0] = Light(XMFLOAT3(1.0, 1.0, -1.0));

	m_Player->Init(Transform(XMFLOAT3(0.0, 50.0, 0.0)));

	for (int i = 0; i < m_pCharactor.size(); i++)
	{
		m_pCharactor[i]->Init();
	}

	//�J�����ݒ�
	m_Camera.SetProjMat(D3DXToRadian(45.0f), 640 / 320, 0.1f, 1000.0f);
	m_Camera.SetViewMat(XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	m_Camera.SetTranslateW(m_Player->m_WorldMat.GetPosition());
}

//�V�[���̍X�V
//pNextScene:���V�[��id
SCENE_MESSAGE GameScene::Update(DWORD* pNextScene)
{
	//Physics�X�V
	PhysicsWorld::GetInstance().StepPhysics();

	//�J������]
	//��
	if(InputManager::GetInstance().GetKeyState(DIK_A))
	{
		m_Camera.RotationL(XMFLOAT3(0.0, -0.02, 0.0));
	}

	//�E
	if (InputManager::GetInstance().GetKeyState(DIK_D))
	{
		m_Camera.RotationL(XMFLOAT3(0.0, 0.02, 0.0));
	}
	//������
	if (InputManager::GetInstance().GetKeyState(DIK_R))
	{
		Init();
	}

	//���W�X�V
	m_Player->Update();
	m_Camera.SetTranslateW(m_Player->m_WorldMat.GetPosition());

	for (int i = 1; i < 30; i++)
		m_pCharactor[i]->Update();

	if (m_pSafeArea[0]->Update())
	{
		return SCENE_RELOAD;
	}
	
	return SCENE_LOOP;
}

//�`��
void GameScene::Render()
{	
	m_Player->Draw(m_Camera, m_Light, m_pRenderer->GetDevContext());

	int Size = m_pCharactor.size();

	for (int i = 0; i < Size; i++)
		m_pCharactor[i]->Draw(m_Camera, m_Light, m_pRenderer->GetDevContext());

	Size = m_StageObject.size();

	for (int i = 0; i < Size; i++)
	{
		m_StageObject[0]->Draw(m_Camera, m_Light, m_pRenderer->GetDevContext());
	}
}

//���
void GameScene::Release()
{
	m_Light.clear();
	m_pCharactor.clear();
	m_Player.reset();
	m_StageObject.clear();
	m_pSafeArea.clear();
}
