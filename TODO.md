# Système
- L'autorepeat du clavier (quand on maintient touche enfoncée par ex.) a une fréquence très faible par rapport au FPS du jeu.
Il faut donc conserver l'entrée jusqu'à la mise à jour effective (i.e : si une touche n'est pas released alors qu'elle est pressée, on ne la met pas par défaut relâchée alors que juste aucun évènement n'a été reçu)

# Structures de données
- Tables de hachage optimisées
- Listes chaînées optimisés
- V-Partitions (voir article nanite)
- Hiérarchie de volumes englobants
# Fichiers
Formats de fichiers à pouvoir lire :
- JSON => prioritaire pour les fichiers de config., de paramètres, etc...
- OBJ => pour pouvoir faire des tests
- TTF ou OTF => polices d'écriture
# Vulkan
**Fonctionnalités à améliorer :**
- GraphicsPipeline à moduler selon les différents stages, pour être plus flexible et manipuler facilement les composants de rendu.
- Pour les buffers et la mémoire :
	- Dans la copie de buffers, attendre en utilisant des *fences* permet de paralléliser les remplissages
	- Créer un allocateur mémoire maison pour permettre de n'utiliser qu'une seule fois vkAllocateMemory et utiliser les offsets pour simuler plusieurs allocations
	- Stocker plusieurs buffers en un seul en utilisant les offsets (permet d'être plus cache-friendly)
	- Si certains blocs mémoires ne sont pas utilisés pour les opérations de rendu et sont *refreshed*, les réutiliser permet aussi d'être cache-friendly et donc d'aller plus vite (connu sous le nom d'*aliasing*)
	- Ajouter des index buffers
- Graphics Pipeline :
	- Créer des fichiers JSON de construction avec les paramètres, le nom de fichier des shaders, les stages, etc...
- Context :
	- Fichier de configuration de contexte 
- Pour les transferts mémoire :
	- ajouter une transfer queue au lieu d'utiliser la graphics queue pour tout faire

**Tout à faire :**
- Uniform Buffers
- Texture mapping
- Depth buffering
- Mipmaps
- Multisampling
- Stencil buffering
- Loading models
- Instanciation géométrique
# Structure d'une application
- Parallélisation des processus de traitement des composants et des ressources
=> protection mémoire correcte et opti.

- Graphes de scène
# Signal System (?)

Ajouter un système de communication par signal à l'intérieur d'une application. Cela simplifiera grandement la programmation des entités et le design

Deux types de signaux :
- Signaux synchrones
- Signaux asynchrones

Asynchrones : plus simple. Les signaux sont traités dès que possible et on n'a pas de préoccupation d'ordonnancement.

Synchrones : Paquets sous forme de file.

Par ailleurs, les entités doivent pouvoir faire efficacement la distinction entre un signal qui leur est destiné ou un signal qui ne l'est pas. Si on  a un paquet de $n$ signaux à traiter, il faut pour une entité fixée pouvoir sélectionner tous les $k$ signaux dont il existe une fonction pour les traiter. Idée : table de hachage des $n$ signaux selon leur identifiant