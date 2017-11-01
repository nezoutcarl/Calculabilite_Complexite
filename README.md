			  DM Calculabilite_Complexite
	   GUIDE D'UTILISATION

####Compilations##
-> Programme
gcc -g hacToSat.c -o [nom_executable]

-> Solver SAT Glucose 4.1
se rendre dans le dossier simp/ et faire make.

####Exécutions ####

##Mise en place Formule SAT suivant les contraintes

-> On exécute le programme compilé avec en argument la hauteur max de notre graphe
./[nom_executable] [hauteur_max]

-> On obtient un fichier .cnf (voir dossier files) identifiable et applicable au Solver SAT 

##Solveur SAT

Il suffit à présent de fournir notre fichier cnf à Glucose, après compilation l'exécutable glucose est disponible dans le dossier simp/  :
./glucose [chemin]/files/fichier.cnf

##Changer de graphe
 remplacer le contenu des fonction orderG(), sizeG() et are_adjacent() par celui du fichier souhaité 