#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stdint.h>

#include <cstring>
#include <iostream>
#include <vector>

#include "Header.hpp"

template <typename T>
class Message {
 public:
  Message<T>(T id) : m_header(id, 0) {}
  // allow constructing of message on receiving side
  Message<T>(Header<T> header, const std::vector<uint8_t>& newData) : m_header(header) {
    m_data.insert(m_data.end(), newData.begin(), newData.end());
  }

  void pushData(const std::string& inStr) {
    m_data.insert(m_data.end(), inStr.begin(), inStr.end());
    m_header.incrementSize(inStr.length());
    std::cout << "current header size: " << m_header.getSize() << std::endl;
  }

  template <typename DataType>
  void pushData(const DataType& in) {
    size_t dataTypeSize = sizeof(DataType);
    size_t idx = m_data.size();

    m_header.incrementSize(dataTypeSize);

    m_data.resize(m_data.size() + dataTypeSize);
    std::memcpy(m_data.data() + idx, &in, dataTypeSize);

    std::cout << "current header size: " << m_header.getSize() << std::endl;
  }

  template <typename DataType>
  void getBytes(DataType& outStructure, size_t start) const {
    if (m_data.empty()) {
      return;
    }
    std::memcpy(&outStructure, m_data.data() + start, sizeof(DataType));
  }

  void printBytes() {
    std::cout << "Bytes: ";
    for (uint8_t byte : m_data) {
      std::cout << byte;
    }
    std::cout << std::endl;
  }

  const std::vector<uint8_t> getHeader() { return m_header.construct(); }
  std::vector<uint8_t> getBytes() { return m_data; }
  size_t getSize() { return m_data.size(); }

 private:
  Header<T> m_header;
  // stores amount of bytes taken up by each thing pushed
  std::vector<uint64_t> m_dataSizes;
  std::vector<uint8_t> m_data;
};

#endif