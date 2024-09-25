#include "GPU-Side/ShaderClass.h"
#include "Application.h"
#include "Scene/DefaultChessGameScene.h"
#include "Core/SceneObject.h"
#include "Scene/MainMenuScene.h"

static constexpr int kStartWindowWidth = 1000;
static constexpr int kStartWindowHeight = 1000;

Chess_Game::Application::Application():
    m_ApplicationProjection({ Viewport{0,0, kStartWindowWidth, kStartWindowHeight} })
{
    const char* kWindowTittle = "Chess";

    Chess_Game::WindowCreateInfo window_create_info{};
    window_create_info.windowHeight = kStartWindowHeight;
    window_create_info.windowWidth = kStartWindowWidth;
    window_create_info.windowTittle = kWindowTittle;
    window_create_info.windowOnEventCallback = std::bind(&Application::OnEvent, this, std::placeholders::_1);
   
    m_ApplicationWindow = std::make_unique<Window>(window_create_info);

    if (!m_ApplicationWindow->IsWindowValid())
        return;

    if (glewInit() != GLEW_OK) {
        CHESS_LOG_FATAL("[GLEW] Failed to initialize GLEW");
        return;
    }
    glewExperimental = GL_TRUE;

    InitAppResource();
    m_ApplicationInitStatus = true;
}

void Chess_Game::Application::StartRenderLoop()
{
    m_CurrentApplicationScene = std::make_shared<MainMenuScene>(this->weak_from_this());
    m_CurrentApplicationScene->InitScene();

    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    while (m_isApplicationRunning) {

        glClear(GL_COLOR_BUFFER_BIT);

        glClearColor(1.f, 0.f, 0.f, 0.0f);

        m_CurrentApplicationScene->OnUpdate();
        m_CurrentApplicationScene->DrawScene(m_ApplicationBatchRenderer);

        m_ApplicationUIManager->DrawUI(m_ApplicationBatchRenderer,*m_TextureAssetLoader);
        m_ApplicationUIManager->PollUIInput(m_ApplicationMouseInput, m_ApplicationBatchRenderer,m_ApplicationProjection);

        m_ApplicationMouseInput.FlushInputPoll();

        m_ApplicationWindow->OnUpdate();

        if (m_ToLoadScene != nullptr && m_CurrentApplicationScene != m_ToLoadScene)
        {
            m_CurrentApplicationScene->DestroyScene();
            m_CurrentApplicationScene = m_ToLoadScene;
            m_ToLoadScene = nullptr;
        
            m_CurrentApplicationScene->InitScene();
        }
    }   

}

void Chess_Game::Application::InitAppResource()
{
    Size2D current_window_size = m_ApplicationWindow->GetWindowSize();

    m_ApplicationDrawableCreator = std::make_shared<DrawableCreator>();
    m_TextureAssetLoader = std::make_shared<AssetLoader>();
    m_ApplicationBatchRenderer = std::make_shared<BatchRenderer>(current_window_size, m_TextureAssetLoader);
    this->AddEventListener(m_ApplicationBatchRenderer->GetIDFramebuffer());

    m_ApplicationUIManager = std::make_shared<UIManager>(current_window_size, m_ApplicationDrawableCreator);

    this->AddEventListener(m_ApplicationUIManager);
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
