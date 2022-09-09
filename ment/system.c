/* g77 interface to system routines crated by Urs Rohrer, PSI */

//#include "f2c.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "unistd.h"
#include "string.h"

random_(float *rnd)
{
   long rn;

   rn = rand();
   *rnd = (float)rn / (float)RAND_MAX;
}

seed_(long *seed)
{
   srand(*seed);
}

timing_(long *secs)
{
   time_t t;
   t = time(NULL);
   *secs = t;
}

putcha_(int *c)
{
   putchar(*c);
}

exit_(int *code)
{
   exit(*code);
}

sleep_(long *secs)
{
   sleep(*secs);
}

int iand_(int *i1, int *i2)
{
   return (*i1 & *i2);
}

mytime_(char *tim)
{
   time_t t;
   char *ts;
   char *p;

   strcpy(tim,"        ");
   t = time(NULL);
   ts = ctime(&t);
   p = strtok(ts," ");
   p = strtok(NULL," ");
   p = strtok(NULL," ");
   p = strtok(NULL," ");
   if (p)
      strncpy(tim,p,strlen(p));
}

mydate_(char *dat)
{
   time_t t;
   char *ts;
   char *p;
   char month[4], day[3], year[5], tim[9], d[10];

   t = time(NULL);
   ts = ctime(&t);

   p = strtok(ts," ");

   p = strtok(NULL," ");
   if (p)
      strcpy(month,p);

   p = strtok(NULL," ");
   if (p)
      strcpy(day,p);

   p  = strtok(NULL," ");
   if (p)
      strcpy(tim,p);

   p = strtok(NULL," ");
   if (p)
      strcpy(year,p);

   year[4] = '\0';
   sprintf(d,"%s-%s-%s",day,month,&year[2]);
   strcpy(dat,"         ");
   strncpy(dat,d,9);
}



