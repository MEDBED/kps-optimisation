#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "kps_heur.h"

kps_sol * buil_empty_sol(kps_prob *pb)
{
	int i,j;
	kps_sol *sol;
	sol = (kps_sol*)malloc(sizeof(kps_sol));
	sol->cl=(int*)malloc(pb->N*sizeof(int));
	sol->s=(int**)malloc(pb->N*sizeof(int*));
	for(i=0;i<pb->N;i++)
		sol->s[i]=(int*)malloc(pb->ni[i]*sizeof(int));

	for(i=0;i<pb->N;i++)
	{
		sol->cl[i]=0;
		for(j=0;j<pb->ni[i];j++)
			sol->s[i][j]=0;
	}
	sol->pro=sol->vol=0;
	return sol;
}
void buil_init_sol(kps_prob *pb, kps_sol *sol)
{
	int cs,i;
	do
	{
START:
		do
			cs=rand()%pb->N;
		while(sol->cl[cs]==1 || sol->cl[cs]==-1);

		if(sol->vol+pb->d[cs]<=pb->b)
		{
			sol->cl[cs]=1;
			sol->pro+=pb->f[cs];
			sol->vol+=pb->d[cs];
		}
		else
		{
			sol->cl[cs]=-1;	

			for(i=0;i<pb->N;i++)
				if(sol->cl[i]==-1)
					sol->cl[i]=0;
			if(i==pb->N)
				goto END;
			else
				goto START;
		}

		//printf("%d\n",cs);
		for(i=0;i<pb->ni[cs];i++)
		{
			if(sol->vol + pb->a[cs][pb->at[cs][i]] <= pb->b)
			{
				sol->vol+=pb->a[cs][pb->at[cs][i]];
				sol->pro+=pb->c[cs][pb->at[cs][i]];
				sol->s[cs][pb->at[cs][i]]=1;
			}
		}
		for(i=0;i<pb->N;i++)
			if(sol->cl[i]==0)
				break;
END:;
	}
	while(i!=pb->N);

	for(i=0;i<pb->N;i++)
		if(sol->cl[i]==-1)
			sol->cl[i]=0;

}

void local_search_TRed(kps_prob *pb, kps_sol *sol)
{
}

void local_search_SVAmel(kps_prob *pb, kps_sol *sol)
{
}
void local_search(kps_prob *pb, kps_sol *sol)
{
	int i, j, k, r, iter; 
	kps_sol *bs, *ns;
	bs= buil_empty_sol(pb);
	ns= buil_empty_sol(pb);
	copy_sol(pb,sol,bs);
	iter=0;
	printf("Iter %d: Obj=%d\tVol=%d\tb=%d\n",iter,sol->pro,sol->vol,pb->b);
	do
	{
		for(i=0;i<pb->N;i++)
		{
			if(sol->cl[i]==1)
			{
				for(j=0;j<pb->ni[i];j++)
				{
					if(sol->s[i][j]==1)
					{
						for(k=0;k<pb->N;k++)
						{
							if(sol->cl[k]==1)
							{
								for(r=0;r<pb->ni[k];r++)
								{
									if(sol->s[k][r]==0)
									{
										copy_sol(pb,sol,ns);
										ns->s[i][j]=0;
										ns->s[k][r]=1;
										ns->pro=ns->pro-pb->c[i][j]+pb->c[k][r];
										ns->vol=ns->vol-pb->a[i][j]+pb->a[k][r];
										if(ns->vol<=pb->b && ns->pro>bs->pro)
										{
											copy_sol(pb,ns,bs);
											goto FI;
										}
									}
								}
							}
						}
						FI:;
					}
				}
			}		
		}
		if(bs->pro > sol->pro)
		{
			copy_sol(pb,bs,sol);
			printf("Iter %d: Obj=%d\tVol=%d\tb=%d\n",++iter,sol->pro,sol->vol,pb->b);
		}
		else
			break;
	}while(1);
	free(ns);
	free(bs);

}
void display_sol(kps_prob *pb, kps_sol *sol)
{
	int i,j;
	printf("Pro=%d\nVol=%d\nb=%d\n",sol->pro,sol->vol,pb->b);
	for(i=0;i<pb->N;i++)
		printf("%d\t",sol->cl[i]);
	printf("\n");
	for(i=0;i<pb->N;i++)
	{
		for(j=0;j<pb->ni[i];j++)
			printf("%d\t",sol->s[i][j]);
		printf("\n");
	}
}

void copy_sol(kps_prob *pb, kps_sol *sol1, kps_sol *sol2)
{
	int i, j, k;
	for(i=0;i<pb->N;i++)
	{
		for(j=0;j<pb->ni[i];j++)
			sol2->s[i][j]=sol1->s[i][j];
		sol2->cl[i]=sol2->cl[i];
	}
	sol2->vol=sol1->vol;
	sol2->pro=sol1->pro;
}