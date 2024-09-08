#pragma once
#include "Core/ApplicationData.h"
#include <GL/glew.h>
#include <vector>
#include "Core/TextureEnum.h"
#include "gpu-side/ShaderClass.h"
#include <array>
namespace Chess_Game
{
    typedef int SamplerBindingPoint;
    typedef unsigned int BindingPointArrayIndex;


    class TextureBatcher
    {
    private:
        constexpr static size_t kShaderSamplerArrayBaseSize = 10;
    public:
        TextureBatcher();
        void BindTextures(ShaderClass& sampler_array_shader)const;
        void Flush();
        size_t PushTextureForRendering(Texture texture_to_push);
    private:
        std::unordered_map<Texture, BindingPointArrayIndex> m_TextureToBindingArrayIndexHash{};
        std::array<SamplerBindingPoint, kShaderSamplerArrayBaseSize> m_BindingPointsArray{};
        BindingPointArrayIndex m_CurrentFreeTextureSlotIndex = 0;
    };
}