#pragma once
#include "Core/Chess_pch.h"
#include "ShaderClass.h"
namespace Chess_Game
{
    struct PipelineCreateInfo
    {

    };

    struct Pipeline
    {
        size_t pipeline_ID{};
        std::shared_ptr<ShaderClass> pipeline_shader{};
        bool enable_blending = false;
        bool enable_mouse_picking = false;
        bool enable_depth_testing = true;
    };


    class PipelineManager
    {
    public:
        PipelineManager();
        ~PipelineManager();
        size_t CreatePipeline();
        const Pipeline& GetPipeline(size_t pipeline_id);
    private:
        std::unordered_map<size_t, Pipeline> m_PipelineHash{};
    };

}