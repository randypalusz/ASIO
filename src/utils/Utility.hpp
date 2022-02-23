#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <memory>
namespace utility {
  template <typename T>
  std::unique_ptr<T> getUniquePtrToType() {
    return std::make_unique<T>();
  }
}  // namespace utility

#endif