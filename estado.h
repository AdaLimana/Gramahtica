#include<string.h>
#include"caracter.h"

typedef struct{

	char *identificador;
	lista alfabeto;
	int estadoFinal;
	int estadoUsuario;/*estado criado pelo usuario, não pela determinização*/

}estado;


estado* criaEstado(int qntCaracter, char *identificador){

        estado *e = (estado *)malloc(sizeof(estado));

        e->identificador = (char *)malloc(strlen(identificador)*sizeof(char));/*aloca dinamicamente o identificador*/
        strcpy(e->identificador, identificador);

        inicializa(&e->alfabeto); /*inicializa a lista do alfabeto*/
        e->estadoFinal = 0;
        return e;
}

void insereNoAlfabeto(estado *e, caracter *c){

        inserefim(&e->alfabeto, (void*)c);

}

