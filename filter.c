/* filter.c */

#include "search.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>

/**
 * Test whether or not the given path should be filtered out of the output.
 * @param   path        Path to file or directory.
 * @param   settings    Settings structure.
 * @return  Whether or not the path should be excluded from the output (true to
 * exclude, false to include).
 */
bool        filter(const char *path, const Settings *settings) {
    bool returnVal = false;
    struct stat buf;
    if (lstat(path, &buf) < 0) {
        fprintf(stderr, "%s", strerror(errno));
        return true;
    }
    /* check -executable, -readable. -writable */
    if (settings->access && access(path, settings->access) == 0)
        return true;
    
    /* Empty files and directories*/
    if (settings->empty) {
        if (S_ISDIR(buf.st_mode)) {
            if (!is_directory_empty(path))
                return true;
        }
        if (S_ISREG(buf.st_mode)) {
            if (buf.st_size != 0)
                return true;
        }
    }

    /* Test file type f = 1, d = 2*/
    if ((settings->type == 1) && !S_ISREG(buf.st_mode))
        return true;
    if ((settings->type == 2) && !S_ISDIR(buf.st_mode))
        return true;
    
    /* Base of file matches shell pattern */
    if (settings->name != NULL) {
        char* tempPath = strdup(path);
        char* bname = strdup(basename(tempPath));
        free(tempPath);
        if (fnmatch(settings->name, bname, 0)) {
            free(bname);
            return true;
        }
        free(bname);
    }
    
    /* Path of file matches shell pattern */
    if (settings->path != NULL) {
        if (fnmatch(settings->path, path, 0))
            return true;
    }
    /* check permissions */
    int perm = buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    if (settings->perm) {
        if (settings->perm != perm)
            return true;
    }

    /* File was modified more recently than "newer" */
    if (settings->newer) {
        time_t mtime = get_mtime(path);
        if (settings->newer > mtime)
            return true;
    }
    
    /* Check file's numeric user ID */
    if (settings->uid >= 0) {
        if (settings->uid != (int) buf.st_uid)
            return true;
    }

    /* Check file's numeric group ID */
    if (settings->gid >= 0) {
        if (settings->gid != (int) buf.st_gid)
            return true;
    }

}
