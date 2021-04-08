/**
 * @file dbusfunctions.h
 * @brief header file of dbusfunctions.c
 */

#ifndef _DBUSFUNCTIONS_H
#define _DBUSFUNCTIONS_H

#include <dbus/dbus.h>

void check_error(DBusError *error);
void respond_to_introspect(DBusConnection *connection, DBusMessage *request);

#endif
