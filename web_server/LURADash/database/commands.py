"""
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: MYSQL Database queries
"""
from sqlalchemy import Table
from datetime import datetime, time

from database.connect import db
from database.models import Flight, FlightData, ControlCommand

def load_flights():
    """Dababase query to load all flights

    Retrives all flight instances from the Flight table
    in the database. Flights are instances of the Flight
    model.

    Args:
        None

    Returns:
        List of Flight instances
    """
    return db.session.query(Flight).all()


def load_flight_data(id_flight):
    """Dababase query to load data based on ID

    Retrives the flight and all flight data instances from 
    the FlightData table in the database. Filters them based
    on the ID of the flight. Flight data are instances of the
    FlightData model.

    Args:
        id_flight: Flight ID represented as a string.

    Returns:
        Flight instance and list of FlightData instances
    """
    flight = db.session.query(Flight).filter(Flight.id_flight == id_flight).first()
    flight_data = db.session.query(FlightData).filter(FlightData.id_flight == id_flight)
    return flight, flight_data


def get_column_data(table, column):
    """Dababase query to load all data from a column

    Retrives all data from a column in a table in the database.

    Args:
        table: The table name as Table.
        column: The column name as Column.

    Returns:
        List of data from the column
    """
    if isinstance(table, Table) and column.description in table.columns:
        values_raw = db.session.query(column).all()
        values = []
        for row in values_raw:
            if isinstance(row[0], bytes):
                values.append(row[0].decode('utf-8'))
            if isinstance(row[0], datetime):
                values.append(row[0].isoformat())
            if isinstance(row[0], time):
                values.append(row[0].isoformat())
            else:
                values.append(row[0])

        return list(set(values))
    else:
        raise ValueError("Invalid table or column name")


def get_table(table):
    """Dababase query to load table	

    Retrives all data from a table in the database based on the
    table name.

    Args:
        table: The table name as Table.

    Returns:
        Dict of table data
    """
    # Get headers and query all rows
    column_headers = [column.name for column in table.columns]
    all_rows = db.session.query(table).all()

    # Format results: Convert each row to a dictionary
    objs = [dict(zip(column_headers, row)) for row in all_rows]
    for obj in objs:
        for key, value in obj.items():
            if isinstance(value, bytes):
                obj[key] = value.decode('utf-8') 
            if isinstance(value, datetime):
                obj[key] = value.isoformat()
            if isinstance(value, time):
                obj[key] = value.isoformat()
    return objs


def get_filtered_table(table, column, filter):
    """Dababase query to load table

    Retrives all data from a table in the database based on the
    table name, column name and value filer.

    Args:
        table: The table name as Table.
        column: The column name as Column.
        filter: The filter value as string.

    Returns:
        Dict of table data
    """
    if isinstance(table, Table) and column.description in table.columns:
        column_headers = [column.name for column in table.columns]
        all_rows = db.session.query(table).filter(column == filter).all()
        
        # Format results: Convert each row to a dictionary
        objs = [dict(zip(column_headers, row)) for row in all_rows]
        for obj in objs:
                for key, value in obj.items():
                    if isinstance(value, bytes):
                        obj[key] = value.decode('utf-8') 
                    if isinstance(value, datetime):
                        obj[key] = value.isoformat()
                    if isinstance(value, time):
                        obj[key] = value.isoformat()
        return objs
    else:
        raise ValueError("Invalid table or column name")
    

def upload_data(data):
    # Upload flight first to return ID
    new_flight = Flight(
        rocket_name=data.get('rocket_name', 'None'),
        motor=data.get('motor', 'None'),
        date_of_launch=data.get('date_of_launch', '1000-01-01'),
        time_of_launch=data.get('time_of_launch', '00:00:00'),
        location=data.get('location', 'None'),
        wind_speed=data.get('wind_speed', 0),
        wind_direction=data.get('wind_direction', 0),
        active_control=data.get('active_control', 0),
        initial_mass=data.get('initial_mass', 0),
        CG_location=data.get('CG_location', 0),
        CP_location=data.get('CP_location', 0),
        comments=data.get('comments', '')
    )
    db.session.add(new_flight)
    db.session.flush()  # Flush to get the new_flight id_flight generated

    # Using the ID from the previous command, parse the csv data
    # into flight_data and control_command
    if 'csvData' in data:
        for entry in data['csvData']:
            flight_data = FlightData(
                id_flight=new_flight.id_flight,
                data_source=data.get('data_source', 'None'),
                raw_data=1,
                timestamp=entry.get('timestamp', '00:00:000:000'),
                flight_stage=entry.get('flight_stage', 'None'),
                high_g_acceleration_x=entry.get('high_g_acceleration_x', 0),
                high_g_acceleration_y=entry.get('high_g_acceleration_y', 0),
                high_g_acceleration_z=entry.get('high_g_acceleration_z', 0),
                imu_gyro_rate_x=entry.get('imu_gyro_rate_x', 0),
                imu_gyro_rate_y=entry.get('imu_gyro_rate_y', 0),
                imu_gyro_rate_z=entry.get('imu_gyro_rate_z', 0),
                imu_gyro_offset_x=entry.get('imu_gyro_offset_x', 0),
                imu_gyro_offset_y=entry.get('imu_gyro_offset_y', 0),
                imu_gyro_offset_z=entry.get('imu_gyro_offset_z', 0),
                imu_acceleration_x=entry.get('imu_acceleration_x', 0),
                imu_acceleration_y=entry.get('imu_acceleration_y', 0),
                imu_acceleration_z=entry.get('imu_acceleration_z', 0),
                ms5611_temperature=entry.get('ms5611_temperature', 0),
                ms5611_pressure=entry.get('ms5611_pressure', 0),
                gps_latitude=entry.get('gps_latitude', 0),
                gps_longitude=entry.get('gps_longitude', 0),
                gps_altitude=entry.get('gps_altitude', 0),
                gps_velocity=entry.get('gps_velocity', 0),
                bme_pressure=entry.get('bme_pressure', 0),
                bme_temperature=entry.get('bme_temperature', 0),
                bme_humidity=entry.get('bme_humidity', 0),
                euler_roll=entry.get('euler_roll', 0),
                euler_pitch=entry.get('euler_pitch', 0),
                euler_yaw=entry.get('euler_yaw', 0),
                euler_rate_roll=entry.get('euler_rate_roll', 0),
                euler_rate_pitch=entry.get('euler_rate_pitch', 0),
                euler_rate_yaw=entry.get('euler_rate_yaw', 0),
                battery=entry.get('battery', 0),
                sattelites=entry.get('sattelites', 0),
                errors=entry.get('errors', 'None')
            )
            db.session.add(flight_data)
            control_command = ControlCommand(
                id_flight=new_flight.id_flight,
                timestamp=entry.get('timestamp', '00:00:000:000'),
                servo_deflection_1=entry.get('servo_deflection_1', 0),
                servo_deflection_2=entry.get('servo_deflection_2', 0),
                servo_deflection_3=entry.get('servo_deflection_3', 0),
                servo_deflection_4=entry.get('servo_deflection_4', 0),
            )
            db.session.add(control_command)

    db.session.commit()

    # Using the same ID, update the controller
