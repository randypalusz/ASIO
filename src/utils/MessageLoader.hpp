#include <unordered_map>
#include <string>
#include <array>
#include <memory>
#include <functional>
#include <type_traits>

#include "Message.hpp"
#include "Utility.hpp"

class MessageLayout {
 public:
  virtual void loadMessage(const Message& m) = 0;
  virtual MessageType getEnum() = 0;
  virtual void print() = 0;
  virtual ~MessageLayout(){};
};

class EmptyMessage : public MessageLayout {
 public:
  MessageType getEnum() override { return MessageType::EMPTY; }
  void print() override { printf("Empty Message...\n"); }
  void loadMessage(const Message& m) {
    // Message should contain nothing, so load nothing...
    return;
  }
};

class TestMessage : public MessageLayout {
 public:
  MessageType getEnum() override { return MessageType::TEST; }
  std::array<std::string, 7> pStrings{};
  struct temp {
    uint8_t a;
    uint8_t b;
  } d[5];
  float x;

  void loadMessage(const Message& m) override {
    m.getBytes(pStrings[0], 0);
    m.getBytes(pStrings[1], 1);
    m.getBytes(pStrings[2], 2);
    m.getBytes(pStrings[3], 3);
    m.getBytes(pStrings[4], 4);
    m.getBytes(pStrings[5], 5);
    m.getBytes(pStrings[6], 6);
    m.getBytes(d, 7);
    m.getBytes(x, 8);
  }

  void print() override {
    printf("TestMessage: \n Strings:\n");
    for (int i = 0; i < pStrings.size(); i++) {
      printf("  pStrings[%d]: %s\n", i, pStrings[i].c_str());
    }
    printf("-------------------------------------------\n");
    for (int i = 0; i <= 4; i++) {
      printf("{d[%d].a, d[%d].b} = {%d, %d}\n", i, i, d[i].a, d[i].b);
    }

    printf("x: %f\n", x);
  }
};

class MessageLoader {
 public:
  MessageLoader(MessageLoader& other) = delete;
  void operator=(const MessageLoader&) = delete;

  static inline MessageLoader* getInstance() {
    if (loader == nullptr) {
      loader = new MessageLoader();
    }
    return loader;
  }
  std::unique_ptr<MessageLayout> getMessage(Message& m) {
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
