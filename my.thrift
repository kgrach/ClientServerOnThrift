typedef i64     DWORD;
typedef i64     SCARDCONTEXT;
typedef i64     LONG_RPC;
typedef binary  LPBYTE_RPC

struct return_ec {
  1: LONG_RPC       retValue
  2: SCARDCONTEXT   cardContext
  3: LPBYTE_RPC     buffer
}

struct return_lr {
  1: LONG_RPC       retValue
  2: LPBYTE_RPC     mszReaders
}

service helloSvc {
   string getMessage(1: string name),
   return_ec EstablishContext(1: DWORD dwScope)
   void Send(1:binary arg)

  return_lr   ListReaders(1: SCARDCONTEXT_RPC hContext, 2: DWORD_RPC pcchReaders)
}




