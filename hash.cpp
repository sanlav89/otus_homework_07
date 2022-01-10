#include "hash.h"

using namespace hash;

//Md5::Md5(const void *buffer, std::size_t bytes) : Hash(buffer, bytes)
//{
//}

//Md5::Md5(const Md5 &other)
//{
//    std::memcpy(&m_value, &other.m_value, sizeof(md5::digest_type));
//}

////Md5 &Md5::operator=(const Md5 &other)
////{
////    std::memcpy(&m_value, &other.m_value, sizeof(md5::digest_type));
////    return *this;
////}

//std::string Md5::toString() const
//{
//    const auto charDigest = reinterpret_cast<const char *>(&m_value);
//    std::string result;
//    boost::algorithm::hex(charDigest, charDigest + sizeof(md5::digest_type),
//                          std::back_inserter(result));
//    return result;
//}

//Hash &Md5::operator=(const Hash &other)
//{
////    *this = Md5(other);
//    return *this;
//}

//void Md5::calculate(const void *buffer, std::size_t bytes)
//{
//    md5 hash;
//    hash.process_bytes(buffer, bytes);
//    hash.get_digest(m_value);
//}

//Crc32::Crc32(const void *buffer, std::size_t bytes) : Hash(buffer, bytes)
//{
//}

//Crc32::Crc32(const Crc32 &other) : m_value(other.m_value)
//{
//}

//std::string Crc32::toString() const
//{
//    const auto charDigest = reinterpret_cast<const char *>(&m_value);
//    std::string result;
//    boost::algorithm::hex(charDigest, charDigest + sizeof(uint32_t),
//                          std::back_inserter(result));
//    return result;
//}

//Hash &Crc32::operator=(const Hash &other)
//{
////    m_value = Crc32(other).m_value;
//    return *this;
//}

//void Crc32::calculate(const void *buffer, std::size_t bytes)
//{
//    boost::crc_32_type crc32;
//    crc32.process_bytes(buffer, bytes);
//    m_value = crc32.checksum();
//}

//uint32_t HashValue::data() const
//{
//    return m_data;
//}

//void HashValue::setData(const uint32_t &data)
//{
//    m_data = data;
//}
