#pragma once
#include <filesystem>
#include <functional>

namespace Ra
{
    using RendererId = std::uint32_t;

    using CallbackFn = std::function<void(const std::string&)>;

    extern CallbackFn errorCallback;
    extern CallbackFn logCallback;

    inline void SetErrorCallback(const CallbackFn& callback) { errorCallback = callback; }
    inline void SetLogCallback(const CallbackFn& callback) { logCallback = callback; }

    std::string GetShadersDir();

    template<class T>
    using Scope = std::unique_ptr<T>;
    template<class T, class... Args>
    Scope<T> MakeScope(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

    template<class T>
    using Ref = std::shared_ptr<T>;
    template<class T, class... Args>
    Ref<T> MakeRef(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
}

#define RA_THROW_ERROR(message) do { Ra::errorCallback(message); }while(0);
#define RA_LOG(message) do { Ra::logCallback(message); }while(0);

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

struct NullType {};

template<class... T>
struct TypeList
{
    using Head = NullType;
    using Tail = NullType;
};

using EmptyTypeList = TypeList<>;

template<class H, class... T>
struct TypeList<H, T...>
{
    using Head = H;
    using Tail = TypeList<T...>;
};

template <class TL>
struct IsEmpty : std::true_type {};

template <>
struct IsEmpty<EmptyTypeList> : std::true_type {};

template<class... Args>
struct IsEmpty<TypeList<Args...>>
    : std::integral_constant<bool,
    std::is_same<typename TypeList<Args...>::Head, NullType>::value&&
    IsEmpty<typename TypeList<Args...>::Tail>::value>  //inheriting whole constant type to have ::value 
{};

template<class TL>
struct Length
    : std::integral_constant<unsigned int, 0>
{};

template<class... Args>
struct Length<TypeList<Args...>>
    : std::integral_constant<unsigned int,
    IsEmpty<TypeList<Args...>>::value ?
    0
    :
    1 + Length<typename TypeList<Args...>::Tail>::value
    >
{};

template<unsigned int N, class TL>
struct TypeAt
{
    using type = NullType;
};

template<class... Args>
struct TypeAt<0, TypeList<Args...>>
{
    using type = typename TypeList<Args...>::Head;
};

template<unsigned int N, class... Args>
struct TypeAt<N, TypeList<Args...>>
{
    static_assert(N > Length<TypeList<Args...>>::value);
    using type = typename TypeAt<N - 1, typename TypeList<Args...>::Tail>::type;
};