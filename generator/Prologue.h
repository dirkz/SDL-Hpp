#pragma once

#include <SDL3/SDL.h>

#include <source_location>
#include <stdexcept>

namespace sdl
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

template <class T> void Release(T *object)
{
}

template <class T> struct UniquePointer
{
    UniquePointer() : m_object{nullptr} {};

    UniquePointer(T *object) : m_object{object}
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

template <class T> void ReleaseFromDevice(SDL_GPUDevice *device, T *object)
{
}

template <class T> struct DeviceOwned
{
    DeviceOwned(SDL_GPUDevice *device, T *object) m_device{device}, m_object{object} {};

  private:
    SDL_GPUDevice *m_device;
    T *m_object = nullptr;
};
