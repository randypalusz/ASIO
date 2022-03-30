#ifndef MESSAGE_LAYOUT_HPP
#define MESSAGE_LAYOUT_HPP
#include <array>
#include <cassert>
#include <string>

#include "Message.hpp"
#include "MessageEnums.hpp"

class MessageLayout {
 public:
  virtual void loadDataFromMessage(const Message& m) = 0;
  virtual void constructMessage(Message& m) {
    assert(m.getType() == getEnum());
    pushLayoutDataIntoMessage(m);
  }
  virtual MessageEnums::Type getEnum() = 0;
  virtual void print() = 0;
  virtual ~MessageLayout(){};

 private:
  virtual void pushLayoutDataIntoMessage(Message& m) = 0;
};

class EmptyMessageLayout : public MessageLayout {
 public:
  MessageEnums::Type getEnum() override { return MessageEnums::Type::EMPTY; }
  void print() override { printf("Empty Message...\n"); }
  void loadDataFromMessage(const Message& m) override {
    // Message should contain nothing, so load nothing...
    return;
  }

 private:
  void pushLayoutDataIntoMessage(Message& m) override { return; }
};

class TestMessageLayout : public MessageLayout {
 public:
  MessageEnums::Type getEnum() override { return MessageEnums::Type::TEST; }
  std::array<std::string, 7> pStrings{};
  struct temp {
    uint8_t a;
    uint8_t b;
  } d[5];
  float x;

  void loadDataFromMessage(const Message& m) override {
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
  void pushLayoutDataIntoMessage(Message& m) override {
    for (int i = 0; i <= 6; i++) {
      m.pushData(pStrings.at(i));
    }
    m.pushData(d);
    m.pushData(x);
  }
};

#endif