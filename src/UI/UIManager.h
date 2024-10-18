#pragma once
#include "UIElement.h"
#include "Panel.h"
#include "Button.h"
#include "GPU-Side/BatchRenderer.h"
#include <type_traits>
#include <queue>
#include <iostream>
#include <assert.h>
#include "Core/AssetLoader.h"
#include "Core/Input.h"
#include "Core/OrthographicApplicationMatrix.h"
#include "GPU-Side/Framebuffer.h"
namespace Chess_Game
{
    class UIManager : public Listener
    {
    private:
        using ElementID = size_t;
        static constexpr size_t kUIElementCount = 40; /// This might be removed if not used.
    public:
        glm::vec2 ConvertScreenToRootWindowPos(glm::vec2 screen_pos);
        UIManager(Size2D window_size, std::shared_ptr<DrawableCreator>& drawable_creator);
        void RemoveWidget(size_t widget_drawable_id);
        void DrawUI(std::shared_ptr<BatchRenderer> application_batch_renderer,
            AssetLoader& application_asset_loader);
        void PollUIInput(const MouseInput& application_input,
            std::shared_ptr<BatchRenderer> application_batch_renderer ,const OrthoViewportHandler& test);
        template<typename T>
        std::shared_ptr<T> CreateUIElement(const glm::vec2 element_pos,
            const glm::vec2 element_size);
        std::shared_ptr<TextFont> GetDefaultTextFont() { return m_DefaultTextFont; }
    private:
        void OnWindowSizeChanged(const WindowResizeEvent& e);
        void OnEvent(const Event& e) override;
        void CreateRootPanel();
    private:
        std::unordered_map<size_t,std::weak_ptr<Element>> m_DrawableIDHash{};
        Size2D m_CurrentWindowSize{};
        glm::mat4 m_ToNDCMatrix{};
        std::shared_ptr<DrawableCreator> m_ApplicationDrawableCreator{};
        std::shared_ptr<Panel> m_RootWindowPanel;
        std::shared_ptr<TextFont> m_DefaultTextFont;
    };

    template<typename T>
    inline std::shared_ptr<T> UIManager::CreateUIElement(const glm::vec2 element_pos,
        const glm::vec2 element_size)
    {
        static_assert(std::is_base_of_v<Element, T> == true && "Call must inherit from 'UIElement'.");

        if (!m_RootWindowPanel)
            CreateRootPanel();

        T* instance = new T(std::dynamic_pointer_cast<UIManager>(
            this->shared_from_this()),m_ApplicationDrawableCreator,
            element_pos, element_size);

        std::shared_ptr<T> result = std::shared_ptr<T>(instance);
        std::weak_ptr<Element> element_weak_ptr_cast = std::dynamic_pointer_cast<Element>(result);

        m_RootWindowPanel->AddChildElement(element_weak_ptr_cast);
        m_DrawableIDHash.emplace(instance->GetElementID(), element_weak_ptr_cast);

        return result;
    }
}