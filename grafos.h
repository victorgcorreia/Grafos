typedef int elem;

typedef struct grafo{
	elem std;
	elem **m;
	int num_vertices;
}G;

G* criar_grafo(int *num_vertices,elem* std, int *erro);
void libera_grafo(G** g, int *erro);
void inserir_aresta(G*g, int *v1, int*v2, elem* peso, int*erro);
void ver_aresta(G*g, int *v1, int*v2, elem* peso, int*erro);