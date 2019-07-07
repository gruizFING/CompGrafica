/* 
 * File:   Cilindro.h
 * Author: Germán_Ruiz
 *
 * Created on 19 de junio de 2012, 20:50
 */

#ifndef CILINDRO_H
#define	CILINDRO_H

#include "Objeto.h"

class Cilindro : public Objeto {
public:
    Cilindro();
    Cilindro(Vector3f centro, Vector3f direccion, float r, float h, int mat);
    Cilindro(const Cilindro& orig);
    virtual ~Cilindro();
    
    int interseccion(Rayo* rayo, float &distancia);
    Vector3f getNormal(Vector3f &punto, int origen);
private:
    Vector3f centro;
    Vector3f direccion;
    float altura;
    float radio, radioCuadrado;

};

#endif	/* CILINDRO_H */

