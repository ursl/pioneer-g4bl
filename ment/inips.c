/* Compiled by Urs Rohrer (PSI) */

#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

inips_(char *fname, char *text)
{
   FILE *fp;
   fp = fopen(fname,"w+");
   fprintf(fp,text);
   fprintf(fp,"\n");
   fclose(fp);
}


