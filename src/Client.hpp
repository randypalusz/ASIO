#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Header.hpp"
#include "Message.hpp"
#include "asio.hpp"

using asio::ip::udp;

class Client {
 public:
  Client(const std::string& ip = "localhost", const std::string& port = "13");
  void run();

 private:
  void sendInit();
  Header decodeHeader();
  Message receiveMessage();

 private:
  asio::io_context m_context{};
  udp::socket m_socket;
  udp::endpoint m_server;
  udp::endpoint m_client;
};

#endif