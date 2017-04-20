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
	//debug("FILTER.c\n");
    bool returnVal = false;
    struct stat buf;
	//debug("path: %s\n", path);
    if (lstat(path, &buf) < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        return true;
    }
	//debug("Test1\n");    
	/* check -executable, -readable. -writable */
    if (settings->access && access(path, settings->access) != 0)
        return true;
    //debug("Test2\n");  
    /* Empty files and directories*/
    if (settings->empty) {
		if (S_ISREG(buf.st_mode)) {
			//debug("%i\n", buf.st_size);
            if (buf.st_size != 0)
                return true;
        }else if (S_ISDIR(buf.st_mode)) {
            if (!is_directory_empty(path))
                return true;
        }else{
			return true;
		}
        
    }

	//debug("Test3\n");      
	/* Test file type f = 1, d = 2*/
    if ((settings->type == 1) && !S_ISREG(buf.st_mode))
        return true;
    if ((settings->type == 2) && !S_ISDIR(buf.st_mode))
        return true;
    
	//debug("Test4\n");      
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
    
	//debug("Test5\n");      
	/* Path of file matches shell pattern */
    if (settings->path != NULL) {
        if (fnmatch(settings->path, path, 0))
            return true;
    }
    
	//debug("Test6\n");	
	/* check permissions */
    int perm = buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    if (settings->perm) {
        if (settings->perm != perm)
            return true;
    }

	//debug("Test7\n");  
	/* File was modified more recently than "newer" */
    if (settings->newer) {
        time_t mtime = get_mtime(path);
        if (settings->newer >= mtime)
            return true;
    }
        
	//debug("Test8\n");  
	/* Check file's numeric user ID */
    if (settings->uid >= 0) {
        if (settings->uid != (int) buf.st_uid)
            return true;
    }

	//debug("Test9\n");    
	/* Check file's numeric group ID */
    if (settings->gid >= 0) {
        if (settings->gid != (int) buf.st_gid)
            return true;
    }
	return false;
}
