// Serial Writer - Nikolai Radke
// Writes file "texte" with a delay of 5ms for each char to 
// specified destination
//
// Compile with "gcc write_Disc0.c" 
// 
// Linux:
// Rename with "mv a.out write_Disc0"
// Set write_Disc0 with "chmod 755 write_Disc0" to be executeable
// Use with "./write_Disc0 > /dev/ttyUSBX (X=used USB port, see
// port number in Arduino IDE

#include <stdio.h>

int main()
{
   int c;
   FILE *datei;
   usleep(5000000);	
   datei=fopen("Disc0","r");
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
