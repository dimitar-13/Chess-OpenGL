#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "DrawableCreator.h"

Chess_Game::DrawableCreator::DrawableCreator()
{
    for (size_t i = 1; i < DrawableCreator::kMaxDrawableCount+1; i++)
    {
        m_DrawableIndexPool.push(i);
    }
}

std::shared_ptr<Chess_Game::Drawable> Chess_Game::DrawableCreator::CreateDrawable()
{
    size_t drawable_id = m_DrawableIndexPool.front();
    m_DrawableIndexPool.pop();

    Drawable* drawable = new Drawable(drawable_id,shared_from_this());
    std::shared_ptr<Drawable> result(drawable);
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
