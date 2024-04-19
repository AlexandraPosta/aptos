/**
 * @organisation Leeds University Rocketry Organisation - LURA
 * @fileoverview Export flight into csv for MATLAB input.
 * @author A. Posta
*/

//#region Data Gathering
function timeStringToSeconds(time_string) {
    const parts = time_string.split(':'); // Splitting by ':'
    if (parts.length !== 4) {
      throw new Error('Invalid time format');
    }
    const minutes = parseInt(parts[0], 10);
    const seconds = parseInt(parts[1], 10);
    const milliseconds = parseInt(parts[2], 10);
    const microseconds = parseInt(parts[3], 10);
  
    // Convert everything into seconds
    return minutes * 60 + seconds + (milliseconds / 1000) + (microseconds / 1000000);
}
  
function updateTimestamp(flight_data) {
    let _new = [];
    let base_time = 0;
    let base_seconds = 0;

    flight_data.forEach((data, index) => {
        if (index === 0) {
            base_time = data.timestamp;
            base_seconds = timeStringToSeconds(base_time);
        } else {
            const total_seconds = timeStringToSeconds(data.timestamp);
            // Subtracting the base time converted to seconds
            _new.push((total_seconds - base_seconds).toFixed(5));
        }
    });
    return _new;
}

function parseTime(timestamp) {
    let parts = timestamp.split(':');
    return parseInt(parts[0]) * 60 + parseInt(parts[1]) + parseInt(parts[2]) / 1000 + parseInt(parts[3]) / 1000000;
}

function getAltitude(flight_data) {
    let altitudes = [];     // meters
    const P0 = 1021.6;      // Standard atmospheric pressure at sea level in mbar
    flight_data.forEach(data => {
        let pressureInMbar = data.ms5611_pressure / 100; // converting from mbar*100 to mbar
        altitudes.push(44330 * (1 - Math.pow(pressureInMbar / P0, 1/5.255)) + 100);
    });
    return altitudes;
}

function getVerticalVelocity(flight_data) {
    vertical_velocity = [];     // meters/second
    let previousTime = parseTime(flight_data[0].timestamp);
    let previousVelocity = 0;

    flight_data.forEach((data, index) => {
        if (index === 0) {
            vertical_velocity.push(0); // Assuming initial velocity is zero
        } else {
            let currentTime = parseTime(data.timestamp);
            let deltaTime = currentTime - previousTime;
            let currentAcceleration = data.imu_acceleration_z * 0.00980665; // converting from mg to m/s^2
            let currentVelocity = previousVelocity + currentAcceleration * deltaTime;
            vertical_velocity.push(currentVelocity.toFixed(3));
            previousTime = currentTime;
            previousVelocity = currentVelocity;
        }
    });
    return vertical_velocity;
}

function getMass(flight, times) {
    const propellant_mass = 0.908;                  // kg
    const burn_time = 1.9;                          // seconds
    const burn_rate = propellant_mass / burn_time;  // kg per second
    let current_mass = [];                          // kg

    times.forEach((time) => {
        // Calculate mass only during the burn period
        if (time <= burn_time) {
            current_mass.push((flight.initial_mass - burn_rate * time));
        } else {
            // After burnout, mass remains constant (only hardware mass)
            current_mass.push(flight.initial_mass - propellant_mass);
        }
    });
    return current_mass;
}

function getLongitudinalMomentOfInertia(flight, masses) {
    let long_moment_inertia = [];                                   // kg*m^2
    const propellant_height = 0.478;                                // meters
    const propellant_mass = 0.908;                                  // kg
    const radius = 0.103;                                           // meters
    const total_height = 1.9;                                       // meters
    const structure_mass = flight.initial_mass - propellant_mass;   // Constant structure mass
    const structure_height = total_height - propellant_height;      // Constant structure height

    
    masses.forEach((mass) => {
        // Remaining propellant mass calculation
        const remaining_propellant_mass = mass - structure_mass;
        const effective_propellant_height = propellant_height * (remaining_propellant_mass / propellant_mass);

        // Calculate moment of inertia for the remaining propellant
        const propellant_I = (1/12) * remaining_propellant_mass * (3 * radius * radius + effective_propellant_height * effective_propellant_height);

        // Moment of inertia of the remaining structure (assumed constant)
        const structure_I = (1/12) * structure_mass * (3 * radius * radius + structure_height * structure_height);

        // Total moment of inertia
        long_moment_inertia.push(propellant_I + structure_I);
    });

    return long_moment_inertia;
}

function getRotationalMomentOfInertia(masses) {
    let rotational_I = [];          // kg*m^2
    const radius = 0.103;           // meters
    masses.forEach((mass) => {
        // Calculate the rotational moment of inertia of the entire rocket, assuming the radius does not change
        rotational_I.push(0.5 * mass * radius * radius);
    });
    return rotational_I;
}

function getCGLocation(flight, masses) {
    let cg_locations = [];          // meters

    masses.forEach((mass) => {
        const consumed_mass = flight.initial_mass - mass;

        // Assuming CG shifts linearly as the propellant burns, and the propellant is at the base of the rocket
        const cg_shift = (consumed_mass / flight.initial_mass) * (flight.CG_location / 2);
        cg_locations.push(flight.CG_location - cg_shift);
    });

    return cg_locations;
}

