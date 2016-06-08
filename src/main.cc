#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <tins/ipv6.h>
#include <tins/tcp_ip/stream_follower.h>
#include <tins/sniffer.h>

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

class StreamProcessor {
public:
  void HandleClientData(Tins::TCPIP::Stream& stream) {
    for(auto&& c : stream.client_payload()) {
      std::cout << c;
    }
  }
private:
};

std::ostream& operator<<(std::ostream& out, const Tins::TCPIP::StreamIdentifier& id) {
  out << "(TODO)";
  return out;
}

class StreamManager {
public:
  StreamManager(Tins::TCPIP::StreamFollower& follower) {
    follower.new_stream_callback([this](auto& stream) {
      this->HandleOpen(stream);
    });
  }

private:
  void HandleOpen(Tins::TCPIP::Stream& stream) {
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

  std::map<
    Tins::TCPIP::StreamIdentifier,
    std::unique_ptr<StreamProcessor>
  > m_streams;
};


int main(int argc, char** argv) {
  auto args = std::vector<std::string>(argv, argv + argc);

  if (args.size() != 3) {
    std::cout << "Usage: " << args[0] << " <port> <interface>\n";
    exit(-1);
  }

  Tins::SnifferConfiguration config;
  config.set_filter("tcp port " + args[1]);

  Tins::Sniffer sniffer(argv[2], config);

  Tins::TCPIP::StreamFollower follower;

  StreamManager sm(follower);

  std::cout << "Sniffing port " << args[1]
            << " on interface " << args[2] << "\n";

  sniffer.sniff_loop([&](Tins::Packet& packet) {
    follower.process_packet(packet);
    return true;
  });
}
