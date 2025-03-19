Les IAs du jeu prennent en entrée la valeur du masque audio en leur position et l'entièreté du masque lumineux de la scène qui leur est accessible, ainsi que les éléments de la scène.

La sortie est une fonction appliquée sur le [[Personnage]] (ex : se déplacer vers le haut, vers le bas)

Dans chacun des deux cas, il existe deux catégorie d'évènements perçus :
- ceux dont le personnage a déjà connaissance
- ceux dont le personnage n'a pas connaissance

Il peut y avoir un gain ou une perte de connaissance d'un évènement. La perte de connaissance d'un évènement implique qu'il existe une durée pour laquelle l'évènement est cru par le personnage mais n'est pas nécessairement existant réellement.

Du point de vue du personnage, cette croyance est pour autant factuelle. Elle n'est pas considéré comme "non réelle" en programmation. C'est lorsque la croyance est brisée, i.e. que le personnage se rend effectivement compte de l'inexistence de l'évènement, celui-ci disparaît de la catégorie de "a connaissance". Comme l'évènement peut ne plus exister, cela ne signifie pas qu'il entre dans la catégorie "n'a pas connaissance".

Des actions d'[[Intelligence Artificielle]] doivent donc être programmés pour les cas :
- a déjà connaissance (interaction avec l'évènement)
- perd connaissance (recherche de l'évènement disparu, etc...)