#ifndef MESSAGE_LAYOUT_HPP
#define MESSAGE_LAYOUT_HPP
#include <any>
#include <array>
#include <string>

#include "Message.hpp"
#include "MessageEnums.hpp"

class Message;

class MessageLayout {
 public:
  virtual void loadMessage(const Message& m) = 0;
  virtual MessageType getEnum() = 0;
  virtual std::any getDataLayout(size_t idx) {
    if (!m_layoutInitialized) {
      defineDataLayout();
      m_layoutInitialized = true;
    }
    return m_layout.at(idx);
  };
  virtual void print() = 0;
  virtual ~MessageLayout(){};

 protected:
  std::vector<std::any> m_layout;
  bool m_layoutInitialized = false;

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

#endif