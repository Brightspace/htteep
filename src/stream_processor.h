#ifndef _STREAM_PROCESSOR_H_
#define _STREAM_PROCESSOR_H_

namespace Tins {
namespace TCPIP {

class Stream;

}
}

class StreamProcessor {
public:
  void HandleClientData(Tins::TCPIP::Stream& stream);

private:
};

#endif

