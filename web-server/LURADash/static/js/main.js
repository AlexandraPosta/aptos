// Update Dashboard Charts
function updateLaunch(chart_data) {
  // TODO
}

function updateAlt(chart_data) {
  var update = {
    x:  [chart_data.labels],
    y:  [chart_data.altitude]
  };
  Plotly.extendTraces('altChart', update, [0], chart_data.labels.length);
}

function updateVelocity(chart_data) {
  var update_0 = {
    x:  [chart_data.labels],
    y:  [chart_data.velocity]
  };
  var update_1 = {
    x:  [chart_data.labels],
    y:  [chart_data.acceleration]
  };

  Plotly.extendTraces('velChart', update_0, [0], chart_data.labels.length);
  Plotly.extendTraces('velChart', update_1, [1], chart_data.labels.length);
}

function updateMap(chart_data) {
  // TODO
}

function updateControl(chart_data) {
  var update_0 = {
    x:  [chart_data.longitude],
    y:  [chart_data.latitude],
    z:  [chart_data.altitude]
  };

  Plotly.extendTraces('controlChart', update_0, [0], chart_data.labels.length);
}

function updateStats(chart_data) {
  document.getElementById('pressure').textContent = chart_data.pressure;
  document.getElementById('temperature').textContent = chart_data.temperature;
  document.getElementById('humidity').textContent = chart_data.humidity;
  document.getElementById('sattelites').textContent = chart_data.sattelites;
  document.getElementById('velocity').textContent = chart_data.velocity[0];
  document.getElementById('acceleration').textContent = chart_data.acceleration[0];
  document.getElementById('flight-stage').textContent = chart_data.flight_stage;
  document.getElementById('battery').textContent = chart_data.battery;
  document.getElementById('nr-errors').textContent = chart_data.error.length;
}

function updateFlightStages(chart_data) {
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
      center: [1.5491, 53.8008], 
      zoom: 3, // Starting zoom level
      pitch: 45, // Angle for 3D perspective
      bearing: -10 // Rotation angle around the map center
    });

    map.on('load', function () {
        // Add control
    });

    var trace_2d_0 = {
      x: [],
      y: [],
      type: 'lines+markers'
    };

    var trace_2d_1 = {
      x: [],
      y: [],
      type: 'lines+markers'
    };

    var trace_3d = {
      x: [],
      y: [],
      z: [],
      type: 'scatter3d'
    };

    const layout_2d = {
      width: 580,  
      height: 250,
      margin: {
        l: 30,  
        r: 0, 
        b: 50,  
        t: 0,  
        pad: 0  
      },
      
      showlegend: true,
      plot_bgcolor: 'white', 
      paper_bgcolor: 'white',
    };

    const layout_3d = {
      width: 580,  
      height: 250,
      margin: {
        l: 50,  
        r: 0, 
        b: 50,  
        t: 0,
      },
      showlegend: true,
      paper_bgcolor: 'white',
    };

    Plotly.newPlot('altChart', [trace_2d_0], layout_2d);
    Plotly.newPlot('velChart', [trace_2d_0, trace_2d_1], layout_2d);
    Plotly.newPlot('controlChart', [trace_3d], layout_3d);

    fetchData();
};