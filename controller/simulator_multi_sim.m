%% HEADER

% Title     :   Simulator (Multi Sim)
% Author    :   Theo Youds
% Started   :   23/02/2023
% Version   :   1.0
% Notes     :   Allows you to quickly run multiple parametric Simulink sims

% Clear current workspace and close figures
clear;
close all;

%% OBJECT SETUP

[aptos, steadyState, OR, controller] = setupObjects('openrocket_data/OpenRocket_data_31-03-23.csv');

simData = SimulationData;

%% GAIN SCHEDULING

% Make changes to Q and R matrices
controller.Q = [10;10;10;10;10;10].*eye(6);
controller.R = [1;1;1;1].*100.*eye(4);

% Define velocity range for scheduling
velocities = linspace(30, 160, 50);

% Perform gain scheduling
for velocity = velocities
        
        % Calculate all aerodynamic coefficient
        aptos = aptos.calcAeroCoeff(velocity);
        
        % Tune LQR
        controller = controller.addGain(aptos, steadyState, velocity);

end

%% PREPARE SIMULATION INPUT

% Define other parameters (comment out the parameter you want to vary)
%motorSpeed = toRadSec(1.16); 
windSpeed = 10;             % m/s
maxDeflection = deg2rad(13);

% Define ranges (set the fixed parameters to 0)
motorSpeeds = linspace(0.09, 0.16, 5);
windSpeeds = 0;             % linspace(0, 10, 5);
deflections = 0;            % linspace(0, 13, 5);

% State which parameter to vary
parameter = motorSpeeds;

% State which Simulink file to run (no file extension required) 
simData.simFile = 'Time_Varying_Model_2022a';

% Run the simulations in a loop
for i = 1:length(parameter)

    fprintf('STARTING SIMULATION %i \n', i);
    
    % Comment out the parameters you DONT want to vary
    motorSpeed = toRadSec(motorSpeeds(i));
    %windSpeed = windSpeeds(i);
    %maxDeflection = deg2rad(deflections(i));

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

    % Run simulation and save data to simOutput
    simOutput = sim(simInput, 'ShowProgress', 'on');            
    
    % Transfer relevant data to simData object
    simData = simData.logData(simOutput);
    disp(' - Data logged');

    fprintf('COMPLETED SIMULATION %i \n \n', i);

end

fprintf('ALL SIMULATIONS COMPLETED \n \n');

%% PLOT PITCH ANGLE COMPARISON

% Pitch angles for different motor speeds
figure(1);
legText = [];
hold on
for i = 1:length(parameter)

    plot(simData.time(:,i), simData.pitchAngle(:,i));
    
    % Write title and legend data according to which parameter was varied
    if parameter == motorSpeeds
        legEntry = sprintf('%.3f s/60deg', parameter(i));
        legText = cat(2, legText, string(legEntry));
        titleLine1 = 'Effect of Motor Speed on Pitch Angle';
        titleLine2 = sprintf('Wind: %i m/s, MaxDef: %.1f deg', windSpeed, rad2deg(maxDeflection));
    elseif parameter == windSpeeds
        legEntry = sprintf('%.1f m/s', parameter(i));
        legText = cat(2, legText, string(legEntry));
        titleLine1 = 'Effect of Wind Speed on Pitch Angle';
        titleLine2 = sprintf('Motor: %.2f s/60deg, MaxDef: %.1f deg', to60deg(motorSpeed), rad2deg(maxDeflection));
    elseif parameter == deflections
        legEntry = sprintf('%.1f deg', parameter(i));
        legText = cat(2, legText, string(legEntry));
        titleLine1 = 'Effect of Max Deflection on Pitch Angle';
        titleLine2 = sprintf('Motor: %.2f s/60deg, Wind: %i deg', to60deg(motorSpeed), windSpeed);
    end

end
legend(legText, 'Location', 'northwest');
titleText = {titleLine1, titleLine2};
title(titleText);
xlabel('Time Since Rail Departure (s)');
ylabel('Pitch Angle (deg)');










