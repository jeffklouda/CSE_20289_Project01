/* driver.c */

#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

char *PROGRAM_NAME = NULL;
char *PATH = NULL;
/* Functions */

void	    usage(const char *program_name, int status) {
    fprintf(stderr, "Usage: %s PATH [OPTIONS] [EXPRESSION]\n", program_name);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "    -executable     File is executable or directory is searchable to user\n");
    fprintf(stderr, "    -readable       File readable to user\n");
    fprintf(stderr, "    -writable       File is writable to user\n\n");
    fprintf(stderr, "    -type [f|d]     File is of type f for regular file or d for directory\n\n");
    fprintf(stderr, "    -empty          File or directory is empty\n\n");
    fprintf(stderr, "    -empty          File or directory is empty\n\n");
    fprintf(stderr, "    -name  pattern  Base of file name matches shell pattern\n");
    fprintf(stderr, "    -path  pattern  Path of file matches shell pattern\n\n");
    fprintf(stderr, "    -perm  mode     File's permission bits are exactly mode (octal)\n");
    fprintf(stderr, "    -newer file     File was modified more recently than file\n\n");
    fprintf(stderr, "    -uid   n        File's numeric user ID is n\n");
    fprintf(stderr, "    -gid   n        File's numeric group ID is n\n");
    fprintf(stderr, "\nExpressions:\n\n");
    fprintf(stderr, "    -print          Display file path (default)\n");
    fprintf(stderr, "    -exec cmd {} ;  Execute command on path\n");
    exit(status);
}

/* Main Execution */

int	    main(int argc, char *argv[]) {
    Settings settings = {
        .access = 0,
        .uid    = -1,
        .gid    = -1,
    };

    int argind = 2;

    /* Parse command line arguments */
    PROGRAM_NAME = argv[0];
    PATH         = argv[1];
    while (argind < argc && strlen(argv[argind]) > 1) {
        char *arg = argv[argind++];
        if (streq(arg, "-executable")){
            settings.access |= X_OK;
        }
        if (streq(arg, "-readable")){
            settings.access |= R_OK;
        }
        if (streq(arg, "-writable")){
            settings.access |= W_OK;
        }
        if (streq(arg, "-type")){
            char *type = argv[argind++];
            if (streq(type, "f")){
                settings.type = 1;
            }else{
                settings.type = 2;
            }
        }
        if (streq(arg, "-empty")){
            settings.empty = 1;
        }
        if (streq(arg, "-name")){
            char *name = argv[argind++];
            settings.name = strdup(name);
        }
        if (streq(arg, "-path")){
            char *path = argv[argind++];
            settings.path = strdup(path);
        }
        if (streq(arg, "-perm")){
            char *perm = argv[argind++];
            settings.perm = (perm[0]-'0')*64+(perm[1]-'0')*8+(perm[2]-'0');
        }
        if (streq(arg, "-newer")){
            char *file = argv[argind++];
            settings.newer = get_mtime(file);
        }
        if (streq(arg, "-uid")){
            char *uid  = argv[argind++];
            settings.uid = atoi(uid);
        }
        if (streq(arg, "-gid")){
            char *gid  = argv[argind++];
            settings.gid = atoi(gid);
        }
        if (streq(arg, "-print")){
            settings.print = 1;
        }
        if (streq(arg, "-exec")){
           debug("allocating: %i\n", (int)argc-argind+1);
           settings.exec_argv = malloc(sizeof(char*)*(argc-argind+1)); 
           int i=0;
           while (argind < argc){
               settings.exec_argv[i++] = strdup(argv[argind++]);     
               settings.exec_argc++;
               debug("argv[%i] = %s\n", i - 1, settings.exec_argv[i-1]);
           }
           //settings.exec_argv[i] = NULL;
           //settings.exec_argc++;
        debug("argv[%i] = %s\n", i, settings.exec_argv[i]);
        debug("argc = %i", settings.exec_argc);

           //settings.exec_argv[i] = strdup(NULL);
           // int starting_point = argind+1;
           // int ending_point   = starting_point;
           // while (argind < argc && strlen(argv[argind]) > 1){
           //     ending_point++;
           //     argind++;
           // }
           // settings.exec_argc = ending_point - starting_point;
           // memcpy(&settings.exec_argv, (argv[0]+starting_point), settings.exec_argc);
        }
    }
    if (!filter(PATH, &settings)){
        execute(PATH, &settings);
    }
    search(PATH, &settings);
    int i = 0;
    for (i = 0; i < settings.exec_argc; i++) {
        if (settings.exec_argv[i] != NULL)
            free(settings.exec_argv[i]);
    }
    free(settings.exec_argv);
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
