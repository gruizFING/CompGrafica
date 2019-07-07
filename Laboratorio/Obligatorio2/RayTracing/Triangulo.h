/* 
 * File:   Triangulo.h
 * Author: Germán_Ruiz
 *
 * Created on 19 de junio de 2012, 02:08
 */

#ifndef TRIANGULO_H
#define	TRIANGULO_H

#include "Plano.h"

class Triangulo : public Plano {
public:
    Triangulo();
    Triangulo(Vector3f p1, Vector3f p2, Vector3f p3, int mat);
    Triangulo(const Triangulo& orig);
    virtual ~Triangulo();
    
    int interseccion(Rayo* rayo, float &distancia);
    //Vector3f getNormal(Vector3f &punto, int origen); Es la de Plano
    
private:
    Vector3f punto1;
    Vector3f punto2;
    Vector3f punto3;
    
    void calcularNormal();
    
};

#endif	/* TRIANGULO_H */

