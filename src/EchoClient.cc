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
  std::array<char, 1> send_buf{'0'};
  socket.send_to(asio::buffer(send_buf), receiver_endpoint);
}

Header<uint8_t> decodeHeader(udp::socket& socket, udp::endpoint& sender_endpoint) {
  std::vector<uint8_t> header_recv_buf(Header<uint8_t>::LENGTH_IN_BYTES);

  // grab the header from the sender
  socket.receive_from(asio::buffer(header_recv_buf), sender_endpoint);
  // reconstruct object from buffer
  Header<uint8_t> h{header_recv_buf};
  h.print();

  std::cout << "Total length of message: " << std::to_string(h.getSize()) << " Bytes"
            << std::endl;
  return h;
}

Message<uint8_t> receiveMessage(udp::socket& socket, udp::endpoint& sender_endpoint) {
  Header<uint8_t> header = decodeHeader(socket, sender_endpoint);

  std::vector<uint8_t> data_recv_buf(header.getSize());
  // grab the data from the sender
  socket.receive_from(asio::buffer(data_recv_buf), sender_endpoint);

  // reconstruct Message object from header + data
  return Message<uint8_t>{header, data_recv_buf};
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
    Message<uint8_t> m = receiveMessage(socket, sender_endpoint);

    m.printBytes();

    // Below is demonstrating that arbitrary data can be restored after send
    struct temp {
      uint8_t a;
      uint8_t b;
    } d[5];
    m.getBytes(d, m.getSize() - 10);
    std::cout << std::to_string(d[0].a) << std::endl;
    std::cout << std::to_string(d[0].b) << std::endl;
    std::cout << std::to_string(d[1].a) << std::endl;
    std::cout << std::to_string(d[1].b) << std::endl;
    std::cout << std::to_string(d[2].a) << std::endl;
    std::cout << std::to_string(d[2].b) << std::endl;
    std::cout << std::to_string(d[3].a) << std::endl;
    std::cout << std::to_string(d[3].b) << std::endl;
    std::cout << std::to_string(d[4].a) << std::endl;
    std::cout << std::to_string(d[4].b) << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}