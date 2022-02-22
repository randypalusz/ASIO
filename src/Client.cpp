#include "Client.hpp"

Client::Client(const std::string& ip, const std::string& port) try : m_socket{m_context} {
  udp::resolver resolver(m_context);
  m_server = *resolver.resolve(udp::v4(), ip, port).begin();
  // TODO: should this be opened here or on sendInit?
  m_socket.open(udp::v4());
} catch (std::exception& e) {
  std::cerr << e.what() << std::endl;
}

void Client::sendInit() {
  std::array<char, 1> send_buf{'0'};
  m_socket.send_to(asio::buffer(send_buf), m_server);
}

Header Client::decodeHeader() {
  std::vector<uint8_t> header_recv_buf(Header::LENGTH_IN_BYTES);

  // grab the header from the sender
  m_socket.receive_from(asio::buffer(header_recv_buf), m_client);
  // reconstruct object from buffer
  Header h{header_recv_buf};
  h.print();
  return h;
}

Message Client::receiveMessage() {
  Header header = decodeHeader();

  std::vector<uint8_t> data_recv_buf(header.getLayoutSize() + header.getSize());
  // grab the data from the sender
  m_socket.receive_from(asio::buffer(data_recv_buf), m_client);

  // reconstruct Message object from header + data
  return Message{header, data_recv_buf};
}

void Client::run() {
  try {
    sendInit();

    udp::endpoint sender_endpoint;
    Message m{receiveMessage()};

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
}