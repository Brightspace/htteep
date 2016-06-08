#include "stream_processor.h"

#include <tins/tcp_ip/stream_follower.h>

#include <iostream>

void StreamProcessor::HandleClientData(Tins::TCPIP::Stream& stream) {
  for(auto&& c : stream.client_payload()) {
    std::cout << c;
  }
}
