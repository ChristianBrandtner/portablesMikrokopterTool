# PKT #

Dokumentation wird oft unterschaetzt! :-)

### TODO (followme.c) ###

* Test FollowMe mit Offset und Kopter, mit Simulator hats geklappt
* Erweiterung: relatives FollowMe zwischen Kopter und PKT
* vorgehen im PKT:
* - Im FollowMe Modus warten bis PH erkannt wird
* - Sobald PH erkannt wird aus den Koordinaten des Kopters und des PKTs ein Offset berechnen und abspeichern
* - Wenn CH im Kopter vom PKT erkannt wird FollowMe Daten an den Kopter senden mit dem gespeicherten Offset



Ein Wiki hier waere für die Entwicklung sinnvoll, um z.B. auf die EEprom Thematik hinzuweisen für "neue" Entwickler. Da gibts bestimmt noch einiges andere. Ich mache mir mal Gedanken dazu.

Oliver Gemesi (Olli42) werde ich auch noch hier mit einbinden, er hat massgeblich die neuen Menüroutinen, Abwärtskopatibilität und andere wichtige Dinge eingebaut.

### pkt.c, pkt_debugserial.c ###
frühere Ergänzungen nachgezogen.
Erweiterung um serielle Debugausgabe zum Testen von Funktionen, Ausgabe von berechneten Werten z.B.

### setup.c, eeprom.c, lcd.c ###
die LCD Orientation Funktion wird von einem User doch benötigt, deshalb kommt die Funktion wieder ins Setup


### Status ###
Offset mit Kopter setzten hinzugefuegt. **"Noch nicht wirklich getestet"**
Cebra: Codesatz Einstellungen bei mir geändert wegen Umlautproblem, erstmal zum testen...
