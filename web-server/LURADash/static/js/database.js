/**
 * @organisation Leeds University Rocketry Organisation - LURA
 * @fileoverview Database interacion from frontend.
 * @author A. Posta
*/

class CurrentOption {
    constructor() {
        this.table = undefined;
        this.columns = undefined;
        this.selected_column = undefined;
        this.filter = undefined;
    }
}

let current_option = new CurrentOption();

//#region Display Table
function dbDisplayTable(table_data) {
    // Remove old table
    if (document.getElementById('db-container').firstChild) {
        document.getElementById('db-container').removeChild(document.getElementById('db-container').firstChild);
    }

    // Create new table
    tbl = document.createElement('table');
    tbl_body = document.createElement('tbody');

    rows = table_data.length;
    cols = Object.keys(table_data[0]).length;

    // Apend Table Header
    tbl_head = document.createElement('thead');
    tbl_head_row = document.createElement('tr');
    tbl_head.appendChild(tbl_head_row);
    tbl.appendChild(tbl_head);
    for (let col = 0; col < cols; col++) {
        tbl_head_data = document.createElement('th');
        tbl_head_data.appendChild(document.createTextNode(Object.keys(table_data[0])[col]));
        tbl_head_row.appendChild(tbl_head_data);
    }

    // Append Table Data
    for (let row = 0; row < rows; row++) {
        const current_row = document.createElement("tr");
        for (let col = 0; col < cols; col++) {
          const current_cell = document.createElement("td");
          const current_text = document.createTextNode(table_data[row][Object.keys(table_data[0])[col]]);
          
          current_cell.appendChild(current_text);
          current_row.appendChild(current_cell);
        }
        tbl_body.appendChild(current_row);
    }
    tbl.appendChild(tbl_body);
    document.getElementById('db-container').appendChild(tbl);
}
//#region Table

//#region Search Bar
function dbTableSearch() {
    try {
        getAvailableTables().then(tables => {
            search = document.getElementById('db-table');
            if (tables) {
                tables.forEach(table => {
                    option = document.createElement('option');
                    option.id = table;
                    option.value = table;
                    option.text = table;
                    search.add(option);        
                }); 
            }
        });
    } catch (error) {
    console.error('Failed to fetch data:', error);
    }
}

function dbColumnSearch() {
    if (current_option.table) {
        getTableColumns(current_option.table).then(columns => {
            current_option.columns = columns;
            search_columns = document.getElementById('db-column');
            if (columns) {
                columns.forEach(column => {
                    option = document.createElement('option');
                    option.id = column.name;
                    option.value = column.name;
                    option.text = column.name;
                    search_columns.add(option);        
                });
            } 
        });
    }
}

function dbFilterSearch() {
    if (current_option.columns) {
        getColumnData(current_option.table, current_option.selected_column).then(data => {
            search_filter = document.getElementById('db-filter');

            if (data) {
                data.forEach(entry => {
                    option = document.createElement('option');
                    option.id = entry;
                    option.value = entry;
                    option.text = entry;
                    search_filter.add(option);        
                }); 
            }   
        });
    }
}
//#endregion


//#region Fetch Data
async function getAvailableTables() {
    try { 
        const response = await fetch('/get-db-tables')
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        return data.tables;
    } catch (error) {
        console.error('Failed to fetch data:', error);
    }
}

async function getTableColumns(table) {
    try {
        const url = new URL(window.location.href + '/get-db-columns');
        url.searchParams.append('table', table);

        const response = await fetch(url, {
            method: 'GET',
            headers: {'Content-Type': 'application/json'}
        });
        
        if (!response.ok) {
          throw new Error('HTTP error! status: ${response.status}');
        }
        const data = await response.json();
        return data.columns;
    } catch (error) {
        console.error('Failed to fetch data:', error);
    }
}

async function getColumnData(table, column) {
    try { 
        const url = new URL(window.location.href + '/get-db-column-data');
        url.searchParams.append('table', table);
        url.searchParams.append('column', column);

        const response = await fetch(url, {
            method: 'GET',
            headers: {'Content-Type': 'application/json'}
        });

        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        return data.data;
    } catch (error) {
        console.error('Failed to fetch data:', error);
    }
}

async function queryDB() {
    try { 
        const url = new URL(window.location.href + '/get-db-table-data');
        url.searchParams.append('table', current_option.table);
        url.searchParams.append('column', current_option.selected_column);
        url.searchParams.append('filter', current_option.filter);

        const response = await fetch(url, {
            method: 'GET',
            headers: {'Content-Type': 'application/json'}
        });

        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        dbDisplayTable(data.data);
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
    document.getElementById('db-table').addEventListener('change', function(event) {
        current_option.table = event.target.value;
        document.getElementById('db-column').options.length = 1;
        document.getElementById('db-filter').options.length = 1;
        dbColumnSearch();
        queryDB();
    }); 

    document.getElementById('db-column').addEventListener('change', function(event) {   
        current_option.selected_column = event.target.value;
        document.getElementById('db-filter').options.length = 1;
        dbFilterSearch();
    });

    document.getElementById('db-filter').addEventListener('change', function(event) {   
        current_option.filter = event.target.value;
        queryDB();
    });
};


// Mount Charts and Event Triggers
window.onload = function() {
    dbTableSearch();
    attachEventHandlers();
};