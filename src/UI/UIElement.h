#pragma once
#include "Core/Drawable.h"
#include "Core/ApplicationData.h"
namespace Chess_Game
{
    struct AxisAlignedBoundingBox
    {
        float x{}, y{};
        float width{}, height{};

        bool IsInsideBox(const glm::vec2& screen_position)const
        {
            return (screen_position.x >= x && screen_position.x <= width) &&
                (screen_position.y >= y && screen_position.y <= height);
        }
    };
    
    enum AnchorPoint_
    {
        AnchorPoint_kMiddle,
        AnchorPoint_kTopLeft,
        AnchorPoint_kBottomLeft,
        AnchorPoint_kTopRight,
        AnchorPoint_kBottomRight,
    };

    struct Margin
    {
        float left{};
        float right{};
        float bottom{};
        float top{};
    };

    class UIManager;
    class UIElement
    {
    public:
        virtual void SetVisibility(bool is_visible) { m_IsVisible = is_visible; }
        void SetScale(const glm::vec2& new_scale);
        void SetMargin(const Margin& new_margin);
        AxisAlignedBoundingBox& GetElementBoundingBox() { return m_ElementBoundingBox; }

    protected:
        friend class UIManager;

        UIElement(size_t element_id,std::weak_ptr<UIManager> ui_manager_ref,
            const Margin& element_margin, AnchorPoint_ element_margin_anchor_point,
            Size2D window_size,
            const glm::vec2& element_scale);
        virtual ~UIElement();
        virtual void OnWidgetPressed() {};
        void UpdateWindowPosition(Size2D new_window_size);
        void CalculateMarginPosition(Size2D window_size);
        void CalculateBoundingBox();
        std::weak_ptr<Drawable> GetDrawable() { return m_UIDrawable; }
    protected:
        std::shared_ptr<Drawable> m_UIDrawable{};
        AxisAlignedBoundingBox m_ElementBoundingBox{};
        size_t m_UIElementID{};
        bool m_IsVisible = true; 
        Margin m_UIElementMargin;
        AnchorPoint_ m_ElementAnchorPoint;
        glm::vec2 m_ElementWindowPos{};
        glm::vec2 m_ElementScale{};
        std::weak_ptr<UIManager> m_UIManager{};

    };
}