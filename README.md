# Hausabrechnung Minitool

Das Projekt stellt eine interaktive C++-Konsolenanwendung zur Verteilung von Hausnebenkosten auf mehrere Wohnungen bereit. Es können allgemeine Verbrauchsdaten sowie spezifische Werte pro Wohnung eingegeben, gespeichert und zur Abrechnung herangezogen werden.

## Funktionsumfang

- Erfassung allgemeiner Kennzahlen (Wasser, Gas, Strom und weitere Nebenkosten)
- Verwaltung beliebig vieler Wohnungen mit Angaben zu Fläche, Bewohnerzahl und Zählerständen
- Berechnung der Kostenanteile pro Wohnung auf Basis von Personen, Fläche und Verbrauch
- Speicherung und erneutes Laden aller Daten über die Datei `abrechnung.sav`

## Voraussetzungen

- C++17-kompatibler Compiler
- Unterstützung für die "_s"-Varianten der C-Standardbibliothek (z. B. MSVC oder aktuelle MinGW-Version)

Für Windows liegt eine vorcompilierte Datei `hausabrechnung_minitool_beta_x86.exe` bei.

## Kompilierung

```bash
g++ -std=c++17 full_app.cpp -o hausabrechnung
```

## Ausführung

```bash
./hausabrechnung
```

Beim Programmstart werden ggf. vorhandene Daten aus `abrechnung.sav` geladen. Anschließend führt ein textbasiertes Menü durch alle Schritte: Kennzahlen erfassen, Wohnungen anlegen, Abrechnung durchführen und Ergebnisse speichern.

## Verwendung

1. Starten Sie das Programm und geben Sie die angeforderten allgemeinen Kennzahlen ein.
2. Legen Sie jede Wohnung nacheinander an (Name, Größe, Personenanzahl, Zählerstände).
3. Über das Hauptmenü können Sie die Abrechnung durchführen, Daten speichern oder bearbeiten.
4. Die berechneten Kostenanteile werden pro Wohnung ausgegeben und können bei Bedarf erneut abgerufen werden.

## Lizenz

Dieses Projekt steht unter der [MIT-Lizenz](LICENSE).
