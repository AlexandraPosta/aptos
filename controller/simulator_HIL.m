%% HEADER

% Title     :   Simulator
% Author    :   Alexandra Posta
% Started   :   19/08/2023
% Version   :   1.0
% Notes     :   Allow you to run HIL using a STM32

% Clear current workspace and close figures
clear;
close all;

%% OBJECT SETUP

[aptos, steadyState, OR, controller] = setupObjects('openrocket_data/OpenRocket_data_31-03-23.csv');


simData = SimulationData;

%% GAIN SCHEDULING

% Make changes to Q and R matrices
controller.Q = [1e-9; 4; 4; 4; 1; 1].*eye(6);
controller.R = 5.*[1;1;1;1].*eye(4);
controller.minControlSpeed = 30;

% Define velocity range for scheduling
velocities = linspace(30, 120, 50);

% Perform gain scheduling
for velocity = velocities
        
        % Calculate all aerodynamic coefficient
        aptos = aptos.calcAeroCoeff(velocity);
        
        % Tune LQR
        controller = controller.addGain(aptos, steadyState, velocity);

end

%% PREPARE SIMULATION INPUT

% Define parameters
motorSpeed = toRadSec(0.32); 
windSpeed = 5;             % m/s
maxDeflection = deg2rad(0);

% State which Simulink file to run (no file extension required) 
simData.simFile = 'Time_Varying_Model_HIL';

% Log current simulation parameters
simData = simData.logParam(motorSpeed, windSpeed, maxDeflection);

% Create Simulation Input file
simInput = Simulink.SimulationInput(simData.simFile);

% Edit wind speed
simInput = changeParam(simInput, simData, 'Windspeed', 'Value', windSpeed);

disp(' - Parameters logged');

%% RUN SIMULATION

% Run simulation and save data to simOutput
simOutput = sim(simInput, 'ShowProgress', 'on');            

% Transfer relevant data to simData object
simData = simData.logData(simOutput);
disp(' - Data logged');

fprintf('COMPLETED SIMULATION \n \n');

%% PLOT DATA

plotSimulinkData(simData,1);

%% EXPORT GAINS

to_fit = squeeze(controller.K(1, 4, :));
controller.gainsToCPP("gains_cpp/gain_values.txt");
controller.interpolateToCPP("gains_cpp/interpolation.txt");
