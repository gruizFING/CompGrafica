/* 
 * File:   Objeto.h
 * Author: Germán_Ruiz
 *
 * Created on 19 de junio de 2012, 02:25
 */

#ifndef OBJETO_H
#define	OBJETO_H

#include "Rayo.h"
#include "math_3d.h"

#define OUTSIDE 1
#define INSIDE  0
#define MISS   -1
#define LIMITE 0.0001f

class Objeto {
public:
    Objeto();
    Objeto(const Objeto& orig);
    virtual ~Objeto();
    
    virtual int interseccion(Rayo* rayo, float &distancia) = 0;
    virtual Vector3f getNormal(Vector3f &punto, int origen) = 0;
    inline int getIdMaterial() { return idMaterial; }
protected:
    int idMaterial;
    
};

#endif	/* OBJETO_H */

