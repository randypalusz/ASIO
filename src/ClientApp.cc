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

Header decodeHeader(udp::socket& socket, udp::endpoint& sender_endpoint) {
  std::vector<uint8_t> header_recv_buf(Header::LENGTH_IN_BYTES);

  // grab the header from the sender
  socket.receive_from(asio::buffer(header_recv_buf), sender_endpoint);
  // reconstruct object from buffer
  Header h{header_recv_buf};
  h.print();
  return h;
}

Message receiveMessage(udp::socket& socket, udp::endpoint& sender_endpoint) {
  Header header = decodeHeader(socket, sender_endpoint);

  std::vector<uint8_t> data_recv_buf(header.getLayoutSize() + header.getSize());
  // grab the data from the sender
  socket.receive_from(asio::buffer(data_recv_buf), sender_endpoint);

  // reconstruct Message object from header + data
  return Message{header, data_recv_buf};
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
    Message m{receiveMessage(socket, sender_endpoint)};

    m.printBytes();
    m.printLayoutBytes();

    // Below is demonstrating that arbitrary data can be restored after send
    struct temp {
      uint8_t a;
      uint8_t b;
    } d[5];
    float x;

    // TODO: hacking this demonstration now - future use will be hard-coded layouts
    //       based on the ID of the message (e.g., if ID is 1, get string (will need to
    //       calc #bytes to read), then int)
    m.getBytes(d, m.getLayoutBytes(7));
    m.getBytes(x, m.getLayoutBytes(8));
    std::cout << "value of d: " << std::endl;
    for (int i = 0; i <= 4; i++) {
      printf("{d[%d].a, d[%d].b} = {%d, %d}\n", i, i, d[i].a, d[i].b);
    }
    printf("value of x: %f", x);

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}