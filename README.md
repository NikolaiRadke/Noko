# ![NOKO](http://www.nikolairadke.de/NOKO/noko_klein.png) NOKO

Das **NOKO-Monster** zum Nachbauen und gern haben. Beschreibung und Anleitungen dazu im [Wiki](https://github.com/NikolaiRadke/NOKO/wiki). Jeder, der Lust hat, darf sich gerne beteiligen, ich bin für Vorschläge stets (Quell-)offen. 

<table border="0">
  <tr>
    <td><img src="http://www.nikolairadke.de/NOKO/noko_nr2.png" /></td>
    <td>
    NOKO ist ein multifunktionales Monster mit einem Arduino Nano als Mikrocontroller, Display, Uhr, Radio, MP3-Player,            Bedienelementen, Akku, Sprachset und vielen Gimmicks, verpackt in ein knuffiges Monsterkostüm - stabil, liebenswert und        manchmal auch lästig. Ganz, wie man ihn haben möchte. Er ist ein Kumpane für's Leben und ein Begleiter am Tag und in der
    Nacht. Alles, was er braucht, ist ab und zu ein bisschen Strom.<br />
    <br>
    NOKO soll unterhalten und nützlich sein, wird sich aber auch einmischen, denn sonst wird ihm schnell langweilig.<br />
    <br />
    <b>NOKO</b> steht für <b>NO</b>ras und ni<b>KO</b>s Monster
    </td> 
  </tr>
</table>

###Hints in english 
*Anything sourcode-related, the circut diagrams and comments on how to compile are written in english, the building documentation and the manual are in german. Feel free to ask me for translation, if you need help. NOKO itself speaks and writes in german. However, someday, he may learn english or other languages... contribute!
Requirements: The [Arduino IDE 1.6.7](https://www.arduino.cc/en/Main/Software) or [Arduino IDE 1.6.6](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous). Put the folder NOKO_Arduino_NOKO/ into your sketch folder. Right now, NOKO.ino won't compile without changing the local "platform.txt". See [HOWTO compile](https://github.com/NikolaiRadke/NOKO/tree/master/HOWTO_Compile) and change the file. Now NOKO.ino should compile.*

###Voraussetzungen
NOKO benötigt die [Arduino IDE 1.6.7](https://www.arduino.cc/en/Main/Software) oder [Arduino IDE 1.6.6](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous). Das Verzeichnis NOKO_Arduino/NOKO/ muss in den Sketch-Ordner kopiert werden. Noch kann NOKO.ino nicht kompiliert werden. Dazu muss die Datei **platform.txt** angepasst werden: [HOWTO Compile](https://github.com/NikolaiRadke/NOKO/tree/master/HOWTO_Compile). Nun sollte sich NOKO.ino kompilieren lassen.

### Verzeichnisse

```
NOKO/
├── Anleitung/
|   Die Anleitung zu NOKO als PDF und eine Verpackungsvorlage.
|   ├── scribus/
|       Anleitungsquelldateien für Scribus mit allen Illustrationen und Schriften.
|
├── HOWTO_Compile/
|   Hinweise und angepasste plattform.txt für die Kompilierung in der Arduino IDE.
|
├── MP3/
|   Sprachdateien für NOKO.
|          
├── Schaltplan/
|   Schaltpläne für NOKO
|   Schaltpläne der NOKO-Module. 
|   ├── geda/
|       Rohe Schaltpläne für gEDA.
|
├── Write_EEPROM/
|   Hilfsprogramme, um Daten über die serielle Verbindung zum Arduino und den EEPROMs zu senden. 
|
├── src/
    Arduino-Sketche
    ├── NOKO/
    |   Arduino-Quelltext mit allen Bibliotheken.
    ├── NOKO_Disk1/
    |   Arduino-Sketch zum Beschreiben des 24LC256-EEPROMs.
    ├── NOKO_EEPROM_Disk0/
        Arduino-Sketch zum Schreiben der Grundeinstellungen in das Arduino-EEPROM und zum Stellen 
        der Echtzeituhr. Optional zum Beschreiben des AH24C32-EEPROMs auf dem Modul der Echtzeituhr.
```
###Los geht's!
Interesse an NOKO? Fein. Hier geht es zum [Wiki](https://github.com/NikolaiRadke/NOKO/wiki).
