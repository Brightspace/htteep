#ifndef _STREAM_MANAGER_H_
#define _STREAM_MANAGER_H_

#include "stream_processor.h"

#include <map>
#include <memory>

namespace Tins {
namespace TCPIP {

class StreamFollower;
class Stream;
class StreamIdentifier;

}
}

class StreamManager {
public:
  StreamManager(Tins::TCPIP::StreamFollower& follower);

private:
  void HandleOpen(Tins::TCPIP::Stream& stream);

  std::map<
    Tins::TCPIP::StreamIdentifier,
    std::unique_ptr<StreamProcessor>
  > m_streams;
};

#endif
