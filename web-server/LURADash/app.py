'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: Entry point for the web server
'''

from flask import Flask, render_template, request, jsonify, after_this_request
from dbconn import get_flight
from datetime import date, datetime
import math
import random

# Create the Flask app
app = Flask(__name__)


def get_data():
    # Parameters for the simulation
    max_altitude = 13000    # Max altitude in meters (13 km)
    duration = 120          # Total duration of flight in seconds
    time_step = 10          # Time step in seconds

    # Simulating altitude, velocity, and acceleration
    time_labels = []
    altitude_values = []
    velocity_values = []
    acceleration_values = []

    longitude_values = []
    latitude_values = []

    # Initial coordinates (example)
    initial_latitude = 28.5721  # Example latitude
    initial_longitude = -80.6480  # Example longitude

    for i in range(0, duration + time_step, time_step):
        time = i
        # Simplified simulation for altitude, velocity, and acceleration
        altitude = max_altitude * math.sin(math.pi * time / (2 * duration))
        velocity = (math.pi * altitude / (2 * duration)) * math.cos(math.pi * time / (2 * duration))
        acceleration = -((math.pi ** 2) * max_altitude / (2 * duration ** 2)) * math.sin(math.pi * time / (2 * duration))

        # Simplified simulation for longitude and latitude
        latitude = initial_latitude + random.uniform(0, 0.0002)
        longitude = initial_longitude + random.uniform(0, 0.0002)

        time_labels.append(f"{time // 60:02d}:{time % 60:02d}.000")
        altitude_values.append(altitude)
        velocity_values.append(velocity)
        acceleration_values.append(acceleration)
        longitude_values.append(longitude)
        latitude_values.append(latitude)

    return jsonify({
        'labels': time_labels,
        'altitude': altitude_values,
        'velocity': velocity_values,
        'acceleration': acceleration_values,
        'pressure': 0,
        'temperature': 0,
        'humidity': 0,
        'sattelites': '0',
        'flight_stage': 'On Pad',
        'latitude': latitude_values,
        'longitude': longitude_values,
        'battery': 0,
        'error': [],
    })


@app.route('/data', methods=['GET'])
def hello():
    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response

    return get_data()


@app.route('/', methods=['GET', 'POST'])
@app.route('/flight-data', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        pass
    else:
        return render_template("flight.html")
    

@app.route('/telemetry', methods=['GET', 'POST'])
def telemetry():
    if request.method == 'POST':
        pass
    else:
        return render_template("telemetry.html")
    

@app.route('/database', methods=['GET', 'POST'])
def database():
    if request.method == 'POST':
        pass
    else:
        return render_template("database.html")


if __name__ == "__main__":
    from database import db, setup_db_model
    
    # SqlAlchemy Database Configuration With Mysql
    app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://root:admin''@localhost/aptosdb'
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
    db.init_app(app)
    app.app_context().push()
    setup_db_model()

    from models import Flight
    test_1 = Flight("rocket_3", "motor_3", date.today(), datetime.now().strftime("%H:%M:%S"), "location_3", 0, 0, 1, "Comment")	
    db.session.add(test_1)
    db.session.commit() 

    app.run(debug=True) # VSCode debug does not work otherwise