#define g 9.80665

// temperature in C, pressure in Pa, Cd, area in m2
double get_k(double temperature, double pressure, double Cd, double A) {
  return (pressure / ((temperature + 273.15) * 287.058)) * Cd * A / 2;
}

// current altitude in meters, current velocity in meters, wind resistance factor, mass in kg
double apogee(double y, double v, double k, double m) {
  return ycurrent + (m / (2 * k)) * log(1 + (k * v * v)/(m * g));
}
