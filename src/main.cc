#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <tins/tcp_ip/stream_follower.h>
#include <tins/sniffer.h>

class Connection {
public:
  void HandleClientData(Tins::TCPIP::Stream& stream) {
    for(auto&& c : stream.client_payload()) {
      std::cout << c;
    }
    std::cout << "\n---\n";
  }
private:
};

class ConnectionManager {
public:
  ConnectionManager(Tins::TCPIP::StreamFollower& follower) {
    follower.new_stream_callback([this](auto& stream) {
      this->HandleNewStream(stream);
    });

    follower.stream_termination_callback([this](auto& stream, auto reason) {
      this->HandleStreamTermination(stream, reason);
    });
  }

  void HandleNewStream(Tins::TCPIP::Stream& stream) {
    std::cout << ":: New stream " << &stream.client_flow() << "\n";

    m_connections[&stream.client_flow()] = std::make_unique<Connection>();

    stream.client_data_callback([&](auto& stream) {
      m_connections[&stream.client_flow()]->HandleClientData(stream);
    });
  }

  void HandleStreamTermination(
    Tins::TCPIP::Stream& stream,
    Tins::TCPIP::StreamFollower::TerminationReason reason
  ) {
    std::cout << ":: Terminated stream " << &stream
              << " for reason " << reason << "\n";

    m_connections.erase(&stream);
  }

private:
  std::map<
    void*,
    std::unique_ptr<Connection>
  > m_connections;
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

  ConnectionManager cm(follower);

  std::cout << "Sniffing port " << args[1]
            << " on interface " << args[2] << "\n";

  sniffer.sniff_loop([&](Tins::Packet& packet) {
    follower.process_packet(packet);
    return true;
  });
}
