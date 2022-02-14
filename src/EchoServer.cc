#include <cstdlib>
#include <functional>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "Header.hpp"

using asio::ip::udp;

void waitForInitSignal(udp::socket& socket, udp::endpoint& endpoint) {
  std::array<char, 1> recv_buf;
  socket.receive_from(asio::buffer(recv_buf), endpoint);
}

std::string make_daytime_string() {
  using namespace std;  // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

// TODO: implement sending a header of fixed size that contains the size info
int main() {
  try {
    asio::io_context io_context;

    udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

    for (;;) {
      udp::endpoint remote_endpoint;

      waitForInitSignal(socket, remote_endpoint);

      std::string message = make_daytime_string();

      asio::error_code ignored_error;
      socket.send_to(asio::buffer(message), remote_endpoint, 0, ignored_error);
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}