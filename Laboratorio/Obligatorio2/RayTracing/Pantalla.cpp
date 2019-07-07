/* 
 * File:   Pantalla.cpp
 * Author: bdi_usr
 * 
 * Created on 19 de junio de 2012, 6:14
 */

#include "Pantalla.h"
#include "string.h"

Pantalla::Pantalla() {
}

FIBITMAP* Pantalla::PicOpen(int ancho,int alto, int bitmap)
{
    return FreeImage_Allocate(ancho, alto, bitmap);
}

BYTE* Pantalla::PicGetScanLine(FIBITMAP* pic, int y)
{
    return FreeImage_GetScanLine(pic, y);
}

int Pantalla::PicClose(FIBITMAP* pic, char* picFile)
{
    if (!FreeImage_Save(FIF_BMP, pic, picFile, BMP_DEFAULT)) {
        return false;
    }
    return true;
}

FIBITMAP** Pantalla::PicsAuxOpen(int ancho,int alto, int bitmap)
{
    FIBITMAP** pics = new FIBITMAP*[5];
    for(int i = 0; i < 5; i++)
        pics[i] = FreeImage_Allocate(ancho, alto, bitmap);
    return pics;
}

 void Pantalla::PicsAuxGetScanLine(BYTE** picsAuxFila, FIBITMAP** picsAux, int y)
{
    for(int i = 0; i < 5; i++)
        picsAuxFila[i] = FreeImage_GetScanLine(picsAux[i], y);
}

void Pantalla::SetPixelsPicsAux(BYTE** picsAuxFila,Color* coloresAux, int bpp)
{
    for(int i = 0; i < 5; i++){
        picsAuxFila[i][FI_RGBA_RED] =   (unsigned char)(fmin(coloresAux[i].Rojo()*255,255));
        picsAuxFila[i][FI_RGBA_GREEN] = (unsigned char)(fmin(coloresAux[i].Verde()*255,255));
        picsAuxFila[i][FI_RGBA_BLUE] =  (unsigned char)(fmin(coloresAux[i].Azul()*255, 255));
        //picFila[FI_RGBA_ALPHA] = (unsigned char)(fmin(color.Alpha()*255,255));
        picsAuxFila[i]+=bpp;
    }
}

void Pantalla::PicsAuxClose(FIBITMAP** picsAux,char* imagenOut)
{
    int largo = 0;
    while (imagenOut[largo++]!='\0');
    largo--;
    
    char imagen[largo-4+1]; 
    strncpy(imagen, imagenOut, sizeof(imagen)-1);
    imagen[sizeof(imagen)-1] = '\0';
    
    char ambiente[largo + 10];
    strncpy(ambiente, imagen, sizeof(imagen)-1);
    strcat (ambiente,"-ambiente.bmp\0");
    char difusidad[largo + 7];
    strncpy(difusidad, imagen, sizeof(imagen)-1);
    strcat (difusidad,"-difuso.bmp\0");
    char especular[largo + 10];
    strncpy(especular, imagen, sizeof(imagen)-1);
    strcat (especular,"-especular.bmp\0");
    char reflexion[largo + 10];
    strncpy(reflexion, imagen, sizeof(imagen)-1);
    strcat (reflexion,"-reflexion.bmp\0");
    char transmision[largo + 12];
    strncpy(transmision, imagen, sizeof(imagen)-1);
    strcat (transmision,"-transmision.bmp\0");
    FreeImage_Save(FIF_BMP, picsAux[0], ambiente,    BMP_DEFAULT);
    FreeImage_Save(FIF_BMP, picsAux[1], difusidad,   BMP_DEFAULT);
    FreeImage_Save(FIF_BMP, picsAux[2], especular,   BMP_DEFAULT);
    FreeImage_Save(FIF_BMP, picsAux[3], reflexion,   BMP_DEFAULT);
    FreeImage_Save(FIF_BMP, picsAux[4], transmision, BMP_DEFAULT);
}

