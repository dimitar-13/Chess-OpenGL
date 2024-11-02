#include "DrawableCreator.h"

Chess_Game::DrawableCreator::DrawableCreator()
{
    for (size_t i = 1; i < DrawableCreator::kMaxDrawableCount +1; i++)
    {
        m_DrawableIndexPool.push(i);
    }
    m_DrawablesHash.reserve(DrawableCreator::kMaxDrawableCount);

    m_DrawableMemoryPool = 
        std::make_shared<MemoryPool<Drawable, kMaxDrawableCount>>();
}

std::shared_ptr<Chess_Game::Drawable> Chess_Game::DrawableCreator::CreateDrawable()
{
    if(m_DrawableIndexPool.empty())
    {
        assert(true && "Cant create more drawables.");
        return std::shared_ptr<Chess_Game::Drawable>();
    }

    size_t drawable_id = m_DrawableIndexPool.front();
    m_DrawableIndexPool.pop();

    std::shared_ptr<Drawable> result =
        m_DrawableMemoryPool->AllocateShared(drawable_id, shared_from_this());

    m_DrawablesHash.emplace(drawable_id,result);
    return result;
}

void Chess_Game::DrawableCreator::DeleteDrawable(size_t drawable_id)
{
    if (m_DrawablesHash.find(drawable_id) != m_DrawablesHash.end())
    {
        m_DrawablesHash.erase(drawable_id);
    }
    m_DrawableIndexPool.push(drawable_id);
}
