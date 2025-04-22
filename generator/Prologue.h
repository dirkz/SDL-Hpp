#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <source_location>
#include <stdexcept>

namespace sdl
{

inline void SDLThrow(std::source_location location)
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

struct FreeVoid
{
    void operator()(void *p)
    {
        SDL_free(p);
    }
};

using Void = std::unique_ptr<void, FreeVoid>;

template <class T> void Destroy(T *object)
{
}

template <class T> struct Deleter
{
    void operator()(T *object)
    {
        Destroy<T>(object);
    }
};

template <class T> void ReleaseFromDevice(SDL_GPUDevice *device, T *object)
{
}

template <class T> struct DeviceOwned
{
    DeviceOwned(SDL_GPUDevice *device, T *object) : m_device{device}, m_object{object} {};
    DeviceOwned(const DeviceOwned &) = delete;

    DeviceOwned &operator=(const DeviceOwned &) = delete;

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

struct Haptic
{
    Haptic(SDL_Haptic *haptic, int effect) : m_haptic{haptic}, m_effect{effect} {};
    Haptic(const Haptic &) = delete;

    Haptic &operator=(const Haptic &) = delete;

    ~Haptic()
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

struct Properties
{
    Properties(SDL_PropertiesID props) : m_properties{props} {};
    Properties(const Properties &) = delete;

    Properties &operator=(const Properties &) = delete;

    ~Properties()
    {
        SDL_DestroyProperties(m_properties);
    }

  private:
    SDL_PropertiesID m_properties;
};

struct CameraSurface
{
    CameraSurface(SDL_Surface *surface, SDL_Camera *camera)
        : m_surface{surface}, m_camera{camera} {};
    CameraSurface(const CameraSurface &) = delete;

    CameraSurface &operator=(const CameraSurface &) = delete;

    ~CameraSurface()
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
