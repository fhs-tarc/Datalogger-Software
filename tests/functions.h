#define R 6.02214 * pow(10,23)
#define M 28.97
#define g 9.80665

double drag(double v, double density, double CdA) {
  return drag = CdA * v * v * density / 2;
}

double density(double pressure, double temperature) {
  return density = (M * pressure) / (R * temperature);
}

double apogee(double ycurrent, double vcurrent, double theta, double pressure, double temperature, double CdA) {
  double density = density(pressure, temperature);
  return apogee = ycurrent + (3 * vcurrent * vcurrent) / (2 * g * drag(vcurrent, density, CdA) * cos(theta));
}
