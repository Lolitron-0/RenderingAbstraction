#pragma once

namespace Ra
{
    /// Enum for texture formats abstraction
    enum class TextureFormat : std::uint8_t
    {
        R32 = 0,
        R16,
        RG,
        RG16,
        R16F,
        R32F,
        RG16F,
        RG32F,
        RGB8,
        RGBA8,
        RGB16,
        RGB16F,
        RGBA16,
        RGBA16F,
        RGB32F,
        RGBA32F,
        Depth24Stencil8,

        //defaults
        Color = RGBA8,
        Depth = Depth24Stencil8
    };

    enum class TextureWrap : std::uint8_t
    {
        ClampToEdge,
        ClampToBorder,
        MirroredRepeat,
        Repeat
    };

    enum class TextureFilter : std::uint8_t
    {
        Nearest,
        Linear
    };

    class Texture
    {
    public:
        Texture() = default;
        virtual ~Texture() = default;
        virtual void LoadFromFile(const std::string& path, TextureFormat format = TextureFormat::Color) {}
        virtual void Load(std::uint8_t* rawData, std::uint32_t width, std::uint32_t height, std::uint32_t channels, TextureFormat format = TextureFormat::Color) {}

        virtual void Bind(int id) = 0;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        std::uint32_t GetWidth() const { return m_Width; }
        std::uint32_t GetHeight() const { return m_Height; }
        TextureFormat GetFormat() const { return m_Format; }
        TextureWrap GetWrap() const { return m_Wrap; }
        TextureFilter GetMinFilter() const { return m_MinFilter; }
        TextureFilter GetMagFilter() const { return m_MagFilter; }
        std::string GetSource() const { return m_Source; }

        virtual RendererId GetNativeTerxtureHandle() const = 0;
        
        virtual void SetMinFilter(TextureFilter filter) = 0;
        virtual void SetMagFilter(TextureFilter filter) = 0;
        virtual void SetWrap(TextureWrap wrap) = 0;

        static Scope<Texture> Create();
        static Scope<Texture> Create(const std::string& path);

        static Ref<Texture> NullTexture;

    protected:
        std::uint32_t m_Width{ 0 }, m_Height{ 0 };
        TextureFormat m_Format;
        TextureWrap m_Wrap{ TextureWrap::Repeat };
        TextureFilter m_MinFilter{ TextureFilter::Linear }, m_MagFilter{ TextureFilter::Linear };
        std::string m_Source{ "Internal resource" };
    };

}
