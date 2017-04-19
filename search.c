/* search.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>

/**
 * Recursively search through the provided root directory
 * @param   root        Path to directory
 * @param   settings    Settings structure
 * @return  Whether or not the search was successful.
 */
int	    search(const char *root, const Settings *settings) {
    //debug("SEARCH.c\n");
    DIR *dir;
    struct dirent *dp;
    int status = EXIT_FAILURE;
    int status_two = EXIT_FAILURE;
    struct stat buf;
    lstat(root, &buf);
    //debug("before: %s\n", root);
    if (!S_ISDIR(buf.st_mode)){
        //debug("regular file: %s\n", root);
        return EXIT_FAILURE;
    }
    if (((dir = opendir(root)) == NULL)){    
        //printf("Couldn't open directory: %s\n", strerror(errno));    
        //closedir(dir);  
        //debug("couldn't open: %s\n", root);   
        return EXIT_FAILURE;
    }
    //debug("after: %s\n", root);
    while ((dp = readdir(dir)) != NULL){
        //debug("%s\n", dp->d_name);
        if (strcmp(dp->d_name, ".")!=0 && strcmp(dp->d_name, "..")!=0){ 
            char *full_path = malloc(strlen(root)+strlen(dp->d_name)+2); 
            strcpy(full_path, root);
            strcat(full_path, "/");
            strcat(full_path, dp->d_name);
            //char buffer[BUFSIZ];
            //strcpy(buffer, full_path);
            //debug("before\n");
            //free(full_path);
            //debug("after\n");
            //debug("%s: recurse here\n", full_path);
            if (!filter(full_path, settings)){
                //debug("before execute\n");
                execute(full_path, settings);
                status = EXIT_SUCCESS;
            }
            status_two = search(full_path, settings);
            free(full_path);        
        }
        
        //debug("%s\n", dp->d_name);
    }

    closedir(dir);
    return (status || status_two);
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
