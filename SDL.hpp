#pragma once

namespace zdl
{

template <class T> void Release(T *t)
{
}

template <> void Release<SDL_Window>(SDL_Window *t)
{
    SDL_DestroyWindow(t);
}

template <> void Release<SDL_Renderer>(SDL_Renderer *t)
{
    SDL_DestroyRenderer(t);
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

using Window = UniquePointer<SDL_Window>;
using Renderer = UniquePointer<SDL_Renderer>;

inline void SDLThrow(std::source_location callerLocation)
{
    const char *unknownError = "(unknown)";
    const char *pErrorMessage = SDL_GetError();

    if (pErrorMessage == nullptr)
    {
        pErrorMessage = unknownError;
    }

    constexpr size_t NumChars = 256;
    char errorWithFileInfo[NumChars];
    SDL_snprintf(errorWithFileInfo, NumChars, "SDL error: %s (%s:%d)", pErrorMessage,
                 callerLocation.file_name(), callerLocation.line());

    SDL_Log("%s", errorWithFileInfo);

    throw std::runtime_error{errorWithFileInfo};
}

inline void Init(SDL_InitFlags flags,
                 std::source_location location = std::source_location::current())
{
    if (!SDL_Init(flags))
    {
        SDLThrow(location);
    }
}

inline void CreateWindowAndRenderer(const char *title, int width, int height,
                                    SDL_WindowFlags window_flags, SDL_Window **window,
                                    SDL_Renderer **renderer,
                                    std::source_location location = std::source_location::current())
{
    if (!SDL_CreateWindowAndRenderer(title, width, height, window_flags, window, renderer))
    {
        SDLThrow(location);
    }
}

inline void SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                               std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderDrawColor(renderer, r, g, b, a))
    {
        SDLThrow(location);
    }
}

inline void RenderClear(SDL_Renderer *renderer,
                        std::source_location location = std::source_location::current())
{
    if (!SDL_RenderClear(renderer))
    {
        SDLThrow(location);
    }
}

inline void RenderGeometry(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Vertex *vertices,
                           int num_vertices, const int *indices, int num_indices,
                           std::source_location location = std::source_location::current())
{
    if (!SDL_RenderGeometry(renderer, texture, vertices, num_vertices, indices, num_indices))
    {
        SDLThrow(location);
    }
}

inline void RenderPresent(SDL_Renderer *renderer,
                          std::source_location location = std::source_location::current())
{
    if (!SDL_RenderPresent(renderer))
    {
        SDLThrow(location);
    }
}

inline void GetWindowSize(SDL_Window *window, int *w, int *h,
                          std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowSize(window, w, h))
    {
        SDLThrow(location);
    }
}

} // namespace zdl
