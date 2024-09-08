#include "TextureResource.h"
#include <cassert>
#include <array>
Chess_Game::TextureBatcher::TextureBatcher()
{
    for (size_t i = 0; i < m_BindingPointsArray.size(); i++)
    {
        m_BindingPointsArray[i] = i;
    }
}

void Chess_Game::TextureBatcher::BindTextures(ShaderClass& sampler_array_shader)const
{
    const char* sampler_array_uniform_name = "u_Textures";
    sampler_array_shader.UseProgram();

    for (const auto&[texture,array_index]:m_TextureToBindingArrayIndexHash)
    {
        glActiveTexture(GL_TEXTURE0 + m_BindingPointsArray[array_index]);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    sampler_array_shader.SetSampler2DArray(sampler_array_uniform_name, m_BindingPointsArray.data(),
        m_TextureToBindingArrayIndexHash.size());

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

size_t Chess_Game::TextureBatcher::PushTextureForRendering(Texture texture_to_push)
{
    if (static_cast<size_t>(m_CurrentFreeTextureSlotIndex + 1) >= kShaderSamplerArrayBaseSize)
        return 0;

    if (m_TextureToBindingArrayIndexHash.find(texture_to_push) != m_TextureToBindingArrayIndexHash.end())
        return m_TextureToBindingArrayIndexHash.at(texture_to_push);

    BindingPointArrayIndex binding_point = m_CurrentFreeTextureSlotIndex;

    m_TextureToBindingArrayIndexHash.emplace(texture_to_push, m_CurrentFreeTextureSlotIndex);

    m_CurrentFreeTextureSlotIndex++;

    return binding_point;
}
