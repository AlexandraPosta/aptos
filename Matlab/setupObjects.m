function [rocket_obj, ss_obj, openRocket_obj, controller_obj] = setupObjects(OpenRocketFileName)

    rocket_obj = Rocket;

    ss_obj = SteadyStateConditions;

    openRocket_obj = OpenRocketData(OpenRocketFileName);

    controller_obj = Controller;

end