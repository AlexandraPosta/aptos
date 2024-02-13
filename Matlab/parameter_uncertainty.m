%% HEADER

% Title     :   Parameter Uncertainty Analysis
% Author    :   Theo Youds
% Started   :   01/03/23
% Version   :   1.0
% Notes     :   Investigates the effect of parameter uncertainty on the
%               stability of the system.

% Clear current workspace
clear;

%% OBJECT SETUP

% State velocity
velocities = linspace(10,200,5);     % m/s

for velocity = velocities 

    [aptos, steadyState, OR, controller] = setupObjects('openrocket_data/OpenRocket_data_23-02-23.csv');

    fprintf('STARTING VELOCITY %.2f m/s \n', velocity)
    
    % Calculate rocket aerodynamic properties
    aptos = aptos.calcAeroCoeff(velocity);

    % Tune LQR
    controller = controller.addGain(aptos, steadyState, velocity);
    
    %% INTRODUCE UNCERTAINTY
    
    % Override uncertain parameters 
    aptos.I_R = ureal('I_R', aptos.I_R, 'Percentage', 50);
    aptos.I_L = ureal('I_L', aptos.I_L, 'Percentage', 50);
    aptos.X_CG = ureal('X_CG', aptos.X_CG, 'Percentage', 50);
    aptos.CLa = ureal('CLa', aptos.CLa, 'Percentage', 50);
    aptos.S_can = ureal('S_can', aptos.S_can, 'Percentage', 50);
    aptos.Xbar_CP_can = ureal('Xbar_CP_can', aptos.Xbar_CP_can, 'Percentage', 30);
    aptos.Y_CP_can = ureal('Y_CP_can', aptos.Y_CP_can, 'Percentage', 20);
    aptos.Z_CP_can = ureal('Z_CP_can', aptos.Z_CP_can, 'Percentage', 20);
    aptos.q = ureal('q', aptos.q, 'Percentage', 25);

    aptos = aptos.calcAeroCoeff(velocity);

    aptos.C1 = ureal('C1', aptos.C1.NominalValue, 'Percentage', 100);
    aptos.C2 = ureal('C1', aptos.C2.NominalValue, 'Percentage', 100);
    aptos.Cdp = ureal('Cdp', aptos.Cdp, 'Percentage', 100);
   
    
    %% FORM STATE SPACE
    
    r = aptos;
    steady = steadyState;
    
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
    a42 = ((r.q*r.S_can*r.CLa*r.Z_CP_can)/r.I_R)*(cos(steady.x_1)-cos(steady.x_2));
    a43 = ((r.q*r.S_can*r.CLa*r.Y_CP_can)/r.I_R)*(cos(steady.y_1)-cos(steady.y_2));
    a44 = -r.Cdp/r.I_R;
    a45 = 0; 
    a46 = 0; 
    
    a51 = 0;
    a52 = 0;
    a53 = 0;
    a54 = -((r.I_R-r.I_L)*steady.rBar)/r.I_L; 
    a55 = -r.C2/r.I_L;
    a56 = -((r.I_R-r.I_L)*steady.pBar)/r.I_L; 
    
    a61 = 0;
    a62 = 0;
    a63 = 0;
    a64 = -((r.I_L-r.I_R)*steady.qBar)/r.I_L;
    a65 = -((r.I_L-r.I_R)*steady.pBar)/r.I_L;
    a66 = -r.C2/r.I_L;
    
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
    
    b41 = (r.q*r.S_can*r.CLa*r.Z_CP_can*(-steady.alphaBar*sin(steady.x_1)+cos(steady.x_1)-steady.x_1*sin(steady.x_1)))/r.I_R;
    b42 = (r.q*r.S_can*r.CLa*r.Z_CP_can*(steady.alphaBar*sin(steady.x_2)+cos(steady.x_2)-steady.x_2*sin(steady.x_2)))/r.I_R;
    b43 = (r.q*r.S_can*r.CLa*r.Y_CP_can*(-steady.betaBar*sin(steady.y_1)+cos(steady.y_1)-steady.y_1*sin(steady.y_1)))/r.I_R;
    b44 = (r.q*r.S_can*r.CLa*r.Y_CP_can*(steady.betaBar*sin(steady.y_2)+cos(steady.y_2)-steady.y_2*sin(steady.y_2)))/r.I_R;
    
    b51 = ((r.q*r.S_can*r.CLa*r.Xbar_CP_can)/r.I_L)*(cos(steady.x_1)-steady.alphaBar*sin(steady.x_1)-steady.x_1*sin(steady.x_1));
    b52 = ((r.q*r.S_can*r.CLa*r.Xbar_CP_can)/r.I_L)*(-cos(steady.x_2)-steady.alphaBar*sin(steady.x_2)+steady.x_2*sin(steady.x_2));
    b53 = 0;
    b54 = 0;
    
    b61 = 0;
    b62 = 0;
    b63 = ((r.q*r.S_can*r.CLa*r.Xbar_CP_can)/r.I_L)*(cos(steady.y_1)-steady.betaBar*sin(steady.y_1)-steady.y_1*sin(steady.y_1));
    b64 = ((r.q*r.S_can*r.CLa*r.Xbar_CP_can)/r.I_L)*(-cos(steady.y_2)-steady.betaBar*sin(steady.y_2)+steady.y_2*sin(steady.y_2)); 
    
    
    % Form A and B matrices from individual elements
    controller.A = [a11,a12,a13,a14,a15,a16;a21,a22,a23,a24,a25,a26;a31,a32,a33,a34,a35,a36;a41,a42,a43,a44,a45,a46;a51,a52,a53,a54,a55,a56;a61,a62,a63,a64,a65,a66];
    controller.B = [b11,b12,b13,b14;b21,b22,b23,b24;b31,b32,b33,b34;b41,b42,b43,b44;b51,b52,b53,b54;b61,b62,b63,b64];
    controller.C = 1.*eye(6);
    controller.D = zeros(6,4);
    
    % Create the uncertain state space model
    sysUncertain = ss(controller.A, controller.B, controller.C, controller.D);

    %% MODEL UNCERTAIN BEHAVIOUR
    
    closedLoop = feedback(sysUncertain*controller.K, eye(6));
    
    options = robOptions('Display', 'on', 'Sensitivity', 'on');
    
    [stabilityMargin, worstPerturbation] = robstab(closedLoop, options);
    
    fprintf('\n \n')

    clear aptos
    clear controller

end

fprintf('COMPLETED ALL TEST CASES \n')



