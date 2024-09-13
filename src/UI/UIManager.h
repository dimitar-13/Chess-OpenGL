#pragma once
#include "UIElement.h"
#include "Button.h"
#include "GPU-Side/BatchRenderer.h"
#include <type_traits>
#include <queue>
#include <iostream>
#include "Core/AssetLoader.h"
#include "Core/Input.h"
#include "Core/OrthographicApplicationMatrix.h"

namespace Chess_Game
{
    class UIManager : public Listener
    {
    private:
        using ElementID = size_t;
        static constexpr size_t kUIElementCount = 10;
    public:
        UIManager(Size2D window_size);
        void DrawUI(std::shared_ptr<BatchRenderer> application_batch_renderer,
            AssetLoader& application_asset_loader);
        void PollUIInput(const MouseInput& application_input, const OrthoViewportHandler& test);
        template<typename T>
        std::shared_ptr<T> CreateUIElement(const Margin& element_margin,
            glm::vec2 element_scale = glm::vec2(1));
    private:
        void OnWindowSizeChanged(const WindowResizeEvent& e);
        void OnEvent(const Event& e) override;
    private:
        std::queue<ElementID> m_IDQueue{};
        std::vector<std::weak_ptr<UIElement>> m_UIElements{};
        Size2D m_CurrentWindowSize{};
        glm::mat4 m_ToNDCMatrix{};
    };

    template<typename T>
    inline std::shared_ptr<T> UIManager::CreateUIElement(const Margin& element_margin,glm::vec2 element_scale)
    {
        static_assert(std::is_base_of_v<UIElement, T> == true && "Call must inherit from 'UIElement'.");
        ElementID id = m_IDQueue.front();
        m_IDQueue.pop();
        T* instance = new T(id, element_margin, m_CurrentWindowSize, element_scale);
        std::shared_ptr<T> result = std::shared_ptr<T>(instance);
        m_UIElements.push_back(std::dynamic_pointer_cast<UIElement>(result));
        return result;
    }
}