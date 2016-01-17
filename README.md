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
<tr><td>3</td><td>ZC</td><td>Entrée de détection du passage à zéro du réseau basse-tension, il s'agit d'un signal binaire qui passe à l'état haut lorsque le réseau est sur son alternance négative<sup>2</sup></td></tr>
<tr><td>4</td><td>GND</td><td>Masse</td></tr>
<tr><td>5</td><td>SDA</td><td>Signal de données du bus I²C (entrée-sortie), doit être équipée d'une résistance de pull-up reliée à VCC<sup>3</sup></td></tr>
<tr><td>6</td><td>AD0</td><td>Bit de poid faible de l'adresse I²C</td></tr>
<tr><td>7</td><td>SCL</td><td>Signal d'horloge du bus I²C (entrée-sortie), doit être équipée d'une résistance de pull-up reliée à VCC<sup>3</sup></td></tr>
<tr><td>8</td><td>VCC</td><td>Tension d'alimentation (2,7 à 5,5 V)</td></tr>
</table>

1. La polarité de la broche __GIFAM__ peut être modifiée en modifiant la constante __GIFAM_POL __dans le fichier config.h et en recompilant le firmware.
2. La polarité de la broche __ZC__ peut être modifiée en modifiant la constante __ZC_POL__dans le fichier config.h et en recompilant le firmware.
3. La valeur de la résistance est calculée conformément au [standard I²C](http://www.ti.com/lit/an/slva689/slva689.pdf), typiquement 4,7k&#x2126; convient très bien pour un système alimenté en 5V (3,3k&#x2126; pour un système 3,3V)

##Contrôle par l'interface I²C
Le __gifam-chip__ se comporte comme un esclave I²C, son adresse<sup>1</sup> est :
<table>
<tr><th>0</th><th>0</th><th>1</th><th>0</th><th>0</th><th>0</th><th>AD0</th><th>RW</th></tr>
</table>

AD0 correspond à l'état de la broche correspondante et permet donc de contrôler 2 gifam-chip sur un même bus I²C.

1. L'adresse de base peut être modifiée en modifiant la constante __TWI_BASE_ADDR __dans le fichier config.h et en recompilant le firmware.

Une écriture permet de modifier le mode GIFAM, une lecture de le lire. 

L'octet lu ou écrit peut prendre les valeurs suivantes:

<table>
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

##Carte d'évaluation
Le schéma complet de la carte d'évaluation __gifam-chip-evb__ est disponible dans le dossier [hardware/](http://gitweb.epsilonrt.com/gifam-chip.git/tree/HEAD:/hardware) au format Proteus 7 avec son PCB et au format PDF dans le dossier [doc/img/](http://gitweb.epsilonrt.com/gifam-chip.git/blob_plain/HEAD:/doc/img/gifam-chip-evb.pdf)

###Détection de zéro
![Alimentation capacitive](http://gitweb.epsilonrt.com/gifam-chip.git/blob_plain/HEAD:/doc/img/zcross.png)

###Alimentation capacitive sans transformateur
![Alimentation capacitive](http://gitweb.epsilonrt.com/gifam-chip.git/blob_plain/HEAD:/doc/img/powersupply.png)

###Interrupteur commandé
![Alimentation capacitive](http://gitweb.epsilonrt.com/gifam-chip.git/blob_plain/HEAD:/doc/img/acswitch.png)