int Pantalla::Draw(Escena* escena, char* imagenOut)
{
    FIBITMAP* pic = PicOpen(escena->ancho,escena->alto,escena->bitmap);
    FIBITMAP** picsAux = PicsAuxOpen(escena->ancho,escena->alto,escena->bitmap);
    BYTE *picFila;
    BYTE *picsAuxFila[5];
    float xRay;
    float yRay = - (escena->uY * escena->alto) / 2;
    Color color;
    Color temp;
    Color coloresAux[5]; int ca;
    int aL1 = escena->antiL1;
    int aL2 = escena->antiL2;
    float sampleRatio = escena->sampleRatio;
    for (int y = 0; y < escena->alto;  ++y)  
    {   
        picFila = PicGetScanLine(pic, y);
        PicsAuxGetScanLine(picsAuxFila, picsAux,y);
        xRay = - (escena->uX * escena->ancho) /2;
        for (int x = 0; x < escena->ancho; ++x)
        {
            color.Set(0,0,0,1);
            temp.Set(0,0,0,1);
            for(ca = 0; ca < 5; ca++)
                coloresAux[ca].Set(0,0,0,1);
            for (float antiX = 0 ; antiX < aL1; antiX++)
            {
                for (float antiY = 0; antiY < aL2; antiY++)
                {
                    Vector3f rayDir(xRay + (antiX*escena->uX)/aL1,yRay + (antiY*escena->uY)/aL2,0);
                    rayDir -= escena->posicionCamara;
                    rayDir.Normalize();
                    Rayo* rayo = new Rayo();
                    rayo->setOrigen(escena->posicionCamara);
                    rayo->setDireccion(rayDir);
                    Trace(temp, rayo, escena, 1, 1.0f, 1.0f, coloresAux, true);
                                  
                    // pseudo photo exposure
                    float exposure = -1.00f; // random exposure value. TODO : determine a good value automatically
                    temp.Azul(1.0f - expf(temp.Azul() * exposure));
                    temp.Rojo(1.0f - expf(temp.Rojo() * exposure));
                    temp.Verde(1.0f - expf(temp.Verde() * exposure));
                    color += (sampleRatio * temp);
                    delete rayo;
                }
            }
            xRay += escena->uX;
            // gamma correction
            color.Rojo(srgbEncode(color.Rojo()));
            color.Verde(srgbEncode(color.Verde()));
            color.Azul(srgbEncode(color.Azul()));
                       
            picFila[FI_RGBA_RED] =   (unsigned char)(fmin(color.Rojo()*255,255));
            picFila[FI_RGBA_GREEN] = (unsigned char)(fmin(color.Verde()*255,255));
            picFila[FI_RGBA_BLUE] =  (unsigned char)(fmin(color.Azul()*255, 255));
            //picFila[FI_RGBA_ALPHA] = (unsigned char)(fmin(color.Alpha()*255,255));
            picFila+=escena->bpp;
            SetPixelsPicsAux(picsAuxFila,coloresAux,escena->bpp);
        }
        yRay += escena->uY;
    }
    PicsAuxClose(picsAux,imagenOut);
    return PicClose(pic,imagenOut);
}

void Pantalla::Trace(Color &color, Rayo* rayo, Escena* escena, int profundidad, float fuerza, float indRefraccion, Color* coloresAuxiliares, bool start)
{
    Vector3f puntoInterseccion, normalInterseccion;
    int idObjetoInter;
 
    if(Intersect(rayo, escena, idObjetoInter, puntoInterseccion, normalInterseccion))
    {
        Shade(color, rayo, escena, idObjetoInter, puntoInterseccion, normalInterseccion, profundidad, fuerza,indRefraccion, coloresAuxiliares, start);
    }
    else
        return ShadeBackground(color);
}

bool Pantalla::Intersect(Rayo* rayo, Escena* escena, int &indObjeto, Vector3f &puntoInterseccion, Vector3f &normalInterseccion)
{
    if (escena->intersect(rayo,indObjeto,puntoInterseccion,normalInterseccion))
    {
        return true; 
    }
    return false;
}

