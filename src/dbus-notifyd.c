//D-Bus Notification Daemon
//
// Raspbian compilation, run from repo root directory:
// gcc src/dbus-notifyd.c src/dbus-print-message.c src/tool-common.c -o dbus-notifyd -I /usr/include/dbus-1.0 -I /usr/lib/arm-linux-gnueabihf/dbus-1.0/include -I /usr/lib/dbus-1.0/include -l dbus-1
//
// Cent 7 compilation, run from repo root directory:
// gcc src/dbus-notifyd.c src/dbus-print-message.c src/tool-common.c -o dbus-notifyd -I /usr/include/dbus-1.0 -I /usr/lib64/dbus-1.0/include -l dbus-1

//#include <config.h>

//#include "dbus/dbus-internals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>

#include <time.h>

#include "dbus-print-message.h"
#include "tool-common.h"

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

//~ char * match;
//~ char * command;

typedef struct{
  char *match;
  char *command;
} option_t;

static DBusHandlerResult
monitor_filter_func (DBusConnection     *connection,
                     DBusMessage        *message,
                     void               *user_data)
{
  //~ long sec = 0, usec = 0;
  //~ _dbus_get_real_time (&sec, &usec);
  
  option_t *opt_in = user_data;

  print_message (message, opt_in->match, opt_in->command);
  
  if (dbus_message_is_signal (message,
                              DBUS_INTERFACE_LOCAL,
                              "Disconnected"))
    exit (0);

  /* Monitors must not allow libdbus to reply to messages, so we eat
   * the message. See bug 1719.
   */
  return DBUS_HANDLER_RESULT_HANDLED;
}

static dbus_bool_t
become_monitor (DBusConnection *connection,
    int numFilters,
    const char * const *filters)
{
  DBusError error = DBUS_ERROR_INIT;
  DBusMessage *m;
  DBusMessage *r;
  int i;
  dbus_uint32_t zero = 0;
  DBusMessageIter appender, array_appender;

  m = dbus_message_new_method_call (DBUS_SERVICE_DBUS,
      DBUS_PATH_DBUS, DBUS_INTERFACE_MONITORING, "BecomeMonitor");
      
  if (m == NULL)
    tool_oom ("becoming a monitor");

  dbus_message_iter_init_append (m, &appender);

  if (!dbus_message_iter_open_container (&appender, DBUS_TYPE_ARRAY, "s",
        &array_appender))
    tool_oom ("opening string array");

  for (i = 0; i < numFilters; i++)
    {
      if (!dbus_message_iter_append_basic (&array_appender, DBUS_TYPE_STRING,
            &filters[i]))
        tool_oom ("adding filter to array");
    }

  if (!dbus_message_iter_close_container (&appender, &array_appender) ||
      !dbus_message_iter_append_basic (&appender, DBUS_TYPE_UINT32, &zero))
    tool_oom ("finishing arguments");

  r = dbus_connection_send_with_reply_and_block (connection, m, -1, &error);

  if (r != NULL)
    {
      dbus_message_unref (r);
    }
  else if (dbus_error_has_name (&error, DBUS_ERROR_UNKNOWN_INTERFACE))
    {
      fprintf (stderr, "dbus-monitor: unable to enable new-style monitoring, "
          "your dbus-daemon is too old. Falling back to eavesdropping.\n");
      dbus_error_free (&error);
    }
  else
    {
      fprintf (stderr, "dbus-monitor: unable to enable new-style monitoring: "
          "%s: \"%s\". Falling back to eavesdropping.\n",
          error.name, error.message);
      dbus_error_free (&error);
    }

  dbus_message_unref (m);

  return (r != NULL);
}

int
main (int argc, char *argv[])
{
  DBusConnection *connection;
  DBusError error;
  DBusBusType type = DBUS_BUS_SESSION;
  //~ match = "Inbound Call";
  //~ command = "/etc/dbus-notifyd/dbus-notifyd_command.sh";
  DBusHandleMessageFunction filter_func = monitor_filter_func;
  char *address = NULL;
  dbus_bool_t seen_bus_type = FALSE;
  //~ BinaryMode binary_mode = BINARY_MODE_NOT;
  int i = 0, j = 0, numFilters = 0;
  //~ char **filters = NULL;
  numFilters = 1;
  char *filter_string = "eavesdrop=true,type='method_call',interface='org.freedesktop.Notifications',member='Notify'";
  char * filters[] = {filter_string};
  
  option_t options = {"Inbound Call", "/etc/dbus-notifyd/dbus-notifyd_command.sh"};
  option_t *options_p = &options;

#ifdef DBUS_WIN
  setvbuf (stdout, NULL, _IONBF, 0);
#else
  setvbuf (stdout, NULL, _IOLBF, 0);
#endif

  dbus_error_init (&error);
  
  connection = dbus_bus_get (type, &error);
  if (connection == NULL)
    {
      const char *where;
      if (address != NULL)
        where = address;
      else
        {
          switch (type)
            {
            case DBUS_BUS_SYSTEM:
              where = "system bus";
              break;
            case DBUS_BUS_SESSION:
              where = "session bus";
              break;
            case DBUS_BUS_STARTER:
            default:
              /* We don't set type to anything else */
              //_dbus_assert_not_reached ("impossible bus type");
              ;
            }
        }
      fprintf (stderr, "Failed to open connection to %s: %s\n",
               where,
               error.message);
      dbus_error_free (&error);
      exit (1);
    }

  dbus_connection_set_route_peer_messages (connection, TRUE);

  if (!dbus_connection_add_filter (connection, filter_func,
                                   //~ _DBUS_INT_TO_POINTER (binary_mode), NULL))
                                   options_p, NULL))
    {
      fprintf (stderr, "Couldn't add filter!\n");
      exit (1);
    }

  if (become_monitor (connection, numFilters,
                      (const char * const *) filters))
    {
      /* no more preparation needed */
    }
  //~ else if (numFilters)
    //~ {
      //~ size_t offset = 0;
      //~ for (i = 0; i < j; i++)
        //~ {
          //~ dbus_bus_add_match (connection, filters[i] + offset, &error);
          //~ if (dbus_error_is_set (&error) && i == 0 && offset == 0)
            //~ {
              //~ /* We might be talking to a pre-1.5.6 dbus-daemon
              //~ * which wouldn't understand eavesdrop=true.
              //~ * If this works, carry on with offset > 0
              //~ * on the remaining iterations. */
              //~ offset = strlen (EAVESDROPPING_RULE) + 1;
              //~ dbus_error_free (&error);
              //~ dbus_bus_add_match (connection, filters[i] + offset, &error);
            //~ }

          //~ if (dbus_error_is_set (&error))
            //~ {
              //~ fprintf (stderr, "Failed to setup match \"%s\": %s\n",
                       //~ filters[i], error.message);
              //~ dbus_error_free (&error);
              //~ exit (1);
            //~ }
          //~ free(filters[i]);
        //~ }
    //~ }
  //~ else
    //~ {
      //~ dbus_bus_add_match (connection,
                          //~ EAVESDROPPING_RULE,
                          //~ &error);
      //~ if (dbus_error_is_set (&error))
        //~ {
          //~ dbus_error_free (&error);
          //~ dbus_bus_add_match (connection,
                              //~ "",
                              //~ &error);
          //~ if (dbus_error_is_set (&error))
            //~ goto lose;
        //~ }
    //~ }

  while (dbus_connection_read_write_dispatch(connection, -1))
    ;
  exit (0);
 lose:
  fprintf (stderr, "Error: %s\n", error.message);
  exit (1);
}