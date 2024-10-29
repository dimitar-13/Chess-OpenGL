#pragma once
#include "Pch/Chess_pch.h"
#include "Core/Drawable.h"
#include "Core/ApplicationData.h"
#include "Core/DrawableCreator.h"
#include "Core/OrthographicApplicationMatrix.h"
#include "gpu-side/BatchRenderer.h"

namespace Chess_Game
{
    /**
     * @brief Enum representing 'Panel' pivot points. 
     * 
     * A panel pivot point is an imaginary point on a 2D square/rectangle.
     * The main idea of this is instead of specifying a raw window/screen position to specify
     * offset from a pivot point and based on the panel metrics(position and size) to calculate
     * the pivot point.
     */
    enum PositionPivot_
    {
        PositionPivot_kMiddle,      ///< The middle of the panel.
        PositionPivot_kTopLeft,     ///< The top left of the panel.
        PositionPivot_kTopRight,    ///< The top right of the panel.
        PositionPivot_kBottomLeft,  ///< The bottom left of the panel.
        PositionPivot_kBottomRight, ///< The bottom right of panel.
    };

    /**
     * @brief Struct Axis-Aligned Bounding Box(AABB).
     * 
     * Struct holds the start(x,y) of the box and the end(width,height).
     * 
     */
    struct AxisAlignedBoundingBox
    {
        float x{}, y{};          ///< The lower left corner of the bounding box.
        float width{}, height{}; ///< The top right coner of the bounding box.

