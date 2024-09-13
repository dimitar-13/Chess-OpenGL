#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "UIManager.h"
Chess_Game::UIManager::UIManager(const std::shared_ptr<BatchRenderer>& global_batch_renderer):
    m_ApplicationBatchRenderer(global_batch_renderer)
{
    for (size_t i = 0; i < kUIElementCount; i++)
    {
        m_IDQueue.push(i);
    }

}

void Chess_Game::UIManager::Update(const glm::mat4& projection_test, AssetLoader& test_loader)
{
    DrawUI(projection_test, test_loader);
    PollUIInput();
}

void Chess_Game::UIManager::PollUIInput()
{
    //Check for input
    //If input is on UI element invoke the on click callback.
}

void Chess_Game::UIManager::DrawUI(const glm::mat4& projection_test, AssetLoader& test_loader)
{
    for (auto& weak_ptr : m_UIElements)
    {
        if (auto ptr = weak_ptr.lock())
        {
            if (auto drawable = ptr->GetDrawable().lock())
            {
                m_ApplicationBatchRenderer->Push(drawable->GetPosition(),
                    drawable->GetScale(), drawable->GetColor(),
                    test_loader.GetTextureAsset(drawable->GetDrawableTextureName()));
            }
        }
    }
    m_ApplicationBatchRenderer->DrawTextureQuadBatch(projection_test);

}
