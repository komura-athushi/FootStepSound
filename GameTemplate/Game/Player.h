#pragma once
class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	const Vector3& GetPosition() const
	{
		return m_position;
	}
private:
	void Move();
	void Rotation();
	void ManageState();
	void PlayAnimation();
	/// <summary>
	/// �A�j���[�V�����C�x���g�p�̊֐��B
	/// </summary>
	/// <param name="clipName">�A�j���[�V�����̖��O�B</param>
	/// <param name="eventName">�A�j���[�V�����C�x���g�̃L�[�̖��O�B</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
private:
	enum enAnimationClip
	{
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Num 
	};

	enum enPlayerState
	{
		enPlayerState_Idle,
		enPlayerState_Run,
		enPlayerState_Num
	};

	ModelRender						m_modelRender;
	AnimationClip					m_animationClips[enAnimationClip_Num];
	enPlayerState					m_playerState = enPlayerState_Idle;
	Vector3							m_position;
	Vector3							m_moveSpeed;
	float							m_animationSpeed = 1.0f;
	FontRender						m_fontRender;
};

