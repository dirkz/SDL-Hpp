#pragma once

namespace zdl
{

template <class T> void Release(T *t)
{
}

template <class T> struct UniquePointer
{
    UniquePointer() : m_object{nullptr} {};

    UniquePointer(T *t) : m_object{t}
    {
    }

    UniquePointer(T &&other) noexcept : m_object{other.m_object}
    {
        other.m_object = nullptr;
    }

    UniquePointer(const UniquePointer &) = delete;

    ~UniquePointer()
    {
        if (m_object != nullptr)
        {
            Release(m_object);
        }
    }

    T **Address()
    {
        return &m_object;
    }

    T *Get()
    {
        return m_object;
    }

    T *operator*()
    {
        return m_object;
    }

    operator T *()
    {
        return m_object;
    }

  private:
    T *m_object = nullptr;
};

