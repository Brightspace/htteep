#include "stream_manager.h"

#include <tins/ipv6.h>
#include <tins/tcp_ip/stream_follower.h>

// TODO: remove once patch lands on master
Tins::TCPIP::StreamIdentifier make_identifier(const Tins::TCPIP::Stream& stream) {
  if (stream.is_v6()) {
    return Tins::TCPIP::StreamIdentifier(
        Tins::TCPIP::StreamIdentifier::serialize(stream.client_addr_v6()), stream.client_port(),
        Tins::TCPIP::StreamIdentifier::serialize(stream.server_addr_v6()), stream.server_port());
  } else {
    return Tins::TCPIP::StreamIdentifier(
        Tins::TCPIP::StreamIdentifier::serialize(stream.client_addr_v4()), stream.client_port(),
        Tins::TCPIP::StreamIdentifier::serialize(stream.server_addr_v4()), stream.server_port());
  }
}

std::ostream& operator<<(std::ostream& out, const Tins::TCPIP::StreamIdentifier& id) {
  out << "(TODO)";
  return out;
}

StreamManager::StreamManager(Tins::TCPIP::StreamFollower& follower) {
  follower.new_stream_callback([this](auto& stream) {
    this->HandleOpen(stream);
  });
}

void StreamManager::HandleOpen(Tins::TCPIP::Stream& stream) {
  auto id = make_identifier(stream);

  std::cout << "\n:: " << id << ": stream opened\n";

  m_streams[id] = std::make_unique<StreamProcessor>();

  stream.client_data_callback([&](auto& stream) {
    m_streams[id]->HandleClientData(stream);
  });

  stream.stream_closed_callback([&](auto& stream) {
    std::cout << "\n:: " << id << ": stream closed\n";
    m_streams.erase(id);
  });
}
