'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: MYSQL Database tables definition
'''

import sys
from sqlalchemy import Column
from sqlalchemy.types import Integer, Float, String, Date, Time, Text, BINARY

from database.dbconn import db

def unicode_to_str(value):
    if sys.version_info[0] == 2:
        return value.encode("utf-8")
    else:
        return value


class Flight(db.Model):
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
                 location="None"	, 
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
        return unicode_to_str("<Flight: id_flight=%s rocket_name='%s' date_of_launch='%s'>" % 
                              (self.id_flight, self.rocket_name, self.date_of_launch))


class FlightData(db.Model):
    __tablename__ = "flight_data"

    id_flight_data = Column(Integer, autoincrement=True, nullable=False, primary_key=True)
    id_flight = Column(Integer, 
        db.ForeignKey("flight.ID_flight", use_alter=True, name="flight.ID_flight", ondelete="SET NULL"),
        nullable=False 
    )
    raw_data = Column(BINARY, nullable=False, default=1)
    data_source = Column(String(10), nullable=False, default="None")
    timestamp = Column(Time, nullable=False, default="00:00:00")
    flight_stage = Column(String(10), nullable=False, default="None")
    barometer_raw = Column(Float)
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
                 barometer_raw=None,
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
        self.barometer_raw = barometer_raw
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


class ControlCommand(db.Model):
    __tablename__ = "control_command"

    id_control_commands = Column(Integer, autoincrement=True, nullable=False, primary_key=True)
    id_flight = Column(Integer, 
        db.ForeignKey("flight.ID_flight", use_alter=True, name="flight.ID_flight", ondelete="SET NULL"),
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
