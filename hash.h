#pragma once

#include <cstring>
#include <boost/uuid/detail/md5.hpp>
#include <boost/crc.hpp>
#include <boost/algorithm/hex.hpp>

namespace hash {

using boost::uuids::detail::md5;

class IHash
{
    virtual void calculate(const void *buffer, std::size_t bytes) = 0;
    virtual std::string toString() const = 0;
};

class Md5 : public IHash
{
public:

    Md5() = default;

    Md5(const void *buffer, std::size_t bytes);

    Md5(const Md5 &other);

    std::string toString() const override;

    friend inline bool operator<(const Md5 &first, const Md5 &second);
    friend inline bool operator==(const Md5 &first, const Md5 &second);
    friend inline bool operator!=(const Md5 &first, const Md5 &second);

private:

    md5::digest_type m_value;

    void calculate(const void *buffer, std::size_t bytes) override;

};

class Crc32 : public IHash
{
public:

    Crc32() = default;

    Crc32(const void *buffer, std::size_t bytes);

    Crc32(const Crc32 &other);

    std::string toString() const override;

    friend inline bool operator<(const Crc32 &first, const Crc32 &second);

private:

    uint32_t m_value;

    void calculate(const void *buffer, std::size_t bytes) override;

};

inline bool operator<(const Md5 &first, const Md5 &second)
{
    auto i = 0;
    while (i < 4) {
        if (first.m_value[i] == second.m_value[i]) {
            i++;
        } else {
            return first.m_value[i] < second.m_value[i];
        }
    }
    return false;
}

inline bool operator==(const Md5 &first, const Md5 &second)
{
    return std::memcmp(first.m_value, second.m_value, sizeof(md5::digest_type)) == 0;
}

inline bool operator!=(const Md5 &first, const Md5 &second)
{
    return !(first == second);
}

inline bool operator<(const Crc32 &first, const Crc32 &second)
{
    return first.m_value < second.m_value;
}

}

