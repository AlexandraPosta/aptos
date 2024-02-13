clear;

load 'robustness_simulation_data.mat' simulations

numSims = length(simulations);

figure();
hold on
for i = 1:numSims
    plot(simulations(i).time, simulations(i).pitchAngle, '-k');
end