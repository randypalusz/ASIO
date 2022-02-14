#include <cstdlib>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "asio.hpp"
#include "Header.hpp"

using asio::ip::udp;

void sendInit(udp::socket& socket, udp::endpoint& receiver_endpoint) {
  std::string send_buf = "0";
  socket.send_to(asio::buffer(send_buf), receiver_endpoint);
}

size_t decodeHeader(udp::socket& socket, udp::endpoint& sender_endpoint) {
  // byte 0 = size
  // bytes 1-99 = unused
  std::vector<char32_t> header_recv_buf(Header::lengthInBytes);
  socket.receive_from(asio::buffer(header_recv_buf), sender_endpoint);

  std::cout << "Header: ";
  for (char32_t c : header_recv_buf) {
    std::cout << c;
  }
  std::cout << std::endl;

  size_t len = header_recv_buf[0];
  std::cout << "Total length of message: " << len << " Bytes" << std::endl;
  return len;
}

std::string receiveMessage(size_t lengthInBytes, udp::socket& socket,
                           udp::endpoint& sender_endpoint) {
  std::vector<char> data_recv_buf(lengthInBytes);

  socket.receive_from(asio::buffer(data_recv_buf), sender_endpoint);

  for (char c : data_recv_buf) {
    std::cout << c;
  }

  return std::string(data_recv_buf.begin(), data_recv_buf.end());
}

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

    sendInit(socket, receiver_endpoint);

    udp::endpoint sender_endpoint;

    size_t len = decodeHeader(socket, sender_endpoint);
    std::string message = receiveMessage(len, socket, sender_endpoint);

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}