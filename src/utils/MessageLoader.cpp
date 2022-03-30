#include "MessageLoader.hpp"

MessageLoader* MessageLoader::getInstance() {
  if (loader == nullptr) {
    loader = new MessageLoader();
  }
  return loader;
}

std::unique_ptr<MessageLayout> MessageLoader::getMessage(Message& m) {
  MessageEnums::Type type = m.getType();
  try {
    auto fn = idToCreatorMap.at(type);
    auto ptr = fn();
    ptr->loadDataFromMessage(m);
    return ptr;
  } catch (std::out_of_range& e) {
    std::cerr << "Could not find message ID: " << MessageEnums::typeToId(type)
              << " in idToCreatorMap, returning "
                 "EmptyMessageLayout..."
              << std::endl;
    return std::make_unique<EmptyMessageLayout>();
  }
}