#include "Drawable.h"
#include "DrawableCreator.h"

Chess_Game::Drawable::~Drawable()
{
    m_DrawableCreator->DeleteDrawable(m_DrawableID);
}
