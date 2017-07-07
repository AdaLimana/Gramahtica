#include"estado.h"
#include<stdio.h>


void inicializaGramatica(lista *gramatica, char *idEstado, int qntCaracter, int qntEstado);
void inicializaEstadosGramatica(lista *gramatica);
void imprimeGramatica(lista *gramatica);
estado* procuraEstado(lista *gramatica, char *idEstado);
estado* criaEstadoGramatica( lista *gramatica, char *idEstado, int estado_usuario);
void ordenaIdentificador(char *identificador);
void determinizaGramatica(lista *gramatica);
void fusaoEstado(lista *gramatica, lista *estadoPossivel);
void excluiRepeticao(char *string);
void excluiGramatica(lista *gramatica);
void excluiEstadoGramatica(lista *gramatica, estado* estadoExclui);
void estadoInalcansavel(lista *gramatica);
void estadoMorto(lista *gramatica);

char *idCaracter;

int main(){


	int qntEstado, qntCaracter, i;
	lista gramatica;
	inicializa(&gramatica);

	printf("\e[H\e[2J");/*limpa a tela*/
	printf("\nInforme a quantidade de estados\n");
	scanf("%d",&qntEstado);

	printf("\e[H\e[2J");/*limpa a tela*/
	printf("\nInforme a quantidade de caracteres do alfabeto\n");
	scanf("%d",&qntCaracter);

	idCaracter = (char*) malloc(qntCaracter*sizeof(char));/*aloca um vetor para todos os caracter do alfabeto*/
	char *idEstado = (char*) malloc(qntEstado*sizeof(char));/*aloca um vetor para todos o estados iniciais*/

	i=0;

	printf("\e[H\e[2J");/*limpa a tela*/
	printf("\nInforme os caracteres do alfabeto\n");

	while(i < qntCaracter){

		scanf(" %c",&idCaracter[i]);
		i++;
		printf("\e[H\e[2J");/*limpa a tela*/
	}
	i=0;

	printf("\e[H\e[2J");/*limpa a tela*/
	printf("\nInforme os identificadores de estados\n");
	while(i < qntEstado){
                scanf(" %c",&idEstado[i]);
                i++;
		printf("\e[H\e[2J");/*limpa a tela*/
        }
        i=0;

	inicializaGramatica(&gramatica, idEstado, qntCaracter, qntEstado);

	inicializaEstadosGramatica(&gramatica);

	imprimeGramatica(&gramatica);


	determinizaGramatica(&gramatica);

	estadoInalcansavel(&gramatica);

	estadoMorto(&gramatica);;

	imprimeGramatica(&gramatica);
	printf("\nGramatica final\n");
	excluiGramatica(&gramatica);
	free(idEstado);
	free(idCaracter);
	return 0;

}



void inicializaGramatica(lista *gramatica, char *idEstado, int qntCaracter, int qntEstado){

	int i=0;
	caracter *pontCaracter;
	char idNovoEstado[1];
	while(i < qntEstado){
		idNovoEstado[0] = idEstado[i];

		if(i == 0){
			criaEstadoGramatica(gramatica, idNovoEstado, 0);/*o primeiro estado é especial, é como se fosse um estado determinizado*/

		}
		else{
			criaEstadoGramatica(gramatica, idNovoEstado, 1);/*passa (1) pois é um estado criado pelo usuário, no caso não pela determinização*/

		}
                i++;
        }


}

estado* criaEstadoGramatica(lista *gramatica, char *idEstado, int estado_usuario){
	int i = 0;
	caracter *pontCaracter;
	estado *e;
	e = criaEstado(strlen(idCaracter),idEstado);
	e->estadoUsuario = estado_usuario;
	while(i < strlen(idCaracter)){/*cria o alfabeto do estado*/
		pontCaracter = criaCaracter((void *)idCaracter[i]);/*cria um caracter para o estado*/
		insereNoAlfabeto( e, pontCaracter);
		i++;

	}
	if(e){
		inserefim(gramatica, (void*)e);/*coloca o novo estado na lista da gramatica*/
	}
	return e;

}

