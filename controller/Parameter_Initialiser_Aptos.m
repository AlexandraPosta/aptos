%% HEADER

%Title      :   Aptos Parameter Initialiser
%Author     :   Theo Youds
%Started    :   07-06-22
%Version    :   1.0
 
%Clear current workspace
clear

%% ROCKET PARAMETERS
    
I_R = 0.009705;                              %Radial mass moment of inertia      

I_L = 1.6042;                                %Longitudinal mass moment of inertia

I = [I_R;I_L;I_L].*eye(3);                  %Moment of inertia matrix

d_body = 0.102;                             %Body diameter                      (m)

l_body = 1.20;                              %Body length                        (m)

A_plan_body = d_body * l_body;              %Body planform area                 (m^2)

l_nose = 0.419;                             %Nose cone length                   (m)

l_rocket = l_nose + l_body;                 %Total rocket length                (m)

V_nose = 1.14e-3;                           %Cone estimate                      (m^3)

A_plan_nose = 0.5*d_body*l_nose;            %Assuming triangular planfrom area  (m^2)

A_ref = pi()*(d_body/2)^2;                  %Reference area                     (m^2)

Cr = 0.12;                                  %Fin root chord length              (m)

Ct = 0.065;                                 %Fin tip chord length               (m)

ss_fin = 0.08;                                   %Fin semi-span                      (m)

rt = d_body/2;                              %Body radius at fin position        (m)

sumTerm = ((Cr+Ct)/2)*(rt^2)*ss_fin+((Cr+2*Ct)/3)*rt*(ss_fin^2)+((Cr+3*Ct)/12)*(ss_fin^3); %For roll damping 

%G_c = atan(Cr/(4*s));                      %Midchord sweep angle               (rad)
G_c_fin = atan((Cr-Ct)/(2*ss_fin));

S_fin = 0.5*ss_fin*(Cr+Ct);                      %Fin planform area                  (m^2)

yMAC = ((1/S_fin)*((23/2)*(ss_fin^2)-4*(ss_fin^3)))/100;  

X_CG = 0.92;                                %Rocket CG from nose tip            (m)

X_T = l_rocket;                                  %Nozzle throat from nose tip        (m)

%% ENVIRONMENTAL PARAMETERS

K_const = 1.1;                %Body lift constant

L = 0.0065;             %Lapse rate                         (degC/m)

T0 = 288.15;            %Sea level temperature              (K)

g = 1.4;                %Ratio of specific heats

GasConst = 287.1;       %Gas constant                       (J/kg/K)


