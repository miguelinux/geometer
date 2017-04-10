#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef IL_PLANE_3D_H_
#define IL_PLANE_3D_H_

#include <cmath>
#include <ostream>

#include <il/point3.h>

namespace il {
  template <class T>
  class Plane3D {
    private:

      /* Nomalización de la normal del plano */
      inline void normalized() {
        double norm;
        norm  = a*a;
        norm += b*b;
        norm += c*c;
        norm  = std::sqrt(norm);
        a = a/norm;
        b = b/norm;
        c = c/norm;
      }

    public:
      /** Constructor por default **/
      Plane3D() {};
      /** Constructor con 3 argumentos **/
      Plane3D(T A, T B, T C, T D) : a(A),b(B),c(C),d(D) { normalized(); };
      /** Constructor con 1 argumento vector **/
      Plane3D(T *V) : a(V[0]),b(V[1]),c(V[2]), d(V[3]){ normalized(); };
      /** Constructor con 1 argumento Plane3D **/
      Plane3D(const Plane3D<T> &P) : a(P.v[0]),b(P.v[1]),c(P.v[2]), d(P.v[3]){ normalized(); };

      inline void set(T A, T B, T C, T D) {
        a=A;
        b=B;
        c=C;
        d=D;
        normalized();
      };

      template <class U>
      inline void set(Point3<U> p, T D) {
        a=p.x;
        b=p.y;
        c=p.z;
        d=D;
        normalized();
      };

      inline void set(T *V) {
        a=V[0];
        b=V[1];
        c=V[2];
        d=V[3];
        normalized();
      };

      inline bool operator==(Plane3D p) {
        return (a == p.a && b == p.b && c == p.c && d == p.d);
      }

      /**  Producto Punto **/
      template <class U>
      inline U operator*(Point3<U> p) {
        return (a*p.x + b*p.y + c*p.z + d);
      }

      template <class U>
      inline void getPoint(Point3<U> &p) {
        p.set(-a*d, -b*d, -c*d);
      }

      template <class U>
      inline void from3Points (Point3<U> p, Point3<U> q, Point3<U> r ) {
        Point3<U> pq,pr,n;
        pq = q-p;
        pr = r-p;
        n = pq^pr;
        a = n.x;
        b = n.y;
        c = n.z;
        d = -(n*p / n.abs());
        normalized();
        if (d>0) {
          a = -a;
          b = -b;
          c = -c;
          d = -d;
        }
      }

      /** Elementos **/
      union {
        struct {
          T a;
          T b;
          T c;
          T d;
        };
        T v[4];
      };

  }; // class Plane3D

  typedef Plane3D<double> Plane3Dd;

  template <class T>
  std::ostream &operator<<(std::ostream &out, Plane3D<T> p){
    out <<  p.a << ", " << p.b << ", " << p.c << ", " << p.d;
    return out;
  };

}; // namespace il

#endif

