/**
 * @organisation Leeds University Rocketry Organisation - LURA
 * @fileoverview This file contains the code for the worker that loads the flight data.
 * @author A. Posta
*/

self.addEventListener('message', function(e) {
    // Receive data from the main script
    let data = e.data;

    data.flight_data.forEach(entry => {
        setTimeout(() => { 
            self.postMessage([data.flight, entry]);
        }, 3000);
    });
});