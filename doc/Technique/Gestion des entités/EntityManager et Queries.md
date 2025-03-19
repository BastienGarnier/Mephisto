## Définitions

**Composant :** structure de données stockant des propriétés mutables. Chaque type de composant possède un identifiant unique. Cet identifiant doit être généré pour chaque structure mais pas pour chaque instance de structure. Toutes les instances d'un type de composant sont stockés dans un unique tableau.

**Entité :** identifiant $e$ auquel sont associés des composants par un archétype.

**Archétype :** on appelle archétype un ensemble de types de composants qui caractérise une entité $a = (c_1, c_2, \dots, c_n)$ 

Si plusieurs entités ont les mêmes composants (de valeurs différentes), elles ont le même archétype. Soient deux archétypes $a = \{a_1, \dots, a_n\}$ et $b = \{b_1, \dots, b_m\}$. $b$ est un sous-archétype de $a$ ssi $\forall c\in{b}, c\in{a}$, et on note alors $b\leq a$.
L'archétype d'une entité $e$ est noté $A(e)$.

On définit le monde comme $W = (Q, P)$ le couple des requêtes et des processus qui seront utilisés.

## Queries

La requête d'un archétype $a$ renvoie toutes les entités $e$ telles que $a\leq A(e)$. Toutes les requêtes sont stockés dans le *entitymanager*. 
La requête doit pouvoir filtrer les éléments selon un paramètre donné en avance.
Le filtre décrit des groupe s d'entités.
L'ensemble des entités d'une requête est donc $Q_{a, F}$, où $a$ est l'archétype et $F$ est le filtre.
Un filtre est un $n$-uplet de groupes $g_1, \dots, g_n$. Pour tout $e$, $F(e) \in{[1;n]}$ est le groupe d'appartenance de $e$ dans $Q_{a, F}$ . L'ensemble des entités de $Q_{a, F}$ est  $\displaystyle\bigsqcup_{i = 1}^{n} g_i$
Il faut que $\bigcap g_i = \emptyset$ , ce qui est assuré car l'image de $F(e)$ est unique. 
Difficulté : trouver la fonction $F$ et l'automatiser.

Quand un processus cherche à obtenir toutes les entités utilisant un sous-archétype $a$ donné, il parcours  les $g_i$ de $Q_{a, F}$ et traite les données de chaque entité. 
### Filtre $F$ 

Doit pouvoir assigner à un groupe selon une valeur spécifique d'un composant. Par exemple, si deux entités possédant un composant de rendu on le même identifiant de texture, elles sont placés dans le même groupe

=> table de hachage avec comme clé la valeur de composant spécifié.
Le filtre est donc la fonction de hachage dans ce cas là.

On veut aussi pouvoir filtrer selon les composants dont la valeur a été modifié, pour ne pas traiter à nouveau ceux qui l'on déjà été.

Si une valeur est modifié sur une valeur affectée par un filtre, elle doit changer de groupe dans la query utilisant ce filtre.

## Initialisation d'un monde

Au chargement d'un monde, on stocke en mémoire toutes les requêtes différentes, c'est-à-dire toutes les combinaisons d'archétypes qui seront utilisés par les processus au cours de l'exécution du programme.

## Dynamique de la structure d'un monde

**Ajout dynamique d'entités :**
Soit $e$ une entité à ajouter à un monde $W = (Q, P)$.
On parcourt chaque $Q_a$ de $Q$. Si $a\leq A(e)$, on ajoute $e$ à $Q_a$ :
- déterminer le groupe unique d'appartenance de $e$
- ajouter une instance de composant à chaque $c\in A(e)$ et les lier dans $e$ 
- ajouter $e$ à son groupe dans $Q_a$

**Suppression dynamique d'entités :**
Soit $e$ une entité à ajouter à un monde $W = (Q, P)$ 
On parcourt chaque $Q_a$ de $Q$. Si $a\leq A(e)$, on supprime $e$ de $Q_a$.
*Problème :* $Q_a$ est un tableau
