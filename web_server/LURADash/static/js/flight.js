/**
 * @organisation Leeds University Rocketry Organisation - LURA
 * @fileoverview This file contains functions used to display flight data on the dashboard.
 * @author A. Posta
*/

//#region Worker
let loader = undefined;

/**
 * Initialise the worker that loads data onto dashboard charts
 * @return {[void]}     None
 */
function initialiseWorker() {
  if (loader == undefined) {
    loader = new Worker('/js/load-flight-data.js');
    
    loader.onmessage = function(e) {
      const processedData = e.data;
      updateSequence(processedData[0], processedData[1]);
    };
  }
}

/**
 * Stops the worker that loads data onto dashboard charts.
 * This is called when switching to other tabs or when a new
 * set of data is loaded. Make sure to terminate before initialising
 * a new worker.
 * @return {[void]}     None
 */
function stopWorker() {
  if (loader) {
    loader.terminate();
    loader = undefined;
  }
}
//#endregion


//#region Dashboard Charts
let times = [];

/**
 * Main function that updates all charts on the dashboard. Checks if the
 * flight data is composed of arrays and if not, converts it to an array.
 * This is a requirements for Plotly.extendTraces() to work.
 * @param   {Flight}  flight        Contains flight information
 * @param   {dict}    flight_data   Contains points of data for each flight
 * @return  {void}                  None
 */
function updateSequence(flight, flight_data) {
  Object.entries(flight_data).forEach(([key, value]) => {
    if (!Array.isArray(value)) {
      Object.defineProperty(flight_data, key, {
          value: [value],
      });
    }
  });
  updateLaunch(flight);
  updateAlt(flight_data.timestamp, flight_data.altitude);
  updateVelocity(flight_data.timestamp, flight_data.vertical_velocity, flight_data.imu_acceleration_z);
  updateControl(flight_data.altitude, 
                flight_data.imu_acceleration_x, 
                flight_data.imu_acceleration_y, 
                flight_data.imu_acceleration_z, 
                flight_data.imu_gyro_rate_x,
                flight_data.imu_gyro_rate_y,
                flight_data.imu_gyro_rate_z);
  updateMap(flight_data.gps_longitude, flight_data.gps_latitude, flight_data.gps_altitude);
  updateStats(flight_data);
  updateFlightStages(flight_data.flight_stage);
}

/**
 * Function that deletes all traces from all charts on the dashboard.
 * @return  {void}             None
 */
function deleteSequence() {
  stopWorker();
  ids = ['altChart', 'velChart', 'controlChart'];

  ids.forEach(graphID => {
    const plotDiv = document.getElementById(graphID);
    const numberOfTraces = plotDiv.data.length

    for (let i = 0; i < numberOfTraces; i++) {
        Plotly.restyle(graphID, 'x', [[]], [i]);
        Plotly.restyle(graphID, 'y', [[]], [i]);
        if (plotDiv.data[i].z) {
          Plotly.restyle(graphID, 'z', [[]], [i]);
        }
    }
  });
}

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

function updateTimestamp(times) {
  let _new = [];
  const base_time = times[0];
  const base_seconds = timeStringToSeconds(base_time);

  times.map(time => {
    const total_seconds = timeStringToSeconds(time);
    let diff_seconds = total_seconds - base_seconds; // Subtracting the base time converted to seconds
    _new.push(diff_seconds.toFixed(5));
  });
  return _new;
}

/**
 * Function that updates the launch information on the dashboard under the
 * launch section.
 * @param  {Flight} flight Contains flight information
 * @return  {void}             None
 */
function updateLaunch(flight) {
  document.getElementById('rocket-name').textContent = flight.rocket_name;
  document.getElementById('motor-name').textContent = flight.motor;
  document.getElementById('date').textContent = flight.date_of_launch;
  document.getElementById('location').textContent = flight.location;
  document.getElementById('wind-direction').textContent = flight.wind_direction;
  document.getElementById('wind-speed').textContent = flight.wind_speed;
}

/**
 * Function that updates the altitude chart on the dashboard. Takes advantage
 * of Plotly.extendTraces() to update the chart in real time.
 * @param  {list} timestamp   List of timestamps in HH:MM:SS
 * @param  {list} altitude    List of altitudes in meters
 * @return {void}             None
 */
