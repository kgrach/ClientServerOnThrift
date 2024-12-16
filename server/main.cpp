#include "gen-cpp/helloSvc.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

class helloSvcHandler : virtual public helloSvcIf {
 public:
  helloSvcHandler() {
    // Your initialization goes here
  }

  void getMessage(std::string& _return, const std::string& name) {

    _return = std::string("Hello ") + name;
  }

  void EstablishContext(return_ec& _return, const DWORD dwScope) {

    char buf[] = {0x01, 0x02, 0x03, 0x00, 0x00};

    _return.retValue = 111;
    _return.cardContext = 999;
    _return.buffer = std::string(buf, 5);
  }

  void Send(const std::string& arg) {
    // Your implementation goes here
    printf("Send\n");
  }

};


int main (void)
{
  int port = 9090;
  ::std::shared_ptr<helloSvcHandler> handler(new helloSvcHandler());
  ::std::shared_ptr<TProcessor> processor(new helloSvcProcessor(handler));
  ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
