#include <unordered_map>
#include <string>
#include <array>
#include <memory>
// #include <functional>
#include <type_traits>

#include "Message.hpp"

class MessageLayout {
 public:
  virtual void loadMessage(Message& m) = 0;
  virtual void print() = 0;
  virtual ~MessageLayout(){};
};

class EmptyMessage : public MessageLayout {
 public:
  void print() override { printf("Empty Message...\n"); }
  void loadMessage(Message& m) { return; }
};

class TestMessage : public MessageLayout {
 public:
  std::array<std::string, 7> pStrings{};
  struct temp {
    uint8_t a;
    uint8_t b;
  } d[5];
  float x;

  void loadMessage(Message& m) override {
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
  // template <typename T>
  std::unique_ptr<MessageLayout> getMessage(uint8_t id, Message& m) {
    // static_assert(std::is_base_of<MessageLayout, T>::value,
    //               "T is not a descendant of MessageLayout");
    // std::unique_ptr<T> t = std::make_unique<T>();
    // t->loadMessage(m);
    // return t;
    switch (id) {
      case 1: {
        std::unique_ptr<TestMessage> t = std::make_unique<TestMessage>();
        t->loadMessage(m);
        return t;
      }
      default:
        break;
    }
    return std::make_unique<EmptyMessage>();
  }

 protected:
  MessageLoader() {}
  static inline MessageLoader* loader = nullptr;

 private:
};
