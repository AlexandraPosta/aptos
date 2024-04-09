%% HEADER

% Title     :   Attitude Visualiser
% Author    :   Theo Youds
% Started   :   11/03/2023
% Version   :   1.0
% Notes     :   Visualises the airspeed as a 3D vector 

% Clear current workspace and close figures
clear;
close all;

%% OBJECT SETUP

[aptos, steadyState, OR, controller] = setupObjects('openrocket_data/OpenRocket_data_23-02-23.csv');

simData = SimulationData;

%% GAIN SCHEDULING

% Make changes to Q and R matrices
controller.Q = [10;10;10;10;10;10].*eye(6);
controller.R = [1;1;1;1].*eye(4);

% Define velocity range for scheduling
velocities = linspace(30, 160, 50);

% Perform gain scheduling
for velocity = velocities
        
        % Calculate all aerodynamic coefficient
        aptos = aptos.calcAeroCoeff(velocity);
        
        % Tune LQR
        controller = controller.addGain(aptos, steadyState, velocity);

end

%% PREPARE SIMULATION INPUTx = 

% Define parameters
motorSpeed = toRadSec(0.16); 
windSpeed = 8;             % m/s
maxDeflection = deg2rad(13);
windDirection = 0;

% State which Simulink file to run (no file extension required) 
simData.simFile = 'Time_Varying_Model_2022a';

% Log current simulation parameters
simData = simData.logParam(motorSpeed, windSpeed, maxDeflection);
disp(' - Parameters logged');

% Create Simulation Input file
simInput = Simulink.SimulationInput(simData.simFile);

% Edit max deflection
simInput = changeParam(simInput, simData, 'Max Deflection', 'UpperLimit', maxDeflection);
simInput = changeParam(simInput, simData, 'Max Deflection', 'LowerLimit', -maxDeflection);
% Edit motor speed
simInput = changeParam(simInput, simData, 'Motor Speed', 'RisingSlewLimit', motorSpeed);
simInput = changeParam(simInput, simData, 'Motor Speed', 'FallingSlewLimit', -motorSpeed);
% Edit wind speed
simInput = changeParam(simInput, simData, 'Windspeed', 'Value', windSpeed);
% Edit wind direction
simInput = changeParam(simInput, simData, 'Wind direction', 'Value', windDirection);

%% RUN SIMULATION

% Run simulation and save data to simOutput
simOutput = sim(simInput, 'ShowProgress', 'on');            

% Transfer relevant data to simData object
simData = simData.logData(simOutput);
disp(' - Data logged');

fprintf('COMPLETED SIMULATION \n \n');

%% PLOT DATA

N = length(simData.time);

x = simData.airspeed_x(1,1,:);
y = simData.airspeed_y;
z = simData.airspeed_z;

axlim = max(x);

for i = 1:N
    quiver3(0, 0, 0, z(i), y(i), x(i)) ;
    xlim([0 axlim]);
    ylim([0 axlim]);
    zlim([0 axlim]);
    title(sprintf('t: %.3f - alpha: %.2f - beta: %.2f', simData.time(i), simData.alpha(i), simData.beta(i)));
    % view([0 0]);  % alpha
    view([90 0]);   % beta

    pause(0.0001);
end
