On veut pouvoir écrire :
```C++
for (EntityId e : entitymanager.query<Window, InputEvent>()) {

}
```
Chaque type de composant est numéroté.
On veut obtenir un identifiant unique par archétype. On utilise des champs de bits pour chaque ensemble de types.
Chaque composant a un énumérant associé $Enum(c)$. Soit $a = (c_1, \dots, c_n)$ un archétype.
Le champ de bits unique associé est :
$$C(a) = 1\ll Enum(c_1) | \dots | 1\ll Enum(c_n)$$
Ce champ est calculable à la compilation et sert de clé pour la unordered_map de requêtes.