/* 
 * File:   Color.h
 * Author: Germán_Ruiz
 *
 * Created on 20 de junio de 2012, 23:01
 */

#ifndef COLOR_H
#define	COLOR_H

class Color {
public:
    Color(){ rojo = 0; azul = 0; verde = 0; alpha = 1; }
    Color(float r, float g, float b,float a) { rojo = r;  verde = g;  azul = b;  alpha = a; }
    Color(const Color& orig) { rojo = orig.rojo;  verde = orig.verde;  azul = orig.azul;  alpha = orig.alpha;  }
    virtual ~Color(){}
    
    inline float Rojo()         { return rojo;  }
    inline float Azul()         { return azul;  }
    inline float Verde()        { return verde; }
    inline float Alpha()        { return alpha; }
    inline void Rojo(float r)   { rojo  = r; }
    inline void Azul(float a)   { azul  = a; }
    inline void Verde(float v)  { verde = v; }
    inline void Alpha(float a)  { alpha = a; }
    inline void Set(float r,float g, float b, float a) { rojo = r;  verde = g;  azul = b;  alpha = a; }
    
    Color& operator= (const Color &c) { rojo = c.rojo;  verde = c.verde;  azul = c.azul; }
    Color& operator+= (const Color &c){ rojo += c.rojo; verde += c.verde; azul += c.azul; }
    Color& operator*= (float c){ rojo *= c; verde *= c; azul *= c; }
    Color& operator* (float c) { rojo *= c; verde *= c; azul *= c; }
private:
    float rojo, verde, azul, alpha;
    

};

inline Color operator+ (Color &c1, Color &c2)
{
    Color c(c1.Rojo() + c2.Rojo(), c1.Verde() + c2.Verde(), c1.Azul() + c2.Azul(), c1.Alpha());
    return c;  
}

inline Color operator* (Color &c1, Color &c2)
{
    Color c(c1.Rojo() * c2.Rojo(), c1.Verde() * c2.Verde(), c1.Azul() * c2.Azul(), c1.Alpha());
    return c;  
}

inline Color operator* (float coef, Color &c1)
{
    return c1 * coef;  
}

#endif	/* COLOR_H */

