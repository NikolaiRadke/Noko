// Serial Writer - Nikolai Radke
// Writes file "texte" with a delay of 5ms for each char to 
// specified destination
//
// Compile with "gcc schreiben.c" 
// Rename with "mv a.out schreiben"
// Set schreiben with "chmod 755 schreiben" to be executeable
// Use with "./schreiben > /dev/ttyUSBX (X=used USB port, see
// port number in Arduino IDE

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
