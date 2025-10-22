#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	animationClips[enAnimClip_Idle].SetLoopFlag(true);
	animationClips[enAnimClip_Walk].Load("Assets/animData/walk.tka");
	animationClips[enAnimClip_Walk].SetLoopFlag(true);
	animationClips[enAnimClip_Jump].Load("Assets/animData/jump.tka");
	animationClips[enAnimClip_Jump].SetLoopFlag(false);
	animationClips[enAnimClip_Run].Load("Assets/animData/run.tka");
	animationClips[enAnimClip_Run].SetLoopFlag(true);

	m_modelRender.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimClip_Num, enModelUpAxisY);
	characterController.Init(25.0f, 75.0f, m_position);
}

Player::~Player()
{

}

void Player::Update()
{
	moveSpeed.x = 0.0f;

	moveSpeed.y -= 0.5f;

	moveSpeed.z = 0.0f;

	m_modelRender.Update();

	Move();

	Rotation();

	ManageState();

	ManageJumpState();

	PlayAnimation();

}

void Player::Rotation()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_rot.SetRotationYFromDirectionXZ(moveSpeed);
		//絵描きさんに回転を教える。
		m_modelRender.SetRotation(m_rot);
	}
}


void Player::Move()
{
	//xzの移動速度を0.0fにする
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;

	//左スティックの入力量を取得
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//Y方向には移動しない
	forward.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量と120.0fを乗算
	right *= stickL.x * 10.0f;
	forward *= stickL.y * 10.0f;

	//移動速度に上記で計算したベクトルを持ってくる
	moveSpeed += right + forward;

	if (g_pad[0]->IsPress(enButtonB))
	{
		moveSpeed.x *= 2;
		moveSpeed.z *= 2;//Bボタンが押されている時はプレイヤーステートを3(走る)に設定
	}

	//キャラコンで移動
	m_position = characterController.Execute(moveSpeed, 1.0f / 60.0f);

	m_modelRender.SetPosition(m_position);

	if (characterController.IsOnGround() == true)
	{
		moveSpeed.y = 0.0f;
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			if (JumpCount < MaxJumpCount)
				moveSpeed.y = 10.0f;
					JumpCount++;
			playerState = 1;
		}

		JumpCount = 0; // 地面にいる時はジャンプ回数をリセット
	}



	m_position = characterController.Execute(moveSpeed, 1.0f);
	if (characterController.IsOnGround() == true)
	{
		playerState = 0;

	}

	m_modelRender.SetPosition(m_position);
}
void Player::ManageState()
{
	//地面に付いていなかったら。
	if (characterController.IsOnGround() == false)
	{
		//ステートを1(ジャンプ中)にする。
		playerState = 1;
		//ここでManageStateの処理を終わらせる。
		return;
	}

	//地面に付いていたら。
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//ステートを2(歩き)にする。
		playerState = 2;

		if (g_pad[0]->IsPress(enButtonB))
		{
			playerState = 3; // プレイヤーステートを3(走る)に設定
		}
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else
	{
		//ステートを0(待機)にする。
		playerState = 0;
	}
}

void Player::ManageJumpState()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		if (JumpCount < MaxJumpCount)
			moveSpeed.y = 10.0f;
		JumpCount++;
		playerState = 1;
	}
}

void Player::PlayAnimation()
{
	switch (playerState) {
		//プレイヤーステートが0(待機)だったら。
	case 0:
		//待機アニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimClip_Idle);
		break;
		//プレイヤーステートが1(ジャンプ中)だったら。
	case 1:
		//ジャンプアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimClip_Jump);
		break;
		//プレイヤーステートが2(歩き)だったら。
	case 2:
		//歩きアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimClip_Walk);
		break;
	case 3:
		//走るアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimClip_Run);
		break;
	}
}

void Player::Render(RenderContext& renderContext)
{
	m_modelRender.Draw(renderContext);
}