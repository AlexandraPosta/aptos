"""
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: MYSQL Database queries
"""

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
