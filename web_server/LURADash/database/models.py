'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: MYSQL Database tables definition
'''

import sys
from datetime import datetime, time
from sqlalchemy import Column
from sqlalchemy.types import Integer, Float, String, Date, Time, Text

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
    rocket_name = Column(String(20), nullable=False, default="None")
    motor = Column(String(20), nullable=False, default="None")
    date_of_launch = Column(Date, nullable=False, default="1000-01-01")
    time_of_launch = Column(Time, nullable=False, default="00:00:00")
    location = Column(String(20), nullable=False, default="None")
    wind_speed = Column(Float)
    wind_direction = Column(String(20), nullable=False, default="None")
    active_control = Column(Integer, nullable=False, default=0)
    initial_mass = Column(Float, nullable=False, default=0)
    CG_location = Column(Float, nullable=False, default=0)
    CP_location = Column(Float, nullable=False, default=0)
    comments = Column(Text())

    def __init__(self,
                 rocket_name="None",
                 motor="None",
                 date_of_launch="1000-01-01",
                 time_of_launch="00:00:00",
                 location="None",
                 wind_speed=0,
                 wind_direction=0,
                 active_control=b'\x01',
                 initial_mass=0,
                 CG_location=0,
                 CP_location=0,
                 comments=None):
        self.rocket_name = rocket_name
        self.motor = motor
        self.date_of_launch = date_of_launch
        self.time_of_launch = time_of_launch
        self.location = location
        self.wind_speed = wind_speed
        self.wind_direction = wind_direction
        self.active_control = active_control
        self.initial_mass = initial_mass
        self.CG_location = CG_location
        self.CP_location = CP_location
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
        data_source: list of data sources as string: flash or telemetry
        timestamp: list of timestamps as string in MIN:SEC:MILI:MICRO format
        flight_stage: list of flight stages as string
        high_g_acceleration_x: list of x-axis accelerations in m/s^2
        high_g_acceleration_y: list of y-axis accelerations in m/s^2
        high_g_acceleration_z: list of z-axis accelerations in m/s^2
        imu_gyro_rate_x: list of x-axis gyro rates in degrees/s
        imu_gyro_rate_y: list of y-axis gyro rates in degrees/s
        imu_gyro_rate_z: list of z-axis gyro rates in degrees/s
        imu_gyro_offset_x: list of x-axis gyro offsets in degrees/s
        imu_gyro_offset_y: list of y-axis gyro offsets in degrees/s
        imu_gyro_offset_z: list of z-axis gyro offsets in degrees/s
        imu_acceleration_x: list of x-axis accelerations in m/s^2
        imu_acceleration_y: list of y-axis accelerations in m/s^2
        imu_acceleration_z: list of z-axis accelerations in m/s^2
        ms5611_temperature: list of temperatures in degrees Celsius
        ms5611_pressure: list of pressures in mbar
        gps_latitude: list of latitudes in degrees
        gps_longitude: list of longitudes in degrees
        gps_altitude: list of altitudes in meters
        gps_velocity: list of velocities in m/s
        bme_pressure: list of pressures in mbar
        bme_temperature: list of temperatures in degrees Celsius
        bme_humidity: list of humidities in percentage
        euler_roll: list of rolls in degrees
        euler_pitch: list of pitches in degrees
        euler_yaw: list of yaws in degrees
        euler_rate_roll: list of rates of rolls in degrees/s
        euler_rate_pitch: list of rates of pitches in degrees/s
        euler_rate_yaw: list of rates of yaws in degrees/s
        battery: list of battery levels in voltage
        sattelites: list of sattelite counts
        errors: list of errors as string    
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
    raw_data = Column(Integer, nullable=False, default=1)
    data_source = Column(String(10), nullable=False, default="None")
    timestamp = Column(String(255), nullable=False, default="00:00:00")
    flight_stage = Column(String(10), nullable=False, default="None")
    high_g_acceleration_x = Column(Float, default="0")
    high_g_acceleration_y = Column(Float, default="0")
    high_g_acceleration_z = Column(Float, default="0")
    imu_gyro_rate_x = Column(Float, default="0")
    imu_gyro_rate_y = Column(Float, default="0")
    imu_gyro_rate_z = Column(Float, default="0")
    imu_gyro_offset_x = Column(Float, default="0")
    imu_gyro_offset_y = Column(Float, default="0")
    imu_gyro_offset_z = Column(Float, default="0")
    imu_acceleration_x = Column(Float, default="0")
    imu_acceleration_y = Column(Float, default="0")
    imu_acceleration_z = Column(Float, default="0")
    ms5611_temperature = Column(Float, default="0")
    ms5611_pressure = Column(Float, default="0")
    gps_latitude = Column(Float, default="0")
    gps_longitude = Column(Float, default="0")
    gps_altitude = Column(Float, default="0")
    gps_velocity = Column(Float, default="0")
    bme_pressure = Column(Integer, default="0")
    bme_temperature = Column(Integer, default="0")
    bme_humidity = Column(Integer, default="0")
    euler_roll = Column(Float, default="0")
    euler_pitch = Column(Float, default="0")
    euler_yaw = Column(Float, default="0")
    euler_rate_roll = Column(Float, default="0")
    euler_rate_pitch = Column(Float, default="0")
    euler_rate_yaw = Column(Float, default="0")
    battery = Column(Float, default="0")
    sattelites = Column(Integer, default="0")
    errors = Column(Text())

    def __init__(self,
                 id_flight=0,
                 raw_data=1,
                 data_source="None",
                 timestamp="00:00:000:000",
                 flight_stage="None",
                 high_g_acceleration_x=0,
                 high_g_acceleration_y=0,
                 high_g_acceleration_z=0,
                 imu_gyro_rate_x=0,
                 imu_gyro_rate_y=0,
                 imu_gyro_rate_z=0,
                 imu_gyro_offset_x=0,
                 imu_gyro_offset_y=0,
                 imu_gyro_offset_z=0,
                 imu_acceleration_x=0,
                 imu_acceleration_y=0,
                 imu_acceleration_z=0,
                 ms5611_temperature=0,
                 ms5611_pressure=0,
                 gps_latitude=0,
                 gps_longitude=0,
                 gps_altitude=0,
                 gps_velocity=0,
                 bme_pressure=0,
                 bme_temperature=0,
                 bme_humidity=0,
                 euler_roll=0,
                 euler_pitch=0,
                 euler_yaw=0,
                 euler_rate_roll=0,
                 euler_rate_pitch=0,
                 euler_rate_yaw=0,
                 battery=0,
                 sattelites=0,
                 errors="None"):
        self.id_flight = id_flight
        self.raw_data = raw_data
        self.data_source = data_source
        self.timestamp = timestamp
        self.flight_stage = flight_stage
        self.high_g_acceleration_x = high_g_acceleration_x
        self.high_g_acceleration_y = high_g_acceleration_y
        self.high_g_acceleration_z = high_g_acceleration_z
        self.imu_gyro_rate_x = imu_gyro_rate_x
        self.imu_gyro_rate_y = imu_gyro_rate_y
        self.imu_gyro_rate_z = imu_gyro_rate_z
        self.imu_gyro_offset_x = imu_gyro_offset_x
        self.imu_gyro_offset_y = imu_gyro_offset_y
        self.imu_gyro_offset_z = imu_gyro_offset_z
        self.imu_acceleration_x = imu_acceleration_x
        self.imu_acceleration_y = imu_acceleration_y
        self.imu_acceleration_z = imu_acceleration_z
        self.ms5611_temperature = ms5611_temperature
        self.ms5611_pressure = ms5611_pressure
        self.gps_latitude = gps_latitude
        self.gps_longitude = gps_longitude
        self.gps_altitude = gps_altitude
        self.gps_velocity = gps_velocity
        self.bme_pressure = bme_pressure
        self.bme_temperature = bme_temperature
        self.bme_humidity = bme_humidity
        self.euler_roll = euler_roll
        self.euler_pitch = euler_pitch
        self.euler_yaw = euler_yaw
        self.euler_rate_roll = euler_rate_roll
        self.euler_rate_pitch = euler_rate_pitch
        self.euler_rate_yaw = euler_rate_yaw
        self.battery = battery
        self.sattelites = sattelites
        self.errors = errors

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
        timestamp: list of timestamps as string in MIN:SEC:MILI:MICRO format
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
    timestamp = Column(String(255), nullable=False, default="00:00:000:000")
    servo_deflection_1 = Column(Float, default=0)
    servo_deflection_2 = Column(Float, default=0)
    servo_deflection_3 = Column(Float, default=0)
    servo_deflection_4 = Column(Float, default=0)

    def __init__(self,
                 id_flight=0,
                 timestamp="00:00:000:000",
                 servo_deflection_1=0,
                 servo_deflection_2=0,
                 servo_deflection_3=0,
                 servo_deflection_4=0):
        self.id_flight = id_flight
        self.timestamp = timestamp
        self.servo_deflection_1 = servo_deflection_1
        self.servo_deflection_2 = servo_deflection_2
        self.servo_deflection_3 = servo_deflection_3
        self.servo_deflection_4 = servo_deflection_4

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
