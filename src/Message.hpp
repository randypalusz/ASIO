#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stdint.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Header.hpp"
#include "MessageEnums.hpp"

// Everything here is big-endian

// TODO: move to .cpp file
class Message {
 public:
  Message(MessageType type) : m_header(type, 0, 0) {}
  // allow constructing of message on receiving side by passing entire receiver buffer
  Message(Header header, const std::vector<uint8_t>& body_recv_buf);
  void pushData(const std::string& inStr);
  inline const MessageType getType() const { return idToMessageType(m_header.getId()); }

  // template needs to be defined in the header
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

  // converts the 64-bit starting locations into a vector of 8-bit values
  // e.g.
  // 0x000000001A300085 -> <0x00, 0x00, 0x00, 0x00, 0x1A, 0x30, 0x00, 0x85>
  // then, appends the data vector to that to get the final message buffer
  // intended for server-side acquisition of data buffer for send
  std::vector<uint8_t> getBytes();
  void getBytes(std::string& outString, size_t dataPosition) const;

  template <typename DataType>
  void getBytes(DataType& outStructure, size_t dataPosition) const {
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

  uint64_t getLayoutBytes(size_t idx) const;

  void printBytes();
  void printLayoutBytes();
  inline void printHeader() { m_header.print(); }
  inline const std::vector<uint8_t> getHeader() { return m_header.construct(); }

  inline size_t getSize() { return m_data.size(); }

 private:
  void packLayoutBytes(const std::vector<uint8_t>& newData);
  Header m_header;
  // stores starting index of each thing pushed to m_data
  std::vector<uint64_t> m_dataLayout;
  std::vector<uint8_t> m_data;
};

#endif