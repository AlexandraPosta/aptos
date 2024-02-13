clear;
clc;
close all;

%% Setup Objects
nominal = Rocket;
uncertain = Rocket;

steadyState = SteadyStateConditions;

OR = OpenRocketData('openrocket_data/OpenRocket_data_31-03-23.csv');

controller = Controller;

%% Tune LQR and Create State Space Model of Nominal Plant

% Make changes to Q and R matrices
controller.Q = [1e-9; 2; 2; 2; 1; 1].*eye(6);
controller.R = 15.*[1;1;1;1].*eye(4);
controller.minControlSpeed = 50;

% Define velocity range for scheduling
velocities = linspace(30, 120, 50);

% Perform gain scheduling
for velocity = velocities
        
        % Calculate all aerodynamic coefficient
        nominal = nominal.calcAeroCoeff(velocity);
        
        % Tune LQR
        controller = controller.addGain(nominal, steadyState, velocity);

end

% State arbitrary mach number for uncertain parameters
uncertain.M = 0.3;
uncertain = uncertain.calcAeroCoeff(velocity);
uncertain.I_L = OR.I_L;
uncertain.I_R = OR.I_R;
uncertain.X_CG = OR.X_CG;
uncertain.time = OR.t;

% State which Simulink file to run (no file extension required) 
simFile = 'Time_Varying_Model_Robustness';

% State number of sims to run
numSims = 1;

% Initialise simulation object array
for j = 1:numSims
    simulations(j) = SimulationData;
end

% Run robustness simulations
for n = 1:numSims

    fprintf('---------------- \n');
    fprintf('Starting Simulation %i \n', n);

    % Calculate uncertain parameters
%     uncertain.d_body = createUncertainParameter(nominal.d_body, 0.003, '_');
%     uncertain.l_nose = createUncertainParameter(nominal.l_nose, 0.010, '_');
%     uncertain.l_body = createUncertainParameter(nominal.l_body, 0.010, '_');
%     uncertain.ss_fin = createUncertainParameter(nominal.ss_fin, 0.002, '_');
%     uncertain.Cr = createUncertainParameter(nominal.Cr, 0.002, '_');
%     uncertain.Ct = createUncertainParameter(nominal.Ct, 0.002, '_');
%     uncertain.X_CP_can = createUncertainParameter(nominal.X_CP_can, 0.010, '_');
%     uncertain.Y_CP_can = createUncertainParameter(nominal.Y_CP_can, 0.005, '_');
%     uncertain.Z_CP_can = createUncertainParameter(nominal.Z_CP_can, 0.005, '_');
%     uncertain.ss_can = createUncertainParameter(nominal.ss_can, 0.001, '_');
%     uncertain.Cr_can = createUncertainParameter(nominal.Cr_can, 0.001, '_');
%     uncertain.Ct_can = createUncertainParameter(nominal.Ct_can, 0.001, '_');
%     uncertain.CLa = createUncertainParameter(nominal.CLa, 50, '%');
% 
%     % Calculate uncertain mass moments of inertia
%     longDeviation = 0.20;
%     radDeviation = 0.20;
%     longScalingFactor = (rand(1) * 2 * longDeviation) - longDeviation + 1;      % random number between 0.8 and 1.2
%     radScalingFactor = (rand(1) * 2 * radDeviation) - radDeviation + 1;
%     uncertain.I_L = OR.I_L .* longScalingFactor;
%     uncertain.I_R = OR.I_R .* radScalingFactor;
%     uncertain.time = OR.t;
% 
%     % Calculate uncertain CG location
%     CGDeviation = 0.05;
%     CGVariation = (rand(1) * 2 * CGDeviation) - CGDeviation;
%     uncertain.X_CG = OR.X_CG + CGVariation;
% 
%     % Calculate parameters that are dependent on the uncertain parameters
%     uncertain = uncertain.calcParamFromUncertainty();

    % Setup new simulation entry 
    simulations(n).uncertain_obj = uncertain;
    simulations(n).simFile = simFile;

    % Create Simulation Input file
    simInput = Simulink.SimulationInput(simFile);

    % Run simulation
    simOutput = sim(simInput, 'ShowProgress', 'on');

    % Log data
    simulations(n).time = simOutput.tout;

    numExports = simOutput.yout.numElements; 

    for i = 1:numExports

        x = sprintf(simOutput.yout{i}.Name);

        % Save each exported variable in simData
        simulations(n).(x) = simOutput.yout{i}.Values.Data;

    end

    fprintf('Completed simulation %i \n \n', n);

end

% Plot results
figure();
hold on
for i = 1:numSims
    plot(simulations(i).time, -simulations(i).pitchAngle, '-k');
end
xlabel('Time Since Ignition (s)');
ylabel('Pitch Angle (deg)');
grid on