#pragma once

namespace Ra
{
    /// Enum for texture formats abstraction
    enum class TextureFormat
    {
        None = -1,
        R32,
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

    /// Enum for texture wrapping mode
    enum class TextureWrap : std::uint8_t
    {
        ClampToEdge,
        ClampToBorder,
        MirroredRepeat,
        Repeat
    };

    /// Enum for texture filtering mode
    enum class TextureFilter : std::uint8_t
    {
        Nearest,
        Linear
    };

    enum class TextureType : std::uint8_t
    {
        Diffuse,
        Specular,
        Normal,
    };

    /// Class for texture abstraction
    class Texture
    {
    public:
        Texture() = default;
        virtual ~Texture() = default;

        /**
         * @brief Binds this texture for specified unit
         * @param id Unit to bind this texture to
        */
        virtual void Bind(int id) = 0;
        /// Binds this texture to previously bound unit (0 by default)
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        std::uint32_t GetWidth() const { return m_Width; }
        std::uint32_t GetHeight() const { return m_Height; }
        TextureFormat GetFormat() const { return m_Format; }
        TextureWrap GetWrap() const { return m_Wrap; }
        TextureFilter GetMinFilter() const { return m_MinFilter; }
        TextureFilter GetMagFilter() const { return m_MagFilter; }
        TextureType GetType() const { return m_Type; }
        std::string GetPath() const { return m_Path; }

        virtual RendererId GetNativeTerxtureHandle() const = 0;

        virtual void SetMinFilter(TextureFilter filter) = 0;
        virtual void SetMagFilter(TextureFilter filter) = 0;
        virtual void SetWrap(TextureWrap wrap) = 0;

        static Ref<Texture> Create(const std::string& path, TextureFormat format = TextureFormat::Color, TextureType type = TextureType::Diffuse);
        static Ref<Texture> Create(std::uint8_t* rawData, std::uint32_t width, std::uint32_t height, std::uint32_t channels, TextureFormat format = TextureFormat::Color, TextureType type = TextureType::Diffuse);

        static Ref<Texture> NullTexture;

    protected:
        virtual void LoadFromFile_(const std::string& path, TextureFormat format = TextureFormat::Color, TextureType type = TextureType::Diffuse) = 0;
        virtual void LoadFromData_(std::uint8_t* rawData, std::uint32_t width, std::uint32_t height, std::uint32_t channels, TextureFormat format = TextureFormat::Color, TextureType type = TextureType::Diffuse) = 0;

    protected:
        std::uint32_t m_Width{ 0 }, m_Height{ 0 };
        TextureFormat m_Format{ TextureFormat::None };
        TextureWrap m_Wrap{ TextureWrap::Repeat };
        TextureFilter m_MinFilter{ TextureFilter::Linear }, m_MagFilter{ TextureFilter::Linear };
        std::string m_Path{ "Internal resource" };
        TextureType m_Type{ TextureType::Diffuse };
    };

}
