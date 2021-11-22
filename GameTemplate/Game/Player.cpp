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
	text = L"Aボタンでアニメーションが速くなる\nBボタンでアニメーションが遅くなる\n";
	text += L"現在のアニメーション再生速度\n";
	text += std::to_wstring(m_animationSpeed);
	m_fontRender.SetText(text.c_str());
}

void Player::Move()
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed.y = 0.0f;

	//このフレームの移動量を求める。
	//左スティックの入力量を受け取る。
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//カメラの前方方向と右方向を取得。
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_moveSpeed += cameraForward * lStick_y * 170.0f;	//奥方向への移動速度を加算。
	m_moveSpeed += cameraRight * lStick_x * 170.0f;		//右方向への移動速度を加算。

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
	//キーの名前が「attack_start」の時。
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