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
    
    //debug("pid = %i\n", pid);
    if (pid == 0){
        char *v[settings->exec_argc];
        int i=0;
        for (i=0; i<settings->exec_argc-1; i++){
            //debug("string arg is %s\n", settings->exec_argv[i]);
            if (settings->exec_argv[i]) { 
                if (strcmp(settings->exec_argv[i], "{}")==0){
                    //debug("replacing argv at %i\n", i);
                    v[i] = path;
                }
                else if (strcmp(settings->exec_argv[i], ";")==0){
                    //debug("replacing ';' at %i\n", i);
                    v[i] = NULL;
                }
                else {
                    v[i] = settings->exec_argv[i];
                }
            }
        }
        v[settings->exec_argc-1] = NULL;
        
        /*int j = 0;
        for(j=0; j < settings->exec_argc; j++) {
            debug("v[%i] = %s\n", j, v[j]);
        }*/
        if (execvp(v[0], v) < 0){
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
