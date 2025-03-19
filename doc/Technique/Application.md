 Une application est un objet qui permet de rendre flexible :
- l'accès à des ressources par des composants/processus
- l'exécution de code relatif à une myriade d'objets possédant les mêmes caractéristiques (composants et archétypes) sans perdre en optimisation
- l'interaction de ces objets entre eux, et interaction avec les autres objets d'une scène

L'idée est de manipuler des requêtes qui contiennent en cache RAM (et possiblement L1/L2 CPU) les entités manipulés, dans un tableau d'identifiants.