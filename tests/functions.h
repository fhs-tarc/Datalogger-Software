double drag(double v, double ce) {
  double drag = v * v * ce;
  return drag;
}
double density(double pressure, double temperature) {
  double r = 6.02214 * pow(10,23);
  double m = 28.97;
  double density = (m * pressure) / (r * temperature);
}
