import matplotlib.pyplot as plt
import csv, math, openrocketparser

# IMPORTANT:
# * must know Cd to 3 decimals
# * must use dry mass
# * must be measured accurately: current altitude, vertical velocity, pressure, temperature

m = 0.0605  # dry mass in kg
area = 0.000491  # area in m^2
Cd = 0.658  # drag coefficient

g = 9.80665


def estimate_apogee(alt, vel, k):
    """Estimates apogee using alt in m, vel in m/s, wind coef k"""
    return alt + (m / (2 * k)) * math.log(1 + (k * vel * vel) / (m * g))


def get_density(temp, pressure):
    """Gets dry air density"""
    return pressure / ((temp + 273.15) * 287.058)


def get_k(density, dragcoeff, area):
    """Gets wind coef k based on density, Cd, area"""
    return density * dragcoeff * area / 2


parser = openrocketparser.OpenRocketParser("simdata.csv")

apogee = parser.get_apogee()
time = parser.get_column("Time")
altitude = parser.get_column("Altitude")


with open("simdata.csv") as f:
    csv_reader = csv.reader(f, delimiter=',')
    lines = 0
    for row in csv_reader:
        if len(row) == 1:  # comments or headers
            lines += 1
        elif 1.5 < float(row[0]) < 8.5:  # time is less than 10 seconds
            density = get_density(15, float(row[49]) * 100)  # find density
            k = get_k(density, Cd, area)  # find wind coefficient
            apogees.append(estimate_apogee(float(row[1]), float(row[2]), k))  # estimate apogee and add to list
            y.append(float(row[1]))  # add actual altitude to list
            t.append(float(row[0]))  # add time to list
            lines += 1

# plots data
plt.plot(t, apogees)
plt.ylabel("Estimated Apogee (m)")
plt.xlabel("Time (s)")
plt.axvline(x=burnouttime, color='k', linestyle='--')  # motor burnout time
plt.axvline(x=apogeetime, color='k', linestyle='--')  # apogee time
plt.axhline(y=apogee, color='k', linestyle='--')  # apogee height
plt.ylim([max(y) - 4, max(y) + 2])
plt.show()
