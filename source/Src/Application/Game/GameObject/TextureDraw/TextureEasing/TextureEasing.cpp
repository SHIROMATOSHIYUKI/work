#include"TextureEasing.h"
#include"../../../../Utility/Easing/Easing.h"

void TextureEasing::Init(std::string name, float cvQty, std::string easingName, Math::Vector2 pos, Math::Rectangle rect, Math::Color color)
{
	m_spTex = GameResourceFactory.GetTexture(name);
	m_cvQty = cvQty;
	if (easingName == "InOutSine") { m_easingName = eInOutSine; }
	else if (easingName == "OutBounce") { m_easingName = eOutBounce; }
	m_pos = pos;
	m_rect = rect;
	m_color = color;
}

void TextureEasing::Update()
{
	if (m_spTex == nullptr) { return; }

	m_accum += m_cvQty;	// ’~Ï—p•Ï”‚É‘‰Á—Ê‚ð‘«‚µ‚±‚Þ

	switch (m_easingName)
	{
	case eInOutSine:m_alpha = easeInOutSine(m_accum, 1.0f);
		break;
	case eOutBounce:m_alpha = easeOutBounce(m_accum, 1.0f);
		break;
	}
	m_color = { m_color.R(),m_color.G(),m_color.B(),m_alpha };
}

void TextureEasing::Draw2D()
{
	if (m_spTex == nullptr) { return; }
	SHADER->m_spriteShader.DrawTex(m_spTex.get(),
		(int)m_pos.x, (int)m_pos.y, &m_rect, &m_color);
}
