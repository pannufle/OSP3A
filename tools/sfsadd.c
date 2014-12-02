#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "structure.h"

#define TAILLE 10
#define BLOCK 256/8
#define LENGTH 32
#define BIT 8

void sfsadd(SimpleFileSystem *sfs , char filename[LENGTH]){
	int i, j, r, h = 0;
	
	// lecture du fichier et recuperation du contenu
	FILE *fp;
	fp = fopen(filename, "r");
	char contenu[TAILLE];
	bool fileInit = false;
	int nbindex = 0; // nombre de tabIndexes qu'on aura dans le file entry
	
	// recupere l'id du fichier pour l'ajouter 
	int idFiles = 0;
	while (sfs->fe[idFiles].size != 0)
		idFiles++;
	
	// lecture du bitmap
	for (i=0; i< BLOCK; i++){	
		for (j=0; j< BIT; j++){
			if ((sfs->bitmap[i]&(1<<j)) == 0){ // si bit block libre
				if (fp == NULL){
					printf("impossible d'ouvrir le fichier\n");
					return; 
				}else{
					int index = (1<<j);
					sfs->bitmap[i] |= index; // bit block utilisé

					// initalisation du file entry avec le nom et la taille	
					if (fileInit == false){
						// recupere le nom du fichier
						while (filename[h] != '\0'){
							sfs->fe[idFiles].name[h] = filename[h];
							h++;
						}
						// recuperer taille fichier
						sfs->fe[idFiles].size = FileSize(filename);
						fileInit = true;
						
					}
					// recupere l'index dans lequel est ranger le contenu
					sfs->fe[idFiles].tabIndexes[nbindex] = index;	

					fgets(contenu, TAILLE, fp); // recupere contenu fichier	

					// ajoute le contenu du fichier par block dans le fileContent correspondant
					for (r=0; r< TAILLE; r++){
						//int id = (r*BIT)+j;
						sfs->fileContent[index][r] = contenu[r];				
					}
				
					// tant qu'on arrive pas à la fin du fichier
					if(fgetc(fp) != EOF){
						nbindex++;
						// vide le tableau contenant le contenu
						for (r=0; r< TAILLE; r++)
							contenu[r] = '\0';
						// se positionne à l'endroit où le bout de contenu s'est arreter
						fseek(fp, TAILLE, 0);
					}else{
						fileInit = false;
						return;
					}
				}
			}
			
		}
	}
}

// Calcule la taille du fichier
int FileSize(char filename[LENGTH]){
	// ouvre le fichier pour le parcourir
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL){
		printf("impossible d'ouvrir le fichier\n");
		return; 
	}

	// calcul la taille du fichier
	int taille = 0;
	while(fgetc(fp) != EOF)
		taille++;

	return taille;
}

