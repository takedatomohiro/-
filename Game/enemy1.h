#pragma once
class enemy1 : public IGameObject
{
	public:
	enemy1();
	~enemy1();
	void Update();
	void Render(RenderContext& rc);
	ModelRender m_modelRender;
	CharacterController characterController;
	Vector3 m_position;
	Vector3 moveSpeed;
	Quaternion m_rot;
	//const Vector3& GetPosition() const { return m_position; }
};

	