'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: MYSQL Database queries
'''

from database.dbconn import db
from database.models import Flight, FlightData, ControlCommand

def load_flights():
    return db.session.query(Flight).all()

def load_flight_data(id_flight):
    flight = db.session.query(Flight).filter(Flight.id_flight == id_flight).first()
    flight_data = db.session.query(FlightData).filter(FlightData.id_flight == id_flight)
    return flight, flight_data