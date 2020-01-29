#include "math.h"
double drag(double v, double ce) {
  double drag = pow(v, 2) * ce;
  return drag;
}
