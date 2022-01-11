#include "hash.h"

using namespace hash;


Md5::Md5(const void *buffer, std::size_t bytes)
{
    calculate(buffer, bytes);
}

Md5::Md5(const Md5 &other)
{
    std::memcpy(&m_value, &other.m_value, sizeof(md5::digest_type));
}

std::string Md5::toString() const
{
    const auto charDigest = reinterpret_cast<const char *>(&m_value);
    std::string result;
    boost::algorithm::hex(charDigest, charDigest + sizeof(md5::digest_type),
                          std::back_inserter(result));
    return result;
}

void Md5::calculate(const void *buffer, std::size_t bytes)
{
    md5 hash;
    hash.process_bytes(buffer, bytes);
    hash.get_digest(m_value);
}

Crc32::Crc32(const void *buffer, std::size_t bytes)
{
    calculate(buffer, bytes);
}

Crc32::Crc32(const Crc32 &other) : m_value(other.m_value) {}

std::string Crc32::toString() const
{
    const auto charDigest = reinterpret_cast<const char *>(&m_value);
    std::string result;
    boost::algorithm::hex(charDigest, charDigest + sizeof(uint32_t),
                          std::back_inserter(result));
    return result;
}

void Crc32::calculate(const void *buffer, std::size_t bytes)
{
    boost::crc_32_type crc32;
    crc32.process_bytes(buffer, bytes);
    m_value = crc32.checksum();
}
