#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Component.h"
#include "GameObject.h"

namespace dae
{
    TextComponent::TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font)
        : Component(owner), m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_textTexture(nullptr)
    {
    }

    void TextComponent::Update(float)
    {
        if (m_needsUpdate)
        {
            const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_color);
            if (surf == nullptr)
            {
                throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
            }

            auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
            if (texture == nullptr)
            {
                throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
            }

            SDL_FreeSurface(surf);
            m_textTexture = std::make_shared<Texture2D>(texture);
            m_needsUpdate = false;
        }
    }

    void TextComponent::Render() const
    {
        if (m_textTexture != nullptr)
        {
            if (auto owner = GetOwner())
            {
                const auto pos = owner->GetWorldPosition();
                Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y); 
            }
        }
    }

    void TextComponent::SetText(const std::string& text)
    {
        m_text = text;
        m_needsUpdate = true;
    }

    void TextComponent::SetColor(const SDL_Color& color)
    {
        m_color = color;
        m_needsUpdate = true;
	}
}
