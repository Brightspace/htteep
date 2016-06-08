#include "stream_processor.h"
#include "stream_manager.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <tins/sniffer.h>

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
