import mysql.connector

# Create a connection object
def get_flight():
    connection = mysql.connector.connect(host="localhost",
                                        port="3306",
                                        database="aptosdb",
                                        user="root",
                                        password="admin")

    cursor = connection.cursor()
    query = "SELECT * FROM aptosdb.flight"
    cursor.execute(query)
    flights = cursor.fetchall()

    cursor.close()
    connection.close()

    return flights