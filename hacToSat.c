#include <stdlib.h>
#include <stdio.h>
#include "all.h"

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









int setNbClauses(int h){
  int res, n, tmp, i,k,j;
  tmp=0;
  n=orderG();
  for( i=0; i<n; i++){
            for ( j=0; j<=h; j++){
                for( k=j+1; k<=h; k++){
		  tmp++;		  
		}
	    }
  }
 // printf("(%d)\n", tmp);
  res= n + tmp;
  res += n +2;
  res += n*h;
  return res;
}

int main(int argc, char *argv[]){
    if(argc !=2 || argv[1]<0){
        exit(EXIT_SUCCESS);
    }
    FILE *cnf_file = NULL;
    
    cnf_file = fopen("dimacs/fichier.cnf", "w");
   
    int n = orderG();
    int i,j,h, tt;
    tt=0;
    int tmp=0;
    int cpt =0;
    int nb_clauses=0; //
    
    int profondeur_max =atoi(argv[1]); 
    int nb_variables=orderG()*(profondeur_max+1);
    int tab[orderG()][profondeur_max];
    i=0;
    while(i<orderG()){  
      j=0;
      while(j<=profondeur_max){
            cpt++;
            tab[i][j]=cpt;
	    j++;
	    printf("%d\n", cpt);
        }
        i++;
    }
    nb_clauses = setNbClauses(profondeur_max);
    
    //le fichier CNF qui contiendra nos clauses
    
    if (cnf_file != NULL){ 
        fprintf(cnf_file, "c\nc commentaires\nc\n");

        fprintf(cnf_file, "p cnf %d %d\n", nb_variables, nb_clauses);
        nb_clauses =0;
        
        //1) Pour chaque sommet possède une hauteur propre
        i=0;
	while(i<orderG()){
	    j=0;
	    while(j<=profondeur_max){
		fprintf(cnf_file, "%d ", tab[i][j]);
		j++;
            }
            fprintf(cnf_file, "0\n");
	    i++;
	    nb_clauses++;
        }
        
        //
        i=0;
        while(i<n){
	    j=0;
	    while(j<=profondeur_max){
		h=j+1;
		while(h<=profondeur_max){
		    fprintf(cnf_file, "-%d -%d ",tab[i][j], tab[i][h]);
		    fprintf(cnf_file, "0\n");
		    h++;
		    nb_clauses++;
                }
                j++;
            }
            i++;
        }
                        
                
        //2) Il y a un unqiue sommet v tq d(v)=0
        i=0;
        while(i<n){
            fprintf(cnf_file, "%d ",tab[i][0]);
	    i++;
        }   
        fprintf(cnf_file, "0\n");
        nb_clauses++;
	i=0;
        while(i<n){
	    j=i+1;
            while(j<=n){
                fprintf(cnf_file, "-%d -%d ",tab[j][0], tab[i][0]);
		
		j++;	      
	    }
            fprintf(cnf_file, "0\n");
	    i++;
	    nb_clauses++;
        }
        //3) Il y a au moins un sommet d(v)=profondeur_max
        
        i=0;
        while(i<n){
            fprintf(cnf_file, "%d ", tab[i][profondeur_max]);
	    i++;
        }
        fprintf(cnf_file, "0\n");
        nb_clauses++;
        //4) Pour chaque sommet, si d(v) positif, il existe un sommet u tq uv appartient à E et d(u)=d(v) -1
        i=0;
        while(i<n){
	    h=1;
	    while(h<=profondeur_max){
                fprintf(cnf_file, "-%d ", tab[i][h]);
                j=0;
		while(j<n){
                    if((are_adjacent(i,j)) && (i!=j)){
                        fprintf(cnf_file,"%d ",tab[j][h-1]);
                    }
                    j++;
                }
                fprintf(cnf_file,"0\n");
		h++;
		nb_clauses++;
	    }
	    i++;
        }
            
         
        }
        
        // On ferme le fichier qui a été ouvert   
        fclose(cnf_file); 
	printf("clauses : %d , variable : %d , hauteur %d\n", nb_clauses, nb_variables, profondeur_max);
	return EXIT_SUCCESS;
    }       

    









