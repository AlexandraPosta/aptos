'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: Fake data generator for the database
'''

from datetime import date, datetime, timedelta
import random
import string
import numpy as np

from database.dbconn import db
from database.models import Flight, FlightData

class RocketSimulator:
    def __init__(self, 
                 initial_altitude, 
                 initial_latitude, 
                 initial_longitude, 
                 max_thrust, 
                 thrust_duration, 
                 initial_temp):
        self.g = 9.81 
        self.flight_stage = 'On Pad'
        self.velocity = np.array([0., 0., 0.])  
        self.altitude = initial_altitude
        self.acceleration = np.array([0., 0., -self.g])  # Acceleration in x, y, z
        self.orientation = np.array([0., 0., 0.])  # Orientation in roll, pitch, yaw
        self.max_thrust = max_thrust  # Maximum thrust
        self.thrust_duration = thrust_duration  # Duration of the thrust
        self.latitude = initial_latitude
        self.longitude = initial_longitude
        self.temperature = initial_temp

    def thrust(self, time_elapsed):
        if time_elapsed < self.thrust_duration:
            # Linearly decrease thrust over time
            return self.max_thrust * (1 - time_elapsed / self.thrust_duration)
        else:
            return 0

    def update(self, time_step, time_elapsed):
        # Calculate current thrust and resulting acceleration
        current_thrust = self.thrust(time_elapsed)
        
        # Accoun for parachute deployment
        if self.flight_stage == 'Main':
            self.acceleration = np.array([0., 0., -self.g/20])
        else:
            self.acceleration = np.array([0., 0., current_thrust - self.g])

        # Add random disturbances to x and y axes
        self.acceleration[:2] += np.random.uniform(-0.1, 0.1, 2)

        # Updates
        self.velocity += self.acceleration * time_step
        self.altitude += self.velocity[2] * time_step + 0.5 * self.acceleration[2] * time_step**2
        self.latitude += random.uniform(0, 0.00001)
        self.longitude += random.uniform(0, 0.00001)
        self.orientation += np.random.uniform(-0.1, 0.1, 3)
        self.temperature -= 0.1

        # Check for landing
        if self.altitude < 0:
            self.altitude = 0

        # Update flight stage
        if self.acceleration[2] > 0:
            self.flight_stage = 'Liftoff'
        elif self.acceleration[2] < 0:
            self.flight_stage = 'Main'
        elif -0.1 < self.acceleration[2] < 0.1:
            self.flight_stage = 'Apogee'
        else:
            self.flight_stage = 'Down'

def generate_fake_entry():
    # Get fake random name
    letters = string.ascii_lowercase
    rocket_name = ''.join(random.choice(letters) for i in range(8))

    # Create fake flight entry
    flight = Flight(rocket_name, 
                    "Motor", 
                    date.today(), 
                    datetime.now().strftime("%H:%M:%S"), 
                    "Location", 
                    random.randint(0,5), 
                    random.randint(-90,90), 
                    0, 
                    "Comment")	
    
    db.session.add(flight)
    db.session.commit() 

    # Create fake flight data entries
    # Simulation parameters
    initial_altitude = 0            # Initial altitude in m  
    max_thrust = 100                # Maximum thrust in m/s^2
    thrust_duration = 10            # Duration of the thrust in seconds
    initial_temp = 15               # Initial temperature in degrees C
    battery = 7.4
    sattelites = 5
    initial_latitude = 28.5721   
    initial_longitude = -80.6480
    time_step = 1                   # Time step in seconds
    total_time = 300                # Total simulation time in seconds
    time_elapsed = 0

    simulator = RocketSimulator(initial_altitude, 
                                initial_latitude, 
                                initial_longitude, 
                                max_thrust, 
                                thrust_duration, 
                                initial_temp)

    while time_elapsed <= total_time:
        simulator.update(time_step, time_elapsed)
        time_elapsed += time_step

        # Appending flight data to the database
        flight_data = FlightData(flight.id_flight,
                                 b'0',
                                 "Flash",
                                 "{:0>8}".format(str(timedelta(seconds=time_elapsed))),
                                 simulator.flight_stage,
                                 0,
                                 simulator.altitude,
                                 simulator.latitude,
                                 simulator.longitude,
                                 simulator.acceleration[0],
                                 simulator.acceleration[1],
                                 simulator.acceleration[2],
                                 simulator.orientation[0],
                                 simulator.orientation[1],
                                 simulator.orientation[2],
                                 simulator.temperature,
                                 0,
                                 battery,
                                 sattelites,
                                 "None",
                                 0,
                                 0,
                                 0,
                                 0)
        db.session.add(flight_data)
        db.session.commit() 

        if simulator.altitude <= 0 and time_elapsed > thrust_duration:
            break  # Stop the simulation if the rocket has reached the ground
        