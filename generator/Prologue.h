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
        if (m_object)
        {
            Release(m_object);
        }
    }

    T *Detach()
    {
        T *object = m_object;
        m_object = nullptr;
        return object;
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
    DeviceOwned(SDL_GPUDevice *device, T *object) : m_device{device}, m_object{object} {};

    ~DeviceOwned()
    {
        if (m_object)
        {
            ReleaseFromDevice(m_device, m_object);
        }
    }

    T *Detach()
    {
        T *object = m_object;
        m_object = nullptr;
        return object;
    }

  private:
    SDL_GPUDevice *m_device;
    T *m_object = nullptr;
};

struct UniqueHaptic
{
    UniqueHaptic(SDL_Haptic *haptic, int effect) : m_haptic{haptic}, m_effect{effect} {};

    ~UniqueHaptic()
    {
        if (m_haptic)
        {
            SDL_DestroyHapticEffect(m_haptic, m_effect);
        }
    }

    SDL_Haptic *Detach()
    {
        SDL_Haptic *haptic = m_haptic;
        m_haptic = nullptr;
        return haptic;
    }

  private:
    SDL_Haptic *m_haptic;
    int m_effect;
};

struct UniqueProperties
{
    UniqueProperties(SDL_PropertiesID props) : m_properties{props} {};

    ~UniqueProperties()
    {
        SDL_DestroyProperties(m_properties);
    }

  private:
    SDL_PropertiesID m_properties;
};

struct UniqueCameraSurface
{
    UniqueCameraSurface(SDL_Surface *surface, SDL_Camera *camera)
        : m_surface{surface}, m_camera{camera} {};

    ~UniqueCameraSurface()
    {
        if (m_surface)
        {
            SDL_ReleaseCameraFrame(m_camera, m_surface);
        }
    }

    SDL_Surface *Detach()
    {
        SDL_Surface *surface = m_surface;
        m_surface = nullptr;
        m_camera = nullptr;
        return surface;
    }

  private:
    SDL_Surface *m_surface;
    SDL_Camera *m_camera;
};
