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
function displayTable() {
    //TODO: Display table
}
//#region Table

//#region Search Bar
function dbTableSearch() {
    try {
        getAvailableTables().then(tables => {
            search = document.getElementById('db-table');
            tables.forEach(table => {
                option = document.createElement('option');
                option.id = table;
                option.value = table;
                option.text = table;
                search.add(option);        
          }); 
        });
    } catch (error) {
    console.error('Failed to fetch data:', error);
    }
}

function dbColumnSearch() {
    search = document.getElementById('db-table');
    if (search.selectedIndex != 0) {
        var option = search.options[search.selectedIndex];
        table = option.value;

        getTableColumns(table).then(columns => {
            current_option.columns = columns;
            search_columns = document.getElementById('db-column');
            columns.forEach(column => {
                option = document.createElement('option');
                option.id = column.name;
                option.value = column.name;
                option.text = column.name;
                search_columns.add(option);        
          }); 
        });
    }
}

function dbDisplay() {
    search_table = document.getElementById('db-table');
    search_columns = document.getElementById('db-column');

    if (search.selectedIndex != 0 && search_columns.selectedIndex != 0) {
        // Find the selected table and column
        var option = search.options[search.selectedIndex];
        table = option.value;
        var option = search_columns.options[search_columns.selectedIndex];
        column = option.value;

        // Generate query
        filter = document.getElementById('db-filter').value;
        query = `SELECT * FROM ${table} WHERE ${column}=${filter};`;
        queryDB(query).then(data => {
            displayTable(data);
        });
    }
}
//#endregion


//#region Fetch Data
async function getAvailableTables() {
    try { 
        const response = await fetch('/get-tables')
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
        const response = await fetch('/get-columns')
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        return data.columns;
    } catch (error) {
        console.error('Failed to fetch data:', error);
    }
}

async function queryDB(query) {
    try { 
        const response = await fetch('/get-table-data')
        if (!response.ok) {
          throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        return data;
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
        dbColumnSearch();
        displayTable();
    }); 

    document.getElementById('db-column').addEventListener('change', function(event) {   
        current_option.current_option = event.target.value;
        dbValueSearch();
    });

    document.getElementById('db-filter').addEventListener('change', function(event) {   
        current_option.filter = event.target.value;
        displayTable();
    });
};


// Mount Charts and Event Triggers
window.onload = function() {
    dbTableSearch();
    attachEventHandlers();
};