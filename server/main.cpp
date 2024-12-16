#include "gen-cpp/helloSvc.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

#include <PCSC/winscard.h>

class helloSvcHandler : virtual public helloSvcIf {
 public:
  helloSvcHandler() {
    // Your initialization goes here
  }

  void getMessage(std::string& _return, const std::string& name) {

    _return = std::string("Hello ") + name;
  }

  void Send(const std::string& arg) {
    // Your implementation goes here
    printf("Send\n");
  }

  void EstablishContext(return_ec& _return, const DWORD_RPC dwScope) {

   	SCARDCONTEXT hContext;

    printf ("Server received SCardEstablishContext dwScope: %ld\n", dwScope);

    LONG rv = SCardEstablishContext(dwScope, NULL, NULL, &hContext);

    printf ("SCardEstablishContext return %ld, Server send hContext: %ld\n", rv, hContext);

    _return.cardContext = hContext;
    _return.retValue = rv;
  }

  void ListReaders(return_lr& _return, const SCARDCONTEXT_RPC hContext, const DWORD_RPC pcchReaders) {
    // Your implementation goes here
    LPSTR szReaderName = NULL;
    DWORD szReaderNameLen = pcchReaders;

    std::string readerBuf;

    printf ("Server received SCardListReaders: SCARDCONTEXT=%ld\n", hContext);

    if(SCARD_AUTOALLOCATE != szReaderNameLen) {
      readerBuf.resize(szReaderNameLen);
      szReaderName = (char*)readerBuf.data();
    }

    LONG rv = SCardListReaders(hContext, NULL, (readerBuf.empty() ? (LPSTR)&szReaderName : szReaderName), &szReaderNameLen);

    printf ("SCardListReaders return %ld, Server send list readers=%s\n", rv, szReaderName);

    _return.retValue = rv;
    _return.mszReaders = std::string(szReaderName, szReaderNameLen);

    if(SCARD_AUTOALLOCATE == pcchReaders) {
      SCardFreeMemory(hContext, szReaderName);
    }

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
