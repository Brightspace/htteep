#ifndef _STREAM_MANAGER_H_
#define _STREAM_MANAGER_H_

#include "stream_processor.h"

#include <string>
#include <memory>

namespace Tins {
namespace TCPIP {

class Stream;

}
}

class StreamManager {
public:
  StreamManager(const char* pcapFilter, const char* interface);
  ~StreamManager();

  void Start();

private:
  void HandleOpen(Tins::TCPIP::Stream& stream);

  struct Impl;

  std::unique_ptr<Impl> m_impl;
};

#endif
