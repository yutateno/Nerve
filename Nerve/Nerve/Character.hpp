#pragma once
#include <string>
#include "Input.hpp"

enum class MOVE_DIRE { leftDown = 0, down, rightDown, left, right, leftUp, up, rightUp };
enum class DIRECTION { left = 0, down, right, up };


class Character
{
private:
	/// �X�v���C�g�̕`��̂��߂�ID
	int m_ID;

	/// �X�v���C�g�̍ő吔
	const int m_spriteNum = 24;

	/// ���W
	float m_x, m_y;

	/// �X�v���C�g�̃R�}����z��p�� -1
	const int m_frame = 3;

	/// �R�}����
	void FrameSprite(MOVE_DIRE direction);

	/// �����̕���
	bool m_direction[4];

	/// �R�}����ҋ@
	int m_frameWait;
	/// �R�}����ҋ@����
	const int m_frameWaitTimer = 500;


public:
	Character();
	~Character();

	void Process();


	int GetID();
	float GetX();
	float GetY();
};

