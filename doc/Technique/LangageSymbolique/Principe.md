# Motivation

Les langages C/C++ n'ont pas de traitement symbolique pur hors contexte du programme. Donc si on veut traiter un symbole, il faut que celui-ci ait été défini dans le programme, et plus précisément dans un module.

Donc si un module $K$ doit utiliser du traitement symbolique, il doit importer tous les modules $m_1, \dots, m_n$ qui définissent les symboles. Cependant, le module $K$ peut être une dépendance de cœur pour le fonctionnement des modules $m_1, \dots, m_n$, ce qui est problématique.

On peut des heures durant disserter que ce problème vient d'une erreur de *design* mais le fait est que c'est parfois nécessaire pour de l'optimisation et de la fluidité syntaxique.

**Exemple :** composants traités par un module $K$ dans *Mephisto* sont dans un tableau dont on veut connaître le type au traitement pour :
- 1 : accéder au bon indice du tableau (énumération de types)
- 2 : appeler la surcharge de fonction qui correspond à ce type (donc associer le type effectif au symbole). Ce travail est effectué directement par le programmeur.
Mais le module d'un composant $X$ ne doit pas à avoir à savoir comment le kernel de Mephisto a été programmé. Il repose sur l'interface de $K$ *avant* l'ajout de $X$. Il faut donc que la surcharge soit définie *dynamiquement* lors de l'ajout du composant. C'est-à-dire qu'un attribut *Composant* est ajouté à la classe $X$ et que le compilateur peut  ajouter $X$ à l'énumération de type sans avoir à l'importer pour éviter les cycles.

Donc : on doit ajouter un préprocesseur de traitement symbolique pur qui peut construire une BdD de symboles avant la compilation, les traiter comme on veut et ensuite modifier le code (générer une copie) pour y appliquer les opérations sur les symboles (par exemple : supprimer lesdits symboles et remplacer par un nombre). Ensuite compiler la copie et la supprimer.

# Idée de syntaxe
Le caractère '@' n'est pas utilisé en C/C++. On peut donc l'utiliser pour insérer du code fonctionnel.

Si on veut calculer une fonction en compile-time et avoir un template générique sur une fonction :
```C
#include <stdio.h>

@
	Fun fibo(Int n) -> Int
		: 0 -> 1
		: 1 -> 1
		: . -> fibo (n-1) + fibo (n-2);;

	Fun ackermann(Int m, Int n) -> Int:
		: 0,. -> n+1
		: ., 0) -> ackermann(m-1, 1)
		: . -> ackermann(m-1, ackermann(m, n-1));;
@

@
	Template add(Symbol T) -> Symbol:
		: . ->
		`
		@T add(@T a, @T b) {
			return a + b;
		}
		`
		: char* ->
		`
		char* add(char * a, char *b) {
			unsigned int n = strlen(a);
			unsigned int m = strlen(b);
			char* c = malloc((n + m + 1)*sizeof(char));
			unsigned int i = 0;
			for (; i < n; i++) {
				c[i] = a[i];
			}
			for (; i < n + m; i++) {
				c[i] = b[i - n];
			}
			c[n + m] = 0;
			return c;
		}
		`;;
@

@
	Enum components;;
@

@TemplatePoint add // la fonction est généré ici du code 

typedef struct Machin {
	@Enum components add Machin
} Machin;

int main() {
	printf("Fibo(10) = %d\n", @fibo(10)});
	printf("Ackermann(2, 5) = %d\n", @ackermann(2, 5));

	int x = @add(int)(5, 6); // on ajoute au template point le code pour int si pas encore existant et on remplace ici par l'appel à ce code, c'est-à-dire la signature du template
	return EXIT_SUCCESS;
}
```

**Problème *a posteriori* :** le langage pensé comme ça ne comprend pas le C. Il faut que le langage comprenne le C et interagisse avec lui, tout en restant potentiellement purement symbolique. Donc qu'on puisse évaluer ou non des symboles.

```C
#include <stdio.h>
#include <stdlib.h>

@
enum components {
	int,
	float
}
@

typedef struct Machin {
	
};
int main() {
	int x = @components.int;
	printf("x = %d\n", x);
}
```