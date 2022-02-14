// https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/tutorial/tutdaytime4.html
#include <cstdlib>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "asio.hpp"
#include "Message.hpp"

using asio::ip::udp;

void sendInit(udp::socket& socket, udp::endpoint& receiver_endpoint) {
  std::string send_buf = "0";
  socket.send_to(asio::buffer(send_buf), receiver_endpoint);
}

Header<int> decodeHeader(udp::socket& socket, udp::endpoint& sender_endpoint) {
  // byte 0 = size
  // bytes 1-99 = unused
  std::vector<uint8_t> header_recv_buf(3);

  socket.receive_from(asio::buffer(header_recv_buf), sender_endpoint);

  std::printf(
      "Header: \n  type: %d\n  high_order_byte: 0x%02X\n  low_order_byte: 0x%02X\n",
      header_recv_buf[0], header_recv_buf[1], header_recv_buf[2]);

  uint16_t len = (header_recv_buf[1] << 8) | header_recv_buf[2];
  std::cout << "Total length of message: " << std::to_string(len) << " Bytes"
            << std::endl;
  return Header<int>{header_recv_buf[0], len};
}

std::string receiveMessage(size_t length, udp::socket& socket,
                           udp::endpoint& sender_endpoint) {
  std::vector<uint8_t> data_recv_buf(length);

  socket.receive_from(asio::buffer(data_recv_buf), sender_endpoint);

  for (char c : data_recv_buf) {
    std::cout << c;
  }
  return std::string(data_recv_buf.begin(), data_recv_buf.end());
}

Message<int> receiveMessage(udp::socket& socket, udp::endpoint& sender_endpoint) {
  Header<int> header = decodeHeader(socket, sender_endpoint);

  std::vector<uint8_t> data_recv_buf(header.size);
  socket.receive_from(asio::buffer(data_recv_buf), sender_endpoint);

  Message<int> m(header.id, header.size, data_recv_buf);
  return m;
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
    Message<int> m = receiveMessage(socket, sender_endpoint);

    m.printBytes();

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}