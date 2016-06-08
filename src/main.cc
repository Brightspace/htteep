#include "stream_processor.h"
#include "stream_manager.h"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <port> <interface>\n";
    exit(-1);
  }

  std::string filter = "tcp port ";
  filter += argv[1];

  StreamManager sm(filter.c_str(), argv[2]);

  sm.Start();
}
