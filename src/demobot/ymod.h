#pragma once

#if defined(__linux__)
#define YMOD_LINUX
#elif defined(__APPLE__)
#define YMOD_OSX
#elif defined(_WIN32)
#define YMOD_WINDOWS
#endif

typedef void *YMOD_SHARED_LIB_HANDLE;

#ifdef YMOD_WINDOWS
#define YMOD_SHARED_LIB_OPEN(path) (YMOD_SHARED_LIB_HANDLE)LoadLibrary(path)
#define YMOD_SHARED_LIB_CLOSE(lib) FreeLibrary(lib)
#define YMOD_SHARED_LIB_FUNC(lib, func) GetProcAddress(lib, func)
#else
#define YMOD_SHARED_LIB_OPEN(path) (YMOD_SHARED_LIB_HANDLE) dlopen(path, RTLD_NOW)
#define YMOD_SHARED_LIB_CLOSE(lib) dlclose(lib)
#define YMOD_SHARED_LIB_FUNC(lib, func) dlsym(lib, func)
#endif

#ifdef YMOD_WINDOWS
#define DLLEXPORT __declspec(dllexport)
#else
// #define DLLEXPORT __attribute__((visibility("default")))
#define DLLEXPORT
#endif
#ifdef __cplusplus
#define YMOD_EXPORT extern "C" DLLEXPORT
#else
#define YMOD_EXPORT DLLEXPORT
#endif