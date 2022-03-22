#ifndef MESSAGE_LOADER_HPP
#define MESSAGE_LOADER_HPP
#include <any>
#include <array>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "Message.hpp"
#include "MessageLayout.hpp"
#include "Utility.hpp"

class MessageLoader {
 public:
  MessageLoader(MessageLoader& other) = delete;
  void operator=(const MessageLoader&) = delete;

  static MessageLoader* getInstance();
  std::unique_ptr<MessageLayout> getMessage(Message& m);

 protected:
  MessageLoader() {}
  static inline MessageLoader* loader = nullptr;

 private:
  using MessageLayoutCreator = std::function<std::unique_ptr<MessageLayout>()>;
  // msgID, function pointer that makes unique_ptr to desired message type
  // TODO: think about combining this with the idToTypeMap in MessageEnums
  //       to create a vector<tuple(uint8_t, MessageType, MessageLayoutCreator)>
  //       This would consolidate everything related to these enums in one spot.
  //       Also think about initializing this new structure upon first MessageLoader
  //       instantiation
  inline static const std::unordered_map<MessageType, MessageLayoutCreator>
      idToCreatorMap{{MessageType::EMPTY, &utility::getUniquePtrToType<EmptyMessage>},
                     {MessageType::TEST, &utility::getUniquePtrToType<TestMessage>}};
};
#endif