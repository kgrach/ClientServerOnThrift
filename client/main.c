#include "gen-c_glib/hello_svc.h"

#include <thrift/c_glib/transport/thrift_transport.h>
#include <thrift/c_glib/transport/thrift_socket.h>
#include <thrift/c_glib/transport/thrift_buffered_transport.h>
#include <thrift/c_glib/protocol/thrift_binary_protocol.h>

#include <glib-object.h>
#include <glib.h>

#include <stdio.h>

int main (void) {

  ThriftSocket *socket;
  ThriftTransport *transport;
  ThriftProtocol *protocol;

  helloSvcIf *client;

  gboolean success;
  GError *error = NULL;

#if (!GLIB_CHECK_VERSION (2, 36, 0))
  g_type_init ();
#endif

  socket    = g_object_new (THRIFT_TYPE_SOCKET,
                            "hostname",  "192.168.1.124",
                            "port",      9090,
                            NULL);
  transport = g_object_new (THRIFT_TYPE_BUFFERED_TRANSPORT,
                            "transport", socket,
                            NULL);
  protocol  = g_object_new (THRIFT_TYPE_BINARY_PROTOCOL,
                            "transport", transport,
                            NULL);

  client = g_object_new (TYPE_HELLO_SVC_CLIENT,
                         "input_protocol",  protocol,
                         "output_protocol", protocol,
                         NULL);

  success = thrift_transport_open(transport, &error);
  
  if(success) {

    gchar *message = NULL;

    if(success && hello_svc_if_get_message (client, &message, "world!", &error)) {
      printf("Server reply: %s\n", message);
      g_free (message);
    }

    long retVal, context;
    return_ec *ret_ec = g_object_new(TYPE_RETURN_EC, NULL);
    
    if (success && hello_svc_if_establish_context(client, &ret_ec, 0x0002, &error)) {


      g_object_get(ret_ec,
                   "retValue", &retVal,
                   "cardContext", &context,
                   NULL);

      printf("Server reply: retValue=%ld, cardContext=%ld\n", retVal, context);                   
    }

    g_object_unref(ret_ec);

    return_lr *ret_lr = g_object_new(TYPE_RETURN_LR, NULL);

    if (success && hello_svc_if_list_readers(client, &ret_lr, context, -1, &error)) {

      long retVal, context;
      GByteArray *buf;
      g_object_get(ret_lr,
                   "retValue", &retVal,
                   "mszReaders", &buf,
                   NULL);

      printf("Server reply: retValue=%ld\n", retVal);                   
    }

    g_object_unref(ret_lr);

    if (!success) {
      
      fprintf (stderr, "Client caught an exception: %s\n", error->message);
      g_clear_error (&error);
    }
  }

  thrift_transport_close (transport, &error);
  success = success && (error == NULL);

  g_clear_error (&error);

  g_object_unref (client);

  g_object_unref (protocol);
  g_object_unref (transport);
  g_object_unref (socket);

  return (success ? 0 : 1);
}