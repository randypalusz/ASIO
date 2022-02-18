#ifndef HEADER_HPP
#define HEADER_HPP

#include <vector>

struct LayoutElement {
  int start;
  int end;
};

// byte 0 = ID
// bytes 1->8, high->low order bytes describing data layout size in bytes
// bytes 9->16, high->low order bytes describing data size in bytes
struct HeaderLayout {
  // TODO: make the ID field 16-bit to support many more message types
  static inline LayoutElement ID{0, 1};
  static inline LayoutElement LayoutSize{1, 9};
  static inline LayoutElement DataSize{9, 17};
  std::vector<LayoutElement> order{ID, LayoutSize, DataSize};
};

// TODO: move to .cpp file
class Header {
 public:
  Header(uint8_t id, uint64_t layoutSize, uint64_t size) {
    m_id = id;
    m_size = size;
    m_layoutSize = layoutSize;
    vectorizeSize();
  }

  Header(const std::vector<uint8_t>& header_recv_vector) {
    m_id = header_recv_vector.at(HeaderLayout::ID.start);
    m_layoutSizeBytes.insert(m_layoutSizeBytes.begin(),
                             header_recv_vector.begin() + HeaderLayout::LayoutSize.start,
                             header_recv_vector.begin() + HeaderLayout::LayoutSize.end);

    m_sizeBytes.insert(m_sizeBytes.begin(),
                       header_recv_vector.begin() + HeaderLayout::DataSize.start,
                       header_recv_vector.begin() + HeaderLayout::DataSize.end);
    setSizeFromVector();
  }
  void setSize(uint64_t newSize) {
    m_size = newSize;
    vectorizeSize();
  }
  void setLayoutSize(uint64_t newSize) {
    m_layoutSize = newSize;
    vectorizeSize();
  }
  void incrementSize(uint64_t sizeToAdd) {
    m_size += sizeToAdd;
    vectorizeSize();
  }
  void incrementLayoutSize(uint64_t sizeToAdd) {
    m_layoutSize += sizeToAdd;
    vectorizeSize();
  }
  const uint64_t getSize() const { return m_size; }
  const uint64_t getLayoutSize() const { return m_layoutSize; }
  const std::vector<uint8_t> getVectorizedSize() const { return m_sizeBytes; }
  const std::vector<uint8_t> getVectorizedLayoutSize() const { return m_layoutSizeBytes; }
  void print() const {
    std::cout << "-----------------------------------" << std::endl;
    std::printf("Header: \n id: %d\n", m_id);
    std::printf(" data layout size bytes (high->low)\n");
    for (int i = 0; i < m_layoutSizeBytes.size(); i++) {
      uint8_t byte = m_layoutSizeBytes.at(i);
      std::printf("     0x%02X\n", byte);
    }
    std::printf(" total layout size: %d\n", getLayoutSize());
    std::printf(" data bytes (high->low)\n");
    for (int i = 0; i < m_sizeBytes.size(); i++) {
      uint8_t byte = m_sizeBytes.at(i);
      std::printf("     0x%02X\n", byte);
    }
    std::printf(" total size: %d\n", getSize());
    std::cout << "-----------------------------------" << std::endl;
  }
  const std::vector<uint8_t> construct() const {
    std::vector<uint8_t> temp{m_id};
    temp.insert(temp.end(), m_layoutSizeBytes.begin(), m_layoutSizeBytes.end());
    temp.insert(temp.end(), m_sizeBytes.begin(), m_sizeBytes.end());
    return temp;
  }

  inline static size_t LENGTH_IN_BYTES = 17;

 private:
  void vectorizeSize() {
    m_sizeBytes.clear();
    m_layoutSizeBytes.clear();
    for (int shiftAmount = (64 - 8); shiftAmount >= 0; shiftAmount -= 8) {
      m_sizeBytes.push_back((m_size >> shiftAmount) & 0x00FF);
      m_layoutSizeBytes.push_back((m_layoutSize >> shiftAmount) & 0x00FF);
    }
  }
  void setSizeFromVector() {
    int shiftAmount = 64 - 8;
    for (uint8_t byte : m_sizeBytes) {
      m_size |= (byte << shiftAmount);
      shiftAmount -= 8;
    }
    shiftAmount = 64 - 8;
    for (uint8_t byte : m_layoutSizeBytes) {
      m_layoutSize |= (byte << shiftAmount);
      shiftAmount -= 8;
    }
  }
  uint8_t m_id{};
  uint64_t m_size = 0;
  uint64_t m_layoutSize = 0;
  std::vector<uint8_t> m_sizeBytes{};
  std::vector<uint8_t> m_layoutSizeBytes{};
};

#endif