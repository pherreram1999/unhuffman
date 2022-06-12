#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <string.h>

typedef struct _diccionario {
    char * codigo;
    int ascii;
    char simbolo;
    int longitd;
    struct _diccionario * sig;
} Diccionario;

void imprimirDiccionario(Diccionario * nav){
    if(nav == NULL){
        printf("diccionario vacio :|\n");
        return;
    }

    printf("Diccionario: \n");
    printf("ASCII \t|\t CODE \t|\t LEN \n");
    printf("--------+---------\n");
    while(nav != NULL){
        printf(" %d \t|\t %s =>\t %d \n",nav->ascii,nav->codigo,nav->longitd);
        nav = nav->sig;
    }
}


Diccionario * crearNodo(int assci, char * codigo){
    Diccionario * dic = (Diccionario *) malloc(sizeof(Diccionario));
    dic->simbolo = (char) assci;
    dic->ascii = assci;
    dic->codigo = codigo;
    dic->longitd = 0;
    dic->sig  = NULL;
    return dic;
}

void insertarNodo(Diccionario ** dic,Diccionario * nuevoNodo){
    if(nuevoNodo == NULL){
        return;
    }

    if(*dic == NULL){
        *dic = nuevoNodo;
        return;
    }

    Diccionario * nav = * dic;
    while(nav->sig != NULL){
        nav = nav->sig;
    }

    nav->sig = nuevoNodo;
}

Diccionario * cargarDiccionario(char * path,int * maxlen,int * noElementos){
    FILE * dicFile = fopen(path,"r");

    if(dicFile == NULL){
        printf("No fue posible leeer el diccionario :\\ \n");
        exit(1);
    }

    char * line = NULL;
    size_t len = 0;
    char ** parts = NULL;

    int ascii = 0;
    char * codigo = NULL;
    Diccionario * nuevoNodo = NULL;


    Diccionario * dic = NULL;
    while(getline(&line,&len,dicFile) != -1){
        parts = strsplit(line,"=");
        ascii = atoi(parts[0]);
        parts = strsplit(parts[1],"\n");
        codigo = parts[0];
        nuevoNodo = crearNodo(ascii,codigo);
        nuevoNodo->longitd = strlen(codigo);
        if(nuevoNodo->longitd > *maxlen){
            *maxlen = nuevoNodo->longitd;
        }
        insertarNodo(&dic,nuevoNodo);
        (*noElementos)++;
    }
    fclose(dicFile);
    return dic;
}

int main(int argc,char ** args) {
    if(argc != 4){
        printf("Favor de proporcionar el nombre del comprimido y su diccionario\n");
        exit(1);
    }
    char * filepath = args[1];
    char * pathdestino = args[2];
    char * diccionarioPath = args[3];

    int maxlen = 0;
    int noElementos = 0;
    Diccionario * dic = cargarDiccionario(diccionarioPath,&maxlen,&noElementos);

    FILE * compremido = fopen(filepath,"r");
    int i = 0;


    FILE * destino = fopen(pathdestino,"w");
    fprintf(destino,"");
    fclose(destino);
    destino = fopen(pathdestino,"a");

    if(destino == NULL){
        printf("Destino no abierto :(");
        exit(1);
    }

    //imprimirDiccionario(dic);
    Diccionario * nav = NULL;
    nav = dic;
    char * codigoTmp;
    int lastPoint = 0;
    unsigned long compremidoMaxLen = 0;
    fseek(compremido,0,SEEK_END);
    // se le resta un uno para saber por el EOF
    compremidoMaxLen = ftell(compremido) - 1;
    int count;
   // imprimirDiccionario(dic);
    rewind(compremido);
    while(lastPoint < compremidoMaxLen){
        count = 0;
        while(nav != NULL){
            codigoTmp = NULL;
            /**
             * Recibe el puntero del archivo
             * el segundo es el offset, es decir cuanto se va mover
             * el tercero el desde donde los va leer, inicio, donde este, al final
             */
            fseek(compremido,lastPoint,SEEK_SET);
            // generamos la palabra y volvemos el puntero a su posicion oiringal
            codigoTmp = (char *) malloc(nav->longitd * sizeof(char));
            for(i = 0; i < nav->longitd; i++){
                codigoTmp[i] = (char) fgetc(compremido);
            }
            codigoTmp[i] = '\0'; // para delimitar la cadena en caso de memoria residual
            fseek(compremido,lastPoint,SEEK_SET);
            if(strcmp(codigoTmp,nav->codigo) == 0){
                fprintf(destino,"%c",nav->simbolo);
                // movemos el puntero en el siguiente posicion
                lastPoint += nav->longitd;
                break;
            }
            count++;
            nav = nav->sig;
        }
        if(noElementos == count){
            printf("\n NO fue posible encontrar una concidencia | cursor: %ld :(\n", ftell(compremido));
            exit(1);
        }
        nav = dic;
    }

    fclose(destino);

    printf("\nDescompresion terminada :D\n");
    printf("¯\\_(ツ)_/¯\n");

    return 0;
}
