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
        int placeholder = -1;
        //printf("polll: %s\n", settings->exec_argv[0]);
        //printf("HE\n");
        //char *v[] =settings->exec_argv;
        //if (strcmp(settings->exec_argv[1], "{}") == 0){
        int i=0;
        for (i=0; i<settings->exec_argc; i++){
            debug("string arg is %s\n", settings->exec_argv[i]);
            if (settings->exec_argv[i]) { 
                if (strcmp(settings->exec_argv[i], "{}")==0){
                    debug("replacing argv at %i\n", i);
                    free(settings->exec_argv[i]);
                    settings->exec_argv[i] = strdup(path);
                    debug("argv changed to %s", settings->exec_argv[i]);
                    placeholder = i;
                    debug("placeholder = %i", placeholder);
                }
                if (strcmp(settings->exec_argv[i], ";")==0){
                    debug("spot 2: %i\n", i); 
                    free(settings->exec_argv[i]);
                    settings->exec_argv[i] = NULL;
                }
            }
        }
        // }
        debug("before execvp: %s\n", settings->exec_argv[0]);
        //debug("second argument: %s\n", settings->exec_argv[1]);
        if (execvp(settings->exec_argv[0], settings->exec_argv) < 0){
            fprintf(stderr, "Unable to exec: %s\n", strerror(errno));
            free(settings->exec_argv[placeholder]);
            debug("are we down here?\n");
            _exit(EXIT_FAILURE);
        }
        debug("what is placeholder? %i\n", placeholder);
        //if (placeholder >= 0) {
            debug("Placeholder = %i\n", placeholder);
            debug("argc = %i\n", settings->exec_argc);
            debug("freeing %s\n", settings->exec_argv[placeholder]);
            free(settings->exec_argv[placeholder]);
        //}
    
    }else{
        //debug("pid is greater than 0\n");
        int status;
        while ((pid = wait(&status)) < 0);
    }
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
