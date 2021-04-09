/**
 * @file iniparse.c
 * @brief this file contains functions needed to pase a INI format file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/hashtable.h"
#include "../inc/iniparse.h"

#define COMMENT ";#"
#define INI_FILE_PATH "/etc/example/daemon.conf"

/**
 * @brief function to count occurrences of a character in a string.
 *
 * @param[in] *src pointer to string where to search.
 * @param[in] ch character searched.
 *
 * @return number of occurrences of ch in *str.
 */
static int count_char(const char *str, const char ch){

    int c = 0;

    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] == ch){
            c++;
        }
    }

    return c;
}

/**
 * @brief function to count occurrences of a character in a string.
 *
 * @param[out] htable hash table to be updated with parsed values.
 *
 * @return 0 is file was parsed or -1 is an error happened.
 */
int ini_file_parse(HashTable* htable){

    FILE *fd;
    char *line = NULL;
    char *section = NULL;
    char *key = NULL;
    char *value = NULL;
    size_t len = 0;
    ssize_t read;
    int ret = 0;

    if((fd = fopen(INI_FILE_PATH, "r")) == NULL){
        perror("Error, config file could not be opened");
        exit(EXIT_FAILURE);
    }

    while(((read = getline(&line, &len, fd)) != -1) && (ret == 0)){
        /* Ignore comment lines */
        if(strchr(COMMENT, line[0])){
            /* do nothing */
        }
        /* Ignore new empty lines */
        else if(line[0] == '\n'){
            /* do nothing */
        }
        /* Detect section line */
        else if(line[0] == '['){
            if((count_char(line, '[') == 1) && (count_char(line, ']') == 1) && (line[strlen(line)-2] == ']')){
                section = upd_str(strtok(line, "[]"));
            }
            else{
                ret = -1;
            }
        }
        /* Detect parameter assignment line */
        else{
            if(count_char(line, '=') == 1){
                key = strtok(line, "=");
                value = strtok(NULL, "\n");
                ht_insert(htable, key, value, section);
            }
            else{
                ret = -1;
            }
        }
    }

    fclose(fd);
    if(section){
        free(section);
    }
    if(line){
        free(line);
    }

    return ret;
}
