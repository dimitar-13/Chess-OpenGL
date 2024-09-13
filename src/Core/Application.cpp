#include "GPU-Side/ShaderClass.h"
#include "Application.h"
#include "Scene/DefaultChessGameScene.h"
#include "Core/SceneObject.h"
#include "Scene/MainMenuScene.h"
Chess_Game::Application::Application():
    m_ApplicationProjection({ Viewport{0,0, 1000, 1000} })
{
    constexpr int kStartWindowWidth = 1000;
    constexpr int kStartWindowHeight = 1000;
    const char* kWindowTittle = "Chess";

    Chess_Game::WindowCreateInfo window_create_info{};
    window_create_info.windowHeight = kStartWindowHeight;
    window_create_info.windowWidth = kStartWindowWidth;
    window_create_info.windowTittle = kWindowTittle;
    window_create_info.windowOnEventCallback = std::bind(&Application::OnEvent, this, std::placeholders::_1);
   

    m_ApplicationWindow = std::make_unique<Window>(window_create_info);
    if (!m_ApplicationWindow->IsWindowValid())
        m_ApplicationInitStatus = false;

}

void Chess_Game::Application::RenderLoop()
{
    m_TextureAssetLoader = std::make_unique<AssetLoader>();

    m_ApplicationBatchRenderer = std::make_shared<BatchRenderer>();
    m_ApplicationUIManager = std::make_shared<UIManager>(m_ApplicationBatchRenderer);

    //m_CurrentApplicationScene = std::make_shared<DefaultChessScene>(this->weak_from_this());
    m_CurrentApplicationScene = std::make_shared<MainMenuScene>(this->weak_from_this());

    m_CurrentApplicationScene->InitScene();


   // auto button_test = m_ApplicationUIManager->CreateUIElement<Button>(TextureName_kButton,
   //     glm::vec3(0),glm::vec2(60));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (m_isApplicationRunning) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glClearColor(1.f, 0.f, 0.f, 1.0f);

        m_CurrentApplicationScene->OnUpdate();
        m_CurrentApplicationScene->DrawScene(m_ApplicationBatchRenderer);

        m_ApplicationUIManager->Update(m_ApplicationProjection.GetMatrix(),*m_TextureAssetLoader);

        m_ApplicationWindow->OnUpdate();
    }

}

void Chess_Game::Application::OnEvent(const Event& e)
{
    if(e.GetEventType() == EventType_kWindowClosed)
        m_isApplicationRunning = false;
    else if (e.GetEventType() == EventType_kWindowResize)
    {
        OnWindowResizeEvent(dynamic_cast<const WindowResizeEvent&>(e));
    }
  
    dynamic_cast<Listener&>(m_ApplicationMouseInput).OnEvent(e);

    for (const auto& weak_listener : m_ActiveEventListeners)
    {
        if (auto& listener = weak_listener.lock())
        {
            listener->OnEvent(e);
        }
    }
}

void Chess_Game::Application::OnWindowResizeEvent(const WindowResizeEvent& e)
{
    Size2D new_window_size = e.GetWindowSize();

    m_ApplicationProjection.UpdateViewport(Viewport{ 0,0, static_cast<float>(new_window_size.width),
        static_cast<float>(new_window_size.height)});

}
