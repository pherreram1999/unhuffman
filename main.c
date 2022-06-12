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

Diccionario * cargarDiccionario(char * path){
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


    Diccionario * dic = cargarDiccionario(diccionarioPath);

    //imprimirDiccionario(dic);
    return 0;
}
