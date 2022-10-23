clear

s = tf('s');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% OBIEKT %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Kt = 0.062;
Kb = 0.062;
J = 0.0551;
B = 0.188;
Re = 0.56;
Le = 0.97 * 10^(-3);
n = 10;

Gw = n*Kt/((Le*s+Re)*(J*s+B)+Kt*Kb);

subplot(2,1,1)
step(Gw)

Tp = 0.1;
Tmax = 3;
N = round(Tmax/Tp);
t = 0:Tp:(N-1)*Tp;
sysd = c2d(Gw,Tp);

a1 = sysd.num{1}(2)
a0 = sysd.num{1}(3)
b2 = sysd.den{1}(1)
b1 = sysd.den{1}(2)
b0 = sysd.den{1}(3)

u = ones(1, N);
w(1) = 0;
w(2) = (a1*u(1) - b1 * w(1)) / b2;
for n=3:N
w(n) = (a1 * u(n-1) + a0 * u(n-2) - b1 * w(n-1) - b0 * w(n-2)) / b2;
end

subplot(2,1,2)
plot(t,w)
title('Dyskretny')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% REGULATOR %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
k = 5.7;
T = 0.27;
tau = 0.2;
Kp = 1.2*T/(tau*k);
Ti = 2*tau;
Td = 0;