void excluiGramatica(lista *gramatica){

	estado *estadoExclui;
	no *noGramatica;
	noGramatica = (no*)gramatica->primeiro;
	while(noGramatica){
		estadoExclui = (estado*) noGramatica->memoria;
		excluiEstadoGramatica(gramatica, estadoExclui);
		noGramatica = (no*)noGramatica->prox;
	}

}

void excluiEstadoGramatica(lista *gramatica, estado *estadoExclui){

	int posicaoEstado = 0;
	no *noGramaticaExclui;
	lista *alfabetoExclui;
	no *noAlfabetoExclui;
	caracter *caracterExclui;
	lista *estadoPossivelExclui;

	noGramaticaExclui = retornaNoPosicao(gramatica, posicaoEstado);/*retorna o (no) referente a posicao*/
	while((estado*)noGramaticaExclui->memoria != estadoExclui){/*busca a posicao do (no) que será excluido da lista gramatica*/
		posicaoEstado++;
		noGramaticaExclui = retornaNoPosicao(gramatica, posicaoEstado);
	}
	alfabetoExclui = &estadoExclui->alfabeto;
	noAlfabetoExclui = (no*)alfabetoExclui->primeiro;
	while(noAlfabetoExclui){
		caracterExclui = (caracter*)noAlfabetoExclui->memoria;
		estadoPossivelExclui = &caracterExclui->estadoPossivel;
		liberalistaB(estadoPossivelExclui);/*exclui a lista, mas nao os (estados) que estavam na memoria, pois estão também em outras listas*/
		noAlfabetoExclui = (no*)noAlfabetoExclui->prox;

	}
	liberalista(alfabetoExclui);/*exclui a lista alfabeto do estado que sera excluido*/
	free(estadoExclui->identificador);/*exclui o identificador do estado que sera excluido*/
	excluiEm(gramatica, posicaoEstado);/*exclui o estado da lista gramatica na sua respectiva posicao*/


}

void inicializaEstadosGramatica(lista *gramatica){

	char idEstado[10];
	char testaEstadoFinal[4];
	no *noListaGramatica;
	estado *pontEstado;


	lista *alfabeto;
	no *noListaAlfabeto;
	caracter *pontCaracter;

	noListaGramatica = (no*)gramatica->primeiro;/*recebe o primeiro no da lista*/

	while(noListaGramatica){
		pontEstado = (estado*) noListaGramatica->memoria;/*recebe o "estado" que o  no da lista aponta*/
		printf("\e[H\e[2J");/*limpa a tela*/
		printf("\n(%s) e estado fina? (sim) ou (nao)\n",pontEstado->identificador);
		scanf("%s", testaEstadoFinal);
		if(!strcmp(testaEstadoFinal, "sim")){
			pontEstado->estadoFinal = 1;

		}

		printf("\nNo estado (%s)\n", pontEstado->identificador);

		alfabeto = &pontEstado->alfabeto; /*passa a lista do alfabeto do estado apontado por (pontEstado)*/
		noListaAlfabeto = (no*)alfabeto->primeiro;/*recebe o primeiro no da lista*/
		while(noListaAlfabeto){
			pontCaracter = (caracter*)noListaAlfabeto->memoria;/*recebe o "caracter que o no da lista aponta"*/
			printf("Lendo (%c) vai para? Se nao tiver estado digite (nenhum)",(char)pontCaracter->identificador);
			while(1){
				scanf("%s", idEstado);
				if(! strcmp(idEstado, "nenhum")){
					break;
				}
				pontEstado= procuraEstado(gramatica, idEstado);
				if(!pontEstado){
					printf("\nEstado nao existe\n");
				}
				else{
					colocaEstado(pontCaracter, (void*)pontEstado);/*coloca para qual estado vai quando é este caracter*/
				}
			}
			noListaAlfabeto = (no*)noListaAlfabeto->prox;/*recebe o proximo no*/

		}
		noListaGramatica = (no*)noListaGramatica->prox;/*recebe o prox no*/
	}
}



