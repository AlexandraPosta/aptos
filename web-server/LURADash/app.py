'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: Entry point for the web server
'''

from flask import Flask, render_template, request, jsonify, after_this_request

from database.dbconn import db, setup_db_model
from database.commands import *
from database.fakedata import generate_fake_entry, get_fake_data


# Create the Flask app
app = Flask(__name__)


@app.route('/get-flights', methods=['GET'])
def flightData():
    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response
    data = load_flights()
    flights = [flight.as_dict() for flight in data] # Convert to dict

    return jsonify(data=flights)


@app.route('/get-flight-data', methods=['GET', 'POST'])
def data():
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response
    flight, flight_data = load_flight_data()
    return jsonify(data={flight.as_dict(), flight_data.as_dict()})


@app.route('/init-data', methods=['GET'])
def initData():
    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response
    data = get_fake_data()
    return jsonify({
        'labels': data[0],
        'altitude': data[1],
        'velocity': data[2],
        'acceleration': data[3],
        'pressure': 0,
        'temperature': data[4],
        'humidity': 0,
        'sattelites': '0',
        'flight_stage': 'On Pad',
        'latitude': data[5],
        'longitude': data[6],
        'battery': 0,
        'error': ['Test_1', 'Test_2'],
    })


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
    # SqlAlchemy Database Configuration With Mysql
    app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://root:admin''@localhost/aptosdb'
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
    db.init_app(app)
    app.app_context().push()
    setup_db_model()

    # Generate fake data into the database
    #generate_fake_entry()

    # Run the app
    app.run(debug=True) # VSCode debug does not work otherwise
