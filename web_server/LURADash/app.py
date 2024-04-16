'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: Entry point for the web server
'''

from flask import Flask, render_template, request, jsonify, after_this_request
from flask_cors import CORS, cross_origin

from database.connect import db, setup_db_model
from database.commands import *
from database.fakedata import generate_fake_entry, get_fake_data


# Create the Flask app
app = Flask(__name__,
            static_url_path='',
            static_folder='static',
            template_folder='templates')


@app.route('/get-flights', methods=['GET'])
def get_flights():
    """API to return the list of flights in the database

    Returns:
        A list of flights in the database
    """
    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response

    data = load_flights()
    flights = [flight.as_dict() for flight in data] # Convert to dict
    return jsonify({'flights': flights})


@app.route('/flight-data/get-flight-data', methods=['GET'])
@app.route('/get-flight-data', methods=['GET'])
def get_flight_data():
    """API to return flight data from the database

    Returns:
        The flight and the flight data
    """
    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response

    flight, flight_data = load_flight_data(request.args.get('id'))
    flight_data = [data.as_dict() for data in flight_data]
    return jsonify({'flight': flight.as_dict(), 'flight_data': flight_data})


@app.route('/fake-data', methods=['GET'])
def init_data():
    """API to return fake data. Currently not used in the web app.

    Returns:
        Fake data
    """
    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response

    data = get_fake_data()
    return jsonify({data})


@app.route('/', methods=['GET', 'POST'])
@app.route('/flight-data', methods=['GET', 'POST'])
def index():
    """Main page of the web app

    Returns:
        The main page of the web app
    """
    if request.method == 'POST':
        pass
    else:
        return render_template("flight-data.html")


@app.route('/telemetry-data', methods=['GET', 'POST'])
def telemetry():
    """Telemetry data page of the web app

    Returns:
        The telemetry data page of the web app
    """
    if request.method == 'POST':
        pass
    else:
        return render_template("telemetry-data.html")


@app.route('/database', methods=['GET', 'POST'])
def database():
    """Database page of the web app

    Returns:
        The database page of the web app
    """
    if request.method == 'POST':
        pass
    else:
        return render_template("database.html")


@app.route('/get-db-tables', methods=['GET'])
def database_tables():
    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response
    tables = db.metadata.tables.keys()
    return jsonify({'tables': list(tables)})


@app.route('/database/get-db-columns', methods=['GET'])
def database_table_columns():
    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response
    
    columns = []
    table = db.metadata.tables.get(request.args.get('table'))
    search = table.columns
    for column in search:
        c = {}
        c['name'] = column.name
        c['type'] = str(column.type)
        columns.append(c)

    return jsonify({'columns': columns})


@app.route('/database/get-db-column-data', methods=['GET'])
def database_column_data():
    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response

    table = db.metadata.tables.get(request.args.get('table'))
    column = table.columns.get(request.args.get('column'))
    data = get_column_data(table, column)
    return jsonify(data=data)


@app.route('/database/get-db-table-data', methods=['GET'])
def database_table_all_data():
    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response

    table = db.metadata.tables.get(request.args.get('table'))
    column = table.columns.get(request.args.get('column'))
    filter = request.args.get('filter')

    if filter != "undefined":
        data = get_filtered_table(table, column, filter)
    else:
        data = get_table(table)

    return jsonify({'data': data})


@app.route('/add-data', methods=['GET', 'POST'])
def add_data():
    """Data ingestion page of the web app

    Returns:
        The data ingestion page of the web app
    """
    if request.method == 'POST':
        pass
    else:
        return render_template("add-data.html")
    

@app.route('/add-data/upload', methods=['POST'])
@cross_origin()
def upload():
    """API store flight data to the database

    Returns:
        The flight and the flight data
    """
    data = request.json
    # Process all data only if CSV exists
    if 'csvData' in data:
        upload_data(data)
    return jsonify({"message": "Data stored successfully"}), 200


if __name__ == "__main__":
    # SqlAlchemy Database Configuration With Mysql
    app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://root:admin''@localhost/aptosdb'
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
    CORS(app)
    db.init_app(app)
    app.app_context().push()
    setup_db_model()

    # Generate fake data into the database
    #generate_fake_entry()
    table = db.metadata.tables['flight']
    all_columns = {}
    for column in table.columns:
        all_columns[column.name] = column.type

    # Run the app
    # VSCode debug works if debug=false
    # Frontend updates on reload if debug=true
    app.run(debug=True)
