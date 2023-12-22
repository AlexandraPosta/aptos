Requirements:
- python 3.6+
- flask
- flask mysql connector
- flask SQLAlchemy 

To set up the webserver:
- install python 3.6+
- setup virtual environment ```pip install virtualenv``` 
- create environment ```virtualenv env```
- activate ```cd env\Scripts\``` then ```.\activate```
- ```pip install flask```
- ```pip install Flask-SQLAlchemy```
- ```pip install mysql-connector-python```

Trobleshoot
- when debugging the flask app, you might not hit the breakpoint using Visual Studio. Make sure to 
  - set the "args" from launch.json to ```--no-debugger, --no-reload```
  - go to app.py and run the app with debug set to ```False```
