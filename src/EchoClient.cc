#include <cstdlib>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "asio.hpp"

using asio::ip::udp;

// int main() {
//   try {
//     asio::io_context io_context;
//     udp::socket socket(io_context, udp::endpoint(udp::v4(), 80));
//     socket.
//   }
// }

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint =
        *resolver.resolve(udp::v4(), argv[1], "daytime").begin();

    udp::socket socket(io_context);
    socket.open(udp::v4());

    // std::array<char, 1> send_buf = {{0}};
    std::string send_buf = "0";
    socket.send_to(asio::buffer(send_buf), receiver_endpoint);

    // std::array<char, 128> recv_buf;

    std::cout << "press enter to initiate receiving" << std::endl;
    std::cin.get();

    while (socket.available()) {
      std::vector<char> recv_buf(socket.available());
      udp::endpoint sender_endpoint;
      size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);

      // std::cout.write(recv_buf.data(), len);
      for (char c : recv_buf) {
        std::cout << c;
      }
      std::cout << std::endl;
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}