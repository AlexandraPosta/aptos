// Update Dashboard Charts
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

function deleteSequence() {
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

function updateLaunch(flight) {
  document.getElementById('rocket-name').textContent = flight.rocket_name;
  document.getElementById('motor-name').textContent = flight.motor;
  document.getElementById('date').textContent = flight.date_of_launch;
  document.getElementById('location').textContent = flight.location;
  document.getElementById('wind-direction').textContent = flight.wind_direction;
  document.getElementById('wind-speed').textContent = flight.wind_speed;
}

function updateAlt(timestamp, altitude) {
  var update = {
    x: [timestamp],
    y: [altitude],
  };
  Plotly.extendTraces('altChart', update, [0], timestamp.length);
}

function updateVelocity(timestamp, velocity, acceleration) {
  var update_0 = {
    x:  [timestamp],
    y:  [acceleration],
  };
  var update_1 = {
    x:  [timestamp],
    y:  [acceleration],
  };

  Plotly.extendTraces('velChart', update_0, [0], acceleration.length);
  Plotly.extendTraces('velChart', update_1, [1], acceleration.length);
}

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

  Plotly.extendTraces('controlChart', update_0, [0], altitude.length);
  Plotly.extendTraces('controlChart', update_1, [1], altitude.length);
}

function updateMap(longitude, latitude, altitude) {
  // TODO
}

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

function updateFlightStages(stage) {
  // TODO
}

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


// Populate Search Bar
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

function flightRun(id_flight) {
  deleteSequence();
  search = document.getElementById('flight-search')
  if (search) {
    var option = search.options[search.selectedIndex];
    id_flight = option.value;

    try {
      fetchFlightData(id_flight).then(data => {      
        data.flight_data.forEach(entry => {
          setTimeout(() => { updateSequence(data.flight, entry); }, 1000);
        });
      });
    } catch (error) {
      console.error('Failed to fetch data:', error);
    }
  }
}

function flightDisplay() {
  search = document.getElementById('flight-search')
  if (search) {
    deleteSequence();

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


// Fetch Data
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

async function fetchFlightData(id_flight) {
  try { 
    const url = new URL(window.location.href + 'get-flight-data');
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


// Event Handlers
function attachEventHandlers() { 
  // Search Bar Event Handler
  document.getElementById('flight-search').addEventListener('change', function(event) {
    flightDisplay();
  });  
}


// Mount Charts and Event Triggers
window.onload = function() {
    mapboxgl.accessToken = 'pk.eyJ1IjoiYWxleHBvc3RhIiwiYSI6ImNscW1sdzFyMDJjNDUyaW8weW9kc2luaWQifQ.bQ-1vhGoRIbowpy5_gNNFw';

    const map = new mapboxgl.Map({
      container: 'map',
      style: 'mapbox://styles/mapbox/satellite-v9', // Mapbox satellite style
      center: [0, 53], 
      zoom: 5, // Starting zoom level
      pitch: 45, // Angle for 3D perspective
      bearing: -10 // Rotation angle around the map center
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