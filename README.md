# ![NOKO](http://www.nikolairadke.de/NOKO/noko_klein.png) NOKO

Das **NOKO Monster** zum Nachbauen und gern haben. Beschreibung und Anleitungen dazu im [Wiki](https://github.com/NikolaiRadke/NOKO/wiki). Jeder, der Lust hat, darf sich gerne beteiligen, ich bin für Vorschläge stets (Quell-)offen. 

->![NOKO_Nr.2](http://www.nikolairadke.de/NOKO/noko_nr2.png)<-

NOKO ist ein multifunktionales Monster mit einem Arduino Nano als Mikrocontroller, Display, Uhr, Radio, MP3-Player, Bedienelementen, Akku, Sprachset und vielen Gimmicks, verpackt in ein knuffiges Monsterkostüm - stabil, liebenswert und manchmal auch lästig. Ganz, wie man ihn haben möchte. 


###English 
*Anything sourcode-related, the circut diagrams and comments on how to compile are written in english, the building documentation is in german. Feel free to ask me for translatation, if you need help. NOKO itself speaks and writes in german. However, someday, he may learn english or other languages... contribute!
Requirements: The [Arduino IDE 1.6.6](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous), i am planning to switch to 1.6.7 soon. Put the folder NOKO_Arduino_NOKO/ into your sketch folder. Right now, NOKO.ino won't compile without changing the local "platform.txt". See [HOWTO compile](https://github.com/NikolaiRadke/NOKO/tree/master/HOWTO_Compile) and change the file. Now NOKO.ino should compile.*

###Voraussetzungen
NOKO benötigt die [Arduino IDE 1.6.6](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous), demnächst werde ich auf 1.6.7 umsteigen. Das Verzeichnis NOKO_Arduino/NOKO/ muss in den Sketch-Ordner kopiert werden. Noch kann NOKO.ino nicht kompiliert werden. Dazu muss die Datei "platform.txt" angepasst werden: [HOWTO Compile](https://github.com/NikolaiRadke/NOKO/tree/master/HOWTO_Compile). Nun sollte sich NOKO.ino kompilieren lassen.

### Verzeichnisse

```
NOKO/
├── HOWTO_Compile/
|   Hinweise und angepasste plattform.txt für die Kompilierung in der Arduino IDE
├── MP3/
|   Sprachdateien für NOKO
├── NOKO_Arduino/
|   Arduino-Sketche
|   ├── NOKO/
|   |   Arduino-Quelltext mit allen Bibliotheken.
|   ├── NOKO_Disk1/
|   |   Arduino-Sketch zum Beschreiben des 24LC256-EEPROMs.
│   |   ├── TTY/
|   |       Hilfsprogramm, um die Textdatei über die serielle Verbindung an den Arduino zu senden.
|   ├── NOKO_EEPROM_Disk0/
|   |   Arduino-Sketch zum Schreiben der Grundeinstellungen in das Arduino-EEPROM und 
│   |   zum Stellen der Echtzeituhr. Optional zum Beschrieben des AH24C32-EEPROMs auf dem Modul der Echtzeituhr.
|   |   ├── TTY/
|   |       Hilfsprogramm, um die Textdatei über die serielle Verbindung an den Arduino zu senden.
├── Schaltplan/
|   Schaltpläne der NOKO-Module. 
|   ├── geda/
|   Rohe Schaltpläne für gEDA

```

WEITERE DATEIEN FOLGEN IN KÜRZE.
