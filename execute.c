/* expression.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Executes the -print or -exec expressions (or both) on the specified path.
 * @param   path        Path to a file or directory
 * @param   settings    Settings structure.
 * @return  Whether or not the execution was successful.
 */
int	    execute(const char *path, const Settings *settings) {
    if (settings->print || settings->exec_argc > 0){
        printf("%s\n", path);
    }
    pid_t pid = fork();
    if (pid < 0){
        fprintf(stderr, "Unable to fork: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if (pid == 0){
        //char *v[] =settings->exec_argv;
        if (execvp(settings->exec_argv[0], settings->exec_argv) < 0){
            fprintf(stderr, "Unable to exec: %s\n", strerror(errno));
            _exit(EXIT_FAILURE);
        }
    
    }else{
        int status;
        while ((pid = wait(&status)) < 0);
    }
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
