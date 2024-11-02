#pragma once
#include "Pch/Chess_pch.h"
#include "UIElement.h"
#include "Panel.h"
#include "GPU-Side/BatchRenderer.h"
#include "Core/Input.h"
namespace Chess_Game
{
    /**
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


    /**
     * @brief Class for creating/register UI widgets.
     *
     * The basic idea of the class is to provide a way of creating a specific UI widget and handle the drawing and deletion 
     * of the widget.
     * 
     * To create any UI element,inheriting from the Element class, you use the 'CreateUIElement<UI-Widget-Class>()' the function
     * does solves the dependencies via dependency Injection(DI) and then it returns a strong ref while keeping a weak ref to the 
     * widget.
     * 
     * The class also handles UI input using the 'PollUIInput()'.
     * 
     */
    class UIManager : public Listener
    {
    public:
        /**
        * @brief Constructs a new UIManager instance.
        *
        * This constructor initializes the UI manager with a given window size and a shared pointer to
        * a DrawableCreator. It also sets up an orthographic projection matrix for converting root
        * window coordinates to normalized device coordinates (NDC) and loads the default 
        * font for UI text rendering.
        *
        * @param window_size The size of the current window (width and height).
        * @param drawable_creator A shared pointer to the DrawableCreator responsible for managing drawable objects.
        */
        UIManager(Size2D window_size, std::shared_ptr<DrawableCreator>& drawable_creator);
        /**
         * @brief Convert a screen postion to root window position.
         * 
         * Root window has a center in the middle meaning that for x and y respectively the root 
         * window has the following ranges: x = {-width/2,width/2} y = {height/2,height/2}.
         * Because of that every UI element is in Root window space and this function converts a 
         * screen space range(x = {0,window_width},y = {0,window_height}) position to Root window one.
         * 
         * @param screen_pos Screen position to be converted.
         */
        glm::vec2 ConvertScreenToRootWindowPos(const glm::vec2& screen_pos);
        /**
         *  @brief Removes widget by drawable ID.
         * Removes widget from the hash map.
         * @param widget_drawable_id Drawable ID of the widget to be removed.
         */
        void RemoveWidget(size_t widget_drawable_id);
        /**
         * @brief Draws the entire UI. 
         * 
         * Iterates through all of the valid UI elements and calls their respective 'Draw()' functions.
         * After that it draws both QuadBatch and TextBatch using a special UI ortho matrix.
         * 
         * @param application_batch_renderer Application batch renderer.
         */
        void DrawUI(std::shared_ptr<BatchRenderer> application_batch_renderer);
        /**
         * @brief Checks if a UI button was pressed using mouse picking.
         *
         * The function iterates thou all of the UI elements and it first do a quick check using the
         * Axis-Aligned Bounding box. If that check succeeds then it does a pixel perfect check using
         * the 'IndexFramebuffer' from the 'BatchRenderer' and if that succeeds it invokes the 
         * OnElementPressed callback.
         * 
         * @note 'IndexFramebuffer' stores every UI drawable index.
         * 
         */
        void PollUIInput(const MouseInput& application_input,
            std::shared_ptr<BatchRenderer> application_batch_renderer,const OrthoViewportHandler& test);
        /**
         * @brief Function used to create an UI element and return it.
         *
         * Function creates and registers the created UI element. It stores a weak_ref to it 
         * in a hash map and returns a strong ref to it.
         * The function also creates the RootWindowPanel on first time beeing called.
         * 
         * @param element_relative_offset Offset of the element.
         * @param element_size Size of the element.
         * @tparam T Polymorphic class of base class 'Element'.
         * @returns Returns strong ref to the newly created UI element.
         */
        template<typename T>
        std::shared_ptr<T> CreateUIElement(const glm::vec2 element_relative_offset,
            const glm::vec2 element_size);
        /**
         * @brief Gets the default text font. 
         * @returns Returns strong ref to the default UI text font.
         */
        std::shared_ptr<TextFont> GetDefaultTextFont() { return m_DefaultTextFont; }
    private:
        /**
         * @brief Called on window resize event.
         *
         * The function is called on window resize event. It recalculates the 
         * root window ortho matrix and it stores the window size.And it resizes the 
         * root window panel.
         * 
         */
        void OnWindowSizeChanged(const WindowResizeEvent& e);
        /**
         * @brief On event inherited from 'Listener'. 
         */
        void OnEvent(const Event& e) override;
        /**
         * @brief Creates the Root window panel.
         * Creates a Root window panel and saves a strong reference to it.
         */
        void CreateRootPanel();
    private:
        std::unordered_map<size_t,std::weak_ptr<Element>> m_DrawableIDHash{}; ///< Registered UI element drawable ID hash.
        Size2D m_CurrentWindowSize{};               ///< Current window size.
        glm::mat4 m_RootWindowOrthographicMatrix{}; ///< Matrix used to conver root window coordinates to NDC coordinates.
        std::shared_ptr<DrawableCreator> m_ApplicationDrawableCreator{}; ///< Application drawable creator/register.
        std::shared_ptr<Panel> m_RootWindowPanel;   ///< Root window panel(lazy initialized on first call of 'CreateUIElement()').
        std::shared_ptr<TextFont> m_DefaultTextFont; ///< Default font for every 'UIText' widget.
    };

    template<typename T>
    inline std::shared_ptr<T> UIManager::CreateUIElement(const glm::vec2 element_relative_offset,
        const glm::vec2 element_size)
    {
        static_assert(std::is_base_of_v<Element, T> == true && "Call must inherit from 'UIElement'.");

        if (!m_RootWindowPanel)
            CreateRootPanel();

        T* instance = new T(std::dynamic_pointer_cast<UIManager>(
            this->shared_from_this()),m_ApplicationDrawableCreator,
            element_relative_offset, element_size);

        std::shared_ptr<T> result = std::shared_ptr<T>(instance);

        m_RootWindowPanel->AddChildElement(result);
        m_DrawableIDHash.emplace(instance->GetElementID(), std::dynamic_pointer_cast<Element>(result));

        return result;
    }
}