#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stdint.h>

#include <cstring>
#include <iostream>
#include <vector>
#include <string>

#include "Header.hpp"

// Everything here is big-endian

// TODO: move to .cpp file
class Message {
 public:
  Message(uint8_t id) : m_header(id, 0, 0) {}
  // allow constructing of message on receiving side by passing entire receiver buffer
  Message(Header header, const std::vector<uint8_t>& body_recv_buf) : m_header(header) {
    packLayoutBytes(body_recv_buf);
    m_data.insert(m_data.end(), body_recv_buf.begin() + header.getLayoutSize(),
                  body_recv_buf.end());
  }

  void pushData(const std::string& inStr) {
    uint64_t idx = m_data.size();
    m_data.insert(m_data.end(), inStr.begin(), inStr.end());
    m_header.incrementSize(inStr.length());
    m_header.incrementLayoutSize(8);
    m_dataLayout.push_back(idx);
  }

  inline const uint8_t getId() const { return m_header.getId(); }

  template <typename DataType>
  void pushData(const DataType& in) {
    uint64_t dataTypeSize = sizeof(DataType);
    uint64_t idx = m_data.size();

    m_header.incrementSize(dataTypeSize);

    m_data.resize(m_data.size() + dataTypeSize);
    std::memcpy(m_data.data() + idx, &in, dataTypeSize);

    // push this information to the data layout
    m_header.incrementLayoutSize(8);
    m_dataLayout.push_back(idx);
  }

  template <typename DataType>
  void getBytes(DataType& outStructure, size_t dataPosition) {
    if (m_data.empty()) {
      return;
    }
    size_t bytesToRead = 0;
    // if grabbing the last piece of data, read to the end
    if (dataPosition == (m_dataLayout.size() - 1)) {
      bytesToRead = m_data.size() - getLayoutBytes(dataPosition);
    } else {
      bytesToRead = getLayoutBytes(dataPosition + 1) - getLayoutBytes(dataPosition);
    }
    if (bytesToRead == 0) {
      return;
    }
    std::memcpy(&outStructure, m_data.data() + getLayoutBytes(dataPosition), bytesToRead);
  }

  void getBytes(std::string& outString, size_t dataPosition) {
    if (m_data.empty()) {
      return;
    }
    size_t bytesToRead = 0;
    // if grabbing the last piece of data, read to the end
    if (dataPosition == (m_dataLayout.size() - 1)) {
      bytesToRead = m_data.size() - getLayoutBytes(dataPosition);
    } else {
      bytesToRead = getLayoutBytes(dataPosition + 1) - getLayoutBytes(dataPosition);
    }
    if (bytesToRead == 0) {
      return;
    }
    // outString.resize(bytesToRead);
    // std::memcpy(&outString[0], m_data.data() + getLayoutBytes(dataPosition),
    // bytesToRead);
    outString.assign(m_data.data() + getLayoutBytes(dataPosition),
                     m_data.data() + getLayoutBytes(dataPosition) + bytesToRead);
  }

  uint64_t getLayoutBytes(size_t idx) {
    try {
      return m_dataLayout.at(idx);
    } catch (const std::exception& e) {
      return 0;
    }
  }

  void printBytes() {
    std::cout << "Bytes: ";
    for (uint8_t byte : m_data) {
      std::cout << byte;
    }
    std::cout << std::endl;
  }

  void printLayoutBytes() {
    std::cout << "Layout Bytes: \n";
    for (uint64_t byte : m_dataLayout) {
      printf("  0x%016X\n", byte);
    }
    std::cout << std::endl;
  }

  void printHeader() { m_header.print(); }

  const std::vector<uint8_t> getHeader() { return m_header.construct(); }

  // converts the 64-bit starting locations into a vector of 8-bit values
  // e.g.
  // 0x000000001A300085 -> <0x00, 0x00, 0x00, 0x00, 0x1A, 0x30, 0x00, 0x85>
  // then, appends the data vector to that to get the final message buffer
  std::vector<uint8_t> getBytes() {
    std::vector<uint8_t> temp{};
    for (uint64_t word : m_dataLayout) {
      for (int shiftAmount = 64 - 8; shiftAmount >= 0; shiftAmount -= 8) {
        temp.push_back((word >> shiftAmount) & 0x00FF);
      }
    }
    temp.insert(temp.end(), m_data.begin(), m_data.end());
    return temp;
  }

  size_t getSize() { return m_data.size(); }

 private:
  void packLayoutBytes(const std::vector<uint8_t>& newData) {
    // m_dataLayout.resize(m_header.getLayoutSize() / 8);
    uint64_t word = 0;
    int shiftAmount = 64;
    for (int i = 0; i <= m_header.getLayoutSize(); i++) {
      shiftAmount -= 8;
      if (shiftAmount < 0) {
        m_dataLayout.push_back(word);
        word = 0;
        shiftAmount = 56;
      }
      word |= (newData.at(i) << shiftAmount);
    }
  }
  Header m_header;
  // stores starting index of each thing pushed to m_data
  std::vector<uint64_t> m_dataLayout;
  std::vector<uint8_t> m_data;
};

#endif