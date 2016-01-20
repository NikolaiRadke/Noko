#include <stdio.h>

int main()
{
   int c;
   FILE *datei;
   usleep(5000000);	
   datei=fopen("texte","r");
   if(datei != NULL)
      {
         while((c=fgetc(datei))!= EOF)
		 {
            putchar(c);
			fflush(stdout);
			usleep(50000);
		 }	
      }
   else
      printf("Konnte Datei nicht finden bzw. öffnen!\n");
 return 0;
}