void determinizaGramatica(lista *gramatica){

	int s;
	estado *pontEstado;

	estado *novoEstado;

	lista *alfabeto;
	no *noListaAlfabeto;
	caracter *pontCaracter;

	lista *estadoPossivel;
	no *noListaEstadoPossivel;
	estado *pontEstadoPossivel;

	no *noListaGramatica = (no*) gramatica->primeiro;
	while(noListaGramatica){

		pontEstado = (estado*)noListaGramatica->memoria;/*recebe o end do estado*/
		alfabeto = &pontEstado->alfabeto;/*recebe a lista de alfabeto do estado*/
		noListaAlfabeto = (no*) alfabeto->primeiro;

		while(noListaAlfabeto){
			pontCaracter = (caracter*) noListaAlfabeto->memoria;/*recebe o end do caracter do alfabeto*/

			estadoPossivel = &pontCaracter->estadoPossivel;
			noListaEstadoPossivel = (no*) estadoPossivel->primeiro;/*ACHO QUE POSSO EXCLUIR*/
			if(estadoPossivel->tamanho > 1){
				fusaoEstado(gramatica, estadoPossivel);
			}
			imprimeGramatica(gramatica);

			printf("\nDeterminizando a Gramatica\n");
			printf("\nDigite qualquer numero para continuar\n");
			scanf("%d",&s);
			noListaAlfabeto = (no*) noListaAlfabeto->prox;

		}
		noListaGramatica = (no*) noListaGramatica->prox;
	}

}


void estadoInalcansavel(lista *gramatica){
	int s;
	int alcansavel = 0;
	no *noGramaticaCompara;
	estado *estadoCompara;
	lista *alfabetoCompara;
	no *noAlfabetoCompara;
	caracter *caracterCompara;
	lista *estadoPossivelCompara;
	no *noEstadoPossivelCompara;

	no* noGramaticaAux;
	no *noGramatica = (no*) gramatica->primeiro;
	estado *estadoAlcansavel; /*se o estado está presente em possivel escolha de outro, esse estado passa ser alcansável*/
	noGramatica = (no*) noGramatica->prox;/*o primeiro estado sempre é alcansável*/
	while(noGramatica){

		noGramaticaCompara = (no*) gramatica->primeiro;
		while(noGramaticaCompara){

			if(noGramaticaCompara != noGramatica){/*não comprara com o mesmo estado*/
				estadoCompara = (estado*) noGramaticaCompara->memoria;

				if(!estadoCompara->estadoUsuario){/*Não compara com estados criados pelo usuário, a não ser que esse tenha se tornado alcansável*/

					alfabetoCompara = &estadoCompara->alfabeto;
					noAlfabetoCompara = (no*) alfabetoCompara->primeiro;
					while(noAlfabetoCompara){
						caracterCompara = (caracter*)noAlfabetoCompara->memoria;
						estadoPossivelCompara = &caracterCompara->estadoPossivel;
						noEstadoPossivelCompara = (no*) estadoPossivelCompara->primeiro;
						while(noEstadoPossivelCompara){
							if(noEstadoPossivelCompara->memoria == noGramatica->memoria){
								estadoAlcansavel = (estado*) noGramatica->memoria;
								estadoAlcansavel->estadoUsuario = 0;/*esse estado ficara, pois é alcansável, e para os outros estados verificarem esse também, ele é setado por 0*/
								alcansavel = 1;
								break;
							}
							noEstadoPossivelCompara = (no*)noEstadoPossivelCompara->prox;
						}
						if(alcansavel){
							break;
						}
						noAlfabetoCompara = (no*) noAlfabetoCompara->prox;
					}

					if(alcansavel){
						break;

					}

				}

			}
			if(alcansavel){
				break;
			}
			noGramaticaCompara = (no*)noGramaticaCompara->prox;

		}
		noGramaticaAux = (no*)noGramatica->prox;
		if(!alcansavel){

			excluiEstadoGramatica(gramatica, (estado*)noGramatica->memoria);
		}
		alcansavel = 0;
		noGramatica = noGramaticaAux;
		imprimeGramatica(gramatica);
		printf("\nEliminando Estados Inalcansaveis\n");
                printf("\nDigite qualquer numero para continuar\n");
                scanf("%d",&s);


	}

}

