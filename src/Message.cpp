#include "Message.hpp"

Message::Message(Header header, const std::vector<uint8_t>& body_recv_buf)
    : m_header(header) {
  if (body_recv_buf.size() == 0) return;
  packLayoutBytes(body_recv_buf);
  m_data.insert(m_data.end(), body_recv_buf.begin() + header.getLayoutSize(),
                body_recv_buf.end());
}