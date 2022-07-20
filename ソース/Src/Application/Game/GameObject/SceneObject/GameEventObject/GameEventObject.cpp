#include"GameEventObject.h"
#include"../../TextureDraw/TextureDraw.h"
#include"../../TextureDraw/FadeInOut/FadeInOut.h"
#include"../../Manager/EnemyManager/EnemyManager.h"
#include"../../Character/Player/Player.h"

void GameEventObject::Init()
{
	SetJson("Data/Json/SceneObject/Game.json");
	// �t�F�[�h�C���N���X�Ăяo��
	std::shared_ptr<FadeInOut> fadeIn = std::make_shared<FadeInOut>();
	fadeIn->Init(true, JsonF("fadeInSpeed"));
	GameSystem::GetInstance().AddObject(fadeIn);
}

void GameEventObject::Update()
{
	// �N���A������B��������
	if (GameEnemyManeger.get()->lastEnemyIsAlive())
	{
		ClearUpdate();
	}
	// �v���C���[�����S������
	if (GameInstance.GetPlayer().lock()->DeathFlg())
	{
		DeathUpdate();
	}
}


void GameEventObject::ClearUpdate()
{
	if (m_clearFirst)
	{
		m_fadeOutTex = std::make_shared<FadeInOut>();
		m_fadeOutTex->StopFadeOutInit(JsonF("firstFadeOut", "fadeSpeed"), JsonF("firstFadeOut", "stopAlpha"), JsonI("firstFadeOut", "stopTime"));
		GameSystem::GetInstance().AddObject(m_fadeOutTex);
		m_clearFirst = false;	// ��x�����ʂ�
	}

	if (!m_fadeOutTex) { return; }

	if (m_fadeOutTex->GetEndFlg()) {
		// �J�E���g�����߂�
		if (m_halfCnt < JsonI("harfProc","halfCntMax")) { m_halfCnt++; }

		// �ŏ��Ɉ�x�����ʂ�
		if (m_harfFirst) {
			// ��
			std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
			spSoundInstance = GameAudio.Play(JsonS("sound","clear"));
			spSoundInstance->SetVolume(JsonF("volume","clear"));
			// �摜
			std::shared_ptr<TextureDraw> CompleteTex = std::make_shared<TextureDraw>();
			CompleteTex->Init(JsonS("texture","complete"), JsonV2("harfProc","completePos"), JsonR("harfProc","completeRect"));
			GameSystem::GetInstance().AddObject(CompleteTex);
			m_harfFirst = false;
		}
	}

	// �J�E���g������������
	if (m_halfCnt >= JsonI("harfProc", "halfCntMax")) {
		// 2���ڂ̃t�F�[�h�A�E�g
		if (m_endOfCntFirst)
		{
			m_secondFadeOutTex = std::make_shared<FadeInOut>();
			m_secondFadeOutTex->Init(false, JsonF("secondFadeOut","fadeSpeed"));
			GameSystem::GetInstance().AddObject(m_secondFadeOutTex);
			m_endOfCntFirst = false;
		}
	}
	if (!m_secondFadeOutTex) { return; }

	// 2���ڂ̃t�F�[�h�A�E�g�I���Ń��U���g�ֈڍs
	if (m_secondFadeOutTex->GetEndFlg()) {
		GameInstance.RequestCangeScene("Result");
	}

}

void GameEventObject::DeathUpdate()
{
	// ���񂾂�Â����Ă���
	//darken(0.03f);
	if (m_deathFirst)
	{
		m_deathFadeOutTex = std::make_shared<FadeInOut>();
		m_deathFadeOutTex->Init(false,JsonF("deathUpdate","fadeOutSpeed"));
		GameSystem::GetInstance().AddObject(m_deathFadeOutTex);
		m_deathFirst = false;
	}
	// �������I�������Q�[���I�[�o�[�ֈړ�
	if (m_deathFadeOutTex->GetEndFlg())
	{
		GameInstance.RequestCangeScene("Death");
	}
}
