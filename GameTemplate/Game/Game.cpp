#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "Background.h"
#include "GameCamera.h"

Game::Game()
{

}

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_background);
}

bool Game::Start()
{
	m_player = NewGO<Player>(0,"player");
	m_background = NewGO<Background>(0, "background");
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	return true;
}

void Game::Update()
{
	
}