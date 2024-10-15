#pragma once
#include "Core/Chess_pch.h"
#include "Core/ApplicationData.h"
#include "Core/TextureEnum.h"
#include "gpu-side/ShaderClass.h"

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
        void BindTextures()const;
        const std::array<SamplerBindingPoint,kShaderSamplerArrayBaseSize>& GetBoundTexturesSlots()const { return m_BindingPointsArray; }
        size_t GetBoundTexturesCount() { return m_TextureToBindingArrayIndexHash.size();}
        void Flush();
        size_t PushTextureForRendering(GLuint texture_to_push);
    private:
        std::unordered_map<GLuint, BindingPointArrayIndex> m_TextureToBindingArrayIndexHash{};
        std::array<SamplerBindingPoint, kShaderSamplerArrayBaseSize> m_BindingPointsArray{};
        BindingPointArrayIndex m_CurrentFreeTextureSlotIndex = 0;
    };
}