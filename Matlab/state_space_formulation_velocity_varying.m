%% HEADER

% Title     :   State Space Formulation
% Author    :   Theo Youds
% Started   :   14/12/2022
% Version   :   1.0
% Notes     :   Performs gain scheduling for the LQR. Uses OOP methods
%               for cleaner code.

% Clear current workspace
clear;

%% OBJECT SETUP

[aptos, steadyState, OR, controller] = setupObjects('openrocket_data/OpenRocket_data_23-02-23.csv');

controller.Q = [10;10;10;10;10;10].*eye(6);
controller.R = [1;1;1;1].*10.*eye(4);

motorSpeed = 1.16;
windSpeed = 10;
maxDeflection = 13;


%% GAIN SCHEDULING
velocities = linspace(30, 160, 50);
for velocity = velocities
    
    % Calculate all aerodynamic coefficient
    aptos = calcAeroCoeff(aptos,velocity);
    
    controller = controller.addGain(aptos, steadyState, velocity);

end
means = mean(controller.K, 3);
normalised = controller.K ./ means;

to_fit = squeeze(controller.K(1, 4, :));
controller.gainsToCPP("gains_cpp/gain_values.txt");
controller.interpolateToCPP("gains_cpp/interpolation.txt");