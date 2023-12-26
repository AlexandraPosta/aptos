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
};