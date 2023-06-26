#pragma once

namespace Ra
{
    using ErrorCallbackFn = std::function<void(const std::string&)>;

    extern ErrorCallbackFn errorCallback;
    
    inline void SetErrorCallback(const ErrorCallbackFn& callback) { errorCallback = callback; }

    template<class T>
    using Scope = std::unique_ptr<T>;

    template<class T>
    using Ref = std::shared_ptr<T>;
}

#include <filesystem>

#define RA_THROW_ERROR(message) do { Ra::errorCallback(message); }while(0);

#ifdef RA_WINDOWS
#define RA_DEBUGBREAK __debugbreak();
#else
#error "Ra only supports Windows!"
#endif // RA_WINDOWS


#ifdef RA_DEBUG
#define RA_FILENAME std::filesystem::path(__FILE__).filename().string()
#define RA_EXPAND(x) x
#define RA_STRINGIFY(x) #x
#define RA_TO_STRING(x) RA_STRINGIFY(x)
#define RA_LINE RA_TO_STRING(__LINE__)

#define RA_ASSERT(cond, message) if (!(cond)) {RA_THROW_ERROR(RA_FILENAME +':'+ RA_LINE + "  Assertion failed! "+message); RA_DEBUGBREAK;}
#else
#define RA_ASSERT(cond, message)
#endif // RA_DEBUG
