#define g 9.80665
// k is the wind coefficient 
// temperature in C, pressure in Pa, Cd estimated experimentally, area in m2
double getK(double density, double dragcoeff, double area) {
  return density * dragcoeff * (area / 2);
}
double getDensity(double temp, double pressure) {
  return pressure / ((temp + 273.15) * 287.058);
}
// current altitude in meters, current velocity in meters, wind resistance factor, mass in kg
double getApogee(double ycurrente, double v, double k, double m) {
  return ycurrent + (m / (2 * k)) * log(1 + (k * v * v)/(m * g));
}
