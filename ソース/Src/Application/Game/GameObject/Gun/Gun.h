#pragma once

#include "../GameObject.h"
class FPSCamera;	

class Gun : public GameObject
{
public:
	Gun(){}
	~Gun() {}

	void SetCamera(std::weak_ptr<KdCamera> pCamera){ m_wpCamera = pCamera; }

	void Init()		override;					// ������
	void Update()	override;					// �X�V
	void Draw2D()	override;					// �N���X�w�A�`��

	const Math::Vector3 GetRayHitPos() const { return m_rayHitPos; }
	void SetWorld(Math::Matrix pos) { m_mWorld = pos; }
	void ShotBullet();							// �e�̔��˔���
private:
	// �e����
	bool			m_canFiring = true;			// �e�����˂ł��邩�ǂ���
	int				m_canShotCount = 0;

	// �e���΂����W���擾
	void SetBulletGoal();						// �e�̃S�[����ݒ�
	float			m_hitDistance = 0.0f;		// ���C��HIT�����ۂ̖{�̂Ƃ̋���
	Math::Vector3	m_rayHitPos;				// ���C�������������W
	Math::Vector3	m_rayDir;					// ���C���W�Ɍ������p�x
	bool rayHit = false;

	std::weak_ptr<KdCamera>		m_wpCamera;		// FPSCamera�̒���KdCamera�̕������󂯂Ƃ�
	std::shared_ptr<KdTexture>	m_crossHairTex;	// �X�R�[�v�p�e�N�X�`��
	Math::Vector2				m_crossHairPos;	// �N���X�w�A�̍��W
};