function updateAlt(timestamp, altitude) {
  var update = {
    x: [timestamp],
    y: [altitude],
  };
  Plotly.extendTraces('altChart', update, [0]);
}

/**
 * Function that updates the velocity chart on the dashboard. Takes advantage
 * of Plotly.extendTraces() to update the chart in real time.
 * @param  {list} timestamp     List of timestamps in MM:ff:SS
 * @param  {list} velocity      List of velocities in m/s
 * @param  {list} acceleration  List of accelerations in milig
 * @return {void}               None
 */
function updateVelocity(timestamp, velocity, acceleration) {
  let acc = acceleration.map(element => element / 1000);
  var update_0 = {
    x:  [timestamp],
    y:  [velocity],
  };
  var update_1 = {
    x:  [timestamp],
    y:  [acc],
  };

  Plotly.extendTraces('velChart', update_0, [0]);
  Plotly.extendTraces('velChart', update_1, [1]);
}

function integrate(data) {
  let integrated_data = [];
  let sum = 0;
  for (let i = 0; i < data.length; i++) {
    sum += data[i];
    integrated_data.push(sum);
  }
  return integrated_data;
}

/**
 * Function that updates the control chart on the dashboard. It compares 
 * the flight of a perfectly vertical ascend with the actual data of the
 * flight. Takes advantage of Plotly.extendTraces() to update the chart
 * in real time.
 * @param  {list} altitude      List of altitudes in meters
 * @param  {list} euler_pitch   List of euler pitch angles
 * @param  {list} euler_yaw     List of euler yaw angles
 * @return {void}               None
 */
function updateControl(altitude, acc_x, acc_y, gyro_rate_x,gyro_rate_y) {
  // Convert acc from milig to m/s^2
  let acc_x_mps2 = acc_x.map(element => element * 9.8 / 1000);
  let acc_y_mps2 = acc_y.map(element => element * 9.8 / 1000);

  // Convert gyro from milidegrees/s to degrees/s
  let gyro_rate_x_deg = gyro_rate_x.map(element => element / 1000);
  let gyro_rate_y_deg = gyro_rate_y.map(element => element / 1000);

  // Integrate acceleration to get velocity
  let velocity_x = integrate(acc_x_mps2);
  let velocity_y = integrate(acc_y_mps2);

  // Integrate velocity to get position
  let position_x = integrate(velocity_x);
  let position_y = integrate(velocity_y);
  position_x = position_x.map((val, i) => val + gyro_rate_x_deg[i]);
  position_y = position_y.map((val, i) => val + gyro_rate_y_deg[i]);

  var update_0 = { // ideal
    x: [Array(altitude.length).fill(altitude[0])],
    y: [Array(altitude.length).fill(altitude[0])],
    z: [altitude],
  };

  var update_1 = { // real
    x:  [position_x],
    y:  [position_y],
    z:  [altitude],
  };

  Plotly.extendTraces('controlChart', update_0, [0]);
  Plotly.extendTraces('controlChart', update_1, [1]);
}

/** 
 * Function that updates the map on the dashboard. Takes advantage of
 * the mapbox toolbok to update the chart in real time.
 * @param  {list} longitude   List of longitudes in degrees
 * @param  {list} latitude    List of latitudes in degrees
 * @param  {list} altitude    List of altitudes in meters
 * @return {void}             None
 */
function updateMap(longitude, latitude, altitude) {
  // TODO
}

/**
 * Function that updates the statistics on the dashboard under the
 * statistics section.
 * @param  {dict} flight_data Dictionary of flight data entries in a list format
 * @return {void}           None
 */
function updateStats(flight_data) {
  end = flight_data.timestamp.length - 1;
  let pressure = flight_data.ms5611_pressure.map(element => element / 100);
  let temperature = flight_data.ms5611_temperature.map(element => element / 100);
  let acc = flight_data.imu_acceleration_z.map(element => element / 1000);

  document.getElementById('pressure').innerHTML  = pressure[end];
  document.getElementById('temperature').innerHTML  = temperature[end];
  document.getElementById('humidity').innerHTML  = flight_data.bme_humidity[end];
  document.getElementById('sattelites').innerHTML  = flight_data.sattelites[end];
  document.getElementById('velocity').innerHTML  = flight_data.vertical_velocity[end]; // TODO
  document.getElementById('acceleration').innerHTML  = acc[end];
  document.getElementById('flight-stage').innerHTML  = flight_data.flight_stage[end];
  document.getElementById('battery').innerHTML  = flight_data.battery[end];
  document.getElementById('nr-errors').innerHTML  = flight_data.errors[end];
}

