#pragma once
#include "UIElement.h"
namespace Chess_Game
{

    class UIManager;
    /**
     * @brief Class representing Panel UI widget.
     * 
     * A panel is a collection of UI elements(polymorphic classes of 'Element') that are 
     * positioned relative to the panel pivot point.
     * 
     * The main idea of the Panel is to provide a way for collection of elements to be organized and 
     * moved in a easy manner.The Panel is in charge of notifying the elements on possition or 
     * visibility changes.
     * 
     * The 'Panel' doesn't manage the lifetime of the children but instead its vice versa.The panel
     * holds weak ref to its children while the children hold a strong ref to the panel they have
     * as parent.
     * 
     * @note There is always 1 root Panel,representing the window, that is created by the 
     * 'UIManager'.
     * 
     */
    class Panel : public Element
    {
    public:
        /**
         * @brief Sets a texture to the panel background.
         * 
         * By default the panel has a white color as background you can change it to custom texture using the function.
         * @note If the base class(Element) member variable m_WidgetDrawable->m_IsEnabled is false then the background wont be visible.
         * @param name_of_texture Enum value of type 'TextureName_'.
         */
        void SetPanelCustomTexture(TextureName_ name_of_texture) { m_WidgetDrawable->m_TextureName = name_of_texture; }
        /**
         * @brief Adds a child element to the panel.
         *
         * The function gets a child element and first it removes its parent using the 
         * 'RemoveChildElement()' function(if parent is NUll then it skips this step).
         * Then it sets the child->Parent = this and calls the 'UpdateElement()' to update the 
         * child postion.
         * 
         * @param child_to_add Ref to the child to be added.
         */
        void AddChildElement(std::shared_ptr<Element> child_to_add);
        /**
         * @brief Called on element update(either position,scale or visibility change).
         *
         * The function calls the Base::OnElementChanged() and then it notifies the child elements using the
         * 'UpdatePanelChilds()'.
         */
        void OnElementChanged()override;
        /**
         * @brief Removes a child element from the child_hash.
         * Overload removing by weak ref.
         * @param child_to_remove Weak ref of the child to be removed.
         */
        void RemoveChildElement(std::weak_ptr<Element> child_to_remove);
        /**
         * @brief Removes a child element from the child_hash.
         * Overload removing by drawable ID.
         * @param child_drawable_id Child drawable ID.
         */
        void RemoveChildElement(size_t child_drawable_id);
    private:
        friend class UIManager;
        /**
         * @brief Creates a Panel class and calls the 'Element' constructor.
         *
         * @param ui_manager_ref Weak ref to the UIManager.
         * @param drawable_creator Ref to drawable creator.
         * @param relative_offset Offset from element panel pivot point.
         * @param element_size Half Size of the element.
         */
        Panel(std::weak_ptr<UIManager> ui_manager_ref,
            std::shared_ptr<DrawableCreator> drawable_creator,
            const glm::vec2& relative_offset,const glm::vec2& element_size);
        /**
         * @brief Calls 'OnElementChanged()' on every valid child ref.
         *
         * Function is called when the panel element position or visibility has been changed.
         * The function then notifies the child elements using the 'OnElementChanged()' function.
         */
        void UpdatePanelChilds();
    private:
        std::unordered_map<size_t, std::weak_ptr<Element>> m_ChildDrawableIDHash{}; ///< Drawable ID element hash representing the panel child elements.
    };
}