#!/usr/bin/python3
# -*- coding: utf-8 -*-
# create_nokolist.py
# ==================
# Version 1.0
# -----------

import sys, os, subprocess
from os import walk
from subprocess import call
import glob
import re

# index ist der Name der ersten MP3 Datei, die erstellt wird
# Bei jeder weiteren MP3 Datei wird dieser Wert erhöht. Die
# erste MP3 Datei muss (logischerweise) um eins größer sein,
# als die letzte NOKO Sprachdatei (Stand 28.2.18 -> 226.mp3)
# Ich habe hier 250 ('just in case') gesetzt
index = 250

# Überprüfung der eingegebenen Parameter
# Parameter 1: Quellverzeichnis (wo liegen die MP3s?)
# Parameter 2: Zielverzeichnis (wo sollen die fertigen MP3s hingeschrieben werden?)
# Parameter 3: Textdatei mit den Namen für den NOKO EEPROM

if len(sys.argv) < 4:
    print("Zu wenig Parameter übergeben:")
    print("create_nokolist.py <quellordner> <zielordner> <zieldatei>")
    exit()

# Überprüfung ob die übergebenen Argumente existieren
# Die Verzeichnisse müssen existieren, sonst wird abgebrochen!
# Die Textdatei darf NICHT exiistieren, sond wird abgebrochen!
if not os.path.isdir(sys.argv[1]):
    print("Quellverzeichnis", sys.argv[1], "existiert nicht!")
    exit()
if not os.path.isdir(sys.argv[2]):
    print("Zielverzeichnis", sys.argv[2], "existiert nicht!")
    exit()
if os.path.isfile(sys.argv[3]):
    print("Datei", sys.argv[3], "existiert!")
    exit()

# Einlesen der Ordnernamen des Quellverzeichnisses
d = []
for (dirpath, dirnames, filenames) in walk(sys.argv[1]):
    d.extend(dirnames)
    break

# In welchem Verzeichnis fangen wir an?
directory_to_check = sys.argv[1]

# Definiere unsere MP3 Wrap Funktion
def mp3_wrap(directory,index):
    # cwd = os.getcwd()
    # print("cwd ===>>>", cwd, "Index ===>>>", index)
    # Alphabetisches sortieren der MP3 Dateinamen
    file_list = sorted(glob.glob("./*mp3"))
    if file_list == []:
        return()
    # Hier wird ein Subprocess erstellt mit den entsprechenden Argumenten. mp3wrap muss installiert sein!
    p1=subprocess.Popen(['mp3wrap', '-v', sys.argv[2].strip() + '/' + str(index).strip() + '.mp3'] + file_list)
    # Hier wird dieses Kommando dann ausgeführt
    output = p1.communicate()[0]
    
# ====================
# === MAIN PROGRAM ===
# ====================

# Das Verzeichnis, dass wir durchsuchen wollen
directory_to_check = sys.argv[1]

# Finde alle Unterverzeichnisse und speichere sie in einer Liste:
directories = [os.path.abspath(x[0]) for x in os.walk(directory_to_check)]

# Wir wollen das "Haupt"verzeichnis (erstes Argument) nicht durchsuchen,
# höchstwahrscheinlich werden sich da keine MP3s drin befinden, daher entfernen
directories.remove(os.path.abspath(directory_to_check))

# Erstellen der Textdatei für den NOKO EEPROM
f = open(sys.argv[3],'w')
for dirname in sorted(directories):
    # Erst einmal in das Verzeichnis gehen.
    os.chdir(dirname)
    # Wenn hier keine MP3 Dateien im Verzeichnis zu finden sind, dann gehe weiter
    if sorted(glob.glob("./*mp3")) == []:
        continue
    # Wenn MP3 Dateien vorhanden sind, dann schreibe den Verzeichnisnamen in die Textdatei. Und zwar schön! ;)
    # hsname soll HörSpielName heißen
    hsname = re.sub('^.*\/', '', dirname)
    # Hier wird der String in 2 x 20 Zeichen aufgeteilt und zentriert. Pure Python Magie!!!
    print(('{s:{c}^{n}}'.format(s=hsname[:20],n=20,c=' ')), '{s:{c}^{n}}'.format(s=hsname[20:40],n=20,c=' '), file=f, end='', sep='')
f.close()

# Hier gehen wir durch alle gefundene Verzeichnisse und rufen unsere
# MP3_wrap Funktion auf
for i in sorted(directories):
    index = index + 1
    os.chdir(i)         # Change working Directory
    mp3_wrap(i,index)      # Run your function

exit()

