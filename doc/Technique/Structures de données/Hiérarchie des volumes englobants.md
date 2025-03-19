
L'idée est de construire une hiérarchie d'encapsulation des entités. Par exemple, une voiture est englobée dans un pavé, et toutes les entités internes à cette voiture sont elles-mêmes englobées dans des pavés à l'intérieur de ce pavé.

**Exemple :** Pour les collisions dans un espace 3D, on n'a alors qu'à chercher dans l'encapsulation juste supérieure à un objet pour détecter ses collisions possibles.

L'idée de généralisation est que l'espace doit pouvoir être défini selon n'importe quel composant, et l'encapsulation également.

Dans le cas de la collision, il faut les données de position, et le calcul de la "capsule" selon un modèle 3D.