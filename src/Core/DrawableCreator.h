#pragma once
#include "Chess_pch.h"
#include "Drawable.h"
#include <queue>
#include "MemoryPool.h"
namespace Chess_Game
{
    class DrawableCreator : public std::enable_shared_from_this<DrawableCreator>
    {
    private:
        static const size_t kMaxDrawableCount = 100;
    public:
        DrawableCreator();
        std::shared_ptr<Drawable> CreateDrawable();
        void DeleteDrawable(size_t drawable_id);
    private:
        std::queue<size_t> m_DrawableIndexPool{};
        std::unordered_map<size_t,std::weak_ptr<Drawable>> m_DrawablesHash{};
        std::shared_ptr<MemoryPool<Drawable, kMaxDrawableCount>> m_DrawableMemoryPool;
    };
}