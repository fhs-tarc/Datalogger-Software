#define g 9.80665
#define m 1 // mass in kg

double drag(double v, double density, double CdA) {
  return CdA * v * v * density / 2;
}

// pressure in Pa, temperature in C
double density(double pressure, double temperature) {
  return (pressure) / ((temperature + 273.15) * 287.058);
}

double apogee(double ycurrent, double vcurrent, double theta, double pressure, double temperature, double CdA) {
  double current_density = density(pressure, temperature);
  return ycurrent + (3 * vcurrent * vcurrent) / (2 * (g + drag(vcurrent, current_density, CdA) * cos(theta)/m));
}
