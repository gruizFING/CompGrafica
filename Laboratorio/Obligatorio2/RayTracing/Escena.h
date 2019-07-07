/* 
 * File:   Escena.h
 * Author: bdi_usr
 *
 * Created on 19 de junio de 2012, 6:03
 */

#ifndef ESCENA_H
#define	ESCENA_H
#include "Material.h"
#include "Objeto.h"
#include "Luz.h"
#include <vector>

#define DIST_MAX 10000000

class Escena {
public:
    
    int ancho, alto, bitmap, bpp, antiL1, antiL2, maxProf; 
    Vector3f posicionCamara;
    float uX, uY, sampleRatio;
    std::vector<Material*> materiales;
    std::vector<Objeto*>   objetos;
    std::vector<Luz*>      luces;
    
    Escena();
    
    int Init(char* xmlIn);
    
    virtual ~Escena();
    
    bool intersect(Rayo* rayo, int &indObjeto, Vector3f &puntoInterseccion, Vector3f &normalInterseccion);
    
private:
    
};

#endif	/* ESCENA_H */

