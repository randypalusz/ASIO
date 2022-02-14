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

void sendHeader(udp::socket& socket, udp::endpoint& endpoint, std::string& message) {
  // byte 0 = size
  // bytes 1-99 = other info (unused)
  std::array<char32_t, Header::lengthInBytes> buffer;
  buffer.fill({});
  size_t sizeInBytes = message.size();
  buffer[0] = sizeInBytes;

  asio::error_code ignored_error;
  socket.send_to(asio::buffer(buffer), endpoint, 0, ignored_error);
}

asio::error_code sendMessage(udp::socket& socket, udp::endpoint& endpoint,
                             std::string message) {
  sendHeader(socket, endpoint, message);

  asio::error_code ignored_error;
  socket.send_to(asio::buffer(message), endpoint, 0, ignored_error);
}

std::string make_daytime_string() {
  std::time_t now = std::time(0);
  return std::ctime(&now);
}

// TODO: implement sending a header of fixed size that contains the size info
int main() {
  try {
    asio::io_context io_context;

    udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

    for (;;) {
      udp::endpoint remote_endpoint;

      waitForInitSignal(socket, remote_endpoint);

      sendMessage(socket, remote_endpoint, make_daytime_string());
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}