clear all; clc;

% Parameters for Analytical and Classical Solution- %
w = 1; 
xi = 0.2;
u_0 = 5;
v_0 = 0;
% End of Parameters---------------------------------%

% Time Parameters-----------------------------------%
dt = 1.7;
tf = 30;
t0 = 0;
% End of Parameters---------------------------------%

% Numerical Analysis--------------------------------%
      nitems  = 2;               % total number of items stored in history
      nreport = 500;             % number of steps to output
      nsteps  = ceil((tf-t0)/dt) + 1; % total number of analysis time steps
      tf = nsteps*dt;
      nreport = min(nreport,nsteps);
      noutput = ceil(nsteps/nreport);
      history = zeros(nreport,nitems);
      npts = 200;
      h = (tf - t0)/(npts - 1);
      
      
% Constants for Analytical, Numerical and Classical Solution-- %

      a = (2 - w^2*dt^2)/(2*(1 + w*dt*xi));
      b = w*dt*sqrt(1 - (1/4)*w^2*dt^2 - xi^2)/(1 + w*dt*xi);
      r2 = a^2 + b^2;
      phi = acos(a/(sqrt(r2)));
      w_D = w*sqrt(1-xi^2);

      D = (1 + w*dt*xi)/(1 - w*dt*xi);
      B = (2 - dt^2*w^2)/(1 - w*dt*xi);
% End of Constants-------------------------------------------- %

% Initialize position at n=0 'xold' and n=-1 'xoldold'---------% 
      xold = u_0;
      xoldold = (2*dt*v_0 - u_0*B/D)/(-1*(1 + 1/D));
      
      %.... Set time and output counters
      t = t0; out = 0; iout = 0;
      
%.... Compute motion by numerical integration--loop over time steps
      for i=1:nsteps
          
%....... Write the values out to history, if appropriate
%        This segment writes a row to 'history' every 'noutput' steps
         if (out==0)
           iout = iout + 1;
           history(iout,:)=[t,xold];
           out = noutput;
         end
         out = out - 1;

%....... Compute the values of the state for the next time step         
         t = t + dt;
         
%....... Compute new position from equation of motion combined with
%        the difference equation for acceleration
         %xnew  = (2*xi*w*v_0 + w^2*xold)*(-dt^2) + 2*xold - xoldold;
         xnew = (xoldold*(w*dt*xi -1) + xold*(2 - w^2*dt^2))/(1+w*dt*xi);
         
%....... Put current state (new) into old slot and old slot into the 
%        oldold slot to get ready for new step. 
         xoldold = xold;
         xold    = xnew;
         
      end % loop over time steps

%.... Put very last point into the history array                
      iout = iout + 1;
      history(iout,:)=[t,xold];      

ucomp = zeros(3,npts);
t = t0;
for i=1:npts
ucomp(1,i) = t;
ucomp(2,i) = (r2)^(t/(2*dt))*(u_0*cos(phi*t/dt) + ((2*dt*v_0 + u_0*B)/(1+D)...
    - u_0*a)*(1/b)*sin(phi*t/dt));
ucomp(3,i) = exp(-xi*w*t)*(u_0*cos(w_D*t)+(v_0+xi*w*u_0)/(w_D)*sin(w_D*t));
t = t + h;
end

 numerical = 1;      % =1 if you want the numerical soluton (black)
 classical = 1;      % =1 if you want the classical solution (red)
 numanaly  = 0;      % =1 if you wnat the analytical solution (blue)
 
fig=figure(1); clf; grid on; axis square; hold on;
xlabel('t'); ylabel('Disp.'); title('Displacement vs. Time');

if (numanaly  == 1)
    p = plot(ucomp(1,:),ucomp(2,:));
    hold on; set(p,'Color','blue','LineWidth',2);
end
if (classical == 1)
    p1 = plot(ucomp(1,:),ucomp(3,:));
    hold on; set(p1,'Color','red','LineWidth',0.5);
end

if (numerical  == 1)
    p = plot(history(1:iout,1),history(1:iout,2),...
        'o','MarkerFaceColor','k','Linewidth',1,...
        'MarkerEdgeColor','k','MarkerSize',3);
end

legend('Analytical','Classical','Numerical')
