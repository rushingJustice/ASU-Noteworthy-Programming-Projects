% MICHAEL JUSTICE (C) %

clc; clear all; clf 
a = 2; % a = 1, Coulomb friction damping
       % a = 2, viscous damping

% Parameters %
u_0 = 4.760;
mu = 0.35;
g = 9.81;
k = 50;
w = 2.3; % shortens wavelengths / larger magnitude
psy = 0.07; % decreases amplitude
v_0 = 0; % increase initial disp.

% Non parameters %
m = (k)/(w^2);
Fd = m*g*mu;
w_D = w*sqrt(1-psy^2)

% Start of damping models %

% Coulomb friction damping %
if a == 1
    c = 2.5; % Number of cycles (half or whole numbers)
    for n = 0:1:c*2
        
    t = n*pi/w:0.01:(n+0.99)*pi/w;
    
        if mod(n, 2) == 0
            sgn = 1; % n is even 
        else
            sgn = -1; % n is odd
        end
    u_t = (u_0 - (1+2*n)*Fd/k)*cos(w*t) + (sgn)*Fd/k    
    
    plot(t,u_t)
    hold on

    end  

    % Viscous damping (no forcing function) %
elseif a == 2
    for t = 0:0.01:25 
        
    u_t = exp(-psy*w*t)*(u_0*cos(w_D*t)+(v_0+psy*w*u_0)/(w_D)*sin(w_D*t));
    plot(t,u_t)
    hold on
    
    end 

end