%% HEADER

% Title     :   Motor Speed Analysis
% Author    :   Theo Youds
% Started   :   22/02/2023
% Version   :   1.0
% Notes     :   Investigates the effect of different motor speeds on system
%               performance.

% Clear current workspace
clear;

%% OBJECT SETUP

[aptos, steadyState, OR, controller] = setupObjects('openrocket_data/OpenRocket_data_23-02-23.csv');

simData = SimulationData;

%% GAIN SCHEDULING

% Make changes to Q and R matrices
controller.Q = [1;1;1;10;10;10].*eye(6);

% Define velocity range for scheduling
velocities = linspace(30, 160, 50);

% Perform gain scheduling
for velocity = velocities
        
        % Calculate all aerodynamic coefficient
        aptos = aptos.calcAeroCoeff(velocity);
        
        % Tune LQR
        controller = controller.addGain(aptos, steadyState, velocity);

end

%% RUN SIMULATIONS

% Define range of motor speeds
motorSpeeds = linspace(0.5, 1.16, 5);
windSpeed = 10;             % m/s
maxDeflection = 0;          % deg

for i = 1:length(motorSpeeds)

    motorSpeed = motorSpeeds(i);

    fprintf('STARTING SIMULATION %i \n', i);

    simData = simData.runSimulation('Time_Varying_Model_2022a', motorSpeed, windSpeed, maxDeflection);

    fprintf('COMPLETED SIMULATION %i \n \n', i);

end

fprintf('ALL SIMULATIONS COMPLETE \n \n')
fprintf('Results found for the following motor speeds \n')
for i = 1:length(motorSpeeds)
    fprintf('(%i) . %.3f s/60deg \n', i, motorSpeeds(i));
end
fprintf('\n');

%% PLOT PITCH ANGLE COMPARISON

% Pitch angles for different motor speeds
figure(1);
legText = [];
hold on
for i = 1:length(motorSpeeds)

    plot(simData.time(:,i), simData.pitchAngle(:,i));
    
    legEntry = sprintf('%.3f s/60deg', motorSpeeds(i));
    legText = cat(2, legText, string(legEntry));

end
legend(legText, 'Location', 'northwest');
title('Effect of Motor Speed on Pitch Angle');
xlabel('Time Since Rail Departure (s)');
ylabel('Pitch Angle (deg)');

