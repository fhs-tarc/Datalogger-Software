"""Parses simulation data from OpenRocket.

Contains functions to get data points at various events and at any given time.
Also allows direct access to data array and to array of data headers and units.

    **Typical Usage:**

    ``
    parser = OpenRocketParser("simdata.csv")
    print(parser.data[0]) #  prints first line of data
    ``
"""


class OpenRocketParser:
    """Class to parse openrocket simulation data.

    Attributes:
        events (dict): Dictionary containing OpenRocket simulation events and
            times. Keys are same as in simulation file. Times are in seconds.
        data (list of dict
    """

    def __init__(self, filename: str):
        """

        Args:
            filename:
        """
        self.events = {}
        self.data = []
        self.times = []
        self.headers = []
        self.units = {}

        with open(filename) as file:
            raw_data = file.readlines()

        for i, line in enumerate(raw_data):
            raw_data[i] = line.rstrip()

        for line in raw_data:
            line_array = line.split()

            if len(line_array) > 1:
                if line_array[1] == "Event":
                    self.events[line_array[2]] = float(line_array[5][2:])
                elif line_array[1] == "Time":
                    self.headers = line[2:].split(",")
                    for i in range(len(self.headers)):
                        header = self.headers[i]
                        start = header.find("(")
                        end = header.find(")")
                        self.headers[i] = header[:start - 1]
                        self.units[self.headers[i]] = header[start + 1:end]
            elif len(line) > 1:
                line_data = {}
                line_array = line.split(",")
                for i, value in enumerate(line_array):
                    line_data[self.headers[i]] = float(value)
                self.data.append(line_data)
                self.times.append(line_data["Time"])

    def get_units(self, field):
        """Finds units of a given field"""
        try:
            return self.units[field]
        except KeyError:
            return ""

    def get_data(self, time):
        """Gets data closest to given time"""
        for line in self.data:
            if line["Time"] >= time:
                return line

    def get_liftoff(self):
        """Gets data at liftoff"""
        try:
            return self.get_data(self.events["LIFTOFF"])
        except KeyError:
            pass

    def get_apogee(self):
        """Gets data at apogee"""
        return max(self.data, key=lambda x: x["Altitude"])

    def get_burnout(self):
        """Gets data at burnout"""
        try:
            return self.get_data(self.events["BURNOUT"])
        except KeyError:
            pass

    def get_eject(self):
        """Gets data at ejection charge"""
        try:
            return self.get_data(self.events["EJECTION_CHARGE"])
        except KeyError:
            pass

    def get_touchdown(self):
        """Gets data at touchdown/last data point"""
        return self.data[-1]

    def get_column(self, header, start=None, end=None):
        """Gets all data points from a single column.
        Can specify start and end point."""
        col = []
        for row in self.data:
            if start is None or start <= row["Time"] <= end:
                col.append(row[header])

        return col


def main():
    parser = OpenRocketParser("simdata.csv")
    print(parser.headers)
    print(parser.get_apogee())
    print(parser.get_column("Time", 1, 2))


if __name__ == '__main__':
    main()
