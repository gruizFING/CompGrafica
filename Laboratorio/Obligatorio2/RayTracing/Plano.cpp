/* 
 * File:   Plano.cpp
 * Author: Germán_Ruiz
 * 
 * Created on 19 de junio de 2012, 00:56
 */

#include "Plano.h"
#include "math_3d.h"
#include "Rayo.h"

Plano::Plano() {
}

Plano::Plano(float a, float b, float c, float px, float py, float pz, int mat)
{
    this->normal.Set(a,b,c);
    this->normal.Normalize();
    this->normalNegada = normal * (-1);
    this->d = -normal.x*px - normal.y*py - normal.z*pz;
    this->idMaterial = mat;
}

Plano::Plano(const Plano& orig) {
}

Plano::~Plano() {
}


int Plano::interseccion(Rayo* rayo, float &distancia)
{
    int res = MISS;
    float vd = this->normal * rayo->getDireccion();
    if (vd == 0) //Rayo paralelo al plano, no hay hit
        return res;
    else
    {
        float vo = -(this->normal * rayo->getOrigen() + this->d);
        distancia = vo / vd;
        if (distancia < 0) // La linea definida por el rayo intersecta el plano detras del origen del rayo, no hay hit
            return res;
        if (vd < 0)
            res = OUTSIDE;
        else
            res = INSIDE;
    }
    if (fabs(distancia) < LIMITE)
        res = MISS;
    return res;
}

Vector3f Plano::getNormal(Vector3f &punto, int origen)
{
    if (origen == OUTSIDE)
        return this->normal;
    else // INSIDE
        return this->normalNegada;
}

