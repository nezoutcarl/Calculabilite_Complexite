#include <stdlib.h>
#include <stdio.h>
#include "all.h"
#include <string.h>

/* ***** Partie concernant le graphe utilisé  ****** */
int orderG(){
    return 95;}

    int sizeG(){
        return  755;
    }

int adj(int u, int v, int n){
    int x,y;
    if (u==v)
        return 0;
    if(u>v){
        x=u;
        y=v;
    }
    else{
        x=v;
        y=u;
    }
    if(x==1)
        return 1;
    if(x<n/2)
        return adj(x,y,n/2);
    if(x==n-1){
        if(y>=n/2) 
            return 1;
        else return 0;
    }
    if(x>=n/2 && y<n/2)
        return adj(x-n/2,y,n/2);
    return 0;
}




int are_adjacent(int u, int v){
    if(0<=u && 0<=v && u<orderG() && v<orderG())
        return adj(u,v,orderG());
    return 0;
}




/* ***************************************************** */


/**
 * @brief obtenir le nombre de clauses que l'on va écrire dans le fichier
 * @param h entier représentant la hauteur max * 
 * 
 * */
int setNbClauses(int h){
  int res, n, tmp, i,k,j;
  tmp=0; //variable pour compter le nombre de clauses de la sous clauses de 1)
  n=orderG();// le nombre de sommets
 
  i=0;
  while(i<n){
    j=0;
    while(j<=h){
      k=j+1;
      while(k<=h){
	tmp++;		  
	k++;	
      }
      j++;      
    }
    i++;
  }
  res= n + tmp; //nombre de clauses contrainte 1)
  res += n +1;//contrainte 2 
  res++;//contrainte 3
  res += n*h; //contranite 4
  return res;
}

char* initCommentaires(){
  
}


/**
 * @brief initialise et écrit dans un fichier la formule cnf correspondante aux contraintes de notre énoncé
 * @param argc nombre de chaînes de caractères sur lequel pointe argv
 * @param argv tableau de pointeurs sur chaines lors de l'exécution du programme
 * */
int main(int argc, char *argv[]){
    //test sur l'exécution correcte du programme : il faut un argument strictement positif
    if(argc !=2 || argv[1]<0){
        exit(ERREUR_ARGUMENTS);//on quite le programme
    }
    //initialisation du descripteur et ouverture du fichier cible
    FILE *cnf_file = NULL;    
    cnf_file = fopen("files/fichier.cnf", "w"); //le fichier CNF qui contiendra nos clauses
    
    /* Déclaration des variables */
    int n; //nombre de sommets
    int i,j,h; // variables de boucles
    int res_com; //variable de retour de saisie 
    int nb_clauses; //nombre de clauses
    int cpt; //compteur pour initialiser notre tableau d'indexs des variables
    int profondeur_max; //variable en argument correspondant à la hauteur maximum établie
    int nb_variables; //nombre de littéraux de notre formule
    char commentaire[50]="";
    char saisie[5];
    
    /* Initialisation des variables */
    n= orderG();
    cpt =0;
    nb_clauses=0;     
    profondeur_max =atoi(argv[1]); 
    nb_variables=orderG()*(profondeur_max+1);
    //déclaration tableau 2D d'indexes de taille nb_variables selon un sommet et sa hauteur potentielle  
    int tab[orderG()][profondeur_max];
    //Initialisation du tableau d'index  
    i=0;
    while(i<orderG()){  //parcours des sommets
      j=0;
      while(j<=profondeur_max){ //parcours des hauteurs
            /* On incrémente le compteur et l'affecte à l'élément courant du tableau */
	    cpt++;
            tab[i][j]=cpt;
	    j++;
        }
        i++;
    }
    //Calcul du nombre de clauses nécessaire afin de l'écrire dans le fichier
    nb_clauses = setNbClauses(profondeur_max);
    
    
    //Si l'ouverture c'est bien passé, lancement de la procédure pour la formule cnf et écriture dans le fichier cible
    if (cnf_file != NULL){ 
        printf("Voulez vous associer un commentaire ? (o/[autres])\n");
	res_com= scanf("%s", saisie);
	if(res_com != 1){
	  printf("hallo\n");
	  fclose(cnf_file);
	  exit(ERREUR_SAISIE);
	}
	else {
	  if(strcmp(saisie,"o")==0){
	    printf("Saisir le commentaire svp (remplacer l'espace par un _ :\n");
	    scanf("%s",commentaire);
	    fprintf(cnf_file, "c\nc %s \nc\n", commentaire);
	  }
	  else{
	    fprintf(cnf_file, "c\nc commentaires\nc\n");
	  }
	}
	//Ecriture dans le fichier de la ligne décrivant la formule cnf
        fprintf(cnf_file, "p cnf %d %d\n", nb_variables, nb_clauses);
        
        /* Ecriture dans le fichier des clauses selon chaque contraintes */
	
        //1) chaque sommet possède une hauteur propre
        i=0;
	while(i<n){//parcours des sommets
	    j=0;
	    while(j<=profondeur_max){//parcours des hauteurs possibles
		fprintf(cnf_file, "%d ", tab[i][j]);
		j++;
            }
            fprintf(cnf_file, "0\n"); //fin de clause
	    i++;
        }
        
        //
        i=0;
        while(i<n){//parcours des sommets
	    j=0;
	    while(j<=profondeur_max){//parcours des hauteurs possibles
		h=j+1;
		while(h<=profondeur_max){
		    fprintf(cnf_file, "-%d -%d ",tab[i][j], tab[i][h]);
		    fprintf(cnf_file, "0\n");//fin de clause
		    h++;		    
                }
                j++;
            }
            i++;
        }
                        
                
        //2) Il y a un unqiue sommet v tq d(v)=0
        i=0;
        while(i<n){//parcours des sommets V(G)
            fprintf(cnf_file, "%d ",tab[i][0]);
	    i++;
        }   
        fprintf(cnf_file, "0\n");//fin de clause
        
	i=0;
        while(i<n){//parcours des sommets
	    j=i+1;
            while(j<=n){
                fprintf(cnf_file, "-%d -%d ",tab[j][0], tab[i][0]);
		
		j++;	      
	    }
            fprintf(cnf_file, "0\n");//fin de clause
	    i++;
	    
        }
        //3) Il y a au moins un sommet d(v)=profondeur_max
        
        i=0;
        while(i<n){//parcours des sommets
            fprintf(cnf_file, "%d ", tab[i][profondeur_max]);
	    i++;
        }
        fprintf(cnf_file, "0\n");//fin de clause
       
        //4) Pour chaque sommet, si d(v) positif, il existe un sommet u tq uv appartient à E et d(u)=d(v) -1
        i=0;
        while(i<n){//parcours des sommets
	    h=1;
	    while(h<=profondeur_max){
                fprintf(cnf_file, "-%d ", tab[i][h]);
                j=0;
		while(j<n){
                    if((are_adjacent(i,j)==1) && (i!=j)){
                        fprintf(cnf_file,"%d ",tab[j][h-1]);
                    }
                    j++;
                }
                fprintf(cnf_file,"0\n");//fin de clause
		h++;
	    }
	    i++;
        }
            
         
        }
        
        // Fermeture du fichier qui a été ouvert   
        fclose(cnf_file); 
	return EXIT_SUCCESS;
    }       

    









