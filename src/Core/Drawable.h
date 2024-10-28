#pragma once
#include "Pch/Chess_pch.h"
#include "TextureEnum.h"
#include "Logging/Logger.h"
namespace Chess_Game
{
    class DrawableCreator;
    /**
     * @brief Class representing a 2D object in the scene.
     *
     * To create a class use 'DrawableCreator.CreateDrawable();'.
     * 
     * This class represents a 2D scene object. The class store a position,color and scale of the
     * object it also has a isEnable state to know if we are going to draw it or not and a 'TextureName_'
     * enum that is used to fetch the loaded texture from the 'AssetLoader'.
     * Additionally it stores an ID of the scene object.
     * 
     * The class requires a shared_ptr to 'DrawableCreator'. This Dependency Injection is used 
     * for ID creation and at later point to notify the 'DrawableCreator' class for 
     * the destruction of the object.
     * 
     * The drawing logic is in the 'BatchRenderer' class.
     */
    class Drawable
    {
    public:
        /**
         * @brief Constructs a drawable.
         *
         * @param drawable_id ID of the drawable.
         * @param drawable_creator Drawable creator.
         */
        Drawable(size_t drawable_id,const std::shared_ptr<DrawableCreator>& drawable_creator):
            m_DrawableID(drawable_id), m_DrawableCreator(drawable_creator){}
        /**
         * @brief Destroys the drawable by notifying the 'DrawableCreator'.
         */
        ~Drawable();
        /**
         * @brief Returns the ID of the drawable.
         *
         * @return Retuns the ID of the drawable.
         */
        size_t GetDrawableID()const { return m_DrawableID; }
    public:
        glm::vec3 m_Position{}; ///< The 2D position + the deapth of the object.
        glm::vec3 m_Color = glm::vec3(1); ///< The color of the object.
        glm::vec2 m_Scale = glm::vec2(30.0f); ///< The object scale.
        /**
         * @brief The enum of the texture to use.
         * To fetch the actual texture index use 'AssetLoader'.
         */
        TextureName_ m_TextureName = TextureName_kWhiteTexture;
        bool m_IsEnabled = true; ///< True if this object will be drawn.
    private:
        size_t m_DrawableID{}; ///< The index of the object asigned by the 'DrawableCreator'.
        std::shared_ptr<DrawableCreator> m_DrawableCreator{};
    };




}