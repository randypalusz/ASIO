#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
template <typename T>
struct Header {
  T id{};
  uint16_t size = 0;
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
    retHeader.push_back((m_header.size >> 8) & 0x00FF);
    retHeader.push_back(m_header.size & 0x00FF);
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