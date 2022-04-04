#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stdint.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Header.hpp"
#include "MessageEnums.hpp"
#include "MessageLayout.hpp"
#include "MessageLoader.hpp"

// Everything here is big-endian
class MessageLayout;
class MessageLoader;

class Message {
 public:
  Message(MessageEnums::Type type);
  // allow constructing of message on receiving side by passing entire receiver buffer
  Message(Header header, const std::vector<uint8_t>& body_recv_buf);
  inline const MessageEnums::Type getType() const {
    return MessageEnums::idToType(m_header.getId());
  }

  template <typename DataType>
  void pushData(const DataType& in);
  void pushData(const std::string& inStr);

  template <typename DataType>
  void getBytes(DataType& outStructure, size_t dataPosition) const;
  void getBytes(std::string& outString, size_t dataPosition) const;
  // converts the 64-bit starting locations into a vector of 8-bit values
  // e.g.
  // 0x000000001A300085 -> <0x00, 0x00, 0x00, 0x00, 0x1A, 0x30, 0x00, 0x85>
  // then, appends the data vector to that to get the final message buffer
  // intended for server-side acquisition of data buffer for sending data out
  std::vector<uint8_t> getBytes();

  uint64_t getDataElementOffset(size_t idx) const;

  void printBytes();
  void printLayoutBytes();
  inline void printHeader() { m_header.print(); }
  inline const std::vector<uint8_t> getHeader() { return m_header.construct(); }
  inline size_t getSize() { return m_data.size(); }

 private:
  void packLayoutBytes(const std::vector<uint8_t>& newData);
  size_t getNumBytesToRead(size_t dataPosition) const;
  Header m_header;
  // TODO: I think this leaks - need to delete this in Message destructor
  MessageLoader* m_loader = nullptr;
  std::unique_ptr<MessageLayout> m_layout = nullptr;
  // stores starting index of each thing pushed to m_data
  std::vector<uint64_t> m_dataLayout;
  std::vector<uint8_t> m_data;
};

template <typename DataType>
void Message::getBytes(DataType& outStructure, size_t dataPosition) const {
  if (m_data.empty()) {
    return;
  }
  size_t bytesToRead = getNumBytesToRead(dataPosition);
  if (bytesToRead == 0) {
    return;
  }
  std::memcpy(&outStructure, m_data.data() + getDataElementOffset(dataPosition),
              bytesToRead);
}

template <typename DataType>
void Message::pushData(const DataType& in) {
  // printf("");
  uint64_t dataTypeSize = sizeof(DataType);
  uint64_t idx = m_data.size();

  m_header.incrementSize(dataTypeSize);

  m_data.resize(m_data.size() + dataTypeSize);
  std::memcpy(m_data.data() + idx, &in, dataTypeSize);

  // push this information to the data layout
  m_header.incrementLayoutSize(8);
  m_dataLayout.push_back(idx);
}

#endif