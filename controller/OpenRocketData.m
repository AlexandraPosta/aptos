 classdef OpenRocketData

    properties
        
        t
        v
        h
        mass
        I_L
        I_R
        X_CG
        M
        mDot
        railHeight = 6 

    end

    methods
        
        % Fill object properties with data from OpenRocket csv
        function obj = OpenRocketData(fileName)
            
            % Read in the OR data csv
            data = readmatrix(fileName, 'NumHeaderLines',1);

            % Find the index of where rocket clears the rail
            idx = find(data(:,2) >= obj.railHeight, 1);

            % Only load in data for after the rocket has left the rail
            obj.t = data(idx:end,1) - data(idx,1);
            obj.h = data(idx:end,2);
            obj.v = data(idx:end,3); 
            obj.mass = data(idx:end,4);
            obj.I_L = data(idx:end,5);
            obj.I_R = data(idx:end,6);
            obj.X_CG = data(idx:end,7);
            obj.M = data(idx:end,8);  

            % Calculate mass derivative
            for i = 1:length(obj.t)
                
                if i == length(obj.t)

                    obj.mDot(i) = 0;

                else

                    dm = obj.mass(i+1) - obj.mass(i);
                    dt = obj.t(i+1) - obj.t(i);
    
                    obj.mDot(i) = dm/dt;

                end

            end

        end
    
    end


end