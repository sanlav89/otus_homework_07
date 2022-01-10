#pragma once

#include <cstring>
#include <type_traits>
#include <boost/uuid/detail/md5.hpp>
#include <boost/crc.hpp>
#include <boost/algorithm/hex.hpp>

namespace hash {

using boost::uuids::detail::md5;

template<typename T>
class HashValue
{
public:

    HashValue() = default;

    HashValue(const HashValue &other)
    {
        std::memcpy(m_data, other.m_data, sizeof(T));
    }

    size_t size() const
    {
        return sizeof(T);
    }

    const void *data() const
    {
        return &m_data;
    }

    void setData(void *data)
    {
        std::memcpy(&m_data, data, size());
    }

    friend inline bool operator==(const HashValue &first, const HashValue &second)
    {
        if (first.size() != second.size()) {
            return false;
        }
        return std::memcmp(first.data(), second.data(), first.size());
    }

    friend inline bool operator<(const HashValue &first, const HashValue &second)
    {
//        auto size = std::min(first.size(), second.size());
//        uint8_t *dataFirst = reinterpret_cast<uint8_t *>(first.data());
//        uint8_t *dataSecond = reinterpret_cast<uint8_t *>(second.data());
//        decltype(size) i = 0;
//        while (i < size) {
//            if (dataFirst[i] == dataSecond[i]) {
//                i++;
//            } else {
//                return dataFirst[i] < dataSecond[i];
//            }
//        }
        return false;
    }

private:

    T m_data;
};

template<typename T>
class Hash
{
public:

    Hash() = default;

    Hash(const Hash &other) : m_value(other.m_value)
    {
    }

    Hash(const void *buffer, std::size_t bytes)
    {
        calculate(buffer, bytes);
    }

    std::string toString() const
    {
        const auto charDigest = reinterpret_cast<const char *>(&m_value);
        std::string result;
        boost::algorithm::hex(charDigest, charDigest + sizeof(T),
                              std::back_inserter(result));
        return result;
    }

    Hash &operator=(const Hash &other)
    {
        m_value = other.m_value;
        return *this;
    }

    friend inline bool operator==(const Hash &first, const Hash &second)
    {
        return first.m_value == second.m_value;
    }

    friend inline bool operator!=(const Hash &first, const Hash &second)
    {
        return !(first.m_value == second.m_value);
    }

    friend inline bool operator<(const Hash &first, const Hash &second)
    {
        return first.m_value < second.m_value;
    }

protected:

    void calculate(const void *buffer, std::size_t bytes)
    {
        static_assert(std::is_same<T, md5::digest_type>::value || std::is_same<T, uint32_t>::value,
                      "Hash type is unsupported!");
        if (std::is_same<T, md5::digest_type>::value) {
            md5 hash;
            hash.process_bytes(buffer, bytes);
            hash.get_digest(m_value);
        } else if (std::is_same<T, uint32_t>::value) {
            boost::crc_32_type crc32;
            crc32.process_bytes(buffer, bytes);
            m_value = crc32.checksum();
        }
    }

    HashValue<T> m_value;

};

using md5_t = Hash<md5::digest_type>;
using crc32_t = Hash<uint32_t>;

//class Md5 : public Hash
//{
//public:

//    Md5() = default;

//    Md5(const void *buffer, std::size_t bytes);

//    Md5(const Md5 &other);

//    std::string toString() const /*override*/;

//    Hash &operator=(const Hash &other) /*override*/;

//    friend inline bool operator<(const Md5 &first, const Md5 &second);

//private:

//    md5::digest_type m_value;

//    void calculate(const void *buffer, std::size_t bytes) override;

//};

//class Crc32 : public Hash
//{
//public:

//    Crc32() = default;

//    Crc32(const void *buffer, std::size_t bytes);

//    Crc32(const Crc32 &other);

//    std::string toString() const /*override*/;

//    Hash &operator=(const Hash &other) /*override*/;

//    friend inline bool operator<(const Crc32 &first, const Crc32 &second);

//private:

//    uint32_t m_value;

//    void calculate(const void *buffer, std::size_t bytes) override;

//};

//inline bool operator<(const Md5 &first, const Md5 &second)
//{
//    auto i = 0;
//    while (i < 4) {
//        if (first.m_value[i] == second.m_value[i]) {
//            i++;
//        } else {
//            return first.m_value[i] < second.m_value[i];
//        }
//    }
//    return false;
//}

//inline bool operator<(const Crc32 &first, const Crc32 &second)
//{
//    return first.m_value < second.m_value;
//}

}

