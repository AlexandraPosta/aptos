## LURA Dash - Stream your rocket flight

LURA Dash is a new web interface tool designed by Leeds Universiy Rocketry Association for visualisation of flight data. It offers multiple pages that enable users to interact with data in various formats. 

The main page features widgets including an altitude versus time graph, vertical velocity, vertical acceleration, and a flight path representation based on sensor fusion, along with other statistics. The "Run from Beginning" button plays an entire flight. Users can stop at any point to examine a particular moment in time. LURA Dash includes tabs for easy handling of CSV-formatted data from the Aptos flight computer. Once visualised and validated, any flight data can be formatted in the appropriate form for the input of our custom controller in MATLAB.

### Features
- load flight off the flight computer
- visualise the final outcome of the flight
- play the entire rocket flight and pause as needed
- visualise the data straight from the database; apply filters as needed
- import CSV file with new flight
- export to CSV that is compatible for the comtroller tuning in MATLAB

### Structure
The repository is structured as follows:

```
web_server
├──README.md
|
├──database           
|   ├──commands.py            # MYSQL Database queries
|   ├──connect.py             # MYSQL Database configuration
|   ├──fakedata.py            # Fake data generator for the database
|   └──models.py              # MYSQL Database tables definition
|
├──static                  
|   ├──3d                     # 3D models using in the frontend
|   ├──assets                 # Images using in the frontend
|   ├──css                    # The main css file
|   └──js                      
|     ├──add-data.js          # Contains functions used to ingest new data in the db
|     ├──custom-card.js       # Custon widgets class
|     ├──custom-data.js       # Custon flight data class
|     ├──database.js          # Database interacion from frontend
|     ├──export.js            # Export flight into csv for MATLAB input
|     ├──flight.js            # Contains functions used to display flight data on the dashboard.
|     ├──load-flight-data.js  # This file contains the code for the worker that loads the flight data.
|     └──telemetry.js         # This file contains functions used to display telemetry data on the dashboard.
|
├──templates               
|   ├──add-data.html          # HTML page that allows user to input flight data
|   ├──base.html              # HTML template for the all the rest of the pages
|   ├──database.html          # HTML page that allows user to display and filter the database 
|   ├──export-data.html       # HTML page that allows user to export data as a controller Simulink input
|   ├──flight-data.html       # main HTML page for flight data visualisation 
|   ├──flight.html            # HTML template for the flight related pages
|   └──telemetry-data.html    # HTML page for the telemetry connection
|
└──app                        # Entry point for the application
```

### Requirements
- python 3.6+
- flask
- flask mysql connector
- flask SQLAlchemy 

### To set up the webserver
- install python 3.6+
- setup virtual environment ```pip install virtualenv``` 
- create environment ```virtualenv env```
- activate ```.\env\Scripts\activate```
- ```pip install flask```
- ```pip install flask-cors```
- ```pip install sqlalchemy```
- ```pip install Flask-SQLAlchemy```
- ```pip install mysql-connector-python```

### Trobleshoot
- when debugging the flask app, you might not hit the breakpoint using Visual Studio. Make sure to 
- set the "args" from launch.json to ```--no-debugger, --no-reload```
- go to app.py and run the app with debug set to ```False```

### Future Improvements
- Integrate the telemetry connection (currenty the page is used as a placeholder);
- In the Dashboard, update the Sattelite view with GNSS data;
- In the Dashboard, add rocket 3D models based on the rocket name. Currently Pathfinder appears by default;
- Add more indications of "loading", "complete load"
- The export data page has various preset parameters (motor weight, trusts, etc...); create a method to dynamically grab the motor data during export routine;
- Include user identification;
- Include caching;
- Host on server;