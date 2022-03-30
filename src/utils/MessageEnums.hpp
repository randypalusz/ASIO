#ifndef MESSAGE_ENUMS_HPP
#define MESSAGE_ENUMS_HPP

#include <type_traits>
#include <unordered_map>

// TODO: organize this stuff at LEAST in a namespace,
// if not a singleton/static class or something
namespace MessageEnums {
  enum class Type : uint8_t { EMPTY, TEST, MESSAGETYPE_NUM_ITEMS };

  using namespace MessageEnums;
  inline uint8_t typeToId(const Type& type) { return static_cast<uint8_t>(type); }

  inline Type idToType(uint8_t id) {
    if (id < static_cast<uint8_t>(Type::MESSAGETYPE_NUM_ITEMS)) {
      return static_cast<Type>(id);
    } else {
      std::cerr << "Message ID: " << id
                << " is out of range - implement this ID in MessageType enum\n";
      return Type::EMPTY;
    }
  }
}  // namespace MessageEnums

// inline static const std::unordered_map<uint8_t, MessageType> idToTypeMap{
//     {messageTypeToId(MessageType::EMPTY), MessageType::EMPTY},
//     {messageTypeToId(MessageType::TEST), MessageType::TEST}};

// inline MessageType idToMessageType(uint8_t id) {
//   try {
//     return idToTypeMap.at(id);
//   } catch (std::exception& e) {
//     std::cerr << "Invalid id - implement this ID in the idToTypeMap...\n";
//     return idToTypeMap.at(0);
//   }
// }
#endif
