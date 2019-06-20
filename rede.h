#include "grafos.h"
#define amigoVerdadeiro 60
#define amigoFalso 40

typedef struct usuario{
	char nome[100];
	int idade;
	char cidade[100];
	char filme[100];
	char time[100];
	char cor[100];
	char senha[100];
} Usuario;

typedef struct rede{
	int num_usuarios;
	Usuario *usuarios[100];
	G* amizades;
	G* porcentagem;
}Rede;

Rede *criar_rede();
Usuario *cadastro(Rede *r);
Usuario *login(Rede *r, char *nome, char *senha);
void imprime_perfil(Rede *r, char *nome);
void imprime_usuarios_rede(Rede *r);
void sugerir_nova_amizade(Rede* rede, Usuario* usuariologado);
void sugerir_remocao(Rede* rede, Usuario* usuariologado);
void remover_amigo(Rede* rede, Usuario* usuario, char* remover);
void ver_solicitacao(Rede *r, Usuario *u);
void achar_pretendente(Rede *r, Usuario *u);
void editar_perfil(Rede *r, Usuario *u);
void editar_senha(Rede *r, Usuario *u);
void enviar_solicitacao(Rede *r, Usuario *a, char *b);
void entrar(Rede *r, Usuario *u);
void fechar(Rede **r);
int amigos_em_comum(Rede *r, Usuario *u1, char *u2);