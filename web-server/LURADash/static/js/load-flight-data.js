/**
 * Worker file to load flight data without blocking the UI main thread.
 * It received the flight data as a message from the main script and 
 * sends it back to be processed every 3 seconds.
 * @return {void}     None
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