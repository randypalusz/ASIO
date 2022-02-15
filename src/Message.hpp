#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <stdint.h>

#include <iostream>
#include <vector>

template <typename T>
class Header {
 public:
  Header<T>(T id, uint64_t size) {
    this->id = id;
    this->size = size;
    vectorizeSize();
  }
  Header<T>(T id, const std::vector<uint8_t> sizeVector) {
    this->id = id;
    this.vectorizedSize = sizeVector;
    setSizeFromVector();
  }
  Header<T>(const std::vector<uint8_t>& header_recv_vector) {
    this->id = header_recv_vector.at(0);
    this->vectorizedSize.insert(this->vectorizedSize.begin(),
                                header_recv_vector.begin() + 1, header_recv_vector.end());
    setSizeFromVector();
  }
  void setSize(uint64_t newSize) {
    this->size = newSize;
    vectorizeSize();
  }
  void incrementSize(uint64_t sizeToAdd) {
    this->size += sizeToAdd;
    vectorizeSize();
  }
  const uint64_t getSize() const { return size; }
  const std::vector<uint8_t> getVectorizedSize() const { return vectorizedSize; }
  void print() const {
    std::printf("Header: \n  id: %d\n  bytes_from_high_to_low:\n", this->id);
    for (int i = 0; i < this->vectorizedSize.size(); i++) {
      uint8_t byte = this->vectorizedSize.at(i);
      std::printf("     0x%02X\n", byte);
    }
  }
  const std::vector<uint8_t> construct() const {
    // TODO: make id field larger to support ID types other than those 1 byte long
    std::vector<uint8_t> temp{id};
    temp.insert(temp.end(), vectorizedSize.begin(), vectorizedSize.end());
    return temp;
  }
  // byte 0 = ID
  // bytes 1->9, high->low order bytes describing data size in bytes
  T id{};
  inline static size_t LENGTH_IN_BYTES = 9;

 private:
  void vectorizeSize() {
    vectorizedSize.clear();
    for (int shiftAmount = (64 - 8); shiftAmount >= 0; shiftAmount -= 8) {
      vectorizedSize.push_back((size >> shiftAmount) & 0x00FF);
    }
  }
  void setSizeFromVector() {
    int shiftAmount = 64 - 8;
    for (uint8_t byte : this->vectorizedSize) {
      size |= (byte << shiftAmount);
      shiftAmount -= 8;
    }
  }
  uint64_t size = 0;
  // from idx 0-7, high->low byte
  std::vector<uint8_t> vectorizedSize{};
};

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
  std::vector<uint8_t> m_data;
};

#endif