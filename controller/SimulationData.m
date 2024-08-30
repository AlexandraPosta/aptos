classdef SimulationData

    properties
        
        name
        simFile
        motorSpeed      % rad/sec
        windSpeed       % m/s
        time            % s
        pitchAngle      % deg
        yawAngle        % deg
        x1              % deg
        x2              % deg
        y1              % deg
        y2              % deg
        v               % m/s
        airspeed_x      % m/s
        airspeed_y      % m/s
        airspeed_z      % m/s
        alpha           % deg
        beta            % deg
        simOutput       % Simulink.SimulationOutput object
        maxDeflection   % rad
        uncertain_obj   % Rocket object of uncertain parameters (for robustness)

    end

    methods

        function obj = logParam(obj, motorSpeed, windSpeed, maxDeflection)
        
            obj.motorSpeed = cat(2, obj.motorSpeed, motorSpeed);
            obj.windSpeed = cat(2, obj.windSpeed, windSpeed);
            obj.maxDeflection = cat(2, obj.maxDeflection, maxDeflection);
            
            % Name the simulation for plotting later
            formatSpec = "Motor: %.2f s/60deg, Wind: %i m/s, MaxDef: %.1f deg";
            titleText = sprintf(formatSpec, to60deg(motorSpeed), windSpeed, rad2deg(maxDeflection));
            obj.name = cat(2, obj.name, titleText);

        end

        function obj = logData(obj, data)
            
            obj.time = cat(2, obj.time, data.tout);

            numExports = data.yout.numElements;

            for i = 1:numExports
    
                x = sprintf(data.yout{i}.Name);

                % Save each exported variable in simData
                obj.(x) = cat(2, obj.(x), data.yout{i}.Values.Data);

            end

        end

        function obj = runSimulation(obj, simulationFile, motorSpeed, windSpeed, maxDeflection)
            
            % State which Simulink file to run
            obj.simFile = simulationFile;

            % Log current simulation parameters
            obj = obj.logParam(motorSpeed, windSpeed, maxDeflection);
            disp(' - Parameters logged');
            
            % Create Simulation Input file
            simInput = Simulink.SimulationInput(obj.simFile);
            
            % Run simulation and save data to simOutput
            obj.simOutput = sim(simInput, 'ShowProgress', 'on');            
            
            % Transfer relevant data to simData object
            obj = obj.logData(obj.simOutput);
            disp(' - Data logged');

        end


    end

end