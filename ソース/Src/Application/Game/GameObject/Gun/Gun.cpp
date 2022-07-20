#include "Gun.h"
#include "../../GameSystem.h"
#include"../Character/Player/Player.h"
#include "../Bullet/Bullet.h"
void Gun::Init()
{
	SetJson("Data/Json/Gun.json");
	m_modelWork.SetModel(GameResourceFactory.GetModelData(JsonS("model","gun")));	// �e���f���̐ݒ�
	m_crossHairTex = GameResourceFactory.GetTexture(JsonS("texture", "crossHair"));		// �N���X�w�A�摜�̐ݒ�
	m_crossHairPos = JsonV2("crossHairPos");															// �N���X�w�A�̍��W�Z�b�g
}

void Gun::Update()
{
	// ���C����Œe�ۂ̖ړI���W���擾
	SetBulletGoal();
	if (!m_canFiring)
	{
		m_canShotCount--;
		if (m_canShotCount <= 0)
		{
			m_canFiring = true;
		}
	}
}

void Gun::Draw2D()
{
	if (!m_crossHairTex) { return; }															// �����N���X�w�A�摜��null��������֐����I��
	std::shared_ptr<const KdCamera> spCamera = m_wpCamera.lock();								// KdCamera�^�̓��ꕨ�Ɍp�����FPS�J����

	SHADER->m_spriteShader.DrawTex(m_crossHairTex.get(), (int)m_crossHairPos.x, (int)m_crossHairPos.y );	// ���W���Z�b�g���ĕ`��
}

void Gun::ShotBullet()
{
	if (m_canFiring)
	{
		std::shared_ptr<Bullet> spBullet = std::make_shared<Bullet>();			// �e�쐬
		spBullet->SetRayHit(rayHit);
		spBullet->SetDir(m_mWorld.Translation(), m_rayHitPos, m_mWorld.Backward());

		spBullet->Init();

		const KdModelWork::Node* pNode = m_modelWork.FindNode("MuzzlePoint");	// ���f������m�[�h��T��

		if (pNode)
		{
			spBullet->SetWorldMatrix(pNode->m_worldTransform * m_mWorld);		// �}�Y���m�[�h�̍s��Ǝ����̍s����|�����킹�Βu����e�𔭎�
		}

		GameSystem::GetInstance().AddObject(spBullet);							// ���X�g�ɒǉ�
		m_canFiring = false;														// ��U���ĂȂ�����
		m_canShotCount = JsonI("shotInterval");
		// �T�E���h
		std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
		spSoundInstance = GameAudio.Play3D(JsonS("sound","shoot"), GetPos());
		spSoundInstance->SetVolume(JsonF("volume","shoot"));
	}
}

void Gun::SetBulletGoal()		// ���C����Œe���΂����W���擾
{
	std::shared_ptr<KdCamera> spCamera = m_wpCamera.lock();										// KdCamera�^�̓��ꕨ�Ɍp�����FPS�J����
	Math::Matrix cameraMatrix = spCamera->GetCameraMatrix();									// �J�����s����擾
	// �N���X�w�A��2D���W��3D���W�ɕϊ�
	Math::Vector3 worldCrossHairPos;															// �N���X�w�A��3D���W
	spCamera->ConvertScreenToWorldDetail(m_crossHairPos, worldCrossHairPos);
	// �N���X�w�A3D���W�ւ̕������擾
	Math::Vector3 bulletAdvanceDir;																// �J��������N���X�w�A3D���W�̕���
	bulletAdvanceDir = worldCrossHairPos - spCamera->GetCameraMatrix().Translation();			// �N���X�w�A����J�������W�܂ł̃x�N�g���擾
	bulletAdvanceDir.Normalize();																// ���K���ō��v�̒l��1�ɂ��ĕ��������擾�@
	// �I�u�W�F�N�g�m�[�h��T���A���C������s��
	const std::list<std::shared_ptr<GameObject>>& objects = GameSystem::GetInstance().GetObjects();		// �Q�[��object�̏Z���ꗗ
	for (const std::shared_ptr<GameObject>& spObject : objects)											// �Q�[��object�̐�����for�������
	{
		if (spObject->GetClassID() != GameObject::eEnemy &&
			spObject->GetClassID() != GameObject::eStage) { continue; }
		RayInfo rayInfo(cameraMatrix.Translation(), bulletAdvanceDir, 1000.0f);							// ���C�̔��ˈʒu�A�����A����
		BumpResult bumpResult;

		spObject->CheckCollisionRay(rayInfo, bumpResult);	// �����蔻��Ăяo��

		if (bumpResult.m_isHit)
		{
			m_rayHitPos = bumpResult.m_HitPos;				//�@���C�������������W��n��
			rayHit = true;
		}
		else
		{
			rayHit = false;
		}
	}
}