/* 
 * File:   Luz.h
 * Author: bdi_usr
 *
 * Created on 20 de junio de 2012, 7:23
 */

#ifndef LUZ_H
#define	LUZ_H

#include "math_3d.h"
#include "Color.h"

class Luz {
public:
    Luz(float x, float y, float z, float intr, float intg, float intb)
    {
        posicion.Set(x,y,z);
        intensidad = Color(intr,intg,intb,1.0f);
    }
    virtual ~Luz(){}
    
    inline Vector3f& Posicion() { return posicion; }
    inline Color& Intensidad()  { return intensidad; }
private:
    Vector3f posicion;
    Color    intensidad;
};

#endif	/* LUZ_H */

