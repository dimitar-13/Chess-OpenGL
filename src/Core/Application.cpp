#include "GPU-Side/ShaderClass.h"
#include "Application.h"
#include "Scene/DefaultChessGameScene.h"
#include "Core/SceneObject.h"
#include "Scene/MainMenuScene.h"
#include "TextFont.h"

static constexpr int kStartWindowWidth = 1000;
static constexpr int kStartWindowHeight = 1000;

Chess_Game::Application::Application():
    m_ApplicationProjection({ Viewport{0,0, kStartWindowWidth, kStartWindowHeight} })
{
    const char* kWindowTittle = "Chess";
    constexpr Size2D kDefaultWindowSize = { kStartWindowWidth,kStartWindowHeight };
    std::function<void(const Event&)>windowOnEventCallback = 
        std::bind(&Application::OnEvent, this, std::placeholders::_1);
   
    m_ApplicationWindow = std::make_unique<Window>(kDefaultWindowSize, kWindowTittle, windowOnEventCallback);

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
    glm::vec4 background_col = glm::vec4(0.824, 0.651, 0.475, 1.0);

    m_CurrentApplicationScene = std::make_shared<MainMenuScene>(this->weak_from_this());
    m_CurrentApplicationScene->InitScene();

    glEnable(GL_BLEND);  
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthFunc(GL_LEQUAL);

    while (m_isApplicationRunning) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glClearColor(background_col.r, background_col.g, background_col.b, background_col.a);

        m_CurrentApplicationScene->OnUpdate();
        m_CurrentApplicationScene->DrawScene(m_ApplicationBatchRenderer);
        m_ApplicationBatchRenderer->FlushTextureBatch();

        glClear(GL_DEPTH_BUFFER_BIT); ///< UI should be always on top to do that we can just clear the depth buffer.
        m_ApplicationUIManager->DrawUI(m_ApplicationBatchRenderer);
        m_ApplicationUIManager->PollUIInput(m_ApplicationMouseInput, m_ApplicationBatchRenderer,m_ApplicationProjection);

        m_ApplicationMouseInput.FlushInputPoll();

        m_ApplicationBatchRenderer->FlushTextureBatch();

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
    m_ApplicationUIManager = std::make_shared<UIManager>(current_window_size, m_ApplicationDrawableCreator);

    m_TextureAssetLoader = std::make_shared<AssetLoader>();
    m_ApplicationBatchRenderer = std::make_shared<BatchRenderer>(current_window_size, m_TextureAssetLoader);

    this->AddEventListener(m_ApplicationBatchRenderer->GetIDFramebuffer());
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

    for (const auto& weak_listener : m_RegisteredEventListeners)
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
