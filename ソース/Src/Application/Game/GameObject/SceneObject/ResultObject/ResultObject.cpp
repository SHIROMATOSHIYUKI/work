#include "ResultObject.h"
#include"../../TextureDraw/TextureDraw.h"
#include"../../TextureDraw/TextureEasing/TextureEasing.h"
#include"../../TextureDraw/FadeInOut/FadeInOut.h"
void ResultObject::Init()
{
	SetJson("Data/Json/SceneObject/Result.json");
	// 2D�摜�`��N���X
	std::shared_ptr<TextureDraw> ResultTex = std::make_shared<TextureDraw>();
	ResultTex->Init(JsonS("texture", "result"));
	GameSystem::GetInstance().AddObject(ResultTex);

	// �C�[�W���O�摜�`��N���X
	std::shared_ptr<TextureEasing> GuideTex = std::make_shared<TextureEasing>();
	GuideTex->Init(JsonS("texture", "guide"), JsonF("easingSpeed"), "InOutSine", JsonV2("easingPos"), JsonR("easingRect"));
	GameSystem::GetInstance().AddObject(GuideTex);

	// �t�F�[�h�C���N���X�Ăяo��
	std::shared_ptr<FadeInOut> fadeIn = std::make_shared<FadeInOut>();
	fadeIn->Init( true, JsonF("fadeInSpeed"));
	GameSystem::GetInstance().AddObject(fadeIn);

	// BGM�̍Đ�
	std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
	spSoundInstance = GameAudio.Play(JsonS("sound", "result"));
	spSoundInstance->SetVolume(JsonF("volume", "result"));
}

void ResultObject::Update()
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		// �t�F�[�h�A�E�g�J�n
		fadeOut = std::make_shared<FadeInOut>();
		fadeOut->Init(false, JsonF("fadeOutSpeed"));
		GameSystem::GetInstance().AddObject(fadeOut);
	}
	if (fadeOut) {
		if (fadeOut->GetEndFlg()) { GameInstance.RequestCangeScene("Title"); }
	}
}