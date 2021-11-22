#pragma once

class Player;
class Background;
class GameCamera;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
private:
	Player*						m_player = nullptr;
	Background*					m_background = nullptr;
	GameCamera*					m_gameCamera = nullptr;
};

