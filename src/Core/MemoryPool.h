#pragma once

#include "Chess_pch.h"
#include <unordered_set>

namespace Chess_Game
{
    template<typename T,size_t _Size>
    class MemoryPool : public std::enable_shared_from_this<MemoryPool<T, _Size>>
    {
    public:
        class Deleter
        {
        public:
            std::shared_ptr<MemoryPool<T, _Size>> m_MemoryPool;
            constexpr void operator()(T* obj_to_destroy) const
            {                
                m_MemoryPool->DeAllocate(obj_to_destroy);
            }
        };
    
    
        MemoryPool()
        {
            m_pool = reinterpret_cast<T*>(::operator new[](sizeof(T)* _Size));
    
            for (size_t i =0;i < _Size;i++)
            {
                m_Free.push(m_pool + i);
            }
    
        }
        ~MemoryPool()
        {
            for (T* obj_ptr:m_Used)
            {
                obj_ptr->~T();
            }
            m_Used.clear();
    
            ::operator delete[](m_pool, sizeof(T)* _Size);
        }
    
        template<typename... Args>
        std::unique_ptr<T,Deleter> Allocate(Args&&... args)
        {  
            assert(!m_Free.empty() && "Cant allocate more memory.");
    
            T* mem_to_create = m_Free.front();
            m_Free.pop();
            m_Used.insert(mem_to_create);
    
            mem_to_create = new(mem_to_create) T(std::forward<Args>(args)...);
         
            return  std::unique_ptr<T, Deleter>(mem_to_create, Deleter{*this});
        }
        template<typename... Args>
        std::shared_ptr<T> AllocateShared(Args&&... args)
        {
            assert(!m_Free.empty() && "Cant allocate more memory.");
    
            T* mem_to_create = m_Free.front();
            m_Free.pop();
            m_Used.insert(mem_to_create);
    
            mem_to_create = new(mem_to_create) T(std::forward<Args>(args)...);
    
            return  std::shared_ptr<T>(mem_to_create, Deleter{this->shared_from_this()});
        }
    
    private:
        void DeAllocate(T* object_to_delete)
        {
            object_to_delete->~T();
            m_Free.push(object_to_delete);
            m_Used.erase(object_to_delete);
        }
    private:
        T* m_pool;
        std::queue<T*> m_Free;
        std::unordered_set<T*> m_Used;
    };
    
}