/**
 * Function that updates the flight stages on the dashboard under the
 * flight stages section.
 * @param  {list} stage List of strings that represent the flight stage of the entry
 * @return {None}       None
 */
function updateFlightStages(stage) {
  // TODO
}

/**
 * Function that creates a new chart on the dashboard. It uses the Plotly
 * and automatically identifies the number of traces and axis that are 
 * needed.
 * @param  {String} graph_name  Name of the graph
 * @param  {Float} height       Height of the graph on the monitor
 * @param  {Float} width        Width of the graph on the monitor
 * @param  {list} axis_name     List of strings that are axis names
 * @param  {list} traces_name   List of strings that are trace names
 * @return {void}               None
 */
function getGraph(graph_name, height, width, axis_name, traces_name) {
  var b = (axis_name.length == 2) ? 80 : 10; // 2D or 3D
  var traces =[];

  traces_name.forEach(trace_name => {
    if (axis_name.length == 2) {
      traces.push({
        x: [],
        y: [],
        type: 'lines+markers',
        name: trace_name
      });
    }
    else {
      traces.push({
        x: [],
        y: [],
        z: [],
        type: 'scatter3d',
        name: trace_name,
        mode: 'markers',
        marker: {
          size: 3,
        }
      });
    }
  });

  var layout = {
    width: width,  
    height: height,
    margin: {
      l: 50,  
      r: 0, 
      b: b,  
      t: 0,  
      pad: 0  
    },
    
    showlegend: true,
    plot_bgcolor: 'white', 
    paper_bgcolor: 'white',

    xaxis: { 
      title: { 
        text: axis_name[0],
        font: {
          family: 'Arial'
        } 
      },
      tickfont: {
        size: 10 
      },
    },
    yaxis: { 
      title: { 
        text: axis_name[1],
        font: {
          family: 'Arial'
        }  
      },
      tickfont: {
        size: 10 
      },
    },
  };

  if (axis_name.length == 3) {
    layout.scene = { 
      xaxis:{title: axis_name[2]},
      yaxis:{title: axis_name[1]},
      zaxis:{title: axis_name[0]},
    };
  }

  Plotly.newPlot(graph_name, traces, layout);
}  
//#endregion


//#region Populate Search Bar
/**
 * Function that populates the search bar with all flights from the database.
 * Flights are in the format: rocket_name-motor-date_of_launch inside the 
 * search bar.
 * @return {void}     None
 */
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
 * Function that plays a flight when the user selects a flight from the search bar.
 * The flight is identified by the id_flight attribute of the option element.
 * This function deletes the previous flight data and stops the previoud worker before 
 * starting a new one with the current data. It also updates the flight stages and 
 * statistics every two seconds (check worker file for further details). 
 * @return {void}     None
 */
function flightRun() {
  search = document.getElementById('flight-search')

  if (search.selectedIndex != 0) {
    deleteSequence();
    stopWorker();
    initialiseWorker();

    var option = search.options[search.selectedIndex];
    id_flight = option.value;

    try {
      fetchFlightData(id_flight).then(data => {      
        if (loader && data) {
          getVelocity(data.flight_data);
          getAltitude(data.flight_data);
          times = [];

          data.flight_data.forEach(entry => {
            times.push(entry.timestamp); 
          });

          let new_timestamp = updateTimestamp(times);

          data.flight_data.forEach((entry, index) => {
            entry.timestamp = new_timestamp[index];
          });

          flight_data.timestamp = new_timestamp;
          loader.postMessage(data);
        }
      });
    } catch (error) {
      console.error('Failed to fetch data:', error);
    }
  }
}

/**
 * Attah the events handler from the "Pause" DOM element
 * @return {void}     None
 */
function flightStop() {
  stopWorker();
}

/**
 * Function that displays the entire duration of a flight. It identifies
 * the flight by the id_flight attribute of the option element.
 * @return {void}     None
 */
