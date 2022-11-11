#include <stdio.h>
#include <dirent.h>
#include <stdlib.h> 
#include <ctype.h>

int  main( int argc, char** argv ){

    // Open meminfo file
    FILE *meminfo = fopen("/proc/meminfo", "r");

    // output2.txt
    FILE *fp = fopen("/tmp/output2.txt", "w+");

    // meminfo.txt
    FILE *fp2 = fopen("/tmp/meminfo.txt", "w+");
    
    //Error check
    if(meminfo == NULL){
        perror("couldn't open '/proc/meminfo'");
        exit( EXIT_FAILURE );
    }

    // allocate memory for line
    char line[256];

    unsigned long memTotal = -1;
    unsigned long memFree = -1;
    unsigned long buffers = -1;
    unsigned long cached = -1;
    unsigned long slab = -1;

    // while the file can read a line
    while(fgets(line, sizeof(line), meminfo))
    {
        sscanf(line, "MemTotal: %lu kB", &memTotal);
        sscanf(line, "MemFree: %lu kB", &memFree);
        sscanf(line, "Buffers: %lu kB", &buffers);
        sscanf(line, "Cached: %lu kB", &cached);
        sscanf(line, "Slab: %lu kB", &slab);

        //copy all lines into meminfo.txt
        fputs(line, fp2);
    }

    if(memTotal == -1 || memFree == -1 || buffers == -1 || cached == -1 || slab == -1){
        perror("couldn't retrive variable");
        exit( EXIT_FAILURE );
    }

    // Do the Math!
    double memUtilNumerator = memTotal - memFree - buffers - cached - slab;
    double memUtil = memUtilNumerator / memTotal;
    double memUtilPercent = memUtil * 100;


    // Write percentage to output2.txt
    fprintf(fp, "%.2f\n", memUtilPercent);

    // clean up
    fclose(meminfo);
    fclose(fp);
    fclose(fp2);
    exit( EXIT_SUCCESS );

    return 0;
}