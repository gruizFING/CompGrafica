/* 
 * File:   Escena.cpp
 * Author: bdi_usr
 * 
 * Created on 19 de junio de 2012, 6:03
 */
#include "Escena.h"
#include <cstddef>
#include <iostream>
#include "pugixml.hpp"
#include "Esfera.h"
#include "Plano.h"
#include "Triangulo.h"
#include "Cilindro.h"

using namespace std;
using namespace pugi;

Escena::Escena() {
}

int Escena::Init(char* xmlIn)
{
    xml_document doc;

    xml_parse_result result = doc.load_file(xmlIn);
    
    xml_node escena = doc.child("escena");
    
    xml_node camara = escena.child("camara");
    ancho = camara.attribute("ancho").as_int();
    alto = camara.attribute("alto").as_int();
    bitmap = camara.attribute("bitmaps").as_int();
    bpp = bitmap/8;
    uX = camara.attribute("ux").as_float() / ancho; 
    uY = camara.attribute("uy").as_float() / alto;
    int antiLevel = camara.attribute("antiLevel").as_int();
    switch (antiLevel)
    {
        case(1):
        {
            antiL1 = 1;
            antiL2 = 1;
            sampleRatio = 1.0f;
            break;
        }
        case(2):
        {
            antiL1 = 1;
            antiL2 = 2;
            sampleRatio = 0.5f;
            break;
        }
        case(4):
        {
            antiL1 = 2;
            antiL2 = 2;
            sampleRatio = 0.25f;
            break;
        }
        case(8):
        {
            antiL1 = 2;
            antiL2 = 4;
            sampleRatio = 0.125f;
            break;
        }
        default:
        {
            antiL1 = 1;
            antiL2 = 2;
            sampleRatio = 0.5f;
            break;
        }   
    }
    
    maxProf = camara.attribute("prof").as_int();
    
    xml_node posicion = camara.child("posicion");
    this->posicionCamara.Set(posicion.attribute("x").as_float(), posicion.attribute("y").as_float(), posicion.attribute("z").as_float());
    
    
    xml_node materiales = escena.child("materiales");
    for (xml_node material = materiales.child("material"); material; material = material.next_sibling("material"))
    {
        xml_node difusidad = material.child("color");
        Color color(difusidad.attribute("r").as_float(), difusidad.attribute("g").as_float(), difusidad.attribute("b").as_float(), difusidad.attribute("a").as_float());
        
        Material* nuevoMaterial = new Material(color,material.attribute("power").as_float(),
                material.attribute("especular").as_float(),material.attribute("difusidad").as_float(),material.attribute("reflexion").as_float(),
                material.attribute("transmision").as_float(),material.attribute("ambiente").as_float(), material.attribute("refraccion").as_float());
        this->materiales.push_back(nuevoMaterial);
    }
    
    xml_node elementos = escena.child("elementos");
    for (xml_node esfera= elementos.child("esfera"); esfera; esfera = esfera.next_sibling("esfera"))
    {
        xml_node posicion = esfera.child("posicion");
        Objeto* nuevaEsfera = new Esfera(posicion.attribute("x").as_float(), posicion.attribute("y").as_float(), posicion.attribute("z").as_float(), esfera.attribute("radio").as_float(), esfera.attribute("material").as_int());
        this->objetos.push_back(nuevaEsfera);
    }
    for (xml_node plano = elementos.child("plano"); plano; plano = plano.next_sibling("plano"))
    {
        xml_node punto = plano.child("punto");
        xml_node normal = plano.child("normal");
        Plano* nuevoPlano = new Plano(normal.attribute("x").as_float(),normal.attribute("y").as_float(),normal.attribute("z").as_float(), 
                punto.attribute("x").as_float(), punto.attribute("y").as_float(),punto.attribute("z").as_float(), plano.attribute("material").as_int());
        this->objetos.push_back(nuevoPlano);
    }
    for (xml_node triangulo = elementos.child("triangulo"); triangulo; triangulo = triangulo.next_sibling("triangulo"))
    {
        xml_node punto1 = triangulo.child("punto");
        xml_node punto2 = punto1.next_sibling("punto");
        xml_node punto3 = punto2.next_sibling("punto");
        Vector3f p1(punto1.attribute("x").as_float(),punto1.attribute("y").as_float(),punto1.attribute("z").as_float());
        Vector3f p2(punto2.attribute("x").as_float(),punto2.attribute("y").as_float(),punto2.attribute("z").as_float());
        Vector3f p3(punto3.attribute("x").as_float(),punto3.attribute("y").as_float(),punto3.attribute("z").as_float());
        Triangulo* nuevoTria = new Triangulo(p1,p2,p3,triangulo.attribute("material").as_int());
        this->objetos.push_back(nuevoTria);
    }
    for (xml_node cilindro = elementos.child("cilindro"); cilindro; cilindro = cilindro.next_sibling("cilindro"))
    {
        xml_node centro = cilindro.child("posicion");
        xml_node direccion = cilindro.child("direccion");
        Vector3f cen(centro.attribute("x").as_float(),centro.attribute("y").as_float(),centro.attribute("z").as_float());
        Vector3f dir(direccion.attribute("x").as_float(),direccion.attribute("y").as_float(),direccion.attribute("z").as_float());
        Cilindro* nuevoCilindro = new Cilindro(cen,dir,cilindro.attribute("radio").as_float(),cilindro.attribute("altura").as_float(),cilindro.attribute("material").as_int());
        this->objetos.push_back(nuevoCilindro);
    }
    for (xml_node luz = elementos.child("luz"); luz; luz = luz.next_sibling("luz"))
    {
        xml_node posicion = luz.child("posicion");
        xml_node intensidad = luz.child("intensidad");
        Luz* nuevaLuz = new Luz(posicion.attribute("x").as_float(), posicion.attribute("y").as_float(), posicion.attribute("z").as_float(), 
                        intensidad.attribute("r").as_float(), intensidad.attribute("g").as_float(), intensidad.attribute("b").as_float());
        this->luces.push_back(nuevaLuz);
    }
    return true;
}

Escena::~Escena() {
    materiales.clear();
    objetos.clear();
    luces.clear();
}


bool Escena::intersect(Rayo* rayo, int &indObjeto, Vector3f &puntoInterseccion, Vector3f &normalInterseccion)
{
    float distancia = DIST_MAX;
    float dist;
    int origenRayo, origen;
    int cantObjetos = this->objetos.size();
    for (int i = 0; i < cantObjetos; i++)
    {
        origen = (this->objetos.at(i))->interseccion(rayo, dist);
        if ((origen == OUTSIDE || origen == INSIDE) && dist < distancia) //Se encontro un objeto mas cercano
        {
            origenRayo = origen;
            distancia = dist;
            indObjeto = i;
        }
    }
    
    if (distancia < DIST_MAX) //hay interseccion, calculo la normal y el punto de interseccion
    {
        puntoInterseccion = rayo->getOrigen() + rayo->getDireccion() * distancia;
        normalInterseccion = (this->objetos.at(indObjeto))->getNormal(puntoInterseccion, origenRayo);
        normalInterseccion.Normalize();
        return true;
    }
    return false;    
}

