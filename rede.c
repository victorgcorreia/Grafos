#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rede.h"

//inimigo = 1;
//solicitação_enviada = 2;
//solicitação_recebida = 3;
//amigo = 4;

//Função que retorna o índice do login do usuario.
int logar_usuario(Rede *r, int num, char *nome, char *senha){

	if(r == NULL || nome == NULL || senha == NULL){
		return -1;
	}

	int i;
	for(i = 0; i < r->num_usuarios; i++ ){
		Usuario *aux = r->usuarios[i];
		if(strcmp(nome, aux->nome) == 0 && strcmp(senha, aux->senha) == 0)
			break;
	}

	if(i == r->num_usuarios)
		i = -1;

	return i;
}

//Função que busca usuario no banco de dados pelo seu nome e retorna seu índice
int busca_usuario(Rede *r, char *nome){
    if (r == NULL || nome == NULL)
    	return -1;

    Usuario *usuario;
    for (int i = 0; i < r->num_usuarios; i++){
        usuario = r->usuarios[i];
        if (strcmp(usuario->nome, nome)==0)
        	return i;
    }
    return -1;
}

//Função que imprime os dados do usuário na tela
void imprime_usuario(Usuario *usuario, FILE *in){
	if (in == NULL || usuario == NULL)
		return;
	fprintf(in, "Nome: %s\n", usuario->nome);
	fprintf(in, "Idade: %d\n", usuario->idade);
	fprintf(in, "Cidade: %s\n", usuario->cidade);
	fprintf(in, "Filme Favorito: %s\n", usuario->filme);
	fprintf(in, "Time de Futebol: %s\n", usuario->time);
	fprintf(in, "Cor Favorita: %s\n\n", usuario->cor);
}

//Função que irá realizar a impressão dos dados de um usuário buscado pelo nome
void imprime_perfil(Rede *r, char *nome){
	if (r == NULL || nome == NULL)
		return;

	int indice = busca_usuario(r, nome);
	if (indice == -1)
		puts("Usuario nao encontrado");
	else
		imprime_usuario(r->usuarios[indice], stdout);
}

//Função que faz a impressão da lista de amigos do usuário
void imprime_lista_amigos(Rede *r, int indice){

	if(r == NULL || indice < 0)
		return;

	printf("\nLista de amigos:\n" );
	int cont = 0;
	int i;
	for(i = 0; i < r->num_usuarios; i++){
		int aux;
		int erro;
		ver_aresta(r->amizades, &indice, &i, &aux, &erro);
		if( aux == 4 ){
			printf("%s\n", r->usuarios[i]->nome );
			cont++;
		}
	}

	if(cont == 0)
		printf("Voce nao possui amigos na rede.\n");
}

//Função que imprime os usuários da rede
void imprime_usuarios_rede(Rede *r){
	if (r == NULL)
		return;
	printf("\nNumero de usuarios na rede: %d\n", r->num_usuarios);
	for (int i = 0; i < r->num_usuarios; i++)
		printf("%s\n", r->usuarios[i]->nome);
	return;
}

//Função que retorna o menor entre a e b
int min(int a, int b){
	if(a>b)
		return b;
	else
		return a;
}

//Função que retorna o maior entre a e b 
int max(int a, int b){
	if(a>b)
		return a;
	else
		return b;
}

//Função que retorna o módulo de x 
int modulo(int x){
	if (x > 0)
		return x;
	else
		return -x;
}

//Função que realiza o login de um usuário na rede
Usuario *login(Rede *r, char *usuario, char *senha){
    if (r == NULL || usuario == NULL || senha == NULL)
    	return NULL;

    int indice = busca_usuario(r, usuario);
    if (indice == -1)
    	return NULL;

    Usuario *atual = r->usuarios[indice];
    if (!strcmp(atual->senha, senha))
    	return atual;

    return NULL;
}

//Função que cria o usuário e retorna o seu ponteiro.
Usuario *criar_usuario(char *nome, int idade, char *cidade, char *filme, char *time, char *cor, char *senha){
	Usuario *usuario = (Usuario *)malloc(sizeof(Usuario));

	usuario->idade = idade;
	strcpy(usuario->nome, nome);
	strcpy(usuario->cidade, cidade);
	strcpy(usuario->filme, filme);
	strcpy(usuario->time, time);
	strcpy(usuario->cor, cor);
	strcpy(usuario->senha, senha);

	return usuario;
}

