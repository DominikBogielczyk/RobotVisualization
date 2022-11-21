clear;

% simulation parameters
Tp = 0.015;
Tmax = 200;
N = round(Tmax/Tp);
t = 0:Tp:(N-1)*Tp;

% PID parameters
k = 5.7;
T = 0.27;
tau = 0.3;
Kp = 1.2*T/(tau*k);
Ti = 2*tau;
Td = 0.1;
dt = 0.015;


pre_err_s = 0;
pre_int_s = 0;
u_s = 0;
err_s = 0;

pre_err_rot = 0;
pre_int_rot = 0;
u_rot = 0;
err_rot = 0;


% Robot parameters
a0 = 0.0246;
a1 = 1.668;
b0 = 0.0;
b1 = -0.7021;
b2 = 1;
L =10;
r = 9;

% start position and rotation of robot
x0 = 50;
y0 = 50;
rot0 = 0;
% vectors of robot position, rotation, velocities and voltages parameters
x = zeros(N,1);
y = zeros(N,1);
rot = zeros(N,1);
u_left = zeros(N,1);
u_right = zeros(N,1);
w_left = zeros(N,1);
w_right = zeros(N,1);
lin_vel = zeros(N,1);
ang_vel = zeros(N,1);
x(1) = x0;
x(2) = x0;
y(1) = y0;
y(2) = y0;
rot(1) = rot0;
rot(2) = rot0;

% vector where PIDs outputs will be write
w_s = zeros(N,1);
w_rot = zeros(N,1);

% destination position of robot
xref = -120;
yref = -70;

% vector where errors will be write
error_s = zeros(N,1);
error_rot = zeros(N,1);

for n=3:N
% calculate errors for rotation and position of robot
error_rot(n) = atan2(yref-y(n-1),xref-x(n-1))-rot(n-1);
error_s(n) = sqrt((yref-y(n-1))^2+(xref-x(n-1))^2) * cos(error_rot(n));

% PID for position error
P_s = Kp * error_s(n);

integral_s = pre_int_s + error_s(n) * dt;
pre_int_s = integral_s;
I_s = (Kp/Ti)*integral_s;

derivative_s = (error_s(n) - pre_err_s)/dt;
pre_err_s = error_s(n);
D_s = Kp * Td * derivative_s;

w_s(n) = P_s + I_s +D_s ;

% PID for rotation error
P_rot = Kp * error_rot(n);

integral_rot = pre_int_rot + error_rot(n) * dt;
pre_int_rot = integral_rot;
I_rot = (Kp/Ti)*integral_rot;

derivative_rot = (error_rot(n) - pre_err_rot)/dt;
pre_err_rot = error_rot(n);
D_rot = Kp * Td * derivative_rot;

w_rot(n) = P_rot + I_rot + D_rot; 

% convertion from PIDs outputs to left and right wheels input
u_left(n) = w_s(n) - w_rot(n);
u_right(n) = w_s(n) + w_rot(n);

if(u_left(n)> 24)
    u_left(n) = 24;
end
if (u_left(n)<-24)
    u_left(n) = -24;
end

if(u_right(n)> 24)
    u_right(n) = 24;
end
if (u_right(n)<-24)
    u_right(n) = -24;
end
    
% calculate angular velocities of wheels
w_left(n) = (a1 * u_left(n-1) + a0 * u_left(n-2) - b1 * w_left(n-1) - b0 * w_left(n-2)) / b2;
w_right(n) = (a1 * u_right(n-1) + a0 * u_right(n-2) - b1 * w_right(n-1) - b0 * w_right(n-2)) / b2;

% calculations of linear and angular velocities of robot
lin_vel(n) = (w_right(n)+w_left(n))*r/2;
ang_vel(n) = (w_right(n)-w_left(n))*r/L;

% calculations of position and rotation of robot
rot(n) = rot(n-1)+ang_vel(n)*Tp;
x(n) = x(n-1) + Tp * lin_vel(n)*cos(rot(n));
y(n) = y(n-1) + Tp * lin_vel(n)*sin(rot(n));
end