#pragma once
#include "UIElement.h"
#include "Button.h"
#include "GPU-Side/BatchRenderer.h"
#include <type_traits>
#include <queue>
#include <iostream>
#include "Core/AssetLoader.h"

namespace Chess_Game
{
    class UIManager
    {
    private:
        using ElementID = size_t;
        static constexpr size_t kUIElementCount = 10;
    public:
        UIManager(const std::shared_ptr<BatchRenderer>& global_batch_renderer);
        void Update(const glm::mat4& projection_test, AssetLoader& test_loader);
        template<typename T>
        std::shared_ptr<T> CreateUIElement(TextureName_ texture,glm::vec2 button_position = glm::vec2(0),
            glm::vec2 button_scale = glm::vec2(1));
    private:
        void PollUIInput();
        void DrawUI(const glm::mat4& projection_test, AssetLoader& test_loader);
    private:
        std::queue<ElementID> m_IDQueue{};
        std::shared_ptr<BatchRenderer> m_ApplicationBatchRenderer{};
        std::vector<std::weak_ptr<UIElement>> m_UIElements{};
    };

    template<typename T>
    inline std::shared_ptr<T> UIManager::CreateUIElement(TextureName_ texture,glm::vec2 button_position,
        glm::vec2 button_scale)
    {
        static_assert(std::is_base_of_v<UIElement, T> == true && "Call must inherit from 'UIElement'.");
        ElementID id = m_IDQueue.front();
        m_IDQueue.pop();
        T* instance = new T(id, button_position, button_scale, texture);
        std::shared_ptr<T> result = std::shared_ptr<T>(instance);
        m_UIElements.push_back(std::dynamic_pointer_cast<UIElement>(result));
        return result;
    }
}