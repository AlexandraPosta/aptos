"""
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: MYSQL Database queries
"""
from sqlalchemy import Table
from datetime import datetime, time

from database.connect import db
from database.models import Flight, FlightData

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
