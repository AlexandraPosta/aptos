from flask_sqlalchemy import SQLAlchemy

flights = {}
flight_data = {}
control_commands = {}

class Flight():
    def __init__(self, 
                 flight_id, 
                 rocket_name, 
                 motor, 
                 date_of_launch, 
                 time_of_launch, 
                 location, 
                 wind_speed, 
                 wind_direction, 
                 active_control, 
                 comments):
    self.id = flight_id
    self.rocket_name = rocket_name
    self.motor = motor
    self.date_of_launch = date_of_launch
    self.time_of_launch = time_of_launch
    self.location = location
    self.wind_speed = wind_speed
    self.wind_direction = wind_direction
    self.active_control = active_control
    self.comments = comments


class FlightData():
    def __init__(self,
                 flight_data_id,
                 flight_id,
                 raw_data,
                 data_source,
                 timestamp,
                 flight_stage,
                 barometer_raw,
                 altitude,
                 latitude,
                 longitude,
                 acceleration_x,
                acceleration_y,
                acceleration_z,
                temperature,
                humidity,
                servo_1,
                servo_2,
                servo_3,
                servo_4):
    self.id = flight_data_id
    self.flight_id = flight_id
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
    self.temperature = temperature
    self.humidity = humidity
    self.servo_1 = servo_1
    self.servo_2 = servo_2
    self.servo_3 = servo_3
    self.servo_4 = servo_4


class ControlCommand:
    def __init__(self,
                 control_command_id,
                 flight_id,
                 timestamp,
                 command_servo_1,
                 command_servo_2,
                 command_servo_3,
                 command_servo_4):
    self.id = control_command_id
    self.flight_id = flight_id
    self.timestamp = timestamp
    self.command_servo_1 = command_servo_1
    self.command_servo_2 = command_servo_2
    self.command_servo_3 = command_servo_3
    self.command_servo_4 = command_servo_4
