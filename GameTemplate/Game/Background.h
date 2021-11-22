#pragma once
class Background : public IGameObject
{
public:
	Background();
	~Background();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	ModelRender	m_modelRender;
	PhysicsStaticObject m_physicsStaticObject;
};

