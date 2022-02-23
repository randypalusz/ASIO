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
  virtual ~MessageLayout(){};
};

class TestMessage : public MessageLayout {
 public:
  std::array<std::string, 7> pStrings{};
  struct temp {
    uint8_t a;
    uint8_t b;
  } p2[5];
  float p3;

  void loadMessage(Message& m) override {
    m.getBytes(pStrings[0], 0);
    m.getBytes(pStrings[1], 1);
    m.getBytes(pStrings[2], 2);
    m.getBytes(pStrings[3], 3);
    m.getBytes(pStrings[4], 4);
    m.getBytes(pStrings[5], 5);
    m.getBytes(pStrings[6], 6);
    m.getBytes(p2, 7);
    m.getBytes(p3, 8);
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
  template <typename T>
  std::unique_ptr<T> getMessage(Message& m) {
    static_assert(std::is_base_of<MessageLayout, T>::value,
                  "T is not a descendant of MessageLayout");
    std::unique_ptr<T> t = std::make_unique<T>();
    t->loadMessage(m);
    return t;
  }

 protected:
  MessageLoader() {}
  static inline MessageLoader* loader = nullptr;

 private:
};