void Pantalla::Shade(Color& color, Rayo* rayo, Escena* escena, int &indObjeto, Vector3f &puntoInterseccion, Vector3f &normalInterseccion, int prof, float fuerza, float indRefraccion, Color* coloresAux, bool start)
{
    Material* material = escena->materiales.at((escena->objetos.at(indObjeto))->getIdMaterial());
  
    int cantLuces = escena->luces.size();
    for (int l = 0; l < cantLuces; l++)
    {
        Luz* luz = escena->luces.at(l);
        if (l == 0 && material->Ambiente() > 0.0f) //La primer luz se toma como ambiente
        {
            color +=  luz->Intensidad() * material->gColor() * fuerza * material->Ambiente();
            if(start)
                coloresAux[0] = luz->Intensidad() * material->gColor() * material->Ambiente();;
        }
        
        Rayo* rayoLuz = new Rayo();       
        Vector3f temp = luz->Posicion() - puntoInterseccion;
        float distLuz = sqrtf(temp * temp);
        rayoLuz->setDireccion(temp.Normalize());
        
        float proyeccionLuz = rayoLuz->getDireccion() * normalInterseccion;
        
        if (proyeccionLuz > 0.0f)
        {
            Vector3f origen = puntoInterseccion + rayoLuz->getDireccion() * LIMITE;
            rayoLuz->setOrigen(origen);
            bool sombra = inShade(rayoLuz, escena, distLuz);
            
            if (!sombra)
            {
                //Difusidad
                if (material->Diffuse() > 0.0f)
                {   
                    float lambert = (rayoLuz->getDireccion() * normalInterseccion) * fuerza;
                    if (lambert > 0)
                    {
                        if(start){
                            coloresAux[1] = luz->Intensidad() * material->gColor() * lambert * material->Diffuse();
                            color +=  coloresAux[1];
                        }
                        else
                            color += luz->Intensidad() * material->gColor() * lambert * material->Diffuse();
                    }
                }
                
                //Blinn-Phong
                if (material->Specular() > 0.0f)
                {
                    Vector3f blinnDir = rayoLuz->getDireccion() - rayo->getDireccion();
                    float temp = blinnDir * blinnDir;
                    if (temp != 0.0f )
                    {
                        float proyeccionVista = rayo->getDireccion() * normalInterseccion;
                        float p = proyeccionLuz - proyeccionVista;
                        float blinn = 0;
                        if (p > 0)
                            blinn = (1 / sqrtf(temp)) * p;
                        if (blinn > 0.0f)
                        {
                            blinn = fuerza * powf(blinn, material->Power());
                            if(start){
                                    coloresAux[2] = luz->Intensidad() * material->gColor() * blinn * material->Specular();
                                    color += coloresAux[2];
                            }
                            else
                                color += luz->Intensidad() * material->gColor() * blinn * material->Specular();
                        }
                    }
                }    
            }
        }
        delete rayoLuz;
    }
    
    if (prof < escena->maxProf)
    {
        //Check reflejo
        if (material->Reflexion() > 0.0)
        {      
            // float fuerzaNueva = fuerza * material->Reflexion();
            float reflet = 2.0f * (rayo->getDireccion() * normalInterseccion);
            Rayo* rayoReflejado = new Rayo();
            Vector3f temp = rayo->getDireccion() - (normalInterseccion * reflet);
            rayoReflejado->setDireccion(temp);
            //Traslado el punto de interseccion en la direccion del rayo una distancia LIMITE para que no salga de la superficie
            Vector3f puntoInicialRayo = puntoInterseccion + rayoReflejado->getDireccion() * LIMITE;
            rayoReflejado->setOrigen(puntoInicialRayo);
            this->Trace(color, rayoReflejado, escena, prof+1, fuerza * material->Reflexion(), indRefraccion, coloresAux,false);
            color *= material->Reflexion();
            if(start){
                    coloresAux[3].Set(1,1,1,1);
                    coloresAux[3]*= material->Reflexion();
            }
            delete rayoReflejado;   
        }
        //Check transparencia
        if (material->Refraccion() > 0.0 && material->Tranmission() > 0.0f)
        {
            //float fuerzaNueva = fuerza * material->Tranmission();
            float n = indRefraccion / material->Refraccion();
            float c1 = - (rayo->getDireccion() * normalInterseccion);
            float c2 = 1.0f - n*n*(1- c1*c1);
            if (c2 > 0.0f)
            {                
                Rayo* rayoRefractado = new Rayo();
                c2 = sqrtf(c2);
                Vector3f temp = rayo->getDireccion() * n + normalInterseccion * (n*c1 - c2);
                //temp.Normalize();
                rayoRefractado->setDireccion(temp);
                //Traslado el punto de interseccion en la direccion del rayo una distancia LIMITE para que no salga de la superficie
                Vector3f puntoInicialRayo = puntoInterseccion + rayoRefractado->getDireccion() * LIMITE;
                rayoRefractado->setOrigen(puntoInicialRayo);
                this->Trace(color,rayoRefractado, escena, prof+1, fuerza * material->Tranmission(), material->Refraccion(), coloresAux, false); //RECURSION
                color *= material->Tranmission();
                if(start){
                    coloresAux[4].Set(1,1,1,1);
                    coloresAux[4]*= material->Tranmission();
                }
                //printf("r = %f, g = %f, b = %f", color.Rojo(),color.Verde(),color.Azul());
                delete rayoRefractado;
            }         
        }
    }
}

bool Pantalla::inShade(Rayo* rayo, Escena* escena, float &distancia)
{
    bool shade = false;
    int origen;
    float dist;
    int cantObjetos = escena->objetos.size();
    for (int i = 0; i < cantObjetos; i++)
    {
        origen = (escena->objetos.at(i))->interseccion(rayo, dist);
        if ((origen == OUTSIDE || origen == INSIDE) && dist < distancia) //Un objeto esta en el camino de la luz, da sombra
        {    
            shade = true;
            break;
        }
    }
    return shade;
}

void Pantalla::ShadeBackground(Color& color)
{
    color.Set(0,0,0,1);
}

Pantalla::~Pantalla(){
}


float Pantalla::srgbEncode(float c)
{
    if (c <= 0.0031308f)
    {
        return 12.92f * c; 
    }
    else
    {
        return 1.055f * powf(c, 0.4166667f) - 0.055f; // Inverse gamma 2.4
    }
}
