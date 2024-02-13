function simIn_obj = changeParam(simIn_obj, simData_obj, block, param, value)
    
    blockPath = sprintf('%s/%s', simData_obj.simFile, block);

    valueStr = sprintf('%.2f', value);
    
    simIn_obj = simIn_obj.setBlockParameter(blockPath, param, valueStr); 

end