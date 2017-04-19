/* search.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>

/**
 * Recursively search through the provided root directory
 * @param   root        Path to directory
 * @param   settings    Settings structure
 * @return  Whether or not the search was successful.
 */
int	    search(const char *root, const Settings *settings) {
    DIR *dir;
    struct dirent *dp;
    if ((dir = opendir(root)) == NULL){    
        return EXIT_FAILURE;
    }
    
    while ((dp = readdir(dir)) != NULL){
        if (filter(dp->d_name, settings)){
            execute(dp->d_name, settings);
        }
        search(dp->d_name, settings);
    }    

    return EXIT_FAILURE;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
