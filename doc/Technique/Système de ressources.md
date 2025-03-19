Données qui devront être partagés par les entités mais qui ne sont pas des composants d'entités. Pointeurs dans les composants vers les ressources :
- Meshes
- Graphics Pipeline
- Vulkan Support (?)

**Problème :** flexibilité des structures dans le code

**Exemple :** on a plusieurs meshes, graphics pipeline, etc..., mais un seul Vulkan core :
```C++
struct VulkanContext {
	VulkanContext {
	}
	~VulkanContext {
	}
	VkInstance instance;
	VkPhysicalDevice physical_device;
	VkDevice device;
	VkQueue graphics_queue;
	VkQueue present_queue;
	QueueFamilyIndices qfi;
} _vulkan;
```
**Question :** organisation du manager de ressources pour pouvoir ajouter n'importe quel type de ressource.
Les ressources doivent pouvoir être nommés, i.e. tables de hachage :
```C++
ressourcesmanager.get<Mesh>("cube"); // -> renvoie le Mesh nommé "cube"
```
On ajoute donc une énumération de types.
On peut donc sauvegarder les données Vulkan dans les ressources donc.
Le destructeur de structure est Ok.