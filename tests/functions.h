#define R 6.02214 * pow(10,23)
#define M 28.97
#define g 9.80665
#define m 1 // mass in kg

double drag(double v, double density, double CdA) {
  return CdA * v * v * density / 2;
}

double density(double pressure, double temperature) {
  return (M * pressure) / (R * temperature);
}

double apogee(double ycurrent, double vcurrent, double theta, double pressure, double temperature, double CdA) {
  double current_density = density(pressure, temperature);
  return ycurrent + (3 * vcurrent * vcurrent) / (2 * (g + drag(vcurrent, current_density, CdA) * cos(theta)/m));
}
