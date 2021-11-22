#include "stdafx.h"
#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/human/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/human/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);

	m_modelRender.Init("Assets/modelData/human/human.tkm",m_animationClips,enAnimationClip_Num);

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	g_soundEngine->ResistWaveFileBank(0,"Assets/sound/step.wav");

	m_fontRender.SetPosition(Vector3(0.0f,400.0f,0.0f));
	return true;
}

void Player::Update()
{
	Move();
	Rotation();
	ManageState();
	PlayAnimation();

	m_modelRender.Update();

	if (g_pad[0]->IsPress(enButtonA))
	{
		m_animationSpeed += g_gameTime->GetFrameDeltaTime();
	}
	if (g_pad[0]->IsPress(enButtonB))
	{
		m_animationSpeed -= g_gameTime->GetFrameDeltaTime();
	}

	std::wstring text;
	text = L"A�{�^���ŃA�j���[�V�����������Ȃ�\nB�{�^���ŃA�j���[�V�������x���Ȃ�\n";
	text += L"���݂̃A�j���[�V�����Đ����x\n";
	text += std::to_wstring(m_animationSpeed);
	m_fontRender.SetText(text.c_str());
}

void Player::Move()
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed.y = 0.0f;

	//���̃t���[���̈ړ��ʂ����߂�B
	//���X�e�B�b�N�̓��͗ʂ��󂯎��B
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//�J�����̑O�������ƉE�������擾�B
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����B
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ�����̈ړ����x���N���A�B
	m_moveSpeed += cameraForward * lStick_y * 170.0f;	//�������ւ̈ړ����x�����Z�B
	m_moveSpeed += cameraRight * lStick_x * 170.0f;		//�E�����ւ̈ړ����x�����Z�B

	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) <= 0.01f &&
		fabsf(m_moveSpeed.z) <= 0.01f)
	{
		return;
	}

	Quaternion	rotation;
	rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	m_modelRender.SetRotation(rotation);
}

void Player::ManageState()
{
	if (fabsf(m_moveSpeed.x) <= 0.01f &&
		fabsf(m_moveSpeed.z) <= 0.01f)
	{
		m_playerState = enPlayerState_Idle;
		return;
	}

	m_playerState = enPlayerState_Run;
}

void Player::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(m_animationSpeed);
	switch (m_playerState)
	{
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.5f);
		break;
	default:
		break;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"step") == 0) 
	{
		SoundSource* se = NewGO<SoundSource>(0, "stepSound");
		se->Init(0);
		se->Play(false);
	}
}

void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);


	m_fontRender.Draw(rc);
}