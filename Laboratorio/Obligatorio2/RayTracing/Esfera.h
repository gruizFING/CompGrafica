/* 
 * File:   Esfera.h
 * Author: Germán_Ruiz
 *
 * Created on 18 de junio de 2012, 21:45
 */

#ifndef ESFERA_H
#define	ESFERA_H

#include "Objeto.h"

class Esfera : public Objeto {
public:
    Esfera();
    Esfera(float x, float y, float z, float radio,int mat);
    Esfera(const Esfera& orig);
    virtual ~Esfera();
    
    int interseccion(Rayo* rayo, float &distancia);
    Vector3f getNormal(Vector3f& punto, int origen); //Si el rayo es originado de adentro hay q negar la normal
    
protected:
    Vector3f centro;
    float radio;
    float radioCuadrado;
    float radioInverso;

};

#endif	/* ESFERA_H */

