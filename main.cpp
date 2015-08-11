#include <iostream>//pour le cout
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "kps_heur.h"
using namespace std;//pour le cout
	
void main()
{	
	time_t start, end;
	int i,j,k,r,tmp;//Compteurs
	double total_time, objval;//Pour calculer le tmeps d'execution
	char result[15]={"resultat.xls"};//Fichier résultat
	errno_t err;
	FILE *fr,*fw,*ff;//Flux d'ouverture des fichiers
	kps_prob *pb;
	kps_sol *sol;
	

	///////
	if((err=fopen_s(&ff,"kps_instances_liste.txt","r"))!=0)
		printf("Erreur d'ouverture du fichier kps_instances_liste.txt...\n");
	///////
	if((err=fopen_s(&fw,"result.xls","w"))!=0)
		printf("Erreur d'ouverture du fichier result.xls...\n");
	fprintf(fw,"Instances\tOBJ\tCPU\n"); 
	for(r=0;r<50;r++)
	{
		//Allocation de la mémoire pour le problème
		pb=(kps_prob*)malloc(sizeof(kps_prob));	
		//Lecture du nom de l'instance
		fscanf(ff,"%s",pb->instance);
		//printf("%s\n",pb->instance);
		//+---------------------------------------------------------------+
		//+Ouverture de l'instance
		//+---------------------------------------------------------------+
		if((err=fopen_s(&fr,pb->instance,"r"))!=0)
			printf("Erreur de lecture de l'instance %s...\n",pb->instance);
		else
			printf("\n\n\nLecture de l'instance %s...\n",pb->instance);
		

		//+---------------------------------------------------------------+
		//+Lecture de l'instance
		//+---------------------------------------------------------------+
		/*lecture de M, N et b*/
		fscanf_s(fr,"%d",&(pb->M));
		fscanf_s(fr,"%d",&(pb->N));
		fscanf_s(fr,"%d",&(pb->b));
		/*allocation dynamique du tableau contenant le nombre d'objet, coût et temps d'installation de chaque classe
				ni[i] présente le nombre d'objet dans la classe i
				f[i] le cout d'intallation de la classe i
				d[i] le temps d'installation de la classe i */
		pb->ni=(int*) malloc (sizeof(int)*pb->N);
		pb->f=(int*) malloc (sizeof(int)*pb->N);
		pb->d=(int*) malloc (sizeof(int)*pb->N);
		/*remplissage de ni, f et d */
		for(i=0; i<pb->N;i++)
			fscanf_s(fr,"%d",&(pb->ni[i]));
		for(i=0; i<pb->N;i++)
			fscanf_s(fr,"%d",&(pb->f[i]));
		for(i=0; i<pb->N;i++)
			fscanf_s(fr,"%d",&(pb->d[i]));
		/*allocation dynamique de c et a
				c est la  liste des profits de chaque objet et a contient le temps nécessaire de production de chaque objet 
				c[i][j] represente le profit de l'objet j de la classe i
				a[i][j] represente le temps de production de l'objet j de la classe i */
		pb->c=(int**) malloc (sizeof(int*)*pb->N);
		for(i=0; i<pb->N; i++)
  			pb->c[i]=(int*) malloc (sizeof(int)*pb->ni[i]);
		pb->a=(int**) malloc (sizeof(int*)*pb->N);
		for(i=0; i<pb->N; i++)
  			pb->a[i]=(int*) malloc (sizeof(int)*pb->ni[i]);
		/*remplissage de c et a*/
		for(i=0;i<pb->N;i++)
			for(j=0;j<pb->ni[i];j++)
				fscanf(fr, "%d %d",&(pb->c[i][j]),&(pb->a[i][j]));
		//Trier les objets de chaque classe selon leurs profits
		pb->ct=(int**) malloc (sizeof(int*)*pb->N);
		for(i=0; i<pb->N; i++)
		{
  			pb->ct[i]=(int*) malloc (sizeof(int)*pb->ni[i]);
			for(j=0; j<pb->ni[i]; j++)
				pb->ct[i][j]=j;
		}
		for(i=0; i<pb->N; i++)
		{
			for(j=0; j<pb->ni[i]-1; j++)
				for(k=j+1; k<pb->ni[i]; k++)
				{
					if(pb->c[i][pb->ct[i][j]]<pb->c[i][pb->ct[i][k]])
					{
						tmp=pb->ct[i][j];
						pb->ct[i][j]=pb->ct[i][k];
						pb->c[i][k]=tmp;
					}
				}
		}
  		
		//Trier les objets de chaque classe selon leurs volumes
		pb->at=(int**) malloc (sizeof(int*)*pb->N);
		for(i=0; i<pb->N; i++)
		{
  			pb->at[i]=(int*) malloc (sizeof(int)*pb->ni[i]);
			for(j=0; j<pb->ni[i]; j++)
				pb->at[i][j]=j;
		}

		for(i=0; i<pb->N; i++)
		{
			for(j=0; j<pb->ni[i]-1; j++)
				for(k=j+1; k<pb->ni[i]; k++)
				{
					if(pb->a[i][pb->at[i][j]]>pb->a[i][pb->at[i][k]])
					{
						tmp=pb->at[i][j];
						pb->at[i][j]=pb->at[i][k];
						pb->at[i][k]=tmp;
					}
				}
		}
		//+---------------------------------------------------------------+
		//+Fermeture de l'instance
		//+---------------------------------------------------------------+
		fclose(fr);
		
		//+---------------------------------------------------------------+
		//+Construction d'une solution vide
		//+---------------------------------------------------------------+
		sol= buil_empty_sol(pb);
		//+---------------------------------------------------------------+
		//+Construction d'une solution initiale
		//+---------------------------------------------------------------+
		start=clock();
		buil_init_sol(pb, sol);
		local_search(pb, sol);
		//local_search_TRed(pb, sol);
		//local_search_SVAmel(pb, sol);
		end=clock();
		
		//display_sol(pb, sol);


		//+---------------------------------------------------------------+
		//+Ecriture de la solution 
		//+---------------------------------------------------------------+
		printf("%s\t%d\t%.2lf\n",pb->instance,sol->pro,(end-start)/1000);
		fprintf(fw,"%s\t%d\t%.2lf\n",pb->instance,sol->pro,(end-start)/1000);
		//liberation de la mémoire  des structures créées pour l'instance courante
		for(i=0;i<pb->N;i++)
		{
			free(pb->a[i]);
			free(pb->c[i]);
		}
		free(pb->a);
		free(pb->c);
		free(pb->d);
		free(pb->ni);
		free(pb->f);
		free(pb);
		free(sol);
		//system("pause");

	}//Fin boucle for(k=....)
	//+---------------------------------------------------------------+
	//+Fermeture du fichier resultat et du fichier liste_instances
	//+---------------------------------------------------------------+
	fclose(fw);
	fclose(ff);
	system("pause");
}
