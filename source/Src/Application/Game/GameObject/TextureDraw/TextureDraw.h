#pragma once
#include"../GameObject.h"
// ‰æ‘œ‚ð•`‰æ‚·‚é‚¾‚¯‚ÌƒNƒ‰ƒX
class TextureDraw :public GameObject
{
public:
	TextureDraw(){}
	~TextureDraw() {}

	void Init(
		std::string name, 
		Math::Vector2 pos = {0,0}, 
		Math::Rectangle rect = { 0,0,1280,720 }, 
		Math::Color color = { 1,1,1,1 });
	void Draw2D() override;
protected:
	std::shared_ptr<KdTexture> m_spTex;
	Math::Rectangle m_rect;
	Math::Vector2 m_pos;
	Math::Color m_color;
};