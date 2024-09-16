#include "D:/c++/OpenGl/Chess-OpenGL/build/CMakeFiles/Chess.dir/Debug/cmake_pch.hxx"
#include "Drawable.h"
#include "DrawableCreator.h"

Chess_Game::Drawable::~Drawable()
{
    m_DrawableCreator->DeleteDrawable(m_DrawableID);
}
