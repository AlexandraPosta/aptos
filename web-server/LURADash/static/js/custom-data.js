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
    
    entries_to_lists(flight_data) {
        flight_data.forEach(entry => {
            this.add_entry(entry)        
        });
    }

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

    remove_entry(entry) {
        //TODO
    }
}
