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

Diccionario * cargarDiccionario(char * path,int * maxlen){
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
    }
    fclose(dicFile);
    return dic;
}

int main(int argc,char ** args) {
    if(argc != 3){
        printf("Favor de proporcionar el nombre del comprimido y su diccionario\n");
        exit(1);
    }
    char * filepath = args[1];
    char * diccionarioPath = args[2];


    int maxlen = 0;
    Diccionario * dic = cargarDiccionario(diccionarioPath,&maxlen);

    FILE * compremido = fopen(filepath,"r");
    int seek = 0;
    int i = 0;


    char * pathdestino = "descompresion.txt";
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

    char codigoTmp[100] = "";
    int lastPoint = 0;
    unsigned long readTimes = 0;
    unsigned long size = 0;

    unsigned long compremidoMaxLen = 0;
    fseek(compremido,0,SEEK_END);
    compremidoMaxLen = ftell(compremido);

    rewind(compremido);
    while(lastPoint <= compremidoMaxLen){
        while(nav->sig != NULL){
            /**
             * Recibe el puntero del archivo
             * el segundo es el offset, es decir cuanto se va mover
             * el tercero el desde donde los va leer, inicio, donde este, al final
             */
            fseek(compremido,lastPoint,SEEK_SET);
            size = nav->longitd * sizeof(char);
            readTimes = fread(codigoTmp,size,1,compremido);
            fseek(compremido,lastPoint,SEEK_SET);
            printf("codigo temp: %s | temp len: %d | nav codigo: %s | len: %d  | puntero: %ld | readed: %ld\n",codigoTmp,strlen(codigoTmp),nav->codigo,nav->longitd,ftell(compremido),readTimes);
            if(strcmp(codigoTmp,nav->codigo) == 0){
                printf("Iguales => %s :%s\n",codigoTmp,nav->codigo);
                lastPoint += nav->longitd;
                break;
            }

            nav = nav->sig;
        }
        nav = dic;
    }

    return 0;
}
