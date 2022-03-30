#include "Client.hpp"
#include "MessageLoader.hpp"
#include <memory>

Client::Client(const std::string& ip, const std::string& port) try : m_socket{m_context} {
  udp::resolver resolver(m_context);
  m_server = *resolver.resolve(udp::v4(), ip, port).begin();
  // TODO: should this be opened here or on sendInit?
  // m_socket.open(udp::v4());
} catch (std::exception& e) {
  std::cerr << e.what() << std::endl;
}

void Client::sendInit(MessageEnums::Type type) {
  m_socket.open(udp::v4());
  std::array<char, 1> send_buf{};
  send_buf.at(0) = static_cast<char>(MessageEnums::typeToId(type));
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

  m_socket.close();
  // reconstruct Message object from header + data
  return Message{header, data_recv_buf};
}

void Client::run() {
  try {
    // MessageLoader* loader = MessageLoader::getInstance();
    sendInit(MessageEnums::Type::TEST);
    Message m{receiveMessage()};

    sendInit(MessageEnums::Type::EMPTY);
    Message m2{receiveMessage()};

    // m.printBytes();
    // m.printLayoutBytes();

    // unique_ptr
    // auto data = loader->getMessage(m);
    // data->print();

    // printf("\nTesting empty message:\n");
    // sendInit(MessageEnums::Type::EMPTY);
    // Message m2{receiveMessage()};
    // auto data2 = loader->getMessage(m2);
    // data2->print();

    // printf("\nTesting loading Message directly into layout\n");
    // sendInit(MessageEnums::Type::TEST);
    // Message m3{receiveMessage()};
    // TestMessageLayout t{};
    // t.loadDataFromMessage(m3);
    // t.print();

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}