#pragma once

#include <cstring>
#include <vector>
#include <boost/uuid/detail/md5.hpp>
#include <boost/crc.hpp>
#include <boost/algorithm/hex.hpp>

namespace hash {

enum HashType {
    AlgMd5,
    AlgCrc32
};

using alg_t = HashType;
using byte_t = char;

using boost::uuids::detail::md5;

class IHashCalculator
{
    virtual void calculate(const void *ibuf, std::size_t bytes, void *obuf) = 0;
};

class Md5Calculator : public IHashCalculator
{
public:
    void calculate(const void *ibuf, std::size_t bytes, void *obuf) override
    {
        md5 hash;
        hash.process_bytes(ibuf, bytes);
        md5::digest_type value;
        hash.get_digest(value);
        std::memcpy(obuf, value, sizeof(md5::digest_type));
    }
};

class Crc32Calculator : public IHashCalculator
{
public:
    void calculate(const void *ibuf, std::size_t bytes, void *obuf) override
    {
        boost::crc_32_type crc32;
        crc32.process_bytes(ibuf, bytes);
        auto value = crc32.checksum();
        std::memcpy(obuf, &value, sizeof(value));
    }
};

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

