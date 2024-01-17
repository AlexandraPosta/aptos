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
/**
 * Main function that updates all charts on the dashboard. Checks if the
 * flight data is composed of arrays and if not, converts it to an array.
 * This is a requirements for Plotly.extendTraces() to work.
 * @param   {Flight}           flight      Contains flight information
 * @param   {dict} flight_data Contains points of data for each flight
 * @return  {void}             None
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
  updateVelocity(flight_data.timestamp, flight_data.acceleration_x, flight_data.acceleration_z);
  updateControl(flight_data.longitude, flight_data.latitude, flight_data.altitude);
  updateMap(flight_data.longitude, flight_data.latitude, flight_data.altitude);
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
 * @param  {list} timestamp     List of timestamps in HH:MM:SS
 * @param  {list} velocity      List of velocities in m/s
 * @param  {list} acceleration  List of accelerations in m/s^2
 * @return {void}               None
 */
function updateVelocity(timestamp, velocity, acceleration) {
  var update_0 = {
    x:  [timestamp],
    y:  [acceleration],
  };
  var update_1 = {
    x:  [timestamp],
    y:  [acceleration],
  };

  Plotly.extendTraces('velChart', update_0, [0]);
  Plotly.extendTraces('velChart', update_1, [1]);
}

/**
 * Function that updates the control chart on the dashboard. It compares 
 * the flight of a perfectly vertical ascend with the actual data of the
 * flight. Takes advantage of Plotly.extendTraces() to update the chart
 * in real time.
 * @param  {list} longitude   List of longitudes in degrees
 * @param  {list} latitude    List of latitudes in degrees
 * @param  {list} altitude    List of altitudes in meters
 * @return {void}             None
 */
function updateControl(longitude, latitude, altitude) {
  var update_0 = { // ideal
    x: [Array(longitude.length).fill(longitude[0])],
    y: [Array(latitude.length).fill(latitude[0])],
    z: [altitude]
  };

  var update_1 = { // real
    x:  [longitude],
    y:  [latitude],
    z:  [altitude]
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
  document.getElementById('pressure').innerHTML  = flight_data.pressure[end];
  document.getElementById('temperature').innerHTML  = flight_data.temperature[end];
  document.getElementById('humidity').innerHTML  = flight_data.humidity[end];
  document.getElementById('sattelites').innerHTML  = flight_data.sattelites[end];
  document.getElementById('velocity').innerHTML  = 0; // TODO
  document.getElementById('acceleration').innerHTML  = flight_data.acceleration_z[end];
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
        name: trace_name
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
      select = document.getElementById('flight-search');
      flights.forEach(flight => {
        option = document.createElement('option');
        option.id = flight.id_flight;
        option.value = flight.id_flight;
        option.text = flight.rocket_name + '-' + flight.motor + '-' + flight.date_of_launch;
        select.add(option);        
      }); 
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
        if (loader) {
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
        flight_data = new FlightData(); // Convert from object to list of entries
        flight_data.entries_to_lists(data.flight_data)
        updateSequence(data.flight, flight_data);
      });
    } catch (error) {
      console.error('Failed to fetch data:', error);
    }
  }
}

/**
 * Generates a report for the selected flight. The report is in both word and PDF 
 * format and downloaded automtically to the user's computer.
 * @return {void}     None
 */
function flightGenerateReport() {
  //TODO 
}
//#endregion


//#region Fetch Data
/**
 * 
 * @param  {type} foo H
 * @return {type}     None
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
 * 
 * @param  {type} foo H
 * @return {type}     None
 */
async function fetchFlightData(id_flight) {
  try { 
    const url = new URL(window.location.href + '/get-flight-data');
    url.searchParams.append('id', id_flight);

    const response = await fetch(url, {
      method: 'GET',
      headers: {'Content-Type': 'application/json'}
    })

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
    getGraph('altChart', 250, window.innerWidth * 0.35,  ['time', 'altitude'], ['altitude']);
    getGraph('velChart', 250, window.innerWidth * 0.35, ['time', 'parameters'], ['v', 'acc']);
    getGraph('controlChart', 250, window.innerWidth * 0.35, ['altitude', 'latitude', 'longitude'], ['ideal', 'real']);

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