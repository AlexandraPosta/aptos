function uncertainParam = createUncertainParameter(nominalValue, deviation, uncertaintyType)

    if uncertaintyType ~= '%' & uncertaintyType ~= '_'

        error('Incorrect uncertainty type. Should be % or _ only.');
        return;

    elseif uncertaintyType == '%'

        upperBound = nominalValue + (deviation/100) * nominalValue;
        lowerBound = nominalValue - (deviation/100) * nominalValue;

    elseif uncertaintyType == '_'

        upperBound = nominalValue + deviation;
        lowerBound = nominalValue - deviation;

    end

    range = upperBound - lowerBound;

    variation = (range * rand(1)) - (0.5 * range);

    uncertainParam = nominalValue + variation;

    return;

end