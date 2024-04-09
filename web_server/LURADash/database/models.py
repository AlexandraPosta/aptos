'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: MYSQL Database tables definition
'''

import sys
from datetime import datetime, time
from sqlalchemy import Column
from sqlalchemy.types import Integer, Float, String, Date, Time, Text, BINARY

from database.connect import db

def unicode_to_str(value):
    """Converts unicode to string

    Converts unicode to string if python version is 2.x

    Args:
        value: Value to be converted

    Returns:
        Value converted to string
    """
    if sys.version_info[0] == 2:
        return value.encode("utf-8")
    else:
        return value


class Flight(db.Model):
    """Flight model for storing flight information

    Access to store the main information of a flights. This
    the main information that stays true throught the flight.

    Attributes:
        id_flight: Flight ID represented as an integer.
        rocket_name: Name of the rocket as string.
        motor: Name of the motor as string.
        date_of_launch: Date of the launch as string in YYYY-MM-DD format.
        time_of_launch: Time of the launch as string in HH:MM:SS format.
        location: Location of the launch as string.
        wind_speed: Wind speed as float.
        wind_direction: Wind direction as float.
        active_control: Active control as boolean.
        comments: Comments as string.
    """

    __tablename__ = "flight"

    id_flight = Column(Integer, autoincrement=True, nullable=False, primary_key=True)
    rocket_name = Column(String(10), nullable=False, default="None")
    motor = Column(String(20), nullable=False, default="None")
    date_of_launch = Column(Date, nullable=False, default="1000-01-01")
    time_of_launch = Column(Time, nullable=False, default="00:00:00")
    location = Column(String(20), nullable=False, default="None")
    wind_speed = Column(Float)
    wind_direction = Column(Float)
    active_control = Column(BINARY, nullable=False, default=0)
    comments = Column(Text())

    def __init__(self,
                 rocket_name="None",
                 motor="None",
                 date_of_launch="1000-01-01",
                 time_of_launch="00:00:00",
                 location="None",
                 wind_speed=0,
                 wind_direction=0,
                 active_control=0,
                 comments=None):
        self.rocket_name = rocket_name
        self.motor = motor
        self.date_of_launch = date_of_launch
        self.time_of_launch = time_of_launch
        self.location = location
        self.wind_speed = wind_speed
        self.wind_direction = wind_direction
        self.active_control = active_control
        self.comments = comments

    def __repr__(self):
        return unicode_to_str("<Flight: id_flight_data=%s rocket_name='%s' date_of_launch='%s'>" % 
                              (self.id_flight, self.rocket_name, self.date_of_launch))

    def as_dict(self):
        """Converts flight to dictionary

        Converts the flight instance to a dictionary. This is
        used to convert the flight instance to a JSON object to 
        allow for easy transmission backend-frontend.

        Returns:
            Dictionary of the flight instance
        """
        obj_dict = {c.name: getattr(self, c.name) for c in self.__table__.columns}

        for key, value in obj_dict.items():
            if isinstance(value, bytes):
                obj_dict[key] = value.decode('utf-8')  # Convert bytes to string or base64 encoding
            if isinstance(value, datetime):
                obj_dict[key] = value.isoformat()
            if isinstance(value, time):
                obj_dict[key] = value.isoformat()

        return obj_dict


class FlightData(db.Model):
    """Flight Data model for storing flight data

    Access to store the flight data. This is the data that
    is collected during the flight.

    Attributes:
        id_flight: list of flight ids
        raw_data: list of raw data values; normally True
        data_source: list of data sources as string: flash or telemetry
        timestamp: list of timestamps as string in HH:MM:SS format
        flight_stage: list of flight stages as string
        pressure: list of pressures in mbar
        altitude: list of altitudes in meters
        latitude: list of latitudes in degrees
        longitude: list of longitudes in degrees
        acceleration_x: list of x-axis accelerations in m/s^2
        acceleration_y: list of y-axis accelerations in m/s^2
        acceleration_z: list of z-axis accelerations in m/s^2
        orientation_x: list of x-axis orientations in degrees
        orientation_y: list of y-axis orientations in degrees  
        orientation_z: list of z-axis orientations in degrees
        temperature: list of temperatures in degrees Celsius
        humidity: list of humidities in percentage
        battery: list of battery levels in voltage
        sattelites: list of sattelite counts
        errors: list of errors as string
        servo_1: list of servo 1 positions as float
        servo_2: list of servo 2 positions as float
        servo_3: list of servo 3 positions as float
        servo_4: list of servo 4 positions as float
    
    """
    __tablename__ = "flight_data"

    id_flight_data = Column(Integer, autoincrement=True, nullable=False, primary_key=True)
    id_flight = Column(Integer,
        db.ForeignKey("flight.ID_flight",
                      use_alter=True,
                      name="flight.ID_flight",
                      ondelete="SET NULL"),
        nullable=False
    )
    raw_data = Column(BINARY, nullable=False, default=1)
    data_source = Column(String(10), nullable=False, default="None")
    timestamp = Column(Time, nullable=False, default="00:00:00")
    flight_stage = Column(String(10), nullable=False, default="None")
    pressure = Column(Float)
    altitude = Column(Float)
    latitude = Column(Float)
    longitude = Column(Float)
    acceleration_x = Column(Float)
    acceleration_y = Column(Float)
    acceleration_z = Column(Float)
    orientation_x = Column(Float)
    orientation_y = Column(Float)
    orientation_z = Column(Float)
    temperature = Column(Float)
    humidity = Column(Float)
    battery = Column(Float)
    sattelites = Column(Integer)
    errors = Column(Text())
    servo_1 = Column(Integer, default=0)
    servo_2 = Column(Integer, default=0)
    servo_3 = Column(Integer, default=0)
    servo_4 = Column(Integer, default=0)

    def __init__(self,
                 id_flight=0,
                 raw_data=1,
                 data_source="None",
                 timestamp="00:00:00",
                 flight_stage="None",
                 pressure=None,
                 altitude=None,
                 latitude=None,
                 longitude=None,
                 acceleration_x=None,
                 acceleration_y=None,
                 acceleration_z=None,
                 orientation_x=None,
                 orientation_y=None,
                 orientation_z=None,
                 temperature=None,
                 humidity=None,
                 battery=None,
                 sattelites=None,
                 errors=None,
                 servo_1=0,
                 servo_2=0,
                 servo_3=0,
                 servo_4=0):
        self.id_flight = id_flight
        self.raw_data = raw_data
        self.data_source = data_source
        self.timestamp = timestamp
        self.flight_stage = flight_stage
        self.pressure = pressure
        self.altitude = altitude
        self.latitude = latitude
        self.longitude = longitude
        self.acceleration_x = acceleration_x
        self.acceleration_y = acceleration_y
        self.acceleration_z = acceleration_z
        self.orientation_x = orientation_x
        self.orientation_y = orientation_y
        self.orientation_z = orientation_z
        self.temperature = temperature
        self.humidity = humidity
        self.battery = battery
        self.sattelites = sattelites
        self.errors = errors
        self.servo_1 = servo_1
        self.servo_2 = servo_2
        self.servo_3 = servo_3
        self.servo_4 = servo_4

    def __repr__(self):
        return unicode_to_str("<Flight Data: id_flight_data=%s id_flight='%s' timestamp='%s'>" % 
                              (self.id_flight_data, self.id_flight, self.timestamp))

    def as_dict(self):
        """Converts flight to dictionary

        Converts the flight instance to a dictionary. This is
        used to convert the flight instance to a JSON object to 
        allow for easy transmission backend-frontend.

        Returns:
            Dictionary of the flight instance
        """
        obj_dict = {c.name: getattr(self, c.name) for c in self.__table__.columns}

        for key, value in obj_dict.items():
            if isinstance(value, bytes):
                obj_dict[key] = value.decode('utf-8')  # Convert bytes to string or base64 encoding
            if isinstance(value, datetime):
                obj_dict[key] = value.isoformat()
            if isinstance(value, time):
                obj_dict[key] = value.isoformat()

        return obj_dict


class ControlCommand(db.Model):
    """Control Command model for storing control commands	

    Access to store the control commands. This is the data that
    is sent to the rocket to control it.

    Attributes:
        id_flight: list of flight ids
        timestamp: list of timestamps as string in HH:MM:SS format
        command_servo_1: list of servo 1 commands as integer
        command_servo_2: list of servo 2 commands as integer
        command_servo_3: list of servo 3 commands as integer
        command_servo_4: list of servo 4 commands as integer

    """
    __tablename__ = "control_command"

    id_control_commands = Column(Integer, autoincrement=True, nullable=False, primary_key=True)
    id_flight = Column(Integer,
        db.ForeignKey("flight.ID_flight",
                      use_alter=True,
                      name="flight.ID_flight",
                      ondelete="SET NULL"),
        nullable=False
    )
    timestamp = Column(Time, nullable=False, default="00:00:00")
    command_servo_1 = Column(Integer, default=0)
    command_servo_2 = Column(Integer, default=0)
    command_servo_3 = Column(Integer, default=0)
    command_servo_4 = Column(Integer, default=0)

    def __init__(self,
                 id_flight=0,
                 timestamp="00:00:00",
                 command_servo_1=0,
                 command_servo_2=0,
                 command_servo_3=0,
                 command_servo_4=0):
        self.id_flight = id_flight
        self.timestamp = timestamp
        self.command_servo_1 = command_servo_1
        self.command_servo_2 = command_servo_2
        self.command_servo_3 = command_servo_3
        self.command_servo_4 = command_servo_4

    def __repr__(self):
        return unicode_to_str("<Control Command: id_control_commands=%s id_flight='%s' timestamp='%s'>" % 
                              (self.id_control_commands, self.id_flight, self.timestamp))

    def as_dict(self):
        """Converts flight to dictionary

        Converts the flight instance to a dictionary. This is
        used to convert the flight instance to a JSON object to 
        allow for easy transmission backend-frontend.

        Returns:
            Dictionary of the flight instance
        """
        obj_dict = {c.name: getattr(self, c.name) for c in self.__table__.columns}

        for key, value in obj_dict.items():
            if isinstance(value, bytes):
                obj_dict[key] = value.decode('utf-8')  # Convert bytes to string or base64 encoding
            if isinstance(value, datetime):
                obj_dict[key] = value.isoformat()
            if isinstance(value, time):
                obj_dict[key] = value.isoformat()

        return obj_dict
