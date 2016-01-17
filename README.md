#Gifam Chip
__Gifam chip__ permet la commande d'un radiateur électrique équipé d'un fil pilote à l'aide d'un microcontroleur ATMEL AVR [attiny45](http://www.atmel.com/devices/attiny45.aspx) à 8 broches. La commande par fil pilote d'un radiateur est décrite sur <http://www.radiateur-electrique.org/fil-pilote-radiateur.php>

Le microcontroleur __attiny45__ reçoit ses ordres par l'intermédiaire d'un bus I²C. Il peut être alimenté entre 2,7 et 5,5 V ce qui lui permet d'être commandé par n'importe quel système équipé d'un bus I²C sur cette plage de tension (Raspberry Pi, Arduino ...). La vitesse d'horloge maximale est de 400 kHz.

Le projet fournit aussi le schéma d'une carte d'évaluation permettant de prendre en main rapidement gifam-chip.

__gifam-chip__ est Copyright © 2013-2016 epsilonRT 
<epsilonrt@btssn.net>, All rights reserved.

Les sources du firmware __gifam-chip__ et du hardware de la carte d'évaluation sont diffusées sous licence open source
CeCILL <http://www.cecill.info>.

## Spécifications éléctriques
Les spécifications électriques sont celles du [attiny45](http://www.atmel.com/devices/attiny45.aspx) et peuvent être consultées dans le paragraphe correspondant de son [datasheet](http://www.atmel.com/Images/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf). 

## Brochage et fonctions des broches

![Brochage](http://gitweb.epsilonrt.com/gifam-chip.git/blob_plain/HEAD:/doc/img/pinout.png)

Le firmware par défaut définit les fonctions de broches de la façon suivante:
<table>
<tr><th>#</th><th>Nom</th><th>Description</th></tr>
<tr><td>1</td><td>RESET</td><td>Broche de RESET active à l'état bas, doit être équipée d'une résistance de pull-up de 10 k&#x2126; reliée à VCC</td></tr>
<tr><td>2</td><td>GIFAM</td><td>Signal de découpage de la tension permettant de générer l'ordre sur le fil pilote <sup>1</sup></td></tr>
<tr><td>3</td><td>ZC</td><td>Entrée de détection du passage à zéro du secteur, il s'agit d'un signal binaire qui passe à l'état haut lorsque le réseau est sur son alternance négative<sup>2</sup></td></tr>
<tr><td>4</td><td>GND</td><td>Masse</td></tr>
<tr><td>5</td><td>SDA</td><td>Signal de données du bus I²C (entrée-sortie), doit être équipée d'une résistance de pull-up reliée à VCC<sup>3</sup></td></tr>
<tr><td>6</td><td>AD0</td><td>Bit de poid faible de l'adresse I²C</td></tr>
<tr><td>7</td><td>SCL</td><td>Signal d'horloge du bus I²C (entrée-sortie), doit être équipée d'une résistance de pull-up reliée à VCC<sup>3</sup></td></tr>
<tr><td>8</td><td>VCC</td><td>Tension d'alimentation (2,7 à 5,5 V)</td></tr>
</table>

1. La polarité de la broche __GIFAM__ peut être modifiée en modifiant la constante __GIFAMOUT_POL__ dans le fichier config.h et en recompilant le firmware.
2. La polarité de la broche __ZC__ peut être modifiée en modifiant la constante __ZCROSS_POL__ dans le fichier config.h et en recompilant le firmware.
3. La valeur de la résistance est calculée conformément au [standard I²C](http://www.ti.com/lit/an/slva689/slva689.pdf), typiquement 4,7k&#x2126; convient très bien pour un système alimenté en 5V (3,3k&#x2126; pour un système 3,3V)

##Contrôle par l'interface I²C
Le __gifam-chip__ se comporte comme un esclave I²C, son adresse<sup>1</sup> est :
<table>
<tr><th>0</th><th>0</th><th>1</th><th>0</th><th>0</th><th>0</th><th>AD0</th><th>RW</th></tr>
</table>

AD0 correspond à l'état de la broche correspondante et permet donc de contrôler 2 gifam-chip sur un même bus I²C.

1. L'adresse de base peut être modifiée en modifiant la constante __TWI_SLAVE_ADDR__ dans le fichier config.h et en recompilant le firmware.

Une écriture permet de modifier le mode GIFAM, une lecture de le lire. Ce qui donne les échanges suivants:

<table>
<caption align="left"><b>Echange en écriture</b></caption>
<tr><th>S</th><th>0</th><th>0</th><th>1</th><th>0</th><th>0</th><th>0</th><th>AD0</th><th>0</th><th>A</th><th>D7</th><th>D6</th><th>D5</th><th>D4</th><th>D3</th><th>D2</th><th>D1</th><th>D0</th><th>A</th><th>P</th></tr>
<tr><td></td><td colspan="8" align="center"><b>Slave Address Byte</b></td><td></td><td colspan="8" align="center"><b>Mode Byte</b></td><td></td><td></td></tr>
</table>

<table>
<caption align="left"><b>Echange en lecture</b></caption>
<tr><th>S</th><th>0</th><th>0</th><th>1</th><th>0</th><th>0</th><th>0</th><th>AD0</th><th>1</th><th>A</th><th>D7</th><th>D6</th><th>D5</th><th>D4</th><th>D3</th><th>D2</th><th>D1</th><th>D0</th><th>A</th><th>P</th></tr>
<tr><td></td><td colspan="8" align="center"><b>Slave Address Byte</b></td><td></td><td colspan="8 align="center"><b>Mode Byte</b></td><td></td><td></td></tr>
</table>

- __S__ Condition de start
- __P__ Condition de stop
- __A__ Accusé réception (ACK)

L'octet lu ou écrit (Mode byte) peut prendre les valeurs suivantes:

<table>
<caption align="left"><b>Mode byte</b></caption>
<tr><th>#</th><th>Mode</th><th>Commande sur le fil pilote</th></tr>
<tr><td>0</td><td>Confort</td><td>Pas de signal (neutre ou en l'air)</td></tr>
<tr><td>1</td><td>Arret forcé</td><td>Alternance positive</td></tr>
<tr><td>2</td><td>Hors Gel</td><td>Alternance négative</td></tr>
<tr><td>3</td><td>Economique</td><td>Signal complet (alternance positive et négative)</td></tr>
<tr><td>4</td><td>Confort -1 °C</td><td>Signal complet 3'' puis pas de signal pendant 4'57''</td></tr>
<tr><td>5</td><td>Confort -2 °C </td><td>Signal complet 7'' puis pas de signal pendant 4'53''</td></tr>
</table>


###Exemple de programme
Les exemples ci-dessous effectuent tous un passage en revue des modes 0 à 3 (Confort, Arret forcé, Hors Gel, Economique...). Le code complet de ses exemples est disponible dans le répertoire [doc/examples](http://gitweb.epsilonrt.com/gifam-chip.git/tree/HEAD:/doc/examples)
####ATMEL AVR en C (AvrIO)
[AvrIO](http://www.epsilonrt.com/avrio) est un projet Open Source sous GNU Lesser General Public License Version 3 dont l'objectif est d'offrir une bibliothèque C pour les microcontrôleurs AVR d'Atmel.

On utilise le module twi de AvrIO, par exemple :

    #include <avrio/twi.h>
    #include <avrio/delay.h>

    int main (void) {
      uint8_t mode = 0; // Mode de départ

      // // Initialise le bus I²C pour un accès en maître à 400 kHz
      vTwiInit ();
      eTwiSetSpeed (400);

      for (;;) {

        // Passe en revue les modes 0 à 3 toutes les 5 secondes.
        eTwiWrite (0x20, mode);
        mode++; // incremente le mode
        if (mode == 4) {
          // Si le mode atteint 4, on le remet à zéro
          mode = 0;
        }
        // On attend 5 s. avant de passer au suivant
        delay_ms (5000);
      }
    }

___On notera qu'AvrIO utilise une notation alignée à gauche des adresses I²C.___

####Raspberry Pi en C (SysIO)
[SysIO](http://www.epsilonrt.com/sysio) est un projet Open Source sous CeCILL Free Software License Version 2.1 dont l'objectif est d'offrir une bibliothèque C, C++ et Python pour accèder aux ressources matérielles du système par une interface standardisée. 

On utilise le module i2c de SysIO, par exemple :

    #include <sysio/i2c.h>
    #include <sysio/delay.h>

    int main (void) {
      int chip;
      uint8_t mode = 0; // Mode de départ

      // Ouvre une connexion I2C vers le gifam-chip  
      chip = iI2cOpen ("/dev/i2c-1", 0x10);

      for (;;) {

        // Passe en revue les modes 0 à 3 toutes les 5 secondes.
        iI2cWrite (chip, mode);
        mode++; // incremente le mode
        if (mode == 4) {
          // Si le mode atteint 4, on le remet à zéro
          mode = 0;
        }
        // On attend 5 s. avant de passer au suivant
        delay_ms (5000);
      }
    }

####Arduino
On peut utiliser la librairie [Wire](https://www.arduino.cc/en/Reference/Wire) de l'Arduino, par exemple :

    #include <Wire.h>

    void setup() {
      Wire.begin(); // Initialise le bus I²C pour un accès en maître
    }

    byte mode = 0;

    void loop() {
   
      // Passe en revue les modes 0 à 3 toutes les 5 secondes.
      Wire.beginTransmission(0x10); // Démarre une transmission vers gifam-chip
      Wire.write(mode);             // Envoi du mode GIFAM
      Wire.endTransmission();       // Fin de transmission

      mode++; // incremente le mode
      if (mode == 4) { 
        // Si le mode atteint 4, on le remet à zéro
        mode = 0;
      }
      // On attend 5 s. avant de passer au suivant
      delay(5000);
    }

##Programmation de l'attiny45

L'utilisation d'un attiny45 comme gifam-chip nécessite une modification des fusibles et une programmation de la mémoire FLASH avec le firmware gifam-chip. Ces deux opérations peuvent être effectuées avec un programmateur ISP (comme AVR Dragon) et un logiciel idoine (AVR Dude, AVR Studio ...).

###Configuration des fusibles du attiny45
Le microcontrôleur [attiny45](http://www.atmel.com/devices/attiny45.aspx) est paramétré de la façon suivante :

- Oscillateur RC à 8 MHz (CKDIV8 unprogrammed -> 1)
- Délai de démarrage à l'alimentation de 6 cycles d'horloge plus 14 cycles et 64 ms lors du RESET (défaut).
- Surveillance de l'alimentation (BOD) désactivée (défaut)

Ce qui donne une configuration de l'octet haut des fusibles à 0xDF (valeur par défaut) et de l'octet bas à 0xE2. 

___Il suffit donc de désactiver CKDIV8 lorsqu'on doit paramètrer un [attiny45](http://www.atmel.com/devices/attiny45.aspx) neuf.___

###Programmation du firmware

La dernière version du firmware est disponible dans le dossier [firmware/](http://gitweb.epsilonrt.com/gifam-chip.git/tree/HEAD:/firmware)

###Modification du firmware
Il est possible de modifier la configuration du firmware grâce au fichier [chip/config.h](http://gitweb.epsilonrt.com/gifam-chip.git/blob/HEAD:/chip/config.h)

##Carte d'évaluation
Le schéma complet de la carte d'évaluation __gifam-chip-evb__ est disponible dans le dossier [hardware/](http://gitweb.epsilonrt.com/gifam-chip.git/tree/HEAD:/hardware) au format Proteus 7 avec son PCB et au format PDF dans le dossier [doc/img/](http://gitweb.epsilonrt.com/gifam-chip.git/blob_plain/HEAD:/doc/img/gifam-chip-evb.pdf) :

- J2 asure la liaison I²C vers la carte de contrôle DVKCAN1, Raspberry Pi, Arduino...
- La phase __L__ et le neutre __N__ du secteur sont connectées à J1 ainsi que le fil pilote __G__ relié au radiateur commandé.
- La led D5 indique lorsqu'elle est allumée que le fil pilote est actif, éclairée lorsqu'une alternance est présente  (Arrêt forcé ou Hors gel), fortement éclairée lorsque les 2 alternances sont présentes (Mode Economie).
- Le connecteur ISP J3 permet la programmation du microcontrôleur avec le firmware ou un programme de test. Il permet aussi le débogage pendant les phases de mise au point.
- Le jumper JP1 permet de régler l'adresse de base I²C du gifam-chip. Mis en place, __AD0__ est à l'état bas (retiré à l'état haut) alors que le bouton poussoir SW1 n'est utilisé que pour les tests du gifam-chip.

###Analyse du fonctionnement de la commande secteur

####Détection de zéro
![Alimentation capacitivehttp://gitweb.epsilonrt.com/gifam-chip.git/blob_plain/HEAD:/doc/img/zcross.png)

Le signal de phase du secteur __L__ est transformé en un signal rectangulaire __ZC__ en opposition de phase avec ce dernier. R7, D4 et D7 assure un écrêtage de la tension secteur afin de commander le transistor MOSFET Q1 en saturé/bloqué. L'opto-coupleur U1 assure un isolement galvanique entre le secteur et le microcontrôleur. L'alternance positive du secteur déclencle la saturation de Q1, qui allume la led de l'opto-coupleur U1 et sature le transistor de sortie qui affirme le signel __ZC__ à 0. Lors de l'alternance négative du secteur, Q1 est bloqué, la led de U1 est éteinte et ZC est à l'état haut.

####Alimentation capacitive sans transformateur
![Alimentation capacitive](http://gitweb.epsilonrt.com/gifam-chip.git/blob_plain/HEAD:/doc/img/powersupply.png)

Cette alimentation fournit le +5V nécessaire à l'alimentation de la led de U1 à partir du secteur. Il s'agit d'une alimentation capacitive sans transformateur.
Comme indiqué dans l'article [Alimentation capacitive de Wikipedia](https://fr.wikipedia.org/wiki/Alimentation_capacitive), l'alimentation capacitive est une alimentation électrique qui utilise la réactance capacitive d'un condensateur pour réduire la tension du réseau à une tension plus basse. Une alimentation capacitive est constituée d'un condensateur, ici C2 en parallèle avec C3, dont la réactance limite le courant qui passe à travers les diodes D1 et D2 qui assure un redressement. Comme protection contre les pointes de tension au cours des opérations de commutation, il y a une résistance R5 connectée en série. Un condensateur électrolytique C1 filtre la tension continue afin que la diode zener D3 assure une stabilisation de la tension à 5V. Un calcul complet des éléments est décrit dans le document [AN954 Transformerless Power Supplies: Resistive and Capacitive](http://ww1.microchip.com/downloads/en/AppNotes/00954A.pdf) de Microchip.

####Interrupteur commandé
![Alimentation capacitive(http://gitweb.epsilonrt.com/gifam-chip.git/blob_plain/HEAD:/doc/img/acswitch.png)

Cette partie permet de découper la tension secteur __L__ pour générer le signal pilote __G__. L'opto-triac U3 assure un isolement galvanique entre le secteur et le microcontrôleur. Lorsque le signal __GIFAM__ est à l'état bas, la led de l'opto-triac U3 est allumée ce qui active le triac interne et la gachette du triac U4. R9 limite le courant de gâchette alors que R14 constitue une charge faisant circuler un petit courant dans le circuit de puissance du triac lorsque le signal __G__ n'est connecté  à aucun radiateur. Le signal __GIFAM__ est activé ou non par le microcontrôleur attiny45 en fonction du mode GIFAM souhaité et de l'alternance du secteur en cours (donnée par ZC).
