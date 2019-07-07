/* 
 * File:   Pantalla.h
 * Author: bdi_usr
 *
 * Created on 19 de junio de 2012, 6:14
 */

#ifndef PANTALLA_H
#define	PANTALLA_H

#include "Escena.h"
#include "Freeimage.h"
#include "Color.h"
#include <cmath>
#include <algorithm>


class Pantalla {
public:
    Pantalla();
    
    int Draw(Escena* escena, char* imagenOut);
    
    virtual ~Pantalla();
    
private:

    FIBITMAP* PicOpen(int ancho,int alto, int bitmap);
    FIBITMAP** PicsAuxOpen(int ancho,int alto, int bitmap);
    BYTE* PicGetScanLine(FIBITMAP* pic, int y);
    void PicsAuxGetScanLine(BYTE** picsAuxFila, FIBITMAP** picsAux, int y);
    int PicClose(FIBITMAP* pic, char* picFile); 
    void PicsAuxClose(FIBITMAP** picsAux,char* imagenOut);
    void SetPixelsPicsAux(BYTE** picsAuxFila,Color* coloresAux, int bpp);
    
    
    void Trace(Color &color, Rayo* rayo, Escena* escena, int prof, float coef,float indiceRefraccionInc, Color* coloresAuxiliares,bool start);
    bool Intersect(Rayo* rayo, Escena* escena, int &indObjeto, Vector3f &puntoInterseccion, Vector3f &normalInterseccion);
    void Shade(Color &color, Rayo* rayo, Escena* escena, int &indObjeto, Vector3f &puntoInterseccion, Vector3f &normalInterseccion, int prof, float coef, float indiceRefraccionInc, Color* coloresAuxiliares,bool start);
    bool inShade(Rayo* rayo, Escena* escena, float &distancia);
    void ShadeBackground(Color &color);
    
    float srgbEncode(float c);

};

#endif	/* PANTALLA_H */

