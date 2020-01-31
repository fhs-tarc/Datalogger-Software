double drag(double v, double density) {
  double a = 1.0;
  double cdrag = 1.0;
  double drag = ((v * v) * density) / 2) * cdrag * a;
  return drag;
}
double density(double pressure, double temperature) {
  double r = 6.02214 * pow(10,23);
  double m = 28.97;
  double density = (m * pressure) / (r * temperature);
  return density;
}
double apogee(double ycurrent, double vcurrent, double theta, double pressure, double temperature) {
  double g = 9.80665;
  double density = density(pressure, temperature);
  double apogee = (3 * vcurrent * vcurrent) / (2 * (g * drag(vcurrent, density) * cos(theta));
  return apogee; 
}
