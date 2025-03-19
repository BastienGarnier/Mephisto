	
Les requêtes doivent pouvoir être programmé sous plusieurs formes, c'est-à-dire selon plusieurs structures de données. En effet, selon les cas d'application (ex. collision, systèmes de particules, etc...), la structure de donnée utilisée pour la requête peut être soit performante (ex. tableaux pour système de particules) soit particulièrement inefficiente (ex. tableaux pour collisions)

Une structure de données pour requête doit pouvoir être posséder les fonctionnalités suivantes au minimum :
- parcours de toutes les entités

Les différentes structures de données envisagées ici sont :
- Tableau de tableaux à trous chaînés (comme les ComponentBuffer mais pour les entités)
- Hiérarchie des volumes englobants


L'idée est d'indiquer le type de structure de donnée utilisé lors de l'instanciation :
```C++
// Définition de la requête
EntityQuery<Structure<EntityId>> q;
```
On peut dans une requête vouloir accéder aux entités autour d'une entité.
Il faut donc effectuer une itération sur les instances de la structure qui contiennent l'entité :
```C++
// Dans un processus
for (Instance &i : q) {
	Entity &e = i.e;
	// actions
}
```
L'utilisation de *i* peut dans certains cas permettre de "remonter" dans la structure. Par exemple, pour obtenir les objets proches, on peut remonter dans l'arbre AABB grâce aux données supplémentaires de l'instance (manipulations peuvent être wrappés comme des méthodes de l'instance, histoire que le programmeur n'ait pas à se soucier de l'implantation).
