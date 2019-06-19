#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rede.h"

int main(){
    int option;

    Rede *r = criar_rede();
    Usuario *usuario;
    char nome[100], senha[100];

    system("clear");

	puts("                          _                       _       _ ");
	puts("     /\\                  | |                     (_)     | |");
	puts("    /  \\     _ __ ___  __| | ___   ___  ___   ___ _  __ _| |");
	puts("   / /\\ \\   | '__/ _ \\/ _` |/ _ \\ / __|/ _ \\ / __| |/ _` | |");
	puts("  / ____ \\  | | |  __/ (_| |  __/ \\__ \\ (_) | (__| | (_| | |");
	puts(" /_/    \\_\\ |_|  \\___|\\__,_|\\___| |___/\\___/ \\___|_|\\__,_|_|");
	printf("\n");
    puts("1: Logar");
    puts("2: Cadastrar");
    puts("3: Ver usuarios na rede");
    puts("4: Ver perfil de um usuario");
    puts("5: Sair");
    scanf("%d", &option);

    while (option != 5){
        switch (option){
            case 1:
                usuario = NULL;
                printf("Digite o nome de usuario: ");
                scanf("%*[\n]");
                scanf("%[^\n]%*c", nome);
                printf("Digite a senha: ");
                scanf("%*[\n]");
                scanf("%[^\n]%*c", senha);

                usuario = login(r, nome, senha);
                if (usuario == NULL)
                    puts("Nome ou senha invalidos...\n");
                
                if(usuario != NULL)
                	entrar(r, usuario);
                
                break;

            case 2:
                usuario = cadastro(r);
                entrar(r, usuario);
                break;

            case 3:
                imprime_usuarios_rede(r);
                getchar();
                break;

            case 4:
                printf("Digite o nome do usuario\n");
                scanf("%*[\n]");
                scanf("%[^\n]", nome);
                imprime_perfil(r, nome);
                getchar();
                break;
        }

        puts("Pressione enter para continuar...");
        getchar();

        system("clear");

		puts("                          _                       _       _ ");
		puts("     /\\                  | |                     (_)     | |");
		puts("    /  \\     _ __ ___  __| | ___   ___  ___   ___ _  __ _| |");
		puts("   / /\\ \\   | '__/ _ \\/ _` |/ _ \\ / __|/ _ \\ / __| |/ _` | |");
		puts("  / ____ \\  | | |  __/ (_| |  __/ \\__ \\ (_) | (__| | (_| | |");
		puts(" /_/    \\_\\ |_|  \\___|\\__,_|\\___| |___/\\___/ \\___|_|\\__,_|_|");
		printf("\n");
	    puts("1: Logar");
	    puts("2: Cadastrar");
	    puts("3: Ver usuarios na rede");
	    puts("4: Ver perfil de um usuario");
	    puts("5: Sair");
        scanf("%d", &option);
    }

    fechar(&r);
}