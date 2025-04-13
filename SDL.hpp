#pragma once

#include <SDL3/SDL.h>

#include <memory>
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

struct Haptic
{
    Haptic(SDL_Haptic *haptic, int effect) : m_haptic{haptic}, m_effect{effect} {};

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

template<>
void Release<SDL_Environment>(SDL_Environment *env)
{
    SDL_DestroyEnvironment(env);
}

auto EnvironmentDeleter = [](SDL_Environment* object) { Release(object); };
using Environment = std::unique_ptr<SDL_Environment, EnvironmentDeleter>;

template<>
void Release<SDL_AsyncIOQueue>(SDL_AsyncIOQueue *queue)
{
    SDL_DestroyAsyncIOQueue(queue);
}

auto AsyncIOQueueDeleter = [](SDL_AsyncIOQueue* object) { Release(object); };
using AsyncIOQueue = std::unique_ptr<SDL_AsyncIOQueue, AsyncIOQueueDeleter>;

template<>
void Release<SDL_Mutex>(SDL_Mutex *mutex)
{
    SDL_DestroyMutex(mutex);
}

auto MutexDeleter = [](SDL_Mutex* object) { Release(object); };
using Mutex = std::unique_ptr<SDL_Mutex, MutexDeleter>;

template<>
void Release<SDL_RWLock>(SDL_RWLock *rwlock)
{
    SDL_DestroyRWLock(rwlock);
}

auto RWLockDeleter = [](SDL_RWLock* object) { Release(object); };
using RWLock = std::unique_ptr<SDL_RWLock, RWLockDeleter>;

template<>
void Release<SDL_Semaphore>(SDL_Semaphore *sem)
{
    SDL_DestroySemaphore(sem);
}

auto SemaphoreDeleter = [](SDL_Semaphore* object) { Release(object); };
using Semaphore = std::unique_ptr<SDL_Semaphore, SemaphoreDeleter>;

template<>
void Release<SDL_Condition>(SDL_Condition *cond)
{
    SDL_DestroyCondition(cond);
}

auto ConditionDeleter = [](SDL_Condition* object) { Release(object); };
using Condition = std::unique_ptr<SDL_Condition, ConditionDeleter>;

template<>
void Release<SDL_AudioStream>(SDL_AudioStream *stream)
{
    SDL_DestroyAudioStream(stream);
}

auto AudioStreamDeleter = [](SDL_AudioStream* object) { Release(object); };
using AudioStream = std::unique_ptr<SDL_AudioStream, AudioStreamDeleter>;

template<>
void Release<SDL_Palette>(SDL_Palette *palette)
{
    SDL_DestroyPalette(palette);
}

auto PaletteDeleter = [](SDL_Palette* object) { Release(object); };
using Palette = std::unique_ptr<SDL_Palette, PaletteDeleter>;

template<>
void Release<SDL_Surface>(SDL_Surface *surface)
{
    SDL_DestroySurface(surface);
}

auto SurfaceDeleter = [](SDL_Surface* object) { Release(object); };
using Surface = std::unique_ptr<SDL_Surface, SurfaceDeleter>;

template<>
void Release<SDL_Window>(SDL_Window *window)
{
    SDL_DestroyWindowSurface(window);
}

auto WindowDeleter = [](SDL_Window* object) { Release(object); };
using Window = std::unique_ptr<SDL_Window, WindowDeleter>;

template<>
void Release<SDL_Cursor>(SDL_Cursor *cursor)
{
    SDL_DestroyCursor(cursor);
}

auto CursorDeleter = [](SDL_Cursor* object) { Release(object); };
using Cursor = std::unique_ptr<SDL_Cursor, CursorDeleter>;

template<>
void Release<SDL_GPUDevice>(SDL_GPUDevice *device)
{
    SDL_DestroyGPUDevice(device);
}

auto GPUDeviceDeleter = [](SDL_GPUDevice* object) { Release(object); };
using GPUDevice = std::unique_ptr<SDL_GPUDevice, GPUDeviceDeleter>;

template<>
void ReleaseFromDevice<SDL_GPUTexture>(SDL_GPUDevice *device,SDL_GPUTexture *texture)
{
    SDL_ReleaseGPUTexture(device, texture);
}

template<>
void ReleaseFromDevice<SDL_GPUSampler>(SDL_GPUDevice *device,SDL_GPUSampler *sampler)
{
    SDL_ReleaseGPUSampler(device, sampler);
}

template<>
void ReleaseFromDevice<SDL_GPUBuffer>(SDL_GPUDevice *device,SDL_GPUBuffer *buffer)
{
    SDL_ReleaseGPUBuffer(device, buffer);
}

template<>
void ReleaseFromDevice<SDL_GPUTransferBuffer>(SDL_GPUDevice *device,SDL_GPUTransferBuffer *transfer_buffer)
{
    SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
}

template<>
void ReleaseFromDevice<SDL_GPUComputePipeline>(SDL_GPUDevice *device,SDL_GPUComputePipeline *compute_pipeline)
{
    SDL_ReleaseGPUComputePipeline(device, compute_pipeline);
}

template<>
void ReleaseFromDevice<SDL_GPUShader>(SDL_GPUDevice *device,SDL_GPUShader *shader)
{
    SDL_ReleaseGPUShader(device, shader);
}

template<>
void ReleaseFromDevice<SDL_GPUGraphicsPipeline>(SDL_GPUDevice *device,SDL_GPUGraphicsPipeline *graphics_pipeline)
{
    SDL_ReleaseGPUGraphicsPipeline(device, graphics_pipeline);
}

template<>
void ReleaseFromDevice<SDL_Window>(SDL_GPUDevice *device,SDL_Window *window)
{
    SDL_ReleaseWindowFromGPUDevice(device, window);
}

template<>
void ReleaseFromDevice<SDL_GPUFence>(SDL_GPUDevice *device,SDL_GPUFence *fence)
{
    SDL_ReleaseGPUFence(device, fence);
}

template<>
void Release<SDL_Process>(SDL_Process *process)
{
    SDL_DestroyProcess(process);
}

auto ProcessDeleter = [](SDL_Process* object) { Release(object); };
using Process = std::unique_ptr<SDL_Process, ProcessDeleter>;

template<>
void Release<SDL_Texture>(SDL_Texture *texture)
{
    SDL_DestroyTexture(texture);
}

auto TextureDeleter = [](SDL_Texture* object) { Release(object); };
using Texture = std::unique_ptr<SDL_Texture, TextureDeleter>;

template<>
void Release<SDL_Renderer>(SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
}

auto RendererDeleter = [](SDL_Renderer* object) { Release(object); };
using Renderer = std::unique_ptr<SDL_Renderer, RendererDeleter>;

template<>
void Release<SDL_Tray>(SDL_Tray *tray)
{
    SDL_DestroyTray(tray);
}

auto TrayDeleter = [](SDL_Tray* object) { Release(object); };
using Tray = std::unique_ptr<SDL_Tray, TrayDeleter>;

inline void *malloc(size_t size, std::source_location location = std::source_location::current())
{
    void *result = SDL_malloc(size);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void *calloc(size_t nmemb, size_t size, std::source_location location = std::source_location::current())
{
    void *result = SDL_calloc(nmemb, size);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void *realloc(void *mem, size_t size, std::source_location location = std::source_location::current())
{
    void *result = SDL_realloc(mem, size);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void free(void *mem)
{
    SDL_free(mem);
}

inline void GetOriginalMemoryFunctions(SDL_malloc_func *malloc_func, SDL_calloc_func *calloc_func, SDL_realloc_func *realloc_func, SDL_free_func *free_func)
{
    SDL_GetOriginalMemoryFunctions(malloc_func, calloc_func, realloc_func, free_func);
}

inline void GetMemoryFunctions(SDL_malloc_func *malloc_func, SDL_calloc_func *calloc_func, SDL_realloc_func *realloc_func, SDL_free_func *free_func)
{
    SDL_GetMemoryFunctions(malloc_func, calloc_func, realloc_func, free_func);
}

inline void SetMemoryFunctions(SDL_malloc_func malloc_func, SDL_calloc_func calloc_func, SDL_realloc_func realloc_func, SDL_free_func free_func, std::source_location location = std::source_location::current())
{
    if (!SDL_SetMemoryFunctions(malloc_func, calloc_func, realloc_func, free_func))
    {
        SDLThrow(location);
    }
}

inline void *aligned_alloc(size_t alignment, size_t size, std::source_location location = std::source_location::current())
{
    void *result = SDL_aligned_alloc(alignment, size);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void aligned_free(void *mem)
{
    SDL_aligned_free(mem);
}

inline int GetNumAllocations()
{
    return SDL_GetNumAllocations();
}

inline SDL_Environment *GetEnvironment(std::source_location location = std::source_location::current())
{
    SDL_Environment *result = SDL_GetEnvironment();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Environment *CreateEnvironment(bool populated, std::source_location location = std::source_location::current())
{
    SDL_Environment *result = SDL_CreateEnvironment(populated);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetEnvironmentVariable(SDL_Environment *env, const char *name, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetEnvironmentVariable(env, name);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char **GetEnvironmentVariables(SDL_Environment *env, std::source_location location = std::source_location::current())
{
    char **result = SDL_GetEnvironmentVariables(env);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetEnvironmentVariable(SDL_Environment *env, const char *name, const char *value, bool overwrite, std::source_location location = std::source_location::current())
{
    if (!SDL_SetEnvironmentVariable(env, name, value, overwrite))
    {
        SDLThrow(location);
    }
}

inline void UnsetEnvironmentVariable(SDL_Environment *env, const char *name, std::source_location location = std::source_location::current())
{
    if (!SDL_UnsetEnvironmentVariable(env, name))
    {
        SDLThrow(location);
    }
}

inline void DestroyEnvironment(SDL_Environment *env)
{
    SDL_DestroyEnvironment(env);
}

inline const char *getenv(const char *name, std::source_location location = std::source_location::current())
{
    const char *result = SDL_getenv(name);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *getenv_unsafe(const char *name, std::source_location location = std::source_location::current())
{
    const char *result = SDL_getenv_unsafe(name);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int setenv_unsafe(const char *name, const char *value, int overwrite)
{
    return SDL_setenv_unsafe(name, value, overwrite);
}

inline int unsetenv_unsafe(const char *name)
{
    return SDL_unsetenv_unsafe(name);
}

inline void qsort(void *base, size_t nmemb, size_t size, SDL_CompareCallback compare)
{
    SDL_qsort(base, nmemb, size, compare);
}

inline void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, SDL_CompareCallback compare, std::source_location location = std::source_location::current())
{
    void *result = SDL_bsearch(key, base, nmemb, size, compare);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void qsort_r(void *base, size_t nmemb, size_t size, SDL_CompareCallback_r compare, void *userdata)
{
    SDL_qsort_r(base, nmemb, size, compare, userdata);
}

inline void *bsearch_r(const void *key, const void *base, size_t nmemb, size_t size, SDL_CompareCallback_r compare, void *userdata, std::source_location location = std::source_location::current())
{
    void *result = SDL_bsearch_r(key, base, nmemb, size, compare, userdata);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int abs(int x)
{
    return SDL_abs(x);
}

inline int isalpha(int x)
{
    return SDL_isalpha(x);
}

inline int isalnum(int x)
{
    return SDL_isalnum(x);
}

inline int isblank(int x)
{
    return SDL_isblank(x);
}

inline int iscntrl(int x)
{
    return SDL_iscntrl(x);
}

inline int isdigit(int x)
{
    return SDL_isdigit(x);
}

inline int isxdigit(int x)
{
    return SDL_isxdigit(x);
}

inline int ispunct(int x)
{
    return SDL_ispunct(x);
}

inline int isspace(int x)
{
    return SDL_isspace(x);
}

inline int isupper(int x)
{
    return SDL_isupper(x);
}

inline int islower(int x)
{
    return SDL_islower(x);
}

inline int isprint(int x)
{
    return SDL_isprint(x);
}

inline int isgraph(int x)
{
    return SDL_isgraph(x);
}

inline int toupper(int x)
{
    return SDL_toupper(x);
}

inline int tolower(int x)
{
    return SDL_tolower(x);
}

inline Uint16 crc16(Uint16 crc, const void *data, size_t len)
{
    return SDL_crc16(crc, data, len);
}

inline Uint32 crc32(Uint32 crc, const void *data, size_t len)
{
    return SDL_crc32(crc, data, len);
}

inline Uint32 murmur3_32(const void *data, size_t len, Uint32 seed)
{
    return SDL_murmur3_32(data, len, seed);
}

inline void *memcpy(void *dst, const void *src, size_t len, std::source_location location = std::source_location::current())
{
    void *result = SDL_memcpy(dst, src, len);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void *memmove(void *dst, const void *src, size_t len, std::source_location location = std::source_location::current())
{
    void *result = SDL_memmove(dst, src, len);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void *memset(void *dst, int c, size_t len, std::source_location location = std::source_location::current())
{
    void *result = SDL_memset(dst, c, len);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void *memset4(void *dst, Uint32 val, size_t dwords, std::source_location location = std::source_location::current())
{
    void *result = SDL_memset4(dst, val, dwords);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int memcmp(const void *s1, const void *s2, size_t len)
{
    return SDL_memcmp(s1, s2, len);
}

inline size_t wcslen(const wchar_t *wstr)
{
    return SDL_wcslen(wstr);
}

inline size_t wcsnlen(const wchar_t *wstr, size_t maxlen)
{
    return SDL_wcsnlen(wstr, maxlen);
}

inline size_t wcslcpy(wchar_t *dst, const wchar_t *src, size_t maxlen)
{
    return SDL_wcslcpy(dst, src, maxlen);
}

inline size_t wcslcat(wchar_t *dst, const wchar_t *src, size_t maxlen)
{
    return SDL_wcslcat(dst, src, maxlen);
}

inline wchar_t *wcsdup(const wchar_t *wstr, std::source_location location = std::source_location::current())
{
    wchar_t *result = SDL_wcsdup(wstr);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline wchar_t *wcsstr(const wchar_t *haystack, const wchar_t *needle, std::source_location location = std::source_location::current())
{
    wchar_t *result = SDL_wcsstr(haystack, needle);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline wchar_t *wcsnstr(const wchar_t *haystack, const wchar_t *needle, size_t maxlen, std::source_location location = std::source_location::current())
{
    wchar_t *result = SDL_wcsnstr(haystack, needle, maxlen);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int wcscmp(const wchar_t *str1, const wchar_t *str2)
{
    return SDL_wcscmp(str1, str2);
}

inline int wcsncmp(const wchar_t *str1, const wchar_t *str2, size_t maxlen)
{
    return SDL_wcsncmp(str1, str2, maxlen);
}

inline int wcscasecmp(const wchar_t *str1, const wchar_t *str2)
{
    return SDL_wcscasecmp(str1, str2);
}

inline int wcsncasecmp(const wchar_t *str1, const wchar_t *str2, size_t maxlen)
{
    return SDL_wcsncasecmp(str1, str2, maxlen);
}

inline long wcstol(const wchar_t *str, wchar_t **endp, int base)
{
    return SDL_wcstol(str, endp, base);
}

inline size_t strlen(const char *str)
{
    return SDL_strlen(str);
}

inline size_t strnlen(const char *str, size_t maxlen)
{
    return SDL_strnlen(str, maxlen);
}

inline size_t strlcpy(char *dst, const char *src, size_t maxlen)
{
    return SDL_strlcpy(dst, src, maxlen);
}

inline size_t utf8strlcpy(char *dst, const char *src, size_t dst_bytes)
{
    return SDL_utf8strlcpy(dst, src, dst_bytes);
}

inline size_t strlcat(char *dst, const char *src, size_t maxlen)
{
    return SDL_strlcat(dst, src, maxlen);
}

inline char *strdup(const char *str, std::source_location location = std::source_location::current())
{
    char *result = SDL_strdup(str);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *strndup(const char *str, size_t maxlen, std::source_location location = std::source_location::current())
{
    char *result = SDL_strndup(str, maxlen);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *strrev(char *str, std::source_location location = std::source_location::current())
{
    char *result = SDL_strrev(str);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *strupr(char *str, std::source_location location = std::source_location::current())
{
    char *result = SDL_strupr(str);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *strlwr(char *str, std::source_location location = std::source_location::current())
{
    char *result = SDL_strlwr(str);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *strchr(const char *str, int c, std::source_location location = std::source_location::current())
{
    char *result = SDL_strchr(str, c);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *strrchr(const char *str, int c, std::source_location location = std::source_location::current())
{
    char *result = SDL_strrchr(str, c);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *strstr(const char *haystack, const char *needle, std::source_location location = std::source_location::current())
{
    char *result = SDL_strstr(haystack, needle);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *strnstr(const char *haystack, const char *needle, size_t maxlen, std::source_location location = std::source_location::current())
{
    char *result = SDL_strnstr(haystack, needle, maxlen);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *strcasestr(const char *haystack, const char *needle, std::source_location location = std::source_location::current())
{
    char *result = SDL_strcasestr(haystack, needle);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *strtok_r(char *str, const char *delim, char **saveptr, std::source_location location = std::source_location::current())
{
    char *result = SDL_strtok_r(str, delim, saveptr);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline size_t utf8strlen(const char *str)
{
    return SDL_utf8strlen(str);
}

inline size_t utf8strnlen(const char *str, size_t bytes)
{
    return SDL_utf8strnlen(str, bytes);
}

inline char *itoa(int value, char *str, int radix, std::source_location location = std::source_location::current())
{
    char *result = SDL_itoa(value, str, radix);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *uitoa(unsigned int value, char *str, int radix, std::source_location location = std::source_location::current())
{
    char *result = SDL_uitoa(value, str, radix);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *ltoa(long value, char *str, int radix, std::source_location location = std::source_location::current())
{
    char *result = SDL_ltoa(value, str, radix);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *ultoa(unsigned long value, char *str, int radix, std::source_location location = std::source_location::current())
{
    char *result = SDL_ultoa(value, str, radix);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *lltoa(long long value, char *str, int radix, std::source_location location = std::source_location::current())
{
    char *result = SDL_lltoa(value, str, radix);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *ulltoa(unsigned long long value, char *str, int radix, std::source_location location = std::source_location::current())
{
    char *result = SDL_ulltoa(value, str, radix);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int atoi(const char *str)
{
    return SDL_atoi(str);
}

inline double atof(const char *str)
{
    return SDL_atof(str);
}

inline long strtol(const char *str, char **endp, int base)
{
    return SDL_strtol(str, endp, base);
}

inline unsigned long strtoul(const char *str, char **endp, int base)
{
    return SDL_strtoul(str, endp, base);
}

inline long long strtoll(const char *str, char **endp, int base)
{
    return SDL_strtoll(str, endp, base);
}

inline unsigned long long strtoull(const char *str, char **endp, int base)
{
    return SDL_strtoull(str, endp, base);
}

inline double strtod(const char *str, char **endp)
{
    return SDL_strtod(str, endp);
}

inline int strcmp(const char *str1, const char *str2)
{
    return SDL_strcmp(str1, str2);
}

inline int strncmp(const char *str1, const char *str2, size_t maxlen)
{
    return SDL_strncmp(str1, str2, maxlen);
}

inline int strcasecmp(const char *str1, const char *str2)
{
    return SDL_strcasecmp(str1, str2);
}

inline int strncasecmp(const char *str1, const char *str2, size_t maxlen)
{
    return SDL_strncasecmp(str1, str2, maxlen);
}

inline char *strpbrk(const char *str, const char *breakset, std::source_location location = std::source_location::current())
{
    char *result = SDL_strpbrk(str, breakset);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline Uint32 StepUTF8(const char **pstr, size_t *pslen)
{
    return SDL_StepUTF8(pstr, pslen);
}

inline Uint32 StepBackUTF8(const char *start, const char **pstr)
{
    return SDL_StepBackUTF8(start, pstr);
}

inline char *UCS4ToUTF8(Uint32 codepoint, char *dst, std::source_location location = std::source_location::current())
{
    char *result = SDL_UCS4ToUTF8(codepoint, dst);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int sscanf(const char *text, const char *fmt)
{
    return SDL_sscanf(text, fmt);
}

inline int vsscanf(const char *text, const char *fmt, va_list ap)
{
    return SDL_vsscanf(text, fmt, ap);
}

inline int snprintf(char *text, size_t maxlen, const char *fmt)
{
    return SDL_snprintf(text, maxlen, fmt);
}

inline int swprintf(wchar_t *text, size_t maxlen, const wchar_t *fmt)
{
    return SDL_swprintf(text, maxlen, fmt);
}

inline int vsnprintf(char *text, size_t maxlen, const char *fmt, va_list ap)
{
    return SDL_vsnprintf(text, maxlen, fmt, ap);
}

inline int vswprintf(wchar_t *text, size_t maxlen, const wchar_t *fmt, va_list ap)
{
    return SDL_vswprintf(text, maxlen, fmt, ap);
}

inline int asprintf(char **strp, const char *fmt)
{
    return SDL_asprintf(strp, fmt);
}

inline int vasprintf(char **strp, const char *fmt, va_list ap)
{
    return SDL_vasprintf(strp, fmt, ap);
}

inline void srand(Uint64 seed)
{
    SDL_srand(seed);
}

inline Sint32 rand(Sint32 n)
{
    return SDL_rand(n);
}

inline float randf()
{
    return SDL_randf();
}

inline Uint32 rand_bits()
{
    return SDL_rand_bits();
}

inline Sint32 rand_r(Uint64 *state, Sint32 n)
{
    return SDL_rand_r(state, n);
}

inline float randf_r(Uint64 *state)
{
    return SDL_randf_r(state);
}

inline Uint32 rand_bits_r(Uint64 *state)
{
    return SDL_rand_bits_r(state);
}

inline double acos(double x)
{
    return SDL_acos(x);
}

inline float acosf(float x)
{
    return SDL_acosf(x);
}

inline double asin(double x)
{
    return SDL_asin(x);
}

inline float asinf(float x)
{
    return SDL_asinf(x);
}

inline double atan(double x)
{
    return SDL_atan(x);
}

inline float atanf(float x)
{
    return SDL_atanf(x);
}

inline double atan2(double y, double x)
{
    return SDL_atan2(y, x);
}

inline float atan2f(float y, float x)
{
    return SDL_atan2f(y, x);
}

inline double ceil(double x)
{
    return SDL_ceil(x);
}

inline float ceilf(float x)
{
    return SDL_ceilf(x);
}

inline double copysign(double x, double y)
{
    return SDL_copysign(x, y);
}

inline float copysignf(float x, float y)
{
    return SDL_copysignf(x, y);
}

inline double cos(double x)
{
    return SDL_cos(x);
}

inline float cosf(float x)
{
    return SDL_cosf(x);
}

inline double exp(double x)
{
    return SDL_exp(x);
}

inline float expf(float x)
{
    return SDL_expf(x);
}

inline double fabs(double x)
{
    return SDL_fabs(x);
}

inline float fabsf(float x)
{
    return SDL_fabsf(x);
}

inline double floor(double x)
{
    return SDL_floor(x);
}

inline float floorf(float x)
{
    return SDL_floorf(x);
}

inline double trunc(double x)
{
    return SDL_trunc(x);
}

inline float truncf(float x)
{
    return SDL_truncf(x);
}

inline double fmod(double x, double y)
{
    return SDL_fmod(x, y);
}

inline float fmodf(float x, float y)
{
    return SDL_fmodf(x, y);
}

inline int isinf(double x)
{
    return SDL_isinf(x);
}

inline int isinff(float x)
{
    return SDL_isinff(x);
}

inline int isnan(double x)
{
    return SDL_isnan(x);
}

inline int isnanf(float x)
{
    return SDL_isnanf(x);
}

inline double log(double x)
{
    return SDL_log(x);
}

inline float logf(float x)
{
    return SDL_logf(x);
}

inline double log10(double x)
{
    return SDL_log10(x);
}

inline float log10f(float x)
{
    return SDL_log10f(x);
}

inline double modf(double x, double *y)
{
    return SDL_modf(x, y);
}

inline float modff(float x, float *y)
{
    return SDL_modff(x, y);
}

inline double pow(double x, double y)
{
    return SDL_pow(x, y);
}

inline float powf(float x, float y)
{
    return SDL_powf(x, y);
}

inline double round(double x)
{
    return SDL_round(x);
}

inline float roundf(float x)
{
    return SDL_roundf(x);
}

inline long lround(double x)
{
    return SDL_lround(x);
}

inline long lroundf(float x)
{
    return SDL_lroundf(x);
}

inline double scalbn(double x, int n)
{
    return SDL_scalbn(x, n);
}

inline float scalbnf(float x, int n)
{
    return SDL_scalbnf(x, n);
}

inline double sin(double x)
{
    return SDL_sin(x);
}

inline float sinf(float x)
{
    return SDL_sinf(x);
}

inline double sqrt(double x)
{
    return SDL_sqrt(x);
}

inline float sqrtf(float x)
{
    return SDL_sqrtf(x);
}

inline double tan(double x)
{
    return SDL_tan(x);
}

inline float tanf(float x)
{
    return SDL_tanf(x);
}

inline SDL_iconv_t iconv_open(const char *tocode, const char *fromcode)
{
    return SDL_iconv_open(tocode, fromcode);
}

inline int iconv_close(SDL_iconv_t cd)
{
    return SDL_iconv_close(cd);
}

inline size_t iconv(SDL_iconv_t cd, const char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft)
{
    return SDL_iconv(cd, inbuf, inbytesleft, outbuf, outbytesleft);
}

inline char *iconv_string(const char *tocode, const char *fromcode, const char *inbuf, size_t inbytesleft, std::source_location location = std::source_location::current())
{
    char *result = SDL_iconv_string(tocode, fromcode, inbuf, inbytesleft);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void size_mul_check_overflow(size_t a, size_t b, size_t *ret, std::source_location location = std::source_location::current())
{
    if (!SDL_size_mul_check_overflow(a, b, ret))
    {
        SDLThrow(location);
    }
}

inline void size_add_check_overflow(size_t a, size_t b, size_t *ret, std::source_location location = std::source_location::current())
{
    if (!SDL_size_add_check_overflow(a, b, ret))
    {
        SDLThrow(location);
    }
}

inline SDL_AssertState ReportAssertion(SDL_AssertData *data, const char *func, const char *file, int line)
{
    return SDL_ReportAssertion(data, func, file, line);
}

inline void SetAssertionHandler(SDL_AssertionHandler handler, void *userdata)
{
    SDL_SetAssertionHandler(handler, userdata);
}

inline SDL_AssertionHandler GetDefaultAssertionHandler()
{
    return SDL_GetDefaultAssertionHandler();
}

inline SDL_AssertionHandler GetAssertionHandler(void **puserdata)
{
    return SDL_GetAssertionHandler(puserdata);
}

inline const SDL_AssertData *GetAssertionReport(std::source_location location = std::source_location::current())
{
    const SDL_AssertData *result = SDL_GetAssertionReport();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void ResetAssertionReport()
{
    SDL_ResetAssertionReport();
}

inline SDL_AsyncIO *AsyncIOFromFile(const char *file, const char *mode, std::source_location location = std::source_location::current())
{
    SDL_AsyncIO *result = SDL_AsyncIOFromFile(file, mode);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline Sint64 GetAsyncIOSize(SDL_AsyncIO *asyncio)
{
    return SDL_GetAsyncIOSize(asyncio);
}

inline void ReadAsyncIO(SDL_AsyncIO *asyncio, void *ptr, Uint64 offset, Uint64 size, SDL_AsyncIOQueue *queue, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadAsyncIO(asyncio, ptr, offset, size, queue, userdata))
    {
        SDLThrow(location);
    }
}

inline void WriteAsyncIO(SDL_AsyncIO *asyncio, void *ptr, Uint64 offset, Uint64 size, SDL_AsyncIOQueue *queue, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteAsyncIO(asyncio, ptr, offset, size, queue, userdata))
    {
        SDLThrow(location);
    }
}

inline void CloseAsyncIO(SDL_AsyncIO *asyncio, bool flush, SDL_AsyncIOQueue *queue, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_CloseAsyncIO(asyncio, flush, queue, userdata))
    {
        SDLThrow(location);
    }
}

inline SDL_AsyncIOQueue *CreateAsyncIOQueue(std::source_location location = std::source_location::current())
{
    SDL_AsyncIOQueue *result = SDL_CreateAsyncIOQueue();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void DestroyAsyncIOQueue(SDL_AsyncIOQueue *queue)
{
    SDL_DestroyAsyncIOQueue(queue);
}

inline void GetAsyncIOResult(SDL_AsyncIOQueue *queue, SDL_AsyncIOOutcome *outcome, std::source_location location = std::source_location::current())
{
    if (!SDL_GetAsyncIOResult(queue, outcome))
    {
        SDLThrow(location);
    }
}

inline void WaitAsyncIOResult(SDL_AsyncIOQueue *queue, SDL_AsyncIOOutcome *outcome, Sint32 timeoutMS, std::source_location location = std::source_location::current())
{
    if (!SDL_WaitAsyncIOResult(queue, outcome, timeoutMS))
    {
        SDLThrow(location);
    }
}

inline void SignalAsyncIOQueue(SDL_AsyncIOQueue *queue)
{
    SDL_SignalAsyncIOQueue(queue);
}

inline void LoadFileAsync(const char *file, SDL_AsyncIOQueue *queue, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_LoadFileAsync(file, queue, userdata))
    {
        SDLThrow(location);
    }
}

inline void TryLockSpinlock(SDL_SpinLock *lock, std::source_location location = std::source_location::current())
{
    if (!SDL_TryLockSpinlock(lock))
    {
        SDLThrow(location);
    }
}

inline void LockSpinlock(SDL_SpinLock *lock)
{
    SDL_LockSpinlock(lock);
}

inline void UnlockSpinlock(SDL_SpinLock *lock)
{
    SDL_UnlockSpinlock(lock);
}

inline void MemoryBarrierReleaseFunction()
{
    SDL_MemoryBarrierReleaseFunction();
}

inline void MemoryBarrierAcquireFunction()
{
    SDL_MemoryBarrierAcquireFunction();
}

inline void CompareAndSwapAtomicInt(SDL_AtomicInt *a, int oldval, int newval, std::source_location location = std::source_location::current())
{
    if (!SDL_CompareAndSwapAtomicInt(a, oldval, newval))
    {
        SDLThrow(location);
    }
}

inline int SetAtomicInt(SDL_AtomicInt *a, int v)
{
    return SDL_SetAtomicInt(a, v);
}

inline int GetAtomicInt(SDL_AtomicInt *a)
{
    return SDL_GetAtomicInt(a);
}

inline int AddAtomicInt(SDL_AtomicInt *a, int v)
{
    return SDL_AddAtomicInt(a, v);
}

inline void CompareAndSwapAtomicU32(SDL_AtomicU32 *a, Uint32 oldval, Uint32 newval, std::source_location location = std::source_location::current())
{
    if (!SDL_CompareAndSwapAtomicU32(a, oldval, newval))
    {
        SDLThrow(location);
    }
}

inline Uint32 SetAtomicU32(SDL_AtomicU32 *a, Uint32 v)
{
    return SDL_SetAtomicU32(a, v);
}

inline Uint32 GetAtomicU32(SDL_AtomicU32 *a)
{
    return SDL_GetAtomicU32(a);
}

inline void CompareAndSwapAtomicPointer(void **a, void *oldval, void *newval, std::source_location location = std::source_location::current())
{
    if (!SDL_CompareAndSwapAtomicPointer(a, oldval, newval))
    {
        SDLThrow(location);
    }
}

inline void *SetAtomicPointer(void **a, void *v, std::source_location location = std::source_location::current())
{
    void *result = SDL_SetAtomicPointer(a, v);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void *GetAtomicPointer(void **a, std::source_location location = std::source_location::current())
{
    void *result = SDL_GetAtomicPointer(a);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline float SwapFloat(float x)
{
    return SDL_SwapFloat(x);
}

inline void SetError(const char *fmt, std::source_location location = std::source_location::current())
{
    if (!SDL_SetError(fmt))
    {
        SDLThrow(location);
    }
}

inline void SetErrorV(const char *fmt, va_list ap, std::source_location location = std::source_location::current())
{
    if (!SDL_SetErrorV(fmt, ap))
    {
        SDLThrow(location);
    }
}

inline void OutOfMemory(std::source_location location = std::source_location::current())
{
    if (!SDL_OutOfMemory())
    {
        SDLThrow(location);
    }
}

inline const char *GetError(std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetError();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void ClearError(std::source_location location = std::source_location::current())
{
    if (!SDL_ClearError())
    {
        SDLThrow(location);
    }
}

inline SDL_PropertiesID GetGlobalProperties()
{
    return SDL_GetGlobalProperties();
}

inline SDL_PropertiesID CreateProperties()
{
    return SDL_CreateProperties();
}

inline void CopyProperties(SDL_PropertiesID src, SDL_PropertiesID dst, std::source_location location = std::source_location::current())
{
    if (!SDL_CopyProperties(src, dst))
    {
        SDLThrow(location);
    }
}

inline void LockProperties(SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    if (!SDL_LockProperties(props))
    {
        SDLThrow(location);
    }
}

inline void UnlockProperties(SDL_PropertiesID props)
{
    SDL_UnlockProperties(props);
}

inline void SetPointerPropertyWithCleanup(SDL_PropertiesID props, const char *name, void *value, SDL_CleanupPropertyCallback cleanup, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_SetPointerPropertyWithCleanup(props, name, value, cleanup, userdata))
    {
        SDLThrow(location);
    }
}

inline void SetPointerProperty(SDL_PropertiesID props, const char *name, void *value, std::source_location location = std::source_location::current())
{
    if (!SDL_SetPointerProperty(props, name, value))
    {
        SDLThrow(location);
    }
}

inline void SetStringProperty(SDL_PropertiesID props, const char *name, const char *value, std::source_location location = std::source_location::current())
{
    if (!SDL_SetStringProperty(props, name, value))
    {
        SDLThrow(location);
    }
}

inline void SetNumberProperty(SDL_PropertiesID props, const char *name, Sint64 value, std::source_location location = std::source_location::current())
{
    if (!SDL_SetNumberProperty(props, name, value))
    {
        SDLThrow(location);
    }
}

inline void SetFloatProperty(SDL_PropertiesID props, const char *name, float value, std::source_location location = std::source_location::current())
{
    if (!SDL_SetFloatProperty(props, name, value))
    {
        SDLThrow(location);
    }
}

inline void SetBooleanProperty(SDL_PropertiesID props, const char *name, bool value, std::source_location location = std::source_location::current())
{
    if (!SDL_SetBooleanProperty(props, name, value))
    {
        SDLThrow(location);
    }
}

inline void HasProperty(SDL_PropertiesID props, const char *name, std::source_location location = std::source_location::current())
{
    if (!SDL_HasProperty(props, name))
    {
        SDLThrow(location);
    }
}

inline SDL_PropertyType GetPropertyType(SDL_PropertiesID props, const char *name)
{
    return SDL_GetPropertyType(props, name);
}

inline void *GetPointerProperty(SDL_PropertiesID props, const char *name, void *default_value, std::source_location location = std::source_location::current())
{
    void *result = SDL_GetPointerProperty(props, name, default_value);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetStringProperty(SDL_PropertiesID props, const char *name, const char *default_value, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetStringProperty(props, name, default_value);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline Sint64 GetNumberProperty(SDL_PropertiesID props, const char *name, Sint64 default_value)
{
    return SDL_GetNumberProperty(props, name, default_value);
}

inline float GetFloatProperty(SDL_PropertiesID props, const char *name, float default_value)
{
    return SDL_GetFloatProperty(props, name, default_value);
}

inline void GetBooleanProperty(SDL_PropertiesID props, const char *name, bool default_value, std::source_location location = std::source_location::current())
{
    if (!SDL_GetBooleanProperty(props, name, default_value))
    {
        SDLThrow(location);
    }
}

inline void ClearProperty(SDL_PropertiesID props, const char *name, std::source_location location = std::source_location::current())
{
    if (!SDL_ClearProperty(props, name))
    {
        SDLThrow(location);
    }
}

inline void EnumerateProperties(SDL_PropertiesID props, SDL_EnumeratePropertiesCallback callback, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_EnumerateProperties(props, callback, userdata))
    {
        SDLThrow(location);
    }
}

inline void DestroyProperties(SDL_PropertiesID props)
{
    SDL_DestroyProperties(props);
}

inline SDL_Thread *CreateThreadRuntime(SDL_ThreadFunction fn, const char *name, void *data, SDL_FunctionPointer pfnBeginThread, SDL_FunctionPointer pfnEndThread, std::source_location location = std::source_location::current())
{
    SDL_Thread *result = SDL_CreateThreadRuntime(fn, name, data, pfnBeginThread, pfnEndThread);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Thread *CreateThreadWithPropertiesRuntime(SDL_PropertiesID props, SDL_FunctionPointer pfnBeginThread, SDL_FunctionPointer pfnEndThread, std::source_location location = std::source_location::current())
{
    SDL_Thread *result = SDL_CreateThreadWithPropertiesRuntime(props, pfnBeginThread, pfnEndThread);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetThreadName(SDL_Thread *thread, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetThreadName(thread);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_ThreadID GetCurrentThreadID()
{
    return SDL_GetCurrentThreadID();
}

inline SDL_ThreadID GetThreadID(SDL_Thread *thread)
{
    return SDL_GetThreadID(thread);
}

inline void SetCurrentThreadPriority(SDL_ThreadPriority priority, std::source_location location = std::source_location::current())
{
    if (!SDL_SetCurrentThreadPriority(priority))
    {
        SDLThrow(location);
    }
}

inline void WaitThread(SDL_Thread *thread, int *status)
{
    SDL_WaitThread(thread, status);
}

inline SDL_ThreadState GetThreadState(SDL_Thread *thread)
{
    return SDL_GetThreadState(thread);
}

inline void DetachThread(SDL_Thread *thread)
{
    SDL_DetachThread(thread);
}

inline void *GetTLS(SDL_TLSID *id, std::source_location location = std::source_location::current())
{
    void *result = SDL_GetTLS(id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetTLS(SDL_TLSID *id, const void *value, SDL_TLSDestructorCallback destructor, std::source_location location = std::source_location::current())
{
    if (!SDL_SetTLS(id, value, destructor))
    {
        SDLThrow(location);
    }
}

inline void CleanupTLS()
{
    SDL_CleanupTLS();
}

inline SDL_Mutex *CreateMutex(std::source_location location = std::source_location::current())
{
    SDL_Mutex *result = SDL_CreateMutex();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void LockMutex(SDL_Mutex *mutex)
{
    SDL_LockMutex(mutex);
}

inline void TryLockMutex(SDL_Mutex *mutex, std::source_location location = std::source_location::current())
{
    if (!SDL_TryLockMutex(mutex))
    {
        SDLThrow(location);
    }
}

inline void UnlockMutex(SDL_Mutex *mutex)
{
    SDL_UnlockMutex(mutex);
}

inline void DestroyMutex(SDL_Mutex *mutex)
{
    SDL_DestroyMutex(mutex);
}

inline SDL_RWLock *CreateRWLock(std::source_location location = std::source_location::current())
{
    SDL_RWLock *result = SDL_CreateRWLock();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void LockRWLockForReading(SDL_RWLock *rwlock)
{
    SDL_LockRWLockForReading(rwlock);
}

inline void LockRWLockForWriting(SDL_RWLock *rwlock)
{
    SDL_LockRWLockForWriting(rwlock);
}

inline void TryLockRWLockForReading(SDL_RWLock *rwlock, std::source_location location = std::source_location::current())
{
    if (!SDL_TryLockRWLockForReading(rwlock))
    {
        SDLThrow(location);
    }
}

inline void TryLockRWLockForWriting(SDL_RWLock *rwlock, std::source_location location = std::source_location::current())
{
    if (!SDL_TryLockRWLockForWriting(rwlock))
    {
        SDLThrow(location);
    }
}

inline void UnlockRWLock(SDL_RWLock *rwlock)
{
    SDL_UnlockRWLock(rwlock);
}

inline void DestroyRWLock(SDL_RWLock *rwlock)
{
    SDL_DestroyRWLock(rwlock);
}

inline SDL_Semaphore *CreateSemaphore(Uint32 initial_value, std::source_location location = std::source_location::current())
{
    SDL_Semaphore *result = SDL_CreateSemaphore(initial_value);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void DestroySemaphore(SDL_Semaphore *sem)
{
    SDL_DestroySemaphore(sem);
}

inline void WaitSemaphore(SDL_Semaphore *sem)
{
    SDL_WaitSemaphore(sem);
}

inline void TryWaitSemaphore(SDL_Semaphore *sem, std::source_location location = std::source_location::current())
{
    if (!SDL_TryWaitSemaphore(sem))
    {
        SDLThrow(location);
    }
}

inline void WaitSemaphoreTimeout(SDL_Semaphore *sem, Sint32 timeoutMS, std::source_location location = std::source_location::current())
{
    if (!SDL_WaitSemaphoreTimeout(sem, timeoutMS))
    {
        SDLThrow(location);
    }
}

inline void SignalSemaphore(SDL_Semaphore *sem)
{
    SDL_SignalSemaphore(sem);
}

inline Uint32 GetSemaphoreValue(SDL_Semaphore *sem)
{
    return SDL_GetSemaphoreValue(sem);
}

inline SDL_Condition *CreateCondition(std::source_location location = std::source_location::current())
{
    SDL_Condition *result = SDL_CreateCondition();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void DestroyCondition(SDL_Condition *cond)
{
    SDL_DestroyCondition(cond);
}

inline void SignalCondition(SDL_Condition *cond)
{
    SDL_SignalCondition(cond);
}

inline void BroadcastCondition(SDL_Condition *cond)
{
    SDL_BroadcastCondition(cond);
}

inline void WaitCondition(SDL_Condition *cond, SDL_Mutex *mutex)
{
    SDL_WaitCondition(cond, mutex);
}

inline void WaitConditionTimeout(SDL_Condition *cond, SDL_Mutex *mutex, Sint32 timeoutMS, std::source_location location = std::source_location::current())
{
    if (!SDL_WaitConditionTimeout(cond, mutex, timeoutMS))
    {
        SDLThrow(location);
    }
}

inline void ShouldInit(SDL_InitState *state, std::source_location location = std::source_location::current())
{
    if (!SDL_ShouldInit(state))
    {
        SDLThrow(location);
    }
}

inline void ShouldQuit(SDL_InitState *state, std::source_location location = std::source_location::current())
{
    if (!SDL_ShouldQuit(state))
    {
        SDLThrow(location);
    }
}

inline void SetInitialized(SDL_InitState *state, bool initialized)
{
    SDL_SetInitialized(state, initialized);
}

inline SDL_IOStream *IOFromFile(const char *file, const char *mode, std::source_location location = std::source_location::current())
{
    SDL_IOStream *result = SDL_IOFromFile(file, mode);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_IOStream *IOFromMem(void *mem, size_t size, std::source_location location = std::source_location::current())
{
    SDL_IOStream *result = SDL_IOFromMem(mem, size);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_IOStream *IOFromConstMem(const void *mem, size_t size, std::source_location location = std::source_location::current())
{
    SDL_IOStream *result = SDL_IOFromConstMem(mem, size);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_IOStream *IOFromDynamicMem(std::source_location location = std::source_location::current())
{
    SDL_IOStream *result = SDL_IOFromDynamicMem();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_IOStream *OpenIO(const SDL_IOStreamInterface *iface, void *userdata, std::source_location location = std::source_location::current())
{
    SDL_IOStream *result = SDL_OpenIO(iface, userdata);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void CloseIO(SDL_IOStream *context, std::source_location location = std::source_location::current())
{
    if (!SDL_CloseIO(context))
    {
        SDLThrow(location);
    }
}

inline SDL_PropertiesID GetIOProperties(SDL_IOStream *context)
{
    return SDL_GetIOProperties(context);
}

inline SDL_IOStatus GetIOStatus(SDL_IOStream *context)
{
    return SDL_GetIOStatus(context);
}

inline Sint64 GetIOSize(SDL_IOStream *context)
{
    return SDL_GetIOSize(context);
}

inline Sint64 SeekIO(SDL_IOStream *context, Sint64 offset, SDL_IOWhence whence)
{
    return SDL_SeekIO(context, offset, whence);
}

inline Sint64 TellIO(SDL_IOStream *context)
{
    return SDL_TellIO(context);
}

inline size_t ReadIO(SDL_IOStream *context, void *ptr, size_t size)
{
    return SDL_ReadIO(context, ptr, size);
}

inline size_t WriteIO(SDL_IOStream *context, const void *ptr, size_t size)
{
    return SDL_WriteIO(context, ptr, size);
}

inline size_t IOprintf(SDL_IOStream *context, const char *fmt)
{
    return SDL_IOprintf(context, fmt);
}

inline size_t IOvprintf(SDL_IOStream *context, const char *fmt, va_list ap)
{
    return SDL_IOvprintf(context, fmt, ap);
}

inline void FlushIO(SDL_IOStream *context, std::source_location location = std::source_location::current())
{
    if (!SDL_FlushIO(context))
    {
        SDLThrow(location);
    }
}

inline void *LoadFile_IO(SDL_IOStream *src, size_t *datasize, bool closeio, std::source_location location = std::source_location::current())
{
    void *result = SDL_LoadFile_IO(src, datasize, closeio);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void *LoadFile(const char *file, size_t *datasize, std::source_location location = std::source_location::current())
{
    void *result = SDL_LoadFile(file, datasize);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SaveFile_IO(SDL_IOStream *src, const void *data, size_t datasize, bool closeio, std::source_location location = std::source_location::current())
{
    if (!SDL_SaveFile_IO(src, data, datasize, closeio))
    {
        SDLThrow(location);
    }
}

inline void SaveFile(const char *file, const void *data, size_t datasize, std::source_location location = std::source_location::current())
{
    if (!SDL_SaveFile(file, data, datasize))
    {
        SDLThrow(location);
    }
}

inline void ReadU8(SDL_IOStream *src, Uint8 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadU8(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadS8(SDL_IOStream *src, Sint8 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadS8(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadU16LE(SDL_IOStream *src, Uint16 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadU16LE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadS16LE(SDL_IOStream *src, Sint16 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadS16LE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadU16BE(SDL_IOStream *src, Uint16 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadU16BE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadS16BE(SDL_IOStream *src, Sint16 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadS16BE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadU32LE(SDL_IOStream *src, Uint32 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadU32LE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadS32LE(SDL_IOStream *src, Sint32 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadS32LE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadU32BE(SDL_IOStream *src, Uint32 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadU32BE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadS32BE(SDL_IOStream *src, Sint32 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadS32BE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadU64LE(SDL_IOStream *src, Uint64 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadU64LE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadS64LE(SDL_IOStream *src, Sint64 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadS64LE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadU64BE(SDL_IOStream *src, Uint64 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadU64BE(src, value))
    {
        SDLThrow(location);
    }
}

inline void ReadS64BE(SDL_IOStream *src, Sint64 *value, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadS64BE(src, value))
    {
        SDLThrow(location);
    }
}

inline void WriteU8(SDL_IOStream *dst, Uint8 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteU8(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteS8(SDL_IOStream *dst, Sint8 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteS8(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteU16LE(SDL_IOStream *dst, Uint16 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteU16LE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteS16LE(SDL_IOStream *dst, Sint16 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteS16LE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteU16BE(SDL_IOStream *dst, Uint16 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteU16BE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteS16BE(SDL_IOStream *dst, Sint16 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteS16BE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteU32LE(SDL_IOStream *dst, Uint32 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteU32LE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteS32LE(SDL_IOStream *dst, Sint32 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteS32LE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteU32BE(SDL_IOStream *dst, Uint32 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteU32BE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteS32BE(SDL_IOStream *dst, Sint32 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteS32BE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteU64LE(SDL_IOStream *dst, Uint64 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteU64LE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteS64LE(SDL_IOStream *dst, Sint64 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteS64LE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteU64BE(SDL_IOStream *dst, Uint64 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteU64BE(dst, value))
    {
        SDLThrow(location);
    }
}

inline void WriteS64BE(SDL_IOStream *dst, Sint64 value, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteS64BE(dst, value))
    {
        SDLThrow(location);
    }
}

inline int GetNumAudioDrivers()
{
    return SDL_GetNumAudioDrivers();
}

inline const char *GetAudioDriver(int index, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetAudioDriver(index);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetCurrentAudioDriver(std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetCurrentAudioDriver();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_AudioDeviceID *GetAudioPlaybackDevices(int *count, std::source_location location = std::source_location::current())
{
    SDL_AudioDeviceID *result = SDL_GetAudioPlaybackDevices(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_AudioDeviceID *GetAudioRecordingDevices(int *count, std::source_location location = std::source_location::current())
{
    SDL_AudioDeviceID *result = SDL_GetAudioRecordingDevices(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetAudioDeviceName(SDL_AudioDeviceID devid, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetAudioDeviceName(devid);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void GetAudioDeviceFormat(SDL_AudioDeviceID devid, SDL_AudioSpec *spec, int *sample_frames, std::source_location location = std::source_location::current())
{
    if (!SDL_GetAudioDeviceFormat(devid, spec, sample_frames))
    {
        SDLThrow(location);
    }
}

inline int *GetAudioDeviceChannelMap(SDL_AudioDeviceID devid, int *count, std::source_location location = std::source_location::current())
{
    int *result = SDL_GetAudioDeviceChannelMap(devid, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_AudioDeviceID OpenAudioDevice(SDL_AudioDeviceID devid, const SDL_AudioSpec *spec)
{
    return SDL_OpenAudioDevice(devid, spec);
}

inline void IsAudioDevicePhysical(SDL_AudioDeviceID devid, std::source_location location = std::source_location::current())
{
    if (!SDL_IsAudioDevicePhysical(devid))
    {
        SDLThrow(location);
    }
}

inline void IsAudioDevicePlayback(SDL_AudioDeviceID devid, std::source_location location = std::source_location::current())
{
    if (!SDL_IsAudioDevicePlayback(devid))
    {
        SDLThrow(location);
    }
}

inline void PauseAudioDevice(SDL_AudioDeviceID devid, std::source_location location = std::source_location::current())
{
    if (!SDL_PauseAudioDevice(devid))
    {
        SDLThrow(location);
    }
}

inline void ResumeAudioDevice(SDL_AudioDeviceID devid, std::source_location location = std::source_location::current())
{
    if (!SDL_ResumeAudioDevice(devid))
    {
        SDLThrow(location);
    }
}

inline void AudioDevicePaused(SDL_AudioDeviceID devid, std::source_location location = std::source_location::current())
{
    if (!SDL_AudioDevicePaused(devid))
    {
        SDLThrow(location);
    }
}

inline float GetAudioDeviceGain(SDL_AudioDeviceID devid)
{
    return SDL_GetAudioDeviceGain(devid);
}

inline void SetAudioDeviceGain(SDL_AudioDeviceID devid, float gain, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAudioDeviceGain(devid, gain))
    {
        SDLThrow(location);
    }
}

inline void CloseAudioDevice(SDL_AudioDeviceID devid)
{
    SDL_CloseAudioDevice(devid);
}

inline void BindAudioStreams(SDL_AudioDeviceID devid, SDL_AudioStream *const *streams, int num_streams, std::source_location location = std::source_location::current())
{
    if (!SDL_BindAudioStreams(devid, streams, num_streams))
    {
        SDLThrow(location);
    }
}

inline void BindAudioStream(SDL_AudioDeviceID devid, SDL_AudioStream *stream, std::source_location location = std::source_location::current())
{
    if (!SDL_BindAudioStream(devid, stream))
    {
        SDLThrow(location);
    }
}

inline void UnbindAudioStreams(SDL_AudioStream *const *streams, int num_streams)
{
    SDL_UnbindAudioStreams(streams, num_streams);
}

inline void UnbindAudioStream(SDL_AudioStream *stream)
{
    SDL_UnbindAudioStream(stream);
}

inline SDL_AudioDeviceID GetAudioStreamDevice(SDL_AudioStream *stream)
{
    return SDL_GetAudioStreamDevice(stream);
}

inline SDL_AudioStream *CreateAudioStream(const SDL_AudioSpec *src_spec, const SDL_AudioSpec *dst_spec, std::source_location location = std::source_location::current())
{
    SDL_AudioStream *result = SDL_CreateAudioStream(src_spec, dst_spec);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_PropertiesID GetAudioStreamProperties(SDL_AudioStream *stream)
{
    return SDL_GetAudioStreamProperties(stream);
}

inline void GetAudioStreamFormat(SDL_AudioStream *stream, SDL_AudioSpec *src_spec, SDL_AudioSpec *dst_spec, std::source_location location = std::source_location::current())
{
    if (!SDL_GetAudioStreamFormat(stream, src_spec, dst_spec))
    {
        SDLThrow(location);
    }
}

inline void SetAudioStreamFormat(SDL_AudioStream *stream, const SDL_AudioSpec *src_spec, const SDL_AudioSpec *dst_spec, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAudioStreamFormat(stream, src_spec, dst_spec))
    {
        SDLThrow(location);
    }
}

inline float GetAudioStreamFrequencyRatio(SDL_AudioStream *stream)
{
    return SDL_GetAudioStreamFrequencyRatio(stream);
}

inline void SetAudioStreamFrequencyRatio(SDL_AudioStream *stream, float ratio, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAudioStreamFrequencyRatio(stream, ratio))
    {
        SDLThrow(location);
    }
}

inline float GetAudioStreamGain(SDL_AudioStream *stream)
{
    return SDL_GetAudioStreamGain(stream);
}

inline void SetAudioStreamGain(SDL_AudioStream *stream, float gain, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAudioStreamGain(stream, gain))
    {
        SDLThrow(location);
    }
}

inline int *GetAudioStreamInputChannelMap(SDL_AudioStream *stream, int *count, std::source_location location = std::source_location::current())
{
    int *result = SDL_GetAudioStreamInputChannelMap(stream, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int *GetAudioStreamOutputChannelMap(SDL_AudioStream *stream, int *count, std::source_location location = std::source_location::current())
{
    int *result = SDL_GetAudioStreamOutputChannelMap(stream, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetAudioStreamInputChannelMap(SDL_AudioStream *stream, const int *chmap, int count, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAudioStreamInputChannelMap(stream, chmap, count))
    {
        SDLThrow(location);
    }
}

inline void SetAudioStreamOutputChannelMap(SDL_AudioStream *stream, const int *chmap, int count, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAudioStreamOutputChannelMap(stream, chmap, count))
    {
        SDLThrow(location);
    }
}

inline void PutAudioStreamData(SDL_AudioStream *stream, const void *buf, int len, std::source_location location = std::source_location::current())
{
    if (!SDL_PutAudioStreamData(stream, buf, len))
    {
        SDLThrow(location);
    }
}

inline int GetAudioStreamData(SDL_AudioStream *stream, void *buf, int len)
{
    return SDL_GetAudioStreamData(stream, buf, len);
}

inline int GetAudioStreamAvailable(SDL_AudioStream *stream)
{
    return SDL_GetAudioStreamAvailable(stream);
}

inline int GetAudioStreamQueued(SDL_AudioStream *stream)
{
    return SDL_GetAudioStreamQueued(stream);
}

inline void FlushAudioStream(SDL_AudioStream *stream, std::source_location location = std::source_location::current())
{
    if (!SDL_FlushAudioStream(stream))
    {
        SDLThrow(location);
    }
}

inline void ClearAudioStream(SDL_AudioStream *stream, std::source_location location = std::source_location::current())
{
    if (!SDL_ClearAudioStream(stream))
    {
        SDLThrow(location);
    }
}

inline void PauseAudioStreamDevice(SDL_AudioStream *stream, std::source_location location = std::source_location::current())
{
    if (!SDL_PauseAudioStreamDevice(stream))
    {
        SDLThrow(location);
    }
}

inline void ResumeAudioStreamDevice(SDL_AudioStream *stream, std::source_location location = std::source_location::current())
{
    if (!SDL_ResumeAudioStreamDevice(stream))
    {
        SDLThrow(location);
    }
}

inline void AudioStreamDevicePaused(SDL_AudioStream *stream, std::source_location location = std::source_location::current())
{
    if (!SDL_AudioStreamDevicePaused(stream))
    {
        SDLThrow(location);
    }
}

inline void LockAudioStream(SDL_AudioStream *stream, std::source_location location = std::source_location::current())
{
    if (!SDL_LockAudioStream(stream))
    {
        SDLThrow(location);
    }
}

inline void UnlockAudioStream(SDL_AudioStream *stream, std::source_location location = std::source_location::current())
{
    if (!SDL_UnlockAudioStream(stream))
    {
        SDLThrow(location);
    }
}

inline void SetAudioStreamGetCallback(SDL_AudioStream *stream, SDL_AudioStreamCallback callback, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAudioStreamGetCallback(stream, callback, userdata))
    {
        SDLThrow(location);
    }
}

inline void SetAudioStreamPutCallback(SDL_AudioStream *stream, SDL_AudioStreamCallback callback, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAudioStreamPutCallback(stream, callback, userdata))
    {
        SDLThrow(location);
    }
}

inline void DestroyAudioStream(SDL_AudioStream *stream)
{
    SDL_DestroyAudioStream(stream);
}

inline SDL_AudioStream *OpenAudioDeviceStream(SDL_AudioDeviceID devid, const SDL_AudioSpec *spec, SDL_AudioStreamCallback callback, void *userdata, std::source_location location = std::source_location::current())
{
    SDL_AudioStream *result = SDL_OpenAudioDeviceStream(devid, spec, callback, userdata);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetAudioPostmixCallback(SDL_AudioDeviceID devid, SDL_AudioPostmixCallback callback, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAudioPostmixCallback(devid, callback, userdata))
    {
        SDLThrow(location);
    }
}

inline void LoadWAV_IO(SDL_IOStream *src, bool closeio, SDL_AudioSpec *spec, Uint8 **audio_buf, Uint32 *audio_len, std::source_location location = std::source_location::current())
{
    if (!SDL_LoadWAV_IO(src, closeio, spec, audio_buf, audio_len))
    {
        SDLThrow(location);
    }
}

inline void LoadWAV(const char *path, SDL_AudioSpec *spec, Uint8 **audio_buf, Uint32 *audio_len, std::source_location location = std::source_location::current())
{
    if (!SDL_LoadWAV(path, spec, audio_buf, audio_len))
    {
        SDLThrow(location);
    }
}

inline void MixAudio(Uint8 *dst, const Uint8 *src, SDL_AudioFormat format, Uint32 len, float volume, std::source_location location = std::source_location::current())
{
    if (!SDL_MixAudio(dst, src, format, len, volume))
    {
        SDLThrow(location);
    }
}

inline void ConvertAudioSamples(const SDL_AudioSpec *src_spec, const Uint8 *src_data, int src_len, const SDL_AudioSpec *dst_spec, Uint8 **dst_data, int *dst_len, std::source_location location = std::source_location::current())
{
    if (!SDL_ConvertAudioSamples(src_spec, src_data, src_len, dst_spec, dst_data, dst_len))
    {
        SDLThrow(location);
    }
}

inline const char *GetAudioFormatName(SDL_AudioFormat format, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetAudioFormatName(format);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int GetSilenceValueForFormat(SDL_AudioFormat format)
{
    return SDL_GetSilenceValueForFormat(format);
}

inline int MostSignificantBitIndex32(Uint32 x)
{
    return SDL_MostSignificantBitIndex32(x);
}

inline void HasExactlyOneBitSet32(Uint32 x, std::source_location location = std::source_location::current())
{
    if (!SDL_HasExactlyOneBitSet32(x))
    {
        SDLThrow(location);
    }
}

inline SDL_BlendMode ComposeCustomBlendMode(SDL_BlendFactor srcColorFactor, SDL_BlendFactor dstColorFactor, SDL_BlendOperation colorOperation, SDL_BlendFactor srcAlphaFactor, SDL_BlendFactor dstAlphaFactor, SDL_BlendOperation alphaOperation)
{
    return SDL_ComposeCustomBlendMode(srcColorFactor, dstColorFactor, colorOperation, srcAlphaFactor, dstAlphaFactor, alphaOperation);
}

inline const char *GetPixelFormatName(SDL_PixelFormat format, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetPixelFormatName(format);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void GetMasksForPixelFormat(SDL_PixelFormat format, int *bpp, Uint32 *Rmask, Uint32 *Gmask, Uint32 *Bmask, Uint32 *Amask, std::source_location location = std::source_location::current())
{
    if (!SDL_GetMasksForPixelFormat(format, bpp, Rmask, Gmask, Bmask, Amask))
    {
        SDLThrow(location);
    }
}

inline SDL_PixelFormat GetPixelFormatForMasks(int bpp, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
    return SDL_GetPixelFormatForMasks(bpp, Rmask, Gmask, Bmask, Amask);
}

inline const SDL_PixelFormatDetails *GetPixelFormatDetails(SDL_PixelFormat format, std::source_location location = std::source_location::current())
{
    const SDL_PixelFormatDetails *result = SDL_GetPixelFormatDetails(format);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Palette *CreatePalette(int ncolors, std::source_location location = std::source_location::current())
{
    SDL_Palette *result = SDL_CreatePalette(ncolors);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetPaletteColors(SDL_Palette *palette, const SDL_Color *colors, int firstcolor, int ncolors, std::source_location location = std::source_location::current())
{
    if (!SDL_SetPaletteColors(palette, colors, firstcolor, ncolors))
    {
        SDLThrow(location);
    }
}

inline void DestroyPalette(SDL_Palette *palette)
{
    SDL_DestroyPalette(palette);
}

inline Uint32 MapRGB(const SDL_PixelFormatDetails *format, const SDL_Palette *palette, Uint8 r, Uint8 g, Uint8 b)
{
    return SDL_MapRGB(format, palette, r, g, b);
}

inline Uint32 MapRGBA(const SDL_PixelFormatDetails *format, const SDL_Palette *palette, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return SDL_MapRGBA(format, palette, r, g, b, a);
}

inline void GetRGB(Uint32 pixel, const SDL_PixelFormatDetails *format, const SDL_Palette *palette, Uint8 *r, Uint8 *g, Uint8 *b)
{
    SDL_GetRGB(pixel, format, palette, r, g, b);
}

inline void GetRGBA(Uint32 pixel, const SDL_PixelFormatDetails *format, const SDL_Palette *palette, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a)
{
    SDL_GetRGBA(pixel, format, palette, r, g, b, a);
}

inline void RectToFRect(const SDL_Rect *rect, SDL_FRect *frect)
{
    SDL_RectToFRect(rect, frect);
}

inline void PointInRect(const SDL_Point *p, const SDL_Rect *r, std::source_location location = std::source_location::current())
{
    if (!SDL_PointInRect(p, r))
    {
        SDLThrow(location);
    }
}

inline void RectEmpty(const SDL_Rect *r, std::source_location location = std::source_location::current())
{
    if (!SDL_RectEmpty(r))
    {
        SDLThrow(location);
    }
}

inline void RectsEqual(const SDL_Rect *a, const SDL_Rect *b, std::source_location location = std::source_location::current())
{
    if (!SDL_RectsEqual(a, b))
    {
        SDLThrow(location);
    }
}

inline void HasRectIntersection(const SDL_Rect *A, const SDL_Rect *B, std::source_location location = std::source_location::current())
{
    if (!SDL_HasRectIntersection(A, B))
    {
        SDLThrow(location);
    }
}

inline void GetRectIntersection(const SDL_Rect *A, const SDL_Rect *B, SDL_Rect *result, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRectIntersection(A, B, result))
    {
        SDLThrow(location);
    }
}

inline void GetRectUnion(const SDL_Rect *A, const SDL_Rect *B, SDL_Rect *result, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRectUnion(A, B, result))
    {
        SDLThrow(location);
    }
}

inline void GetRectEnclosingPoints(const SDL_Point *points, int count, const SDL_Rect *clip, SDL_Rect *result, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRectEnclosingPoints(points, count, clip, result))
    {
        SDLThrow(location);
    }
}

inline void GetRectAndLineIntersection(const SDL_Rect *rect, int *X1, int *Y1, int *X2, int *Y2, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRectAndLineIntersection(rect, X1, Y1, X2, Y2))
    {
        SDLThrow(location);
    }
}

inline void PointInRectFloat(const SDL_FPoint *p, const SDL_FRect *r, std::source_location location = std::source_location::current())
{
    if (!SDL_PointInRectFloat(p, r))
    {
        SDLThrow(location);
    }
}

inline void RectEmptyFloat(const SDL_FRect *r, std::source_location location = std::source_location::current())
{
    if (!SDL_RectEmptyFloat(r))
    {
        SDLThrow(location);
    }
}

inline void RectsEqualEpsilon(const SDL_FRect *a, const SDL_FRect *b, float epsilon, std::source_location location = std::source_location::current())
{
    if (!SDL_RectsEqualEpsilon(a, b, epsilon))
    {
        SDLThrow(location);
    }
}

inline void RectsEqualFloat(const SDL_FRect *a, const SDL_FRect *b, std::source_location location = std::source_location::current())
{
    if (!SDL_RectsEqualFloat(a, b))
    {
        SDLThrow(location);
    }
}

inline void HasRectIntersectionFloat(const SDL_FRect *A, const SDL_FRect *B, std::source_location location = std::source_location::current())
{
    if (!SDL_HasRectIntersectionFloat(A, B))
    {
        SDLThrow(location);
    }
}

inline void GetRectIntersectionFloat(const SDL_FRect *A, const SDL_FRect *B, SDL_FRect *result, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRectIntersectionFloat(A, B, result))
    {
        SDLThrow(location);
    }
}

inline void GetRectUnionFloat(const SDL_FRect *A, const SDL_FRect *B, SDL_FRect *result, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRectUnionFloat(A, B, result))
    {
        SDLThrow(location);
    }
}

inline void GetRectEnclosingPointsFloat(const SDL_FPoint *points, int count, const SDL_FRect *clip, SDL_FRect *result, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRectEnclosingPointsFloat(points, count, clip, result))
    {
        SDLThrow(location);
    }
}

inline void GetRectAndLineIntersectionFloat(const SDL_FRect *rect, float *X1, float *Y1, float *X2, float *Y2, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRectAndLineIntersectionFloat(rect, X1, Y1, X2, Y2))
    {
        SDLThrow(location);
    }
}

inline SDL_Surface *CreateSurface(int width, int height, SDL_PixelFormat format, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_CreateSurface(width, height, format);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Surface *CreateSurfaceFrom(int width, int height, SDL_PixelFormat format, void *pixels, int pitch, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_CreateSurfaceFrom(width, height, format, pixels, pitch);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void DestroySurface(SDL_Surface *surface)
{
    SDL_DestroySurface(surface);
}

inline SDL_PropertiesID GetSurfaceProperties(SDL_Surface *surface)
{
    return SDL_GetSurfaceProperties(surface);
}

inline void SetSurfaceColorspace(SDL_Surface *surface, SDL_Colorspace colorspace, std::source_location location = std::source_location::current())
{
    if (!SDL_SetSurfaceColorspace(surface, colorspace))
    {
        SDLThrow(location);
    }
}

inline SDL_Colorspace GetSurfaceColorspace(SDL_Surface *surface)
{
    return SDL_GetSurfaceColorspace(surface);
}

inline SDL_Palette *CreateSurfacePalette(SDL_Surface *surface, std::source_location location = std::source_location::current())
{
    SDL_Palette *result = SDL_CreateSurfacePalette(surface);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetSurfacePalette(SDL_Surface *surface, SDL_Palette *palette, std::source_location location = std::source_location::current())
{
    if (!SDL_SetSurfacePalette(surface, palette))
    {
        SDLThrow(location);
    }
}

inline SDL_Palette *GetSurfacePalette(SDL_Surface *surface, std::source_location location = std::source_location::current())
{
    SDL_Palette *result = SDL_GetSurfacePalette(surface);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void AddSurfaceAlternateImage(SDL_Surface *surface, SDL_Surface *image, std::source_location location = std::source_location::current())
{
    if (!SDL_AddSurfaceAlternateImage(surface, image))
    {
        SDLThrow(location);
    }
}

inline void SurfaceHasAlternateImages(SDL_Surface *surface, std::source_location location = std::source_location::current())
{
    if (!SDL_SurfaceHasAlternateImages(surface))
    {
        SDLThrow(location);
    }
}

inline SDL_Surface **GetSurfaceImages(SDL_Surface *surface, int *count, std::source_location location = std::source_location::current())
{
    SDL_Surface **result = SDL_GetSurfaceImages(surface, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void RemoveSurfaceAlternateImages(SDL_Surface *surface)
{
    SDL_RemoveSurfaceAlternateImages(surface);
}

inline void LockSurface(SDL_Surface *surface, std::source_location location = std::source_location::current())
{
    if (!SDL_LockSurface(surface))
    {
        SDLThrow(location);
    }
}

inline void UnlockSurface(SDL_Surface *surface)
{
    SDL_UnlockSurface(surface);
}

inline SDL_Surface *LoadBMP_IO(SDL_IOStream *src, bool closeio, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_LoadBMP_IO(src, closeio);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Surface *LoadBMP(const char *file, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_LoadBMP(file);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SaveBMP_IO(SDL_Surface *surface, SDL_IOStream *dst, bool closeio, std::source_location location = std::source_location::current())
{
    if (!SDL_SaveBMP_IO(surface, dst, closeio))
    {
        SDLThrow(location);
    }
}

inline void SaveBMP(SDL_Surface *surface, const char *file, std::source_location location = std::source_location::current())
{
    if (!SDL_SaveBMP(surface, file))
    {
        SDLThrow(location);
    }
}

inline void SetSurfaceRLE(SDL_Surface *surface, bool enabled, std::source_location location = std::source_location::current())
{
    if (!SDL_SetSurfaceRLE(surface, enabled))
    {
        SDLThrow(location);
    }
}

inline void SurfaceHasRLE(SDL_Surface *surface, std::source_location location = std::source_location::current())
{
    if (!SDL_SurfaceHasRLE(surface))
    {
        SDLThrow(location);
    }
}

inline void SetSurfaceColorKey(SDL_Surface *surface, bool enabled, Uint32 key, std::source_location location = std::source_location::current())
{
    if (!SDL_SetSurfaceColorKey(surface, enabled, key))
    {
        SDLThrow(location);
    }
}

inline void SurfaceHasColorKey(SDL_Surface *surface, std::source_location location = std::source_location::current())
{
    if (!SDL_SurfaceHasColorKey(surface))
    {
        SDLThrow(location);
    }
}

inline void GetSurfaceColorKey(SDL_Surface *surface, Uint32 *key, std::source_location location = std::source_location::current())
{
    if (!SDL_GetSurfaceColorKey(surface, key))
    {
        SDLThrow(location);
    }
}

inline void SetSurfaceColorMod(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b, std::source_location location = std::source_location::current())
{
    if (!SDL_SetSurfaceColorMod(surface, r, g, b))
    {
        SDLThrow(location);
    }
}

inline void GetSurfaceColorMod(SDL_Surface *surface, Uint8 *r, Uint8 *g, Uint8 *b, std::source_location location = std::source_location::current())
{
    if (!SDL_GetSurfaceColorMod(surface, r, g, b))
    {
        SDLThrow(location);
    }
}

inline void SetSurfaceAlphaMod(SDL_Surface *surface, Uint8 alpha, std::source_location location = std::source_location::current())
{
    if (!SDL_SetSurfaceAlphaMod(surface, alpha))
    {
        SDLThrow(location);
    }
}

inline void GetSurfaceAlphaMod(SDL_Surface *surface, Uint8 *alpha, std::source_location location = std::source_location::current())
{
    if (!SDL_GetSurfaceAlphaMod(surface, alpha))
    {
        SDLThrow(location);
    }
}

inline void SetSurfaceBlendMode(SDL_Surface *surface, SDL_BlendMode blendMode, std::source_location location = std::source_location::current())
{
    if (!SDL_SetSurfaceBlendMode(surface, blendMode))
    {
        SDLThrow(location);
    }
}

inline void GetSurfaceBlendMode(SDL_Surface *surface, SDL_BlendMode *blendMode, std::source_location location = std::source_location::current())
{
    if (!SDL_GetSurfaceBlendMode(surface, blendMode))
    {
        SDLThrow(location);
    }
}

inline void SetSurfaceClipRect(SDL_Surface *surface, const SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_SetSurfaceClipRect(surface, rect))
    {
        SDLThrow(location);
    }
}

inline void GetSurfaceClipRect(SDL_Surface *surface, SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_GetSurfaceClipRect(surface, rect))
    {
        SDLThrow(location);
    }
}

inline void FlipSurface(SDL_Surface *surface, SDL_FlipMode flip, std::source_location location = std::source_location::current())
{
    if (!SDL_FlipSurface(surface, flip))
    {
        SDLThrow(location);
    }
}

inline SDL_Surface *DuplicateSurface(SDL_Surface *surface, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_DuplicateSurface(surface);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Surface *ScaleSurface(SDL_Surface *surface, int width, int height, SDL_ScaleMode scaleMode, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_ScaleSurface(surface, width, height, scaleMode);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Surface *ConvertSurface(SDL_Surface *surface, SDL_PixelFormat format, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_ConvertSurface(surface, format);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Surface *ConvertSurfaceAndColorspace(SDL_Surface *surface, SDL_PixelFormat format, SDL_Palette *palette, SDL_Colorspace colorspace, SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_ConvertSurfaceAndColorspace(surface, format, palette, colorspace, props);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void ConvertPixels(int width, int height, SDL_PixelFormat src_format, const void *src, int src_pitch, SDL_PixelFormat dst_format, void *dst, int dst_pitch, std::source_location location = std::source_location::current())
{
    if (!SDL_ConvertPixels(width, height, src_format, src, src_pitch, dst_format, dst, dst_pitch))
    {
        SDLThrow(location);
    }
}

inline void ConvertPixelsAndColorspace(int width, int height, SDL_PixelFormat src_format, SDL_Colorspace src_colorspace, SDL_PropertiesID src_properties, const void *src, int src_pitch, SDL_PixelFormat dst_format, SDL_Colorspace dst_colorspace, SDL_PropertiesID dst_properties, void *dst, int dst_pitch, std::source_location location = std::source_location::current())
{
    if (!SDL_ConvertPixelsAndColorspace(width, height, src_format, src_colorspace, src_properties, src, src_pitch, dst_format, dst_colorspace, dst_properties, dst, dst_pitch))
    {
        SDLThrow(location);
    }
}

inline void PremultiplyAlpha(int width, int height, SDL_PixelFormat src_format, const void *src, int src_pitch, SDL_PixelFormat dst_format, void *dst, int dst_pitch, bool linear, std::source_location location = std::source_location::current())
{
    if (!SDL_PremultiplyAlpha(width, height, src_format, src, src_pitch, dst_format, dst, dst_pitch, linear))
    {
        SDLThrow(location);
    }
}

inline void PremultiplySurfaceAlpha(SDL_Surface *surface, bool linear, std::source_location location = std::source_location::current())
{
    if (!SDL_PremultiplySurfaceAlpha(surface, linear))
    {
        SDLThrow(location);
    }
}

inline void ClearSurface(SDL_Surface *surface, float r, float g, float b, float a, std::source_location location = std::source_location::current())
{
    if (!SDL_ClearSurface(surface, r, g, b, a))
    {
        SDLThrow(location);
    }
}

inline void FillSurfaceRect(SDL_Surface *dst, const SDL_Rect *rect, Uint32 color, std::source_location location = std::source_location::current())
{
    if (!SDL_FillSurfaceRect(dst, rect, color))
    {
        SDLThrow(location);
    }
}

inline void FillSurfaceRects(SDL_Surface *dst, const SDL_Rect *rects, int count, Uint32 color, std::source_location location = std::source_location::current())
{
    if (!SDL_FillSurfaceRects(dst, rects, count, color))
    {
        SDLThrow(location);
    }
}

inline void BlitSurface(SDL_Surface *src, const SDL_Rect *srcrect, SDL_Surface *dst, const SDL_Rect *dstrect, std::source_location location = std::source_location::current())
{
    if (!SDL_BlitSurface(src, srcrect, dst, dstrect))
    {
        SDLThrow(location);
    }
}

inline void BlitSurfaceUnchecked(SDL_Surface *src, const SDL_Rect *srcrect, SDL_Surface *dst, const SDL_Rect *dstrect, std::source_location location = std::source_location::current())
{
    if (!SDL_BlitSurfaceUnchecked(src, srcrect, dst, dstrect))
    {
        SDLThrow(location);
    }
}

inline void BlitSurfaceScaled(SDL_Surface *src, const SDL_Rect *srcrect, SDL_Surface *dst, const SDL_Rect *dstrect, SDL_ScaleMode scaleMode, std::source_location location = std::source_location::current())
{
    if (!SDL_BlitSurfaceScaled(src, srcrect, dst, dstrect, scaleMode))
    {
        SDLThrow(location);
    }
}

inline void BlitSurfaceUncheckedScaled(SDL_Surface *src, const SDL_Rect *srcrect, SDL_Surface *dst, const SDL_Rect *dstrect, SDL_ScaleMode scaleMode, std::source_location location = std::source_location::current())
{
    if (!SDL_BlitSurfaceUncheckedScaled(src, srcrect, dst, dstrect, scaleMode))
    {
        SDLThrow(location);
    }
}

inline void StretchSurface(SDL_Surface *src, const SDL_Rect *srcrect, SDL_Surface *dst, const SDL_Rect *dstrect, SDL_ScaleMode scaleMode, std::source_location location = std::source_location::current())
{
    if (!SDL_StretchSurface(src, srcrect, dst, dstrect, scaleMode))
    {
        SDLThrow(location);
    }
}

inline void BlitSurfaceTiled(SDL_Surface *src, const SDL_Rect *srcrect, SDL_Surface *dst, const SDL_Rect *dstrect, std::source_location location = std::source_location::current())
{
    if (!SDL_BlitSurfaceTiled(src, srcrect, dst, dstrect))
    {
        SDLThrow(location);
    }
}

inline void BlitSurfaceTiledWithScale(SDL_Surface *src, const SDL_Rect *srcrect, float scale, SDL_ScaleMode scaleMode, SDL_Surface *dst, const SDL_Rect *dstrect, std::source_location location = std::source_location::current())
{
    if (!SDL_BlitSurfaceTiledWithScale(src, srcrect, scale, scaleMode, dst, dstrect))
    {
        SDLThrow(location);
    }
}

inline void BlitSurface9Grid(SDL_Surface *src, const SDL_Rect *srcrect, int left_width, int right_width, int top_height, int bottom_height, float scale, SDL_ScaleMode scaleMode, SDL_Surface *dst, const SDL_Rect *dstrect, std::source_location location = std::source_location::current())
{
    if (!SDL_BlitSurface9Grid(src, srcrect, left_width, right_width, top_height, bottom_height, scale, scaleMode, dst, dstrect))
    {
        SDLThrow(location);
    }
}

inline Uint32 MapSurfaceRGB(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b)
{
    return SDL_MapSurfaceRGB(surface, r, g, b);
}

inline Uint32 MapSurfaceRGBA(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return SDL_MapSurfaceRGBA(surface, r, g, b, a);
}

inline void ReadSurfacePixel(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadSurfacePixel(surface, x, y, r, g, b, a))
    {
        SDLThrow(location);
    }
}

inline void ReadSurfacePixelFloat(SDL_Surface *surface, int x, int y, float *r, float *g, float *b, float *a, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadSurfacePixelFloat(surface, x, y, r, g, b, a))
    {
        SDLThrow(location);
    }
}

inline void WriteSurfacePixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteSurfacePixel(surface, x, y, r, g, b, a))
    {
        SDLThrow(location);
    }
}

inline void WriteSurfacePixelFloat(SDL_Surface *surface, int x, int y, float r, float g, float b, float a, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteSurfacePixelFloat(surface, x, y, r, g, b, a))
    {
        SDLThrow(location);
    }
}

inline int GetNumCameraDrivers()
{
    return SDL_GetNumCameraDrivers();
}

inline const char *GetCameraDriver(int index, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetCameraDriver(index);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetCurrentCameraDriver(std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetCurrentCameraDriver();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_CameraID *GetCameras(int *count, std::source_location location = std::source_location::current())
{
    SDL_CameraID *result = SDL_GetCameras(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_CameraSpec **GetCameraSupportedFormats(SDL_CameraID instance_id, int *count, std::source_location location = std::source_location::current())
{
    SDL_CameraSpec **result = SDL_GetCameraSupportedFormats(instance_id, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetCameraName(SDL_CameraID instance_id, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetCameraName(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_CameraPosition GetCameraPosition(SDL_CameraID instance_id)
{
    return SDL_GetCameraPosition(instance_id);
}

inline SDL_Camera *OpenCamera(SDL_CameraID instance_id, const SDL_CameraSpec *spec, std::source_location location = std::source_location::current())
{
    SDL_Camera *result = SDL_OpenCamera(instance_id, spec);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int GetCameraPermissionState(SDL_Camera *camera)
{
    return SDL_GetCameraPermissionState(camera);
}

inline SDL_CameraID GetCameraID(SDL_Camera *camera)
{
    return SDL_GetCameraID(camera);
}

inline SDL_PropertiesID GetCameraProperties(SDL_Camera *camera)
{
    return SDL_GetCameraProperties(camera);
}

inline void GetCameraFormat(SDL_Camera *camera, SDL_CameraSpec *spec, std::source_location location = std::source_location::current())
{
    if (!SDL_GetCameraFormat(camera, spec))
    {
        SDLThrow(location);
    }
}

inline SDL_Surface *AcquireCameraFrame(SDL_Camera *camera, Uint64 *timestampNS, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_AcquireCameraFrame(camera, timestampNS);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void ReleaseCameraFrame(SDL_Camera *camera, SDL_Surface *frame)
{
    SDL_ReleaseCameraFrame(camera, frame);
}

inline void CloseCamera(SDL_Camera *camera)
{
    SDL_CloseCamera(camera);
}

inline void SetClipboardText(const char *text, std::source_location location = std::source_location::current())
{
    if (!SDL_SetClipboardText(text))
    {
        SDLThrow(location);
    }
}

inline char *GetClipboardText(std::source_location location = std::source_location::current())
{
    char *result = SDL_GetClipboardText();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void HasClipboardText(std::source_location location = std::source_location::current())
{
    if (!SDL_HasClipboardText())
    {
        SDLThrow(location);
    }
}

inline void SetPrimarySelectionText(const char *text, std::source_location location = std::source_location::current())
{
    if (!SDL_SetPrimarySelectionText(text))
    {
        SDLThrow(location);
    }
}

inline char *GetPrimarySelectionText(std::source_location location = std::source_location::current())
{
    char *result = SDL_GetPrimarySelectionText();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void HasPrimarySelectionText(std::source_location location = std::source_location::current())
{
    if (!SDL_HasPrimarySelectionText())
    {
        SDLThrow(location);
    }
}

inline void SetClipboardData(SDL_ClipboardDataCallback callback, SDL_ClipboardCleanupCallback cleanup, void *userdata, const char **mime_types, size_t num_mime_types, std::source_location location = std::source_location::current())
{
    if (!SDL_SetClipboardData(callback, cleanup, userdata, mime_types, num_mime_types))
    {
        SDLThrow(location);
    }
}

inline void ClearClipboardData(std::source_location location = std::source_location::current())
{
    if (!SDL_ClearClipboardData())
    {
        SDLThrow(location);
    }
}

inline void *GetClipboardData(const char *mime_type, size_t *size, std::source_location location = std::source_location::current())
{
    void *result = SDL_GetClipboardData(mime_type, size);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void HasClipboardData(const char *mime_type, std::source_location location = std::source_location::current())
{
    if (!SDL_HasClipboardData(mime_type))
    {
        SDLThrow(location);
    }
}

inline char **GetClipboardMimeTypes(size_t *num_mime_types, std::source_location location = std::source_location::current())
{
    char **result = SDL_GetClipboardMimeTypes(num_mime_types);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int GetNumLogicalCPUCores()
{
    return SDL_GetNumLogicalCPUCores();
}

inline int GetCPUCacheLineSize()
{
    return SDL_GetCPUCacheLineSize();
}

inline void HasAltiVec(std::source_location location = std::source_location::current())
{
    if (!SDL_HasAltiVec())
    {
        SDLThrow(location);
    }
}

inline void HasMMX(std::source_location location = std::source_location::current())
{
    if (!SDL_HasMMX())
    {
        SDLThrow(location);
    }
}

inline void HasSSE(std::source_location location = std::source_location::current())
{
    if (!SDL_HasSSE())
    {
        SDLThrow(location);
    }
}

inline void HasSSE2(std::source_location location = std::source_location::current())
{
    if (!SDL_HasSSE2())
    {
        SDLThrow(location);
    }
}

inline void HasSSE3(std::source_location location = std::source_location::current())
{
    if (!SDL_HasSSE3())
    {
        SDLThrow(location);
    }
}

inline void HasSSE41(std::source_location location = std::source_location::current())
{
    if (!SDL_HasSSE41())
    {
        SDLThrow(location);
    }
}

inline void HasSSE42(std::source_location location = std::source_location::current())
{
    if (!SDL_HasSSE42())
    {
        SDLThrow(location);
    }
}

inline void HasAVX(std::source_location location = std::source_location::current())
{
    if (!SDL_HasAVX())
    {
        SDLThrow(location);
    }
}

inline void HasAVX2(std::source_location location = std::source_location::current())
{
    if (!SDL_HasAVX2())
    {
        SDLThrow(location);
    }
}

inline void HasAVX512F(std::source_location location = std::source_location::current())
{
    if (!SDL_HasAVX512F())
    {
        SDLThrow(location);
    }
}

inline void HasARMSIMD(std::source_location location = std::source_location::current())
{
    if (!SDL_HasARMSIMD())
    {
        SDLThrow(location);
    }
}

inline void HasNEON(std::source_location location = std::source_location::current())
{
    if (!SDL_HasNEON())
    {
        SDLThrow(location);
    }
}

inline void HasLSX(std::source_location location = std::source_location::current())
{
    if (!SDL_HasLSX())
    {
        SDLThrow(location);
    }
}

inline void HasLASX(std::source_location location = std::source_location::current())
{
    if (!SDL_HasLASX())
    {
        SDLThrow(location);
    }
}

inline int GetSystemRAM()
{
    return SDL_GetSystemRAM();
}

inline size_t GetSIMDAlignment()
{
    return SDL_GetSIMDAlignment();
}

inline int GetNumVideoDrivers()
{
    return SDL_GetNumVideoDrivers();
}

inline const char *GetVideoDriver(int index, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetVideoDriver(index);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetCurrentVideoDriver(std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetCurrentVideoDriver();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_SystemTheme GetSystemTheme()
{
    return SDL_GetSystemTheme();
}

inline SDL_DisplayID *GetDisplays(int *count, std::source_location location = std::source_location::current())
{
    SDL_DisplayID *result = SDL_GetDisplays(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_DisplayID GetPrimaryDisplay()
{
    return SDL_GetPrimaryDisplay();
}

inline SDL_PropertiesID GetDisplayProperties(SDL_DisplayID displayID)
{
    return SDL_GetDisplayProperties(displayID);
}

inline const char *GetDisplayName(SDL_DisplayID displayID, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetDisplayName(displayID);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void GetDisplayBounds(SDL_DisplayID displayID, SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_GetDisplayBounds(displayID, rect))
    {
        SDLThrow(location);
    }
}

inline void GetDisplayUsableBounds(SDL_DisplayID displayID, SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_GetDisplayUsableBounds(displayID, rect))
    {
        SDLThrow(location);
    }
}

inline SDL_DisplayOrientation GetNaturalDisplayOrientation(SDL_DisplayID displayID)
{
    return SDL_GetNaturalDisplayOrientation(displayID);
}

inline SDL_DisplayOrientation GetCurrentDisplayOrientation(SDL_DisplayID displayID)
{
    return SDL_GetCurrentDisplayOrientation(displayID);
}

inline float GetDisplayContentScale(SDL_DisplayID displayID)
{
    return SDL_GetDisplayContentScale(displayID);
}

inline SDL_DisplayMode **GetFullscreenDisplayModes(SDL_DisplayID displayID, int *count, std::source_location location = std::source_location::current())
{
    SDL_DisplayMode **result = SDL_GetFullscreenDisplayModes(displayID, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void GetClosestFullscreenDisplayMode(SDL_DisplayID displayID, int w, int h, float refresh_rate, bool include_high_density_modes, SDL_DisplayMode *closest, std::source_location location = std::source_location::current())
{
    if (!SDL_GetClosestFullscreenDisplayMode(displayID, w, h, refresh_rate, include_high_density_modes, closest))
    {
        SDLThrow(location);
    }
}

inline const SDL_DisplayMode *GetDesktopDisplayMode(SDL_DisplayID displayID, std::source_location location = std::source_location::current())
{
    const SDL_DisplayMode *result = SDL_GetDesktopDisplayMode(displayID);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const SDL_DisplayMode *GetCurrentDisplayMode(SDL_DisplayID displayID, std::source_location location = std::source_location::current())
{
    const SDL_DisplayMode *result = SDL_GetCurrentDisplayMode(displayID);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_DisplayID GetDisplayForPoint(const SDL_Point *point)
{
    return SDL_GetDisplayForPoint(point);
}

inline SDL_DisplayID GetDisplayForRect(const SDL_Rect *rect)
{
    return SDL_GetDisplayForRect(rect);
}

inline SDL_DisplayID GetDisplayForWindow(SDL_Window *window)
{
    return SDL_GetDisplayForWindow(window);
}

inline float GetWindowPixelDensity(SDL_Window *window)
{
    return SDL_GetWindowPixelDensity(window);
}

inline float GetWindowDisplayScale(SDL_Window *window)
{
    return SDL_GetWindowDisplayScale(window);
}

inline void SetWindowFullscreenMode(SDL_Window *window, const SDL_DisplayMode *mode, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowFullscreenMode(window, mode))
    {
        SDLThrow(location);
    }
}

inline const SDL_DisplayMode *GetWindowFullscreenMode(SDL_Window *window, std::source_location location = std::source_location::current())
{
    const SDL_DisplayMode *result = SDL_GetWindowFullscreenMode(window);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void *GetWindowICCProfile(SDL_Window *window, size_t *size, std::source_location location = std::source_location::current())
{
    void *result = SDL_GetWindowICCProfile(window, size);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_PixelFormat GetWindowPixelFormat(SDL_Window *window)
{
    return SDL_GetWindowPixelFormat(window);
}

inline SDL_Window **GetWindows(int *count, std::source_location location = std::source_location::current())
{
    SDL_Window **result = SDL_GetWindows(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Window *CreateWindow(const char *title, int w, int h, SDL_WindowFlags flags, std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_CreateWindow(title, w, h, flags);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Window *CreatePopupWindow(SDL_Window *parent, int offset_x, int offset_y, int w, int h, SDL_WindowFlags flags, std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_CreatePopupWindow(parent, offset_x, offset_y, w, h, flags);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Window *CreateWindowWithProperties(SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_CreateWindowWithProperties(props);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_WindowID GetWindowID(SDL_Window *window)
{
    return SDL_GetWindowID(window);
}

inline SDL_Window *GetWindowFromID(SDL_WindowID id, std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_GetWindowFromID(id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Window *GetWindowParent(SDL_Window *window, std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_GetWindowParent(window);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_PropertiesID GetWindowProperties(SDL_Window *window)
{
    return SDL_GetWindowProperties(window);
}

inline SDL_WindowFlags GetWindowFlags(SDL_Window *window)
{
    return SDL_GetWindowFlags(window);
}

inline void SetWindowTitle(SDL_Window *window, const char *title, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowTitle(window, title))
    {
        SDLThrow(location);
    }
}

inline const char *GetWindowTitle(SDL_Window *window, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetWindowTitle(window);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetWindowIcon(SDL_Window *window, SDL_Surface *icon, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowIcon(window, icon))
    {
        SDLThrow(location);
    }
}

inline void SetWindowPosition(SDL_Window *window, int x, int y, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowPosition(window, x, y))
    {
        SDLThrow(location);
    }
}

inline void GetWindowPosition(SDL_Window *window, int *x, int *y, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowPosition(window, x, y))
    {
        SDLThrow(location);
    }
}

inline void SetWindowSize(SDL_Window *window, int w, int h, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowSize(window, w, h))
    {
        SDLThrow(location);
    }
}

inline void GetWindowSize(SDL_Window *window, int *w, int *h, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowSize(window, w, h))
    {
        SDLThrow(location);
    }
}

inline void GetWindowSafeArea(SDL_Window *window, SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowSafeArea(window, rect))
    {
        SDLThrow(location);
    }
}

inline void SetWindowAspectRatio(SDL_Window *window, float min_aspect, float max_aspect, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowAspectRatio(window, min_aspect, max_aspect))
    {
        SDLThrow(location);
    }
}

inline void GetWindowAspectRatio(SDL_Window *window, float *min_aspect, float *max_aspect, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowAspectRatio(window, min_aspect, max_aspect))
    {
        SDLThrow(location);
    }
}

inline void GetWindowBordersSize(SDL_Window *window, int *top, int *left, int *bottom, int *right, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowBordersSize(window, top, left, bottom, right))
    {
        SDLThrow(location);
    }
}

inline void GetWindowSizeInPixels(SDL_Window *window, int *w, int *h, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowSizeInPixels(window, w, h))
    {
        SDLThrow(location);
    }
}

inline void SetWindowMinimumSize(SDL_Window *window, int min_w, int min_h, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowMinimumSize(window, min_w, min_h))
    {
        SDLThrow(location);
    }
}

inline void GetWindowMinimumSize(SDL_Window *window, int *w, int *h, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowMinimumSize(window, w, h))
    {
        SDLThrow(location);
    }
}

inline void SetWindowMaximumSize(SDL_Window *window, int max_w, int max_h, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowMaximumSize(window, max_w, max_h))
    {
        SDLThrow(location);
    }
}

inline void GetWindowMaximumSize(SDL_Window *window, int *w, int *h, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowMaximumSize(window, w, h))
    {
        SDLThrow(location);
    }
}

inline void SetWindowBordered(SDL_Window *window, bool bordered, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowBordered(window, bordered))
    {
        SDLThrow(location);
    }
}

inline void SetWindowResizable(SDL_Window *window, bool resizable, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowResizable(window, resizable))
    {
        SDLThrow(location);
    }
}

inline void SetWindowAlwaysOnTop(SDL_Window *window, bool on_top, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowAlwaysOnTop(window, on_top))
    {
        SDLThrow(location);
    }
}

inline void ShowWindow(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_ShowWindow(window))
    {
        SDLThrow(location);
    }
}

inline void HideWindow(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_HideWindow(window))
    {
        SDLThrow(location);
    }
}

inline void RaiseWindow(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_RaiseWindow(window))
    {
        SDLThrow(location);
    }
}

inline void MaximizeWindow(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_MaximizeWindow(window))
    {
        SDLThrow(location);
    }
}

inline void MinimizeWindow(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_MinimizeWindow(window))
    {
        SDLThrow(location);
    }
}

inline void RestoreWindow(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_RestoreWindow(window))
    {
        SDLThrow(location);
    }
}

inline void SetWindowFullscreen(SDL_Window *window, bool fullscreen, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowFullscreen(window, fullscreen))
    {
        SDLThrow(location);
    }
}

inline void SyncWindow(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_SyncWindow(window))
    {
        SDLThrow(location);
    }
}

inline void WindowHasSurface(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_WindowHasSurface(window))
    {
        SDLThrow(location);
    }
}

inline SDL_Surface *GetWindowSurface(SDL_Window *window, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_GetWindowSurface(window);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetWindowSurfaceVSync(SDL_Window *window, int vsync, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowSurfaceVSync(window, vsync))
    {
        SDLThrow(location);
    }
}

inline void GetWindowSurfaceVSync(SDL_Window *window, int *vsync, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowSurfaceVSync(window, vsync))
    {
        SDLThrow(location);
    }
}

inline void UpdateWindowSurface(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_UpdateWindowSurface(window))
    {
        SDLThrow(location);
    }
}

inline void UpdateWindowSurfaceRects(SDL_Window *window, const SDL_Rect *rects, int numrects, std::source_location location = std::source_location::current())
{
    if (!SDL_UpdateWindowSurfaceRects(window, rects, numrects))
    {
        SDLThrow(location);
    }
}

inline void DestroyWindowSurface(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_DestroyWindowSurface(window))
    {
        SDLThrow(location);
    }
}

inline void SetWindowKeyboardGrab(SDL_Window *window, bool grabbed, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowKeyboardGrab(window, grabbed))
    {
        SDLThrow(location);
    }
}

inline void SetWindowMouseGrab(SDL_Window *window, bool grabbed, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowMouseGrab(window, grabbed))
    {
        SDLThrow(location);
    }
}

inline void GetWindowKeyboardGrab(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowKeyboardGrab(window))
    {
        SDLThrow(location);
    }
}

inline void GetWindowMouseGrab(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowMouseGrab(window))
    {
        SDLThrow(location);
    }
}

inline SDL_Window *GetGrabbedWindow(std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_GetGrabbedWindow();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetWindowMouseRect(SDL_Window *window, const SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowMouseRect(window, rect))
    {
        SDLThrow(location);
    }
}

inline const SDL_Rect *GetWindowMouseRect(SDL_Window *window, std::source_location location = std::source_location::current())
{
    const SDL_Rect *result = SDL_GetWindowMouseRect(window);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetWindowOpacity(SDL_Window *window, float opacity, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowOpacity(window, opacity))
    {
        SDLThrow(location);
    }
}

inline float GetWindowOpacity(SDL_Window *window)
{
    return SDL_GetWindowOpacity(window);
}

inline void SetWindowParent(SDL_Window *window, SDL_Window *parent, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowParent(window, parent))
    {
        SDLThrow(location);
    }
}

inline void SetWindowModal(SDL_Window *window, bool modal, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowModal(window, modal))
    {
        SDLThrow(location);
    }
}

inline void SetWindowFocusable(SDL_Window *window, bool focusable, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowFocusable(window, focusable))
    {
        SDLThrow(location);
    }
}

inline void ShowWindowSystemMenu(SDL_Window *window, int x, int y, std::source_location location = std::source_location::current())
{
    if (!SDL_ShowWindowSystemMenu(window, x, y))
    {
        SDLThrow(location);
    }
}

inline void SetWindowHitTest(SDL_Window *window, SDL_HitTest callback, void *callback_data, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowHitTest(window, callback, callback_data))
    {
        SDLThrow(location);
    }
}

inline void SetWindowShape(SDL_Window *window, SDL_Surface *shape, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowShape(window, shape))
    {
        SDLThrow(location);
    }
}

inline void FlashWindow(SDL_Window *window, SDL_FlashOperation operation, std::source_location location = std::source_location::current())
{
    if (!SDL_FlashWindow(window, operation))
    {
        SDLThrow(location);
    }
}

inline void DestroyWindow(SDL_Window *window)
{
    SDL_DestroyWindow(window);
}

inline void ScreenSaverEnabled(std::source_location location = std::source_location::current())
{
    if (!SDL_ScreenSaverEnabled())
    {
        SDLThrow(location);
    }
}

inline void EnableScreenSaver(std::source_location location = std::source_location::current())
{
    if (!SDL_EnableScreenSaver())
    {
        SDLThrow(location);
    }
}

inline void DisableScreenSaver(std::source_location location = std::source_location::current())
{
    if (!SDL_DisableScreenSaver())
    {
        SDLThrow(location);
    }
}

inline void GL_LoadLibrary(const char *path, std::source_location location = std::source_location::current())
{
    if (!SDL_GL_LoadLibrary(path))
    {
        SDLThrow(location);
    }
}

inline SDL_FunctionPointer GL_GetProcAddress(const char *proc)
{
    return SDL_GL_GetProcAddress(proc);
}

inline SDL_FunctionPointer EGL_GetProcAddress(const char *proc)
{
    return SDL_EGL_GetProcAddress(proc);
}

inline void GL_UnloadLibrary()
{
    SDL_GL_UnloadLibrary();
}

inline void GL_ExtensionSupported(const char *extension, std::source_location location = std::source_location::current())
{
    if (!SDL_GL_ExtensionSupported(extension))
    {
        SDLThrow(location);
    }
}

inline void GL_ResetAttributes()
{
    SDL_GL_ResetAttributes();
}

inline void GL_SetAttribute(SDL_GLAttr attr, int value, std::source_location location = std::source_location::current())
{
    if (!SDL_GL_SetAttribute(attr, value))
    {
        SDLThrow(location);
    }
}

inline void GL_GetAttribute(SDL_GLAttr attr, int *value, std::source_location location = std::source_location::current())
{
    if (!SDL_GL_GetAttribute(attr, value))
    {
        SDLThrow(location);
    }
}

inline SDL_GLContext GL_CreateContext(SDL_Window *window)
{
    return SDL_GL_CreateContext(window);
}

inline void GL_MakeCurrent(SDL_Window *window, SDL_GLContext context, std::source_location location = std::source_location::current())
{
    if (!SDL_GL_MakeCurrent(window, context))
    {
        SDLThrow(location);
    }
}

inline SDL_Window *GL_GetCurrentWindow(std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_GL_GetCurrentWindow();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GLContext GL_GetCurrentContext()
{
    return SDL_GL_GetCurrentContext();
}

inline SDL_EGLDisplay EGL_GetCurrentDisplay()
{
    return SDL_EGL_GetCurrentDisplay();
}

inline SDL_EGLConfig EGL_GetCurrentConfig()
{
    return SDL_EGL_GetCurrentConfig();
}

inline SDL_EGLSurface EGL_GetWindowSurface(SDL_Window *window)
{
    return SDL_EGL_GetWindowSurface(window);
}

inline void EGL_SetAttributeCallbacks(SDL_EGLAttribArrayCallback platformAttribCallback, SDL_EGLIntArrayCallback surfaceAttribCallback, SDL_EGLIntArrayCallback contextAttribCallback, void *userdata)
{
    SDL_EGL_SetAttributeCallbacks(platformAttribCallback, surfaceAttribCallback, contextAttribCallback, userdata);
}

inline void GL_SetSwapInterval(int interval, std::source_location location = std::source_location::current())
{
    if (!SDL_GL_SetSwapInterval(interval))
    {
        SDLThrow(location);
    }
}

inline void GL_GetSwapInterval(int *interval, std::source_location location = std::source_location::current())
{
    if (!SDL_GL_GetSwapInterval(interval))
    {
        SDLThrow(location);
    }
}

inline void GL_SwapWindow(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_GL_SwapWindow(window))
    {
        SDLThrow(location);
    }
}

inline void GL_DestroyContext(SDL_GLContext context, std::source_location location = std::source_location::current())
{
    if (!SDL_GL_DestroyContext(context))
    {
        SDLThrow(location);
    }
}

inline void ShowOpenFileDialog(SDL_DialogFileCallback callback, void *userdata, SDL_Window *window, const SDL_DialogFileFilter *filters, int nfilters, const char *default_location, bool allow_many)
{
    SDL_ShowOpenFileDialog(callback, userdata, window, filters, nfilters, default_location, allow_many);
}

inline void ShowSaveFileDialog(SDL_DialogFileCallback callback, void *userdata, SDL_Window *window, const SDL_DialogFileFilter *filters, int nfilters, const char *default_location)
{
    SDL_ShowSaveFileDialog(callback, userdata, window, filters, nfilters, default_location);
}

inline void ShowOpenFolderDialog(SDL_DialogFileCallback callback, void *userdata, SDL_Window *window, const char *default_location, bool allow_many)
{
    SDL_ShowOpenFolderDialog(callback, userdata, window, default_location, allow_many);
}

inline void ShowFileDialogWithProperties(SDL_FileDialogType type, SDL_DialogFileCallback callback, void *userdata, SDL_PropertiesID props)
{
    SDL_ShowFileDialogWithProperties(type, callback, userdata, props);
}

inline void GUIDToString(SDL_GUID guid, char *pszGUID, int cbGUID)
{
    SDL_GUIDToString(guid, pszGUID, cbGUID);
}

inline SDL_GUID StringToGUID(const char *pchGUID)
{
    return SDL_StringToGUID(pchGUID);
}

inline SDL_PowerState GetPowerInfo(int *seconds, int *percent)
{
    return SDL_GetPowerInfo(seconds, percent);
}

inline SDL_SensorID *GetSensors(int *count, std::source_location location = std::source_location::current())
{
    SDL_SensorID *result = SDL_GetSensors(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetSensorNameForID(SDL_SensorID instance_id, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetSensorNameForID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_SensorType GetSensorTypeForID(SDL_SensorID instance_id)
{
    return SDL_GetSensorTypeForID(instance_id);
}

inline int GetSensorNonPortableTypeForID(SDL_SensorID instance_id)
{
    return SDL_GetSensorNonPortableTypeForID(instance_id);
}

inline SDL_Sensor *OpenSensor(SDL_SensorID instance_id, std::source_location location = std::source_location::current())
{
    SDL_Sensor *result = SDL_OpenSensor(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Sensor *GetSensorFromID(SDL_SensorID instance_id, std::source_location location = std::source_location::current())
{
    SDL_Sensor *result = SDL_GetSensorFromID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_PropertiesID GetSensorProperties(SDL_Sensor *sensor)
{
    return SDL_GetSensorProperties(sensor);
}

inline const char *GetSensorName(SDL_Sensor *sensor, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetSensorName(sensor);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_SensorType GetSensorType(SDL_Sensor *sensor)
{
    return SDL_GetSensorType(sensor);
}

inline int GetSensorNonPortableType(SDL_Sensor *sensor)
{
    return SDL_GetSensorNonPortableType(sensor);
}

inline SDL_SensorID GetSensorID(SDL_Sensor *sensor)
{
    return SDL_GetSensorID(sensor);
}

inline void GetSensorData(SDL_Sensor *sensor, float *data, int num_values, std::source_location location = std::source_location::current())
{
    if (!SDL_GetSensorData(sensor, data, num_values))
    {
        SDLThrow(location);
    }
}

inline void CloseSensor(SDL_Sensor *sensor)
{
    SDL_CloseSensor(sensor);
}

inline void UpdateSensors()
{
    SDL_UpdateSensors();
}

inline void LockJoysticks()
{
    SDL_LockJoysticks();
}

inline void UnlockJoysticks()
{
    SDL_UnlockJoysticks();
}

inline void HasJoystick(std::source_location location = std::source_location::current())
{
    if (!SDL_HasJoystick())
    {
        SDLThrow(location);
    }
}

inline SDL_JoystickID *GetJoysticks(int *count, std::source_location location = std::source_location::current())
{
    SDL_JoystickID *result = SDL_GetJoysticks(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetJoystickNameForID(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetJoystickNameForID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetJoystickPathForID(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetJoystickPathForID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int GetJoystickPlayerIndexForID(SDL_JoystickID instance_id)
{
    return SDL_GetJoystickPlayerIndexForID(instance_id);
}

inline SDL_GUID GetJoystickGUIDForID(SDL_JoystickID instance_id)
{
    return SDL_GetJoystickGUIDForID(instance_id);
}

inline Uint16 GetJoystickVendorForID(SDL_JoystickID instance_id)
{
    return SDL_GetJoystickVendorForID(instance_id);
}

inline Uint16 GetJoystickProductForID(SDL_JoystickID instance_id)
{
    return SDL_GetJoystickProductForID(instance_id);
}

inline Uint16 GetJoystickProductVersionForID(SDL_JoystickID instance_id)
{
    return SDL_GetJoystickProductVersionForID(instance_id);
}

inline SDL_JoystickType GetJoystickTypeForID(SDL_JoystickID instance_id)
{
    return SDL_GetJoystickTypeForID(instance_id);
}

inline SDL_Joystick *OpenJoystick(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    SDL_Joystick *result = SDL_OpenJoystick(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Joystick *GetJoystickFromID(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    SDL_Joystick *result = SDL_GetJoystickFromID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Joystick *GetJoystickFromPlayerIndex(int player_index, std::source_location location = std::source_location::current())
{
    SDL_Joystick *result = SDL_GetJoystickFromPlayerIndex(player_index);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_JoystickID AttachVirtualJoystick(const SDL_VirtualJoystickDesc *desc)
{
    return SDL_AttachVirtualJoystick(desc);
}

inline void DetachVirtualJoystick(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    if (!SDL_DetachVirtualJoystick(instance_id))
    {
        SDLThrow(location);
    }
}

inline void IsJoystickVirtual(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    if (!SDL_IsJoystickVirtual(instance_id))
    {
        SDLThrow(location);
    }
}

inline void SetJoystickVirtualAxis(SDL_Joystick *joystick, int axis, Sint16 value, std::source_location location = std::source_location::current())
{
    if (!SDL_SetJoystickVirtualAxis(joystick, axis, value))
    {
        SDLThrow(location);
    }
}

inline void SetJoystickVirtualBall(SDL_Joystick *joystick, int ball, Sint16 xrel, Sint16 yrel, std::source_location location = std::source_location::current())
{
    if (!SDL_SetJoystickVirtualBall(joystick, ball, xrel, yrel))
    {
        SDLThrow(location);
    }
}

inline void SetJoystickVirtualButton(SDL_Joystick *joystick, int button, bool down, std::source_location location = std::source_location::current())
{
    if (!SDL_SetJoystickVirtualButton(joystick, button, down))
    {
        SDLThrow(location);
    }
}

inline void SetJoystickVirtualHat(SDL_Joystick *joystick, int hat, Uint8 value, std::source_location location = std::source_location::current())
{
    if (!SDL_SetJoystickVirtualHat(joystick, hat, value))
    {
        SDLThrow(location);
    }
}

inline void SetJoystickVirtualTouchpad(SDL_Joystick *joystick, int touchpad, int finger, bool down, float x, float y, float pressure, std::source_location location = std::source_location::current())
{
    if (!SDL_SetJoystickVirtualTouchpad(joystick, touchpad, finger, down, x, y, pressure))
    {
        SDLThrow(location);
    }
}

inline void SendJoystickVirtualSensorData(SDL_Joystick *joystick, SDL_SensorType type, Uint64 sensor_timestamp, const float *data, int num_values, std::source_location location = std::source_location::current())
{
    if (!SDL_SendJoystickVirtualSensorData(joystick, type, sensor_timestamp, data, num_values))
    {
        SDLThrow(location);
    }
}

inline SDL_PropertiesID GetJoystickProperties(SDL_Joystick *joystick)
{
    return SDL_GetJoystickProperties(joystick);
}

inline const char *GetJoystickName(SDL_Joystick *joystick, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetJoystickName(joystick);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetJoystickPath(SDL_Joystick *joystick, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetJoystickPath(joystick);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int GetJoystickPlayerIndex(SDL_Joystick *joystick)
{
    return SDL_GetJoystickPlayerIndex(joystick);
}

inline void SetJoystickPlayerIndex(SDL_Joystick *joystick, int player_index, std::source_location location = std::source_location::current())
{
    if (!SDL_SetJoystickPlayerIndex(joystick, player_index))
    {
        SDLThrow(location);
    }
}

inline SDL_GUID GetJoystickGUID(SDL_Joystick *joystick)
{
    return SDL_GetJoystickGUID(joystick);
}

inline Uint16 GetJoystickVendor(SDL_Joystick *joystick)
{
    return SDL_GetJoystickVendor(joystick);
}

inline Uint16 GetJoystickProduct(SDL_Joystick *joystick)
{
    return SDL_GetJoystickProduct(joystick);
}

inline Uint16 GetJoystickProductVersion(SDL_Joystick *joystick)
{
    return SDL_GetJoystickProductVersion(joystick);
}

inline Uint16 GetJoystickFirmwareVersion(SDL_Joystick *joystick)
{
    return SDL_GetJoystickFirmwareVersion(joystick);
}

inline const char *GetJoystickSerial(SDL_Joystick *joystick, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetJoystickSerial(joystick);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_JoystickType GetJoystickType(SDL_Joystick *joystick)
{
    return SDL_GetJoystickType(joystick);
}

inline void GetJoystickGUIDInfo(SDL_GUID guid, Uint16 *vendor, Uint16 *product, Uint16 *version, Uint16 *crc16)
{
    SDL_GetJoystickGUIDInfo(guid, vendor, product, version, crc16);
}

inline void JoystickConnected(SDL_Joystick *joystick, std::source_location location = std::source_location::current())
{
    if (!SDL_JoystickConnected(joystick))
    {
        SDLThrow(location);
    }
}

inline SDL_JoystickID GetJoystickID(SDL_Joystick *joystick)
{
    return SDL_GetJoystickID(joystick);
}

inline int GetNumJoystickAxes(SDL_Joystick *joystick)
{
    return SDL_GetNumJoystickAxes(joystick);
}

inline int GetNumJoystickBalls(SDL_Joystick *joystick)
{
    return SDL_GetNumJoystickBalls(joystick);
}

inline int GetNumJoystickHats(SDL_Joystick *joystick)
{
    return SDL_GetNumJoystickHats(joystick);
}

inline int GetNumJoystickButtons(SDL_Joystick *joystick)
{
    return SDL_GetNumJoystickButtons(joystick);
}

inline void SetJoystickEventsEnabled(bool enabled)
{
    SDL_SetJoystickEventsEnabled(enabled);
}

inline void JoystickEventsEnabled(std::source_location location = std::source_location::current())
{
    if (!SDL_JoystickEventsEnabled())
    {
        SDLThrow(location);
    }
}

inline void UpdateJoysticks()
{
    SDL_UpdateJoysticks();
}

inline Sint16 GetJoystickAxis(SDL_Joystick *joystick, int axis)
{
    return SDL_GetJoystickAxis(joystick, axis);
}

inline void GetJoystickAxisInitialState(SDL_Joystick *joystick, int axis, Sint16 *state, std::source_location location = std::source_location::current())
{
    if (!SDL_GetJoystickAxisInitialState(joystick, axis, state))
    {
        SDLThrow(location);
    }
}

inline void GetJoystickBall(SDL_Joystick *joystick, int ball, int *dx, int *dy, std::source_location location = std::source_location::current())
{
    if (!SDL_GetJoystickBall(joystick, ball, dx, dy))
    {
        SDLThrow(location);
    }
}

inline Uint8 GetJoystickHat(SDL_Joystick *joystick, int hat)
{
    return SDL_GetJoystickHat(joystick, hat);
}

inline void GetJoystickButton(SDL_Joystick *joystick, int button, std::source_location location = std::source_location::current())
{
    if (!SDL_GetJoystickButton(joystick, button))
    {
        SDLThrow(location);
    }
}

inline void RumbleJoystick(SDL_Joystick *joystick, Uint16 low_frequency_rumble, Uint16 high_frequency_rumble, Uint32 duration_ms, std::source_location location = std::source_location::current())
{
    if (!SDL_RumbleJoystick(joystick, low_frequency_rumble, high_frequency_rumble, duration_ms))
    {
        SDLThrow(location);
    }
}

inline void RumbleJoystickTriggers(SDL_Joystick *joystick, Uint16 left_rumble, Uint16 right_rumble, Uint32 duration_ms, std::source_location location = std::source_location::current())
{
    if (!SDL_RumbleJoystickTriggers(joystick, left_rumble, right_rumble, duration_ms))
    {
        SDLThrow(location);
    }
}

inline void SetJoystickLED(SDL_Joystick *joystick, Uint8 red, Uint8 green, Uint8 blue, std::source_location location = std::source_location::current())
{
    if (!SDL_SetJoystickLED(joystick, red, green, blue))
    {
        SDLThrow(location);
    }
}

inline void SendJoystickEffect(SDL_Joystick *joystick, const void *data, int size, std::source_location location = std::source_location::current())
{
    if (!SDL_SendJoystickEffect(joystick, data, size))
    {
        SDLThrow(location);
    }
}

inline void CloseJoystick(SDL_Joystick *joystick)
{
    SDL_CloseJoystick(joystick);
}

inline SDL_JoystickConnectionState GetJoystickConnectionState(SDL_Joystick *joystick)
{
    return SDL_GetJoystickConnectionState(joystick);
}

inline SDL_PowerState GetJoystickPowerInfo(SDL_Joystick *joystick, int *percent)
{
    return SDL_GetJoystickPowerInfo(joystick, percent);
}

inline int AddGamepadMapping(const char *mapping)
{
    return SDL_AddGamepadMapping(mapping);
}

inline int AddGamepadMappingsFromIO(SDL_IOStream *src, bool closeio)
{
    return SDL_AddGamepadMappingsFromIO(src, closeio);
}

inline int AddGamepadMappingsFromFile(const char *file)
{
    return SDL_AddGamepadMappingsFromFile(file);
}

inline void ReloadGamepadMappings(std::source_location location = std::source_location::current())
{
    if (!SDL_ReloadGamepadMappings())
    {
        SDLThrow(location);
    }
}

inline char **GetGamepadMappings(int *count, std::source_location location = std::source_location::current())
{
    char **result = SDL_GetGamepadMappings(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *GetGamepadMappingForGUID(SDL_GUID guid, std::source_location location = std::source_location::current())
{
    char *result = SDL_GetGamepadMappingForGUID(guid);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *GetGamepadMapping(SDL_Gamepad *gamepad, std::source_location location = std::source_location::current())
{
    char *result = SDL_GetGamepadMapping(gamepad);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetGamepadMapping(SDL_JoystickID instance_id, const char *mapping, std::source_location location = std::source_location::current())
{
    if (!SDL_SetGamepadMapping(instance_id, mapping))
    {
        SDLThrow(location);
    }
}

inline void HasGamepad(std::source_location location = std::source_location::current())
{
    if (!SDL_HasGamepad())
    {
        SDLThrow(location);
    }
}

inline SDL_JoystickID *GetGamepads(int *count, std::source_location location = std::source_location::current())
{
    SDL_JoystickID *result = SDL_GetGamepads(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void IsGamepad(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    if (!SDL_IsGamepad(instance_id))
    {
        SDLThrow(location);
    }
}

inline const char *GetGamepadNameForID(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGamepadNameForID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetGamepadPathForID(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGamepadPathForID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int GetGamepadPlayerIndexForID(SDL_JoystickID instance_id)
{
    return SDL_GetGamepadPlayerIndexForID(instance_id);
}

inline SDL_GUID GetGamepadGUIDForID(SDL_JoystickID instance_id)
{
    return SDL_GetGamepadGUIDForID(instance_id);
}

inline Uint16 GetGamepadVendorForID(SDL_JoystickID instance_id)
{
    return SDL_GetGamepadVendorForID(instance_id);
}

inline Uint16 GetGamepadProductForID(SDL_JoystickID instance_id)
{
    return SDL_GetGamepadProductForID(instance_id);
}

inline Uint16 GetGamepadProductVersionForID(SDL_JoystickID instance_id)
{
    return SDL_GetGamepadProductVersionForID(instance_id);
}

inline SDL_GamepadType GetGamepadTypeForID(SDL_JoystickID instance_id)
{
    return SDL_GetGamepadTypeForID(instance_id);
}

inline SDL_GamepadType GetRealGamepadTypeForID(SDL_JoystickID instance_id)
{
    return SDL_GetRealGamepadTypeForID(instance_id);
}

inline char *GetGamepadMappingForID(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    char *result = SDL_GetGamepadMappingForID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Gamepad *OpenGamepad(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    SDL_Gamepad *result = SDL_OpenGamepad(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Gamepad *GetGamepadFromID(SDL_JoystickID instance_id, std::source_location location = std::source_location::current())
{
    SDL_Gamepad *result = SDL_GetGamepadFromID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Gamepad *GetGamepadFromPlayerIndex(int player_index, std::source_location location = std::source_location::current())
{
    SDL_Gamepad *result = SDL_GetGamepadFromPlayerIndex(player_index);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_PropertiesID GetGamepadProperties(SDL_Gamepad *gamepad)
{
    return SDL_GetGamepadProperties(gamepad);
}

inline SDL_JoystickID GetGamepadID(SDL_Gamepad *gamepad)
{
    return SDL_GetGamepadID(gamepad);
}

inline const char *GetGamepadName(SDL_Gamepad *gamepad, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGamepadName(gamepad);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetGamepadPath(SDL_Gamepad *gamepad, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGamepadPath(gamepad);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GamepadType GetGamepadType(SDL_Gamepad *gamepad)
{
    return SDL_GetGamepadType(gamepad);
}

inline SDL_GamepadType GetRealGamepadType(SDL_Gamepad *gamepad)
{
    return SDL_GetRealGamepadType(gamepad);
}

inline int GetGamepadPlayerIndex(SDL_Gamepad *gamepad)
{
    return SDL_GetGamepadPlayerIndex(gamepad);
}

inline void SetGamepadPlayerIndex(SDL_Gamepad *gamepad, int player_index, std::source_location location = std::source_location::current())
{
    if (!SDL_SetGamepadPlayerIndex(gamepad, player_index))
    {
        SDLThrow(location);
    }
}

inline Uint16 GetGamepadVendor(SDL_Gamepad *gamepad)
{
    return SDL_GetGamepadVendor(gamepad);
}

inline Uint16 GetGamepadProduct(SDL_Gamepad *gamepad)
{
    return SDL_GetGamepadProduct(gamepad);
}

inline Uint16 GetGamepadProductVersion(SDL_Gamepad *gamepad)
{
    return SDL_GetGamepadProductVersion(gamepad);
}

inline Uint16 GetGamepadFirmwareVersion(SDL_Gamepad *gamepad)
{
    return SDL_GetGamepadFirmwareVersion(gamepad);
}

inline const char *GetGamepadSerial(SDL_Gamepad *gamepad, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGamepadSerial(gamepad);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline Uint64 GetGamepadSteamHandle(SDL_Gamepad *gamepad)
{
    return SDL_GetGamepadSteamHandle(gamepad);
}

inline SDL_JoystickConnectionState GetGamepadConnectionState(SDL_Gamepad *gamepad)
{
    return SDL_GetGamepadConnectionState(gamepad);
}

inline SDL_PowerState GetGamepadPowerInfo(SDL_Gamepad *gamepad, int *percent)
{
    return SDL_GetGamepadPowerInfo(gamepad, percent);
}

inline void GamepadConnected(SDL_Gamepad *gamepad, std::source_location location = std::source_location::current())
{
    if (!SDL_GamepadConnected(gamepad))
    {
        SDLThrow(location);
    }
}

inline SDL_Joystick *GetGamepadJoystick(SDL_Gamepad *gamepad, std::source_location location = std::source_location::current())
{
    SDL_Joystick *result = SDL_GetGamepadJoystick(gamepad);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetGamepadEventsEnabled(bool enabled)
{
    SDL_SetGamepadEventsEnabled(enabled);
}

inline void GamepadEventsEnabled(std::source_location location = std::source_location::current())
{
    if (!SDL_GamepadEventsEnabled())
    {
        SDLThrow(location);
    }
}

inline SDL_GamepadBinding **GetGamepadBindings(SDL_Gamepad *gamepad, int *count, std::source_location location = std::source_location::current())
{
    SDL_GamepadBinding **result = SDL_GetGamepadBindings(gamepad, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void UpdateGamepads()
{
    SDL_UpdateGamepads();
}

inline SDL_GamepadType GetGamepadTypeFromString(const char *str)
{
    return SDL_GetGamepadTypeFromString(str);
}

inline const char *GetGamepadStringForType(SDL_GamepadType type, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGamepadStringForType(type);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GamepadAxis GetGamepadAxisFromString(const char *str)
{
    return SDL_GetGamepadAxisFromString(str);
}

inline const char *GetGamepadStringForAxis(SDL_GamepadAxis axis, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGamepadStringForAxis(axis);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void GamepadHasAxis(SDL_Gamepad *gamepad, SDL_GamepadAxis axis, std::source_location location = std::source_location::current())
{
    if (!SDL_GamepadHasAxis(gamepad, axis))
    {
        SDLThrow(location);
    }
}

inline Sint16 GetGamepadAxis(SDL_Gamepad *gamepad, SDL_GamepadAxis axis)
{
    return SDL_GetGamepadAxis(gamepad, axis);
}

inline SDL_GamepadButton GetGamepadButtonFromString(const char *str)
{
    return SDL_GetGamepadButtonFromString(str);
}

inline const char *GetGamepadStringForButton(SDL_GamepadButton button, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGamepadStringForButton(button);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void GamepadHasButton(SDL_Gamepad *gamepad, SDL_GamepadButton button, std::source_location location = std::source_location::current())
{
    if (!SDL_GamepadHasButton(gamepad, button))
    {
        SDLThrow(location);
    }
}

inline void GetGamepadButton(SDL_Gamepad *gamepad, SDL_GamepadButton button, std::source_location location = std::source_location::current())
{
    if (!SDL_GetGamepadButton(gamepad, button))
    {
        SDLThrow(location);
    }
}

inline SDL_GamepadButtonLabel GetGamepadButtonLabelForType(SDL_GamepadType type, SDL_GamepadButton button)
{
    return SDL_GetGamepadButtonLabelForType(type, button);
}

inline SDL_GamepadButtonLabel GetGamepadButtonLabel(SDL_Gamepad *gamepad, SDL_GamepadButton button)
{
    return SDL_GetGamepadButtonLabel(gamepad, button);
}

inline int GetNumGamepadTouchpads(SDL_Gamepad *gamepad)
{
    return SDL_GetNumGamepadTouchpads(gamepad);
}

inline int GetNumGamepadTouchpadFingers(SDL_Gamepad *gamepad, int touchpad)
{
    return SDL_GetNumGamepadTouchpadFingers(gamepad, touchpad);
}

inline void GetGamepadTouchpadFinger(SDL_Gamepad *gamepad, int touchpad, int finger, bool *down, float *x, float *y, float *pressure, std::source_location location = std::source_location::current())
{
    if (!SDL_GetGamepadTouchpadFinger(gamepad, touchpad, finger, down, x, y, pressure))
    {
        SDLThrow(location);
    }
}

inline void GamepadHasSensor(SDL_Gamepad *gamepad, SDL_SensorType type, std::source_location location = std::source_location::current())
{
    if (!SDL_GamepadHasSensor(gamepad, type))
    {
        SDLThrow(location);
    }
}

inline void SetGamepadSensorEnabled(SDL_Gamepad *gamepad, SDL_SensorType type, bool enabled, std::source_location location = std::source_location::current())
{
    if (!SDL_SetGamepadSensorEnabled(gamepad, type, enabled))
    {
        SDLThrow(location);
    }
}

inline void GamepadSensorEnabled(SDL_Gamepad *gamepad, SDL_SensorType type, std::source_location location = std::source_location::current())
{
    if (!SDL_GamepadSensorEnabled(gamepad, type))
    {
        SDLThrow(location);
    }
}

inline float GetGamepadSensorDataRate(SDL_Gamepad *gamepad, SDL_SensorType type)
{
    return SDL_GetGamepadSensorDataRate(gamepad, type);
}

inline void GetGamepadSensorData(SDL_Gamepad *gamepad, SDL_SensorType type, float *data, int num_values, std::source_location location = std::source_location::current())
{
    if (!SDL_GetGamepadSensorData(gamepad, type, data, num_values))
    {
        SDLThrow(location);
    }
}

inline void RumbleGamepad(SDL_Gamepad *gamepad, Uint16 low_frequency_rumble, Uint16 high_frequency_rumble, Uint32 duration_ms, std::source_location location = std::source_location::current())
{
    if (!SDL_RumbleGamepad(gamepad, low_frequency_rumble, high_frequency_rumble, duration_ms))
    {
        SDLThrow(location);
    }
}

inline void RumbleGamepadTriggers(SDL_Gamepad *gamepad, Uint16 left_rumble, Uint16 right_rumble, Uint32 duration_ms, std::source_location location = std::source_location::current())
{
    if (!SDL_RumbleGamepadTriggers(gamepad, left_rumble, right_rumble, duration_ms))
    {
        SDLThrow(location);
    }
}

inline void SetGamepadLED(SDL_Gamepad *gamepad, Uint8 red, Uint8 green, Uint8 blue, std::source_location location = std::source_location::current())
{
    if (!SDL_SetGamepadLED(gamepad, red, green, blue))
    {
        SDLThrow(location);
    }
}

inline void SendGamepadEffect(SDL_Gamepad *gamepad, const void *data, int size, std::source_location location = std::source_location::current())
{
    if (!SDL_SendGamepadEffect(gamepad, data, size))
    {
        SDLThrow(location);
    }
}

inline void CloseGamepad(SDL_Gamepad *gamepad)
{
    SDL_CloseGamepad(gamepad);
}

inline const char *GetGamepadAppleSFSymbolsNameForButton(SDL_Gamepad *gamepad, SDL_GamepadButton button, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGamepadAppleSFSymbolsNameForButton(gamepad, button);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetGamepadAppleSFSymbolsNameForAxis(SDL_Gamepad *gamepad, SDL_GamepadAxis axis, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGamepadAppleSFSymbolsNameForAxis(gamepad, axis);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void HasKeyboard(std::source_location location = std::source_location::current())
{
    if (!SDL_HasKeyboard())
    {
        SDLThrow(location);
    }
}

inline SDL_KeyboardID *GetKeyboards(int *count, std::source_location location = std::source_location::current())
{
    SDL_KeyboardID *result = SDL_GetKeyboards(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetKeyboardNameForID(SDL_KeyboardID instance_id, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetKeyboardNameForID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Window *GetKeyboardFocus(std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_GetKeyboardFocus();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const bool *GetKeyboardState(int *numkeys, std::source_location location = std::source_location::current())
{
    const bool *result = SDL_GetKeyboardState(numkeys);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void ResetKeyboard()
{
    SDL_ResetKeyboard();
}

inline SDL_Keymod GetModState()
{
    return SDL_GetModState();
}

inline void SetModState(SDL_Keymod modstate)
{
    SDL_SetModState(modstate);
}

inline SDL_Keycode GetKeyFromScancode(SDL_Scancode scancode, SDL_Keymod modstate, bool key_event)
{
    return SDL_GetKeyFromScancode(scancode, modstate, key_event);
}

inline SDL_Scancode GetScancodeFromKey(SDL_Keycode key, SDL_Keymod *modstate)
{
    return SDL_GetScancodeFromKey(key, modstate);
}

inline void SetScancodeName(SDL_Scancode scancode, const char *name, std::source_location location = std::source_location::current())
{
    if (!SDL_SetScancodeName(scancode, name))
    {
        SDLThrow(location);
    }
}

inline const char *GetScancodeName(SDL_Scancode scancode, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetScancodeName(scancode);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Scancode GetScancodeFromName(const char *name)
{
    return SDL_GetScancodeFromName(name);
}

inline const char *GetKeyName(SDL_Keycode key, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetKeyName(key);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Keycode GetKeyFromName(const char *name)
{
    return SDL_GetKeyFromName(name);
}

inline void StartTextInput(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_StartTextInput(window))
    {
        SDLThrow(location);
    }
}

inline void StartTextInputWithProperties(SDL_Window *window, SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    if (!SDL_StartTextInputWithProperties(window, props))
    {
        SDLThrow(location);
    }
}

inline void TextInputActive(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_TextInputActive(window))
    {
        SDLThrow(location);
    }
}

inline void StopTextInput(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_StopTextInput(window))
    {
        SDLThrow(location);
    }
}

inline void ClearComposition(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_ClearComposition(window))
    {
        SDLThrow(location);
    }
}

inline void SetTextInputArea(SDL_Window *window, const SDL_Rect *rect, int cursor, std::source_location location = std::source_location::current())
{
    if (!SDL_SetTextInputArea(window, rect, cursor))
    {
        SDLThrow(location);
    }
}

inline void GetTextInputArea(SDL_Window *window, SDL_Rect *rect, int *cursor, std::source_location location = std::source_location::current())
{
    if (!SDL_GetTextInputArea(window, rect, cursor))
    {
        SDLThrow(location);
    }
}

inline void HasScreenKeyboardSupport(std::source_location location = std::source_location::current())
{
    if (!SDL_HasScreenKeyboardSupport())
    {
        SDLThrow(location);
    }
}

inline void ScreenKeyboardShown(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_ScreenKeyboardShown(window))
    {
        SDLThrow(location);
    }
}

inline void HasMouse(std::source_location location = std::source_location::current())
{
    if (!SDL_HasMouse())
    {
        SDLThrow(location);
    }
}

inline SDL_MouseID *GetMice(int *count, std::source_location location = std::source_location::current())
{
    SDL_MouseID *result = SDL_GetMice(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetMouseNameForID(SDL_MouseID instance_id, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetMouseNameForID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Window *GetMouseFocus(std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_GetMouseFocus();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_MouseButtonFlags GetMouseState(float *x, float *y)
{
    return SDL_GetMouseState(x, y);
}

inline SDL_MouseButtonFlags GetGlobalMouseState(float *x, float *y)
{
    return SDL_GetGlobalMouseState(x, y);
}

inline SDL_MouseButtonFlags GetRelativeMouseState(float *x, float *y)
{
    return SDL_GetRelativeMouseState(x, y);
}

inline void WarpMouseInWindow(SDL_Window *window, float x, float y)
{
    SDL_WarpMouseInWindow(window, x, y);
}

inline void WarpMouseGlobal(float x, float y, std::source_location location = std::source_location::current())
{
    if (!SDL_WarpMouseGlobal(x, y))
    {
        SDLThrow(location);
    }
}

inline void SetWindowRelativeMouseMode(SDL_Window *window, bool enabled, std::source_location location = std::source_location::current())
{
    if (!SDL_SetWindowRelativeMouseMode(window, enabled))
    {
        SDLThrow(location);
    }
}

inline void GetWindowRelativeMouseMode(SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_GetWindowRelativeMouseMode(window))
    {
        SDLThrow(location);
    }
}

inline void CaptureMouse(bool enabled, std::source_location location = std::source_location::current())
{
    if (!SDL_CaptureMouse(enabled))
    {
        SDLThrow(location);
    }
}

inline SDL_Cursor *CreateCursor(const Uint8 *data, const Uint8 *mask, int w, int h, int hot_x, int hot_y, std::source_location location = std::source_location::current())
{
    SDL_Cursor *result = SDL_CreateCursor(data, mask, w, h, hot_x, hot_y);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Cursor *CreateColorCursor(SDL_Surface *surface, int hot_x, int hot_y, std::source_location location = std::source_location::current())
{
    SDL_Cursor *result = SDL_CreateColorCursor(surface, hot_x, hot_y);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Cursor *CreateSystemCursor(SDL_SystemCursor id, std::source_location location = std::source_location::current())
{
    SDL_Cursor *result = SDL_CreateSystemCursor(id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetCursor(SDL_Cursor *cursor, std::source_location location = std::source_location::current())
{
    if (!SDL_SetCursor(cursor))
    {
        SDLThrow(location);
    }
}

inline SDL_Cursor *GetCursor(std::source_location location = std::source_location::current())
{
    SDL_Cursor *result = SDL_GetCursor();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Cursor *GetDefaultCursor(std::source_location location = std::source_location::current())
{
    SDL_Cursor *result = SDL_GetDefaultCursor();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void DestroyCursor(SDL_Cursor *cursor)
{
    SDL_DestroyCursor(cursor);
}

inline void ShowCursor(std::source_location location = std::source_location::current())
{
    if (!SDL_ShowCursor())
    {
        SDLThrow(location);
    }
}

inline void HideCursor(std::source_location location = std::source_location::current())
{
    if (!SDL_HideCursor())
    {
        SDLThrow(location);
    }
}

inline void CursorVisible(std::source_location location = std::source_location::current())
{
    if (!SDL_CursorVisible())
    {
        SDLThrow(location);
    }
}

inline SDL_TouchID *GetTouchDevices(int *count, std::source_location location = std::source_location::current())
{
    SDL_TouchID *result = SDL_GetTouchDevices(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetTouchDeviceName(SDL_TouchID touchID, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetTouchDeviceName(touchID);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_TouchDeviceType GetTouchDeviceType(SDL_TouchID touchID)
{
    return SDL_GetTouchDeviceType(touchID);
}

inline SDL_Finger **GetTouchFingers(SDL_TouchID touchID, int *count, std::source_location location = std::source_location::current())
{
    SDL_Finger **result = SDL_GetTouchFingers(touchID, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void PumpEvents()
{
    SDL_PumpEvents();
}

inline int PeepEvents(SDL_Event *events, int numevents, SDL_EventAction action, Uint32 minType, Uint32 maxType)
{
    return SDL_PeepEvents(events, numevents, action, minType, maxType);
}

inline void HasEvent(Uint32 type, std::source_location location = std::source_location::current())
{
    if (!SDL_HasEvent(type))
    {
        SDLThrow(location);
    }
}

inline void HasEvents(Uint32 minType, Uint32 maxType, std::source_location location = std::source_location::current())
{
    if (!SDL_HasEvents(minType, maxType))
    {
        SDLThrow(location);
    }
}

inline void FlushEvent(Uint32 type)
{
    SDL_FlushEvent(type);
}

inline void FlushEvents(Uint32 minType, Uint32 maxType)
{
    SDL_FlushEvents(minType, maxType);
}

inline void PollEvent(SDL_Event *event, std::source_location location = std::source_location::current())
{
    if (!SDL_PollEvent(event))
    {
        SDLThrow(location);
    }
}

inline void WaitEvent(SDL_Event *event, std::source_location location = std::source_location::current())
{
    if (!SDL_WaitEvent(event))
    {
        SDLThrow(location);
    }
}

inline void WaitEventTimeout(SDL_Event *event, Sint32 timeoutMS, std::source_location location = std::source_location::current())
{
    if (!SDL_WaitEventTimeout(event, timeoutMS))
    {
        SDLThrow(location);
    }
}

inline void PushEvent(SDL_Event *event, std::source_location location = std::source_location::current())
{
    if (!SDL_PushEvent(event))
    {
        SDLThrow(location);
    }
}

inline void SetEventFilter(SDL_EventFilter filter, void *userdata)
{
    SDL_SetEventFilter(filter, userdata);
}

inline void GetEventFilter(SDL_EventFilter *filter, void **userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_GetEventFilter(filter, userdata))
    {
        SDLThrow(location);
    }
}

inline void AddEventWatch(SDL_EventFilter filter, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_AddEventWatch(filter, userdata))
    {
        SDLThrow(location);
    }
}

inline void RemoveEventWatch(SDL_EventFilter filter, void *userdata)
{
    SDL_RemoveEventWatch(filter, userdata);
}

inline void FilterEvents(SDL_EventFilter filter, void *userdata)
{
    SDL_FilterEvents(filter, userdata);
}

inline void SetEventEnabled(Uint32 type, bool enabled)
{
    SDL_SetEventEnabled(type, enabled);
}

inline void EventEnabled(Uint32 type, std::source_location location = std::source_location::current())
{
    if (!SDL_EventEnabled(type))
    {
        SDLThrow(location);
    }
}

inline Uint32 RegisterEvents(int numevents)
{
    return SDL_RegisterEvents(numevents);
}

inline SDL_Window *GetWindowFromEvent(const SDL_Event *event, std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_GetWindowFromEvent(event);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetBasePath(std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetBasePath();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *GetPrefPath(const char *org, const char *app, std::source_location location = std::source_location::current())
{
    char *result = SDL_GetPrefPath(org, app);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetUserFolder(SDL_Folder folder, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetUserFolder(folder);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void CreateDirectory(const char *path, std::source_location location = std::source_location::current())
{
    if (!SDL_CreateDirectory(path))
    {
        SDLThrow(location);
    }
}

inline void EnumerateDirectory(const char *path, SDL_EnumerateDirectoryCallback callback, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_EnumerateDirectory(path, callback, userdata))
    {
        SDLThrow(location);
    }
}

inline void RemovePath(const char *path, std::source_location location = std::source_location::current())
{
    if (!SDL_RemovePath(path))
    {
        SDLThrow(location);
    }
}

inline void RenamePath(const char *oldpath, const char *newpath, std::source_location location = std::source_location::current())
{
    if (!SDL_RenamePath(oldpath, newpath))
    {
        SDLThrow(location);
    }
}

inline void CopyFile(const char *oldpath, const char *newpath, std::source_location location = std::source_location::current())
{
    if (!SDL_CopyFile(oldpath, newpath))
    {
        SDLThrow(location);
    }
}

inline void GetPathInfo(const char *path, SDL_PathInfo *info, std::source_location location = std::source_location::current())
{
    if (!SDL_GetPathInfo(path, info))
    {
        SDLThrow(location);
    }
}

inline char **GlobDirectory(const char *path, const char *pattern, SDL_GlobFlags flags, int *count, std::source_location location = std::source_location::current())
{
    char **result = SDL_GlobDirectory(path, pattern, flags, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline char *GetCurrentDirectory(std::source_location location = std::source_location::current())
{
    char *result = SDL_GetCurrentDirectory();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void GPUSupportsShaderFormats(SDL_GPUShaderFormat format_flags, const char *name, std::source_location location = std::source_location::current())
{
    if (!SDL_GPUSupportsShaderFormats(format_flags, name))
    {
        SDLThrow(location);
    }
}

inline void GPUSupportsProperties(SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    if (!SDL_GPUSupportsProperties(props))
    {
        SDLThrow(location);
    }
}

inline SDL_GPUDevice *CreateGPUDevice(SDL_GPUShaderFormat format_flags, bool debug_mode, const char *name, std::source_location location = std::source_location::current())
{
    SDL_GPUDevice *result = SDL_CreateGPUDevice(format_flags, debug_mode, name);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GPUDevice *CreateGPUDeviceWithProperties(SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    SDL_GPUDevice *result = SDL_CreateGPUDeviceWithProperties(props);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void DestroyGPUDevice(SDL_GPUDevice *device)
{
    SDL_DestroyGPUDevice(device);
}

inline int GetNumGPUDrivers()
{
    return SDL_GetNumGPUDrivers();
}

inline const char *GetGPUDriver(int index, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGPUDriver(index);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetGPUDeviceDriver(SDL_GPUDevice *device, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetGPUDeviceDriver(device);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GPUShaderFormat GetGPUShaderFormats(SDL_GPUDevice *device)
{
    return SDL_GetGPUShaderFormats(device);
}

inline SDL_GPUComputePipeline *CreateGPUComputePipeline(SDL_GPUDevice *device, const SDL_GPUComputePipelineCreateInfo *createinfo, std::source_location location = std::source_location::current())
{
    SDL_GPUComputePipeline *result = SDL_CreateGPUComputePipeline(device, createinfo);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GPUGraphicsPipeline *CreateGPUGraphicsPipeline(SDL_GPUDevice *device, const SDL_GPUGraphicsPipelineCreateInfo *createinfo, std::source_location location = std::source_location::current())
{
    SDL_GPUGraphicsPipeline *result = SDL_CreateGPUGraphicsPipeline(device, createinfo);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GPUSampler *CreateGPUSampler(SDL_GPUDevice *device, const SDL_GPUSamplerCreateInfo *createinfo, std::source_location location = std::source_location::current())
{
    SDL_GPUSampler *result = SDL_CreateGPUSampler(device, createinfo);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GPUShader *CreateGPUShader(SDL_GPUDevice *device, const SDL_GPUShaderCreateInfo *createinfo, std::source_location location = std::source_location::current())
{
    SDL_GPUShader *result = SDL_CreateGPUShader(device, createinfo);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GPUTexture *CreateGPUTexture(SDL_GPUDevice *device, const SDL_GPUTextureCreateInfo *createinfo, std::source_location location = std::source_location::current())
{
    SDL_GPUTexture *result = SDL_CreateGPUTexture(device, createinfo);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GPUBuffer *CreateGPUBuffer(SDL_GPUDevice *device, const SDL_GPUBufferCreateInfo *createinfo, std::source_location location = std::source_location::current())
{
    SDL_GPUBuffer *result = SDL_CreateGPUBuffer(device, createinfo);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_GPUTransferBuffer *CreateGPUTransferBuffer(SDL_GPUDevice *device, const SDL_GPUTransferBufferCreateInfo *createinfo, std::source_location location = std::source_location::current())
{
    SDL_GPUTransferBuffer *result = SDL_CreateGPUTransferBuffer(device, createinfo);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetGPUBufferName(SDL_GPUDevice *device, SDL_GPUBuffer *buffer, const char *text)
{
    SDL_SetGPUBufferName(device, buffer, text);
}

inline void SetGPUTextureName(SDL_GPUDevice *device, SDL_GPUTexture *texture, const char *text)
{
    SDL_SetGPUTextureName(device, texture, text);
}

inline void InsertGPUDebugLabel(SDL_GPUCommandBuffer *command_buffer, const char *text)
{
    SDL_InsertGPUDebugLabel(command_buffer, text);
}

inline void PushGPUDebugGroup(SDL_GPUCommandBuffer *command_buffer, const char *name)
{
    SDL_PushGPUDebugGroup(command_buffer, name);
}

inline void PopGPUDebugGroup(SDL_GPUCommandBuffer *command_buffer)
{
    SDL_PopGPUDebugGroup(command_buffer);
}

inline void ReleaseGPUTexture(SDL_GPUDevice *device, SDL_GPUTexture *texture)
{
    SDL_ReleaseGPUTexture(device, texture);
}

inline void ReleaseGPUSampler(SDL_GPUDevice *device, SDL_GPUSampler *sampler)
{
    SDL_ReleaseGPUSampler(device, sampler);
}

inline void ReleaseGPUBuffer(SDL_GPUDevice *device, SDL_GPUBuffer *buffer)
{
    SDL_ReleaseGPUBuffer(device, buffer);
}

inline void ReleaseGPUTransferBuffer(SDL_GPUDevice *device, SDL_GPUTransferBuffer *transfer_buffer)
{
    SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
}

inline void ReleaseGPUComputePipeline(SDL_GPUDevice *device, SDL_GPUComputePipeline *compute_pipeline)
{
    SDL_ReleaseGPUComputePipeline(device, compute_pipeline);
}

inline void ReleaseGPUShader(SDL_GPUDevice *device, SDL_GPUShader *shader)
{
    SDL_ReleaseGPUShader(device, shader);
}

inline void ReleaseGPUGraphicsPipeline(SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *graphics_pipeline)
{
    SDL_ReleaseGPUGraphicsPipeline(device, graphics_pipeline);
}

inline SDL_GPUCommandBuffer *AcquireGPUCommandBuffer(SDL_GPUDevice *device, std::source_location location = std::source_location::current())
{
    SDL_GPUCommandBuffer *result = SDL_AcquireGPUCommandBuffer(device);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void PushGPUVertexUniformData(SDL_GPUCommandBuffer *command_buffer, Uint32 slot_index, const void *data, Uint32 length)
{
    SDL_PushGPUVertexUniformData(command_buffer, slot_index, data, length);
}

inline void PushGPUFragmentUniformData(SDL_GPUCommandBuffer *command_buffer, Uint32 slot_index, const void *data, Uint32 length)
{
    SDL_PushGPUFragmentUniformData(command_buffer, slot_index, data, length);
}

inline void PushGPUComputeUniformData(SDL_GPUCommandBuffer *command_buffer, Uint32 slot_index, const void *data, Uint32 length)
{
    SDL_PushGPUComputeUniformData(command_buffer, slot_index, data, length);
}

inline SDL_GPURenderPass *BeginGPURenderPass(SDL_GPUCommandBuffer *command_buffer, const SDL_GPUColorTargetInfo *color_target_infos, Uint32 num_color_targets, const SDL_GPUDepthStencilTargetInfo *depth_stencil_target_info, std::source_location location = std::source_location::current())
{
    SDL_GPURenderPass *result = SDL_BeginGPURenderPass(command_buffer, color_target_infos, num_color_targets, depth_stencil_target_info);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void BindGPUGraphicsPipeline(SDL_GPURenderPass *render_pass, SDL_GPUGraphicsPipeline *graphics_pipeline)
{
    SDL_BindGPUGraphicsPipeline(render_pass, graphics_pipeline);
}

inline void SetGPUViewport(SDL_GPURenderPass *render_pass, const SDL_GPUViewport *viewport)
{
    SDL_SetGPUViewport(render_pass, viewport);
}

inline void SetGPUScissor(SDL_GPURenderPass *render_pass, const SDL_Rect *scissor)
{
    SDL_SetGPUScissor(render_pass, scissor);
}

inline void SetGPUBlendConstants(SDL_GPURenderPass *render_pass, SDL_FColor blend_constants)
{
    SDL_SetGPUBlendConstants(render_pass, blend_constants);
}

inline void SetGPUStencilReference(SDL_GPURenderPass *render_pass, Uint8 reference)
{
    SDL_SetGPUStencilReference(render_pass, reference);
}

inline void BindGPUVertexBuffers(SDL_GPURenderPass *render_pass, Uint32 first_slot, const SDL_GPUBufferBinding *bindings, Uint32 num_bindings)
{
    SDL_BindGPUVertexBuffers(render_pass, first_slot, bindings, num_bindings);
}

inline void BindGPUIndexBuffer(SDL_GPURenderPass *render_pass, const SDL_GPUBufferBinding *binding, SDL_GPUIndexElementSize index_element_size)
{
    SDL_BindGPUIndexBuffer(render_pass, binding, index_element_size);
}

inline void BindGPUVertexSamplers(SDL_GPURenderPass *render_pass, Uint32 first_slot, const SDL_GPUTextureSamplerBinding *texture_sampler_bindings, Uint32 num_bindings)
{
    SDL_BindGPUVertexSamplers(render_pass, first_slot, texture_sampler_bindings, num_bindings);
}

inline void BindGPUVertexStorageTextures(SDL_GPURenderPass *render_pass, Uint32 first_slot, SDL_GPUTexture *const *storage_textures, Uint32 num_bindings)
{
    SDL_BindGPUVertexStorageTextures(render_pass, first_slot, storage_textures, num_bindings);
}

inline void BindGPUVertexStorageBuffers(SDL_GPURenderPass *render_pass, Uint32 first_slot, SDL_GPUBuffer *const *storage_buffers, Uint32 num_bindings)
{
    SDL_BindGPUVertexStorageBuffers(render_pass, first_slot, storage_buffers, num_bindings);
}

inline void BindGPUFragmentSamplers(SDL_GPURenderPass *render_pass, Uint32 first_slot, const SDL_GPUTextureSamplerBinding *texture_sampler_bindings, Uint32 num_bindings)
{
    SDL_BindGPUFragmentSamplers(render_pass, first_slot, texture_sampler_bindings, num_bindings);
}

inline void BindGPUFragmentStorageTextures(SDL_GPURenderPass *render_pass, Uint32 first_slot, SDL_GPUTexture *const *storage_textures, Uint32 num_bindings)
{
    SDL_BindGPUFragmentStorageTextures(render_pass, first_slot, storage_textures, num_bindings);
}

inline void BindGPUFragmentStorageBuffers(SDL_GPURenderPass *render_pass, Uint32 first_slot, SDL_GPUBuffer *const *storage_buffers, Uint32 num_bindings)
{
    SDL_BindGPUFragmentStorageBuffers(render_pass, first_slot, storage_buffers, num_bindings);
}

inline void DrawGPUIndexedPrimitives(SDL_GPURenderPass *render_pass, Uint32 num_indices, Uint32 num_instances, Uint32 first_index, Sint32 vertex_offset, Uint32 first_instance)
{
    SDL_DrawGPUIndexedPrimitives(render_pass, num_indices, num_instances, first_index, vertex_offset, first_instance);
}

inline void DrawGPUPrimitives(SDL_GPURenderPass *render_pass, Uint32 num_vertices, Uint32 num_instances, Uint32 first_vertex, Uint32 first_instance)
{
    SDL_DrawGPUPrimitives(render_pass, num_vertices, num_instances, first_vertex, first_instance);
}

inline void DrawGPUPrimitivesIndirect(SDL_GPURenderPass *render_pass, SDL_GPUBuffer *buffer, Uint32 offset, Uint32 draw_count)
{
    SDL_DrawGPUPrimitivesIndirect(render_pass, buffer, offset, draw_count);
}

inline void DrawGPUIndexedPrimitivesIndirect(SDL_GPURenderPass *render_pass, SDL_GPUBuffer *buffer, Uint32 offset, Uint32 draw_count)
{
    SDL_DrawGPUIndexedPrimitivesIndirect(render_pass, buffer, offset, draw_count);
}

inline void EndGPURenderPass(SDL_GPURenderPass *render_pass)
{
    SDL_EndGPURenderPass(render_pass);
}

inline SDL_GPUComputePass *BeginGPUComputePass(SDL_GPUCommandBuffer *command_buffer, const SDL_GPUStorageTextureReadWriteBinding *storage_texture_bindings, Uint32 num_storage_texture_bindings, const SDL_GPUStorageBufferReadWriteBinding *storage_buffer_bindings, Uint32 num_storage_buffer_bindings, std::source_location location = std::source_location::current())
{
    SDL_GPUComputePass *result = SDL_BeginGPUComputePass(command_buffer, storage_texture_bindings, num_storage_texture_bindings, storage_buffer_bindings, num_storage_buffer_bindings);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void BindGPUComputePipeline(SDL_GPUComputePass *compute_pass, SDL_GPUComputePipeline *compute_pipeline)
{
    SDL_BindGPUComputePipeline(compute_pass, compute_pipeline);
}

inline void BindGPUComputeSamplers(SDL_GPUComputePass *compute_pass, Uint32 first_slot, const SDL_GPUTextureSamplerBinding *texture_sampler_bindings, Uint32 num_bindings)
{
    SDL_BindGPUComputeSamplers(compute_pass, first_slot, texture_sampler_bindings, num_bindings);
}

inline void BindGPUComputeStorageTextures(SDL_GPUComputePass *compute_pass, Uint32 first_slot, SDL_GPUTexture *const *storage_textures, Uint32 num_bindings)
{
    SDL_BindGPUComputeStorageTextures(compute_pass, first_slot, storage_textures, num_bindings);
}

inline void BindGPUComputeStorageBuffers(SDL_GPUComputePass *compute_pass, Uint32 first_slot, SDL_GPUBuffer *const *storage_buffers, Uint32 num_bindings)
{
    SDL_BindGPUComputeStorageBuffers(compute_pass, first_slot, storage_buffers, num_bindings);
}

inline void DispatchGPUCompute(SDL_GPUComputePass *compute_pass, Uint32 groupcount_x, Uint32 groupcount_y, Uint32 groupcount_z)
{
    SDL_DispatchGPUCompute(compute_pass, groupcount_x, groupcount_y, groupcount_z);
}

inline void DispatchGPUComputeIndirect(SDL_GPUComputePass *compute_pass, SDL_GPUBuffer *buffer, Uint32 offset)
{
    SDL_DispatchGPUComputeIndirect(compute_pass, buffer, offset);
}

inline void EndGPUComputePass(SDL_GPUComputePass *compute_pass)
{
    SDL_EndGPUComputePass(compute_pass);
}

inline void *MapGPUTransferBuffer(SDL_GPUDevice *device, SDL_GPUTransferBuffer *transfer_buffer, bool cycle, std::source_location location = std::source_location::current())
{
    void *result = SDL_MapGPUTransferBuffer(device, transfer_buffer, cycle);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void UnmapGPUTransferBuffer(SDL_GPUDevice *device, SDL_GPUTransferBuffer *transfer_buffer)
{
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
}

inline SDL_GPUCopyPass *BeginGPUCopyPass(SDL_GPUCommandBuffer *command_buffer, std::source_location location = std::source_location::current())
{
    SDL_GPUCopyPass *result = SDL_BeginGPUCopyPass(command_buffer);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void UploadToGPUTexture(SDL_GPUCopyPass *copy_pass, const SDL_GPUTextureTransferInfo *source, const SDL_GPUTextureRegion *destination, bool cycle)
{
    SDL_UploadToGPUTexture(copy_pass, source, destination, cycle);
}

inline void UploadToGPUBuffer(SDL_GPUCopyPass *copy_pass, const SDL_GPUTransferBufferLocation *source, const SDL_GPUBufferRegion *destination, bool cycle)
{
    SDL_UploadToGPUBuffer(copy_pass, source, destination, cycle);
}

inline void CopyGPUTextureToTexture(SDL_GPUCopyPass *copy_pass, const SDL_GPUTextureLocation *source, const SDL_GPUTextureLocation *destination, Uint32 w, Uint32 h, Uint32 d, bool cycle)
{
    SDL_CopyGPUTextureToTexture(copy_pass, source, destination, w, h, d, cycle);
}

inline void CopyGPUBufferToBuffer(SDL_GPUCopyPass *copy_pass, const SDL_GPUBufferLocation *source, const SDL_GPUBufferLocation *destination, Uint32 size, bool cycle)
{
    SDL_CopyGPUBufferToBuffer(copy_pass, source, destination, size, cycle);
}

inline void DownloadFromGPUTexture(SDL_GPUCopyPass *copy_pass, const SDL_GPUTextureRegion *source, const SDL_GPUTextureTransferInfo *destination)
{
    SDL_DownloadFromGPUTexture(copy_pass, source, destination);
}

inline void DownloadFromGPUBuffer(SDL_GPUCopyPass *copy_pass, const SDL_GPUBufferRegion *source, const SDL_GPUTransferBufferLocation *destination)
{
    SDL_DownloadFromGPUBuffer(copy_pass, source, destination);
}

inline void EndGPUCopyPass(SDL_GPUCopyPass *copy_pass)
{
    SDL_EndGPUCopyPass(copy_pass);
}

inline void GenerateMipmapsForGPUTexture(SDL_GPUCommandBuffer *command_buffer, SDL_GPUTexture *texture)
{
    SDL_GenerateMipmapsForGPUTexture(command_buffer, texture);
}

inline void BlitGPUTexture(SDL_GPUCommandBuffer *command_buffer, const SDL_GPUBlitInfo *info)
{
    SDL_BlitGPUTexture(command_buffer, info);
}

inline void WindowSupportsGPUSwapchainComposition(SDL_GPUDevice *device, SDL_Window *window, SDL_GPUSwapchainComposition swapchain_composition, std::source_location location = std::source_location::current())
{
    if (!SDL_WindowSupportsGPUSwapchainComposition(device, window, swapchain_composition))
    {
        SDLThrow(location);
    }
}

inline void WindowSupportsGPUPresentMode(SDL_GPUDevice *device, SDL_Window *window, SDL_GPUPresentMode present_mode, std::source_location location = std::source_location::current())
{
    if (!SDL_WindowSupportsGPUPresentMode(device, window, present_mode))
    {
        SDLThrow(location);
    }
}

inline void ClaimWindowForGPUDevice(SDL_GPUDevice *device, SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_ClaimWindowForGPUDevice(device, window))
    {
        SDLThrow(location);
    }
}

inline void ReleaseWindowFromGPUDevice(SDL_GPUDevice *device, SDL_Window *window)
{
    SDL_ReleaseWindowFromGPUDevice(device, window);
}

inline void SetGPUSwapchainParameters(SDL_GPUDevice *device, SDL_Window *window, SDL_GPUSwapchainComposition swapchain_composition, SDL_GPUPresentMode present_mode, std::source_location location = std::source_location::current())
{
    if (!SDL_SetGPUSwapchainParameters(device, window, swapchain_composition, present_mode))
    {
        SDLThrow(location);
    }
}

inline void SetGPUAllowedFramesInFlight(SDL_GPUDevice *device, Uint32 allowed_frames_in_flight, std::source_location location = std::source_location::current())
{
    if (!SDL_SetGPUAllowedFramesInFlight(device, allowed_frames_in_flight))
    {
        SDLThrow(location);
    }
}

inline SDL_GPUTextureFormat GetGPUSwapchainTextureFormat(SDL_GPUDevice *device, SDL_Window *window)
{
    return SDL_GetGPUSwapchainTextureFormat(device, window);
}

inline void AcquireGPUSwapchainTexture(SDL_GPUCommandBuffer *command_buffer, SDL_Window *window, SDL_GPUTexture **swapchain_texture, Uint32 *swapchain_texture_width, Uint32 *swapchain_texture_height, std::source_location location = std::source_location::current())
{
    if (!SDL_AcquireGPUSwapchainTexture(command_buffer, window, swapchain_texture, swapchain_texture_width, swapchain_texture_height))
    {
        SDLThrow(location);
    }
}

inline void WaitForGPUSwapchain(SDL_GPUDevice *device, SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_WaitForGPUSwapchain(device, window))
    {
        SDLThrow(location);
    }
}

inline void WaitAndAcquireGPUSwapchainTexture(SDL_GPUCommandBuffer *command_buffer, SDL_Window *window, SDL_GPUTexture **swapchain_texture, Uint32 *swapchain_texture_width, Uint32 *swapchain_texture_height, std::source_location location = std::source_location::current())
{
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, swapchain_texture, swapchain_texture_width, swapchain_texture_height))
    {
        SDLThrow(location);
    }
}

inline void SubmitGPUCommandBuffer(SDL_GPUCommandBuffer *command_buffer, std::source_location location = std::source_location::current())
{
    if (!SDL_SubmitGPUCommandBuffer(command_buffer))
    {
        SDLThrow(location);
    }
}

inline SDL_GPUFence *SubmitGPUCommandBufferAndAcquireFence(SDL_GPUCommandBuffer *command_buffer, std::source_location location = std::source_location::current())
{
    SDL_GPUFence *result = SDL_SubmitGPUCommandBufferAndAcquireFence(command_buffer);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void CancelGPUCommandBuffer(SDL_GPUCommandBuffer *command_buffer, std::source_location location = std::source_location::current())
{
    if (!SDL_CancelGPUCommandBuffer(command_buffer))
    {
        SDLThrow(location);
    }
}

inline void WaitForGPUIdle(SDL_GPUDevice *device, std::source_location location = std::source_location::current())
{
    if (!SDL_WaitForGPUIdle(device))
    {
        SDLThrow(location);
    }
}

inline void WaitForGPUFences(SDL_GPUDevice *device, bool wait_all, SDL_GPUFence *const *fences, Uint32 num_fences, std::source_location location = std::source_location::current())
{
    if (!SDL_WaitForGPUFences(device, wait_all, fences, num_fences))
    {
        SDLThrow(location);
    }
}

inline void QueryGPUFence(SDL_GPUDevice *device, SDL_GPUFence *fence, std::source_location location = std::source_location::current())
{
    if (!SDL_QueryGPUFence(device, fence))
    {
        SDLThrow(location);
    }
}

inline void ReleaseGPUFence(SDL_GPUDevice *device, SDL_GPUFence *fence)
{
    SDL_ReleaseGPUFence(device, fence);
}

inline Uint32 GPUTextureFormatTexelBlockSize(SDL_GPUTextureFormat format)
{
    return SDL_GPUTextureFormatTexelBlockSize(format);
}

inline void GPUTextureSupportsFormat(SDL_GPUDevice *device, SDL_GPUTextureFormat format, SDL_GPUTextureType type, SDL_GPUTextureUsageFlags usage, std::source_location location = std::source_location::current())
{
    if (!SDL_GPUTextureSupportsFormat(device, format, type, usage))
    {
        SDLThrow(location);
    }
}

inline void GPUTextureSupportsSampleCount(SDL_GPUDevice *device, SDL_GPUTextureFormat format, SDL_GPUSampleCount sample_count, std::source_location location = std::source_location::current())
{
    if (!SDL_GPUTextureSupportsSampleCount(device, format, sample_count))
    {
        SDLThrow(location);
    }
}

inline Uint32 CalculateGPUTextureFormatSize(SDL_GPUTextureFormat format, Uint32 width, Uint32 height, Uint32 depth_or_layer_count)
{
    return SDL_CalculateGPUTextureFormatSize(format, width, height, depth_or_layer_count);
}

inline SDL_HapticID *GetHaptics(int *count, std::source_location location = std::source_location::current())
{
    SDL_HapticID *result = SDL_GetHaptics(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetHapticNameForID(SDL_HapticID instance_id, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetHapticNameForID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Haptic *OpenHaptic(SDL_HapticID instance_id, std::source_location location = std::source_location::current())
{
    SDL_Haptic *result = SDL_OpenHaptic(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Haptic *GetHapticFromID(SDL_HapticID instance_id, std::source_location location = std::source_location::current())
{
    SDL_Haptic *result = SDL_GetHapticFromID(instance_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_HapticID GetHapticID(SDL_Haptic *haptic)
{
    return SDL_GetHapticID(haptic);
}

inline const char *GetHapticName(SDL_Haptic *haptic, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetHapticName(haptic);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void IsMouseHaptic(std::source_location location = std::source_location::current())
{
    if (!SDL_IsMouseHaptic())
    {
        SDLThrow(location);
    }
}

inline SDL_Haptic *OpenHapticFromMouse(std::source_location location = std::source_location::current())
{
    SDL_Haptic *result = SDL_OpenHapticFromMouse();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void IsJoystickHaptic(SDL_Joystick *joystick, std::source_location location = std::source_location::current())
{
    if (!SDL_IsJoystickHaptic(joystick))
    {
        SDLThrow(location);
    }
}

inline SDL_Haptic *OpenHapticFromJoystick(SDL_Joystick *joystick, std::source_location location = std::source_location::current())
{
    SDL_Haptic *result = SDL_OpenHapticFromJoystick(joystick);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void CloseHaptic(SDL_Haptic *haptic)
{
    SDL_CloseHaptic(haptic);
}

inline int GetMaxHapticEffects(SDL_Haptic *haptic)
{
    return SDL_GetMaxHapticEffects(haptic);
}

inline int GetMaxHapticEffectsPlaying(SDL_Haptic *haptic)
{
    return SDL_GetMaxHapticEffectsPlaying(haptic);
}

inline Uint32 GetHapticFeatures(SDL_Haptic *haptic)
{
    return SDL_GetHapticFeatures(haptic);
}

inline int GetNumHapticAxes(SDL_Haptic *haptic)
{
    return SDL_GetNumHapticAxes(haptic);
}

inline void HapticEffectSupported(SDL_Haptic *haptic, const SDL_HapticEffect *effect, std::source_location location = std::source_location::current())
{
    if (!SDL_HapticEffectSupported(haptic, effect))
    {
        SDLThrow(location);
    }
}

inline int CreateHapticEffect(SDL_Haptic *haptic, const SDL_HapticEffect *effect)
{
    return SDL_CreateHapticEffect(haptic, effect);
}

inline void UpdateHapticEffect(SDL_Haptic *haptic, int effect, const SDL_HapticEffect *data, std::source_location location = std::source_location::current())
{
    if (!SDL_UpdateHapticEffect(haptic, effect, data))
    {
        SDLThrow(location);
    }
}

inline void RunHapticEffect(SDL_Haptic *haptic, int effect, Uint32 iterations, std::source_location location = std::source_location::current())
{
    if (!SDL_RunHapticEffect(haptic, effect, iterations))
    {
        SDLThrow(location);
    }
}

inline void StopHapticEffect(SDL_Haptic *haptic, int effect, std::source_location location = std::source_location::current())
{
    if (!SDL_StopHapticEffect(haptic, effect))
    {
        SDLThrow(location);
    }
}

inline void DestroyHapticEffect(SDL_Haptic *haptic, int effect)
{
    SDL_DestroyHapticEffect(haptic, effect);
}

inline void GetHapticEffectStatus(SDL_Haptic *haptic, int effect, std::source_location location = std::source_location::current())
{
    if (!SDL_GetHapticEffectStatus(haptic, effect))
    {
        SDLThrow(location);
    }
}

inline void SetHapticGain(SDL_Haptic *haptic, int gain, std::source_location location = std::source_location::current())
{
    if (!SDL_SetHapticGain(haptic, gain))
    {
        SDLThrow(location);
    }
}

inline void SetHapticAutocenter(SDL_Haptic *haptic, int autocenter, std::source_location location = std::source_location::current())
{
    if (!SDL_SetHapticAutocenter(haptic, autocenter))
    {
        SDLThrow(location);
    }
}

inline void PauseHaptic(SDL_Haptic *haptic, std::source_location location = std::source_location::current())
{
    if (!SDL_PauseHaptic(haptic))
    {
        SDLThrow(location);
    }
}

inline void ResumeHaptic(SDL_Haptic *haptic, std::source_location location = std::source_location::current())
{
    if (!SDL_ResumeHaptic(haptic))
    {
        SDLThrow(location);
    }
}

inline void StopHapticEffects(SDL_Haptic *haptic, std::source_location location = std::source_location::current())
{
    if (!SDL_StopHapticEffects(haptic))
    {
        SDLThrow(location);
    }
}

inline void HapticRumbleSupported(SDL_Haptic *haptic, std::source_location location = std::source_location::current())
{
    if (!SDL_HapticRumbleSupported(haptic))
    {
        SDLThrow(location);
    }
}

inline void InitHapticRumble(SDL_Haptic *haptic, std::source_location location = std::source_location::current())
{
    if (!SDL_InitHapticRumble(haptic))
    {
        SDLThrow(location);
    }
}

inline void PlayHapticRumble(SDL_Haptic *haptic, float strength, Uint32 length, std::source_location location = std::source_location::current())
{
    if (!SDL_PlayHapticRumble(haptic, strength, length))
    {
        SDLThrow(location);
    }
}

inline void StopHapticRumble(SDL_Haptic *haptic, std::source_location location = std::source_location::current())
{
    if (!SDL_StopHapticRumble(haptic))
    {
        SDLThrow(location);
    }
}

inline int hid_init()
{
    return SDL_hid_init();
}

inline int hid_exit()
{
    return SDL_hid_exit();
}

inline Uint32 hid_device_change_count()
{
    return SDL_hid_device_change_count();
}

inline SDL_hid_device_info *hid_enumerate(unsigned short vendor_id, unsigned short product_id, std::source_location location = std::source_location::current())
{
    SDL_hid_device_info *result = SDL_hid_enumerate(vendor_id, product_id);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void hid_free_enumeration(SDL_hid_device_info *devs)
{
    SDL_hid_free_enumeration(devs);
}

inline SDL_hid_device *hid_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number, std::source_location location = std::source_location::current())
{
    SDL_hid_device *result = SDL_hid_open(vendor_id, product_id, serial_number);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_hid_device *hid_open_path(const char *path, std::source_location location = std::source_location::current())
{
    SDL_hid_device *result = SDL_hid_open_path(path);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int hid_write(SDL_hid_device *dev, const unsigned char *data, size_t length)
{
    return SDL_hid_write(dev, data, length);
}

inline int hid_read_timeout(SDL_hid_device *dev, unsigned char *data, size_t length, int milliseconds)
{
    return SDL_hid_read_timeout(dev, data, length, milliseconds);
}

inline int hid_read(SDL_hid_device *dev, unsigned char *data, size_t length)
{
    return SDL_hid_read(dev, data, length);
}

inline int hid_set_nonblocking(SDL_hid_device *dev, int nonblock)
{
    return SDL_hid_set_nonblocking(dev, nonblock);
}

inline int hid_send_feature_report(SDL_hid_device *dev, const unsigned char *data, size_t length)
{
    return SDL_hid_send_feature_report(dev, data, length);
}

inline int hid_get_feature_report(SDL_hid_device *dev, unsigned char *data, size_t length)
{
    return SDL_hid_get_feature_report(dev, data, length);
}

inline int hid_get_input_report(SDL_hid_device *dev, unsigned char *data, size_t length)
{
    return SDL_hid_get_input_report(dev, data, length);
}

inline int hid_close(SDL_hid_device *dev)
{
    return SDL_hid_close(dev);
}

inline int hid_get_manufacturer_string(SDL_hid_device *dev, wchar_t *string, size_t maxlen)
{
    return SDL_hid_get_manufacturer_string(dev, string, maxlen);
}

inline int hid_get_product_string(SDL_hid_device *dev, wchar_t *string, size_t maxlen)
{
    return SDL_hid_get_product_string(dev, string, maxlen);
}

inline int hid_get_serial_number_string(SDL_hid_device *dev, wchar_t *string, size_t maxlen)
{
    return SDL_hid_get_serial_number_string(dev, string, maxlen);
}

inline int hid_get_indexed_string(SDL_hid_device *dev, int string_index, wchar_t *string, size_t maxlen)
{
    return SDL_hid_get_indexed_string(dev, string_index, string, maxlen);
}

inline SDL_hid_device_info *hid_get_device_info(SDL_hid_device *dev, std::source_location location = std::source_location::current())
{
    SDL_hid_device_info *result = SDL_hid_get_device_info(dev);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline int hid_get_report_descriptor(SDL_hid_device *dev, unsigned char *buf, size_t buf_size)
{
    return SDL_hid_get_report_descriptor(dev, buf, buf_size);
}

inline void hid_ble_scan(bool active)
{
    SDL_hid_ble_scan(active);
}

inline void SetHintWithPriority(const char *name, const char *value, SDL_HintPriority priority, std::source_location location = std::source_location::current())
{
    if (!SDL_SetHintWithPriority(name, value, priority))
    {
        SDLThrow(location);
    }
}

inline void SetHint(const char *name, const char *value, std::source_location location = std::source_location::current())
{
    if (!SDL_SetHint(name, value))
    {
        SDLThrow(location);
    }
}

inline void ResetHint(const char *name, std::source_location location = std::source_location::current())
{
    if (!SDL_ResetHint(name))
    {
        SDLThrow(location);
    }
}

inline void ResetHints()
{
    SDL_ResetHints();
}

inline const char *GetHint(const char *name, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetHint(name);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void GetHintBoolean(const char *name, bool default_value, std::source_location location = std::source_location::current())
{
    if (!SDL_GetHintBoolean(name, default_value))
    {
        SDLThrow(location);
    }
}

inline void AddHintCallback(const char *name, SDL_HintCallback callback, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_AddHintCallback(name, callback, userdata))
    {
        SDLThrow(location);
    }
}

inline void RemoveHintCallback(const char *name, SDL_HintCallback callback, void *userdata)
{
    SDL_RemoveHintCallback(name, callback, userdata);
}

inline void Init(SDL_InitFlags flags, std::source_location location = std::source_location::current())
{
    if (!SDL_Init(flags))
    {
        SDLThrow(location);
    }
}

inline void InitSubSystem(SDL_InitFlags flags, std::source_location location = std::source_location::current())
{
    if (!SDL_InitSubSystem(flags))
    {
        SDLThrow(location);
    }
}

inline void QuitSubSystem(SDL_InitFlags flags)
{
    SDL_QuitSubSystem(flags);
}

inline SDL_InitFlags WasInit(SDL_InitFlags flags)
{
    return SDL_WasInit(flags);
}

inline void Quit()
{
    SDL_Quit();
}

inline void IsMainThread(std::source_location location = std::source_location::current())
{
    if (!SDL_IsMainThread())
    {
        SDLThrow(location);
    }
}

inline void RunOnMainThread(SDL_MainThreadCallback callback, void *userdata, bool wait_complete, std::source_location location = std::source_location::current())
{
    if (!SDL_RunOnMainThread(callback, userdata, wait_complete))
    {
        SDLThrow(location);
    }
}

inline void SetAppMetadata(const char *appname, const char *appversion, const char *appidentifier, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAppMetadata(appname, appversion, appidentifier))
    {
        SDLThrow(location);
    }
}

inline void SetAppMetadataProperty(const char *name, const char *value, std::source_location location = std::source_location::current())
{
    if (!SDL_SetAppMetadataProperty(name, value))
    {
        SDLThrow(location);
    }
}

inline const char *GetAppMetadataProperty(const char *name, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetAppMetadataProperty(name);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_SharedObject *LoadObject(const char *sofile, std::source_location location = std::source_location::current())
{
    SDL_SharedObject *result = SDL_LoadObject(sofile);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_FunctionPointer LoadFunction(SDL_SharedObject *handle, const char *name)
{
    return SDL_LoadFunction(handle, name);
}

inline void UnloadObject(SDL_SharedObject *handle)
{
    SDL_UnloadObject(handle);
}

inline SDL_Locale **GetPreferredLocales(int *count, std::source_location location = std::source_location::current())
{
    SDL_Locale **result = SDL_GetPreferredLocales(count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetLogPriorities(SDL_LogPriority priority)
{
    SDL_SetLogPriorities(priority);
}

inline void SetLogPriority(int category, SDL_LogPriority priority)
{
    SDL_SetLogPriority(category, priority);
}

inline SDL_LogPriority GetLogPriority(int category)
{
    return SDL_GetLogPriority(category);
}

inline void ResetLogPriorities()
{
    SDL_ResetLogPriorities();
}

inline void SetLogPriorityPrefix(SDL_LogPriority priority, const char *prefix, std::source_location location = std::source_location::current())
{
    if (!SDL_SetLogPriorityPrefix(priority, prefix))
    {
        SDLThrow(location);
    }
}

inline void Log(const char *fmt)
{
    SDL_Log(fmt);
}

inline void LogTrace(int category, const char *fmt)
{
    SDL_LogTrace(category, fmt);
}

inline void LogVerbose(int category, const char *fmt)
{
    SDL_LogVerbose(category, fmt);
}

inline void LogDebug(int category, const char *fmt)
{
    SDL_LogDebug(category, fmt);
}

inline void LogInfo(int category, const char *fmt)
{
    SDL_LogInfo(category, fmt);
}

inline void LogWarn(int category, const char *fmt)
{
    SDL_LogWarn(category, fmt);
}

inline void LogError(int category, const char *fmt)
{
    SDL_LogError(category, fmt);
}

inline void LogCritical(int category, const char *fmt)
{
    SDL_LogCritical(category, fmt);
}

inline void LogMessage(int category, SDL_LogPriority priority, const char *fmt)
{
    SDL_LogMessage(category, priority, fmt);
}

inline void LogMessageV(int category, SDL_LogPriority priority, const char *fmt, va_list ap)
{
    SDL_LogMessageV(category, priority, fmt, ap);
}

inline SDL_LogOutputFunction GetDefaultLogOutputFunction()
{
    return SDL_GetDefaultLogOutputFunction();
}

inline void GetLogOutputFunction(SDL_LogOutputFunction *callback, void **userdata)
{
    SDL_GetLogOutputFunction(callback, userdata);
}

inline void SetLogOutputFunction(SDL_LogOutputFunction callback, void *userdata)
{
    SDL_SetLogOutputFunction(callback, userdata);
}

inline void ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid, std::source_location location = std::source_location::current())
{
    if (!SDL_ShowMessageBox(messageboxdata, buttonid))
    {
        SDLThrow(location);
    }
}

inline void ShowSimpleMessageBox(SDL_MessageBoxFlags flags, const char *title, const char *message, SDL_Window *window, std::source_location location = std::source_location::current())
{
    if (!SDL_ShowSimpleMessageBox(flags, title, message, window))
    {
        SDLThrow(location);
    }
}

inline SDL_MetalView Metal_CreateView(SDL_Window *window)
{
    return SDL_Metal_CreateView(window);
}

inline void Metal_DestroyView(SDL_MetalView view)
{
    SDL_Metal_DestroyView(view);
}

inline void *Metal_GetLayer(SDL_MetalView view, std::source_location location = std::source_location::current())
{
    void *result = SDL_Metal_GetLayer(view);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void OpenURL(const char *url, std::source_location location = std::source_location::current())
{
    if (!SDL_OpenURL(url))
    {
        SDLThrow(location);
    }
}

inline const char *GetPlatform(std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetPlatform();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Process *CreateProcess(const char *const *args, bool pipe_stdio, std::source_location location = std::source_location::current())
{
    SDL_Process *result = SDL_CreateProcess(args, pipe_stdio);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Process *CreateProcessWithProperties(SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    SDL_Process *result = SDL_CreateProcessWithProperties(props);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_PropertiesID GetProcessProperties(SDL_Process *process)
{
    return SDL_GetProcessProperties(process);
}

inline void *ReadProcess(SDL_Process *process, size_t *datasize, int *exitcode, std::source_location location = std::source_location::current())
{
    void *result = SDL_ReadProcess(process, datasize, exitcode);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_IOStream *GetProcessInput(SDL_Process *process, std::source_location location = std::source_location::current())
{
    SDL_IOStream *result = SDL_GetProcessInput(process);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_IOStream *GetProcessOutput(SDL_Process *process, std::source_location location = std::source_location::current())
{
    SDL_IOStream *result = SDL_GetProcessOutput(process);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void KillProcess(SDL_Process *process, bool force, std::source_location location = std::source_location::current())
{
    if (!SDL_KillProcess(process, force))
    {
        SDLThrow(location);
    }
}

inline void WaitProcess(SDL_Process *process, bool block, int *exitcode, std::source_location location = std::source_location::current())
{
    if (!SDL_WaitProcess(process, block, exitcode))
    {
        SDLThrow(location);
    }
}

inline void DestroyProcess(SDL_Process *process)
{
    SDL_DestroyProcess(process);
}

inline int GetNumRenderDrivers()
{
    return SDL_GetNumRenderDrivers();
}

inline const char *GetRenderDriver(int index, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetRenderDriver(index);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void CreateWindowAndRenderer(const char *title, int width, int height, SDL_WindowFlags window_flags, SDL_Window **window, SDL_Renderer **renderer, std::source_location location = std::source_location::current())
{
    if (!SDL_CreateWindowAndRenderer(title, width, height, window_flags, window, renderer))
    {
        SDLThrow(location);
    }
}

inline SDL_Renderer *CreateRenderer(SDL_Window *window, const char *name, std::source_location location = std::source_location::current())
{
    SDL_Renderer *result = SDL_CreateRenderer(window, name);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Renderer *CreateRendererWithProperties(SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    SDL_Renderer *result = SDL_CreateRendererWithProperties(props);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Renderer *CreateSoftwareRenderer(SDL_Surface *surface, std::source_location location = std::source_location::current())
{
    SDL_Renderer *result = SDL_CreateSoftwareRenderer(surface);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Renderer *GetRenderer(SDL_Window *window, std::source_location location = std::source_location::current())
{
    SDL_Renderer *result = SDL_GetRenderer(window);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Window *GetRenderWindow(SDL_Renderer *renderer, std::source_location location = std::source_location::current())
{
    SDL_Window *result = SDL_GetRenderWindow(renderer);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const char *GetRendererName(SDL_Renderer *renderer, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetRendererName(renderer);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_PropertiesID GetRendererProperties(SDL_Renderer *renderer)
{
    return SDL_GetRendererProperties(renderer);
}

inline void GetRenderOutputSize(SDL_Renderer *renderer, int *w, int *h, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderOutputSize(renderer, w, h))
    {
        SDLThrow(location);
    }
}

inline void GetCurrentRenderOutputSize(SDL_Renderer *renderer, int *w, int *h, std::source_location location = std::source_location::current())
{
    if (!SDL_GetCurrentRenderOutputSize(renderer, w, h))
    {
        SDLThrow(location);
    }
}

inline SDL_Texture *CreateTexture(SDL_Renderer *renderer, SDL_PixelFormat format, SDL_TextureAccess access, int w, int h, std::source_location location = std::source_location::current())
{
    SDL_Texture *result = SDL_CreateTexture(renderer, format, access, w, h);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Texture *CreateTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface, std::source_location location = std::source_location::current())
{
    SDL_Texture *result = SDL_CreateTextureFromSurface(renderer, surface);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Texture *CreateTextureWithProperties(SDL_Renderer *renderer, SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    SDL_Texture *result = SDL_CreateTextureWithProperties(renderer, props);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_PropertiesID GetTextureProperties(SDL_Texture *texture)
{
    return SDL_GetTextureProperties(texture);
}

inline SDL_Renderer *GetRendererFromTexture(SDL_Texture *texture, std::source_location location = std::source_location::current())
{
    SDL_Renderer *result = SDL_GetRendererFromTexture(texture);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void GetTextureSize(SDL_Texture *texture, float *w, float *h, std::source_location location = std::source_location::current())
{
    if (!SDL_GetTextureSize(texture, w, h))
    {
        SDLThrow(location);
    }
}

inline void SetTextureColorMod(SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b, std::source_location location = std::source_location::current())
{
    if (!SDL_SetTextureColorMod(texture, r, g, b))
    {
        SDLThrow(location);
    }
}

inline void SetTextureColorModFloat(SDL_Texture *texture, float r, float g, float b, std::source_location location = std::source_location::current())
{
    if (!SDL_SetTextureColorModFloat(texture, r, g, b))
    {
        SDLThrow(location);
    }
}

inline void GetTextureColorMod(SDL_Texture *texture, Uint8 *r, Uint8 *g, Uint8 *b, std::source_location location = std::source_location::current())
{
    if (!SDL_GetTextureColorMod(texture, r, g, b))
    {
        SDLThrow(location);
    }
}

inline void GetTextureColorModFloat(SDL_Texture *texture, float *r, float *g, float *b, std::source_location location = std::source_location::current())
{
    if (!SDL_GetTextureColorModFloat(texture, r, g, b))
    {
        SDLThrow(location);
    }
}

inline void SetTextureAlphaMod(SDL_Texture *texture, Uint8 alpha, std::source_location location = std::source_location::current())
{
    if (!SDL_SetTextureAlphaMod(texture, alpha))
    {
        SDLThrow(location);
    }
}

inline void SetTextureAlphaModFloat(SDL_Texture *texture, float alpha, std::source_location location = std::source_location::current())
{
    if (!SDL_SetTextureAlphaModFloat(texture, alpha))
    {
        SDLThrow(location);
    }
}

inline void GetTextureAlphaMod(SDL_Texture *texture, Uint8 *alpha, std::source_location location = std::source_location::current())
{
    if (!SDL_GetTextureAlphaMod(texture, alpha))
    {
        SDLThrow(location);
    }
}

inline void GetTextureAlphaModFloat(SDL_Texture *texture, float *alpha, std::source_location location = std::source_location::current())
{
    if (!SDL_GetTextureAlphaModFloat(texture, alpha))
    {
        SDLThrow(location);
    }
}

inline void SetTextureBlendMode(SDL_Texture *texture, SDL_BlendMode blendMode, std::source_location location = std::source_location::current())
{
    if (!SDL_SetTextureBlendMode(texture, blendMode))
    {
        SDLThrow(location);
    }
}

inline void GetTextureBlendMode(SDL_Texture *texture, SDL_BlendMode *blendMode, std::source_location location = std::source_location::current())
{
    if (!SDL_GetTextureBlendMode(texture, blendMode))
    {
        SDLThrow(location);
    }
}

inline void SetTextureScaleMode(SDL_Texture *texture, SDL_ScaleMode scaleMode, std::source_location location = std::source_location::current())
{
    if (!SDL_SetTextureScaleMode(texture, scaleMode))
    {
        SDLThrow(location);
    }
}

inline void GetTextureScaleMode(SDL_Texture *texture, SDL_ScaleMode *scaleMode, std::source_location location = std::source_location::current())
{
    if (!SDL_GetTextureScaleMode(texture, scaleMode))
    {
        SDLThrow(location);
    }
}

inline void UpdateTexture(SDL_Texture *texture, const SDL_Rect *rect, const void *pixels, int pitch, std::source_location location = std::source_location::current())
{
    if (!SDL_UpdateTexture(texture, rect, pixels, pitch))
    {
        SDLThrow(location);
    }
}

inline void UpdateYUVTexture(SDL_Texture *texture, const SDL_Rect *rect, const Uint8 *Yplane, int Ypitch, const Uint8 *Uplane, int Upitch, const Uint8 *Vplane, int Vpitch, std::source_location location = std::source_location::current())
{
    if (!SDL_UpdateYUVTexture(texture, rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch))
    {
        SDLThrow(location);
    }
}

inline void UpdateNVTexture(SDL_Texture *texture, const SDL_Rect *rect, const Uint8 *Yplane, int Ypitch, const Uint8 *UVplane, int UVpitch, std::source_location location = std::source_location::current())
{
    if (!SDL_UpdateNVTexture(texture, rect, Yplane, Ypitch, UVplane, UVpitch))
    {
        SDLThrow(location);
    }
}

inline void LockTexture(SDL_Texture *texture, const SDL_Rect *rect, void **pixels, int *pitch, std::source_location location = std::source_location::current())
{
    if (!SDL_LockTexture(texture, rect, pixels, pitch))
    {
        SDLThrow(location);
    }
}

inline void LockTextureToSurface(SDL_Texture *texture, const SDL_Rect *rect, SDL_Surface **surface, std::source_location location = std::source_location::current())
{
    if (!SDL_LockTextureToSurface(texture, rect, surface))
    {
        SDLThrow(location);
    }
}

inline void UnlockTexture(SDL_Texture *texture)
{
    SDL_UnlockTexture(texture);
}

inline void SetRenderTarget(SDL_Renderer *renderer, SDL_Texture *texture, std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderTarget(renderer, texture))
    {
        SDLThrow(location);
    }
}

inline SDL_Texture *GetRenderTarget(SDL_Renderer *renderer, std::source_location location = std::source_location::current())
{
    SDL_Texture *result = SDL_GetRenderTarget(renderer);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetRenderLogicalPresentation(SDL_Renderer *renderer, int w, int h, SDL_RendererLogicalPresentation mode, std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderLogicalPresentation(renderer, w, h, mode))
    {
        SDLThrow(location);
    }
}

inline void GetRenderLogicalPresentation(SDL_Renderer *renderer, int *w, int *h, SDL_RendererLogicalPresentation *mode, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderLogicalPresentation(renderer, w, h, mode))
    {
        SDLThrow(location);
    }
}

inline void GetRenderLogicalPresentationRect(SDL_Renderer *renderer, SDL_FRect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderLogicalPresentationRect(renderer, rect))
    {
        SDLThrow(location);
    }
}

inline void RenderCoordinatesFromWindow(SDL_Renderer *renderer, float window_x, float window_y, float *x, float *y, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderCoordinatesFromWindow(renderer, window_x, window_y, x, y))
    {
        SDLThrow(location);
    }
}

inline void RenderCoordinatesToWindow(SDL_Renderer *renderer, float x, float y, float *window_x, float *window_y, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderCoordinatesToWindow(renderer, x, y, window_x, window_y))
    {
        SDLThrow(location);
    }
}

inline void ConvertEventToRenderCoordinates(SDL_Renderer *renderer, SDL_Event *event, std::source_location location = std::source_location::current())
{
    if (!SDL_ConvertEventToRenderCoordinates(renderer, event))
    {
        SDLThrow(location);
    }
}

inline void SetRenderViewport(SDL_Renderer *renderer, const SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderViewport(renderer, rect))
    {
        SDLThrow(location);
    }
}

inline void GetRenderViewport(SDL_Renderer *renderer, SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderViewport(renderer, rect))
    {
        SDLThrow(location);
    }
}

inline void RenderViewportSet(SDL_Renderer *renderer, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderViewportSet(renderer))
    {
        SDLThrow(location);
    }
}

inline void GetRenderSafeArea(SDL_Renderer *renderer, SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderSafeArea(renderer, rect))
    {
        SDLThrow(location);
    }
}

inline void SetRenderClipRect(SDL_Renderer *renderer, const SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderClipRect(renderer, rect))
    {
        SDLThrow(location);
    }
}

inline void GetRenderClipRect(SDL_Renderer *renderer, SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderClipRect(renderer, rect))
    {
        SDLThrow(location);
    }
}

inline void RenderClipEnabled(SDL_Renderer *renderer, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderClipEnabled(renderer))
    {
        SDLThrow(location);
    }
}

inline void SetRenderScale(SDL_Renderer *renderer, float scaleX, float scaleY, std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderScale(renderer, scaleX, scaleY))
    {
        SDLThrow(location);
    }
}

inline void GetRenderScale(SDL_Renderer *renderer, float *scaleX, float *scaleY, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderScale(renderer, scaleX, scaleY))
    {
        SDLThrow(location);
    }
}

inline void SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a, std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderDrawColor(renderer, r, g, b, a))
    {
        SDLThrow(location);
    }
}

inline void SetRenderDrawColorFloat(SDL_Renderer *renderer, float r, float g, float b, float a, std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderDrawColorFloat(renderer, r, g, b, a))
    {
        SDLThrow(location);
    }
}

inline void GetRenderDrawColor(SDL_Renderer *renderer, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderDrawColor(renderer, r, g, b, a))
    {
        SDLThrow(location);
    }
}

inline void GetRenderDrawColorFloat(SDL_Renderer *renderer, float *r, float *g, float *b, float *a, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderDrawColorFloat(renderer, r, g, b, a))
    {
        SDLThrow(location);
    }
}

inline void SetRenderColorScale(SDL_Renderer *renderer, float scale, std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderColorScale(renderer, scale))
    {
        SDLThrow(location);
    }
}

inline void GetRenderColorScale(SDL_Renderer *renderer, float *scale, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderColorScale(renderer, scale))
    {
        SDLThrow(location);
    }
}

inline void SetRenderDrawBlendMode(SDL_Renderer *renderer, SDL_BlendMode blendMode, std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderDrawBlendMode(renderer, blendMode))
    {
        SDLThrow(location);
    }
}

inline void GetRenderDrawBlendMode(SDL_Renderer *renderer, SDL_BlendMode *blendMode, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderDrawBlendMode(renderer, blendMode))
    {
        SDLThrow(location);
    }
}

inline void RenderClear(SDL_Renderer *renderer, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderClear(renderer))
    {
        SDLThrow(location);
    }
}

inline void RenderPoint(SDL_Renderer *renderer, float x, float y, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderPoint(renderer, x, y))
    {
        SDLThrow(location);
    }
}

inline void RenderPoints(SDL_Renderer *renderer, const SDL_FPoint *points, int count, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderPoints(renderer, points, count))
    {
        SDLThrow(location);
    }
}

inline void RenderLine(SDL_Renderer *renderer, float x1, float y1, float x2, float y2, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderLine(renderer, x1, y1, x2, y2))
    {
        SDLThrow(location);
    }
}

inline void RenderLines(SDL_Renderer *renderer, const SDL_FPoint *points, int count, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderLines(renderer, points, count))
    {
        SDLThrow(location);
    }
}

inline void RenderRect(SDL_Renderer *renderer, const SDL_FRect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderRect(renderer, rect))
    {
        SDLThrow(location);
    }
}

inline void RenderRects(SDL_Renderer *renderer, const SDL_FRect *rects, int count, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderRects(renderer, rects, count))
    {
        SDLThrow(location);
    }
}

inline void RenderFillRect(SDL_Renderer *renderer, const SDL_FRect *rect, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderFillRect(renderer, rect))
    {
        SDLThrow(location);
    }
}

inline void RenderFillRects(SDL_Renderer *renderer, const SDL_FRect *rects, int count, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderFillRects(renderer, rects, count))
    {
        SDLThrow(location);
    }
}

inline void RenderTexture(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect *srcrect, const SDL_FRect *dstrect, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderTexture(renderer, texture, srcrect, dstrect))
    {
        SDLThrow(location);
    }
}

inline void RenderTextureRotated(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect *srcrect, const SDL_FRect *dstrect, double angle, const SDL_FPoint *center, SDL_FlipMode flip, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderTextureRotated(renderer, texture, srcrect, dstrect, angle, center, flip))
    {
        SDLThrow(location);
    }
}

inline void RenderTextureAffine(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect *srcrect, const SDL_FPoint *origin, const SDL_FPoint *right, const SDL_FPoint *down, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderTextureAffine(renderer, texture, srcrect, origin, right, down))
    {
        SDLThrow(location);
    }
}

inline void RenderTextureTiled(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect *srcrect, float scale, const SDL_FRect *dstrect, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderTextureTiled(renderer, texture, srcrect, scale, dstrect))
    {
        SDLThrow(location);
    }
}

inline void RenderTexture9Grid(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_FRect *srcrect, float left_width, float right_width, float top_height, float bottom_height, float scale, const SDL_FRect *dstrect, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderTexture9Grid(renderer, texture, srcrect, left_width, right_width, top_height, bottom_height, scale, dstrect))
    {
        SDLThrow(location);
    }
}

inline void RenderGeometry(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Vertex *vertices, int num_vertices, const int *indices, int num_indices, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderGeometry(renderer, texture, vertices, num_vertices, indices, num_indices))
    {
        SDLThrow(location);
    }
}

inline void RenderGeometryRaw(SDL_Renderer *renderer, SDL_Texture *texture, const float *xy, int xy_stride, const SDL_FColor *color, int color_stride, const float *uv, int uv_stride, int num_vertices, const void *indices, int num_indices, int size_indices, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderGeometryRaw(renderer, texture, xy, xy_stride, color, color_stride, uv, uv_stride, num_vertices, indices, num_indices, size_indices))
    {
        SDLThrow(location);
    }
}

inline SDL_Surface *RenderReadPixels(SDL_Renderer *renderer, const SDL_Rect *rect, std::source_location location = std::source_location::current())
{
    SDL_Surface *result = SDL_RenderReadPixels(renderer, rect);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void RenderPresent(SDL_Renderer *renderer, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderPresent(renderer))
    {
        SDLThrow(location);
    }
}

inline void DestroyTexture(SDL_Texture *texture)
{
    SDL_DestroyTexture(texture);
}

inline void DestroyRenderer(SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
}

inline void FlushRenderer(SDL_Renderer *renderer, std::source_location location = std::source_location::current())
{
    if (!SDL_FlushRenderer(renderer))
    {
        SDLThrow(location);
    }
}

inline void *GetRenderMetalLayer(SDL_Renderer *renderer, std::source_location location = std::source_location::current())
{
    void *result = SDL_GetRenderMetalLayer(renderer);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void *GetRenderMetalCommandEncoder(SDL_Renderer *renderer, std::source_location location = std::source_location::current())
{
    void *result = SDL_GetRenderMetalCommandEncoder(renderer);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void AddVulkanRenderSemaphores(SDL_Renderer *renderer, Uint32 wait_stage_mask, Sint64 wait_semaphore, Sint64 signal_semaphore, std::source_location location = std::source_location::current())
{
    if (!SDL_AddVulkanRenderSemaphores(renderer, wait_stage_mask, wait_semaphore, signal_semaphore))
    {
        SDLThrow(location);
    }
}

inline void SetRenderVSync(SDL_Renderer *renderer, int vsync, std::source_location location = std::source_location::current())
{
    if (!SDL_SetRenderVSync(renderer, vsync))
    {
        SDLThrow(location);
    }
}

inline void GetRenderVSync(SDL_Renderer *renderer, int *vsync, std::source_location location = std::source_location::current())
{
    if (!SDL_GetRenderVSync(renderer, vsync))
    {
        SDLThrow(location);
    }
}

inline void RenderDebugText(SDL_Renderer *renderer, float x, float y, const char *str, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderDebugText(renderer, x, y, str))
    {
        SDLThrow(location);
    }
}

inline void RenderDebugTextFormat(SDL_Renderer *renderer, float x, float y, const char *fmt, std::source_location location = std::source_location::current())
{
    if (!SDL_RenderDebugTextFormat(renderer, x, y, fmt))
    {
        SDLThrow(location);
    }
}

inline SDL_Storage *OpenTitleStorage(const char *override, SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    SDL_Storage *result = SDL_OpenTitleStorage(override, props);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Storage *OpenUserStorage(const char *org, const char *app, SDL_PropertiesID props, std::source_location location = std::source_location::current())
{
    SDL_Storage *result = SDL_OpenUserStorage(org, app, props);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Storage *OpenFileStorage(const char *path, std::source_location location = std::source_location::current())
{
    SDL_Storage *result = SDL_OpenFileStorage(path);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Storage *OpenStorage(const SDL_StorageInterface *iface, void *userdata, std::source_location location = std::source_location::current())
{
    SDL_Storage *result = SDL_OpenStorage(iface, userdata);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void CloseStorage(SDL_Storage *storage, std::source_location location = std::source_location::current())
{
    if (!SDL_CloseStorage(storage))
    {
        SDLThrow(location);
    }
}

inline void StorageReady(SDL_Storage *storage, std::source_location location = std::source_location::current())
{
    if (!SDL_StorageReady(storage))
    {
        SDLThrow(location);
    }
}

inline void GetStorageFileSize(SDL_Storage *storage, const char *path, Uint64 *length, std::source_location location = std::source_location::current())
{
    if (!SDL_GetStorageFileSize(storage, path, length))
    {
        SDLThrow(location);
    }
}

inline void ReadStorageFile(SDL_Storage *storage, const char *path, void *destination, Uint64 length, std::source_location location = std::source_location::current())
{
    if (!SDL_ReadStorageFile(storage, path, destination, length))
    {
        SDLThrow(location);
    }
}

inline void WriteStorageFile(SDL_Storage *storage, const char *path, const void *source, Uint64 length, std::source_location location = std::source_location::current())
{
    if (!SDL_WriteStorageFile(storage, path, source, length))
    {
        SDLThrow(location);
    }
}

inline void CreateStorageDirectory(SDL_Storage *storage, const char *path, std::source_location location = std::source_location::current())
{
    if (!SDL_CreateStorageDirectory(storage, path))
    {
        SDLThrow(location);
    }
}

inline void EnumerateStorageDirectory(SDL_Storage *storage, const char *path, SDL_EnumerateDirectoryCallback callback, void *userdata, std::source_location location = std::source_location::current())
{
    if (!SDL_EnumerateStorageDirectory(storage, path, callback, userdata))
    {
        SDLThrow(location);
    }
}

inline void RemoveStoragePath(SDL_Storage *storage, const char *path, std::source_location location = std::source_location::current())
{
    if (!SDL_RemoveStoragePath(storage, path))
    {
        SDLThrow(location);
    }
}

inline void RenameStoragePath(SDL_Storage *storage, const char *oldpath, const char *newpath, std::source_location location = std::source_location::current())
{
    if (!SDL_RenameStoragePath(storage, oldpath, newpath))
    {
        SDLThrow(location);
    }
}

inline void CopyStorageFile(SDL_Storage *storage, const char *oldpath, const char *newpath, std::source_location location = std::source_location::current())
{
    if (!SDL_CopyStorageFile(storage, oldpath, newpath))
    {
        SDLThrow(location);
    }
}

inline void GetStoragePathInfo(SDL_Storage *storage, const char *path, SDL_PathInfo *info, std::source_location location = std::source_location::current())
{
    if (!SDL_GetStoragePathInfo(storage, path, info))
    {
        SDLThrow(location);
    }
}

inline Uint64 GetStorageSpaceRemaining(SDL_Storage *storage)
{
    return SDL_GetStorageSpaceRemaining(storage);
}

inline char **GlobStorageDirectory(SDL_Storage *storage, const char *path, const char *pattern, SDL_GlobFlags flags, int *count, std::source_location location = std::source_location::current())
{
    char **result = SDL_GlobStorageDirectory(storage, path, pattern, flags, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetWindowsMessageHook(SDL_WindowsMessageHook callback, void *userdata)
{
    SDL_SetWindowsMessageHook(callback, userdata);
}

inline int GetDirect3D9AdapterIndex(SDL_DisplayID displayID)
{
    return SDL_GetDirect3D9AdapterIndex(displayID);
}

inline void GetDXGIOutputInfo(SDL_DisplayID displayID, int *adapterIndex, int *outputIndex, std::source_location location = std::source_location::current())
{
    if (!SDL_GetDXGIOutputInfo(displayID, adapterIndex, outputIndex))
    {
        SDLThrow(location);
    }
}

inline void SetX11EventHook(SDL_X11EventHook callback, void *userdata)
{
    SDL_SetX11EventHook(callback, userdata);
}

inline void IsTablet(std::source_location location = std::source_location::current())
{
    if (!SDL_IsTablet())
    {
        SDLThrow(location);
    }
}

inline void IsTV(std::source_location location = std::source_location::current())
{
    if (!SDL_IsTV())
    {
        SDLThrow(location);
    }
}

inline SDL_Sandbox GetSandbox()
{
    return SDL_GetSandbox();
}

inline void OnApplicationWillTerminate()
{
    SDL_OnApplicationWillTerminate();
}

inline void OnApplicationDidReceiveMemoryWarning()
{
    SDL_OnApplicationDidReceiveMemoryWarning();
}

inline void OnApplicationWillEnterBackground()
{
    SDL_OnApplicationWillEnterBackground();
}

inline void OnApplicationDidEnterBackground()
{
    SDL_OnApplicationDidEnterBackground();
}

inline void OnApplicationWillEnterForeground()
{
    SDL_OnApplicationWillEnterForeground();
}

inline void OnApplicationDidEnterForeground()
{
    SDL_OnApplicationDidEnterForeground();
}

inline void GetDateTimeLocalePreferences(SDL_DateFormat *dateFormat, SDL_TimeFormat *timeFormat, std::source_location location = std::source_location::current())
{
    if (!SDL_GetDateTimeLocalePreferences(dateFormat, timeFormat))
    {
        SDLThrow(location);
    }
}

inline void GetCurrentTime(SDL_Time *ticks, std::source_location location = std::source_location::current())
{
    if (!SDL_GetCurrentTime(ticks))
    {
        SDLThrow(location);
    }
}

inline void TimeToDateTime(SDL_Time ticks, SDL_DateTime *dt, bool localTime, std::source_location location = std::source_location::current())
{
    if (!SDL_TimeToDateTime(ticks, dt, localTime))
    {
        SDLThrow(location);
    }
}

inline void DateTimeToTime(const SDL_DateTime *dt, SDL_Time *ticks, std::source_location location = std::source_location::current())
{
    if (!SDL_DateTimeToTime(dt, ticks))
    {
        SDLThrow(location);
    }
}

inline void TimeToWindows(SDL_Time ticks, Uint32 *dwLowDateTime, Uint32 *dwHighDateTime)
{
    SDL_TimeToWindows(ticks, dwLowDateTime, dwHighDateTime);
}

inline SDL_Time TimeFromWindows(Uint32 dwLowDateTime, Uint32 dwHighDateTime)
{
    return SDL_TimeFromWindows(dwLowDateTime, dwHighDateTime);
}

inline int GetDaysInMonth(int year, int month)
{
    return SDL_GetDaysInMonth(year, month);
}

inline int GetDayOfYear(int year, int month, int day)
{
    return SDL_GetDayOfYear(year, month, day);
}

inline int GetDayOfWeek(int year, int month, int day)
{
    return SDL_GetDayOfWeek(year, month, day);
}

inline Uint64 GetTicks()
{
    return SDL_GetTicks();
}

inline Uint64 GetTicksNS()
{
    return SDL_GetTicksNS();
}

inline Uint64 GetPerformanceCounter()
{
    return SDL_GetPerformanceCounter();
}

inline Uint64 GetPerformanceFrequency()
{
    return SDL_GetPerformanceFrequency();
}

inline void Delay(Uint32 ms)
{
    SDL_Delay(ms);
}

inline void DelayNS(Uint64 ns)
{
    SDL_DelayNS(ns);
}

inline void DelayPrecise(Uint64 ns)
{
    SDL_DelayPrecise(ns);
}

inline SDL_TimerID AddTimer(Uint32 interval, SDL_TimerCallback callback, void *userdata)
{
    return SDL_AddTimer(interval, callback, userdata);
}

inline SDL_TimerID AddTimerNS(Uint64 interval, SDL_NSTimerCallback callback, void *userdata)
{
    return SDL_AddTimerNS(interval, callback, userdata);
}

inline void RemoveTimer(SDL_TimerID id, std::source_location location = std::source_location::current())
{
    if (!SDL_RemoveTimer(id))
    {
        SDLThrow(location);
    }
}

inline SDL_Tray *CreateTray(SDL_Surface *icon, const char *tooltip, std::source_location location = std::source_location::current())
{
    SDL_Tray *result = SDL_CreateTray(icon, tooltip);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetTrayIcon(SDL_Tray *tray, SDL_Surface *icon)
{
    SDL_SetTrayIcon(tray, icon);
}

inline void SetTrayTooltip(SDL_Tray *tray, const char *tooltip)
{
    SDL_SetTrayTooltip(tray, tooltip);
}

inline SDL_TrayMenu *CreateTrayMenu(SDL_Tray *tray, std::source_location location = std::source_location::current())
{
    SDL_TrayMenu *result = SDL_CreateTrayMenu(tray);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_TrayMenu *CreateTraySubmenu(SDL_TrayEntry *entry, std::source_location location = std::source_location::current())
{
    SDL_TrayMenu *result = SDL_CreateTraySubmenu(entry);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_TrayMenu *GetTrayMenu(SDL_Tray *tray, std::source_location location = std::source_location::current())
{
    SDL_TrayMenu *result = SDL_GetTrayMenu(tray);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_TrayMenu *GetTraySubmenu(SDL_TrayEntry *entry, std::source_location location = std::source_location::current())
{
    SDL_TrayMenu *result = SDL_GetTraySubmenu(entry);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline const SDL_TrayEntry **GetTrayEntries(SDL_TrayMenu *menu, int *count, std::source_location location = std::source_location::current())
{
    const SDL_TrayEntry **result = SDL_GetTrayEntries(menu, count);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void RemoveTrayEntry(SDL_TrayEntry *entry)
{
    SDL_RemoveTrayEntry(entry);
}

inline SDL_TrayEntry *InsertTrayEntryAt(SDL_TrayMenu *menu, int pos, const char *label, SDL_TrayEntryFlags flags, std::source_location location = std::source_location::current())
{
    SDL_TrayEntry *result = SDL_InsertTrayEntryAt(menu, pos, label, flags);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetTrayEntryLabel(SDL_TrayEntry *entry, const char *label)
{
    SDL_SetTrayEntryLabel(entry, label);
}

inline const char *GetTrayEntryLabel(SDL_TrayEntry *entry, std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetTrayEntryLabel(entry);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void SetTrayEntryChecked(SDL_TrayEntry *entry, bool checked)
{
    SDL_SetTrayEntryChecked(entry, checked);
}

inline void GetTrayEntryChecked(SDL_TrayEntry *entry, std::source_location location = std::source_location::current())
{
    if (!SDL_GetTrayEntryChecked(entry))
    {
        SDLThrow(location);
    }
}

inline void SetTrayEntryEnabled(SDL_TrayEntry *entry, bool enabled)
{
    SDL_SetTrayEntryEnabled(entry, enabled);
}

inline void GetTrayEntryEnabled(SDL_TrayEntry *entry, std::source_location location = std::source_location::current())
{
    if (!SDL_GetTrayEntryEnabled(entry))
    {
        SDLThrow(location);
    }
}

inline void SetTrayEntryCallback(SDL_TrayEntry *entry, SDL_TrayCallback callback, void *userdata)
{
    SDL_SetTrayEntryCallback(entry, callback, userdata);
}

inline void ClickTrayEntry(SDL_TrayEntry *entry)
{
    SDL_ClickTrayEntry(entry);
}

inline void DestroyTray(SDL_Tray *tray)
{
    SDL_DestroyTray(tray);
}

inline SDL_TrayMenu *GetTrayEntryParent(SDL_TrayEntry *entry, std::source_location location = std::source_location::current())
{
    SDL_TrayMenu *result = SDL_GetTrayEntryParent(entry);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_TrayEntry *GetTrayMenuParentEntry(SDL_TrayMenu *menu, std::source_location location = std::source_location::current())
{
    SDL_TrayEntry *result = SDL_GetTrayMenuParentEntry(menu);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline SDL_Tray *GetTrayMenuParentTray(SDL_TrayMenu *menu, std::source_location location = std::source_location::current())
{
    SDL_Tray *result = SDL_GetTrayMenuParentTray(menu);
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

inline void UpdateTrays()
{
    SDL_UpdateTrays();
}

inline int GetVersion()
{
    return SDL_GetVersion();
}

inline const char *GetRevision(std::source_location location = std::source_location::current())
{
    const char *result = SDL_GetRevision();
    if (!result)
    {
        SDLThrow(location);
    }
    return result;
}

} // namespace sdl
