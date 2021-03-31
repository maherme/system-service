#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char *argv[]){

    pid_t rf;

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

    return 0;
}
