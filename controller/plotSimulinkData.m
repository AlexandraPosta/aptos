function plotSimulinkData(simData_obj, n)

    % Check selection is valid
    if isInt(n) && (0 < n) && (n <= size(simData_obj.time, 2))
    
        % Pitch angle and canard deflection for chosen motor speed
        figure(2);
        subplot(2,1,1);                             % attitude
        hold on
        plot(simData_obj.time(:,n), simData_obj.pitchAngle(:,n));   % pitch
        plot(simData_obj.time(:,n), simData_obj.yawAngle(:,n));     % yaw
        legend('Pitch', 'Yaw');
        title(sprintf('Pitch Angle (%s)', simData_obj.name(n)));
        xlabel('Time Since Rail Departure (s)');
        ylabel('Pitch Angle (deg)');
        xlim([0 12]);
        grid on
        
        subplot(2,1,2);                             % canard deflections
        hold on
        plot(simData_obj.time(:,n), simData_obj.x1(:,n));
        plot(simData_obj.time(:,n), simData_obj.x2(:,n));
        plot(simData_obj.time(:,n), simData_obj.y1(:,n));
        plot(simData_obj.time(:,n), simData_obj.y2(:,n));
        hold off
        legend('x_1','x_2','y_1','y_2', 'Location', 'east');
        title(sprintf('Canard Deflections (%s)', simData_obj.name(n)));
        xlabel('Time Since Rail Departure (s)');
        ylabel('Canard Deflections (deg)');
        xlim([0 12]);
        ylim([-13 13])
        grid on
    
    else
        disp('Please choose a valid data set');
    end

end