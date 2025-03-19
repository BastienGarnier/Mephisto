## P'tite réflexion sur les processus et les ressources
La console est offerte au programmeur dès le lancement d'un programme. C'est donc une ressource. Or, une fenêtre est un objet de nature similaire à celle de la console, parfaitement indépendant du contenu d'une scène de jeu, et qui doit être accessible depuis partout dans le code. Donc la fenêtre devrait être une ressource. Mais c'est une ressource actualisé à chaque tour (puisqu'il faut lire les entrées clavier/souris et afficher le contenu de scènes). Donc il faut un processus qui la manipule. Mais pas un processus du jeu... En fait, j'ai l'impression qu'il faut différencier deux systèmes :
- le système abstrait, c'est-à-dire le jeu
- le système concret qui permet au jeu de tourner, c'est-à-dire tout ce qui permet de soutenir le jeu et de le faire interagir avec l'environnement (typiquement : affichage, lire les entrées, lire/écrire dans des fichiers, etc...)
L'idée m'a pas l'air correct, parce-qu'elle se veut généralisable alors qu'il n'y aura toujours que 2 systèmes au max (typiquement : gestion des ressources et gestion des composants). Et ça ne fait que compliquer les interactions de l'un avec l'autre, surtout puisque les accès sont à sens unique (ressources accédées par les composants). Donc on ne peut pas simplement écrire deux systèmes l'un à côté de l'autre.
On pourrait simplement écrire un processus ne prenant aucune requête de composants qui manipule les ressources (?)

Mais il faut que ce processus tourne avant tous les autres... donc pas propre, sauf à introduire des couches de priorités sur les processus.

Un processus de priorité N est exécuté avant un processus de priorité N+1. On choisi un ordre croissant dans l'idée que des processus ajoutés plus tard sont forcément moins prioritaires, et qu'on veut des entiers naturels pour simplifier.

Et comme on ne parallélise _a priori_ que chaque processus individuellement, il n'y a pas de problème à un tel ordre.

La parallélisation sur les processus individuellement permet :
- d'éviter les problèmes de mémoire partagé
- de ne pas empiéter sur d'autres logiques
- des performances optimales puisque les mêmes données sont en cache