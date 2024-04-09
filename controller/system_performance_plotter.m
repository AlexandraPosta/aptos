close all

load 'withControl_windModel.mat' simData
withControl = simData;

load 'noControl_windModel.mat' simData
noControl = simData;

figure();

lw = 1.5;

subplot(3,1,1);
hold on
box on
plot(withControl.time, withControl.pitchAngle, '-', 'LineWidth', lw);
plot(noControl.time, noControl.pitchAngle, ':', 'LineWidth', lw);
xlabel('Time Since Rail Departure (s)');
ylabel('Pitch Angle (deg)');
legend('With Control', 'Free Response', 'Location','southoutside', 'Orientation','horizontal')
xlim([0 10.2]);

subplot(3,1,2);
hold on
box on
plot(withControl.time, withControl.yawAngle, '-', 'LineWidth', lw);
plot(noControl.time, noControl.yawAngle, ':', 'LineWidth', lw);
xlabel('Time Since Rail Departure (s)');
ylabel('Yaw Angle (deg)');
legend('With Control', 'Free Response', 'Location','southoutside', 'Orientation','horizontal')
xlim([0 10.2]);

subplot(3,1,3);
hold on
box on
plot(withControl.time, withControl.x1, '-', 'LineWidth', lw);
plot(withControl.time, withControl.x2, '-', 'LineWidth', lw);
plot(withControl.time, withControl.y1, '-', 'LineWidth', lw);
plot(withControl.time, withControl.y2, '-', 'LineWidth', lw);
xlabel('Time Since Rail Departure (s)');
ylabel('Canard Deflections (deg)');
legend('x_1', 'x_2', 'y_1', 'y_2', 'Location','southoutside', 'Orientation','horizontal');
ylim([-12 12]);
xline(0, 'HandleVisibility','off');
xlim([0 10.2]);
xline(7.87, 'Label', {'Active Control', 'Off'}, 'LabelVerticalAlignment','bottom', 'LabelOrientation','aligned', 'HandleVisibility','off', 'FontSize', 8, 'LineWidth', 1.2);