        /**
         * @brief Returns true if the provided screen position is inside the AABB.
         * 
         * @param screen_position The position to check if its in the AABB.
         * 
         * @returns Returns true if the provided screen position is inside the AABB. 
         */
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
     */
    class Element : public std::enable_shared_from_this<Element>
    {
    protected:
        friend class UIManager;
        friend class Panel;
        /**
         * @brief Construct a UI element.
         * 
         * Construct a valid UI element and sets the related information.
         * The constructor also uses the provided 'DrawableCreator' to create a valid drawable 
         * for the UI element.
         * 
         * @param ui_manager_ref Weak ref to the UIManager.
         * @param drawable_creator Ref to drawable creator.
         * @param relative_offset Offset from element panel pivot point.
         * @param element_size Half Size of the element.
         */
        Element(std::weak_ptr<UIManager> ui_manager_ref,
            std::shared_ptr<DrawableCreator> drawable_creator,
            const glm::vec2& relative_offset,
            const glm::vec2& element_size);
    public:
#pragma region Getters
        /**
         * @brief Gets the element Z-Depth.
         * @returns The Z-Depth of the element.
         */
        float GetElementDepth()const { return m_DepthLayer; }
        /**
         * @brief Gets the position pivot.
         * @returns Returns the element position pivot.
         */
        PositionPivot_ GetPositionPivot()const { return m_ParentPosPivot; }
        /**
         * @brief Gets the element relative position/offset.
         * @returns Returns the element relative position/offset.
         */
        glm::vec2 GetRelativePos()const { return m_ElementRelativePos; }
        /**
         * @brief Gets the element visibility.
         * @returns Returns true if the element is visible and false if not.
         */
        bool IsElementVisible()const { return m_WidgetDrawable->m_IsEnabled; }
        /**
         * @brief Returns true if the element is enabled otherwise false.
         * @returns Returns true if the element is enabled otherwise false.
         */
        bool IsElementEnabled()const { return m_IsElementEnabled; }
        /**
         * @brief Gets the element size.
         * @returns Returns the element size.
         */
        glm::vec2 GetElementSize()const { return m_WidgetDrawable->m_Scale; }
        /**
         * @brief Gets the element drawable ID.
         * @returns Returns the element drawable ID.
         */
        size_t GetElementID()const { return m_WidgetDrawable->GetDrawableID(); }
        /**
         * @brief Gets the element bounding box.
         * @returns Returns the element bounding box.
         */
        const AxisAlignedBoundingBox& GetElementBoundingBox()const { return m_BoundingBox; }
#pragma endregion

#pragma region Setters
        /**
         * @brief Sets the relative position/offset.
         * Sets the relative position/offset to the provided one and invokes 'OnElementChanged()'.
         * @param new_pos The new relative position/offset to use.
         */
        void SetRelativePosition(const glm::vec2& new_pos);
        /**
         * @brief Sets the position pivot to the provided one.
         *
         * Sets the position pivot to the provided one and invokes 'OnElementChanged()'.
         * @param new_pivot The new pivot to be used.
         */
        void SetPositionPivot(PositionPivot_ new_pivot);
        /**
         * @brief Sets the element size to the provided one.
         * Sets the element size to the provided one and invokes 'OnElementChanged()'.
         * @param new_size The new element size.
         */
        void ResizeElement(const glm::vec2& new_size);
        /**
         * @brief Sets the element Z-depth to the provided one.
         * Sets the element Z-Depth to the provided one and invokes 'OnElementChanged()'.
         * @param depth_layer_value The new element Z-Depth.
         */
        void SetElementDepth(float depth_layer_value);
        /**
         * @brief Sets the element visibility.
         * Sets the element visibility to the provided one and invokes 'OnElementChanged()'.
         * @note Visibility means if the object will be drawn.
         * @param is_visible The new element visibility to use.
         */
        void SetVisibility(bool is_visible);
        /**
         * @brief Enables or disables the element.
         * Enables or disables the element and invokes 'OnElementChanged()'.
         * @note Enable or disable means if the object will be used by the UIManager.
         *       This is used for the Panel to hide all of the child objects without overlapping 
         *       the pawn background logic.
         * @param is_enabled Enables or disables if true or false respectively.
         */
        void EnableElement(bool is_enabled);
#pragma endregion
        /**
         * @brief Gets the parent pivot position.
         * @returns Returns the parent pivot position.
         */
        glm::vec2 GetParentPivotPos(PositionPivot_ pivot)const;
        /**
         * @brief Gets the absolute element screen position.
         *
         * Recursively goes to the parent and their parents and calculates 
         * @returns Returns the absolute element screen position.
         */
        glm::vec2 GetScreenPos();
        /**
         * @brief Destroys the UI element.
         * 
         * Remove the element from its parent and then it destroys the element using the 
         * 'UIElement:RemoveWidget()' which removes the widget from the 'UIElement' hash.
         */
        virtual ~Element();
        /**
         * @brief Called when element is pressed.  
         */
        virtual void OnElementPressed() {};
    private:
        /**
         * @brief Updates the element using the element parent.
         *
         * Updates the element Bounding box based on the current size and the screen position.
         * TODO: Update the 'Drawable' postion.
         */
        void UpdateElement();
        /**
         * @brief Calculates the element bounding box based on the position and size.
         *
         * @param screen_pos   The element absolute screen position.
         * @param element_size The element size.
         */
        void CalculateElementBoundingBox(const glm::vec2& screen_pos,const glm::vec2& element_size);
        /**
         * @brief Draws the UI element based on the stored data.
         * 
         * Uses the provided 'BatchRenderer' to submit for drawing.
         * If not overwritten the function submits to the textured quad batch.
         * 
         * @param batch_renderer Renderer to use.
         */
        virtual void Draw(BatchRenderer& batch_renderer);
    protected:
        /**
         * @brief Called on element change.
         * 
         * This function is called when element position,size etc. has changed.
         * It calls the 'UpdateElement()'.
         */
        virtual void OnElementChanged() { UpdateElement(); };
    protected:
        glm::vec2 m_ElementRelativePos = glm::vec2(0);  ///< Element parent pivot postion/offset. 
        float m_DepthLayer = 0.0f;                      ///< Element Z-Depth.
        AxisAlignedBoundingBox m_BoundingBox;           ///< A bounding for fast mouse picking.
        std::weak_ptr<UIManager> m_UIManager;           ///< Weak_ref to the UI manager.
        std::shared_ptr<Panel> m_Parent;                ///< Strong_ref to the element parent by default is 'RootWindowPanel'.
        PositionPivot_ m_ParentPosPivot = PositionPivot_kMiddle; ///< Panel pivot position to use with the 'm_ElementRelativePos'.
        std::shared_ptr<Drawable> m_WidgetDrawable;              ///< The drawable used for drawing.
        /**
         * This flag represents if an element is enabled. An enabled element is mainly used 
         * in the 'Panel'. Say you want to hide a panel with all with its content inside aka Enable it
         * with single IsForDrawing flag this is not enough. 
         * That's why this flags is needed to save the data. 
         */
        bool m_IsElementEnabled = true;                          
    };
}