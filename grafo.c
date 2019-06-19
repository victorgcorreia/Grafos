#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "grafo.h"

G* criar_grafo(int *num_vertices,elem* std, int *erro){
	G *a;
	a = malloc(sizeof(G));
	a->std = *std;
	a->m = (elem**)malloc(sizeof( elem* ) * (*num_vertices) );

	int i,j;
	for (i = 0; i < (*num_vertices); i++){
		a->m[i] = (elem*)malloc(sizeof(elem) * (*num_vertices));
	}

	for (i = 0; i < (*num_vertices); i++){
		for (j = 0; j < (*num_vertices); j++){
			a->m[i][j] = a->std;
		}
	}

	a->num_vertices = (*num_vertices);

	return a;
	*erro = 0;
}

void libera_grafo(G** g, int *erro){
	if((*g) == NULL)
		return;
	int i;
	for (i = 0; i < (*g)->num_vertices; i++){
		free( (*g)->m[i]);
	}

	free((*g)->m);
	free ((*g));
	*g = NULL;
	*erro = 0;
}

void inserir_aresta(G*g, int *v1, int*v2, elem* peso, int*erro){
	if(g == NULL){
		*erro = 1;
		return;
	}

	if((*v1) >= g->num_vertices || (*v2) >= g->num_vertices){
		*erro = 2;
		return;
	}

	g->m[ *v1][ *v2] = *peso;
	*erro = 0;
}

void ver_aresta(G*g, int *v1, int*v2, elem* peso, int* erro){
	if(g == NULL){
		*erro = 1;
		return;
	}

	if((*v1) >= g->num_vertices || (*v2) >= g->num_vertices){
		*erro = 2;
		return;
	}

	*peso = g->m[*v1][*v2];
	*erro = 0;
}