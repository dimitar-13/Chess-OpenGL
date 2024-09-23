#pragma once
#include "Core/Drawable.h"
#include "Core/ApplicationData.h"
#include "Core/DrawableCreator.h"
#include "Core/OrthographicApplicationMatrix.h"
namespace Chess_Game
{

    //struct ClampedFloat
    //{
    //    ClampedFloat(float value)
    //    {
    //        clamped_value = glm::clamp(value, 0.0f, 1.0f);
    //    }
    //    void operator=(float value)
    //    {
    //        clamped_value = glm::clamp(value, 0.0f, 1.0f);
    //    }
    //    ClampedFloat& operator+=(ClampedFloat other)
    //    {
    //        clamped_value = glm::clamp(clamped_value + other.clamped_value, 0.0f, 1.0f);
    //        return *this;
    //    }
    //    ClampedFloat& operator+=(float other)
    //    {
    //        clamped_value = glm::clamp(clamped_value + other, 0.0f, 1.0f);
    //        return *this;
    //    }
    //    ClampedFloat operator+(ClampedFloat other) const
    //    {
    //        return ClampedFloat(glm::clamp(clamped_value + other.clamped_value, 0.0f, 1.0f));
    //    }
    //
    //    ClampedFloat operator+(float other) const
    //    {
    //        return ClampedFloat(glm::clamp(clamped_value + other, 0.0f, 1.0f));
    //    }
    //
    //    float clamped_value{};
    //};



    struct Margin
    {
        float left{};
        float right{};
        float bottom{};
        float top{};
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

        Element(size_t element_id, std::weak_ptr<UIManager> ui_manager_ref,
            DrawableCreator& drawable_creator,const Margin& element_margin);
    public:
        void SetMargin(const Margin& new_margin);
        const Margin& GetMargin()const { return m_ElementMargin; }
        virtual void SetVisibility(bool is_visible);
        bool GetElementVisibility()const { return m_IsElementEnabled; }
        glm::vec2 GetElementSize()const { return m_CurrentElementSize; }
        virtual ~Element();
        size_t GetElementID()const { return m_ElementID; }
        std::shared_ptr<Drawable> GetDrawable() { return m_UIDrawable; }
        const AxisAlignedBoundingBox& GetElementBoundingBox()const { return m_BoundingBox; }
        void OnParentSizeChanged();
        virtual void OnElementPositionChange(glm::vec2 new_position);
        virtual void ResizeElement(glm::vec2 new_size);
        virtual void OnElementPressed() {};
    private:
        void CalculateElementScreenPosition();
        void CalculateElementBoundingBox();
        void UpdateDrawable();
    protected:
        size_t m_ElementID;
        glm::vec2 m_CurrentElementSize;
        Margin m_ElementMargin;
        AxisAlignedBoundingBox m_BoundingBox;
        bool m_IsElementEnabled = true;
        glm::vec2 m_ElementPos = glm::vec2(0);
        std::shared_ptr<Drawable> m_UIDrawable;
        std::weak_ptr<UIManager> m_UIManager;
        std::shared_ptr<Element> m_Parent;
    };
}