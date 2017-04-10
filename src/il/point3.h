#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef IL_POINT_3_H_
#define IL_POINT_3_H_

#include <cmath>
#include <ostream>

namespace il {
  template <class T>
  class Point3 {
    public:
      /** Constructor por default **/
      Point3() {};
      /** Constructor con 3 argumentos **/
      Point3(T X, T Y, T Z) : x(X),y(Y),z(Z) {};
      /** Constructor con 1 argumento vector **/
      Point3(T *V) : x(V[0]),y(V[1]),z(V[2]) {};
      /** Constructor con 1 argumento Point3 **/
      Point3(const Point3<T> &P) : x(P.v[0]),y(P.v[1]),z(P.v[2]) {};

      inline void set(T A, T B, T C) {
        x=A;
        y=B;
        z=C;
      };

      inline void set(T *V) {
        x=V[0];
        y=V[1];
        z=V[2];
      };

      /** Operador de comparación de igualdad*/
      inline bool operator==(Point3 p) {
        return (x == p.x && y == p.y && z == p.z);
      }

      /** Operador de suma*/
      inline Point3 operator+(Point3 p) {
        return Point3(x+p.x, y+p.y, z+p.z);
      }

      /** Operador de resta*/
      inline Point3 operator-(Point3 p) {
        return Point3(x-p.x, y-p.y, z-p.z);
      }

      /** Operador de multiplicación por escalar*/
      inline Point3 operator*(int p) {
        return Point3(x*p, y*p, z*p);
      }
      inline Point3 operator*(short p) {
        return Point3(x*p, y*p, z*p);
      }
      inline Point3 operator*(float p) {
        return Point3(x*p, y*p, z*p);
      }
      inline Point3 operator*(double p) {
        return Point3(x*p, y*p, z*p);
      }

      /**  Dot product **/
      inline T operator*(Point3 p) {
        return (x*p.x + y*p.y + z*p.z);
      }

      /**  Cross product **/
      inline Point3 operator^(Point3 p) {
        Point3 cross;
        cross.x = y*p.z - z*p.y;
        cross.y = z*p.x - x*p.z;
        cross.z = x*p.y - y*p.x;
        return cross;
      }

      /* Nomalización del vector */
      inline double abs() {
        double resultado;
        resultado  = x*x;
        resultado += y*y;
        resultado += z*z;
        return std::sqrt(resultado);
      }

      /* Nomalización del vector */
      inline Point3 normalized() {
        double norm = abs();
        Point3 p;
        p.x = x/norm;
        p.y = y/norm;
        p.z = z/norm;
        return p;
      }

      /** Elementos **/
      union {
        struct {
          T x;
          T y;
          T z;
        };
        T v[3];
      };

  }; // class Point3

  template <class T>
  std::ostream &operator<<(std::ostream &out, Point3<T> p){
    out <<  p.x << ", " << p.y << ", " << p.z;
    return out;
  };

  typedef Point3<short>  Point3s;
  typedef Point3<int>    Point3i;
  typedef Point3<float>  Point3f;
  typedef Point3<double> Point3d;

}; // namespace il

#endif

