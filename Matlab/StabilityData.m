classdef StabilityData

    properties

        diskMarginInput
        diskMarginOutput
        multiMarginInput
        multiMarginOutput
        multiMarginIO
        states = ['Roll', 'Pitch', 'Yaw', 'Roll Rate', 'Pitch Rate', 'Yaw Rate'];
        inputs = ['x1', 'x2', 'y1', 'y2'];

    end

    methods

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