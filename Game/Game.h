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
	SkyCube* m_skyCube = nullptr;			//空。
	Player* m_player = nullptr;			//プレイヤー。
	SoundSource* m_gameBGM = nullptr; // ゲーム中のBGM。
	GameCamera* m_gamecamera = nullptr;	//ゲームカメラ。
	BackGround* m_background = nullptr;	//背景。
	enemy1* m_enemy1 = nullptr;			//敵1。
};