void estadoMorto(lista *gramatica){
	int s;
	int estadoMorto = 0;
	no *noGramatica;
	estado *pontEstado;
	lista *alfabetoGramatica;
	no *noAlfabetoGramatica;
	caracter *pontCaracter;
	lista *estadoPossivelGramatica;
	no *noEstadoPossivelGramatica;

	no *noGramaticaAux;
	noGramatica = (no*)gramatica->primeiro;
	while(noGramatica){
		pontEstado = (estado*) noGramatica->memoria;
		alfabetoGramatica = &pontEstado->alfabeto;
		noAlfabetoGramatica = (no*) alfabetoGramatica->primeiro;
		while(noAlfabetoGramatica){

			if(pontEstado->estadoFinal){/*se for estado final náo pode ser estado morto*/
                                estadoMorto = 1;
                                break;
                        }

			pontCaracter = (caracter*)noAlfabetoGramatica->memoria;
			estadoPossivelGramatica = &pontCaracter->estadoPossivel;
			noEstadoPossivelGramatica = (no*)estadoPossivelGramatica->primeiro;

			while(noEstadoPossivelGramatica){
				if(pontEstado != (estado*)noEstadoPossivelGramatica->memoria){
					estadoMorto = 1;
					break;
				}
				noEstadoPossivelGramatica = (no*) noEstadoPossivelGramatica->prox;

			}
			if(estadoMorto){
				break;
			}
			noAlfabetoGramatica = (no*) noAlfabetoGramatica->prox;

		}
		noGramaticaAux = (no*) noGramatica->prox;
		if(!estadoMorto){
			excluiEstadoGramatica(gramatica, (estado*)noGramatica->memoria);
		}
		noGramatica = noGramaticaAux;
		estadoMorto = 0;
		imprimeGramatica(gramatica);
		printf("\nEliminando Estados Mortos\n");
                printf("\nDigite qualquer numero para continuar\n");
                scanf("%d",&s);
	}

}

void ordenaIdentificador(char *identificador){

        int i=0;
        char aux;
        int cont = strlen(identificador)-1;
        while(0 < cont){

                while(i < cont){
                        if(identificador[i] > identificador[i+1]){
                                aux = identificador[i+1];
                                identificador[i+1] = identificador[i];
                                identificador[i] = aux;

                        }
                        i++;
                }
                i = 0;
                cont--;
        }

}


void excluiRepeticao(char *string){

        int i=0;
        int j=0;
        int cont=0;
        int indNovaString=0;
        int tamanho = 1+strlen(string);
        char *novaString = (char*)malloc(tamanho*sizeof(char));

        while(i < tamanho-1){
                j=0;
                while(j < i){

                        if(string[i] == novaString[j]){
                                cont = 1;
                                break;
                        }
                        j++;
                }
                if(!cont){
                        novaString[indNovaString] = string[i];
                        indNovaString++;
                }
                else{
                        cont = 0;
                }
                i++;
        }

        strcpy(string, novaString);
        free(novaString);


}

