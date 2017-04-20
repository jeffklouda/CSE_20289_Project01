/* expression.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


/**
 * Executes the -print or -exec expressions (or both) on the specified path.
 * @param   path        Path to a file or directory
 * @param   settings    Settings structure.
 * @return  Whether or not the execution was successful.
 */
int	    execute(const char *path, const Settings *settings) {
    //debug("EXECUTE.c\n");
       
    if (settings->print && settings->exec_argc>0){
        printf("%s\n", path);
    }else if (!(!settings->print && settings->exec_argc>0)){
        printf("%s\n", path);
    }
   //debug("settings->print: %i\n", settings->print);
   //debug("path: %s\n", path);
   //debug("settings->exec_argc: %i\n", settings->exec_argc);
    pid_t pid = fork(); 
    if (pid < 0){
        fprintf(stderr, "Unable to fork: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    //debug("%i\n", pid);
    
    if (pid == 0){
        //printf("polll: %s\n", settings->exec_argv[0]);
        //printf("HE\n");
        //char *v[] =settings->exec_argv;
        debug("before execvp: %s\n", settings->exec_argv[0]);
        debug("second argument: %s\n", settings->exec_argv[1]);
        if (execvp(settings->exec_argv[0], settings->exec_argv) < 0){
            fprintf(stderr, "Unable to exec: %s\n", strerror(errno));
            _exit(EXIT_FAILURE);
        }
    
    }else{
        //debug("pid is greater than 0\n");
        int status;
        while ((pid = wait(&status)) < 0);
    }
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
