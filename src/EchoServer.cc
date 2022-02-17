// https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio/tutorial/tutdaytime5.html
#include <cstdlib>
#include <functional>
#include <iostream>
#include <thread>

#include "Message.hpp"
#include "asio.hpp"

using asio::ip::udp;

void waitForInitSignal(udp::socket& socket, udp::endpoint& endpoint) {
  std::array<char, 1> recv_buf;
  socket.receive_from(asio::buffer(recv_buf), endpoint);
}

template <typename T>
asio::error_code sendMessage(udp::socket& socket, udp::endpoint& endpoint,
                             Message<T> message) {
  asio::error_code ignored_error;
  // std::vector<uint8_t> temp = message.getBytes();
  // send header
  socket.send_to(asio::buffer(message.getHeader()), endpoint, 0, ignored_error);
  // send body
  socket.send_to(asio::buffer(message.getBytes()), endpoint, 0, ignored_error);
  return ignored_error;
}

std::string make_daytime_string(const std::string& addition = "") {
  std::time_t now = std::time(0);
  std::string s(std::ctime(&now));
  s = s + addition;
  return s;
}

int main(int argc, char* argv[]) {
  int port = 13;
  std::string inputString;
  for (int i = 1; i < argc; i++) {
    switch (i) {
      case 1:
        port = atoi(argv[i]);
        break;
      case 2:
        inputString = argv[i];
        break;
      default:
        break;
    }
  }

  try {
    asio::io_context io_context;

    udp::socket socket(io_context, udp::endpoint(udp::v4(), port));

    for (;;) {
      udp::endpoint remote_endpoint;

      waitForInitSignal(socket, remote_endpoint);

      Message<uint8_t> m(1);
      m.pushData("hello");
      m.pushData("world");
      m.pushData(inputString);
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

      m.printHeader();
      m.printBytes();
      m.printLayoutBytes();
      sendMessage(socket, remote_endpoint, m);
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}