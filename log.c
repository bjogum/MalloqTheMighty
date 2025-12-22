#include <stdio.h>
#include <stdlib.h>
#define LOGFILE "log.txt"


void saveStr(char *str){ 
    
    // open the file for writing
    FILE *file = fopen(LOGFILE, "a");
    if (file == NULL)
    {
        printf("Error opening the file %s", LOGFILE);
    }
    // write to the text file
    fprintf(file, "%s\n", str);

    // close the file
    fclose(file);
}