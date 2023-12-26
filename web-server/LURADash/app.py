'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: Entry point for the web server
'''

from flask import Flask, render_template, request
from dbconn import get_flight
from datetime import date, datetime


# Create the Flask app
app = Flask(__name__)


@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        pass
    else:
        flights = get_flight()
        return render_template("index.html", flights=flights)


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