# Reverse Engineering von Funkprotokollen

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
    - [ABCD on/off](steckdose/signals/abcd_onoff.zip)
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
- **Companion Fehler**: Der GNU Radio Companion kann den Flowgraph nicht korrekt bauen, wenn er falsche Blockbeschreibungen geladen hat. Nach jedem Build sollten die Blöcke über des Reload-Icon (rechts in der Top-Bar) neu geladen werden.
- **Import Fehler**: Python findet das Modul nicht. Überprüfe den bei `sudo make install` ausgegebenen Pfad und ergänze ihn ggf. in die Umgebungsvariable `PYTHONPATH`
- **Property does not exist**: Dies bedeutet zumeist, dass es ein Fehler bei den Python-Bindings gibt. Meist passiert das durch eine nicht gefundene Bibliothek. Führe `sudo ldconfig <DIR>` aus, wobei du das Verzeichnis der Bibliothek entsprechend einfügst (bei mir z.B. `/usr/local/lib`).

# Reverse Engineering of RF protocols

We have been using wireless devices every day since multiple year. Especially with the rise of the Internet of Things (IoT), we can ask the question: 
Are all these devices really sending only the information the manufactors state? And how simply can we listen to this?

In this presentation we look take a look at the basics of wireless communication, and how we can analyze it using Software Defined Radios (SDR).
Afterwards we use our newly learned knowledge in practice by analyzing two different devices. 
In the end I present some ideas, how you can deepen your knowledge in this topic.

## Contents

- [Slides (in German)](slides.pdf)
- [Remote sockets](steckdose/)
  - [Custom GNU Radio Blocks](steckdose/gr-brennenstuhl/) (Demodulator, Dekodierer, Modulator)
  - [Flowgraphs](steckdose/flowgraphs/)
  - [Signals](steckdose/signals/)
    - [A on](steckdose/signals/a_on.zip)
    - [C on DIP 10101](steckdose/signals/c_on.zip)
    - [ABCD on/off](steckdose/signals/abcd_onoff.zip)
    - [A on, DIP varying](steckdose/signals/dip.zip)
  - [Notes](steckdose/notes/)
- [Remote thermometer](thermometer/)
  - [Custom GNU Radio Blocks](thermometer/gr-globaltronics/) (Demodulator, Dekodierer, Modulator)
  - [Flowgraphs](thermometer/flowgraphs/)
  - [Signals](thermometer/signals/)
    - [First Test (1 MHz)](thermometer/signals/first_test.zip)
    - [Second Test (200 kHz)](thermometer/signals/second_test.zip)
    - [Water (startint at 90° C, 200 kHz)](thermometer/signals/water.zip)
  - [Notes](thermometer/notes/)
- Practical tips (see below)

## Practical tips

### Build of modules

GNU Radio modules can be build using the following commands:
```
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig
```

The most prevalant reasons for this to fail, is CMake not finding dependencies.
There are optional dependencies (like Doxygen), that won't matter, but for mandatory packets I recommend using your favorite search engine to find the right packet for your distro.

### Usage of custom blocks

There can be different issues, when using custom blocks:
- **Companion Errors**: The GNU Radio Companion can't build the flowgraph correctly, when it has loaded outdated information. Use the Reload-Icon (in the top bar on the right) to reload all the blocks.
- **Import Error**: Python doesn't find the module. Check the paths printed by `sudo make install` and put it in the `PYTHONPATH` environment variable.
- **Property of module does not exist**: This indicates an error concerning the Python bindings, which is often caused if a shared library is not found. Execute `sudo ldconfig <DIR>` using the directory containing the shared library of the custom module (personally I need to use `/usr/local/lib` here).


&copy; 2020 Ben Swierzy

