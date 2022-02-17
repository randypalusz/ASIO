#ifndef HEADER_HPP
#define HEADER_HPP

#include <vector>

template <typename T>
class Header {
 public:
  Header<T>(T id, uint64_t layoutSize, uint64_t size) {
    this->id = id;
    this->size = size;
    this->layoutSize = layoutSize;
    vectorizeSize();
  }
  // Header<T>(T id, const std::vector<uint8_t> layoutSizeVector,
  //           const std::vector<uint8_t> sizeVector) {
  //   this->id = id;
  //   this->vectorizedSize = sizeVector;
  //   this->vectorizedLayoutSize = layoutSizeVector;
  //   setSizeFromVector();
  // }
  Header<T>(const std::vector<uint8_t>& header_recv_vector) {
    this->id = header_recv_vector.at(0);
    this->vectorizedLayoutSize.insert(this->vectorizedLayoutSize.begin(),
                                      header_recv_vector.begin() + 1,
                                      header_recv_vector.begin() + 9);
    this->vectorizedSize.insert(this->vectorizedSize.begin(),
                                header_recv_vector.begin() + 9, header_recv_vector.end());
    setSizeFromVector();
  }
  void setSize(uint64_t newSize) {
    this->size = newSize;
    vectorizeSize();
  }
  void setLayoutSize(uint64_t newSize) {
    this->layoutSize = newSize;
    vectorizeSize();
  }
  void incrementSize(uint64_t sizeToAdd) {
    this->size += sizeToAdd;
    vectorizeSize();
  }
  void incrementLayoutSize(uint64_t sizeToAdd) {
    this->layoutSize += sizeToAdd;
    vectorizeSize();
  }
  const uint64_t getSize() const { return size; }
  const uint64_t getLayoutSize() const { return layoutSize; }
  const std::vector<uint8_t> getVectorizedSize() const { return vectorizedSize; }
  const std::vector<uint8_t> getVectorizedLayoutSize() const {
    return vectorizedLayoutSize;
  }
  void print() const {
    std::cout << "-----------------------------------" << std::endl;
    std::printf("Header: \n id: %d\n", this->id);
    std::printf(" data layout size bytes (high->low)\n");
    for (int i = 0; i < this->vectorizedLayoutSize.size(); i++) {
      uint8_t byte = this->vectorizedLayoutSize.at(i);
      std::printf("     0x%02X\n", byte);
    }
    std::printf(" total layout size: %d\n", getLayoutSize());
    std::printf(" data bytes (high->low)\n");
    for (int i = 0; i < this->vectorizedSize.size(); i++) {
      uint8_t byte = this->vectorizedSize.at(i);
      std::printf("     0x%02X\n", byte);
    }
    std::printf(" total size: %d\n", getSize());
    std::cout << "-----------------------------------" << std::endl;
  }
  const std::vector<uint8_t> construct() const {
    // TODO: make id field larger to support ID types other than those 1 byte long
    std::vector<uint8_t> temp{id};
    temp.insert(temp.end(), vectorizedLayoutSize.begin(), vectorizedLayoutSize.end());
    temp.insert(temp.end(), vectorizedSize.begin(), vectorizedSize.end());
    return temp;
  }
  // byte 0 = ID
  // bytes 1->8, high->low order bytes describing data layout size in bytes
  // bytes 9->16, high->low order bytes describing data size in bytes
  T id{};
  inline static size_t LENGTH_IN_BYTES = 17;

 private:
  void vectorizeSize() {
    vectorizedSize.clear();
    vectorizedLayoutSize.clear();
    for (int shiftAmount = (64 - 8); shiftAmount >= 0; shiftAmount -= 8) {
      vectorizedSize.push_back((size >> shiftAmount) & 0x00FF);
      vectorizedLayoutSize.push_back((layoutSize >> shiftAmount) & 0x00FF);
    }
  }
  void setSizeFromVector() {
    int shiftAmount = 64 - 8;
    for (uint8_t byte : this->vectorizedSize) {
      size |= (byte << shiftAmount);
      shiftAmount -= 8;
    }
    shiftAmount = 64 - 8;
    for (uint8_t byte : this->vectorizedLayoutSize) {
      layoutSize |= (byte << shiftAmount);
      shiftAmount -= 8;
    }
  }
  uint64_t size = 0;
  uint64_t layoutSize = 0;
  std::vector<uint8_t> vectorizedSize{};
  std::vector<uint8_t> vectorizedLayoutSize{};
};

#endif