#include<stdio.h>
typedef struct
{
	char instance[50];
	int M;/*nbre total d'objets*/
	int N;/*nbre de classes*/
	int b;/*le temps disponible (la capacité du sac)*/
	int *ni;/* nombre d'objets pour chaque classe i*/
	int *f;/*coût d'installation de chaque classe*/
	int *d;/*temps d'installation de chaque classe*/
	int **a;/* a[i][j] represente le temps de production de l'objet j de la classe i */
	int **c;/* c[i][j] represente le profit de l'objet j de la classe i*/

	int **at;/* les indices des objets de chaque classe triées dans l'ordre croissant des volumes */
	int **ct;/* les indice des objets de chaque classe triées dans l'ordre décroissant des profits*/


}kps_prob;

typedef struct
{
	int **s;//tableau binaire de taille N*ni (détermine si un objet d'une classe est mis dans le sac ou non.
	int *cl;//tableau binaire de taille N (détrmine si une classe est ouverte ou non)
	int vol;
	int pro;
}kps_sol;

kps_sol * buil_empty_sol(kps_prob *pb);
void buil_init_sol(kps_prob *pb, kps_sol *sol);
void local_search(kps_prob *pb, kps_sol *sol);
void local_search_TRed(kps_prob *pb, kps_sol *sol);
void local_search_SVAmel(kps_prob *pb, kps_sol *sol);
void display_sol(kps_prob *pb, kps_sol *sol);
void copy_sol(kps_prob *pb, kps_sol *sol1, kps_sol *sol2);
