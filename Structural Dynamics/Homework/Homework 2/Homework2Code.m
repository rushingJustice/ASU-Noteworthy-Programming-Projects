clc; clear all; clf
a = 3; % a = 0 for SFF only, a = 1 impulsive system, a = 2 for CFF, a = 3 for RS 

w = 3; % natural frequency
psy = 0.1;
u_0 = 5;
v_0 = 0;
P_0 = 10; % increases amplitude
beta = 0.5; % SFF only
omega = 1; % SFF only - driving freq. 
dt = 0.05;
t_f = 30;

w_D = w*sqrt(1-psy^2);
D = (1-beta.^2)^2+(2*psy*beta)^2;

% SFF only %
if a == 0;
u_s = 2*psy*beta*P_0/D;

for t = 0:dt:t_f
u_p = -2*psy*beta*P_0/D*cos(omega*t)+(1-beta.^2)/D*P_0*sin(omega*t);    
u_t = exp(-psy*w*t)*((u_0+u_s)*cos(w_D*t)+(v_0+psy*w*(u_0+u_s)-(1-beta.^2)/D*P_0*omega)*sin(w_D*t)) + u_p;
plot(t,u_t)
hold on
end
title('Sinusoidal forcing function')

% Sinusoidal forcing function and zero forcing function %

elseif a == 1;
 
        
t_0 = 2*pi/omega;
u_s = 2*psy*beta*P_0/D;

for t = 0:dt:t_0
u_p = -2*psy*beta*P_0/D*cos(omega*t)+(1-beta.^2)/D*P_0*sin(omega*t);    
u_t = exp(-psy*w*t)*((u_0+u_s)*cos(w_D*t)+(v_0+psy*w*(u_0+u_s)-(1-beta.^2)/D*P_0*omega)*sin(w_D*t)) + u_p;
plot(t,u_t)
hold on
end

u_0 = u_t;

 for t = t_0:dt:t_f      
    u_t = exp(-psy*w*(t-t_0))*(u_0*cos(w_D*(t-t_0))+(v_0+psy*w*u_0)/(w_D)*sin(w_D*(t-t_0)));
    plot(t,u_t)
    hold on
 end 
 
title('Impulsive System')
  

% Constant forcing function %
elseif a == 2;
    
u_s =  P_0/(w^2);
u_p = u_s;
    
for t = 0:dt:t_f
u_h = exp(-psy*w*t)*((u_0-u_s)*cos(w_D*t)+(v_0+psy*w*(u_0-u_s))/(w_D)*sin(w_D*t));
u_t = (u_h + u_p);

plot (t,u_t)
hold on
end
title('Constant Forcing Function')

% Response Spectrum %
elseif a == 3

wsteps = 50; % integer steps taken for omega, from 1 to 'n' %

D = (1-beta.^2)^2+(2*psy*beta)^2;
t_0 = pi/omega;
u_s = 2*psy*beta*P_0/D;

w = 1:wsteps;
for j = 1:wsteps
t = 0:dt:t_0;
w_D = w(j)*sqrt(1-psy^2);
u_p = -2*psy*beta*P_0/D*cos(omega*t)+(1-beta.^2)/D*P_0*sin(omega*t);    
u_t = exp(-psy*w(j)*t).*((u_0+u_s)*cos(w_D*t)+(v_0+psy*w(j)*(u_0+u_s)-(1-beta.^2)/D*P_0*omega)*sin(w_D*t)) + u_p;
ui(j,:) = u_t;


end


for j = 1:wsteps
t = t_0:dt:t_f;   
w_D = w(j)*sqrt(1-psy^2);
u_t1 = exp(-psy*w(j)*(t-t_0)).*(ui(w(j),ceil(t_0/dt))*cos(w_D*(t-t_0))+(v_0+psy*w(j)*u_0)/(w_D)*sin(w_D*(t-t_0)));
uii(j,:) = u_t1;
end

U_w = [ui uii];

Max_u = max(abs(U_w),[],2);


figure(1); clf(1); plot(Max_u);
xlabel('w (natural freq.)'); ylabel('Max Displacement'); title('Response Spectrum of Impulsive System');

end 

