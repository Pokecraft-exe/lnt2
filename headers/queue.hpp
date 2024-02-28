#pragma once
#include <stddef.h>
#include "iterator.hpp"
#include "allocator.hpp"

template <class T>
class queue {
private:
    void* base_ptr;
    T* FROM;
    T* TO;
    
    typedef struct Iterator 
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;  // or also value_type*
        using reference         = T&;  // or also value_type&

        Iterator(pointer ptr) : m_ptr(ptr) {}
        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        // Prefix increment
        Iterator& operator++() { m_ptr++; return *this; }  

        // Postfix increment
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

        Iterator begin() { return Iterator(FROM); }
        Iterator end()   { return Iterator(TO); }
    private:
        pointer m_ptr;
    } iterator;
public:
    queue() {
        base_ptr = alloc(1);
        FROM = (T*)base_ptr;
        TO = (T*)base_ptr;
    };
    iterator begin() { return iterator(FROM); }
    iterator end() { return iterator(TO >= FROM? TO + 1 : TO - 1); }
    size_t size() {
        return (size_t)(TO - FROM);
    }
    T pop_front() {
        FROM+=sizeof(T);
        return *(T*)FROM;
    }
    void push_back(T element) {
        size_t _size = size();
        void* newptr = alloc(TO - FROM + sizeof(T));
        memcpy(newptr, FROM, _size);
        free(base_ptr);
        base_ptr = newptr;
        FROM = (T*)newptr;
        *(FROM + _size) = element;
        TO = (T*)(_size + sizeof(T));
    }
};