/* 
 * File:   Plano.h
 * Author: Germán_Ruiz
 *
 * Created on 19 de junio de 2012, 00:56
 */

#ifndef PLANO_H
#define	PLANO_H

#include "Objeto.h"

class Plano : public Objeto {
public:
    Plano();
    Plano(float a, float b, float c, float px, float py, float pz, int mat);
    Plano(const Plano& orig);
    virtual ~Plano();
    
    int interseccion(Rayo* rayo, float &distancia); //Si devuelve OUTSIDE entonces la normal, sino negada
    Vector3f getNormal(Vector3f &punto, int origen);
protected:
    Vector3f normal, normalNegada;
    float d;

};

#endif	/* PLANO_H */

