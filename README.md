# Reverse Engineering von Funkprotokollen

*TODO: English version below*

Funkende Gegenstände sind längst Teil unseres Alltags geworden. Insbesondere durch den Aufschwung des Internet of Things (IoT) stellt sich aber immer mehr die Frage: Senden die ganzen Geräte wirklich nur das, was sie auch sollen? Und kann man da einfach ungefragt mithören?

In diesem Vortrag schauen wir uns im ersten Teil die Grundlagen drahtloser Kommunikation an. Ergänzend dazu betrachten wir, wie sich nahezu beliebige drahtlose Kommunikation mit Hilfe von Software Defined Radios untersuchen lässt. Im zweiten Teil nutzen wir unsere Kenntnisse in der Praxis zum Reverse Engineering von zwei Funkprotokollen. Und falls ihr noch nicht genug hattet, gibt es abschließend einige Anreize, wie ihr selbst Hand anlegen könnt.

## Inhalte des Repositories

- [Folien des Vortrags](slides.pdf)
- [Dokumente zu Funksteckdosen](steckdose/)
  - [Implementierung eigener Blöcke](steckdose/gr-brennenstuhl/) (Demodulator, Dekodierer, Modulator)
  - [Flowgraphs](steckdose/flowgraphs/)
  - [Signale](steckdose/signals/)
    - [A on](steckdose/signals/a_on.zip)
    - [C on DIP 10101](steckdose/signals/c_on.zip)
    - [ABCD on/off](stackdose/signals/abcd_onoff.zip)
    - [A on, DIP variiert](steckdose/signals/dip.zip)
  - [Notizen](steckdose/notes/)
- [Dokumente zum Funkthermometer](thermometer/)
  - [Implementierung eigener Blöcke](thermometer/gr-globaltronics/) (Demodulator, Dekodierer, Modulator)
  - [Flowgraphs](thermometer/flowgraphs/)
  - [Signale](thermometer/signals/)
    - [Erster Test (1 MHz)](thermometer/signals/first_test.zip)
    - [Zweiter Test (200 kHz)](thermometer/signals/second_test.zip)
    - [Das Wasser (Start bei über 90°, 200 kHz)](thermometer/signals/water.zip)
  - [Notizen](thermometer/notes/)
- Praktische Tipps (siehe unten)

## Praktische Tipps

### Bauen von Modulen

GNU Radio Module können durch folgende Befehle gebaut werden:
```
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig
```

Hierbei können im Wesentlichen der Fehler auftreten, dass CMake Abhängigkeiten nicht findet. 
Es gibt optionale Abhängigkeiten (z.B. Doxygen) die egal sind, für notwendige Abhängigkeiten sollte man die Suchmaschine seiner Wahl nutzen um das zugehörige Paket ausfindig zu machen.

### Benutzung von eigenen Blöcken

Es kann zu verschiedenen Problemen beim Benutzen von eigenen Blöcken kommen.
- **Import Fehler**: Python findet das Modul nicht. Überprüfe den bei `sudo make install` ausgegebenen Pfad und ergänze ihn ggf. in die Umgebungsvariable `PYTHONPATH`
- **Property does not exist**: Dies bedeutet zumeist, dass es ein Fehler bei den Python-Bindings gibt. Meist passiert das durch eine nicht gefundene Bibliothek. Führe `sudo ldconfig <DIR>` aus, wobei du das Verzeichnis der Bibliothek entsprechend einfügst (bei mir z.B. `/usr/local/lib`).

&copy; 2020 Ben Swierzy

