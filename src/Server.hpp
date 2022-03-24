#ifndef SERVER_HPP
#define SERVER_HPP

#include "Message.hpp"
#include "MessageLayout.hpp"
#include "asio.hpp"
#include <string>

using asio::ip::udp;

class Server {
 public:
  Server(int port = 13, const std::string& inputString = "");
  void run();

 private:
  // collects the init signal, returns the message type based on the init byte
  MessageType waitForInitSignal();
  // temp function to build a known message
  // will break out into different messages based on init from Client
  // (currently, Client only sends a single char to initiate send from Server)
  Message buildMessage(MessageType type);
  asio::error_code sendMessage(Message message);

 private:
  asio::io_context m_context{};
  int m_port{};
  std::string m_inputString{""};
  udp::socket m_socket;
  udp::endpoint m_remoteEndpoint;
};

#endif