function flightDisplay() {
  search = document.getElementById('flight-search')
  if (search.selectedIndex != 0) {
    deleteSequence();
    stopWorker();

    var option = search.options[search.selectedIndex];
    id_flight = option.value;
    
    try {
      fetchFlightData(id_flight).then(data => {
        if (data) {
          flight_data = new FlightData(); // Convert from object to list of entries
          getVelocity(data.flight_data);
          getAltitude(data.flight_data);
          flight_data.entries_to_lists(data.flight_data);
          let new_timestamp = updateTimestamp(flight_data.timestamp);
          flight_data.timestamp = new_timestamp;
          updateSequence(data.flight, flight_data);
        }
      });
    } catch (error) {
      console.error('Failed to fetch data:', error);
    }
  }
}
//#endregion


//#region Fetch Data
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

/**
 * 
 * @param  {}     
 * @return {}    
 */
function parseTime(timestamp) {
  let parts = timestamp.split(':');
  return parseInt(parts[0]) * 60 + parseInt(parts[1]) + parseInt(parts[2]) / 1000 + parseInt(parts[3]) / 1000000;
}

/**
 * 
 * @param  {}     
 * @return {}    
 */
function getVelocity(flight_data) {
  let previousTime = parseTime(flight_data[0].timestamp);
  let previousVelocity = 0;

  flight_data.forEach((data, index) => {
    if (index === 0) {
      data.vertical_velocity = 0; // Assuming initial velocity is zero
    } else {
      let currentTime = parseTime(data.timestamp);
      let deltaTime = currentTime - previousTime;
      let currentAcceleration = data.imu_acceleration_z / 1000; // converting from mGal to m/s^2
      let currentVelocity = previousVelocity + currentAcceleration * deltaTime;
      data.vertical_velocity = currentVelocity.toFixed(3);
      previousTime = currentTime;
      previousVelocity = currentVelocity;
    }
  });
}

/**
 * 
 * @param  {}     
 * @return {}    
 */
function getAltitude(flight_data) {
  const P0 = 1021.6; // Standard atmospheric pressure at sea level in mbar
  flight_data.forEach(data => {
    let pressureInMbar = data.ms5611_pressure / 100; // converting from mbar*100 to mbar
    data.altitude = 44330 * (1 - Math.pow(pressureInMbar / P0, 1/5.255)) + 100;
  });
}
//#endregion


/**
 * Attach windows.onload DOM Events
 * @return {void}     None
 */
function attachEventHandlers() { 
  // Search Bar Event Handler
  document.getElementById('flight-search').addEventListener('change', function(event) {
    flightDisplay();
  });  

  // Stop worker when switching to other tabs
  document.getElementById('flight-web').addEventListener('click', function(event) {
    stopWorker();
  });
  document.getElementById('telemetry-web').addEventListener('click', function(event) {
    stopWorker();
  });
  document.getElementById('db-web').addEventListener('click', function(event) {
    stopWorker();
  });
}


// Mount Charts and Event Triggers
window.onload = function() {  
  if (window.Worker) {
    initialiseWorker();
  } 
  
  // Create Map
  mapboxgl.accessToken = 'pk.eyJ1IjoiYWxleHBvc3RhIiwiYSI6ImNscW1sdzFyMDJjNDUyaW8weW9kc2luaWQifQ.bQ-1vhGoRIbowpy5_gNNFw';

    const map = new mapboxgl.Map({
      container: 'map',
      style: 'mapbox://styles/mapbox/satellite-v9', // Mapbox satellite style
      center: [0, 53], 
      zoom: 5,          // Starting zoom level
      pitch: 45,        // Angle for 3D perspective
      bearing: -10      // Rotation angle around the map center
    });

    map.on('load', function () {
        // Add control
        map.resize();
    });

    // Create charts
    getGraph('altChart', 250, window.innerWidth * 0.35,  ['Time (s)', 'Altitude (m)'], ['altitude']);
    getGraph('velChart', 250, window.innerWidth * 0.35, ['Time (s)', 'Parameters (m/s, g)'], ['v', 'acc']);
    getGraph('controlChart', 250, window.innerWidth * 0.35, ['Altitude', 'Position x', 'Position y'], ['ideal', 'real']);

    // Attach event handlers
    attachEventHandlers();
    
    // Populate search bar with flights
    flightSearchBar();
};

window.onresize = function() {
  Plotly.relayout('altChart', {
      'width': window.innerWidth * 0.35,
  });

  Plotly.relayout('velChart', {
    'width': window.innerWidth * 0.35,
  });

  Plotly.relayout('controlChart', {
    'width': window.innerWidth * 0.35,
  });
};