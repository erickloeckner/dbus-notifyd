/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/* dbus-print-message.h  Utility function to print out a message
 *
 * Copyright (C) 2003 Philip Blundell <philb@gnu.org>
 * Copyright (C) 2003 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "dbus-print-message.h"

#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "tool-common.h"

static int
print_iter (DBusMessageIter *iter, char * match)
{
  int flag = 0;
  
  do
    {
      int type = dbus_message_iter_get_arg_type (iter);

      if (type == DBUS_TYPE_INVALID)
        break;

      switch (type)
        {
        case DBUS_TYPE_STRING:
          {
            char *val;
            time_t current_time;
            char buf[64];
                        
            current_time = time(NULL);
            strftime(buf, sizeof buf, "%Y/%b/%d %X%z - ", localtime(&current_time));
            dbus_message_iter_get_basic (iter, &val);

            if (!strcmp(val, match)) {
              flag++;
              printf ("%s%s\n", buf, val);
            }
            break;
          }
        default:
          break;
        }
    } while (dbus_message_iter_next (iter));
    
    return flag;
}

void
print_message (DBusMessage *message, char * match, char * command)
{
  DBusMessageIter iter;
  const char *sender;

  sender = dbus_message_get_sender (message);
  
  if (strcmp(sender, "org.freedesktop.DBus")) {
    dbus_message_iter_init (message, &iter);
    if (print_iter (&iter, match)) {
      system(command);
      //~ printf("***MATCH***\n");
    }
    fflush (stdout);
  }
}
