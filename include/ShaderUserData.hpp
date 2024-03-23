#pragma once
#include "rapch.h"

namespace Ra
{
    template<class... T>
    class ShaderUserData
    {
    public:
        using Types = TypeList<T...>;

        ShaderUserData() = default;
        ShaderUserData(std::uint32_t size, const char names[], std::tuple<T...> data)
        {
            for (std::uint32_t i{0}; i < size; i++)
            {
                m_Data.push_back(std::make_pair(names[i], data[i]));
            }
        }

        template<std::uint32_t index>
        auto GetData() const -> std::pair<std::string, typename TypeAt<index, Types>::type>
        {
            std::pair<std::string, typename TypeAt<index, Types>::type> ret{};
            ret.first = m_Data[index].first;
            ret.second = std::any_cast<typename TypeAt<index, Types>::type>(m_Data[index].second);
            return ret;
        }

        std::uint32_t GetSize() const { return m_Data.size(); }

    private:
        std::vector<std::pair<std::string, std::any>> m_Data;
    };
}
