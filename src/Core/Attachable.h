#pragma once
#include "Drawable.h"
#include <memory>

namespace Chess_Game
{
    //TODO: Remove this
    class Attachable
    {
    public:
        void AttachDrawable(std::shared_ptr<Drawable>& drawable_to_attach) { m_AttachedDrawable = drawable_to_attach; }
        void Detach() { m_AttachedDrawable = nullptr; }
        std::weak_ptr<Drawable> GetDrawable() { return m_AttachedDrawable; }
    private:
        std::shared_ptr<Drawable> m_AttachedDrawable{};
    };
}