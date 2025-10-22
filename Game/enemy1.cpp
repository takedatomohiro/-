#include "stdafx.h"
#include "enemy1.h"
#include "player.h"

enemy1::enemy1()
{
	// �����ʒu�Ȃ�
	m_position = Vector3(0.0f, 0.0f, 250.0f);
	moveSpeed = Vector3::Zero;
	m_rot = Quaternion::Identity;

	m_modelRender.Init("Assets/stage/red.tkm");
    m_modelRender.SetScale({ 10.0f,5.0f,10.0f });

	// �L�����N�^�[�R���g���[���[������
	characterController.Init(1.0f, 2.0f, m_position); // ���a�A�����ȂǓK�X
}

enemy1::~enemy1()
{
    //m_modelRender.Release();
}

void enemy1::Update()
{
    // ========== �ÓI�ϐ� ==========
    static int moveDir = 1;                // 1:�E�A-1:��
    static float patrolSpeed = 100.0f;       // �ʏ�ړ����x�i�������j
    static float chaseSpeed = 265.0f;        // �ǐՎ��̑��x�i���߁j
    static float moveRange = 300.0f;       // ���E�ړ��͈�
    static Vector3 startPos = m_position;  // �����ʒu��ۑ�

    static bool isChasing = false;         // �ǐՒ���
    float chaseRange = 450.0f;             // �ǐՊJ�n����
    float chaseStopRange = 650.0f;         // �ǐՉ�������

    // ========== �v���C���[�擾 ==========
    Player* player = FindGO<Player>("player");
    if (player == nullptr) {
        return; // �v���C���[���܂����݂��Ȃ��Ȃ牽�����Ȃ�
    }

    Vector3 playerPos = player->GetPosition();

    // ========== �v���C���[�Ƃ̋��� ==========
    Vector3 toPlayer = playerPos - m_position;
    float distance = toPlayer.Length();

    // ========== ��Ԑ؂�ւ� ==========
    if (distance < chaseRange) {
        isChasing = true;
    }
    else if (distance > chaseStopRange) {
        isChasing = false;
    }

    // ========== �ړ��v�Z ==========
    moveSpeed = Vector3::Zero;

    if (isChasing)
    {
        // --- �v���C���[�ǐ� ---
        toPlayer.Normalize();
        moveSpeed.x = toPlayer.x * chaseSpeed; // X�����̂ݒǐ�
        moveSpeed.z = toPlayer.z * chaseSpeed;

        // --- �v���C���[�����ɉ�] ---
        float angleY = atan2f(toPlayer.x, toPlayer.z); // Y����]�p�����߂�
        m_rot.SetRotation(Vector3(0.0f, 1.0f, 0.0f), angleY);
    }
    else
    {
        // --- �ʏ�p�g���[���i���E�����j ---
        moveSpeed.x = patrolSpeed * moveDir;

        // �͈͊O�ɍs�����甽�]
        if (m_position.x > startPos.x + moveRange) {
            moveDir = -1;
        }
        else if (m_position.x < startPos.x - moveRange) {
            moveDir = 1;
        }

        // ���E�̌����𔽉f�i�E����0��, ������180���j
        if (moveDir > 0)
            m_rot.SetRotationDeg(Vector3(0.0f, 1.0f, 0.0f), 0.0f);
        else
            m_rot.SetRotationDeg(Vector3(0.0f, 1.0f, 0.0f), 180.0f);
    }

    // ========== ���ۂ̈ړ� ==========
    m_position = characterController.Execute(moveSpeed, 1.0f / 60.0f);

    // ========== ���f���ʒu�X�V ==========
    m_modelRender.SetPosition(m_position);
    m_modelRender.SetRotation(m_rot);
    m_modelRender.Update();
}

void enemy1::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