void imprimeGramatica(lista *gramatica){
	printf("\e[H\e[2J");/*limpa a tela*/
	no *noListaGramatica;
	estado *pontEstado;
	lista *alfabeto;
	no *noListaAlfabeto;
	caracter *pontCaracter;

	lista *estadoPossivel;
	no *noListaEstadoPossivel;
	estado *pontEstadoPossivel;

	void *estadosCaracter;

	noListaGramatica = (no*)gramatica->primeiro;

	while(noListaGramatica){
		pontEstado = (estado*) noListaGramatica->memoria;
                printf("ESTADO (%s)\t", pontEstado->identificador);

		alfabeto = &pontEstado->alfabeto;
		noListaAlfabeto = (no*) alfabeto->primeiro;

		while(noListaAlfabeto){
                        pontCaracter = (caracter*)noListaAlfabeto->memoria;
			estadoPossivel = &pontCaracter->estadoPossivel;
			noListaEstadoPossivel = (no*)estadoPossivel->primeiro;

			while(noListaEstadoPossivel){
				pontEstadoPossivel = (estado*) noListaEstadoPossivel->memoria;
				printf("%c,(%s)\t",(char)pontCaracter->identificador, pontEstadoPossivel->identificador);
				noListaEstadoPossivel = (no*) noListaEstadoPossivel->prox;
                        }
			noListaAlfabeto = (no*)noListaAlfabeto->prox;
                }
		if(pontEstado->estadoFinal){
			printf("\t &");
		}
		printf("\n");
		noListaGramatica = (no*) noListaGramatica->prox;
        }

}

estado* procuraEstado(lista *gramatica, char *idEstado){

	no *noListaGramatica;
	estado *pontEstado;
	noListaGramatica = (no*)gramatica->primeiro;
	while(noListaGramatica){
		pontEstado = (estado*) noListaGramatica->memoria;
		if(!strcmp(pontEstado->identificador, idEstado)){
			return pontEstado;
		}
		noListaGramatica = (no*)noListaGramatica->prox;
	}
	return NULL;

}




