import matplotlib.pyplot as plt
import csv, math, openrocketparser

# Notes:
# * must know Cd to 3 decimals
# * must use dry mass
# * must be measured or estimated on vehicle: altitude, vertical velocity, pressure, temperature

g = -9.80665


class DragPanel:
    def __init__(self, Cd, area):
        self.area = area
        self.Cd = Cd
        self.deployed = 1.0

    def get_drag(self, density, vel):
        return self.deployed * self.Cd * self.area * density * (vel * vel) / 2

    def set_deployment(self, value):
        if value > 1:
            self.deployed = 1.0
        elif value < 0:
            self.deployed = 0.0
        else:
            self.deployed = value


def estimate_apogee(alt, vel, k):
    """Estimates apogee using alt in m, vel in m/s, wind coef k"""
    return alt + (m / (2 * k)) * math.log(1 + (k * vel * vel) / (m * g))


def get_density(temp, pressure):
    """Gets dry air density"""
    return pressure / ((temp + 273.15) * 287.058)


def get_pressure(altitude):
    """p = 101325 (1 - 2.25577 10-5 h)^5.25588"""
    return 101325 * (1 - 2.25577 * 10 ** -5 * altitude) ** 5.25588


def get_k(density, dragcoeff, area):
    """Gets wind coef k based on density, Cd, area"""
    return density * dragcoeff * area / 2


parser = openrocketparser.OpenRocketParser("simdata.csv")

apogee = parser.get_apogee()["Altitude"]
apogeetime = parser.get_apogee()["Time"]
burnout_data = parser.get_burnout()
burnout = burnout_data["Time"]
apogee_estimates = []

panel = DragPanel(1, 3*0.06*0.02)
panel.set_deployment(1)

# time = parser.get_column("Time")
# altitude = parser.get_column("Altitude")
times = []
altitudes = []

Cd = burnout_data["Drag coefficient"]
area = burnout_data["Reference area"] * 0.01 ** 2
m = burnout_data["Mass"] / 1000.0
print(m)

# print(Cd)
# print(area)

air_press = burnout_data["Air pressure"] * 100
air_temp = burnout_data["Air temperature"]
rho = get_density(air_temp, air_press)

v = burnout_data["Vertical velocity"]
a = g
y = burnout_data["Altitude"]
dt = 0.0001
time = burnout_data["Time"]
last_time = time - dt
done = False

while not done:
    air_press = get_pressure(y)
    rho = get_density(air_temp, air_press)
    k = get_k(rho, Cd, area)
    drag = Cd * area * rho * (v * v) / 2

    if v > 0:
        drag *= -1

    paneldrag = -1 * panel.get_drag(rho, v)
    a = g + (drag + paneldrag / m)
    v += a * dt
    y += v * dt

    times.append(time)
    altitudes.append(y)

    last_time = time
    time += dt

    if y < 0 or time > 1000:
        done = True

diff = abs(apogee - max(altitudes))
error = 100 * diff / max(altitudes)
print("Apogee difference: " + str(diff))
print("Percent error: " + str(error)[:5] + "%")

# for row in parser.data:
#     if row["Time"] < burnout:
#         continue
#
#     rho = get_density(row["Air temperature"], row["Air pressure"] * 100)
#     drag = Cd * area * rho * (v^2)
#     # k = get_k(rho, Cd, area)
#     # apogee_estimates.append(estimate_apogee(row["Altitude"], row["Vertical velocity"], k))

# plots data
# plt.plot(time, apogee_estimates)
plt.plot(times, altitudes)
plt.ylabel("Estimated Apogee (m)")
plt.xlabel("Time (s)")
# plt.axvline(x=burnout, color='k', linestyle='--')  # motor burnout time
# plt.axvline(x=apogeetime, color='k', linestyle='--')  # apogee time
# plt.axhline(y=apogee, color='k', linestyle='--')  # apogee height
# plt.ylim([apogee - 4, apogee + 2])
# plt.xlim([0, apogeetime + 1])
plt.show()
