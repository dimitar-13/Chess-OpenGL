#pragma once
#include "Core/Drawable.h"
#include "Core/ApplicationData.h"
#include "Core/DrawableCreator.h"
#include "Core/OrthographicApplicationMatrix.h"
#include "gpu-side/BatchRenderer.h"

namespace Chess_Game
{
    enum PositionPivot_
    {
        PositionPivot_kMiddle,
        PositionPivot_kTopLeft,
        PositionPivot_kTopRight,
        PositionPivot_kBottomLeft,
        PositionPivot_kBottomRight,
    };

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


    class UIManager;
    class Panel;
    class Element : public std::enable_shared_from_this<Element>
    {
    protected:
        friend class UIManager;
        friend class Panel;

        Element(std::weak_ptr<UIManager> ui_manager_ref,
            std::shared_ptr<DrawableCreator> drawable_creator,
            const glm::vec2& position,
            const glm::vec2& element_size);
    public:
        void SetRelativePosition(const glm::vec2& new_pos);
        void SetPositionPivot(PositionPivot_ new_pivot);
        void ResizeElement(const glm::vec2& new_size);
        void SetElementDepth(float depth_layer_value);
        float GetElementDepth()const { return m_DepthLayer; }
        PositionPivot_ GetPositionPivot()const { return m_ParentPosPivot; }
        glm::vec2 GetRelativePos()const { return m_ElementRelativePos; }
        void SetVisibility(bool is_visible);
        void EnableElement(bool is_enabled);
        bool IsElementVisible()const { return m_WidgetDrawable->m_IsEnabled; }
        bool IsElementEnabled()const { return m_IsElementEnabled; }
        glm::vec2 GetElementSize()const { return m_WidgetDrawable->m_Scale; }
        glm::vec2 GetPivotPos(PositionPivot_ pivot)const;
        glm::vec2 GetScreenPos();
        virtual ~Element();
        size_t GetElementID()const { return m_WidgetDrawable->GetDrawableID(); }
        const AxisAlignedBoundingBox& GetElementBoundingBox()const { return m_BoundingBox; }
        virtual void OnElementPressed() {};
    private:
        void UpdateElement();
        void CalculateElementBoundingBox(glm::vec2 screen_pos,glm::vec2 size);
        virtual void Draw(BatchRenderer& batch_renderer);
    protected:
        virtual void OnElementChanged() { UpdateElement(); };
        virtual void EnableDrawable(bool is_visible) { m_WidgetDrawable->m_IsEnabled = is_visible; };
    protected:
        glm::vec2 m_ElementRelativePos = glm::vec2(0);
        float m_DepthLayer = 0.0f;
        AxisAlignedBoundingBox m_BoundingBox;
        std::weak_ptr<UIManager> m_UIManager;
        std::shared_ptr<Panel> m_Parent;
        PositionPivot_ m_ParentPosPivot = PositionPivot_kMiddle;
        std::shared_ptr<Drawable> m_WidgetDrawable;
        bool m_IsElementEnabled = true;
    };
}