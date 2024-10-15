#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "UIManager.h"

glm::vec2 Chess_Game::UIManager::ConvertScreenToRootWindowPos(glm::vec2 screen_pos)
{
    glm::vec2 screen_to_root_window_pos(screen_pos.x, screen_pos.y);
    glm::vec2 half_window_size(m_CurrentWindowSize.width, m_CurrentWindowSize.height);
    half_window_size /= 2.f;

    screen_to_root_window_pos -= half_window_size;

    return screen_to_root_window_pos;
}

Chess_Game::UIManager::UIManager(Size2D window_size, std::shared_ptr<DrawableCreator>& drawable_creator):
    m_CurrentWindowSize(window_size),m_ApplicationDrawableCreator(drawable_creator)
{

    glm::vec2 half_win_size = { window_size.width, window_size.height };
    half_win_size /= 2.0f;

    m_ToNDCMatrix = glm::ortho<float>(-half_win_size.x, half_win_size.x,
        -half_win_size.y, half_win_size.y);


    m_DefaultTextFont =std::make_shared<TextFont>();
}

void Chess_Game::UIManager::CreateRootPanel()
{
    glm::vec2 half_win_size = { m_CurrentWindowSize.width, m_CurrentWindowSize.height };
    half_win_size /= 2.0f;

    Panel* instance = 
        new Panel(std::dynamic_pointer_cast<UIManager>(this->shared_from_this()),
            m_ApplicationDrawableCreator,
       glm::vec2(0), half_win_size);


    std::shared_ptr<Panel> result = std::shared_ptr<Panel>(instance);
    m_RootWindowPanel = result;
    m_RootWindowPanel->SetVisibility(false);

    m_UIElements.push_back(std::dynamic_pointer_cast<Element>(result));
}

void Chess_Game::UIManager::RemoveWidget()
{
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
            element->Draw(*application_batch_renderer);
        }
    }
    application_batch_renderer->DrawTextureQuadBatch(m_ToNDCMatrix,true);
    application_batch_renderer->DrawTextBatch(m_ToNDCMatrix);

}

void Chess_Game::UIManager::PollUIInput(const MouseInput& application_input,
    std::shared_ptr<BatchRenderer> application_batch_renderer,const OrthoViewportHandler& test)
{
    if (application_input.IsMouseButtonPressed(MouseButton_kLeftMouseButton))
    {
        auto& mouse_pos_bottom_left = application_input.GetMousePositionBottomLeft();
        glm::vec2 mouse_screen_pos(mouse_pos_bottom_left.x, mouse_pos_bottom_left.y);

        glm::vec2 screen_to_root_win_pos = ConvertScreenToRootWindowPos(mouse_screen_pos);

        for (auto& weak_element : m_UIElements)
        {
            if (auto element = weak_element.lock())
            {       
                if (!element->IsElementEnabled() || !element->IsElementVisible())
                    continue;

               if (element->GetElementBoundingBox().IsInsideBox(screen_to_root_win_pos))
               {       
                  size_t drawable_id = application_batch_renderer->GetIDFramebuffer()->GetPixelData(
                       mouse_pos_bottom_left.x, mouse_pos_bottom_left.y);

                  if(element->GetElementID() == drawable_id)
                      element->OnElementPressed();
               }

            }
        }
    }
}

void Chess_Game::UIManager::OnWindowSizeChanged(const WindowResizeEvent& e)
{
    m_CurrentWindowSize = e.GetWindowSize();
    glm::vec2 half_win_size = { m_CurrentWindowSize.width, m_CurrentWindowSize.height };
    half_win_size /= 2.0f;
    m_ToNDCMatrix = glm::ortho<float>(-half_win_size.x, half_win_size.x,
                                      -half_win_size.y, half_win_size.y);

    m_RootWindowPanel->ResizeElement(half_win_size);
}

void Chess_Game::UIManager::OnEvent(const Event& e)
{
    if (e.GetEventType() == EventType_kWindowResize)
    {
        OnWindowSizeChanged(dynamic_cast<const WindowResizeEvent&>(e));
    }
}
