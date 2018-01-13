# PYROFB-Arduino

Pour compiler le code il faut téléchargé la library EnableInterrupt.h

Croquis\Inclure une bibliothéque ...\Gérer les bibliothèques

et rechercher EnableInterrupt la version 0.9.8 est fonctionnelle.

Merci à Mike "GreyGnome" Schwager pour ce partage.

Programmer le Arduino avec le code dans Sketch.ino

Les BUS-INFO-OUT des séquenceurs doivent être connectés à la pin 4,5,6,7 de l'arduino

Connecté une GND du séquenceur vers l'arduino

ouvrir le moniteur série RS232 de l'IDE Arduino

Chaque changement d'état ARMED/GO/STOP/END doit être tracé

En plus de l'état du séquenceur sur le moniteur série, il est possible de brancher des LED en matrice:
C1, C2, C3, C4: 10, 11, 12, 13
L1, L2, L3, L4:	0, 1, 2, 3

Afin de pouvoir visualiser l'état des séquenceurs.
