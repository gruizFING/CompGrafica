/* 
 * File:   main.cpp
 * Author: bdi_usr
 *
 * Created on 19 de junio de 2012, 5:59
 */

#include <cstdlib>
#include <dir.h>
#include <time.h>
#include <iostream>
#include "Escena.h"
#include "Pantalla.h"

using namespace std;
#undef main
/*
 * 
 */
int main(int argc, char** argv) {
    
    if(argc > 3 || argc < 1)
        return -1;
    char* imagen;
    char archivo[128];
    if (argc < 3)
    {
        time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char carpeta[128];
        strftime(carpeta,128,"%d-%m-%y",tlocal);
        mkdir(carpeta);
        
        strftime(archivo,128,"%d-%m-%y/%H-%M-%S.bmp",tlocal);
        cout << archivo;
        imagen = &archivo[0];
    }
    else
        imagen = argv[2];
    
    if(argc < 2)
    {
        char config[128];
        sprintf(config,"configEscena.xml");
        argv[1] = config;
    }
    
    Escena *escena = new Escena();
    if (!escena->Init(argv[1]))
    {
        cout << "Error al inicializar escena." << endl;
        return -1;
    }
   
    Pantalla* pantalla = new Pantalla();

    if (!pantalla->Draw(escena,imagen))
    {
        cout << "Error al generar la imagen." << endl;
        return -1;
    }
    delete escena;
    return 0;
}