void fusaoEstado(lista *gramatica, lista *estadoPossivel){

	int idTamanho = 1;

	char *idEstado;

	estado *novoEstado;
	no *buscaNo;

	no *noListaEstadoPossivel;
	estado *pontEstadoPossivel;


	lista *alfabetoEstadoPossivel;
	no *noListaAlfabetoEstadoPossivel;
	caracter *pontCaracterEstadoPossivel;

	lista novoEstadoPossivel;
	no *noListaNovoEstadoPossivel;
	estado *pontNovoEstadoPossivel;

	lista *alfabetoNovoEstadoPossivel;
	no *noListaAlfabetoNovoEstadoPossivel;
	caracter *pontCaracterNovoEstadoPossivel;

	lista *estadoPossivelNovoEstadoPossivel;
	no *noListaEstadoPossivelNovoEstadoPossivel;
	estado *pontEstadoPossivelNovoEstadoPossivel;

	noListaEstadoPossivel = (no*) estadoPossivel->primeiro;

	while(noListaEstadoPossivel){/*determina o tamanho nome do novo estado*/
		pontEstadoPossivel = (estado*)noListaEstadoPossivel->memoria;
		idTamanho = idTamanho + strlen(pontEstadoPossivel->identificador);
		noListaEstadoPossivel = (no*) noListaEstadoPossivel->prox;
	}

	idEstado = (char*)malloc(idTamanho * sizeof(char));/*aloca o tamanho necessario para o novo nome*/
	memset(idEstado,'\0', idTamanho);/*zera a estring para o novo nome*/

	noListaEstadoPossivel = (no*) estadoPossivel->primeiro;

	while(noListaEstadoPossivel){/*determina o nome do novo estado*/
		pontEstadoPossivel = (estado*)noListaEstadoPossivel->memoria;

		if(!strstr(idEstado, pontEstadoPossivel->identificador)){/*verifica se a string já não está contida no idEstado */
			 strcat(idEstado, pontEstadoPossivel->identificador);/*concatena os identificadores para um novo nome*/
		}

		noListaEstadoPossivel = (no*)noListaEstadoPossivel->prox;
	}

	ordenaIdentificador(idEstado);/*ordena os identificadores em ordem alfabetica*/
	excluiRepeticao(idEstado);/*Exclui a repeticao dos identificadores*/

	novoEstado = procuraEstado(gramatica, idEstado);
	if(!novoEstado){/*se o estado nao existir*/
		novoEstado = criaEstadoGramatica(gramatica, idEstado, 0);/*cria o novo estado setanto o seu nome, passa (0)pois não é um estado que foi criado no inicio*/
		inicializa(&novoEstadoPossivel);/*guarda as referencias dos estado que derivaram o novo estado*/

		noListaEstadoPossivel = (no*) estadoPossivel->primeiro;

		while(noListaEstadoPossivel){/*guarda a referencia dos estados derivados, e verifica algum deles era final, caso for verdade o novo estado também e fina*/
			pontEstadoPossivel = (estado*)noListaEstadoPossivel->memoria;
			if(pontEstadoPossivel->estadoFinal){
				novoEstado->estadoFinal = 1;

			}
			inserefim(&novoEstadoPossivel, (void*)pontEstadoPossivel);

			noListaEstadoPossivel = (no*)noListaEstadoPossivel->prox;
		}
		liberaEstado(estadoPossivel);/*exclui os estado anteriores da lista*/
        	inserefim(estadoPossivel, (void*)novoEstado);

		alfabetoEstadoPossivel = &novoEstado->alfabeto;
		noListaAlfabetoEstadoPossivel = (no*) alfabetoEstadoPossivel->primeiro;

		while(noListaAlfabetoEstadoPossivel){

			pontCaracterEstadoPossivel = (caracter*) noListaAlfabetoEstadoPossivel->memoria;/*determina um caracter para ver seus estados*/

			noListaNovoEstadoPossivel = (no*) novoEstadoPossivel.primeiro;

			while(noListaNovoEstadoPossivel){/*pega dos estados que o derivaram os destinos para determinado caracter*/
				pontNovoEstadoPossivel = (estado *)noListaNovoEstadoPossivel->memoria;
				alfabetoNovoEstadoPossivel = &pontNovoEstadoPossivel->alfabeto;
				noListaAlfabetoNovoEstadoPossivel = (no*) alfabetoNovoEstadoPossivel->primeiro;

				while(noListaAlfabetoNovoEstadoPossivel){
					pontCaracterNovoEstadoPossivel = (caracter*)noListaAlfabetoNovoEstadoPossivel->memoria;
					/*verifica se saos os mesmos caracterres*/
					if(pontCaracterEstadoPossivel->identificador == pontCaracterNovoEstadoPossivel->identificador){

						/*recebe a lista de estadoPossiveis do caracter*/
						estadoPossivelNovoEstadoPossivel =&pontCaracterNovoEstadoPossivel->estadoPossivel;
						noListaEstadoPossivelNovoEstadoPossivel = (no*)estadoPossivelNovoEstadoPossivel->primeiro;

						while(noListaEstadoPossivelNovoEstadoPossivel){

							pontEstadoPossivelNovoEstadoPossivel = (estado*) noListaEstadoPossivelNovoEstadoPossivel->memoria;
							colocaEstado(pontCaracterEstadoPossivel, (void*)pontEstadoPossivelNovoEstadoPossivel);
							noListaEstadoPossivelNovoEstadoPossivel = (no*) noListaEstadoPossivelNovoEstadoPossivel->prox;
						}

					}

					noListaAlfabetoNovoEstadoPossivel = (no*) noListaAlfabetoNovoEstadoPossivel->prox;
				}
				noListaNovoEstadoPossivel = (no*) noListaNovoEstadoPossivel->prox;
			}
			noListaAlfabetoEstadoPossivel = (no*) noListaAlfabetoEstadoPossivel->prox;
		}

	}
	else{
		liberaEstado(estadoPossivel);/*exclui os estado anteriores da lista*/
		inserefim(estadoPossivel, (void*)novoEstado);
	}

}

