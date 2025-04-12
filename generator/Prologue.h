#pragma once

#include <SDL3/SDL.h>

namespace zdl
{

void SDLThrow(std::source_location location)
{
    constexpr size_t BufferSize = 256;
    char error[BufferSize];
    const char *pErrorMessage = SDL_GetError();
    if (!pErrorMessage)
    {
        pErrorMessage = "(unknown)";
    }
    SDL_snprintf(error, BufferSize, "SDL error: %s", pErrorMessage);
    SDL_Log(error);
    throw std::runtime_error{error};
}

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
