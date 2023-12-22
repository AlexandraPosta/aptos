from flask import Flask, render_template, request

from dbconn import get_flight

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
    app.run(debug=False) # VSCode debug does not work otherwise