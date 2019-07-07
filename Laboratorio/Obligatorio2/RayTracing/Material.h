/* 
 * File:   Material.h
 * Author: bdi_usr
 *
 * Created on 20 de junio de 2012, 7:36
 */

#ifndef MATERIAL_H
#define	MATERIAL_H

#include "Color.h"

class Material {
public:
    Material(Color& col,float pow, float Ks,float Kd, float Kr, float Kt,float Ka, float iR):
    color(col), power(pow), kReflexion(Kr),
    kSpecular(Ks),kTransmision(Kt),kDifusidad(Kd),kAmbiente(Ka),indiceRefraccion(iR){}
    virtual ~Material(){}

    inline float Reflexion()  { return kReflexion; }
    inline float Specular()   { return kSpecular;     }
    inline float Diffuse()    { return kDifusidad;   }
    inline float Tranmission() { return kTransmision;  }
    inline float Ambiente()   { return kAmbiente; }
    inline float Refraccion() { return indiceRefraccion; }
    inline float Power()      { return power;        }
    inline Color& gColor()     { return color;     }
    
private:
    float kReflexion, kSpecular, kDifusidad, kTransmision, kAmbiente, indiceRefraccion, power;
    Color color;
};

#endif	/* MATERIAL_H */