//Função que libera a mémoria utilizada por um usuário
void libera_usuario(Usuario **usuario){
	
	if(usuario == NULL)
		return;
	free(*usuario);
	*usuario = NULL;
	return;
}

//Função que faz a leitura do arquivo contendo os dados dos usuários
void le_arquivo_usuarios(Rede *r){
	if (r == NULL)
		return;

	FILE *in = fopen("usuarios.txt", "r");

	Usuario *usuarios[100];

	char nome[100], cidade[100], filme[100], time[100], cor[100], senha[100];
	int idade;
	int i;

	for (i = 0; !feof(in); i++){
		fscanf(in, "%*[^:]%*c%*c%[^\n]%*c", nome);
		fscanf(in, "%*[^:]%*c%*c%d%*c", &idade);
		fscanf(in, "%*[^:]%*c%*c%[^\n]%*c", cidade);
		fscanf(in, "%*[^:]%*c%*c%[^\n]%*c", filme);
		fscanf(in, "%*[^:]%*c%*c%[^\n]%*c", time);
		fscanf(in, "%*[^:]%*c%*c%[^\n]%*c", cor);
		fscanf(in, "%*[^:]%*c%*c%[^\n]%*c", senha);

		if (feof(in))
			break;

		r->usuarios[i] = criar_usuario(nome, idade, cidade, filme, time, cor, senha);
	}

	r->num_usuarios = i;

	fclose(in);
}

//Função que cria um grafo com matriz baseado no arquivo de amizades dos usuários
G *cria_grafo_amizade(){

	FILE *in;
	in = fopen("amizades.txt", "r");

	int flag = 0, max = 100, erro = 0, aux, n;

	G* grafo = criar_grafo(&max, &flag, &erro);
	if (erro != 0){
		fclose(in);
		exit(0);
	}

	if(in == NULL){
		return grafo;
	}

	fscanf(in, "%d", &n);
	erro = 0;

	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){
			fscanf(in, "%d", &aux);
			inserir_aresta(grafo, &i, &j, &aux, &erro);
			if (erro != 0){
				fclose(in);
				exit(0);
			}
		}
	}
	fclose(in);
	return grafo;
}

//Função que retorna a afinidade entre dois usuários, sendo essa afinidade entre 0 (baixa) e 100 (alta)
int usuarios_afinidade(Usuario *usuario1, Usuario *usuario2, Rede *r){

	if (usuario1 == NULL || usuario2 == NULL)
		return 0;
	if (usuario1 == usuario2)
		return 100;

	double aux = 0;

	if (usuario1->idade == usuario2->idade)
		aux++;
	if (!strcmp(usuario1->cidade, usuario2->cidade))
		aux++;
	if (!strcmp(usuario1->filme, usuario2->filme))
		aux++;
	if (!strcmp(usuario1->time, usuario2->time))
		aux++;
	if (!strcmp(usuario1->cor, usuario2->cor))
		aux++;

	aux = aux/5;

	return (int)(aux*100);
}

//Função que cria o grafo de porcentagem da rede
G *cria_grafo_porcentagem(Rede *r){

	if(r == NULL)
		return NULL;

	int flag = 0, num_vertices = 100, erro = 0, i, j, afinidade;

	G *grafo = criar_grafo(&num_vertices, &flag, &erro);

	for(i = 0; i < r->num_usuarios; i++){
		for(j = i + 1; j < r->num_usuarios; j++){
			afinidade = usuarios_afinidade(r->usuarios[i], r->usuarios[j], r);
			inserir_aresta(grafo, &i, &j , &afinidade, &erro);
			inserir_aresta(grafo, &j, &i , &afinidade, &erro);
		}
	}

	return grafo;

}

//Função que, ao adicionar um novo usuário na rede, calcula as afinidades com os outros usuários e armazena no grafo de porcentagem
void adiciona_grafo_porcentagem(Rede *r){

	if(r == NULL){
		return;
	}

	if(r->num_usuarios > 100)
		return;

	int erro = 0, i, afinidade;
	int ult = r->num_usuarios - 1;

	for(i = 0; i < r->num_usuarios - 1; i++){
		afinidade = usuarios_afinidade(r->usuarios[ult], r->usuarios[i], r);
		inserir_aresta(r->porcentagem, &i, &ult , &afinidade, &erro);
		inserir_aresta(r->porcentagem, &ult, &i , &afinidade, &erro);
	}

	return;
}


