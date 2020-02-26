import csv, math
import matplotlib.pyplot as plt

# IMPORTANT:
# * must know Cd to 3 decimals
# * must use dry mass
# * must be measured accurately: current altitude, vertical velocity, pressure, temperature

m = 0.0605  # dry mass in kg
g = 9.80665

t = []
y = []
apogees = []


# estimates apogee using current altitude in m, velocity in m/s, and wind coefficient
def estimate_apogee(alt, vel, k):
    return alt + (m / (2 * k)) * math.log(1 + (k * vel * vel) / (m * g))


# finds density of dry air based on temperature in C, and pressure in Pa
def get_density(temp, pressure):
    return pressure / ((temp + 273.15) * 287.058)


# finds wind coefficient k based on density in kg/m3, drag coefficient Cd, and area in m2
def get_k(density, dragcoeff, area):
    return density * dragcoeff * area / 2


with open("simdata.csv") as f:
    csv_reader = csv.reader(f, delimiter=',')
    lines = 0
    for row in csv_reader:
        if lines == 0:  # first row is headers
            lines += 1
        elif float(row[0]) < 10:  # time is less than 10 seconds
            density = get_density(15, float(row[49]) * 100)  # find density
            k = get_k(density, 0.658, 0.000491)  # find wind coefficient
            apogees.append(estimate_apogee(float(row[1]), float(row[2]), k))  # estimate apogee and add to list
            y.append(float(row[1]))  # add actual altitude to list
            t.append(float(row[0]))  # add time to list
            lines += 1

# plots data
plt.plot(t, apogees)
plt.ylabel("Estimated Apogee (m)")
plt.xlabel("Time (s)")
plt.axvline(x=1.8639, color='k', linestyle='--')  # motor burnout time
plt.axvline(x=7.7939, color='k', linestyle='--')  # apogee time
plt.axhline(y=max(y), color='k', linestyle='--')  # apogee height
plt.ylim([max(y) - 4, max(y) + 2])
plt.show()
