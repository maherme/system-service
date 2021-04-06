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
#include "../inc/hashtable.h"
#include "../inc/iniparse.h"

static HashTable *ht;

static void reload_config(void);
static void log_config(void);
static void sig_handler(int sig_num);
static void subscriptions(void);

/**
 * @brief main function of the system service.
 */
int main(int argc, char *argv[]){

    pid_t rf;
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

    if(ini_file_parse(ht) != 0){
        perror("Error: ini file could not be parsed");
    }

    subscriptions();

    while(true){
        pause();
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
