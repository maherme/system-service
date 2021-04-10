/**
 * @file main.c
 * @brief this file contains main function of the system service.
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <syslog.h>
#include <string.h>
#include <dbus/dbus.h>
#include "../inc/hashtable.h"
#include "../inc/iniparse.h"
#include "../inc/dbusfunctions.h"

static HashTable *ht;

static void reload_config(void);
static void log_config(void);
static void sig_handler(int sig_num);
static void subscriptions(void);
static DBusHandlerResult service_messages(DBusConnection *connection, DBusMessage *message, void *user_data);

/**
 * @brief main function of the system service.
 */
int main(int argc, char *argv[]){

    pid_t rf;
    DBusConnection *connection;
    DBusError error;
    DBusObjectPathVTable vtable;
    ht = ht_create_table(16);

    rf = fork();
    switch(rf){
        /* fork returns error */
        case -1:
            perror("Error: fork could not be completed");
            exit(EXIT_FAILURE);
        /* Child process, it is the daemon process */
        case 0:
            break;
        /* Parent process, let it terminate */
        default:
            exit(EXIT_SUCCESS);
    }

    /* Parsing the ini file */
    if(ini_file_parse(ht) != 0){
        perror("Error: ini file could not be parsed");
    }

    /* Subscribing to system signals */
    subscriptions();

    /* Connecting to D-Bus */
    dbus_error_init(&error);
    /* Connection to bus session */
    connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
    check_error(&error);
    /* Requesting a name */
    dbus_bus_request_name(connection, "com.redhat.SystemService", 0, &error);
    check_error(&error);
    /* Linking the callback function */
    vtable.message_function = service_messages;
    vtable.unregister_function = NULL;
    /* Registering a new object for the service */
    dbus_connection_try_register_object_path(connection, "/com/redhat/SystemService", &vtable, NULL, &error);
    check_error(&error);

    while(true){
        /* Waiting for reading a message */
        dbus_connection_read_write_dispatch(connection, 1000);
    }

    ht_free_table(ht);

    return 0;
}

/**
 * @brief function to reload the configuration by reparsing the
 * configuration file.
 *
 * @return void.
 */
static void reload_config(void){

    if(ini_file_parse(ht) != 0){
        perror("Error: reloading config, ini file could not be parsed");
    }
}

/**
 * @brief function to print out parsed configuration to system log.
 *
 * @return void.
 */
static void log_config(void){

    size_t i = 0;
    HtItem *item = NULL;

    openlog("system-daemon", LOG_PID, LOG_USER);
    for(i = 0; i < ht->size; i++){
        item = ht->items[i];
        while(item){
            syslog(LOG_INFO, "%s=%s in [%s]", item->key, item->value, item->section);
            item = item->next;
        }
    }
    closelog();
}

/**
 * @brief function for signal handling.
 *
 * @param[in] sig_num number of raised signal.
 *
 * @return void.
 */
static void sig_handler(int sig_num){

    switch(sig_num){
        case SIGHUP:
            reload_config();
            break;
        case SIGUSR1:
            log_config();
            break;
        default:
            break;
    }
}

/**
 * @brief function for signal subscriptions.
 *
 * @return void.
 */
static void subscriptions(void){

    struct sigaction psa;
    memset(&psa, 0, sizeof psa);
    psa.sa_handler = sig_handler;

    if(sigaction(SIGHUP, &psa, NULL) != 0){
        perror("Error: SIGHUP subscription failed");
        exit(EXIT_FAILURE);
    }

    if(sigaction(SIGUSR1, &psa, NULL) != 0){
        perror("Error: SIGUSR1 subscription failed");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief callback function for managing the method calling through d-bus. It follows a specific
 * format defined in dbus.freedesktop.org (DBusObjectPathMessageFunction).
 *
 * @param[in] *connection.
 * @param[in] *message pointer to the message to be sent or received over the connection.
 * @param[in] *user_data
 *
 * @return the message handler result (type enum DBusHandlerResult).
 */
static DBusHandlerResult service_messages(DBusConnection *connection, DBusMessage *message, void *user_data){

    DBusHandlerResult result;
    const char *interface_name = dbus_message_get_interface(message);
    const char *member_name = dbus_message_get_member(message);

    if((strcmp("org.freedesktop.DBus.Introspectable", interface_name) == 0) && (strcmp("Introspect", member_name) == 0)){
        respond_to_introspect(connection, message);
        result = DBUS_HANDLER_RESULT_HANDLED;
    }
    else if((strcmp("com.redhat.SystemService", interface_name) == 0) && (strcmp("ReloadConfig", member_name) == 0)){
        reload_config();
        result = DBUS_HANDLER_RESULT_HANDLED;
    }
    else if((strcmp("com.redhat.SystemService", interface_name) == 0) && (strcmp("LogConfig", member_name) == 0)){
        log_config();
        result = DBUS_HANDLER_RESULT_HANDLED;
    }
    else{
        result = DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

    return result;
}
