clear

% State PID gains
Kp = 550;
Ki = 1000;
Kd = 9000;

% Create PID transfer function
PID_tf = tf([Kd, Kp, Ki], [1, 0])

% Create servo motor transfer function
% servo_tf = tf([]);

% loop_tf = (servo_tf * PID_tf) / (1 - servo_tf * PID_tf);

    