//Função que cria a rede
Rede *criar_rede(){

	Rede *r = (Rede*)malloc(sizeof(Rede));

	if(r == NULL)
		return NULL;

	le_arquivo_usuarios(r);

	r->amizades = cria_grafo_amizade();
	r->porcentagem = cria_grafo_porcentagem(r);

	return r;
}

//Função que libera a memoria utilizada pela rede
void libera_rede(Rede **rede){

	if(rede == NULL || *rede == NULL)
		return;

	Rede *r = *rede;
	int erro =0, i;

	libera_grafo( &r->amizades , &erro);
	libera_grafo( &r->porcentagem , &erro);

	for(i = 0; i < r->num_usuarios; i++){
		libera_usuario( &r->usuarios[i] );
	}

	free(r);
	*rede = NULL;
	return;
}


//Função que calcula o número de amigos em comum entre dois usuários
int amigos_em_comum(Rede *r, Usuario *usuario1, char *usuario2){
	int indice1 = busca_usuario(r, usuario1->nome);
	int indice2 = busca_usuario(r, usuario2);
	int afinidade1, afinidade2, erro, aux = 0;

	for (int i = 0; i < r->num_usuarios; i++){
		ver_aresta(r->amizades, &indice1, &i, &afinidade1, &erro);
		ver_aresta(r->amizades, &indice2, &i, &afinidade2, &erro);
		if (afinidade1 == 4 && afinidade2 == 4)
			aux++;
	}
	return aux;
}

//Função que sugere possiveis novas amizades
void sugerir_nova_amizade(Rede* r, Usuario* usuario){
	
	int k;
	for (int i = 0; i < r->num_usuarios; ++i){	
		if (r->usuarios[i] == usuario){
			k = i;
			break;
		}
	}

	int afinidade, amizade, erro = 0, flag = 1;

	for (int i = 0; i < r->num_usuarios; ++i){
		ver_aresta(r->porcentagem, &k, &i, &afinidade, &erro);
		if (afinidade > amigoVerdadeiro && k != i){
			ver_aresta(r->amizades, &k, &i, &amizade, &erro);
			if (amizade == 1 || amizade == 3){
				if (flag){
					printf("Sugestoes de amizade:\n");
					flag = 0;
				}
				printf("\t%s com %d%% de afinidade\n", r->usuarios[i]->nome, afinidade);
			}
		}
	}
	if (flag){
		printf("Nao temos sugestoes no momento.\n");
	}
	return;
}

//Função que sugere possiveis remocoes ao usuário 
void sugerir_remocao(Rede* r, Usuario* usuario){
	int k = busca_usuario(r, usuario->nome);

	int afinidade, amizade, erro = 0, flag = 1;
	for (int i = 0; i < r->num_usuarios; ++i){
		ver_aresta(r->porcentagem, &k, &i, &afinidade, &erro);
		if (afinidade < amigoFalso && k != i){
			ver_aresta(r->amizades, &k, &i, &amizade, &erro);
			if (amizade == 4){
				if (flag){
					printf("Sugestoes de remocao:\n");
					flag = 0;
				}
			printf("\t%s com %d%% de afinidade\n", r->usuarios[i]->nome, afinidade);
			}
		}
	}
	if (flag){
		printf("Nao temos sugestoes no momento.\n");
	}
	return;
}

//Função que faz a remoção de um amigo da lista de amigos do usuário
void remover_amigo(Rede* r, Usuario* usuario, char* nome){
	int i = busca_usuario(r, usuario->nome);
	int j = busca_usuario(r, nome);

	if(i == j){
		puts("Nome do usuario a ser excluido igual ao do usuario logado");
		return;
	}

	if (j == -1){
		puts("Usuario nao encontrado.");
		return;
	}

	int inimigo = 1, erro = 0;
	inserir_aresta(r->amizades, &i, &j, &inimigo, &erro);
	inserir_aresta(r->amizades, &j, &i, &inimigo, &erro);
	printf("%s removido da sua lista de amigos.\n", nome);
	return;
}

//Função que faz a busca e indica um possivel namorado(a) para o usuário
void achar_pretendente(Rede* r, Usuario* usuario){
	int k = busca_usuario(r, usuario->nome);

	int aux, erro = 0, maior = 0, indice = 0;
	
	for (int i = 0; i < r->num_usuarios; ++i){
		ver_aresta(r->porcentagem, &k, &i, &aux, &erro);
		if (r->usuarios[i] != usuario && aux > maior){
			maior = aux;
			indice = i;
		}
	}

	if(maior!=0)
		printf("Recomendamos conversar com %s! (%d%% de afinidade).\n", r->usuarios[indice]->nome, maior);
	else
		printf("Nao encontramos um possivel namorado(a) no momento...\n");
	return;
}

