/**
 * @organisation Leeds University Rocketry Organisation - LURA
 * @fileoverview This file contains functions used to ingest new data in the db.
 * @author A. Posta
*/

const flightStages = {
    0: "LAUNCHPAD",
    1: "ASCENT",
    2: "APOGEE",
    3: "DESCENT",
    4: "LANDING"
  }

function displaySuccess(message) {
    console.log('Success:', message)
    document.getElementById('loadingIndicator').style.display = 'none'
    document.getElementById('successIndicator').style.display = 'inline'
    document.getElementById('failIndicator').style.display = 'none'
}

function diplayError(error) {
    console.error('Error:', error)
    document.getElementById('loadingIndicator').style.display = 'none'
    document.getElementById('successIndicator').style.display = 'none'
    document.getElementById('failIndicator').style.display = 'inline'
}

function parseCSV(text) {
    const lines = text.split('\n');
    const result = [];
    const headers = lines[0].split(',');

    for (let i = 1; i < lines.length; i++) {
        const obj = {};
        const currentline = lines[i].split(',');

        if (currentline.length === headers.length) {
            headers.forEach((header, j) => {
                obj[header] = currentline[j];
            });
            result.push(obj);
        }
    }
    return result;
}

/**
 * Attach windows.onload DOM Events
 * @return {void}     None
 */
function attachEventHandlers() { 
    document.getElementById('rocket-data-form').addEventListener('submit', function(event) {
        event.preventDefault()

        // Show loading indicator
        document.getElementById('loadingIndicator').style.display = 'inline'

        // Collect all non-file form data
        const formData = new FormData(event.target);
        const jsonData = {};
        formData.forEach((value, key) => {
            if (key !== 'csvFile') { // Skip file input
                if (key == 'active_control') {
                    jsonData[key] = value === 'on' ? 1 : 0;
                } else if (key == 'raw_data') {
                    jsonData[key] = value === 'on' ? 1 : 0;
                } else {
                    jsonData[key] = value;
                }
            }
            if (key == 'flight_stage') {
                jsonData[key] = flightStages[value]
            }
        });

        // Process CSV file if uploaded
        const fileInput = document.getElementById('csvFile');
        if (fileInput.files.length > 0) {
            const file = fileInput.files[0];
            console.log("Processing CSV file:", file.name);
            
            // Construct file reader
            const reader = new FileReader();
            reader.onload = function(e) {
                const text = e.target.result;
                const data = parseCSV(text);
                jsonData.csvData = data;
                // Send data to the backend
                fetch('http://localhost:5000/add-data/upload', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify(jsonData)
                })
                .then(response => response.json())
                .then(data => displaySuccess(data.message))
                .catch((error) => diplayError(error));
            };
            reader.readAsText(file);

        } else {
            diplayError("No CSV file uploaded.")
        }
    });
}

window.onload = function() {
    attachEventHandlers();
};