#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef IL_POINT_2_H_
#define IL_POINT_2_H_

#include <ostream>

namespace il {
  template <class T>
  class Point2 {
    public:
      /** Constructor por default **/
      Point2() {};
      /** Constructor con 3 argumentos **/
      Point2(T X, T Y ) : x(X),y(Y) {};
      /** Constructor con 1 argumento vector **/
      Point2(T *V) : x(V[0]),y(V[1]) {};

      /** Operador de comparación de igualdad*/
      inline bool operator==(Point2 p) {
        return (x == p.x && y == p.y );
      }

      /** Operador de suma*/
      inline Point2 operator+(Point2 p) {
        return Point2(x+p.x, y+p.y);
      }

      /** Operador de resta*/
      inline Point2 operator-(Point2 p) {
        return Point2(x-p.x, y-p.y);
      }

      /** Operador de multiplicación por escalar*/
      inline Point2 operator*(int p) {
        return Point2(x*p, y*p);
      }
      inline Point2 operator*(short p) {
        return Point2(x*p, y*p);
      }
      inline Point2 operator*(float p) {
        return Point2(x*p, y*p);
      }
      inline Point2 operator*(double p) {
        return Point2(x*p, y*p);
      }

      /** Elementos **/
      union {
        struct {
          T x;
          T y;
        };
        T v[2];
      };
  }; // class Point2

  template <class T>
  std::ostream &operator<<(std::ostream &out, Point2<T> p){
    out <<  p.x << ", " << p.y;
    return out;
  };

  typedef Point2<short>  Point2s;
  typedef Point2<int>    Point2i;
  typedef Point2<float>  Point2f;
  typedef Point2<double> Point2d;

}; // namespace il

#endif

