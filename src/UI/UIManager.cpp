#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "UIManager.h"

Chess_Game::UIManager::UIManager(Size2D window_size, std::shared_ptr<DrawableCreator>& drawable_creator):
    m_CurrentWindowSize(window_size),m_ApplicationDrawableCreator(drawable_creator)
{
    m_MousePickingFramebuffer = std::make_unique<IntFramebuffer>(window_size);

    for (size_t i = 0; i < kUIElementCount; i++)
    {
        m_IDQueue.push(i);
    }
    m_ToNDCMatrix = glm::ortho<float>(0, window_size.width, 0, window_size.height);
}

void Chess_Game::UIManager::RemoveWidget(ElementID widget_id)
{
    m_IDQueue.push(widget_id);
    for (size_t i =0; i < m_UIElements.size();i++)
    {
        if (m_UIElements[i].expired())
        {        
           m_UIElements.erase(m_UIElements.begin() + i);
           break;         
        }
    }
}

void Chess_Game::UIManager::DrawUI(std::shared_ptr<BatchRenderer> application_batch_renderer,
    AssetLoader& application_asset_loader)
{
    for (auto& weak_element : m_UIElements)
    {
        if (auto element = weak_element.lock())
        {
            auto drawable = element->GetDrawable().lock();
            application_batch_renderer->Push(drawable->GetDrawableID(),drawable->GetPosition(), drawable->GetScale(),
                drawable->GetColor(), application_asset_loader.GetTextureAsset(drawable->GetDrawableTextureName()));
        }
    }
    application_batch_renderer->DrawTextureQuadBatchToIndexBuffer(*m_MousePickingFramebuffer,m_ToNDCMatrix);
    application_batch_renderer->DrawTextureQuadBatch(m_ToNDCMatrix);
}

void Chess_Game::UIManager::PollUIInput(const MouseInput& application_input,
    std::shared_ptr<BatchRenderer> application_batch_renderer,const OrthoViewportHandler& test)
{
    if (application_input.IsMouseButtonPressed(MouseButton_kLeftMouseButton))
    {
        auto& mouse_pos_bottom_left = application_input.GetMousePositionBottomLeft();

        for (auto& weak_element : m_UIElements)
        {
            if (auto element = weak_element.lock())
            {            
                if (element->GetElementBoundingBox().IsInsideBox(
                    glm::vec2(mouse_pos_bottom_left.x, mouse_pos_bottom_left.y)))
                {
                    size_t drawable_id = 
                        m_MousePickingFramebuffer->GetPixelData(mouse_pos_bottom_left.x,
                        mouse_pos_bottom_left.y);

                    if(element->m_UIDrawable->GetDrawableID() == drawable_id)
                        element->OnWidgetPressed();
                }

            }
        }
    }

    //Check for input
    //If input is on UI element invoke the on click callback.
}

void Chess_Game::UIManager::OnWindowSizeChanged(const WindowResizeEvent& e)
{
    m_CurrentWindowSize = e.GetWindowSize();
    m_ToNDCMatrix = glm::ortho<float>(0, m_CurrentWindowSize.width, 0, m_CurrentWindowSize.height);
    m_MousePickingFramebuffer->ResizeFramebuffer(m_CurrentWindowSize);

    for (auto& weak_element : m_UIElements)
    {
        if (auto element = weak_element.lock())
        {
            element->UpdateWindowPosition(m_CurrentWindowSize);
        }
    }

}

void Chess_Game::UIManager::OnEvent(const Event& e)
{
    if (e.GetEventType() == EventType_kWindowResize)
    {
        OnWindowSizeChanged(dynamic_cast<const WindowResizeEvent&>(e));
    }
}
