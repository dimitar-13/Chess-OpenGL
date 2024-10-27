#pragma once
#include "Chess_pch.h"
#include <unordered_set>

namespace Chess_Game
{
    /**
     * @brief A fixed-size memory pool for efficient memory allocation and deallocation.
     *
     * This `MemoryPool` class template manages a pre-allocated, fixed-size pool of objects of type `T`.
     * Memory is allocated and deallocated from this pool, which avoids the need for dynamic memory
     * allocations each time an object of type `T` is required. The memory pool is designed to work
     * with unique pointers and shared pointers, allowing for controlled deallocation via custom deleters.
     *
     * @tparam T Type of the objects to be stored in the memory pool.
     * @tparam _Size Number of objects that the memory pool can hold.
     */
    template<typename T, size_t _Size>
    class MemoryPool : public std::enable_shared_from_this<MemoryPool<T, _Size>>
    {
    public:
        /**
         * @brief Custom deleter for objects in the memory pool.
         *
         * The `Deleter` struct is used to return objects to the memory pool when they are no longer needed.
         * The `operator()` calls the `DeAllocate` function of `MemoryPool`, passing in the pointer to the
         * object that should be returned to the pool.
         */
        class Deleter
        {
        public:
            std::shared_ptr<MemoryPool<T, _Size>> m_MemoryPool;  ///< Shared pointer to the memory pool.

            /**
             * @brief Returns the object to the memory pool for reuse.
             *
             * Calls `DeAllocate` on the associated `MemoryPool` instance, allowing the pool to reclaim
             * the memory of the destroyed object.
             *
             * @param obj_to_destroy Pointer to the object being deleted.
             */
            constexpr void operator()(T* obj_to_destroy) const
            {
                m_MemoryPool->DeAllocate(obj_to_destroy);
            }
        };

        /**
         * @brief Constructs a memory pool with space for `_Size` objects of type `T`.
         *
         * The constructor allocates a contiguous memory block for `_Size` instances of `T`.
         * All allocated objects are stored in a free list (queue) that can be drawn from
         * when objects are requested.
         */
        MemoryPool()
        {
            m_pool = reinterpret_cast<T*>(::operator new[](sizeof(T)* _Size));

            for (size_t i = 0; i < _Size; i++)
            {
                m_Free.push(m_pool + i);
            }
        }

        /**
         * @brief Destructor that reclaims all allocated memory.
         *
         * Destroys all objects currently in use, deallocates the memory block,
         * and clears both the used and free lists.
         */
        ~MemoryPool()
        {
            for (T* obj_ptr : m_Used)
            {
                obj_ptr->~T();
            }
            m_Used.clear();

            ::operator delete[](m_pool, sizeof(T)* _Size);
        }

        /**
         * @brief Allocates an object from the memory pool and returns it as a unique pointer.
         *
         * This function allocates memory for an object by taking a slot from the free list,
         * constructs the object in-place using the provided arguments, and returns a unique pointer
         * with a custom deleter that will return the object to the pool when deleted.
         *
         * @tparam Args Variadic template for arguments to forward to `T`'s constructor.
         * @param args Arguments to forward to the `T` constructor.
         * @return std::unique_ptr<T, Deleter> A unique pointer to the newly created object.
         */
        template<typename... Args>
        std::unique_ptr<T, Deleter> Allocate(Args&&... args)
        {
            assert(!m_Free.empty() && "Can't allocate more memory.");

            T* mem_to_create = m_Free.front();
            m_Free.pop();
            m_Used.insert(mem_to_create);

            mem_to_create = new(mem_to_create) T(std::forward<Args>(args)...);

            return std::unique_ptr<T, Deleter>(mem_to_create, Deleter{ this->shared_from_this() });
        }

        /**
         * @brief Allocates an object from the memory pool and returns it as a shared pointer.
         *
         * Similar to `Allocate`, this function allocates an object from the pool but returns it as a
         * shared pointer with a custom deleter. This allows for multiple shared owners of the object.
         *
         * @tparam Args Variadic template for arguments to forward to `T`'s constructor.
         * @param args Arguments to forward to the `T` constructor.
         * @return std::shared_ptr<T> A shared pointer to the newly created object.
         */
        template<typename... Args>
        std::shared_ptr<T> AllocateShared(Args&&... args)
        {
            assert(!m_Free.empty() && "Can't allocate more memory.");

            T* mem_to_create = m_Free.front();
            m_Free.pop();
            m_Used.insert(mem_to_create);

            mem_to_create = new(mem_to_create) T(std::forward<Args>(args)...);

            return std::shared_ptr<T>(mem_to_create, Deleter{ this->shared_from_this() });
        }

    private:
        /**
         * @brief Deallocates an object and returns it to the free list.
         *
         * Calls the destructor on the object and adds its memory back to the free list.
         *
         * @param object_to_delete Pointer to the object being returned to the pool.
         */
        void DeAllocate(T* object_to_delete)
        {
            object_to_delete->~T();      
            m_Free.push(object_to_delete);
            m_Used.erase(object_to_delete);
        }

    private:
        T* m_pool;                                ///< Pointer to the pre-allocated memory block for the pool.
        std::queue<T*> m_Free;                    ///< Queue storing pointers to free slots in the pool.
        std::unordered_set<T*> m_Used;            ///< Set storing pointers to currently allocated objects.
    };

    
}