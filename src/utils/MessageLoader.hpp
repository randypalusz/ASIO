#include <any>
#include <array>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "Message.hpp"
#include "Utility.hpp"

class MessageLayout {
 public:
  virtual void loadMessage(const Message& m) = 0;
  virtual MessageType getEnum() = 0;
  virtual std::any getDataLayout(size_t idx) { return m_layout.at(idx); };
  virtual void print() = 0;
  virtual ~MessageLayout(){};

 protected:
  std::vector<std::any> m_layout;

 private:
  virtual void defineDataLayout() = 0;
};

class EmptyMessage : public MessageLayout {
 public:
  MessageType getEnum() override { return MessageType::EMPTY; }
  void print() override { printf("Empty Message...\n"); }
  std::any getDataLayout(size_t idx) override { return 0; }
  void loadMessage(const Message& m) override {
    // Message should contain nothing, so load nothing...
    return;
  }

 protected:
  void defineDataLayout() override {}
};

class TestMessage : public MessageLayout {
 public:
  MessageType getEnum() override { return MessageType::TEST; }
  // TODO: tightly couple the ordering here with the actual ordering of the message data
  // when creating a new message server-side
  std::array<std::string, 7> pStrings{};
  struct temp {
    uint8_t a;
    uint8_t b;
  } d[5];
  float x;

  void loadMessage(const Message& m) override {
    for (int i = 0; i <= 6; i++) {
      m.getBytes(pStrings.at(i), i);
    }
    // m.getBytes(pStrings[0], 0);
    // m.getBytes(pStrings[1], 1);
    // m.getBytes(pStrings[2], 2);
    // m.getBytes(pStrings[3], 3);
    // m.getBytes(pStrings[4], 4);
    // m.getBytes(pStrings[5], 5);
    // m.getBytes(pStrings[6], 6);
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

 private:
  void defineDataLayout() override {
    m_layout.clear();
    m_layout.push_back(pStrings.at(0));
    m_layout.push_back(pStrings.at(1));
    m_layout.push_back(pStrings.at(2));
    m_layout.push_back(pStrings.at(3));
    m_layout.push_back(pStrings.at(4));
    m_layout.push_back(pStrings.at(5));
    m_layout.push_back(pStrings.at(6));
    m_layout.push_back(d);
    m_layout.push_back(x);
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
