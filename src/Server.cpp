#include "Server.hpp"
#include "asio.hpp"

Server::Server(int port, const std::string& inputString) try
    : m_socket{m_context, udp::endpoint(udp::v4(), port)},
      m_port{port},
      m_inputString{inputString} {
} catch (std::exception& e) {
  std::cerr << e.what() << std::endl;
}

MessageType Server::waitForInitSignal() {
  std::array<char, 1> recv_buf;
  m_socket.receive_from(asio::buffer(recv_buf), m_remoteEndpoint);
  MessageType type = idToMessageType(recv_buf.at(0));
  return type;
}

asio::error_code Server::sendMessage(Message message) {
  asio::error_code ignored_error;
  // send header
  m_socket.send_to(asio::buffer(message.getHeader()), m_remoteEndpoint, 0, ignored_error);
  // send body
  m_socket.send_to(asio::buffer(message.getBytes()), m_remoteEndpoint, 0, ignored_error);
  return ignored_error;
}

Message Server::buildMessage(MessageType type) {
  switch (type) {
    case MessageType::TEST: {
      Message m(MessageType::TEST);
      m.pushData("hello");
      m.pushData("world");
      m.pushData(m_inputString);
      m.pushData("\n");
      m.pushData("{\n\"this\": 3,\n\"is\": \'a\',\n\"test\": \'hello\'\n}");
      m.pushData(
          "wuyzYHjnKGX5wztdWX25B2QtjxmLhTkDWNmojf40aemRRqk2ih9YvoCREBEBi6Ltvv9Eh93NUkkiWD"
          "SgVxtl2qSPNJocYrJNW661mjS7Z2CjiKnkmGCtV1LrzqlBX9aOWPQ4DAayNDFS9HKgvqJzRAkLOrNJ"
          "EoUUvtgZT9aGwfXtK6FDlEbEi8DmtRo5haqkISNtHPSbf3QtKszH2DJEqqyqP8Bm9thn5V8gW0j9vO"
          "xanGt1o8JOxAs4RincoLlWMl0K3oKXKko3Ojy8CFxh7X8Bn5FkA8B7JdMDQtvVcKG3SXiWf44NxrZP"
          "I1zfI2zNJrAqo1PODtS1lgr7gbobIDqCo09fHphVeKR6a6LmbHOuT7bSlUsMFgPxMhYzaT1bMWnTU4"
          "2vhGbcKBw5oTHx7IF10DrTzhYaKP6jI2K8eXONSOp25Skx3Y13c5pfy0JODXMzNcNdBQQRkURdkl28"
          "jIkxumN0YOcwejaPn8twAJRRCXq7BUj1tGE5ogXv5xIBSJpDGibuumkYjzxWhMI53DlwIPvKTOIQPV"
          "uVUKoj0tSzGgQUQG76DXJRTAvzMgH9osy7KLozhGy2EAYbUq0EWizGOBFYP4W00zk8oQC88SULhDKN"
          "RAestXPLBRsacWKN4drS2QBsZFq2zJatVjajrhstmHQnusauRqiq5zBYaxmh7fEzTx8bjcxGrzpuJn"
          "7WHV721pHC8YJ1wjtUdLWfe7pwYY28nQeRWEjJL1Pc9NjKdKCg6FSaFvakXKVQ35n0QZ00X8LUlLFl"
          "hyA9IYAqSpHPT2REuws2bxUqRwXHvMVssLfhEdQorm71NzKYbSHJ1KAgVWZyEvZMKcmRS2Ji0GvXBb"
          "HmuOaf4jAin696jefbyK8rLRAV6qRzPAxrCRYtvNcIkKh1hsIgl8sQaAsH6XTfTAd0nxNjwnnAeGVR"
          "LuTpSeCZ18iPIzYHOWoPcltujyRVg22viHRYRf4lNtYOCEYAdbBMNeQlzV7gSe6_THEEND");
      m.pushData(
          "{\"name\": \"Gilbert\", \"wins\": [[\"straight\", \"7, Ace\"], [\"one pair\", "
          "\"10, Heart\"]]}");
      struct temp {
        uint8_t a = 3;
        uint8_t b = 4;
      } d[5];
      d[4].b = 5;
      m.pushData(d);
      m.pushData(3.0f);

      m.printHeader();
      m.printBytes();
      m.printLayoutBytes();

      return m;
    }
    case MessageType::EMPTY: {
      Message m(MessageType::EMPTY);
      return m;
    }
    default:
      break;
  }
}

void Server::run() {
  for (;;) {
    MessageType type = waitForInitSignal();
    Message m{buildMessage(type)};
    sendMessage(m);
  }
}