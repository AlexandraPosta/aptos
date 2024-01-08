'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: Entry point for the web server
'''

from flask import Flask, render_template, request, jsonify, after_this_request
import math
import random

# Create the Flask app
app = Flask(__name__)

def get_data():
    # Parameters for the simulation
    max_altitude = 13000    # Max altitude in meters (13 km)
    duration = 120          # Total duration of flight in seconds
    time_step = 5           # Time step in seconds
    time_labels = []
    altitude_values = []
    velocity_values = []
    acceleration_values = []

    # Initial data
    initial_latitude = 28.5721
    initial_longitude = -80.6480 
    initial_temperature = 15

    longitude_values = [initial_latitude]
    latitude_values = [initial_longitude]
    temperature_values = [initial_temperature]
    j = 0

    for i in range(0, duration + time_step, time_step):
        time = i
        # Simplified simulation for altitude, velocity, and acceleration
        altitude = max_altitude * math.sin(math.pi * time / (2 * duration))
        velocity = (math.pi * altitude / (2 * duration)) * math.cos(math.pi * time / (2 * duration))
        acceleration = -((math.pi ** 2) * max_altitude / (2 * duration ** 2)) * math.sin(math.pi * time / (2 * duration))

        # Simplified simulation for longitude and latitude
        latitude = latitude_values[j] - random.uniform(0, -0.00001)
        longitude = longitude_values[j] + random.uniform(0, -0.00001)
        temperature = temperature_values[j] - random.uniform(0, 0.5)
        j += 1
        
        # Appending values to the lists
        time_labels.append(f"{time // 60:02d}:{time % 60:02d}.000")
        altitude_values.append(altitude)
        velocity_values.append(velocity)
        acceleration_values.append(acceleration)
        longitude_values.append(longitude)
        latitude_values.append(latitude)
        temperature_values.append(temperature)

    return jsonify({
        'labels': time_labels,
        'altitude': altitude_values,
        'velocity': velocity_values,
        'acceleration': acceleration_values,
        'pressure': 0,
        'temperature': temperature_values,
        'humidity': 0,
        'sattelites': '0',
        'flight_stage': 'On Pad',
        'latitude': latitude_values,
        'longitude': longitude_values,
        'battery': 0,
        'error': ['Test_1', 'Test_2'],
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
        return render_template("flight-data.html")

@app.route('/telemetry-data', methods=['GET', 'POST'])
def telemetry():
    if request.method == 'POST':
        pass
    else:
        return render_template("telemetry-data.html")

@app.route('/database', methods=['GET', 'POST'])
def database():
    if request.method == 'POST':
        pass
    else:
        return render_template("database.html")

if __name__ == "__main__":
    from database.dbconn import db, setup_db_model
    
    # SqlAlchemy Database Configuration With Mysql
    app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://root:admin''@localhost/aptosdb'
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
    db.init_app(app)
    app.app_context().push()
    setup_db_model()

    from database.fakedata import generate_fake_entry
    generate_fake_entry()

    # Run the app
    app.run(debug=False) # VSCode debug does not work otherwise
