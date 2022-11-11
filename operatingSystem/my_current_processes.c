#include <stdio.h>
#include <dirent.h>
#include <stdlib.h> 
#include <ctype.h>

int  main( int argc, char** argv ){

    DIR *procDir;
    struct dirent *dp;
    FILE *fp;

    // Error check
    if ((procDir = opendir("/proc")) == NULL) {
        perror("couldn't open '/proc'");
        exit( EXIT_FAILURE );
    }

    // create or open text file
    fp = fopen("/tmp/output1.txt", "w+");

    // Loop through process directories 
    while( (dp = readdir(procDir)) != NULL){
        if( dp->d_type == DT_DIR && isdigit( dp->d_name[0] ) ){            
            // Write directory name to output1.txt
            fprintf(fp, "%s\n", dp->d_name);
        }
    }

    // clean up
    fclose(fp);
    closedir(procDir);
    exit( EXIT_SUCCESS );

    return 0;
}