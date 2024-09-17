#pragma once
#include "Core/Chess_pch.h"
#include "TextureEnum.h"
#include "Logging/Logger.h"
namespace Chess_Game
{
    class DrawableCreator;
    class Drawable
    {
        friend class DrawableCreator;
    private:
        Drawable(size_t drawable_id,const std::shared_ptr<DrawableCreator>& drawable_creator):
            m_DrawableID(drawable_id), m_DrawableCreator(drawable_creator)
        {}     
    public:
        ~Drawable();
        glm::vec3 GetPosition()const { return m_Position; }
        const glm::vec3& GetColor()const { return m_Color; }
        glm::vec2 GetScale()const { return m_Scale; }
        size_t GetDrawableID()const { return m_DrawableID; }
        void SetPosition(const glm::vec3& new_position) { m_Position = new_position; }
        void SetScale(const glm::vec2& new_scale) { m_Scale = new_scale; }
        void SetColor(const glm::vec3& color) { m_Color = color; }
        TextureName_ GetDrawableTextureName()const { return m_TextureName; }
        void SetDrawableTextureName(TextureName_ new_texture_name) { m_TextureName = new_texture_name; }
        void EnableDrawable(bool is_enabled) { m_IsEnabled = is_enabled; }
        bool IsDrawableEnabled()const { return m_IsEnabled; }
    private:
        size_t m_DrawableID{};
        glm::vec3 m_Position{};
        glm::vec3 m_Color = glm::vec3(1);
        glm::vec2 m_Scale = glm::vec2(30.0f);
        TextureName_ m_TextureName = TextureName_kWhiteTexture;
        std::shared_ptr<DrawableCreator> m_DrawableCreator{};
        bool m_IsEnabled = true;
    };




}