#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef IL_HOUGH_3_H_
#define IL_HOUGH_3_H_

#include <cmath>
#include <ostream>

#include <il/plane3d.h>

namespace il {
  template <class T>
  class Hough3 {
    public:
      /** Constructor por default **/
      Hough3() {};
      /** Constructor con 3 argumentos **/
      Hough3(T X, T Y, T Z) : r(X),t(Y),p(Z) {};
      /** Constructor con 1 argumento vector **/
      Hough3(T *V) : r(V[0]),t(V[1]),p(V[2]) {};
      /** Constructor con 1 argumento Hough3 **/
      Hough3(const Hough3<T> &P) : r(P.v[0]),t(P.v[1]),p(P.v[2]) {};

      inline void set(T A, T B, T C) {
        r=A;
        t=B;
        p=C;
      };

      inline void set(T *V) {
        r=V[0];
        t=V[1];
        p=V[2];
      };

      /** Operador de comparación de igualdad*/
      inline bool operator==(Hough3 p) {
        return (r == p.r && t == p.t && p == p.p);
      }

      template <class U>
      inline void fromPlane(Plane3D<U> p)
      {
        this->r = -p.d;
        this->t = std::acos(p.c);
        this->p = std::atan2(p.b, p.a);
      }

      /**  Producto Punto **/
      template <class U>
      inline U operator*(Point3<U> p) {
        U result;
        result  = p.x * std::sin(this->t)*std::cos(this->p);
        result += p.y * std::sin(this->t)*std::sin(this->p);
        result += p.z * std::cos(this->t);
        result += -r;
        return result;
      }

      /** Elementos **/
      union {
        struct {
          T r; ///< RHO, radius or radial distance 
          T t; ///< THETA, inclination (or polar angle) 0..PI
          T p; ///< PSI, azimuth (or azimuthal angle)   0..2PI
        };
        T v[3];
      };

  }; // class Hough3

  template <class T>
  std::ostream &operator<<(std::ostream &out, Hough3<T> p){
    out <<  p.r << ", " << p.t << ", " << p.p;
    return out;
  };

  typedef Hough3<double> Hough3d;

}; // namespace il

#endif