//Função que faz a atualização do arquivo contendo os dados dos usuários
void atualizar_arquivo_usuarios(Rede *r){

	if(r == NULL){
		return;
	}

	FILE *in = fopen("usuarios.txt", "w");

	if(in == NULL)
		return;

	int i;

	for(i = 0; i < r->num_usuarios; i++){
		if(i != 0)
			fprintf( in, "\n" );

		Usuario *aux = r->usuarios[i];
		fprintf(in, "nome do usuário: %s\n", aux->nome);
		fprintf(in, "idade: %d\n", aux->idade);
		fprintf(in, "cidade: %s\n", aux->cidade);
		fprintf(in, "filme predileto: %s\n", aux->filme);
		fprintf(in, "time: %s\n", aux->time);
		fprintf(in, "cor predileta: %s\n", aux->cor);
		fprintf(in, "senha: %s\n", aux->senha);
	}
	fclose(in);
	return;
}

//Função que atualiza o arquivo de amizade dos usuários da rede
void atualizar_arquivo_amizades(Rede *r){
	if(r == NULL){
		return;
	}

	FILE *in = fopen("amizades.txt", "w");

	if(in == NULL)
		return;

	fprintf(in, "%d\n",  r->num_usuarios);

	int erro = 0, i, j, aux;

	for(i = 0; i < r->num_usuarios; i++){
		for(j = 0; j < r->num_usuarios; j++){
			ver_aresta(r->amizades, &i, &j, &aux, &erro);
			fprintf(in,"%d ", aux);
		}
		fprintf(in, "\n");
	}

	fclose(in);
	return;
}

//Função que faz a saída do programa e assim fechando a rede
void fechar(Rede **r){
	atualizar_arquivo_amizades(*r);
	atualizar_arquivo_usuarios(*r);
	libera_rede(r);
}

//Função que realiza o cadastro de um novo usuário
Usuario *cadastro(Rede *r){

	if(r == NULL)
		return NULL;

	if(r->num_usuarios > 100){
		printf("Numero excedido de usuarios na rede\n");
		return NULL;
	}

	char nome[100];
	int idade;
	char cidade[100];
	char filme[100];
	char time[100];
	char cor[100];
	char senha[100], confirmacao[100];

	int u = 0;
	while (u != -1){
		printf("Digite seu nome:\n");
		scanf("%*[\n]");
		scanf("%[^\n]%*c", nome);
		u = busca_usuario(r, nome);
		if (u != -1) puts("Usuario ja cadastrado.\n");
	}

	printf("Digite sua idade:\n");
	scanf("%d", &idade);

	printf("Digite sua cidade:\n");
	scanf("%*[\n]");
	scanf("%[^\n]%*c", cidade);

	printf("Digite o seu filme preferido\n");
	scanf("%*[\n]");
	scanf("%[^\n]%*c", filme);

	printf("Digite o time que voce torce:\n");
	scanf("%*[\n]");
	scanf("%[^\n]%*c", time);

	printf("Digite sua cor preferida:\n");
	scanf("%*[\n]");
	scanf("%[^\n]%*c", cor);

	while(1){
		printf("Digite a senha:\n");
		scanf("%s", senha);
		printf("Confirme a senha:\n");
		scanf("%s", confirmacao);

		if( strcmp(senha, confirmacao) == 0  && strlen(senha) != 0){
			break;
		}else{
			if(strlen(senha) == 0) printf("\nA senha nao pode ser vazia	\n");
			else
				printf("\nSenha e confirmacao nao bateram\n");
		}

	}

	Usuario *novo = criar_usuario(nome, idade, cidade, filme, time, cor, senha);
	r->usuarios[ r->num_usuarios ] = novo;
	r->num_usuarios += 1;
	adiciona_grafo_porcentagem(r);
	printf("Usuario adicionado com sucesso!!\n");
	return novo;
}

