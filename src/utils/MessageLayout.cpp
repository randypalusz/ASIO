#include "MessageLayout.hpp"

void MessageLayout::constructMessage(Message& m) {
  assert(m.getType() == getEnum());
  pushLayoutDataIntoMessage(m);
}

void TestMessageLayout::loadDataFromMessage(const Message& m) {
  for (int i = 0; i <= 6; i++) {
    m.getBytes(pStrings.at(i), i);
  }
  m.getBytes(d, 7);
  m.getBytes(x, 8);
}

void TestMessageLayout::pushLayoutDataIntoMessage(Message& m) {
  for (int i = 0; i <= 6; i++) {
    m.pushData(pStrings.at(i));
  }
  m.pushData(d);
  m.pushData(x);
}