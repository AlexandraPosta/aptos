classdef Controller 

    properties 
        
        A = zeros(6,6);
        B = zeros(6,4);
        C
        D
        Q = [1;1;1;10;10;10].*eye(6);
        R = [1;1;1;1].*100.*eye(4);
        K = zeros(4, 6, 0);
        velocities = zeros(0);
        diskMarginInput
        diskMarginOutput
        multiMarginInput
        multiMarginOutput
        multiMarginIO
        states = ['Roll', 'Pitch', 'Yaw', 'Roll Rate', 'Pitch Rate', 'Yaw Rate'];
        inputs = ['x1', 'x2', 'y1', 'y2'];
        minControlSpeed = 50;     % m/s
        

    end
    properties (Dependent)
        num_gains;
        num_inputs;
        num_outputs;
    end
    
    
    methods
        function a = get.num_gains(obj)
            a = size(obj.K,3);
        end
        function a = get.num_inputs(obj)
            a = size(obj.K, 2);
        end
        function a = get.num_outputs(obj)
            a = size(obj.K, 1);
        end
        function obj = buildAMat(obj, rocket_obj, steadyState_obj)
            
            r = rocket_obj;
            ss = steadyState_obj;

            % Build elements for A matrix
            a11 = 0;  
            a12 = 0;
            a13 = 0;
            a14 = 1;
            a15 = 0;
            a16 = 0;
            
            a21 = 0;
            a22 = 0;
            a23 = 0;
            a24 = 0;
            a25 = 1;
            a26 = 0;
            
            a31 = 0;
            a32 = 0;
            a33 = 0;
            a34 = 0;
            a35 = 0;
            a36 = 1;
            
            a41 = 0;
            a43 = 0;
            a42 = 0;
            a44 = -r.Cdp/r.I_R;
            a45 = 0; 
            a46 = 0; 
            
            a51 = 0;
            a52 = 0;
            a53 = 0;
            a54 = 0;
            a55 = -r.C2/r.I_L;
            a56 = 0;
            
            a61 = 0;
            a62 = 0;
            a63 = 0;
            a64 = 0;
            a65 = 0;
            a66 = -r.C2/r.I_L;
            
            % Form A matrix from individual elements
            obj.A = [a11,a12,a13,a14,a15,a16;a21,a22,a23,a24,a25,a26;a31,a32,a33,a34,a35,a36;a41,a42,a43,a44,a45,a46;a51,a52,a53,a54,a55,a56;a61,a62,a63,a64,a65,a66];

        end

        function obj = buildBMat(obj, rocket_obj, steadyState_obj)

            r = rocket_obj;
            ss = steadyState_obj;
            
            % Build elements for B matrix
            b11 = 0;
            b12 = 0;
            b13 = 0;
            b14 = 0;
            
            b21 = 0;
            b22 = 0;
            b23 = 0;
            b24 = 0;
            
            b31 = 0;
            b32 = 0;
            b33 = 0;
            b34 = 0;
            
            b41 = -(r.q*r.S_can*r.CLa*r.Z_CP_can)/r.I_R;
            b42 = -(r.q*r.S_can*r.CLa*r.Z_CP_can)/r.I_R;
            b43 = -(r.q*r.S_can*r.CLa*r.Y_CP_can)/r.I_R;
            b44 = -(r.q*r.S_can*r.CLa*r.Y_CP_can)/r.I_R;
            
            b51 = -(r.q*r.S_can*r.CLa*r.Xbar_CP_can)/r.I_L;
            b52 = (r.q*r.S_can*r.CLa*r.Xbar_CP_can)/r.I_L;
            b53 = 0;
            b54 = 0;
            
            b61 = 0;
            b62 = 0;
            b63 = -(r.q*r.S_can*r.CLa*r.Xbar_CP_can)/r.I_L;
            b64 = (r.q*r.S_can*r.CLa*r.Xbar_CP_can)/r.I_L; 
            
            % Form B matrix from individual elements
            obj.B = [b11,b12,b13,b14;b21,b22,b23,b24;b31,b32,b33,b34;b41,b42,b43,b44;b51,b52,b53,b54;b61,b62,b63,b64];

        end

        function K = tuneLQR(obj)
            
            K = lqr(obj.A, obj.B, obj.Q, obj.R);

        end

        function obj = addGain(obj, rocket_obj, steady_state_obj, velocity)
            % Calculate A and B matrices for state space
            obj = obj.buildAMat(rocket_obj, steady_state_obj);
            obj = obj.buildBMat(rocket_obj, steady_state_obj);
            % Calculate the new gain
            newK = obj.tuneLQR();
            % Add the new gain
            obj.K = cat(3, obj.K, newK);
            obj.velocities = [obj.velocities velocity];
        end

        function gainsToCPP(obj, filepath)
            lines = [];
            lines = [lines "{"];
            for i=1:obj.num_gains
                lines = [lines "    {"];
                for j = 1:obj.num_outputs
                    line = "        {";
                    
                    line = line  + regexprep(num2str(obj.K(j,:,i)),'\s+',',');
                    
                    %line = line(1:end-1);
                    line = line + "},";
                    lines = [lines line];
                end
                lines = [lines "    },"];
            end

            lines = [lines "}"];
            writelines(lines,filepath);
            
        end
        
        function interpolateToCPP(obj, filepath)
            
            a = zeros(4, 6);
            b = zeros(4, 6);
            c = zeros(4, 6);
            d = zeros(4, 6);
            mask = false(4, 6);
            for i = 1:4
                for j = 1:6
                    obj.K
                    
                    [fitted_values, gof] = fit(obj.velocities', squeeze(obj.K(i, j, :)), "exp2");
                    a(i, j) = fitted_values.a;
                    b(i, j) = fitted_values.b;
                    c(i, j) = fitted_values.c;
                    d(i, j) = fitted_values.d;
                    gof.rsquare
                    if gof.rsquare > 0.9
                        mask(i, j) = true;
                    end
                end
            end
            
            initial_lines = [];
            interp_lines = [];
            for i = 1:4
                for j = 1:6
                    
                    if mask(i, j)
                        initial_lines = [initial_lines sprintf("gain[ravelIndex(%d, %d)] = %f;", i-1, j-1, obj.K(i, j, 1))];
                        interp_lines = [interp_lines sprintf("gain[ravelIndex(%d, %d)] = %f*exp(%f*velocity) + %f*exp(%f*velocity);", i-1, j-1, a(i,j), b(i,j), c(i, j), d(i, j))];
                    else
                        initial_lines = [initial_lines sprintf("gain[ravelIndex(%d, %d)] = %f;", i-1, j-1, mean(obj.K(i, j, :)))];
                    end
                    

                end
            end

            writelines([initial_lines "" interp_lines] ,filepath);
            

        end

        function dispMargins(obj)
            
            % Print header text
            fprintf('\n Stability Margins \n \n');

            disp('  Varying...      GM Low     GM High     PM Low   PM High     DM');

            % Print dividing line
            divider = '-------------------------------------------------------------------';
            disp(divider);
            
            % Loop at a time input variations
            for i = 1:length(obj.diskMarginInput)
                
                gainMarginLow = obj.diskMarginInput(i).GainMargin(1);
                gainMarginHigh = obj.diskMarginInput(i).GainMargin(2);
                phaseMarginLow = obj.diskMarginInput(i).PhaseMargin(1);
                phaseMarginHigh = obj.diskMarginInput(i).PhaseMargin(2);
                diskMargin = obj.diskMarginInput(i).DiskMargin;

                % Clean up any high numbers near infinity
                if gainMarginHigh > 1000 

                    gainMarginHigh = Inf;

                end
                
                data = sprintf('    Input %i        %.2f        %.2f        %.2f    %.2f    %.2f',i,gainMarginLow, gainMarginHigh, phaseMarginLow, phaseMarginHigh, diskMargin);

                disp(data);

            end
    
            % Print dividing line
            disp(divider);

            % Multi input variations
            gainMarginLow = obj.multiMarginInput.GainMargin(1);
            gainMarginHigh = obj.multiMarginInput.GainMargin(2);
            phaseMarginLow = obj.multiMarginInput.PhaseMargin(1);
            phaseMarginHigh = obj.multiMarginInput.PhaseMargin(2);
            diskMargin = obj.multiMarginInput.DiskMargin;

            data = sprintf('    All inputs     %.2f        %.2f        %.2f    %.2f    %.2f',gainMarginLow, gainMarginHigh, phaseMarginLow, phaseMarginHigh, diskMargin);

            disp(data);

            % Print dividing line
            disp(divider);

            % Output variations
            for i = 1:length(obj.diskMarginOutput)
                
                gainMarginLow = obj.diskMarginOutput(i).GainMargin(1);
                gainMarginHigh = obj.diskMarginOutput(i).GainMargin(2);
                phaseMarginLow = obj.diskMarginOutput(i).PhaseMargin(1);
                phaseMarginHigh = obj.diskMarginOutput(i).PhaseMargin(2);
                diskMargin = obj.diskMarginOutput(i).DiskMargin;
                
                % Clean up any high numbers near infinity
                if gainMarginHigh > 1000 

                    gainMarginHigh = Inf;

                end

                data = sprintf('    State %i        %.2f        %.2f        %.2f    %.2f    %.2f',i,gainMarginLow, gainMarginHigh, phaseMarginLow, phaseMarginHigh, diskMargin);

                disp(data);

            end

            % Print dividing line
            disp(divider);

            % Multi input variations
            gainMarginLow = obj.multiMarginOutput.GainMargin(1);
            gainMarginHigh = obj.multiMarginOutput.GainMargin(2);
            phaseMarginLow = obj.multiMarginOutput.PhaseMargin(1);
            phaseMarginHigh = obj.multiMarginOutput.PhaseMargin(2);
            diskMargin = obj.multiMarginOutput.DiskMargin;

            data = sprintf('    All outputs    %.2f        %.2f        %.2f    %.2f    %.2f',gainMarginLow, gainMarginHigh, phaseMarginLow, phaseMarginHigh, diskMargin);

            disp(data);

            % Print dividing line
            disp(divider);

            % Multi input and output variations
            gainMarginLow = obj.multiMarginIO.GainMargin(1);
            gainMarginHigh = obj.multiMarginIO.GainMargin(2);
            phaseMarginLow = obj.multiMarginIO.PhaseMargin(1);
            phaseMarginHigh = obj.multiMarginIO.PhaseMargin(2);
            diskMargin = obj.multiMarginIO.DiskMargin;

            data = sprintf('    I & O          %.2f        %.2f        %.2f    %.2f    %.2f',gainMarginLow, gainMarginHigh, phaseMarginLow, phaseMarginHigh, diskMargin);

            disp(data);

            % Print dividing line
            disp(divider);

        end


    end


end