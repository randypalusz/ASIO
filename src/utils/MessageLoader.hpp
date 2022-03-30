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

class Message;
class MessageLayout;

class MessageLoader {
 public:
  MessageLoader(MessageLoader& other) = delete;
  void operator=(const MessageLoader&) = delete;

  static MessageLoader* getInstance();
  std::unique_ptr<MessageLayout> getMessage(Message& m);
  // TODO: think about returning a shared_ptr here:
  //       purpose would be to allow the Client to grab a concrete layout
  //       if known while the Message itself can still hold onto a reference
  //       of this layout if needed.
  std::unique_ptr<MessageLayout> getLayout(MessageEnums::Type type);

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
  static void initializeCreatorMap();
  inline static std::unordered_map<MessageEnums::Type, MessageLayoutCreator>
      idToCreatorMap;
};
#endif