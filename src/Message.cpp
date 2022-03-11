#include "Message.hpp"

Message::Message(Header header, const std::vector<uint8_t>& body_recv_buf)
    : m_header(header) {
  if (body_recv_buf.size() == 0) return;
  packLayoutBytes(body_recv_buf);
  m_data.insert(m_data.end(), body_recv_buf.begin() + header.getLayoutSize(),
                body_recv_buf.end());
}

void Message::pushData(const std::string& inStr) {
  uint64_t idx = m_data.size();
  m_data.insert(m_data.end(), inStr.begin(), inStr.end());
  m_header.incrementSize(inStr.length());
  m_header.incrementLayoutSize(8);
  m_dataLayout.push_back(idx);
}

void Message::getBytes(std::string& outString, size_t dataPosition) const {
  if (m_data.empty()) {
    return;
  }
  size_t bytesToRead = getNumBytesToRead(dataPosition);
  if (bytesToRead == 0) {
    return;
  }
  outString.assign(m_data.data() + getDataElementOffset(dataPosition),
                   m_data.data() + getDataElementOffset(dataPosition) + bytesToRead);
}

uint64_t Message::getDataElementOffset(size_t idx) const {
  try {
    return std::as_const(m_dataLayout).at(idx);
  } catch (const std::exception& e) {
    return 0;
  }
}

void Message::printBytes() {
  std::cout << "Bytes: ";
  for (uint8_t byte : m_data) {
    std::cout << byte;
  }
  std::cout << std::endl;
}

void Message::printLayoutBytes() {
  std::cout << "Layout Bytes: \n";
  for (uint64_t byte : m_dataLayout) {
    printf("  0x%016llX\n", byte);
  }
  std::cout << std::endl;
}

std::vector<uint8_t> Message::getBytes() {
  std::vector<uint8_t> temp{};
  for (uint64_t word : m_dataLayout) {
    for (int shiftAmount = 64 - 8; shiftAmount >= 0; shiftAmount -= 8) {
      temp.push_back((word >> shiftAmount) & 0x00FF);
    }
  }
  temp.insert(temp.end(), m_data.begin(), m_data.end());
  return temp;
}

void Message::packLayoutBytes(const std::vector<uint8_t>& newData) {
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

size_t Message::getNumBytesToRead(size_t dataPosition) const {
  size_t bytesToRead = 0;
  // if grabbing the last piece of data, read to the end
  if (dataPosition == (m_dataLayout.size() - 1)) {
    bytesToRead = m_data.size() - getDataElementOffset(dataPosition);
  } else {
    bytesToRead =
        getDataElementOffset(dataPosition + 1) - getDataElementOffset(dataPosition);
  }
  return bytesToRead;
}