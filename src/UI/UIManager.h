#pragma once
#include "UIElement.h"
#include "Button.h"
#include "GPU-Side/BatchRenderer.h"
#include <type_traits>
#include <queue>
#include <iostream>
#include <assert.h>
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
        Size2D GetCurrentWindowSize()const { return m_CurrentWindowSize; }
        void RemoveWidget(ElementID widget_id);
        void DrawUI(std::shared_ptr<BatchRenderer> application_batch_renderer,
            AssetLoader& application_asset_loader);
        void PollUIInput(const MouseInput& application_input, const OrthoViewportHandler& test);
        template<typename T>
        std::shared_ptr<T> CreateUIElement(const Margin& element_margin,
            AnchorPoint_ element_margin_anchor_point = AnchorPoint_kMiddle,
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
    inline std::shared_ptr<T> UIManager::CreateUIElement(const Margin& element_margin,
        AnchorPoint_ element_margin_anchor_point,glm::vec2 element_scale)
    {
        static_assert(std::is_base_of_v<UIElement, T> == true && "Call must inherit from 'UIElement'.");
        if (m_IDQueue.size() == 0)
        {
            assert(!(m_IDQueue.size() == 0) && "Max UI count reached.");
            return std::shared_ptr<T>();
        }

        ElementID id = m_IDQueue.front();
        m_IDQueue.pop();
        T* instance = new T(id,std::dynamic_pointer_cast<UIManager>(this->shared_from_this()),
            element_margin, element_margin_anchor_point, m_CurrentWindowSize, element_scale);

        std::shared_ptr<T> result = std::shared_ptr<T>(instance);
        m_UIElements.push_back(std::dynamic_pointer_cast<UIElement>(result));
        return result;
    }
}