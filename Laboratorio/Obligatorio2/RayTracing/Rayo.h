/* 
 * File:   Rayo.h
 * Author: Germán_Ruiz
 *
 * Created on 18 de junio de 2012, 23:18
 */

#ifndef RAYO_H
#define	RAYO_H

#include "math_3d.h"

class Rayo {
public:
    Rayo();
    Rayo(float xo, float yo, float zo, float xd, float yd, float zd);
    Rayo(const Rayo& orig);
    virtual ~Rayo();
    
    inline Vector3f getOrigen() { return origen; }
    inline Vector3f getDireccion() { return direccion; }
    inline void setOrigen(Vector3f &o) { origen = o; };
    inline void setDireccion(Vector3f &d) { direccion = d; }
private:
    Vector3f origen;
    Vector3f direccion;
    
};

#endif	/* RAYO_H */

