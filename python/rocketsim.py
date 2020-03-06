import math
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from enum import Enum

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


class Motor:
    def __init__(self, filename):
        self.thrustcurve = []
        self.time = []
        self.thrust = []
        self.mass = 0
        self.prop_mass = 0

        if filename:
            self.load_from_eng(filename)

    def load_from_eng(self, filename):
        with open(filename) as f:
            lines = f.readlines()

        for line in lines:
            if line[0] is " ":
                line = line.strip().split(" ")
                line = [float(a) for a in line]
                self.thrustcurve.append(line)
                self.time.append(line[0])
                self.thrust.append(line[1])
            elif line[0] is not ";":
                line = line.strip().split(" ")
                self.prop_mass = line[4]
                self.mass = line[5]

    def get_thrust(self, time):
        for i, t in enumerate(self.time):
            if time < t:
                x0 = self.time[i - 1]
                x1 = t
                y0 = self.thrust[i - 1]
                y1 = self.thrust[i]
                break

        try:
            xdiff = x1 - x0

            return (y0 * (x1 - time) + y1 * (time - x0)) / xdiff
        except UnboundLocalError:
            return 0

    def get_impulse(self):
        return -np.trapz(self.time, self.thrust)

    def get_burnout(self):
        return max(self.time)

    def get_mass(self, time):
        pass


class States(Enum):
    IDLE = 0
    ASCENT = 1
    DESCENT = 2
    TOUCHDOWN = 3


class Simulator:
    def __init__(self, mass, motor: Motor = None, cd=0.0, area=0.0):
        self.mass = mass
        self.motor = motor
        self.Cd = cd
        self.area = area
        self.air_temp = 15  # C

        self.state = States.IDLE
        self.time = 0
        self.altitude = 0
        self.velocity = 0
        self.acceleration = 0
        self.drag = 0
        self.pressure = 0
        self.density = 0

        self.columns = ["Time", "Altitude", "Velocity", "Acceleration", "Drag"]
        self.data = pd.DataFrame(columns=self.columns)

    def run(self, dt):
        done = False
        self.state = States.ASCENT

        while not done:
            self.physics_step(dt)
            self.data = self.data.append(pd.Series([self.time, self.altitude, self.velocity, self.acceleration, self.drag],
                                                   index=self.columns), ignore_index=True)

            self.time += dt

            if self.velocity < 0 and self.altitude < 0 and self.time > 1:
                done = True

            if self.velocity < 0 and self.state == States.ASCENT:
                dt = 0.1
                self.state = States.DESCENT

    def physics_step(self, dt):
        self.altitude += self.velocity * dt
        self.velocity += self.acceleration * dt
        self.pressure = self.get_pressure(self.altitude)
        self.density = self.get_density(self.air_temp, self.pressure)
        print(self.density)
        self.drag = (self.velocity ** 2) * self.get_k(self.density, self.Cd, self.area)
        if self.velocity > 0:
            self.drag *= -1
        self.acceleration = g + (self.motor.get_thrust(self.time) + self.drag) / self.mass
        # print(self.motor.get_thrust(self.time)*dt)

    @staticmethod
    def get_pressure(altitude):
        """p = 101325 (1 - 2.25577 10-5 h)^5.25588"""
        return 101325 * (1 - 2.25577 * 10 ** -5 * altitude) ** 5.25588

    @staticmethod
    def get_density(temperature, pressure):
        """Gets dry air density"""
        return pressure / ((temperature + 273.15) * 287.058)

    @staticmethod
    def get_k(density, cd, area):
        """Gets wind coef k based on density, Cd, area"""
        return density * cd * area / 2


def test_motor():
    eng = Motor("Estes_C6.eng")
    x = [a[0] for a in eng.thrustcurve]
    y = [a[1] for a in eng.thrustcurve]
    print(eng.get_impulse())
    print(eng.time)
    print(eng.thrust)
    plt.plot(x, y)
    plt.axvline(x=eng.get_burnout(), color='k', linestyle='--')
    plt.show()


def test_sim():
    plt.interactive(True)
    sim = Simulator(0.0712, Motor("Estes_C6.eng"), 0.65886, 0.00049087)
    sim.run(0.001)
    sim.data.plot(x="Time", y=["Altitude", "Velocity", "Acceleration", "Drag"])
    print("Apogee: " + str(max(sim.data["Altitude"])))


if __name__ == '__main__':
    test_sim()