//Função que possibilita a edição do perfil do usuário
void editar_perfil(Rede *r, Usuario *u){

	int idade, aux1, aux2;
	char nome[100];
	char cidade[100];
	char filme[100];
	char time[100];
	char cor[100];
	char senha[100], confirmacao[100];

	printf("\n");
	puts("1 - Nome");
	puts("2 - Idade");
	puts("3 - Cidade");
	puts("4 - Filme favorito");
	puts("5 - Time");
	puts("6 - Cor favorita");
	printf("Digite sua opcao : ");
	scanf("%d", &aux1);
	printf("\n");
	switch(aux1){	
		case 1:
		aux2 = 0;
		while (aux2 != -1){
			printf("Nome:\n");
			scanf("%*[\n]");
			scanf("%[^\n]%*c", nome);
			aux2 = busca_usuario(r, nome);
			if (aux2 != -1)
				puts("Nome indisponível.\n");
		}
		
		puts("Nome alterado com sucesso.");
		strcpy(u->nome, nome);
		break;

		case 2:
		printf("Idade:\n");
		scanf("%d", &idade);
		
		puts("Idade alterada com sucesso.");
		u->idade = idade;	
		break;

		case 3:
		printf("Cidade:\n");
		scanf("%*[\n]");
		scanf("%[^\n]%*c", cidade);

		puts("Cidade alterada com sucesso.");
		strcpy(u->cidade, cidade);
		break;

		case 4:
		printf("Filme favorito\n");
		scanf("%*[\n]");
		scanf("%[^\n]%*c", filme);
		
		puts("Filme favorito alterado com sucesso.");
		strcpy(u->filme, filme);
		break;

		case 5:
		printf("Time:\n");
		scanf("%*[\n]");
		scanf("%[^\n]%*c", time);
		
		puts("Time alterado com sucesso.");
		strcpy(u->time, time);
		break;

		case 6:
		printf("Cor favorita:\n");
		scanf("%*[\n]");
		scanf("%[^\n]%*c", cor);

		strcpy(u->cor, cor);
		puts("Cor alterada com sucesso.");
		break;

	}
	
	return;
}

//Função que possibilita a edição da senha do usuário, pedindo a sua senha atual como medida de segurança
void editar_senha(Rede *r, Usuario *u){
	char senha[100], atual[100];
	char confirmacao[100];
	
	printf("Digite sua senha atual: ");
	scanf("%s", atual);
	if(strcmp(atual, u->senha)!=0){
		puts("Senha incorreta");
		return;
	}

	while(1){
		printf("Digite a nova senha:\n");
		scanf("%s", senha);
		printf("Confirme a nova senha:\n");
		scanf("%s", confirmacao);

		if(strcmp(senha, confirmacao) == 0  && strlen(senha) != 0){
			break;
		}else{
			if(strlen(senha) == 0)
				printf("\nA senha nao pode ter 0 caracteres\n");
			else
				printf("\nSenhas diferentes\n");
		}

	}
	strcpy(u->senha, senha);
	return;
}

//Função que realiza o envio de uma solicitação de amizade para um usuário na rede
void enviar_solicitacao(Rede *r, Usuario *u, char *nome){
    if (r == NULL || u == NULL || nome == NULL)
    	return;

    int indice1 = busca_usuario(r, u->nome);
    int indice2 = busca_usuario(r, nome);
    
    if (indice1 == indice2){
        puts("Voce nao pode enviar solicitacao para si mesmo.");
        return;
    }

    if (indice2 == -1){
        puts("Nao existe usuario com esse nome.");
        return;
    }

    int afinidade, erro, flag1 = 2, flag2 = 3;
    
    inserir_aresta(r->amizades, &indice1, &indice2, &flag1, &erro);
    inserir_aresta(r->amizades, &indice2, &indice1, &flag2, &erro);

    puts("Solicitacao enviada com sucesso.");
    return;
}

//Função que aceita a solicitação do usuário e torna os usuários amigos 
void aceita_solicitacao(Rede *r, Usuario *usuario, int i){
    int indice, erro, amigo = 4;
    indice = busca_usuario(r, usuario->nome);
    inserir_aresta(r->amizades, &i, &indice, &amigo, &erro);
    inserir_aresta(r->amizades, &indice, &i, &amigo, &erro);
}


//Função chamada caso o usuário queira recusar a solicitação de amizade
void recusa_solicitacao(Rede *r, Usuario *usuario, int i){
    int indice, erro, amigo = 1;
    indice = busca_usuario(r, usuario->nome);
    inserir_aresta(r->amizades, &i, &indice, &amigo, &erro);
    inserir_aresta(r->amizades, &indice, &i, &amigo, &erro);
}

