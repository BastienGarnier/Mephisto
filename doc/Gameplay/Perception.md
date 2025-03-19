Chaque personnage a deux moyens de perception :
- auditif
- visuel
# Détail du visuel

Chaque émission de lumière est simplement une valeur en un point $(x; y)$. Il faut prendre en compte la diffraction.

On commence par émettre selon sommets des objets 2D (on peut mettre des light-maps correspondant à des masques vectoriels de ces objets en moins précis, pour avoir moins de calcul à faire) avec une luminosité $L_{1}(d)$ décroissante selon $d$.

Ensuite, on part de chaque extrémité rencontrée de distance $d_{e}$ de la source initiale et on la considère comme un source lumineuse à part entière de luminosité $L_{1}(d_{e})$. On l'ajoute alors à tous les pixels dont la luminosité est inférieure à celle de $L_{1}(d_{e})$.

Il doit s'agir d'une simulation du phénomène de diffraction : https://culturesciencesphysique.ens-lyon.fr/ressource/simu-diffraction.xml

On peut donc construire cela sur un masque de lumière qui est ensuite affiché par dessus l'écran.
# Détail de l'auditif

L'émission du son suit exactement le même schéma que celui de la lumière (phénomène ondulatoire). On a simplement à ne pas afficher graphiquement l'évolution du son mais à utiliser les valeurs du masque pour le comportement des personnages de manière générale.

Dans le cas du joueur spécifiquement, le son est joué à la valeur du masque en la position du joueur.