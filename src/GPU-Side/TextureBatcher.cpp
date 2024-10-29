#include "TextureBatcher.h"

Chess_Game::TextureBatcher::TextureBatcher()
{
    for (size_t i = 0; i < m_BindingPointsArray.size(); i++)
    {
        m_BindingPointsArray[i] = i;
    }
}

void Chess_Game::TextureBatcher::BindTextures()const
{
    for (const auto&[texture,array_index]:m_TextureToBindingArrayIndexHash)
    {
        glActiveTexture(GL_TEXTURE0 + m_BindingPointsArray[array_index]);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}

void Chess_Game::TextureBatcher::Flush()
{
    //for (const auto& [texture, array_index] : m_TextureToBindingArrayIndexHash)
    //{
    //    glActiveTexture(GL_TEXTURE0 + m_BindingPointsArray[array_index]);
    //    glBindTexture(GL_TEXTURE_2D, 0);
    //}

    m_TextureToBindingArrayIndexHash.clear();
    m_CurrentFreeTextureSlotIndex = 0;
}

size_t Chess_Game::TextureBatcher::PushTextureForRendering(GLuint texture_to_push)
{
    if (static_cast<size_t>(m_CurrentFreeTextureSlotIndex + 1) >= kShaderSamplerArrayBaseSize)
    {
        assert(true && "No more space in the batch renderer.");
        return 0;
    }

    if (m_TextureToBindingArrayIndexHash.find(texture_to_push) != m_TextureToBindingArrayIndexHash.end())
        return m_TextureToBindingArrayIndexHash.at(texture_to_push);

    size_t binding_point = m_CurrentFreeTextureSlotIndex;

    m_TextureToBindingArrayIndexHash.emplace(texture_to_push, m_CurrentFreeTextureSlotIndex);

    m_CurrentFreeTextureSlotIndex++;

    return binding_point;
}
