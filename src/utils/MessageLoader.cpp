#include "MessageLoader.hpp"

MessageLoader* MessageLoader::getInstance() {
  if (loader == nullptr) {
    loader = new MessageLoader();
  }
  return loader;
}

std::unique_ptr<MessageLayout> MessageLoader::getMessage(Message& m) {
  MessageType type = m.getType();
  try {
    auto fn = idToCreatorMap.at(type);
    auto ptr = fn();
    ptr->loadMessage(m);
    return ptr;
  } catch (std::out_of_range& e) {
    std::cerr
        << "Could not find message type in idToCreatorMap, returning EmptyMessage..."
        << std::endl;
    return std::make_unique<EmptyMessage>();
  }
}