# Mephisto Game Engine

Pourquoi *Mephisto* ? Parce-que c'est l'enfer à coder ! (et que le nom est stylé)

Moteur de jeu en C++ dans un objectif d'apprentissage puis de production amateur de jeux. L'idée est donc d'utiliser le moins d'outils externes et de faire soi-même le plus possible.

Pour le moment, j'utilise les outils suivants :
- C++ 23
- XCB pour X11 (nécessaire pour le fenêtrage sous Linux)
- Vulkan (nécessaire pour l'interaction avec le GPU)
- GCC et Make pour la compilation
- La bibliothèque de calcul vectoriel d'Agner Fog, qui effectue des optimisations de calcul en passant par les instructions SIMD, SSE et AVX (très long à implanter et peu intéressant d'un point de vue pédagogique)

## Généralités sur le système applicatif

On a des composants, qui sont les données évolutives des entités (ex. position, PV, etc...), qui définissent les entités par agrégation. Les entités ne sont donc que des identifiants.
Les ressources servent de données fondamentales de l'application :
- soient trop générales pour être spécifique à une entité ou un processus
- soient uniques et qui ne doivent surtout pas être multipliés (ex : descripteur de fichier, maillage 3D, texture, VulkanContext, etc...)

En général, les 2.

Les composants peuvent avoir des champs pointant vers les ressources.

Les processus sont les fonctions évolutives des entités.

On peut imaginer un processus qui s'occupe d'appliquer les forces aux objets, un processus pour les collisions, etc... L'idée est d'obtenir un système fortement parallélisable et *cache-friendly*.

## Langage et APIs

Langage de programmation : C++23
Bibliothèques utilisées : 
- XCB
- Vulkan
- STL 23
- outils spécifiques au moteur (pour l'instant, voir la TODO list ^^)
