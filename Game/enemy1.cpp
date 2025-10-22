#include "stdafx.h"
#include "enemy1.h"
#include "player.h"

enemy1::enemy1()
{
	// 初期位置など
	m_position = Vector3(0.0f, 0.0f, 250.0f);
	moveSpeed = Vector3::Zero;
	m_rot = Quaternion::Identity;

	m_modelRender.Init("Assets/stage/red.tkm");
    m_modelRender.SetScale({ 10.0f,5.0f,10.0f });

	// キャラクターコントローラー初期化
	characterController.Init(1.0f, 2.0f, m_position); // 半径、高さなど適宜
}

enemy1::~enemy1()
{
    //m_modelRender.Release();
}

void enemy1::Update()
{
    // ========== 静的変数 ==========
    static int moveDir = 1;                // 1:右、-1:左
    static float patrolSpeed = 100.0f;       // 通常移動速度（ゆっくり）
    static float chaseSpeed = 265.0f;        // 追跡時の速度（速め）
    static float moveRange = 300.0f;       // 左右移動範囲
    static Vector3 startPos = m_position;  // 初期位置を保存

    static bool isChasing = false;         // 追跡中か
    float chaseRange = 450.0f;             // 追跡開始距離
    float chaseStopRange = 650.0f;         // 追跡解除距離

    // ========== プレイヤー取得 ==========
    Player* player = FindGO<Player>("player");
    if (player == nullptr) {
        return; // プレイヤーがまだ存在しないなら何もしない
    }

    Vector3 playerPos = player->GetPosition();

    // ========== プレイヤーとの距離 ==========
    Vector3 toPlayer = playerPos - m_position;
    float distance = toPlayer.Length();

    // ========== 状態切り替え ==========
    if (distance < chaseRange) {
        isChasing = true;
    }
    else if (distance > chaseStopRange) {
        isChasing = false;
    }

    // ========== 移動計算 ==========
    moveSpeed = Vector3::Zero;

    if (isChasing)
    {
        // --- プレイヤー追跡 ---
        toPlayer.Normalize();
        moveSpeed.x = toPlayer.x * chaseSpeed; // X方向のみ追跡
        moveSpeed.z = toPlayer.z * chaseSpeed;

        // --- プレイヤー方向に回転 ---
        float angleY = atan2f(toPlayer.x, toPlayer.z); // Y軸回転角を求める
        m_rot.SetRotation(Vector3(0.0f, 1.0f, 0.0f), angleY);
    }
    else
    {
        // --- 通常パトロール（左右往復） ---
        moveSpeed.x = patrolSpeed * moveDir;

        // 範囲外に行ったら反転
        if (m_position.x > startPos.x + moveRange) {
            moveDir = -1;
        }
        else if (m_position.x < startPos.x - moveRange) {
            moveDir = 1;
        }

        // 左右の向きを反映（右向き0°, 左向き180°）
        if (moveDir > 0)
            m_rot.SetRotationDeg(Vector3(0.0f, 1.0f, 0.0f), 0.0f);
        else
            m_rot.SetRotationDeg(Vector3(0.0f, 1.0f, 0.0f), 180.0f);
    }

    // ========== 実際の移動 ==========
    m_position = characterController.Execute(moveSpeed, 1.0f / 60.0f);

    // ========== モデル位置更新 ==========
    m_modelRender.SetPosition(m_position);
    m_modelRender.SetRotation(m_rot);
    m_modelRender.Update();
}

void enemy1::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
