classdef Rocket

    properties
        
        I_R
        I_L
        I
        d_body
        l_body
        A_plan_body
        l_nose
        l_rocket
        V_nose
        A_plan_nose
        A_ref
        Cr
        Ct
        ss_fin
        rt
        sumTerm
        G_c_fin
        S_fin
        yMAC
        X_CG
        X_T
        v
        M
        q
        rho = 1.2256;       % sea level
        K_const = 1.1;
        CNa_rocket
        CNa_noCan
        X_CP
        X_CP_noCan
        CNa_nose
        CP_nose
        CNa_body
        CP_body
        CNa_fins
        CP_fins
        N
        CNa_1
        CNa0
        X_CP_can
        Z_CP_can
        Y_CP_can
        Xbar_CP_can
        CNa_can
        CLa
        ss_can
        Cr_can
        Ct_can
        S_can
        G_c_can
        C1
        C2
        Cdp
        Ccd
        railHeight = 6;     % m
        time                % for robustness analysis only 

    end

    methods
        
        % Constructor method to define rocket parameters
        function obj = Rocket()

            obj.I_R = 0.01657;                                  % Radial mass moment of inertia      

            obj.I_L = 3.02;                                     % Longitudinal mass moment of inertia
            
            obj.I = [obj.I_R;obj.I_L;obj.I_L].*eye(3);          % Moment of inertia matrix
            
            obj.d_body = 0.103;                                 % Body diameter                      (m)
            
            obj.l_body = 0.122 + 0.915 + 0.4 + 0.065 + 0.061;   % Body length                        (m)
            
            obj.A_plan_body = obj.d_body * obj.l_body;          % Body planform area                 (m^2)
            
            obj.l_nose = 0.507;                                 % Nose cone length                   (m)
            
            obj.l_rocket = obj.l_nose + obj.l_body;             % Total rocket length                (m)
            
            obj.V_nose = 2.226e-3;                              % Volume of revolution of ogive      (m^3)
            
            obj.A_plan_nose = 0.0349;                           % 2 x integral of ogive nose formula (m^2)
            
            obj.A_ref = pi()*(obj.d_body/2)^2;                  % Reference area                     (m^2)
            
            obj.Cr = 0.12;                                      % Fin root chord length              (m)
            
            obj.Ct = 0.065;                                     % Fin tip chord length               (m)
            
            obj.ss_fin = 0.14;                                  % Fin semi-span                      (m)
            
            obj.rt = obj.d_body/2;                              % Body radius at fin position        (m)
            
            obj.sumTerm = calcSumTerm(obj);                     % For roll damping 
    
            obj.G_c_fin = atan((obj.Cr-obj.Ct)/(2*obj.ss_fin)); % Midchord sweep angle               (rad)
            
            obj.S_fin = 0.5*obj.ss_fin*(obj.Cr+obj.Ct);         % Fin planform area                  (m^2)
            
            obj.yMAC = ((1/obj.S_fin)*((23/2)*(obj.ss_fin^2)-4*(obj.ss_fin^3)))/100;  
            
            obj.X_CG = 1.33;                                    % Rocket CG from nose tip            (m)
            
            obj.X_T = obj.l_rocket;                             % Nozzle throat from nose tip        (m)

            obj.CNa_rocket = 18.5;                              % From OpenRocket assuming AoA=0

            obj.CNa_noCan = 14.5;                               % From OpenRocket assuming AoA=0

            obj.X_CP = 1.50;                                    % CP from OpenRocket, AoA = 0
            
            obj.X_CP_noCan = 1.75;                              % Same but without canards

            obj.CNa_nose = 2;                                   % From OpenRocket, assuming AoA=0

            obj.CP_nose = 0.236;                                % From OpenRocket

            obj.CNa_body = 0;                                   % From Simulink assuming AoA=0

            obj.CP_body = obj.l_nose+(obj.l_body/2);            % From OpenRocket

            obj.CNa_fins = 13.1;                                % From OpenRocket assuming AoA=0

            obj.CP_fins = 2.00;                                 % From OpenRocket

            obj.N = 4;                                          % Number of rear fins

            obj.CNa_can = 4.0;                                  % From Simulink assuming AoA=0

            obj.CLa = 2.86;                                     % canard lift curve slope (/rad)

            obj.ss_can = 0.07;                                  % canard semi-span (m)
 
            obj.Cr_can = 0.06;                                  % canard root chord length (m)

            obj.Ct_can = 0.05;                                  % canard tip chord length (m)

            obj.S_can = 0.5*obj.ss_can*(obj.Cr_can+obj.Ct_can);             % Canard planform area (m^2)

            obj.G_c_can = atan((obj.Cr_can-obj.Ct_can)/(2*obj.ss_can));     % canard midchord sweep angle

            obj.X_CP_can = 0.589;                               % CP of canard from nose cone (m)

            obj.Y_CP_can = (obj.d_body/2)+(obj.ss_can/2);       % CP of canard from body centre (m)

            obj.Z_CP_can = (obj.d_body/2)+(obj.ss_can/2);       % CP of canard from body centre (m)

            obj.Xbar_CP_can = obj.X_CP_can - obj.X_CG; 


        end
        
        % Calculate the sum term needed for roll damping
        function sumTerm = calcSumTerm(obj)

            Cr = obj.Cr;
            Ct = obj.Ct;
            ss_fin = obj.ss_fin;
            rt = obj.rt;

            sumTerm = ((Cr+Ct)/2)*(rt^2)*ss_fin+((Cr+2*Ct)/3)*rt*(ss_fin^2)+((Cr+3*Ct)/12)*(ss_fin^3);

        end

        % Dynamic pressure
        function q = calcDynamicPressure(obj)

            q = 0.5 * obj.rho * obj.v^2;

        end

        % Calculate all aerodynamic coefficients
        function obj = calcAeroCoeff(obj, v)
            
            obj.v = v;                                          % Velocity
            obj.M = obj.v / 343;                                % Mach number
            obj.q = calcDynamicPressure(obj);                   % Dynamic pressure
            obj.CNa_1 = calcCNa_1(obj);  
            obj.CNa0 = (2*pi)/sqrt(1-obj.M^2);

            obj.C1 = calcCorrCoeff(obj);
            obj.C2 = calcDampCoeff(obj);
            obj.Cdp = calcRollDampCoeff(obj);
            obj.Ccd = calcRollForceCoeff(obj);

        end
        
        % Corrective moment coefficient with no canards
        function C1 = calcCorrCoeff(obj)

            C1 = obj.q*obj.A_ref*obj.CNa_noCan*(obj.X_CP_noCan-obj.X_CG);

        end

        % Damping moment coefficient (no jet damping)
        function C2 = calcDampCoeff(obj)
            
            C2_sumTerm = obj.CNa_nose*(obj.CP_nose-obj.X_CG)^2+obj.CNa_body*(obj.CP_body-obj.X_CG)^2+obj.CNa_can*(obj.X_CP_can-obj.X_CG)^2+obj.CNa_fins*(obj.CP_fins-obj.X_CG)^2;
            
            C2 = 0.5*obj.rho*obj.A_ref*obj.v*C2_sumTerm;

        end

        % Roll damping coefficient
        function Cdp = calcRollDampCoeff(obj)

            Cdp = 0.5*obj.rho*obj.v*obj.A_ref*obj.d_body*obj.N*obj.CNa0*obj.sumTerm;

        end

        % Roll forcing coefficient due to fin cant
        function Ccd = calcRollForceCoeff(obj)

            Ccd = obj.q*obj.A_ref*obj.N*(obj.yMAC+obj.rt)*(obj.CNa_1);

        end

        % CNa of single fin
        function CNa_1 = calcCNa_1(obj)

            CNa_1 = (2*3.142*((obj.ss_fin^2)/obj.A_ref))/(1+sqrt(1+((obj.ss_fin^2)*sqrt(1-obj.M^2))/(obj.S_fin*cos(obj.G_c_fin))));

        end

        % Calculate parameters from uncertain values (robustness analysis only)
        function obj = calcParamFromUncertainty(obj)
            
            obj.A_plan_body = obj.d_body * obj.l_body;          % Body planform area                 (m^2)
            
            obj.l_rocket = obj.l_nose + obj.l_body;             % Total rocket length                (m)
            
            obj.A_ref = pi()*(obj.d_body/2)^2;                  % Reference area                     (m^2)
            
            obj.rt = obj.d_body/2;                              % Body radius at fin position        (m)
            
            obj.sumTerm = calcSumTerm(obj);                     % For roll damping 
    
            obj.G_c_fin = atan((obj.Cr-obj.Ct)/(2*obj.ss_fin)); % Midchord sweep angle               (rad)
            
            obj.S_fin = 0.5*obj.ss_fin*(obj.Cr+obj.Ct);         % Fin planform area                  (m^2)
            
            obj.yMAC = ((1/obj.S_fin)*((23/2)*(obj.ss_fin^2)-4*(obj.ss_fin^3)))/100;  
            
            obj.X_T = obj.l_rocket;                             % Nozzle throat from nose tip        (m)

            obj.CP_body = obj.l_nose+(obj.l_body/2);            % From OpenRocket

            obj.S_can = 0.5*obj.ss_can*(obj.Cr_can+obj.Ct_can);             % Canard planform area (m^2)

            obj.G_c_can = atan((obj.Cr_can-obj.Ct_can)/(2*obj.ss_can));     % canard midchord sweep angle

            obj.CNa_1 = calcCNa_1(obj);  
            
            obj.CNa0 = (2*pi)/sqrt(1-obj.M^2);

        end

    end

end