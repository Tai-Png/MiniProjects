#include <stdlib.h>
#include <stdio.h>

int main() {

   // os release file and output file pointers
   FILE *osRelease, *fp;
   
   // Error checks
   osRelease = fopen("/etc/os-release", "r");
   if (osRelease == NULL) {
      printf("could not open /os-release file \n");
      exit(EXIT_FAILURE);
   }

   fp = fopen("/tmp/output3.txt", "w");
   if (fp == NULL) {
      fclose(osRelease);
      printf("could not open output3.txt \n");
      exit(EXIT_FAILURE);
   }

   // Copy each line in os-release file
   char line[256];

   while(fgets(line, sizeof(line), osRelease))
   {
      fputs(line, fp);
   }

   // done
   printf("File copied successfully.\n");
   fclose(osRelease);
   fclose(fp);

   return 0;
}
