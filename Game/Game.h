#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class GameCamera;
class BackGround;
class enemy1;
class Game : public IGameObject
{
public:
	Game();
	~Game();

	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
	Vector3 m_pos;
	float m_timer = 60.0f;
	FontRender m_timeFontRender;
	FontRender m_distance;
	SkyCube* m_skyCube = nullptr;			//��B
	Player* m_player = nullptr;			//�v���C���[�B
	SoundSource* m_gameBGM = nullptr; // �Q�[������BGM�B
	GameCamera* m_gamecamera = nullptr;	//�Q�[���J�����B
	BackGround* m_background = nullptr;	//�w�i�B
	enemy1* m_enemy1 = nullptr;			//�G1�B
};


