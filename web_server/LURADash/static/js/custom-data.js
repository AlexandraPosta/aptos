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
* @param {list} id_flight               - List of flight ids
* @param {list} raw_data                - List of raw data values; normally True
* @param {list} data_source             - List of data sources as string: flash or telemetry
* @param {list} timestamp               - List of timestamps as string in HH:MM:SS format
* @param {list} flight_stage            - List of flight stages as string
* @param {list} high_g_acceleration_x   - List of x-axis accelerations in mili g
* @param {list} high_g_acceleration_y   - List of y-axis accelerations in mili g
* @param {list} high_g_acceleration_z   - List of z-axis accelerations in mili g
* @param {list} imu_gyro_rate_x         - List of x-axis gyro rates in mili degrees per second
* @param {list} imu_gyro_rate_y         - List of y-axis gyro rates in mili degrees per second
* @param {list} imu_gyro_rate_z         - List of z-axis gyro rates in mili degrees per second
* @param {list} imu_gyro_offset_x       - List of x-axis gyro offsets in mili degrees per second
* @param {list} imu_gyro_offset_y       - List of y-axis gyro offsets in mili degrees per second
* @param {list} imu_gyro_offset_z       - List of z-axis gyro offsets in mili degrees per second
* @param {list} imu_acceleration_x      - List of x-axis accelerations in mili g
* @param {list} imu_acceleration_y      - List of y-axis accelerations in mili g
* @param {list} imu_acceleration_z      - List of z-axis accelerations in mili g
* @param {list} ms5611_temperature      - List of temperatures in degrees Celsius * 100
* @param {list} ms5611_pressure         - List of pressures in mbar * 100
* @param {list} gps_latitude            - List of latitudes in degrees
* @param {list} gps_longitude           - List of longitudes in degrees
* @param {list} gps_altitude            - List of altitudes in meters 
* @param {list} gps_velocity            - List of velocities in m/s
* @param {list} bme_pressure            - List of pressures in mbar
* @param {list} bme_temperature         - List of temperatures in degrees Celsius
* @param {list} bme_humidity            - List of humidities in percentage
* @param {list} euler_roll              - List of roll angles in milidegrees
* @param {list} euler_pitch             - List of pitch angles in milidegrees
* @param {list} euler_yaw               - List of yaw angles in milidegrees
* @param {list} euler_rate_roll         - List of roll rates in milidegrees per second
* @param {list} euler_rate_pitch        - List of pitch rates in milidegrees per second
* @param {list} euler_rate_yaw          - List of yaw rates in milidegrees per second
* @param {list} euler_kalman_roll       - List of kalman roll angles in milidegrees
* @param {list} euler_kalman_pitch      - List of kalman pitch angles in milidegrees
* @param {list} euler_kalman_yaw        - List of kalman yaw angles in milidegrees
* @param {list} battery                 - List of battery levels in voltage
* @param {list} sattelites              - List of sattelite counts
* @param {list} errors                  - List of errors as string
* @param {list} servo_deflection_1      - List of servo 1 deflection as milidegrees*10 as float
* @param {list} servo_deflection_2      - List of servo 2 deflection as milidegrees*10 as float
* @param {list} servo_deflection_3      - List of servo 3 deflection as milidegrees*10 as float
* @param {list} servo_deflection_4      - List of servo 4 deflection as milidegrees*10 as float
* @param {list} altitude                - List of altitudes in meters
* @param {list} vertical_velocity       - List of vertical velocities in m/s
* @property {None} 
*/
class FlightData {
    constructor() {
        this.id_flight = [];
        this.raw_data = [];
        this.data_source = [];
        this.timestamp = [];
        this.flight_stage = [];
        this.high_g_acceleration_x = [];
        this.high_g_acceleration_y = [];
        this.high_g_acceleration_z = [];
        this.imu_gyro_rate_x = [];
        this.imu_gyro_rate_y = [];
        this.imu_gyro_rate_z = [];
        this.imu_gyro_offset_x = [];
        this.imu_gyro_offset_y = [];
        this.imu_gyro_offset_z = [];
        this.imu_acceleration_x = [];
        this.imu_acceleration_y = [];
        this.imu_acceleration_z = [];
        this.ms5611_temperature = [];
        this.ms5611_pressure = [];
        this.gps_latitude = [];
        this.gps_longitude = [];
        this.gps_altitude = [];
        this.gps_velocity = [];
        this.bme_pressure = [];
        this.bme_temperature = [];
        this.bme_humidity = [];
        this.euler_roll = [];
        this.euler_pitch = [];
        this.euler_yaw = [];
        this.euler_rate_roll = [];
        this.euler_rate_pitch = [];
        this.euler_rate_yaw = [];
        this.euler_kalman_roll = [];
        this.euler_kalman_pitch = [];
        this.euler_kalman_yaw = [];
        this.battery = [];
        this.sattelites = [];
        this.errors = [];
        this.servo_deflection_1 = [];
        this.servo_deflection_2 = [];
        this.servo_deflection_3 = [];
        this.servo_deflection_4 = [];
        this.altitude = [];
        this.vertical_velocity = [];        
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
        this.high_g_acceleration_x.push(entry.high_g_acceleration_x);
        this.high_g_acceleration_y.push(entry.high_g_acceleration_y);
        this.high_g_acceleration_z.push(entry.high_g_acceleration_z);
        this.imu_gyro_rate_x.push(entry.imu_gyro_rate_x);
        this.imu_gyro_rate_y.push(entry.imu_gyro_rate_y);
        this.imu_gyro_rate_z.push(entry.imu_gyro_rate_z);
        this.imu_gyro_offset_x.push(entry.imu_gyro_offset_x);
        this.imu_gyro_offset_y.push(entry.imu_gyro_offset_y);
        this.imu_gyro_offset_z.push(entry.imu_gyro_offset_z);
        this.imu_acceleration_x.push(entry.imu_acceleration_x);
        this.imu_acceleration_y.push(entry.imu_acceleration_y);
        this.imu_acceleration_z.push(entry.imu_acceleration_z);
        this.ms5611_temperature.push(entry.ms5611_temperature);
        this.ms5611_pressure.push(entry.ms5611_pressure);
        this.gps_latitude.push(entry.gps_latitude);
        this.gps_longitude.push(entry.gps_longitude);
        this.gps_altitude.push(entry.gps_altitude);
        this.gps_velocity.push(entry.gps_velocity);
        this.bme_pressure.push(entry.bme_pressure);
        this.bme_temperature.push(entry.bme_temperature);
        this.bme_humidity.push(entry.bme_humidity);
        this.euler_roll.push(entry.euler_roll);
        this.euler_pitch.push(entry.euler_pitch);
        this.euler_yaw.push(entry.euler_yaw);
        this.euler_rate_roll.push(entry.euler_rate_roll);
        this.euler_rate_pitch.push(entry.euler_rate_pitch);
        this.euler_rate_yaw.push(entry.euler_rate_yaw);
        this.euler_kalman_roll.push(entry.euler_kalman_roll);
        this.euler_kalman_pitch.push(entry.euler_kalman_pitch);
        this.euler_kalman_yaw.push(entry.euler_kalman_yaw);
        this.battery.push(entry.battery);
        this.sattelites.push(entry.sattelites);
        this.errors.push(entry.errors);
        this.servo_deflection_1.push(entry.servo_deflection_1);
        this.servo_deflection_2.push(entry.servo_deflection_2);
        this.servo_deflection_3.push(entry.servo_deflection_3);
        this.servo_deflection_4.push(entry.servo_deflection_4);
        this.altitude.push(entry.altitude);
        this.vertical_velocity.push(entry.vertical_velocity);
    }
}
