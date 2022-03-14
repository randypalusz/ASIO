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
  inline static const std::unordered_map<MessageType, MessageLayoutCreator>
      idToCreatorMap{{MessageType::EMPTY, &utility::getUniquePtrToType<EmptyMessage>},
                     {MessageType::TEST, &utility::getUniquePtrToType<TestMessage>}};
};
#endif