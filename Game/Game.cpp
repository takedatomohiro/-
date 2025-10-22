#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "enemy1.h"

Game::Game()
{
	m_player = NewGO<Player>(0, "player");
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	m_background = NewGO<BackGround>(0, "background");
	m_enemy1 = NewGO<enemy1>(0, "enemy1");
}

Game::~Game()
{
	
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{
}