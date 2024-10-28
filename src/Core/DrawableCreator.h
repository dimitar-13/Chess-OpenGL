#pragma once
#include "Pch/Chess_pch.h"
#include "Drawable.h"
#include "MemoryPool.h"
namespace Chess_Game
{
    /**
     * @brief Drawable creator/register for creating drawables.
     *
     * The class provides a way to create drawable and return a shared ref to it.
     * The class uses the 'MemoryPool' to pre-allocate a set number of drawable.
     * When a drawable is needed to be created the class creates it setting its ID and then its returned.
     * Every drawable created is kept via weak ref and when it comes time to delete it it returns the used ID for it to be reused.
     * 
     */
    class DrawableCreator : public std::enable_shared_from_this<DrawableCreator>
    {
    public:
        /**
         * @brief Constructor initializes the ID queue and the memory pool.
         * 
         * The constructor initializes the ID queue with values from 1 to 'kMaxDrawableCount' + 1 and it creates a shared ref 
         * of the 'MemoryPool' for fast allocation.
         */
        DrawableCreator();
        /**
         * @brief Creates a new drawable and returns a shared ref ot it.
         *
         * Gets memory allocated by the memory pool and construct the drawable passing the next available ID in the queue.
         * 
         * @returns Returns the newly created drawable.
         */
        std::shared_ptr<Drawable> CreateDrawable();
        /**
         * @brief Deletes drawable by specified ID.
         *
         * This function should be called from a drawable destructor.
         * The function removes the drawable from the weak ref hash map and push the used ID back in the queue.
         * @param drawable_id ID of the drawable to be deleted.
         */
        void DeleteDrawable(size_t drawable_id);
    private:
        static const size_t kMaxDrawableCount = 100;                                    ///< Max drawable count that can be created.
        std::queue<size_t> m_DrawableIndexPool{};                                       ///< Index queue pool for asigning ID to drawables.
        std::unordered_map<size_t,std::weak_ptr<Drawable>> m_DrawablesHash{};           ///< Drawable ID and weak ref hash of the valid drawables.
        std::shared_ptr<MemoryPool<Drawable, kMaxDrawableCount>> m_DrawableMemoryPool;  ///< Memory pool for allocating resource for drawable.
    };
}