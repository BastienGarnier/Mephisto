Les entités utilisent des ressources. Toutes les ressources utilisées par le jeu doivent être optimisées en accès (lecture/écriture/etc...)

Pour cela, on considère un ressource manager qui stockera :
- fenêtres
- textures/cubemaps
- modèles 3D (références vers les buffers de sommets)
- fichiers ouverts sur le disque
- animations (graphes, squelettes, etc...)
Cela permet de sécuriser les accès et notamment:
- de faciliter la libération et l'allocation de la mémoire
- de ne stocker qu'une fois chaque ressource
- de n'avoir qu'un lieu d'accès aux données
Il faut alors que les ressources puissent être associées aux entités utilisées dans les scènes. Plus précisément, les ressources doivent êtres associées aux composants associées aux entités. Par exemple, un composant de rendu doit avoir associé un maillage, des textures, etc... (i.e. un modèle), des animations, etc..., une pipeline de rendu, etc...

Il faut donc avoir un système d'association des données dans les composants, spécifique à chaque composant décrit, qui associe à certains champs spécifiques au composant les pointeurs vers les données voulues.
### Analyse de propriétés des composants
L'affichage d'une entité est le point fondamental d'un moteur de jeu, quelque soit la nature de cet affichage (console, graphique 2D ou 3D).
L'analyse des propriétés d'un système permettant l'affichage d'une entité permet donc de déterminer certaines premières propriétés nécessaires au bon fonctionnement des composants.

Les ressources doivent être triés pour optimiser l'affichage => les composants associées aux entités ne sont pas traités dans ordre quelconque, mais dans un ordre spécifique aux données qu'ils contiennent.
Cela signifie en particulier que le traitement des entités ne doit pas s'effectuer entité par entité mais type de composant par type de composant. Chaque type de composant possède un comportement global, qui affecte la manière donc il s'exécute sur l'ensemble des entités associées (exemple du tri des maillages et textures avant l'affichage local), et un comportement local, qui affecte un sous-ensemble d'entités associées (exemple de l'affichage de chaque entité unique). On peut considérer le cas du comportement local comme un cas particulier du comportement global.

Si plusieurs entités possèdent le même composant, elles doivent être traités par un même processus qui puisse coordonner son exécution vis-à-vis de chacune d'entre elles.

Finalement, le "composant" d'une entité spécifique n'est qu'un ensemble de données associées à cette entité qui se trouve associé à un processus qui utilise ces données.

Une entité est un conteneur identifié de composants traités chacun individuellement par un processus. L'entité n'est donc qu'un identifiant auquel est associé un ensemble de composants dont chacun subit un traitement par un processus.

**Finalement**, les composants véritablement sont les données composantes de l'entité.
Chaque entité est un ensemble identifié de composants, qui sont des données balisées utilisées par des processus exécutant une logique spécifique sur la base des données des composants. Une entité peut être parente d'autres entités. Cela permet donc de construire une arborescence d'entités, et donc des sous-mondes relatifs.
Une *scène* est la donnée d'un ensemble de processus déterminant des actions régissant l'évolution de données assemblées/unies en composantes. Ces composantes peuvent être "composées" grâce à des identifiants appelés entités.

**Idée :** Certains processus peuvent nécessiter plusieurs composants pour leur bon fonctionnement. En particulier, cela permet d'unir certains processus autour de mêmes composants. Par exemple, l'affichage d'une entité et son traitement physique sont deux processus qui utiliseront chacun la position/rotation de l'entité.
**Question :** Les données des composantes sont-elles considérées comme des ressources quelconques ?
Il serait intéressant de stocker toutes les données d'un type de composant dans la même région de la mémoire, pour permettre optimisation du CPU puisque les processus manipuleront les données des composants par type, et donc au même instant.

## Problème des données globales

Pour Vulkan, initialisation physique n'est effectuée qu'une seule fois. Données de PhysicalDevice, graphics present queues, swapchainsupport details, etc... n'apparaissent qu'une seule fois dans le code, et sont indépendants des instances de rendus (i.e. des fenêtres).

On remarque aussi que certains composants sont naturellement choisis pendant la programmation, et ne doivent donc pas être *calculés* mais déterminés avant même la compilation. On peut penser par exemple au système de rendu.

En fait, ces données ne doivent pas être globales, mais unique pour le processus (c'est-à-dire qu'il s'agit de données du processus et non pas de données d'entités).

Par exemple, l'affichage 3D est un processus sur des entités, et les données spécifiques à cet affichage doivent lui être attribué.

# Compile time components

On connaît les types de composant à la compilation. On doit donc pouvoir les numéroter à la compilation. On peut alors utiliser  ces numéros comme identifiants.
