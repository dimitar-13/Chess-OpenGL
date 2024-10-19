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
    /**
     * @brief Base class for creating a UI element.
     * 
     * The class provides base UI element functionalities and several virtual functions.
     * 
     * Here is where im gonna talk about the UI system and how it works.
     * I will split it into 3 parts:
     * 1) Parent-Child relationship
     * 
     * Every 'Element' can have a Parent meaning the UI is Parent-Child relationship.
     * By default every element will have the RootWindowPanelElement as their parent.
     * 
     * When you create a Element of any kind using the 'UIManager.CreateUIElement<ClassOfElement>();'
     * The newly created element gets the RootWindowPanelElement assigned as their parent.
     * If you want to create another panel and change the parent to be that new panel you can. Because
     * panel has friend access to the element priv/protected members.
     * 
     * Root window.
     *  ___________________
     * | Child panel       |
     * |  __________       |
     * | |          |      |    
     * | ||Element| |      |
     * | |          |      |
     * | |__________|      |
     * |                   |
     * |___________________|
     * 
     * Here the Element hierarchy will be like this:
     * 
     * Element->Parent = Child panel->Parent = Root window->Parent = NULL
     * 
     * 2) Positioning
     * 
     * To make it not quite annoying to work and a little bit flexible. I decided to use anchor point
     * positioning or as called in the project 'PositionPivot_' these are imaginary positions on the parent
     * object. 
     * 
     * When you create a UI element instead of specifying a raw position you specify a 
     * offset from a pivot. By default every UI element uses the middle pivot point. This can be changed
     * using the 'SetPositionPivot(PositionPivot_ new_pivot);' and specifying one of the enums.
     * 
     * This allows us when it we need to get the raw window position to use the Parent-Child relationship
     * and recursion to get the position. By unwinding to the root element and getting its pivot point
     * position and offsetting it with the 'm_ElementRelativePos' offset until we accumulate the 
     * final window pos. See 'GetScreenPos()' on how this works.
     * 
     * 
     * 3) Drawing how and where ? 
     * 
     * The drawing is handled by the 'UIManager'. Every UI element can define its own drawing logic
     * by overriding the 'Draw()' function. Every frame the 'UIManager' calls the 'Draw()' function 
     * of every valid element.
     * 
     */
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