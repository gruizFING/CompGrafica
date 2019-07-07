/* 
 * File:   Rayo.cpp
 * Author: Germán_Ruiz
 * 
 * Created on 18 de junio de 2012, 23:18
 */

#include "Rayo.h"
#include "math_3d.h"

Rayo::Rayo() {
}

Rayo::Rayo(const Rayo& orig) {
}


Rayo::Rayo(float xo, float yo, float zo, float xd, float yd, float zd)
{
    this->origen.Set(xo,yo,zo);
    this->direccion.Set(xd,yd,zd);
}

Rayo::~Rayo() {
}

