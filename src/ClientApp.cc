// https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/tutorial/tutdaytime4.html
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "Client.hpp"

int main(int argc, char* argv[]) {
  std::string ip{"localhost"};
  std::string port{"13"};
  // try {
  // if too many args, or ip specified without port, throw error
  if ((argc > 3) || (argc == 2)) {
    std::cerr << "Usage: client <host> <port>" << std::endl;
    return 1;
  }
  for (int i = 1; i < argc; i++) {
    switch (i) {
      case 1:
        ip = argv[i];
        break;
      case 2:
        port = argv[i];
        break;
      default:
        break;
    }
  }

  Client c{ip, port};
  c.run();

  return 0;
}