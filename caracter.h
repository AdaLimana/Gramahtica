#include<stdlib.h>
#include"lista.h"

typedef struct{

	void *identificador;
	lista estadoPossivel;

}caracter;


caracter *criaCaracter(void *identificador){
        int i=0;
        caracter *c = (caracter*) malloc(sizeof(caracter));
        c->identificador = identificador;
        inicializa(&c->estadoPossivel);/*inicializa a lista de estados ppossiveis do caracter*/

        return c;/*retorna o end do caracter*/

}

void colocaEstado(caracter *c, void *novoEstado){

        inserefim(&c->estadoPossivel,  novoEstado);/*insere um possivel estado para o caracter*/

}

void liberaEstado(lista *estadoPossivel){

        int i = 0;
        while(estadoPossivel->tamanho){
                tiraInicio(estadoPossivel);

        }

}

