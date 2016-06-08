#include "stream_manager.h"

#include <tins/ipv6.h>
#include <tins/sniffer.h>
#include <tins/tcp_ip/stream_follower.h>

struct StreamManager::Impl {
  Tins::SnifferConfiguration config;
  Tins::TCPIP::StreamFollower follower;
  std::unique_ptr<Tins::Sniffer> sniffer;

  std::map<
    Tins::TCPIP::StreamIdentifier,
    std::unique_ptr<StreamProcessor>
  > streams;
};

// TODO: remove once patch lands on master
Tins::TCPIP::StreamIdentifier make_identifier(const Tins::TCPIP::Stream& stream) {
  if (stream.is_v6()) {
    return Tins::TCPIP::StreamIdentifier(
        Tins::TCPIP::StreamIdentifier::serialize(stream.client_addr_v6()), stream.client_port(),
        Tins::TCPIP::StreamIdentifier::serialize(stream.server_addr_v6()), stream.server_port());
  } else {
    return Tins::TCPIP::StreamIdentifier(
        Tins::TCPIP::StreamIdentifier::serialize(stream.client_addr_v4()), stream.client_port(), Tins::TCPIP::StreamIdentifier::serialize(stream.server_addr_v4()), stream.server_port()); } } std::ostream& operator<<(std::ostream& out, const Tins::TCPIP::StreamIdentifier& id) {
  out << "(TODO)";
  return out;
}

StreamManager::StreamManager(const char* pcapFilter, const char* interface) {
  m_impl = std::make_unique<StreamManager::Impl>();

  m_impl->config.set_filter(pcapFilter);

  m_impl->sniffer = std::make_unique<Tins::Sniffer>(interface, m_impl->config);

  m_impl->follower.new_stream_callback([this](auto& stream) {
    this->HandleOpen(stream);
  });
}

StreamManager::~StreamManager() = default;

void StreamManager::Start() {
  std::cout << "Listening...\n";
  m_impl->sniffer->sniff_loop([&](Tins::Packet& packet) {
    m_impl->follower.process_packet(packet);
    return true;
  });
}

void StreamManager::HandleOpen(Tins::TCPIP::Stream& stream) {
  auto id = make_identifier(stream);

  std::cout << "\n:: " << id << ": stream opened\n";

  m_impl->streams[id] = std::make_unique<StreamProcessor>();

  stream.client_data_callback([&](auto& stream) {
    m_impl->streams[id]->HandleClientData(stream);
  });

  stream.stream_closed_callback([&](auto& stream) {
    std::cout << "\n:: " << id << ": stream closed\n";
    m_impl->streams.erase(id);
  });
}
