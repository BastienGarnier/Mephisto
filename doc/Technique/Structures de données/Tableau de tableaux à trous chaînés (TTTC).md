En anglais : Linked Holes Array
Structure de données indicé pour le traitement séquentiel d'instances indépendantes ou presque indépendantes (exemple : rendu, systèmes de particules, animation, etc...):
- insertion en $\mathcal{O}(1)$ amortie (constante de l'ordre de $10^{-6}$)
- suppression en $\mathcal{O}(1)$
- accès indicé en $\mathcal{O}(1)$ 

Un TTTC contient :
- un tableau de *hugepages* qui sert à contenir les composants -> il faut un accès en O(1). *a priori* bon, les détails plus bas
- la taille en octets de la donnée d'une instance. La taille est soit nulle (*tag*), soit supérieure ou égale à $sizeof(indice)$ (intérêt pour stocker la chaîne de trous, en général 4)
- nombre d'instances par page $n_i$
- l'indice de la première instance invalide $i_{fi}$ (si aucune, égal à -1). 
Chaque instance de composant contient un premier octet qui indique si l'instance est valide ou non.
- si valide : on peut utiliser normalement la donnée
- si invalide : les 4 octets suivants (qu'on suppose exister par hypothèse) représentent l'indice de la prochaine instance invalide (liste chaînée de trous).
**L'idée :** dans le cas où on supprime un élément, on ne veut pas décaler les indices de tous les autres instances pour combler le vide, et en même temps, on ne veut pas perdre le trou indéfiniment. On voudrais que dans le cas où on ajoute puis supprime alternativement des éléments, la fuite de mémoire soit nulle.
## Variante peu efficace en pratique
**Problème :** dans le cas d'un parcours séquentiel, les trous ralentissent considérablement le parcours.
**Solution :** Au lieu d'avoir des trous simples, on a des blocs de trous qui pointent non seulement vers le bloc suivant, mais aussi vers l'instance valide juste suivante. Dans le cas où deux blocs sont adjacents, on les fusionne.
