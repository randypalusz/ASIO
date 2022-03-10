#ifndef MESSAGE_ENUMS_HPP
#define MESSAGE_ENUMS_HPP

#include <type_traits>
#include <unordered_map>

// TODO: organize this stuff at LEAST in a namespace,
// if not a singleton/static class or something
enum class MessageType : uint8_t { EMPTY = 0, TEST = 1 };

inline uint8_t messageTypeToId(const MessageType& type) {
  return static_cast<uint8_t>(type);
}

inline static const std::unordered_map<uint8_t, MessageType> idToTypeMap{
    {messageTypeToId(MessageType::EMPTY), MessageType::EMPTY},
    {messageTypeToId(MessageType::TEST), MessageType::TEST}};

inline MessageType idToMessageType(uint8_t id) {
  try {
    return idToTypeMap.at(id);
  } catch (std::exception& e) {
    std::cerr << "Invalid id - implement this ID in the idToTypeMap...\n";
    return idToTypeMap.at(0);
  }
}
#endif
