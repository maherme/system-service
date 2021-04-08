#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dbus/dbus.h>

/**
 * @brief function to check errors produced by usage of dbus APIs.
 *
 * @param[in] *error pointer to struct for managing dbus errors (DBusError).
 *
 * @return void.
 */
void check_error(DBusError *error){

    if (dbus_error_is_set(error)) {
        perror(error->message);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief 
 *
 * @param[in] *connection pointer to handler the connection for answering to the method called.
 * @param[in] *request pointer to the message to be received over the connection.
 *
 * @return void.
 */
void respond_to_introspect(DBusConnection *connection, DBusMessage *request){

    DBusMessage *reply;

    const char *introspection_data =
        " <!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\" "
        "\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">"
        " <!-- dbus-sharp 0.8.1 -->"
        " <node>"
        "   <interface name=\"org.freedesktop.DBus.Introspectable\">"
        "     <method name=\"Introspect\">"
        "       <arg name=\"data\" direction=\"out\" type=\"s\" />"
        "     </method>"
        "   </interface>"
        "   <interface name=\"com.redhat.SystemService\">"
        "     <method name=\"ReloadConfig\">"
        "     </method>"
        "     <method name=\"LogConfig\">"
        "     </method>"
        "   </interface>"
        " </node>";

    reply = dbus_message_new_method_return(request);
    dbus_message_append_args(reply, DBUS_TYPE_STRING, &introspection_data, DBUS_TYPE_INVALID);
    dbus_connection_send(connection, reply, NULL);
    dbus_message_unref(reply);
}
