// Serial Writer - Nikolai Radke
// Version 20112018
//
// Writes file "EEPROM" with a delay of 5ms for each char to 
// specified destination
//
// Compile with "gcc write_eeprom.c" 
// Rename with "mv a.out write_eeprom"
// Set schreiben with "chmod 755 write_eeprom" to be executeable
// Use with "./write_eeprom > /dev/ttyUSBX (X=used USB port, see
// port number in Arduino IDE
//
// Thanks to jlegen 

#include <stdio.h>

int main()
{
   int c;
   long j=0;
   long k;
   FILE *file;
   file=fopen("EEPROM","r");
   if(file != NULL)
   {
        usleep(5000000);	
        fseek(file, 0L, SEEK_END);
		k = ftell(file);
		rewind(file);
        while((c=fgetc(file))!= EOF)
		{
	        fprintf(stderr, "Bytes written: %d/%d\r", j, k);
			fflush(stderr);
            putchar(c);
			fflush(stdout);
			usleep(50000);
			j++;
		 }	
   }
   else
      printf("Could not find or open file!\n");
 return 0;
}
