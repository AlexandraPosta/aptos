%% HEADER

% Title     :   LQR Tuning Analyis
% Author    :   Theo Youds
% Started   :   22/02/2023
% Version   :   1.0
% Notes     :   For a given motor speed, test different LQR tunings

% Clear current workspace
clear;

%% OBJECT SETUP

[aptos, steadyState, OR, controller] = setupObjects('openrocket_data/OpenRocket_data_23-02-23.csv');

simData = SimulationData;

%% RUN SIMULATION

% Define parameters
motorSpeed = 1.16;          % s/60deg
windSpeed = 10;             % m/s

% Define velocity range for scheduling
velocities = linspace(30, 160, 50);

% Decide how many simulations to run
numSims = 4;

% Preallocate rateWeight vector for speed
rateWeight = zeros(1,numSims);

for i = 1:numSims
    
    % Define Q and R matrices
    rateWeight(i) = 10^(i-1);
    controller.Q = [1000; 10000; 10000;rateWeight(i);rateWeight(i);rateWeight(i)].*eye(6);
    controller.R = [1;1;1;1].*100.*eye(4);

    % Perform gain scheduling
    for velocity = velocities
        
        % Calculate all aerodynamic coefficient
        aptos = aptos.calcAeroCoeff(velocity);
        
        % Tune LQR
        controller = controller.addGain(aptos, steadyState, velocity);

    end

    fprintf('STARTING SIMULATION %i \n', i);

    % Run the simulation
    simData = simData.runSimulation('Time_Varying_Model_2022a', motorSpeed, windSpeed);

    fprintf('COMPLETED SIMULATION %i \n \n', i);

end

fprintf('ALL SIMULATIONS COMPLETE \n \n')

%% PLOT DATA

% Pitch angles for different motor speeds
figure(1);
legText = [];
hold on
for i = 1:numSims

    plot(simData.time(:,i), simData.pitchAngle(:,i));
    
    legEntry = sprintf('Weight: %i', rateWeight(i));
    legText = cat(2, legText, string(legEntry));

end
legend(legText, 'Location', 'northwest');
title('Effect of Rate Weighting on Pitch Angle');
xlabel('Time Since Rail Departure (s)');
ylabel('Pitch Angle (deg)');