//Função que mostra a solicitação de amizade de um usuário da rede para o usuário logado
void procedimento_solicitacao(Rede *r, Usuario *usuario, int i){
    int opcao;
    int afinidade = usuarios_afinidade(usuario, r->usuarios[i], r);
    printf("Voce recebeu solicitacao de %s com %d %% de chance de amizade verdadeira \n", r->usuarios[i]->nome, afinidade);
    printf("1: aceitar\n2: recusar\n");
    
    scanf("%d", &opcao);
    if (opcao == 1){
    	aceita_solicitacao(r, usuario, i);
    	printf("Solicitacao aceita com sucesso\n");
    }else{
    	recusa_solicitacao(r, usuario, i);
    	printf("Solicitacao recusada com sucesso\n");
    }
    return;
}

//Função que mostra todas as solicitações de amizade no momento para o usuário logado
void ver_solicitacao(Rede *r, Usuario *usuario){
    if (r == NULL || usuario == NULL)
    	return;

    int indice = busca_usuario(r, usuario->nome);

    int aux, erro, flag = 0;
    for (int i = 0; i < r->num_usuarios; i++){
        ver_aresta(r->amizades, &indice, &i, &aux, &erro);
        if (aux == 3){
            flag = 1;
            procedimento_solicitacao(r, usuario, i);
        }
    }
    if (!flag){
        puts("Nao ha solicitacoes de amizades.");
    }
}

//Função principal que chama as outras funcionalidades.
void entrar(Rede *r, Usuario *u){
    if (r == NULL || u == NULL) 
        return;

    int option, indice;
    char nome[100], senha[100];

    system("clear");
    printf("Bem Vindo %s!\n", u->nome);
    puts("1:\tMeu perfil");
    puts("2: \tEditar perfil");
    puts("3: \tEditar senha");
    puts("4: \tEncontrar provavel parceiro");
    puts("5: \tEnviar solicitacao de amizade");
    puts("6: \tRemover amigo");
    puts("7: \tSugerir amigo falso");
    puts("8: \tSugerir nova amizade");
    puts("9: \tVer lista de amigos");
    puts("10: \tVer perfil do usuario");
    puts("11: \tVer solicitacoes de amizade");
    puts("12:\tVer usuarios na rede");
    puts("13:\tSair");
    scanf("%d", &option);

    while (option != 13){
        switch (option){
        	case 1:
        		imprime_perfil(r, u->nome);
    	    	getchar();
	        	break;

            case 2: 
            	editar_perfil(r, u);
               	getchar();
               	break;

            case 3:
            	editar_senha(r, u);
            	getchar();
            	break;

            case 4:
                achar_pretendente(r, u);
                getchar();
                break;

            case 5:
                puts("Digite o nome do usuario");
                scanf("%*[\n]");
                scanf("%[^\n]%*c", nome);
                enviar_solicitacao(r, u, nome);
                getchar();
                break;

            case 6:
                puts("Digite o nome do usuario");
                scanf("%*[\n]");
                scanf("%[^\n]%*c", nome);
                remover_amigo(r, u, nome);
                break;

            case 7:
                sugerir_remocao(r, u);
                getchar();
                break;

            case 8:
                sugerir_nova_amizade(r, u);
                getchar();
                break;

            case 9:
            	indice = busca_usuario(r, u->nome);
              	imprime_lista_amigos(r, indice);
              	printf("\n");
              	getchar();
                break;

            case 10:
            	printf("Digite o nome do usuario\n");
                scanf("%*[\n]");
                scanf("%[^\n]", nome);
                imprime_perfil(r, nome);
                getchar();
                break;

            case 11:
       			ver_solicitacao(r, u);
                getchar();
                break;

            case 12:
            	imprime_usuarios_rede(r);
            	getchar();
                break;
        }

        puts("Pressione enter para continuar...");
        getchar();

        if (option != 13){
            system("clear");
		    printf("Bem Vindo %s!\n", u->nome);
    		puts("1: \tMeu perfil");
		    puts("2: \tEditar perfil");
		    puts("3: \tEditar senha");
		    puts("4: \tEncontrar provavel parceiro");
		    puts("5: \tEnviar solicitacao de amizade");
		    puts("6: \tRemover amigo");
		    puts("7: \tSugerir amigo falso");
		    puts("8: \tSugerir nova amizade");
		    puts("9: \tVer lista de amigos");
		    puts("10: \tVer perfil do usuario");
		    puts("11: \tVer solicitacoes de amizade");
		    puts("12:\tVer usuarios na rede");
		    puts("13:\tSair");
            scanf("%d", &option);
        }
    }
}
