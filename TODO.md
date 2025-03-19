- L'autorepeat du clavier (quand on maintient touche enfoncée par ex.) a une fréquence très faible par rapport au FPS du jeu.
Il faut donc conserver l'entrée jusqu'à la mise à jour effective (i.e : si une touche n'est pas released alors qu'elle est pressée, on ne la met pas par défaut relâchée alors que juste aucun évènement n'a été reçu)

- Construire un système ECS fonctionnel :
	- Gérer l'alignement des ComponentBuffers sur 4 ou 8 octets pour améliorer les performances en vitesse (à check si véritable amélioration)

- Utiliser un système de fenêtrage basé sur les ECS (pour l'instant, les fenêtres sont en mono pour les tests d'évènements). Une fenêtre devrait être simplement un ID avec :
	- composant XCB
	- composant Vulkan
	- composant InputEvent

# Structure d'une application
On construit une application selon le principe ECS (Embedded Composant System).
Il faut donc *a minima* un manager de ressources et un manager de scènes, qui systématiseront l'utilisation des composants autour des entités.

### Du parallélisme
Il est plus qu'intéressant de pouvoir paralléliser certains composants, notamment puisque cela permet d'améliorer les performances en laissant au SE le soin de partager les actions sur les différents cœurs du CPU.
# Signal System

Ajouter un système de communication par signal à l'intérieur d'une application. Cela simplifiera grandement la programmation des entités et le design

Deux types de signaux :
- Signaux synchrones
- Signaux asynchrones

Asynchrones : plus simple. Les signaux sont traités dès que possible et on n'a pas de préoccupation d'ordonnancement.

Synchrones : Paquets sous forme de file.

Par ailleurs, les entités doivent pouvoir faire efficacement la distinction entre un signal qui leur est destiné ou un signal qui ne l'est pas. Si on  a un paquet de $n$ signaux à traiter, il faut pour une entité fixée pouvoir sélectionner tous les $k$ signaux dont il existe une fonction pour les traiter. Idée : table de hachage des $n$ signaux selon leur identifiant