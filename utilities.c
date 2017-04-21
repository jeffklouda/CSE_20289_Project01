/* utilities.c */

#include "search.h"

#include <errno.h>
#include <string.h>
#include <time.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Test whether or not a directory is empty.
 * @param   path        Path to directory.
 * @return  Whether or not a directory is empty.
 */
bool        is_directory_empty(const char *path) {
    DIR* checkDir;
    //checkDir = opendir(path);
    if ((checkDir = opendir (path)) == NULL) {
        fprintf(stderr, "pathname: %s and error: %s\n", path, strerror(errno));
        return false;
    }
    int i = 0;
    if (checkDir) {
        struct dirent* ent;
        errno = 0;
        while((ent = readdir(checkDir)) != NULL) {
            if (errno != 0) {
                fprintf(stderr, "%s\n", strerror(errno));
                closedir(checkDir);
                return true;
            }
            i++;
            if (i > 2) {
                break;
            }
        }
    }
    else {
        fprintf(stderr,"%s\n", strerror(errno));
        return true;
    }
    if (i > 2) {
        closedir(checkDir);
        return false;
    }
    closedir(checkDir);
    return true;
}

/**
 * Retrieve the modification time of the given file.
 * @param   path        Path to file of directory.
 * @return  The modification time of the given file.
 */
time_t      get_mtime(const char *path) {
    struct stat st;
    lstat(path, &st);
    //if (!st) {
    //    printf ("get_mtime Error: %s", strerror(errno));
    //    return 0;
    //}
    return (time_t)st.st_mtim.tv_sec;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
