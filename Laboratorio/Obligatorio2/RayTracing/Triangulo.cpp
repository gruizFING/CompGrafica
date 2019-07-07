/* 
 * File:   Triangulo.cpp
 * Author: Germán_Ruiz
 * 
 * Created on 19 de junio de 2012, 02:08
 */

#include "Triangulo.h"
#include "math_3d.h"
#include "Plano.h"
#include "Rayo.h"
#include <stdlib.h>

Triangulo::Triangulo() {
}

Triangulo::Triangulo(const Triangulo& orig) {
}

Triangulo::Triangulo(Vector3f p1, Vector3f p2, Vector3f p3, int mat)
{
    this->punto1 = p1;
    this->punto2 = p2;
    this->punto3 = p3;
    this->calcularNormal();
    this->d = (this->punto1 * this->normal) * (-1);
    this->idMaterial = mat;
}

void Triangulo::calcularNormal()
{
    Vector3f p1 = this->punto3 - this->punto1;
    p1.Normalize();
    Vector3f p2 = this->punto2 - this->punto1;
    p2.Normalize();
    this->normal = p1.Cross(p2);
    this->normal.Normalize();
    this->normalNegada = this->normal * (-1.0);
}


Triangulo::~Triangulo() {
}


int Triangulo::interseccion(Rayo* rayo, float &distancia)
{
    int res = Plano::interseccion(rayo,distancia);
    if (res != MISS) //interseca el plano pero hay que ver si el rayo pasa por el triangulo
    {
        //Siguiendo el algoritmo propuesto en el libro
        Vector3f puntoInterseccion = rayo->getOrigen() + rayo->getDireccion() * distancia;
        //Pasar a 2D el triangulo
        float xAbs = abs((int) this->normal.x);
        float yAbs = abs((int) this->normal.y);
        float zAbs = abs((int) this->normal.z);
        Vector2f vert1, vert2, vert3, puntoInter;
        if (xAbs > yAbs && xAbs > zAbs) //domina la coordenada x
        {
            vert1.Set(this->punto1.y, this->punto1.z);
            vert2.Set(this->punto2.y, this->punto2.z);
            vert3.Set(this->punto3.y, this->punto3.z);
            puntoInter.Set(puntoInterseccion.y,puntoInterseccion.z);
        }
        else if (yAbs > xAbs && yAbs > zAbs) //domina y
        {
            vert1.Set(this->punto1.x, this->punto1.z);
            vert2.Set(this->punto2.x, this->punto2.z);
            vert3.Set(this->punto3.x, this->punto3.z);
            puntoInter.Set(puntoInterseccion.x,puntoInterseccion.z);     
        }
        else //z dominante
        {
            vert1.Set(this->punto1.x, this->punto1.y);
            vert2.Set(this->punto2.x, this->punto2.y);
            vert3.Set(this->punto3.x, this->punto3.y);
            puntoInter.Set(puntoInterseccion.x,puntoInterseccion.y);    
        }
        //Ahora translado el punto de interseccion al origen
        Vector2f vert1_1 = vert1 - puntoInter;
        Vector2f vert2_2 = vert2 - puntoInter;
        Vector2f vert3_3 = vert3 - puntoInter;
        //Numero de cruces
        int nc = 0;
        int sh = vert1_1.y < 0 ? -1 : 1;
        int shn = vert2_2.y < 0 ? -1 : 1;
        if (sh != shn)
        {
            if (vert1_1.x > 0 && vert2_2.x > 0) //la linea cruza
                nc++;
            else if (vert1_1.x > 0 || vert2_2.x > 0) //la linea puede cruzar
            {
                if ((vert1_1.x - vert1_1.y * (vert2_2.x - vert1_1.x) / (vert2_2.y - vert1_1.y)) > 0) //hay cruce
                    nc++;
            }
        }
        //Proximo lado
        sh = shn;
        shn = vert3_3.y < 0 ? -1 : 1;
        if (sh != shn)
        {
            if (vert2_2.x > 0 && vert3_3.x > 0) //la linea cruza
                nc++;
            else if (vert2_2.x > 0 || vert3_3.x > 0) //la linea puede cruzar
            {
                if ((vert2_2.x - vert2_2.y * (vert3_3.x - vert2_2.x) / (vert3_3.y - vert2_2.y)) > 0) //hay cruce
                    nc++;
            }
        }
        //Proximo lado
        sh = shn;
        shn = vert1_1.y < 0 ? -1 : 1;
        if (sh != shn)
        {
            if (vert3_3.x > 0 && vert1_1.x > 0) //la linea cruza
                nc++;
            else if (vert3_3.x > 0 || vert1_1.x > 0) //la linea puede cruzar
            {
                if ((vert3_3.x - vert3_3.y * (vert1_1.x - vert3_3.x) / (vert1_1.y - vert3_3.y)) > 0) //hay cruce
                    nc++;
            }
        }
        if (nc % 2 == 0) //par, entonces el rayo no pasa por el triangulo
            res = MISS;
        //Si es impar queda como estaba, es un hit y es de afuera o de adentro al plano
    }
    if (fabs(distancia) < LIMITE)
        res = MISS;
    return res;
}

