/**
 * @file main.c
 * @brief this file contains main function of the system service.
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "../inc/hashtable.h"
#include "../inc/iniparse.h"

/**
 * @brief main function of the system service.
 */
int main(int argc, char *argv[]){

    pid_t rf;
    HashTable* ht = ht_create_table(16);

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

    ht_free_table(ht);

    return 0;
}
