/**
 * @organisation Leeds University Rocketry Organisation - LURA
 * @fileoverview Custon flight data class.
 * @author A. Posta
*/

/**
* Class to store flight data in array like lists. When return from server, 
* the data is in JSON format, which is not very useful for plotting. This class 
* converts the JSON data into lists, which can be used for plotting.
* @class FlightData
* @param {list} id_flight       - List of flight ids
* @param {list} raw_data        - List of raw data values; normally True
* @param {list} data_source     - List of data sources as string: flash or telemetry
* @param {list} timestamp       - List of timestamps as string in HH:MM:SS format
* @param {list} flight_stage    - List of flight stages as string
* @param {list} pressure        - List of pressures in mbar
* @param {list} altitude        - List of altitudes in meters
* @param {list} latitude        - List of latitudes in degrees
* @param {list} longitude       - List of longitudes in degrees
* @param {list} acceleration_x  - List of x-axis accelerations in m/s^2
* @param {list} acceleration_y  - List of y-axis accelerations in m/s^2
* @param {list} acceleration_z  - List of z-axis accelerations in m/s^2
* @param {list} orientation_x   - List of x-axis orientations in degrees
* @param {list} orientation_y   - List of y-axis orientations in degrees  
* @param {list} orientation_z   - List of z-axis orientations in degrees
* @param {list} temperature     - List of temperatures in degrees Celsius
* @param {list} humidity        - List of humidities in percentage
* @param {list} battery         - List of battery levels in voltage
* @param {list} sattelites      - List of sattelite counts
* @param {list} errors          - List of errors as string
* @param {list} servo_1         - List of servo 1 positions as float
* @param {list} servo_2         - List of servo 2 positions as float
* @param {list} servo_3         - List of servo 3 positions as float
* @param {list} servo_4         - List of servo 4 positions as float
* @property {None} 
*/
class FlightData {
    constructor() {
        this.id_flight = [];
        this.raw_data = [];
        this.data_source = [];
        this.timestamp = [];
        this.flight_stage = [];
        this.pressure = [];
        this.altitude = [];
        this.latitude = [];
        this.longitude = [];
        this.acceleration_x = [];
        this.acceleration_y = [];
        this.acceleration_z = [];
        this.orientation_x = [];
        this.orientation_y = [];
        this.orientation_z = [];
        this.temperature = [];
        this.humidity = [];
        this.battery = [];
        this.sattelites = [];
        this.errors = [];
        this.servo_1 = [];
        this.servo_2 = [];
        this.servo_3 = [];
        this.servo_4 = [];
    }
    
    /**
     * Converts JSON data to lists
     * @param {JSON} flight_data    JSON data from server
     * @returns {void}              None
     */
    entries_to_lists(flight_data) {
        flight_data.forEach(entry => {
            this.add_entry(entry)        
        });
    }

    /**
     * Adds entry to lists
     * @param {JSON} entry          JSON data from server
     * @returns {void}              None
     */
    add_entry(entry) {
        this.id_flight.push(entry.id_flight);
        this.raw_data.push(entry.raw_data);
        this.data_source.push(entry.data_source);
        this.timestamp.push(entry.timestamp);
        this.flight_stage.push(entry.flight_stage);
        this.pressure.push(entry.pressure);
        this.altitude.push(entry.altitude);
        this.latitude.push(entry.latitude);
        this.longitude.push(entry.longitude);
        this.acceleration_x.push(entry.acceleration_x);
        this.acceleration_y.push(entry.acceleration_y);
        this.acceleration_z.push(entry.acceleration_z);
        this.orientation_x.push(entry.orientation_x);
        this.orientation_y.push(entry.orientation_y);
        this.orientation_z.push(entry.orientation_z);
        this.temperature.push(entry.temperature);
        this.humidity.push(entry.humidity);
        this.battery.push(entry.battery);
        this.sattelites.push(entry.sattelites);
        this.errors.push(entry.errors);
        this.servo_1.push(entry.servo_1);
        this.servo_2.push(entry.servo_2);
        this.servo_3.push(entry.servo_3);
        this.servo_4.push(entry.servo_4);
    }
}
