// https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/tutorial/tutdaytime5.html
#include <cstdlib>
#include <functional>
#include <iostream>
#include <thread>

#include "Server.hpp"
#include "Message.hpp"
#include "asio.hpp"

using asio::ip::udp;
int main(int argc, char* argv[]) {
  int port = 13;
  std::string inputString;
  for (int i = 1; i < argc; i++) {
    switch (i) {
      case 1:
        port = atoi(argv[i]);
        break;
      case 2:
        inputString = argv[i];
        break;
      default:
        break;
    }
  }
  asio::io_context io_context;
  Server s{io_context, port, inputString};
  s.run();

  return 0;
}