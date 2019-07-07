/* 
 * File:   Cilindro.cpp
 * Author: Germán_Ruiz
 * 
 * Created on 19 de junio de 2012, 20:51
 */

#include "Cilindro.h"
#include "Rayo.h"
#include "math_3d.h"

Cilindro::Cilindro() {
}

Cilindro::Cilindro(Vector3f centro, Vector3f direccion, float r, float h, int mat) {
    this->centro = centro;
    this->direccion = direccion; // Siempre vertical
    this->altura = h;
    this->radio = r;
    this->radioCuadrado = r*r;
    this->idMaterial = mat;
}

Cilindro::Cilindro(const Cilindro& orig) {
}

Cilindro::~Cilindro() {
}

int Cilindro::interseccion(Rayo* rayo, float &root) 
{
    root = 20000;
    Vector3f Rd = rayo->getDireccion();
    Vector3f Ro = centro - rayo->getOrigen();
    Vector3f pnt_intrsct;
    Vector3f pvt_top = centro;
    pvt_top.y+= altura/2;
    Vector3f pvt_bottom = centro;
    pvt_bottom.y-= altura/2;
    float a = Rd.x * Rd.x + Rd.z * Rd.z;
    float b = Ro.x * Rd.x + Ro.z * Rd.z;
    float c = Ro.x * Ro.x + Ro.z * Ro.z - radioCuadrado;
    float disc = b * b - a*c;
    float d, root1, root2;
    int return_value = MISS;
    // If the discriminant is less than 0, then we totally miss the cylinder.
    if (disc > 0.0) {
        d = sqrt(disc);
        root2 = (b + d) / a;
        root1 = (b - d) / a;
        // If root2 < 0, then root1 is also < 0, so they are both misses.
        if (root2 > 0 && root1 > 0) {
            root = fmin(root1,root1);
            return_value = OUTSIDE;
        }
        else if(root1 > 0)
        {
            root = root1;
            return_value = INSIDE;
        }
        else if(root2 > 0)
        {
            root = root2;
            return_value = INSIDE;
        }

        pnt_intrsct = rayo->getOrigen() + Rd*root;
        // Limit the y values
        if ((pnt_intrsct.y > pvt_top.y + LIMITE) || (pnt_intrsct.y < pvt_bottom.y - LIMITE)) {
            pnt_intrsct = rayo->getOrigen() + Rd*root2;
            // Are we too high in our first hit, but hit the back wall later
            if ((pnt_intrsct.y > pvt_top.y + LIMITE) || (pnt_intrsct.y < pvt_bottom.y - LIMITE)) {
                return_value = MISS;
            }
        }
        // Check to see if it hits the end planes
        // We know the normal of the end planes is y_min=y_max=(0,1,0)
        // Therfore the dot product is simply the y portion of the
        // We know the point Q = y_min or y_max
        // If the direction of the Ray is not parallel to the top or bottom...
        
        Ro = rayo->getOrigen();
        if (Rd.y != 0.) {
            // check top cap
            root1 = -(Ro.y - pvt_top.y) / Rd.y;
            if (root1 > 0.) {
                pnt_intrsct = rayo->getOrigen() + Rd * root1 - centro;
                if (pnt_intrsct.x * pnt_intrsct.x + pnt_intrsct.z * pnt_intrsct.z <= radioCuadrado + LIMITE) {
                    if(root1 < root)
                        root = root1;
                        return_value = OUTSIDE;
                }
            }
            // check bottom cap
            root2 = -(Ro.y - pvt_bottom.y) / Rd.y;
            if (root2 > 0.) {
                pnt_intrsct = rayo->getOrigen() + Rd * root2 - centro;
                if (pnt_intrsct.x * pnt_intrsct.x + pnt_intrsct.z * pnt_intrsct.z <= radioCuadrado + LIMITE) {
                    if ((root2<root1)&&(root2<root)) {
                        root = root2;
                        return_value = OUTSIDE;
                    }
                }
            }
        } // end if Rd.y
        
    }
    
    return return_value;











    /*
    int res = MISS;
    //Primero traslado el rayo a las coordenadas del cilindro
    Vector3f ori = rayo->getOrigen() - this->centro;
    Vector3f dir = rayo->getDireccion() - this->centro;
    dir.Normalize();
    
    float a = powf(dir.x,2) + powf(dir.z,2); 
    float b = 2 * (ori.x * dir.x + ori.z * dir.z);
    float c = powf(ori.x,2) + powf(ori.z,2) - this->radioCuadrado;
    float zmin = centro.y - altura/2;
    float zmax = centro.y + altura/2;
    float det = powf(b,2) - 4 * a * c;
    if (det > 0)
    {
        float t1 = (-b - sqrt(det)) / 2*a;
        float t2 = (-b + sqrt(det)) / 2*a;
        float z1 = ori.y + t1*dir.y;
        float z2 = ori.y + t2*dir.y;
        float aux;
        if (t1 > 0 || t2 > 0) //Posible interseccion
        {
            if (t1 > 0 && t2 > 0 && (z1 > zmin && z1 < zmax) && (z2 >zmin && z2 < zmax))
            {
                res = OUTSIDE;
                distancia = t1 < t2 ? t1 : t2;
                aux = t1 < t2 ? t2 : t1;
            }
            else if((z1 > zmin && z1 < zmax) && t1 > 0)
            {
                res = INSIDE;
                distancia = t1;
            }
            else if((z2 > zmin && z2 < zmax) && t2 > 0)
            {
                res = INSIDE;
                distancia = t2;
            }
            /*
            Vector3f puntoInterseccion = rayo->getOrigen() + rayo->getDireccion() * distancia;
            if (res == INSIDE)
            {
                if (puntoInterseccion.y + LIMITE > this->altura || puntoInterseccion.y <= 0)
                {
                  if (rayo->getDireccion().y > 0)
                  {
                      float t = (this->altura - rayo->getOrigen().y) / rayo->getDireccion().y;
                      if (t > 0)
                      {
                        Vector3f corteArriba = rayo->getOrigen() + rayo->getDireccion() * t;
                        float dist =  sqrtf(powf(this->centro.x - corteArriba.x,2) + powf(this->altura - corteArriba.y,2) + powf(this->centro.z - corteArriba.z,2));
                        if (dist <= this->radio)
                        {
                            distancia = t;
                        }
                        else
                            res = MISS;
                      }
                      else
                          res = MISS;
                  }
                  ///////////////
                }
            }
            else
            {
                if (puntoInterseccion.y + LIMITE > this->altura || puntoInterseccion.y <= 0)
                {
                    Vector3f puntoInterseccion2 = rayo->getOrigen() + rayo->getDireccion() * aux;
                    if (!(puntoInterseccion2.y + LIMITE > this->altura || puntoInterseccion2.y <= 0))
                    {
                        if (rayo->getDireccion().y > 0)
                        {
                            float t = (this->altura - rayo->getOrigen().y) / rayo->getDireccion().y;
                            if (t > 0)
                            {
                                Vector3f corteArriba = rayo->getOrigen() + rayo->getDireccion() * t;
                                float dist =  sqrtf(powf(this->centro.x - corteArriba.x,2) + powf(this->altura - corteArriba.y,2) + powf(this->centro.z - corteArriba.z,2));
                                if (dist <= this->radio)
                                {
                                    distancia = t;
                                }
                                else
                                    res = MISS;
                            }
                            else
                                res = MISS;
                        }
                        ///  
                    }                    
                }                 
            }
     * 
        } 
    }
    if (fabs(distancia) < LIMITE)
        res = MISS;
    return res;
     * */
}

Vector3f Cilindro::getNormal(Vector3f &punto, int origen) {
    Vector3f res;
    float zmin = centro.y - (altura / 2.0f);
    float zmax = centro.y + (altura / 2.0f);
    if (punto.y <= zmax + 0.0001 && punto.y >= zmax - 0.0001) { //Interseccion con la tapa de arriba
        if (origen == OUTSIDE)
            res.Set(0, 1, 0);
        else
            res.Set(0, -1, 0);
    } else if (punto.y <= zmin + 0.0001 && punto.y >= zmin - 0.0001) { //Interseccion con la tapa de abajo
        if (origen == OUTSIDE)
            res.Set(0, -1, 0);
        else
            res.Set(0, 1, 0);
    } else {
        if (origen == OUTSIDE)
            res.Set((punto.x + this->centro.x)/radio, 0, (punto.z + this->centro.z)/radio);
        else
            res.Set(-(punto.x + this->centro.x)/radio, 0, -(punto.z + this->centro.z)/radio);
    }
    return res.Normalize();
}

