#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stdint.h>

#include <iostream>
#include <vector>

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

// TODO: make header broken size (8*8 bytes) a function here
template <typename T>
struct Header {
  T id{};
  uint64_t size = 0;
};

template <typename T>
class Message {
 public:
  Message<T>(T id) { m_header.id = id; }
  // allow constructing of message on receiving side
  Message<T>(T id, uint16_t size, const std::vector<uint8_t>& in) {
    m_header.id = id;
    m_header.size = size;
    m_data.insert(m_data.end(), in.begin(), in.end());
  }

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
    return retHeader;
  }
  std::vector<uint8_t> getBytes() { return m_data; }
  size_t getSize() { return m_data.size(); }

 private:
  Header<T> m_header;
  std::vector<uint8_t> m_data;
};

#endif