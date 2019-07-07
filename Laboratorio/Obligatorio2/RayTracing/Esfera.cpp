/* 
 * File:   Esfera.cpp
 * Author: Germán_Ruiz
 * 
 * Created on 18 de junio de 2012, 21:45
 */

#include <basetyps.h>

#include "Esfera.h"
#include "math_3d.h"
#include "Rayo.h"
#include "math.h"

Esfera::Esfera() {
}

Esfera::Esfera(const Esfera& orig) {
}


Esfera::Esfera(float x, float y, float z, float radio, int mat)
{
    this->centro.Set(x,y,z);
    this->radio = radio;
    this->radioCuadrado = radio * radio;
    this->radioInverso = 1 / radio;
    this->idMaterial = mat;
}

Esfera::~Esfera() {
}


int Esfera::interseccion(Rayo* rayo, float &distancia)
{
    Vector3f oc = this->centro - rayo->getOrigen();
    float l2oc = oc * oc;
    int res = MISS;
    if (l2oc < this->radioCuadrado) // El origen del rayo se encuentra dentro de la esfera, hay hit
        res =  INSIDE;
    float tca = oc * rayo->getDireccion();
    if (tca < 0 && res == MISS) // Si el origen del rayo se encuentra affuera de la esfera y tca < 0 no va a ver hit
        return res;
    else
    {
        float t2hc = this->radioCuadrado - l2oc + tca * tca;
        if (t2hc < 0 && res == MISS) // No hay hit
            return res;
        else
        {
            if (res == MISS) // El rayo se origina afuera y hay hit
            {
                distancia = tca - sqrt(t2hc);
                res = OUTSIDE;
            }
            else // El rayo adentro de la esfera
                distancia = tca + sqrt(t2hc);
        }
    }
    if (fabs(distancia) < LIMITE) //Por problemas de precision, el rayo se origina desde la superficie de la esfera
        res = MISS;
    return res;
}


Vector3f Esfera::getNormal(Vector3f &punto, int origen)
{
    Vector3f norm;
    if (origen == OUTSIDE)
        norm.Set((punto.x - this->centro.x) * this->radioInverso,
                 (punto.y - this->centro.y) * this->radioInverso,
                 (punto.z - this->centro.z) * this->radioInverso);
    else //INSIDE
        norm.Set(-(punto.x - this->centro.x) * this->radioInverso,
                 -(punto.y - this->centro.y) * this->radioInverso,
                 -(punto.z - this->centro.z) * this->radioInverso);
    return norm;
}
