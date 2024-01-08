// Update Dashboard Charts
function updateLaunch(chart_data) {
  // TODO
}

function updateAlt(chart_data) {
  var update = {
    x:  [chart_data.labels],
    y:  [chart_data.altitude],
  };
  Plotly.extendTraces('altChart', update, [0], chart_data.labels.length);
}

function updateVelocity(chart_data) {
  var update_0 = {
    x:  [chart_data.labels],
    y:  [chart_data.velocity],
  };
  var update_1 = {
    x:  [chart_data.labels],
    y:  [chart_data.acceleration],
  };

  Plotly.extendTraces('velChart', update_0, [0], chart_data.labels.length);
  Plotly.extendTraces('velChart', update_1, [1], chart_data.labels.length);
}

function updateMap(chart_data) {
  // TODO
}

function updateControl(chart_data) {
  var update_0 = { // ideal
    x: [Array(chart_data.longitude.length).fill(chart_data.longitude[0])],
    y: [Array(chart_data.latitude.length).fill(chart_data.latitude[0])],
    z: [chart_data.altitude]
  };

  var update_1 = { // real
    x:  [chart_data.longitude],
    y:  [chart_data.latitude],
    z:  [chart_data.altitude]
  };

  Plotly.extendTraces('controlChart', update_0, [0], chart_data.labels.length);
  Plotly.extendTraces('controlChart', update_1, [1], chart_data.labels.length);
}

function updateStats(chart_data) {
  document.getElementById('pressure').textContent = chart_data.pressure;
  document.getElementById('temperature').textContent = chart_data.temperature[0];
  document.getElementById('humidity').textContent = chart_data.humidity;
  document.getElementById('sattelites').textContent = chart_data.sattelites;
  document.getElementById('velocity').textContent = chart_data.velocity[0];
  document.getElementById('acceleration').textContent = chart_data.acceleration[0];
  document.getElementById('flight-stage').textContent = chart_data.flight_stage;
  document.getElementById('battery').textContent = chart_data.battery;
  document.getElementById('nr-errors').textContent = chart_data.error.length;
}

function updateFlightStages(chart_data) {
  // TODO
}

function get_graph(graph_name, height, width, axis_name, traces_name) {
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


// Fetch Dashboard Data
async function fetchData() {
  try {
    fetch('/data')
      .then(response => response.json())
      .then(data => {
        updateLaunch(data);
        updateAlt(data);
        updateVelocity(data);
        updateControl(data);
        updateMap(data);
        updateStats(data);
        updateFlightStages(data);
      }); 
  } 
  catch (error) {
      console.error('Failed to fetch data:', error);
  }
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

    get_graph('altChart', 250, window.innerWidth * 0.35,  ['time', 'altitude'], ['altitude']);
    get_graph('velChart', 250, window.innerWidth * 0.35, ['time', 'parameters'], ['v', 'acc']);
    get_graph('controlChart', 250, window.innerWidth * 0.35, ['altitude', 'latitude', 'longitude'], ['ideal', 'real']);

    fetchData();
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