function getMachNumber(flight_data, vertical_velocity) {
    let mach = [];
    const gamma = 1.4;  // Adiabatic index for air
    const R = 287;      // Specific gas constant for air, J/(kg·K)

    flight_data.forEach((data, index) => {
        // Convert temperature from Celsius to Kelvin
        const temperature_K = data.ms5611_temperature/100 + 273.15;

        // Calculate the speed of sound in air at the given temperature
        const speed_of_sound = Math.sqrt(gamma * R * temperature_K);

        // Calculate the Mach number
        mach.push(vertical_velocity[index] / speed_of_sound);
    });

    return mach;
}

//#endregion

//#region CSV
function convertToCSV(data) {
    // CSV header
    const headers = "Times,Altitudem,VerticalVelocityms,Masskg,LongitudinalMomentOfInertiakgm2,RotationalMomentOfInertiakgm2,CGLocationm,MachNumber,AirPressurembar";

    // Map each row array to a CSV string and join them with newline characters
    const rows = data.map(row => row.join(","));
    rows.unshift(headers);  // Add headers at the beginning

    // Join all rows with newline to form the complete CSV string
    return rows.join("\n");
}

function downloadCSV(csvContent, filename) {
    // Create a Blob from the CSV String
    const blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });

    // Create a link element
    const link = document.createElement("a");
    const url = URL.createObjectURL(blob);
    link.setAttribute("href", url);
    link.setAttribute("download", filename);

    // Append the link to the body (required for Firefox)
    document.body.appendChild(link);

    // Simulate a click on the link
    link.click();

    // Clean up by removing the link
    document.body.removeChild(link);
}

function exportData() {
    search = document.getElementById('flight-search')
    var option = search.options[search.selectedIndex];
    id_flight = option.value;

    try {
        fetchFlightData(id_flight).then(data => {
          if (data) {
            flight_data = new FlightData();

            let times = updateTimestamp(data.flight_data);
            let vertical_velocities = getVerticalVelocity(data.flight_data);
            let altitudes = getAltitude(data.flight_data);
            let masses = getMass(data.flight, times);
            let l_moment_of_inertia = getLongitudinalMomentOfInertia(data.flight, masses);
            let r_moment_of_inertia = getRotationalMomentOfInertia(masses);
            let cg_locations = getCGLocation(data.flight, masses);
            let mach_numbers = getMachNumber(data.flight_data, vertical_velocities);
            let pressure = [];

            data.flight_data.forEach(entry => {
                pressure.push(entry.ms5611_pressure / 100); // converting from mbar*100 to mbar    
            });

            // Export data to CSV
            const combined_data = times.map((time, index) => [time, 
                                                              altitudes[index], 
                                                              vertical_velocities[index],
                                                              masses[index],
                                                              l_moment_of_inertia[index],
                                                              r_moment_of_inertia[index],
                                                              cg_locations[index],
                                                              mach_numbers[index],
                                                              pressure[index]
                                                            ]);
            const csv_content = convertToCSV(combined_data);
            downloadCSV(csv_content, "my_data.csv");
          }
        });
    } catch (error) {
        console.error('Failed to fetch data:', error);
    }
}
//#endregion

//#region Search Bar
function flightSearchBar() {
    try {
        fetchFlights().then(flights => {
        if (flights) {
            select = document.getElementById('flight-search');
            flights.forEach(flight => {
                option = document.createElement('option');
                option.id = flight.id_flight;
                option.value = flight.id_flight;
                option.text = flight.rocket_name + '-' + flight.motor + '-' + flight.date_of_launch;
                select.add(option);        
            });
        }
        });
    } catch (error) {
        console.error('Failed to fetch data:', error);
    }
}

/**
 * Function that fetches all flights from the database.
 * @return {Flight}     List of flights
 */
async function fetchFlights() {
    try { 
      const response = await fetch('/get-flights')
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();
      return data.flights;
    } catch (error) {
      console.error('Failed to fetch data:', error);
    }
}

/**
 * Function that fetches a flight from the database.
 * @param  {int} id_flight    ID of the flight
 * @return {data}             Flight and flight data
 */
async function fetchFlightData(id_flight) {
    try { 
      const url = new URL(window.location.href + '/get-flight-data');
      url.searchParams.append('id', id_flight);
  
      const response = await fetch(url, {
        method: 'GET',
        headers: {'Content-Type': 'application/json'}
      });
  
      if (!response.ok) {
        throw new Error('HTTP error! status: ${response.status}');
      }
      const data = await response.json();
      return data; // JSON data with flight and flight_data
    } catch (error) {
      console.error('Failed to fetch data:', error);
    }
  }
//#endregion


/**
 * Attach windows.onload DOM Events
 * @return {void}     None
 */
function attachEventHandlers() {};


// Mount Charts and Event Triggers
window.onload = function() {
    // Attach event handlers
    attachEventHandlers();
    
    // Populate search bar with flights
    flightSearchBar();
};