#pragma once
#include "Core/Chess_pch.h"
#include "TextureEnum.h"
#include "Logging/Logger.h"
namespace Chess_Game
{
    class DrawableCreator;
    class Drawable
    {
    public:
        Drawable(size_t drawable_id,const std::shared_ptr<DrawableCreator>& drawable_creator):
            m_DrawableID(drawable_id), m_DrawableCreator(drawable_creator){}     
        ~Drawable();
        size_t GetDrawableID()const { return m_DrawableID; }
    public:
        glm::vec3 m_Position{};
        glm::vec3 m_Color = glm::vec3(1);
        glm::vec2 m_Scale = glm::vec2(30.0f);
        TextureName_ m_TextureName = TextureName_kWhiteTexture;
        bool m_IsEnabled = true;
    private:
        size_t m_DrawableID{};
        std::shared_ptr<DrawableCreator> m_DrawableCreator{};
    };




}