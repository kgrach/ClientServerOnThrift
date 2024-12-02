typedef i64     DWORD;
typedef i64     CARDCONTEXT;
typedef i64     LONG;

struct return_ec {
  1: LONG         retValue,
  2: CARDCONTEXT  cardContext
}

service helloSvc {
   string getMessage(1: string name),
   return_ec EstablishContext(1: DWORD dwScope)
}




