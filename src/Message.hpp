#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <vector>
#include <stdint.h>

// input size in bytes (chars), outputs vector of 8 bytes
// at a time that when concatenated represents the whole size
std::vector<uint8_t> headerSizeToVector(uint64_t size) {
  std::vector<uint8_t> retVector{};
  for (int shiftAmount = (64 - 8); shiftAmount >= 0; shiftAmount -= 8) {
    retVector.push_back((size >> shiftAmount) & 0x00FF);
  }
  return retVector;
}

uint64_t vectorToHeaderSize(const std::vector<uint8_t>& in) {
  uint64_t size = 0;
  int shiftAmount = 64 - 8;
  for (uint8_t byte : in) {
    size |= (byte << shiftAmount);
    shiftAmount -= 8;
  }
  return size;
}

void printHeader(const std::vector<uint8_t>& in) {
  std::printf("Header: \n  type: %d\n  bytes_from_high_to_low:\n", in.at(0));
  for (int i = 1; i < in.size(); i++) {
    uint8_t byte = in.at(i);
    std::printf("     0x%02X\n", byte);
  }
  // for (uint8_t byte : in) {
  // }
  // 0x%02X\n  low_order_byte: 0x%02X\n",
  // in[0], in[1], in[2]);
}

template <typename T>
struct Header {
  T id{};
  uint64_t size = 0;
};

template <typename T>
class Message {
 public:
  Message<T>(T id) { m_header.id = id; }
  Message<T>(T id, uint16_t size, const std::vector<uint8_t> in) {
    m_header.id = id;
    m_header.size = size;
    m_data.insert(m_data.end(), in.begin(), in.end());
  }

  size_t getSize() { return m_data.size(); }
  // template <typename DataType>
  // void pushData(const DataType& newData) {
  //   std::cout << "- Pushing data -" << std::endl;
  //   std::cout << "sizeof(DataType) = " << sizeof(DataType) << std::endl;
  //   std::cout << "newData = " << newData << std::endl;
  //   // Check that the type of the data being pushed is trivially copyable
  //   static_assert(std::is_standard_layout<DataType>::value,
  //                 "Data is too complex to be pushed into vector");

  //   // Cache current size of vector, as this will be the point we insert the data
  //   size_t i = m_data.size();

  //   // Resize the vector by the size of the data being pushed
  //   m_data.resize(m_data.size() + sizeof(DataType));

  //   // Physically copy the data into the newly allocated vector space
  //   std::memcpy(m_data.data() + i, &newData, sizeof(DataType));

  //   // Recalculate the message size
  //   m_header.size = m_data.size();
  // }
  // outData is the output parameter that will contain the data from the vector
  // template <typename DataType>
  // void popData(DataType& outData) {
  //   std::cout << "- Popping data -" << std::endl;
  //   std::cout << "sizeof(DataType) = " << sizeof(DataType) << std::endl;
  //   // Cache current size of vector, as this will be the point we insert the data
  //   size_t i = m_data.size() - sizeof(DataType);

  //   // Physically copy the data into the newly allocated vector space
  //   std::memcpy(&outData, m_data.data() + i, sizeof(DataType));
  //   std::cout << "outData = ";
  //   for (int j = i; j < m_data.size(); j++) {
  //     std::cout << m_data.at(j);
  //   }

  //   std::cout << std::endl;

  //   // Resize the vector by the size of the data being pushed
  //   m_data.resize(i);

  //   // Recalculate the message size
  //   m_header.size = m_data.size();
  // }
  void pushData(const std::string& inStr) {
    m_data.insert(m_data.end(), inStr.begin(), inStr.end());
    m_header.size += inStr.length();
    std::cout << "current header size: " << m_header.size << std::endl;
  }

  void printBytes() {
    std::cout << "Bytes: ";
    for (uint8_t byte : m_data) {
      std::cout << byte;
    }
    std::cout << std::endl;
  }

  std::vector<uint8_t> getHeader() {
    std::vector<uint8_t> retHeader{};
    retHeader.push_back(m_header.id);
    std::vector<uint8_t> sizeVector{headerSizeToVector(m_header.size)};
    retHeader.insert(retHeader.end(), sizeVector.begin(), sizeVector.end());
    // retHeader.push_back((m_header.size >> 8) & 0x00FF);
    // retHeader.push_back(m_header.size & 0x00FF);
    return retHeader;
  }
  std::vector<uint8_t> getBytes() {
    // std::vector<uint8_t> retData{};
    // retData.insert(retData.end(), m_data.begin(), m_data.end());

    return m_data;
  }

 private:
  Header<T> m_header;
  std::vector<uint8_t> m_data;
};

#endif