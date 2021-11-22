#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	m_toPos = Vector3(0.0f, 100.0f, -120.0f);
	m_player = FindGO<Player>("player");
	return true;
}

void GameCamera::Update()
{
	Vector3 playerPosition = m_player->GetPosition();
	playerPosition.y += 100.0f;
	g_camera3D->SetTarget(playerPosition);
	g_camera3D->SetPosition(playerPosition + m_toPos);

}