// https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/tutorial/tutdaytime4.html
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "Message.hpp"
#include "asio.hpp"

using asio::ip::udp;

void sendInit(udp::socket& socket, udp::endpoint& receiver_endpoint) {
  std::string send_buf = "0";
  socket.send_to(asio::buffer(send_buf), receiver_endpoint);
}

Header<int> decodeHeader(udp::socket& socket, udp::endpoint& sender_endpoint) {
  // byte 0 = size
  // bytes 1-99 = unused
  std::vector<uint8_t> header_recv_buf(9);

  socket.receive_from(asio::buffer(header_recv_buf), sender_endpoint);

  printHeader(header_recv_buf);

  // uint16_t len = (header_recv_buf[1] << 8) | header_recv_buf[2];
  std::vector<uint8_t> headerSizeVector{header_recv_buf.begin() + 1,
                                        header_recv_buf.end()};

  uint64_t len = vectorToHeaderSize(headerSizeVector);
  std::cout << "Total length of message: " << std::to_string(len) << " Bytes"
            << std::endl;
  return Header<int>{header_recv_buf[0], len};
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