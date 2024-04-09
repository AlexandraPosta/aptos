# Project Aptos – Control System Matlab/Simulink Setup Document
The Matlab project and all its files can be found on GitHub using the following link. Clone the repository to have a local version and check for changes regularly.

### Simplified Run
In order to run the Simulink file (Time_Varying_Model_2022a) ensure the following add-ons are installed: 
- Simulink v23.2
- Control System Toolbox v23.2
- Simscape v23.2
- Simscape Electrical v23.2
- Curve Fitting Toolbox v23.2
- Aerospace Blockset v23.2
- Aerospace Toolbox v23.2
- Robotics System Toolbox v23.2

To check what add-ons you have installed: 
1. open Simulink and select the ‘Get Add-ons’ button located under the ‘APPS’ tab
2. select the ‘Manage Add-ons’ button found in the top right-hand corner
3. the Add-ons that are installed and active will appear here
4. search for and install the missing Add-ons.

With the main Matlab window open, from the file directory select the ‘ProjectAptos24.prj’ file and open it. Opening this file will automatically run the simulation file and Simulink file. Graphs will appear on screen when the simulation file has completed and the scopes in the Simulink window can be viewed with results. This method has been implemented to make the setup and initial running of the simulation as easy as possible.


### Manual Run
If the above method fails, please see the following instructions on how to manually run the files.
In order to run the simulation, first navigate to and open the ‘simulation.m’ file. Ensure that line 15 reads:	‘aptos, steadyState, OR, controller = setupObjects(‘openrocket_data/OpenRocket_data_31-03-23.csv’)
Ensure that the current working directory is the folder containing the files for the project.
On the ‘Editor’ tab, press the play button and wait for the simulation to finish. When the simulation is finished a figure containing two graphs will appear.
Open the Time_Varying_Model_2022a and ensure that there are no missing variables or errors, these will be shown in red and obvious upon opening the file. Ensure that the ‘Stop Time’ found on the ‘Simulation’ tab is set to 12 seconds. Press the ‘Run’ button located next to the stop time. Once the model has finished running, all scopes will show information when double clicked.
Document Created by S.Bruton		Project Aptos
09/02/2024

